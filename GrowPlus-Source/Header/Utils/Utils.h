#pragma once

// Menghubungkan beberapa include yang dibutuhkan 
#include <iostream>	// untuk input output konsol
#include <string>	// Basic string
#include <stdexcept>	// Untuk handle error
#include <fstream>	// Membaca/Menulis file
#include <filesystem> // entahlah yang pasti buat baca file
#include <nlohmann/json.hpp>	// Json untuk cpp
#include <openssl/evp.h>
using json = nlohmann::json;

#include <Header/Utils/ConsoleOutput.h>		// Menghubungkan header cknsoleouput kesini
#include <Header/Utils/GenerateStruct.h>

// Default path
inline std::string _DATABASE = "database/", _DATA = "file/";

class Utils {
public:
	// Fungsi untuk hashing dengan SHA-256 menggunakan EVP
	static std::string hashWithSHA256(const std::string& data);
	static std::string readFile(const std::string path);	// Membaca dan mengembalikan isi file
	// method untuk membaca file json
	static json readJson(const std::string path);
	static void writeFile(const std::string path, const std::string& ctx);	// Menulis file
	static void writeJson(const std::string path, const json& ctx);	// Menulis file dengan format json
	static void consoleLog(const OutputType& type, const std::string ctx);	// Menampilkan text ke konsol
	static int generateID(GenerateType type);	// Generate id secara berurutan dimulai dari
	// Method untuk mengubah int dari 1000 menjadi 1,000
	static std::string parseMoney(int value);
	// Fungsi untuk memecah string menjadi array string
	static std::vector<std::string> explode(const std::string& delimiter, const std::string& str);
	// Mendapatkan waktu dalam detik sekarang sejak epoch
	static std::time_t getCurrentSec() {
		return std::chrono::duration_cast<std::chrono::seconds>(
			std::chrono::system_clock::now().time_since_epoch()
		).count();
	}
private:
};