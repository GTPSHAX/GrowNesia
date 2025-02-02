// Kita include header yang dibutuhkan 
#include <Header/Server/ENetServer.h>
#include <Header/Utils/Utils.h>
#include <Header/Player/GrowPlayer.h>
#include <Header/Server/GrowServer.h>
#include <thread>
#include <chrono>

// Disini kita akan membuat controller untuk menghandler 3 jenis packet event, diantaranya adalah connect, packet rechive, dan disconnect
void ENetServer::startService() {
	// Kita akan menjalankan server secara thread (terpisah dengan aplikasi utama)
	std::thread([this]() {
		// Kita akan buat variabel untuk eventnya terlebih dahulu 
		ENetEvent event;
		while (true) {
			try {
				if (this->isPaused()) {
					continue;
				}
				if (this->getENet() == nullptr) {
					return Utils::consoleLog(ERR, "ENetHost dengan id " + std::to_string(this->getID()) + " mengalami crash!");
				}
				if (enet_host_service(this->server, &event, 1000) > 0) {
					// Nah disini kita akan buat sebuah peer atau koneksi playernya
					ENetPeer* peer = event.peer;

					if (!peer || !this->server || peer == nullptr || this->server == nullptr) {
						return; // Hindari crash jika peer atau server null
					}

					// Kita akan menggunakan switch karena pada kondisi seperti ini switch lebih baik dibandingkan dengan if else
					// Cara kerja switch: switch akan membandingkan hasil key dengan semua case yang ada, jika ada case yang cocok maka akan langsung locat dan mengeksekusi kode tersebut
					switch (event.type) {
					case ENET_EVENT_TYPE_CONNECT: {	// Disini adalah tempat dimana enet menghandle peer yang connect ke servernya
						// Disetiap case akan kita berikan sebuat try catch untuk menangani error yang dikembalikan, sehingga tidak menyebabkan program berhenti 
						try
						{
							/*peer->data = new GrowPlayer(peer, "", "");
							pInfo(peer)->SendPacket()->OnConsoleMessage("Connected to `wGrowServer ID``, Please wait!");
							Utils::consoleLog(INF, std::to_string(this->getENet()->connectedPeers));*/
							GrowServer::getData().onlinePeer++;
							if (!this->Handler(peer).EventConnect()) {
								enet_peer_disconnect_later(peer, 0);
							}
						}
						catch (const std::exception& e)
						{
							Utils::consoleLog(SUC, std::string("[" + std::to_string(this->getID()) + "] Error packet case connect: ") + e.what());
						}
						catch (...)
						{
							Utils::consoleLog(SUC, std::string("[" + std::to_string(this->getID()) + "] Error packet case connect: Tidak diketahui "));
						}
						break;
					}
					case ENET_EVENT_TYPE_RECEIVE: {	// Disini adalah tempat dimana enet menghandle packet yang dikirim oleh peer

						enet_packet_destroy(event.packet);	// Menghancurkan packet yang diterima 
						break;
					}
					case ENET_EVENT_TYPE_DISCONNECT: {	// Disini adalah tempat dimana enet menghandle peer yang terputus dari server
						Utils::consoleLog(WRN, "Player disconnected!");
						GrowServer::getData().onlinePeer--;
						peer->data = NULL;
						delete peer->data;
						break;
					}
					}
				}
			}
			catch (...) {
				Utils::consoleLog(ERR, "[" + std::to_string(this->getENet()->address.port) + "] Has crashed! (Handled)");
			}
		}
	}).detach();
}

//void ENetHandler::EventConnect() {
//	this->peer->data = new GrowPlayer(peer, "", "");
//	std::thread([this]() {
//		while (true) {
//			//std::this_thread::sleep_for(std::chrono::mi(2));
//			pInfo(peer)->SendPacket()->OnConsoleMessage("Connected to `wGrowServer ID``, Please wait!");
//			Utils::consoleLog(INF, std::to_string(this->server->connectedPeers));
//		}
//		}).detach();
//}