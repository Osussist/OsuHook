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
	inline static Signature AudioTime = Signature("DB 5C 24 34 8B 44 24 34", 0x2);
	inline static Signature BeatmapChecksum = Signature("", 0x0); // I dont think its possible to get this signature
};