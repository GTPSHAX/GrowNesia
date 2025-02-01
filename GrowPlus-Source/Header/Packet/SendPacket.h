#pragma once

// Kita include header yang dibutuhkan
#include <Header/Packet/GamePacket.h>

class SendPacket_ {
public:
	SendPacket_(ENetPeer* peer) {
		this->peer = peer;
		std::cout << "Created new packet" << std::endl;
	}
	~SendPacket_() {
		std::cout << "Packet sended!" << std::endl;
	}

	// Mengirimkan pesan ke konsol player
	void OnConsoleMessage(const std::string ctx) {
		GamePacket p;
		p.Insert("OnConsoleMessage");
		p.Insert(ctx);
		p.CreatePacket(this->peer);
	}
private:
	ENetPeer* peer;
};