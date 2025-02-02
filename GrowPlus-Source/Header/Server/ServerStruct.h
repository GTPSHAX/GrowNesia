#pragma once

struct ServerData {
	// Semua jumlah online peer akan disimpan disini
	int onlinePeer = 0;

	long long upTime = 0;
};

struct BASE_PORT {
	uint16_t normal, creative;
};

struct SUBSERVER_DATA {
	uint8_t amount;
	int maxConnectedPeer;
};