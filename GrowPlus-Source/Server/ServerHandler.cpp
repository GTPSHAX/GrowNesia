
// Kita hubungkan headernya kesini
#include <Header/Server/ENetServer.h>
#include <Header/Utils/Utils.h>
#include <Header/Player/GrowPlayer.h>
#include <Header/Server/GrowServer.h>
#include <Header/Packet/GrowPacket.h>
#include <Header/Handler/PacketMessageHandler.h>
#include <thread>
#include <chrono>
#include <ws2ipdef.h>

bool ENetHandler::EventConnect() {
	try
	{
		// Hindari overwrite tanpa delete sebelumnya
		// Cek apakah data peer sudah ada, jika ada, hapus terlebih dahulu untuk menghindari memory leak
		if (this->peer->data != nullptr) {
			delete static_cast<GrowPlayer*>(this->peer->data);
		}

		// Buat objek GrowPlayer baru dan simpan ke dalam data peer
		this->peer->data = new GrowPlayer(this->peer, "", "");
		GrowPlayer* pInfo = (GrowPlayer*)this->peer->data;

		// Mendapatkan dynamic IP peer
		char clientConnection[INET6_ADDRSTRLEN]; // Buffer yang lebih besar untuk menampung alamat IP
		int result = enet_address_get_host_ip(&this->peer->address, clientConnection, sizeof(clientConnection));

		if (result < 0) {
			// Jika gagal mendapatkan alamat IP, log pesan error
			Utils::consoleLog(ERR, "Failed to get peer IP address.");
		}
		else {
			// Simpan alamat IP ke dalam data pemain
			pInfo->setData().dynamicIP = clientConnection; // Pastikan dynamicIP adalah std::string
			Utils::consoleLog(INF, "Peer connected with IP: " + pInfo->getData().dynamicIP);
		}

		// Ini aku juga gak tau buat apa
		pInfo->SendPacket()->send_(1, nullptr, 0);

		// kita simpan id server saat ini
		uint16_t currentID = 0;
		// Loop melalui semua server yang tersedia
		for (auto* server : GrowServer::getServers()) {
			// daoatkan server saat ini
			if (server->getENet() == this->server) {
				currentID = server->getID();
				break;
			}
		}

		// Beritahu player bahwa dia sudah terhubung ke server
		pInfo->SendPacket()->OnConsoleMessage("Connected to `w" + GrowServer::Config.getServerName() + "``, standing on server: `w" + std::to_string(currentID) + "``. Please wait!");

		// Ambil round trip time (ping) dalam milidetik
		int ping_ms = peer->roundTripTime;

		// Ambil estimasi packet loss dalam persen
		float packet_loss_percent = (peer->packetLoss / 100.0f); // ENet menyimpan nilai dalam basis 1/100

		std::ostringstream stream; // Membuat stream untuk mengonversi angka menjadi string
		stream << std::fixed << std::setprecision(2) << packet_loss_percent; // Mengatur format angka dengan 2 angka desimal
		std::string packet_loss_str = stream.str(); // Menyimpan hasil konversi dalam bentuk string

		pInfo->SendPacket()->OnConsoleMessage("Your ping: `w" + std::to_string(ping_ms) + "ms ``[`#" + packet_loss_str + "% ``Packet loss]");
		return true;
	}
	catch (const std::exception& e)
	{
		// Tangkap dan log error yang terjadi selama proses koneksi
		Utils::consoleLog(ERR, std::string("Error pada handler connect: ") + e.what());
	}
	catch (...)
	{
		// Tangkap error yang tidak diketahui jenisnya
	}

	// Jika terjadi error, kembalikan false
	return false;
}
bool ENetHandler::EventReceive() {
	// Kita cek apakah peer ini valid atau tidak
	if (this->peer->data == NULL || this->peer->state != ENET_PEER_STATE_CONNECTED) {
		Utils::consoleLog(SEC, "Invalid peer detected, disconnecting now.");
		return false;
	}

	// Kita cek apakah panjang dari packet yabg dikirim valid
	if (this->peer->incomingDataTotal >= 25000) {
		Utils::consoleLog(SEC, "Suspicious packet receive, disconnecting now.");
		return false;
	}
	if (event.packet->dataLength < 0x4 || event.packet->dataLength > 0x400) {
		return true;	// tidak di handle
	}

	// kita definisikan pInfo agar mudah diakses
	GrowPlayer* pInfo = (GrowPlayer*)this->peer->data;

	// Kita tambahkan pembatas pengiriman paket (packet send limiter) untuk client
	// Ini berguna agar client tidak mengirim terlalu banyak paket dalam waktu singkat

	// Jika waktu saat ini sudah lebih dari 1 detik sejak pengiriman terakhir
	if (pInfo->getData().currentSec + 1000 < Utils::getCurrentSec()) {
		// Reset jumlah paket yang dikirim oleh client
		pInfo->setData().allPacketSended = 0;
		// Perbarui waktu terakhir pengiriman paket
		pInfo->setData().currentSec = Utils::getCurrentSec();
	}
	else {
		// Jika jumlah paket yang dikirim melebihi batas (560 paket per detik)
		if (pInfo->getData().allPacketSended >= 560) {
			// Kirim pesan ke client bahwa mereka mengirim terlalu banyak paket dan akan terputus dari server
			pInfo->SendPacket()->OnConsoleMessage("Too many packets sent at once. Disconnecting from the server!");
			return false; // Kembalikan false untuk menghentikan proses lebih lanjut
		}
		else {
			// Jika belum mencapai batas, tambahkan jumlah paket yang dikirim
			pInfo->setData().allPacketSended++;
		}
	}

	PacketMessageHandler* pHandler = new PacketMessageHandler(this->server, this->event, this->peer);
	bool returned = false;
	switch (GrowPacket::GetMessageTypeFromPacket(event.packet)) {
		case NET_MESSAGE_GENERIC_TEXT: {
			returned = pHandler->GenericText();
		}
		case NET_MESSAGE_GAME_MESSAGE: {

		}
		case NET_MESSAGE_GAME_PACKET: {

		}
		default: {
			Utils::consoleLog(WRN, "Unknown packet message type!");
			break;
		}
	}

	delete pHandler;
	return returned;
}