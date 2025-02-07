
// Agar file ini di compile kita harus menyertakan header dari file ini
// Fungsi dari file ini adalah untuk menulis proses yang panjang, sehingga tidak menggangu struktur dari file header

// Pertama, kita harus include header yang dibutuhkan terlebih  dahulu
#include <Header/Server/ServerConfig.h>
#include <SDK/Proton/TextScanner.h>
#include <Header/Utils/Utils.h>

// Kemudian kita akan mendefinisikan functionnya beserta kode cppnya, soalnya yang di header kuta cuma membuat sebuah inisialisasi awal/prototype
void ServerConfig::loadConfig() {	// const berarti membuat nilainya tetap, dan & berarti datanya akan diturunkan secara langsung tanpa dicopy terlebih dahulu 
	// Kita akan membuat sebuah penanganan error yang dilempar oleh program
	try
	{
		// Kita akan membuat objek dengan class TextScanner
		TextScanner ctx;

		// Kita akan load datanya dari sebuah file
		ctx.LoadFile(_DATA + "config/server.txt");

		// Disini kita cek dulu apakah file berhasil dibudiloadka apa enggak
		if (!ctx.IsLoaded()) {
			throw std::runtime_error("Failed to load: " + _DATA + "config/server.txt");
		}

		// Kita akan memasukkan datanga ke server
		this->setServerName(ctx.GetParmString("server_name", 1));
		this->setClist(Utils::explode(",", ctx.GetParmString("clist", 1)));
		this->setMaintain(ctx.GetParmUInt("maintain", 1));

		// Nah sekarang untuk bagian enetnya
		// Kita akan load datanya dari sebuah file
		ctx.LoadFile(_DATA + "config/enet.txt");

		// Disini kita cek dulu apakah file berhasil dibudiloadka apa enggak
		if (!ctx.IsLoaded()) {
			throw std::runtime_error("Failed to load: " + _DATA + "config/enet.txt");
		}

		BASE_PORT base{};
		base.creative = ctx.GetParmUInt("base_port_creative", 1);
		base.normal = ctx.GetParmUInt("base_port_normal", 1);
		this->setBasePort(base);

		SUBSERVER_DATA data{};
		data.amount = ctx.GetParmUInt("subserver", 1);
		data.maxConnectedPeer = ctx.GetParmUInt("maxpeers", 1);
		this->setSubServerData(data);
	}
	catch (const std::exception& e)
	{
		Utils::consoleLog(ERR, std::string("Error while loading " + _DATA + "config: ") + e.what());
	}
}