// Menghubungkan semua header file
#include <Header/Utils/Utils.h>
#include <Header/Server/GrowServer.h>
#include <Header/Player/GrowPlayer.h>
#include <Header/Packet/GrowPacket.h>
#include <Header/Items/GrowItems.h>

#include <openssl/evp.h>
#include <iomanip>
#include <sstream>
#include <thread>

// Fungsi untuk hashing dengan SHA-256 menggunakan EVP
std::string hashWithSHA256(const std::string& data) {
	EVP_MD_CTX* context = EVP_MD_CTX_new(); // Alokasikan konteks hash
	const EVP_MD* md = EVP_sha256();       // Pilih algoritma SHA-256
	unsigned char hash[EVP_MAX_MD_SIZE];
	unsigned int hashLength = 0;

	if (EVP_DigestInit_ex(context, md, nullptr) != 1) {
		EVP_MD_CTX_free(context);
		throw std::runtime_error("Failed to initialize digest context");
	}

	if (EVP_DigestUpdate(context, data.c_str(), data.size()) != 1) {
		EVP_MD_CTX_free(context);
		throw std::runtime_error("Failed to update digest");
	}

	if (EVP_DigestFinal_ex(context, hash, &hashLength) != 1) {
		EVP_MD_CTX_free(context);
		throw std::runtime_error("Failed to finalize digest");
	}

	EVP_MD_CTX_free(context); // Bebaskan memori konteks

	// Ubah hasil hash menjadi string heksadesimal
	std::stringstream ss;
	for (unsigned int i = 0; i < hashLength; ++i) {
		ss << std::hex << std::setw(2) << std::setfill('0') << (int)hash[i];
	}

	return ss.str();
}

BOOL WINAPI ConsoleHandler(DWORD dwType) {
	switch (dwType) {
	case CTRL_LOGOFF_EVENT: case CTRL_SHUTDOWN_EVENT: case CTRL_CLOSE_EVENT:
	{
		Utils::consoleLog(INF, "Saving and closing application...");
		std::cin.get();

		//server_::save::trigger(false);
		return TRUE;
	}
	default:
	{
		break;
	}
	}
	return FALSE;
}

int main(int argc, char* argv[]) {
	std::system("TITLE GrowServer ~ ENetHost untuk Growtopia Private Server");

	BOOL ret = SetConsoleCtrlHandler(ConsoleHandler, TRUE);
	srand(unsigned int(time(nullptr)));
	GrowServer::getData().upTime = time(nullptr);

	Utils::consoleLog(INF, "Source ini dibuat oleh GrowPlus Team dan dibantu oleh teman-teman lainnya.");
	Utils::consoleLog(INF, "Ini merupakan pembuatan ulang dari source GT3 oleh Sebia, kemudian disempurnakan oleh Tianvan, dan dikembangkan oleh KingPS Developer Team.");
	std::cout << std::endl;

	try
	{
		/*std::string data;
		std::cout << "Enter data to hash: ";
		std::cin >> data;

		try {
			std::string hash = hashWithSHA256(data);
			std::cout << "SHA-256 Hash: " << hash << std::endl;
		}
		catch (const std::exception& e) {
			std::cerr << "Error: " << e.what() << std::endl;
		}*/

		Utils::consoleLog(INF, "Memuat data...");
		GrowItems::encodeDat(_DATA + "items/items.json");

		Utils::consoleLog(INF, "Inialisasi ENetHost...");
		GrowServer::init(17000, NORMAL, 0);
		GrowServer::getServerByID(1)->startService();
		std::this_thread::sleep_for(std::chrono::seconds(30));
		GrowServer::getServerByID(1)->setPause(true);
		std::this_thread::sleep_for(std::chrono::seconds(30));
		GrowServer::getServerByID(1)->setPause(false);

		while (true) {
			std::this_thread::sleep_for(std::chrono::seconds(1));
		}
	}
	catch (const std::exception& e)
	{
		std::cout << e.what();
	}
	std::cin.get();
	return 0;
}