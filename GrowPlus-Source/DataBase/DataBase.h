#pragma once

// hubungkan header yang diperlukan
#include <string>
#include <filesystem>

#include <Header/Utils/Utils.h>

#include "PlayerData.h"

// Class untuk menangani semua database
class DataBase {
public:
    // Constructor
    DataBase(const std::string& salt) : hashSalt(salt) {
        Utils::consoleLog(SUC, "DataBase telah dibuat!");
        Player = new PlayerDB(hashSalt);
    }

    // Destructor untuk membebaskan memori
    ~DataBase() {
        cleanUp();
    }

    // method untuk membebaskan memory
    void cleanUp();

    // kita buat objek untuk database player
    PlayerDB* Player;
private:
    
    // tambhkan salt untuk keamanan server (hati hati jika ingin menganti hash)
    std::string hashSalt = "";
};

static DataBase DB("");