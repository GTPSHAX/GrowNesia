
// Kita include headernya kesini
#include <string>
#include <Header/Packet/GrowPacket.h>
#include <Header/Handler/PacketMessageHandler.h>
#include <Header/Utils/Utils.h>
#include <Header/Player/GrowPlayer.h>
#include <SDK/Proton/TextScanner.h>
#include <DataBase/DataBase.h>
#include <Header/Server/GrowServer.h>

bool PacketMessageHandler::GenericText() {
	// Kita ubah packet menjadi text
	std::string cch = GrowPacket::parseText(this->event.packet);

	// definisikan pInfo disini sebagai pointer
	GrowPlayer* pInfo = (GrowPlayer*)this->peer->data;

	// logs cch disini
	Utils::consoleLog(DBG, "Packet receive from " + std::string(pInfo->getData().type == AccountType::GUEST ? "Guest account" : pInfo->getCredentials().tankIDName) + ":\n" + cch);

	try
	{
		// kita kasih cek apakah ini merupakan akun guest (ditandai dengan uid yang invalid/0)
		if (pInfo->getCredentials().UID == 0) {
			// kita akan menangani semua packet yang diperlukan oleh guest account

			// kita gunakan textscanner karena lebih efisien dibandingkan dengan explode
			TextScanner pkt(cch.c_str());

			// mengambil growid dan password
			pInfo->setCredentials().tankIDName = pkt.GetParmString("tankIDName", 1);
			pInfo->setCredentials().tankIDPass = pkt.GetParmString("tankIDPass", 1);

			// redirect jika subserver penuh
			// cari id server ini dulu
			uint8_t currentID = 0;
			// loop ke semua server yang ada
			for (auto* server : GrowServer::getServers()) {
				// kita hiraukan server saat ini
				if (server->getENet() == this->server) {
					currentID = server->getID();
					break;	// mengakhiri loopnya
				}
			}

			// mengambil server saat ini
			ENetServer* currentServer = GrowServer::getServerByID(currentID);

			// cek apakah server ini penuh
			if (this->server->connectedPeers >= currentServer->getMaxPeers() || currentID == 1) {
				for (auto* server : GrowServer::getServers()) {
					// kita hiraukan server saat ini
					if (server->getENet() == this->server) {
						continue;
					}

					// Cek apakah server lain masih memiliki slot kosong
					if (server->getENet()->connectedPeers < server->getMaxPeers()) {
						// Kirim pesan ke pemain bahwa server penuh dan redirect ke server lain
						pInfo->SendPacket()->OnConsoleMessage("`4FULL OF CAPACITY`o: Sorry, server with ID `w" + std::to_string(currentID) + " ``has reached player limit, redirect to another server...");
						pInfo->SendPacket()->OnSendToServer(server->getENet(), eLoginMode::CLIENT_LOGIN, pInfo->getCredentials().tankIDName);
						pInfo->SendPacket()->SetHasGrowID(pInfo->getCredentials());
						return false;
					}
				}

				// Jika semua server penuh, kirim pesan bahwa server sedang overload
				pInfo->SendPacket()->OnConsoleMessage("`4SERVER OVERLOADED`o: Sorry, our servers are currently at max capacity with " + Utils::parseMoney(GrowServer::getData().onlinePeer) + " Online, please try later. We are working to improve this!");
				return false;
			}

			// regis atau login?
			if (pInfo->getCredentials().tankIDName.empty()) {
				// jika tidaka da data player pada database, maka akan dianggap sebagai akun guest
				pInfo->setData().type = AccountType::GUEST;
				pInfo->setCredentials().UID = Utils::generateID(ACCOUNT);
			}
			else {
				// jika ada, kita load datanya
				std::cout << int(DB.Player->checkCredentials(pInfo)) << std::endl;
				switch (int(DB.Player->checkCredentials(pInfo))) {
					case 1: {
						pInfo->SendPacket()->OnConsoleMessage("`4Success``: Account found. Please wait!");
						pInfo->setData().type = AccountType::NOT_GUEST;
						break;
					}
					case 0: {
						pInfo->SendPacket()->OnConsoleMessage("`4Failed``: Error while loading data. Please report to `#Admin servers``!");
						return false;
					}
					case -1: {
						pInfo->SendPacket()->OnConsoleMessage("`4Failed``: Account not found. Login as guest, please wait!");
						pInfo->setData().type = AccountType::GUEST;
						pInfo->setCredentials().UID = Utils::generateID(ACCOUNT);
						break;
					}
					case -2: {
						pInfo->SendPacket()->OnConsoleMessage("`4Failed``: Credentials not found.");
						return false;
					}
					case -3: {
						pInfo->SendPacket()->OnConsoleMessage("`4Failed``: Mismatch in GrowID or Password.");
						return false;
					}
					default: {
						pInfo->SendPacket()->OnConsoleMessage("`4Failed``: Unknown.");
						return false;
					}
				}
			}

			// kita simpan sesi akun
			pInfo->SendPacket()->SetHasGrowID(pInfo->getCredentials());
		}
		else {
			// kalau disini untuk menghandle semua packet yang dikirim sama player (bukan akun guest)
		}
	}
	catch (const std::exception& e)
	{
		Utils::consoleLog(ERR, (std::string)"Error while handling generic text: " + e.what());
	}

	return false;
}