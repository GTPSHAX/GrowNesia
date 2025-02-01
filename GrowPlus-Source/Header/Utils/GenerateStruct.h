#pragma once

enum GenerateType : uint8_t {
	ACCOUNT = 0,
	WORLD,
	GUILD
};

struct {
	int account = 0, world = 0, guild = 0;
} GenerateData;