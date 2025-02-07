
// Kita include headernya kesini
#include <string>
#include <Header/Packet/GrowPacket.h>
#include <Header/Handler/PacketMessageHandler.h>
#include <Header/Utils/Utils.h>
#include <Header/Player/GrowPlayer.h>
#include <SDK/Proton/TextScanner.h>
#include <DataBase/DataBase.h>

bool PacketMessageHandler::GenericText() {
	// Kita ubah packet menjadi text
	std::string cch = GrowPacket::parseText(this->event.packet);

	// definisikan pInfo disini sebagai pointer
	GrowPlayer* pInfo = (GrowPlayer*)this->peer->data;

	try
	{
		// kita kasih cek apakah ini merupakan akun guest (ditandai dengan uid yang invalid/0)
		if (pInfo->getCredentials().UID == 0) {
			// kita akan menangani semua packet yang diperlukan oleh guest account

			// kita gunakan textscanner karena lebih efisien dibandingkan dengan explode
			TextScanner pkt(cch.c_str());
			if (!DB.Player->isExist(pkt.GetParmString("tankIDName", 1))) {
				pInfo->setData().type;
			}
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