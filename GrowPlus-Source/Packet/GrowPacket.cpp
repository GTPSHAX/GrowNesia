// Kita hubungkan heaser yang diperlukan 
#include <Header/Packet/GrowPacket.h>
#include <Header/Utils/Utils.h>

char* GrowPacket::parseText(ENetPacket* packet) {
	char zero = 0;
	memcpy(packet->data + packet->dataLength - 1, &zero, 1);
	return (char*)(packet->data + 4);
}
int GrowPacket::GetMessageTypeFromPacket(ENetPacket* packet) {
	int result;
	if (packet->dataLength > 3u) {
		result = *(packet->data);
	}
	else {
		Utils::consoleLog(WRN, "Bad packet length, ignoring message.");
		result = 0;
	}
	return result;
}