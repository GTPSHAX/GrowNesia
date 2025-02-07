#pragma once

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
	bool checkCredentials(const std::string& growid, const std::string& password) {
		// pastikan growid dan password memiliki nilai
		if (growid.empty() || password.empty()) {
			return false;
		}

		// cek apakah path ditemukan
		if (!isExist(growid + "/credentials.json")) {
			return false;
		}

		try
		{
			json pData = Utils::readJson(_DATABASE + "players/" + growid + "/credentials.json");

			// cek apakah passwordnya sama
			if (Utils::hashWithSHA256(password) == pData["password"] && hashSalt == pData["salt"]) {
				// disini kita akan mengembalikan true karena passwordnya benar
				return true;
			}
		}
		catch (const std::exception& e)
		{
			Utils::consoleLog(ERR, "Error while checking credentials for " + growid + ": " + e.what());
		}

		// kembalikan false karena semua statement salah
		return false;
	}
private:
	std::string hashSalt;
};