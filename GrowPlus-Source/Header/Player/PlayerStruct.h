#pragma once

// Ini sepertinya bot farming kayak cps
struct FarmingSlave {
	int id = 0, x = 0, y = 0, plantingTree = 0, stock = 0, hand, active = 0, magnetron_x, magnetron_y, magnetron_id;
	int32_t netID;
	std::string owner, worlds;
};
// Kita sebisa mungkin menghindari jenis variabel yang memakan memory cukup besar pada peer player
struct {
	std::vector<std::pair<int, long long int>> data = {};
	std::pair<int, long long int> getByID(int id) {
		for (auto& slave : data) {
			if (slave.first == id) {
				return slave;
			}
		}
	}
} GlobalFarmingSlave;

struct PlayerCredential {	// Struct terpisah yang hanya berisi credentials player
	std::string tankIDName = "",	// Bisa disebut juga sebagai growid
		tankIDPass = "";	// Password dengan hash
	int UID = 0;	// Nomor unik yang berbeda pada setiap player
};

struct PlayerData {
	FarmingSlave Slave;
};