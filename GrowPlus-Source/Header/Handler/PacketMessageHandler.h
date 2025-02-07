#pragma once  // Ini untuk memastikan file header hanya di-include sekali dalam proses kompilasi

// Menambahkan library yang digunakan 
#include <x64-windows/include/enet/enet.h>  

// Pada file ini kita akan meng-handle atau mengelola packet message yang diterima oleh server
// Kita buat objek dalam bentuk class agar lebih terstruktur dan mudah dibaca serta diterapkan 
class PacketMessageHandler {
public:
    // Constructor: Fungsi ini dipanggil saat objek PacketMessageHandler dibuat
    // Di sini kita menginisialisasi peer (klien), server, dan event untuk menangani komunikasi
    PacketMessageHandler(ENetHost* server, ENetEvent event, ENetPeer* peer)
        : peer(peer), server(server), event(event) {}  // Menyimpan data yang diperlukan untuk komunikasi dengan server dan peer

    // Destructor: Fungsi ini dipanggil ketika objek PacketMessageHandler dihancurkan
    ~PacketMessageHandler() {}  // Tidak ada proses khusus di sini, jadi kosong

    // Fungsi untuk menangani pesan teks yang lebih umum
    bool GenericText();

    // Fungsi untuk menangani pesan teks khusus untuk game
    bool GameText();

    // Fungsi untuk menangani packet yang berhubungan dengan game
    bool GamePacket();

private:
    ENetPeer* peer;  // Menyimpan pointer ke peer (klien) yang terhubung dengan server
    ENetHost* server;  // Menyimpan pointer ke server yang sedang berjalan
    ENetEvent event;  // Menyimpan event yang terjadi (misalnya, data yang diterima)
};
