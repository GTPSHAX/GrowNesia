#pragma once

// Menghubungkan beberapa include yang dibutuhkan 
#include <vector>

#include <Header/Server/ServerStruct.h>	// Menghubungkan server struct
#include <Header/Server/ENetServer.h>	// Menghubungkan enet server ke growtopia server class
#include <Header/Utils/Utils.h>			// Menghubungkan utils

class GrowServer {
public:
	static void init(const uint16_t& port, const ServerType& type, const uint8_t maxPeers);	 // Mendeklarasikan method untuk inisialisasi/membuat enet server
	static std::vector<ENetServer*> getServers() { return servers; }	// method untuk mendapatkan semua enet server
	static ENetServer* getServerByID(uint16_t id);	// Method untuk mendapatkan server berbasis id
	static ServerData& getData() { return Data; };	// Method untuk mendapatkan data
	// Method untuk Mendapatkan semua online peer pada setiap servers
	static int getOnlinePeers() { return onlinePeer; }
	// Method untuk menimpa data jumlah peer
	static void setOnlinePeers(int count) { onlinePeer = count; }
private:
	static std::vector<ENetServer*> servers;	// Semua enet server akan disimpan disin
	static ServerData Data;	// Data server akan disimpan disini
	// Semua jumlah online peer akan disimpan disini
	static int onlinePeer;
};