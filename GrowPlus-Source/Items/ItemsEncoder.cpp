// Kita include apa yang kita butuhkan
#include <string>

// Kita lerlu menghubungkan header agar cpp ini di compile oleh compiler
#include <Header/Utils/Utils.h>
#include <Header/Items/GrowItems.h>

void GrowItems::encodeDat(const std::string path) {
	// Command to execute
	std::string command = "node ./" + _DATA + "items/encoder/app.js " + path;

	// Execute the command
	int result = std::system(command.c_str());

	// Check the result
	if (result != 0) {
		throw std::runtime_error("Faild to encode the file");
	}
}