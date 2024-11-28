#pragma once
#include <string>

class Signature {
public:
	Signature(std::string pattern, int offset = 0x0) {
		this->pattern = pattern;
		this->offset = offset;
	}
	std::string pattern = "";
	int offset = 0x0;
};

class Signatures {
public:
	inline static Signature baseAddr = Signature("F8 01 74 04 83 65", 0x0);
	inline static Signature audioTimeAddr = Signature("DB 5C 24 34 8B 44 24 34", 0x2);
};

// [baseAddr - 0xc] -> read_ptr = beatmapAddr
// [beatmapAddr + 0x6c] -> read_int -> read_string = beatmapChecksum