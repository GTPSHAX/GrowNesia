
// Kita hubungkan headernya kesini
#include <Header/Server/ENetServer.h>
#include <Header/Utils/Utils.h>
#include <Header/Player/GrowPlayer.h>
#include <Header/Server/GrowServer.h>
#include <thread>
#include <chrono>

bool ENetHandler::EventConnect() {
	try
	{
		// Hindari overwrite tanpa delete sebelumnya
		if (this->peer->data != nullptr) {
			delete static_cast<GrowPlayer*>(this->peer->data);
		}

		this->peer->data = new GrowPlayer(this->peer, "", "");
		GrowPlayer* pInfo = (GrowPlayer*)this->peer->data;

		// ini akan dieksekusi kalo server full
		if (this->server->connectedPeers >= this->getMaxPeers()) {
			pInfo->SendPacket()->OnConsoleMessage("`4SERVER OVERLOADED`o: Sorry, our servers are currently at max capacity with " + Utils::parseMoney(GrowServer::getData().onlinePeer) + " Online, please try later. We are working to improve this!");
			return false;
		}

		pInfo->SendPacket()->OnConsoleMessage("Connected to `wGrowServer ID``, Please wait!");
		Utils::consoleLog(INF, std::to_string(this->server->connectedPeers));
		return true;
	}
	catch (const std::exception& e)
	{
		Utils::consoleLog(ERR, std::string("Error pada handler connect: ") + e.what());
	}
	catch (...) {}
	
	return false;
}