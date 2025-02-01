#pragma once

// Menghubungkan include yang diperlukan 

// Menghubungkan header lainnya
#include <Header/Player/PlayerStruct.h>
#include <Header/Player/RoleStruct.h>
#include <Header/Utils/Utils.h>
#include <Header/Packet/SendPacket.h>

class GrowPlayer {
public:
	GrowPlayer(ENetPeer* peer, std::string tankIDName, std::string tankIDPass) : peer(peer), Credentials{ tankIDName, tankIDPass } {
		Utils::consoleLog(INF, "Membuat objek player baru dengan growID " + this->Credentials.tankIDName);
	}
	~GrowPlayer() {}

	PlayerCredential getCredentials() { return this->Credentials; }		// Method untuk mendapatkan credentials player
	PlayerData getData() { return this->Data; }		// Method untuk mendapatkan data player
	PlayerRole getRole() const { return this->Role; }		// Method untuk mendapatkan role player
	std::unique_ptr<SendPacket_> SendPacket() { return std::make_unique<SendPacket_>(this->peer); }	// Method untuk mengirimkan template packet
private:
	ENetPeer* peer;
	PlayerCredential Credentials;
	PlayerData Data;
	PlayerRole Role;
};
//#define pInfo(peer) (static_cast<GrowPlayer*>(peer->data))