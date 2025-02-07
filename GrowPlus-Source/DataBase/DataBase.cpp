
// hubungkan headernya
#include "DataBase.h"

void DataBase::cleanUp() {
    if (Player) {
        delete Player;
        Player = nullptr;
    }
}