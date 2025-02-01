#pragma once

// Di file ini kita akan mendefinisikan beberapa role yang ada didalam in-game

// Ini adala basic role type player yang memisahkan power antara player biasa dengan moderator
enum class RoleType {
	NORMAL,
	GUARDIAN,
	MODERATOR,
	DEVELOPER
};

// Spesial role berarti role ini memiliki kelebihan khusus tanpa harus memiliki RoleName atau tanpa terpaku dengan power RoleType
enum class RoleSpecial {
	PROMOTER,
	DEVELOPER,
	DESIGNER
};

// Disini tempat nama nama role yang dibungkus dengan enum agar mudah dibaca
enum class RoleName {
	PLAYER,
	VIP,
	VVIP,
	MVP,
	MMVP,
	MOD,
	SMOD,
	UMOD,
	STAFF,
	SSTAFF,
	COOWNER,
	OWNER,
	COFOUNDER,
	FOUNDER
};

// Kita bungkus 3 jenis diatas menjadi 1 struct
struct PlayerRole {
	RoleType type;
	RoleSpecial special;
	RoleName name;

	std::string getDisplayName(RoleName name);
};