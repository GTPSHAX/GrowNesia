// Kita hubungkan heaser yang diperlukan 
#include <Header/Packet/GrowPacket.h>

char* GrowPacket::parseText(ENetPacket* packet) {
	char zero = 0;
	memcpy(packet->data + packet->dataLength - 1, &zero, 1);
	return (char*)(packet->data + 4);
}