#pragma once

// Pengetahuan dasar!
//Function dengan tipe struct
//Struct adalah kotak yang berisi beberapa data sekaligus seperti nama dan umur seseorang.Jika sebuah function memiliki tipe struct, berarti function tersebut akan membuat kotak itu dan mengembalikannya.
//
//Contoh sehari - hari
//Bayangkan ingin membuat kartu nama.Bisa dibuat function bernama buatKartuNama yang menerima nama dan umur lalu mengembalikan kartu yang sudah diisi dengan data tersebut.
//
//Function ini seperti pekerja yang membuat sesuatu berupa struct dan memberikannya kembali.

//Variabel dengan tipe struct
//Struct adalah kotak yang berisi beberapa data sekaligus seperti nama dan umur seseorang.Jika variabel memiliki tipe struct, berarti variabel tersebut adalah sebuah kotak yang bisa menyimpan data yang sesuai dengan struct itu.
//
//Contoh sehari - hari
//Bayangkan punya dompet yang bisa menyimpan kartu nama.Jika kartu nama adalah struct, maka dompet adalah variabel dengan tipe struct yang menyimpan kartu itu.
//
//Variabel dengan tipe struct digunakan untuk menyimpan data dalam satu kesatuan sehingga lebih rapi dan mudah digunakan.

// variabel this merupakan nilai pada objek saat ini, jika tidak menggunakan this bisa aja nilainya berbeda dengan keadaan aslinya


// Kita harus include header yang dibutuhkan 
#include <string>
#include <vector>
#include <Header/Server/ServerStruct.h>

class ServerConfig {
public:
	// Kita buat method untuk serverName
	// Method untuk mendapatkan nama server
	std::string getServerName() const {	// atribut const berarti method ini tidak bisa mengubah value dari variabelnya (lebih aman)
		return this->serverName;
	}
	// Method untuk menimpa value dari nama server
	void setServerName(const std::string& name) {	// const berarti membuat nilainya tetap, dan & berarti datanya akan diturunkan secara langsung tanpa dicopy terlebih dahulu 
		this->serverName = name;
		// Void tidak perlu mengembalikan sebuah nilai
	}

	// Kita buat method yang berhubungan dengan port
	// Method untuk mendapatkan base port
	BASE_PORT getBasePort() const {	// atribut const berarti method ini tidak bisa mengubah value dari variabelnya (lebih aman)
		// Contoh function dengan tipe struct
		return this->BasePort;	// Nilai yang dikembalikan harus memiliki tipe yang sama
	}
	// Method untuk mengatur base port
	void setBasePort(const BASE_PORT& port) {	// const berarti membuat nilainya tetap, dan & berarti datanya akan diturunkan secara langsung tanpa dicopy terlebih dahulu 
		this->BasePort = port;
		// Void tidak perlu mengembalikan sebuah nilai
	}

	// Kita buat method yang berhubungan dengan Sub-Server
	// Method untuk mendapatkan subserver config
	SUBSERVER_DATA getSubServerData() const {	// atribut const berarti method ini tidak bisa mengubah value dari variabelnya (lebih aman)
		// contoh function dengan tipe struct
		return this->SubServer;	// Nilai yang dikembalikan harus memiliki tipe yang sama
	}
	//Method untuk mengatur subserver config
	void setSubServerData(const SUBSERVER_DATA& Data) {	// const berarti membuat nilainya tetap, dan & berarti datanya akan diturunkan secara langsung tanpa dicopy terlebih dahulu 
		this->SubServer = Data;
		// Void tidak perlu mengembalikan  sebuah nilai
	}

	// Kita buat method untuk maintain
	// Method untuk mendapatkan maintain
	bool getMaintain() const {	// atribut const berarti method ini tidak bisa mengubah value dari variabelnya (lebih aman)
		return this->maintain;
	}
	// Method untuk menimpa value dari maintain
	void setMaintain(const bool& status) {	// const berarti membuat nilainya tetap, dan & berarti datanya akan diturunkan secara langsung tanpa dicopy terlebih dahulu 
		this->maintain = status;
		// Void tidak perlu mengembalikan sebuah nilai
	}

	// Kita buat method untuk clist
	// Method untuk mendapatkan value dari clist
	std::vector<std::string> getClist() const {	// Const tidak akan mengubah data
		// Tipe vector seperti array tapi lebih moderen, vector disini memiliki 1 dimensi dengan tipe string (text)
		return this->clist;	// Nilai yang dikembalikan harus sesuai dengan tipe methodnya
	}
	// Method untuk menimpa value dari clist
	void setClist(const std::vector<std::string>& clist) {	// const berarti membuat nilainya tetap, dan & berarti datanya akan diturunkan secara langsung tanpa dicopy terlebih dahulu 
		// Tipe void berarti method ini tidak akan mengembalikan nilai apapun
		// pada parameter pertama dia memiliki tipe vector seperti array tapi lebih moderen, vector disini memiliki 1 dimensi dengan tipe string (text)
		this->clist = clist;	// FYI jika kondisinya seperti ini, value dari this->clist sama clist berbeda (walaupun namanya sama) karena letah dari pemosisian dibedakan dengan variabel this
	}

	// Nah disini kita akan buat method dengan proses paling berat pada class ini, sehingga kita harus memisahkannya pada file yang berbeda, dengan extension cpp
	void loadConfig();
private:
	// Variabel dengan jenis string (text) yang menyimpan nama server
	std::string serverName;
	 
	// Variabel dengan jenis struct yang menampung config untuk port server
	BASE_PORT BasePort;

	// Variabel dengan jenis struct yang menampung config untuk subserver 
	SUBSERVER_DATA SubServer;

	// Variabel sengan jenis boolean (jenis paling ringan yang hanya menampung 1/0, true/false) untuk menentukan apakah servers mengalami maintain
	bool maintain;

	// Variabel dengan jenis vector 1 dimensi yang bertipe string (text) vector seperti array tapi lebih modern
	std::vector<std::string> clist;	// clist adalah orang yang memiliki power paling tinggi yang dapat mengakses semua fitur dengan bebas
};
