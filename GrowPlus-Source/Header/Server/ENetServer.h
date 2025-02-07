#pragma once

#include <x64-windows/include/enet/enet.h>
#include <cstdint>

#include <Header/Server/ENetHandler.h>

// Membuat enum erver type (Agar lebih readeable)
enum ServerType : bool {
	NORMAL = false,
	CREATIVE = true
};

class ENetServer {
public:
	ENetServer(uint8_t id, ServerType type, ENetHost* server, uint8_t maxPeers) : id(id), type(type), server(server), maxPeers(maxPeers) {
		this->pauseService = false;
		Utils::consoleLog(SUC, "[" + std::to_string(server->address.port) + "] ENetHost dibuat.");
	}
	~ENetServer() {
		delete this->server;
		Utils::consoleLog(SUC, "[" + std::to_string(server->address.port) + "] ENetHost dihapus.");
	}

	ServerType getType() const { return this->type; }	// Mendapatkan type (Const berarti gak bisa ngubah data typenya)
	uint16_t getID() const { return this->id; }			// Mendapatkan server ID
	ENetHost* getENet() const { return this->server; }	// Mendapatkan server
	uint8_t getMaxPeers() const { return this->maxPeers; }	// Method untuk mendapatkan max peers yang dapat terhubung ke server
	void setMaxPeers(uint8_t amount) { this->maxPeers = amount; }	// Method untuk mengatur kax peers (EXPERIMENTAL ONLY)

	void setPause(bool pause) { this->pauseService = pause; }	// Melakukan set pause service (Kalo true nanti servernya akan berhenti merespon server)
	bool isPaused() const { return this->pauseService; }	// Method untuk mengecek apakah server sedang di pause apa tidak

	void startService();	// Method untuk memulai server handler

	ENetHandler Handler(ENetPeer* peer, ENetEvent event) { return ENetHandler(this->server, peer, this->maxPeers, event); }
private:
	ENetHost* server;
	ServerType type;
	uint8_t id;
	bool pauseService;
	uint8_t maxPeers;
};