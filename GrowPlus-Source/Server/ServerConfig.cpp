
// Agar file ini di compile kita harus menyertakan header dari file ini
// Fungsi dari file ini adalah untuk menulis proses yang panjang, sehingga tidak menggangu struktur dari file header

// Pertama, kita harus include header yang dibutuhkan terlebih  dahulu
#include <Header/Server/ServerConfig.h>
#include <SDK/Proton/TextScanner.h>

// Kemudian kita akan mendefinisikan functionnya beserta kode cppnya, soalnya yang di header kuta cuma membuat sebuah inisialisasi awal/prototype
void ServerConfig::loadConfig(const std::string& pathFile) {	// const berarti membuat nilainya tetap, dan & berarti datanya akan diturunkan secara langsung tanpa dicopy terlebih dahulu 
	TextScanner ctx;
	ctx.LoadFile(pathFile);
}