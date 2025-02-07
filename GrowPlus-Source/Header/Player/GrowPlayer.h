#pragma once

// Menghubungkan include yang diperlukan 

// Menghubungkan header lainnya
#include <Header/Player/PlayerStruct.h>
#include <Header/Player/RoleStruct.h>
#include <Header/Utils/Utils.h>
#include <Header/Packet/GamePacket.h>

class GrowPlayer {
public:
	GrowPlayer(ENetPeer* peer, std::string tankIDName, std::string tankIDPass) : peer(peer), Credentials{ tankIDName, tankIDPass } {
		Utils::consoleLog(INF, "Membuat objek player baru dengan growID " + this->Credentials.tankIDName);
	}
	~GrowPlayer() {}

	// method untuk Mendapatkan data credentials (privasi)
	PlayerCredential getCredentials() const { return this->Credentials; }
	// method untuk mengatur data credentials
	PlayerCredential& setCredentials() { return this->Credentials; }

	PlayerRole getRole() const { return this->Role; }		// Method untuk mendapatkan role player
	std::unique_ptr<GamePacket> SendPacket() { return std::make_unique<GamePacket>(this->peer); }	// Method untuk mengirimkan template packet

	// Method untuk mendapatkan data player
	PlayerData getData() const {
		return this->Data;
	}
	// Method untuk mengupdate Data player
	PlayerData& setData() {
		return this->Data;
	}
private:
	ENetPeer* peer;
	PlayerCredential Credentials;
	PlayerData Data;
	PlayerRole Role;
};
//#define pInfo(peer) (static_cast<GrowPlayer*>(peer->data))