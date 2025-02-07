// Menghubungkan semua header file
#include <Header/Utils/Utils.h>
#include <Header/Server/GrowServer.h>
#include <Header/Player/GrowPlayer.h>
#include <Header/Packet/GrowPacket.h>
#include <Header/Items/GrowItems.h>

#include <iomanip>
#include <sstream>
#include <thread>

BOOL WINAPI ConsoleHandler(DWORD dwType) {
	switch (dwType) {
	case CTRL_LOGOFF_EVENT: case CTRL_SHUTDOWN_EVENT: case CTRL_CLOSE_EVENT:
	{
		Utils::consoleLog(INF, "Saving and closing application...");
		std::cin.get();

		//server_::save::trigger(false);
		return TRUE;
	}
	default:
	{
		break;
	}
	}
	return FALSE;
}

int main(int argc, char* argv[]) {
	std::system("TITLE GrowServer ~ ENetHost untuk Growtopia Private Server");

	BOOL ret = SetConsoleCtrlHandler(ConsoleHandler, TRUE);
	srand(unsigned int(time(nullptr)));
	GrowServer::getData().upTime = time(nullptr);

	Utils::consoleLog(INF, "Source ini dibuat oleh GrowPlus Team dan dibantu oleh teman-teman lainnya.");
	Utils::consoleLog(INF, "Ini merupakan pembuatan ulang dari source GT3 oleh Sebia, kemudian disempurnakan oleh Tianvan, dan dikembangkan oleh KingPS Developer Team.");
	std::cout << std::endl;

	try
	{
		// Kita muat semua data yang diperlukan 
		Utils::consoleLog(INF, "Memuat data...");
		GrowServer::Config.loadConfig();
		GrowItems::encodeDat(_DATA + "items/items.json");

		// Inisialisasi enet host
		Utils::consoleLog(INF, "Inialisasi ENetHost...");
		for (uint16_t i = 0; i < GrowServer::Config.getSubServerData().amount; i++) {
			GrowServer::init(GrowServer::Config.getBasePort().normal + i, NORMAL, GrowServer::Config.getSubServerData().maxConnectedPeer);
		}
		if (GrowServer::Config.getBasePort().creative > 0) {
			for (uint16_t i = 0; i < GrowServer::Config.getSubServerData().amount; i++) {
				GrowServer::init(GrowServer::Config.getBasePort().creative + i, CREATIVE, GrowServer::Config.getSubServerData().maxConnectedPeer);
			}
		}

		// Memulai enet server
		// Kita loop setiap server yang terdaftar pada growserver
		for (auto* server : GrowServer::getServers()) {
			server->startService();
		}

		// Menjaga agar program utama tidak berhenti 
		while (true) {
			std::this_thread::sleep_for(std::chrono::seconds(1));
		}
	}
	catch (const std::exception& e)
	{
		std::cout << e.what();
	}
	std::cin.get();
	return 0;
}