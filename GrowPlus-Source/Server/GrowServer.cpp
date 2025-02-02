#include <Header/Server/GrowServer.h>	// Menghubungkan header daru GrowServer

// Pertama kita perlu mendeklarasikan variabel pada GrowServer
std::vector<ENetServer*> GrowServer::servers = {};
ServerData GrowServer::Data;

// Disini kita akan membuat kode c++/cpp untuk setiap method yang telah didefinisikan pada header GrowServer
void GrowServer::init(const uint16_t& port, const ServerType& type, const uint8_t maxPeers) {
	// Fungsi ini digunakan untuk melakukan inialisasi/membuat enet host yang kita perlukan
	if (enet_initialize() != 0) {
		return Utils::consoleLog(ERR, "Error tidak diketahui saat menginialisasikan ENet.");
	}
	atexit(enet_deinitialize);  // Menjalankan enet_deinitialize saat program berakhir

	// Membuat server
	ENetAddress address;
	address.host = ENET_HOST_ANY;	// Kita akan menggunakan semua host yang tersedia (0.0.0.0)
	address.port = port;	// Kita akan menggunakan id sebagai portnya
	ENetHost* server = enet_host_create(&address, maxPeers + 10, 10, 0, 0); // Membuat server dengan alamat dan port yang sudah di inisialisasi
	if (server == NULL) {
		return Utils::consoleLog(ERR, "Gagal membuat ENetHost dengan port " + std::to_string(address.port));  // Jika gagal membuat server
	}

	// Opsional: checksum untuk verifikasi paket
	server->checksum = enet_crc32;

	// Kompresi opsional
	enet_host_compress_with_range_coder(server);

	// Membuat dan menambahkan server ke dalam vector servers
	ENetServer* growtopiaServer = new ENetServer(servers.size() + 1, type, server, maxPeers);
	servers.push_back(growtopiaServer);
}
ENetServer* GrowServer::getServerByID(uint16_t id) {
	for (ENetServer* server : servers) {
		if (server->getID() == id) {
			return server;
		}
	}
	return nullptr;
}