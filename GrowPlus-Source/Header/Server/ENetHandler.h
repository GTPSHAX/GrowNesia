#pragma once

#include <Header/Utils/Utils.h>
class ENetHandler {
public:
	ENetHandler(ENetHost* server, ENetPeer* peer, uint8_t maxPeers) : server(server), peer(peer), maxPeers(maxPeers) {
		Utils::consoleLog(SUC, "[" + std::to_string(this->server->address.port) + "] Starting ENetHandler...");
	}
	~ENetHandler() {
		Utils::consoleLog(SUC, "[" + std::to_string(this->server->address.port) + "] Stopped ENetHandler.");
	}

	bool EventConnect();	// Handler even conect

	uint8_t getMaxPeers() const { return this->maxPeers; }	// Method untuk mendapatkan max peers yang dapat terhubung ke server
private:
	ENetHost* server;
	ENetPeer* peer;
	uint8_t maxPeers;
};