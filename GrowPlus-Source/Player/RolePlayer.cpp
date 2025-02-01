
// Menghubungkan header yang diperlukan 
#include <string>
#include <Header/Player/RoleStruct.h>

// Nah disini kita akan mendefinisikan ulang method dan menuliskan kode cpp nya, soalnya di header kita cuman mendefinisikan nama method, tidak dengan kode cpp nya, setauku ini dinamakan prototype

// Kita akan membuat method untuk mendapatkan display name dari enum role name
std::string PlayerRole::getDisplayName(RoleName name) {
	// Kita akan menggunakan switch karena pada kondisi seperti ini switch lebih baik dibandingkan dengan if else
	// Cara kerja switch: switch akan membandingkan hasil key dengan semua case yang ada, jika ada case yang cocok maka akan langsung locat dan mengeksekusi kode tersebut

	switch (name)	// name adalah representasi dari key
	{
		case RoleName::PLAYER: {
			return "PLAYER";
		}
		case RoleName::VIP: {
			return "VIP";
		}
		case RoleName::VVIP: {
			return "VIP+";
		}
		case RoleName::MVP: {
			return "MVP";
		}
		case RoleName::MMVP: {
			return "MVP+";
		}
		case RoleName::MOD: {
			return "MODERATOR";
		}
		case RoleName::SMOD: {
			return "SUPER MODERATOR";
		}
		case RoleName::UMOD: {
			return "ULTRA MODERATOR";
		}
		case RoleName::STAFF: {
			return "STAFF";
		}
		case RoleName::SSTAFF: {
			return "STAFF+";
		}
		case RoleName::COOWNER: {
			return "CO-OWNER";
		}
		case RoleName::OWNER: {
			return "OWNER";
		}
		case RoleName::COFOUNDER: {
			return "CO-FOUNDER";
		}
		case RoleName::FOUNDER: {
			return "FOUNDER";
		}
	}
}