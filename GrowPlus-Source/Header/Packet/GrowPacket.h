#pragma once

// Kita include header yang dibutuhkan
#include <Header/Packet/VariantList.h>
#include <Header/Packet/PacketEnum.h>

class GrowPacket {
public:
	static char* parseText(ENetPacket* packet);
	static int GetMessageTypeFromPacket(ENetPacket* packet);
private:
};
