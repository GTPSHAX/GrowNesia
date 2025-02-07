#pragma once

#include <Header/Utils/Utils.h>
class ENetHandler {
public:
	ENetHandler(ENetHost* server, ENetPeer* peer, uint8_t maxPeers, ENetEvent event) : server(server), peer(peer), maxPeers(maxPeers), event(event) {
		Utils::consoleLog(SUC, "[" + std::to_string(this->server->address.port) + "] Starting ENetHandler...");
	}
	~ENetHandler() {
		Utils::consoleLog(SUC, "[" + std::to_string(this->server->address.port) + "] Stopped ENetHandler.");
	}

	bool EventConnect();	// Handler even conect
	bool EventReceive();	// Handler event packet receive 

	uint8_t getMaxPeers() const { return this->maxPeers; }	// Method untuk mendapatkan max peers yang dapat terhubung ke server
private:
	ENetHost* server;
	ENetPeer* peer;
	ENetEvent event;
	uint8_t maxPeers;
};