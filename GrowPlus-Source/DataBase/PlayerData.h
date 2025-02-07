#pragma once

// menghubungkan jeaser yang dibutuhkan 
#include <Header/Player/GrowPlayer.h>

// class untuk menangani semua database player
class PlayerDB {
public:
	// kita buat construct untuk mengisi beberapa data
	PlayerDB(const std::string& hashSalt) : hashSalt(hashSalt) {}

	// method untuk mengecek apakah lokasi file ada/ditemukan
	bool isExist(const std::string& path) {
		return std::filesystem::exists(_DATABASE + "/players/" + path);
	}

	// method untuk mengecek apakah credentials cocok dengan yang ada pada databse
	int checkCredentials(GrowPlayer* pInfo) {
		// pastikan growid dan password memiliki nilai
		if (!isExist(pInfo->getCredentials().tankIDName + "/")) {
			// database not found
			return -1;
		}

		// cek apakah path ditemukan
		if (!isExist(pInfo->getCredentials().tankIDName + "/credentials.json")) {
			// credentials not found
			return -2;
		}

		try
		{
			json pData = Utils::readJson(_DATABASE + "players/" + pInfo->getCredentials().tankIDName + "/credentials.json");

			// cek apakah passwordnya sama
			if (Utils::hashWithSHA256(pInfo->getCredentials().tankIDPass ) == pData["password"] && hashSalt == pData["salt"]) {
				// disini kita akan mengembalikan true karena passwordnya benar
				return 1;
			}
		}
		catch (const std::exception& e)
		{
			Utils::consoleLog(ERR, "Error while checking credentials for " + pInfo->getCredentials().tankIDName + ": " + e.what());
			// error
			return 0;
		}

		// kembalikan false karena semua statement salah
		return -3;
	}
private:
	std::string hashSalt;
};