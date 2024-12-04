#pragma once
#include <string>

class Signature {
public:
	Signature(std::string givenPattern, int givenOffset = 0x0) : pattern(givenPattern), offset(givenOffset) { return; }

	std::string pattern = "";
	int offset = 0x0;
};

// Might be useful to add different types of signatures in the future, for now only focus on IDA style ones
// IDAStyle = "F8 01 74 ?? 83 65" <- the "??" is the wildcard byte
// Mask&Bytes = "\xF8\x01\x74\x00\x83\x65" and "xxxx?xx" <- the "?" is the wildcard byte