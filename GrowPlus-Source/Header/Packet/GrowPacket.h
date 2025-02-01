#pragma once

// Kita include header yang dibutuhkan
#include <Header/Packet/GamePacket.h>

class GrowPacket {
public:
	static char* parseText(ENetPacket* packet);
private:
};
