#pragma once

// Kita include header yang dibutuhkan
#include <Header/Packet/VariantList.h>

enum class eLoginMode : uint8_t {
	CLIENT_LOGIN = 0,
	REDIRECT_LOGIN = 1,
	REDIRECT_SUBSERVER = 2,
	REDIRECT_SUBSERVER_SILENT = 3,
};

class GamePacket {
public:
	GamePacket(ENetPeer* peer) {
		this->peer = peer;
		std::cout << "Created new packet" << std::endl;
	}
	~GamePacket() {
		std::cout << "Packet sended!" << std::endl;
	}

	// Mengirimkan pesan ke konsol player
	void OnConsoleMessage(const std::string ctx) {
		VariantList p;
		p.Insert("OnConsoleMessage");
		p.Insert(ctx);
		p.CreatePacket(this->peer);
	}
	// mengirim player ke server lain
	void OnSendToServer(ENetHost* target, const eLoginMode& lmode, const std::string& tankIDName) {
		VariantList p;
		p.Insert("OnSendToServer");
		p.Insert(target->address.port);
		p.Insert("GrowNesia");
		p.Insert(this->peer->connectID);
		p.Insert(target->address.host + "|0|-1");
		p.Insert(uint8_t(lmode));
		p.Insert(tankIDName);
		p.CreatePacket(this->peer);
	}
	// Sumpah ini aku juga gak tau sebenarnya buat apa
	void send_(int num, char* data, const int len) {
		const auto packet = enet_packet_create(nullptr, len + 5, ENET_PACKET_FLAG_RELIABLE);
		memcpy(packet->data, &num, 4);
		if (data != nullptr) {
			memcpy(packet->data + 2, data, len);
		}
		char zero = 0;
		memcpy(packet->data + 2 + len, &zero, 1);
		enet_peer_send(this->peer, 0, packet);
	}
private:
	ENetPeer* peer;
};