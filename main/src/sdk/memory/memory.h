#pragma once
#include "../../utils/logger.h"
#include "../objects/signatures.h"
#include <psapi.h>
#include <vector>

class Memory {
public:
	Memory() : logger(0, false) { return; }
	Memory(Logger sdkLogger, HANDLE processHandle);

	uintptr_t pattern_scan(Signature signature);

	uintptr_t baseAddr;
	uintptr_t audioTimeAddr;
private:
	Logger logger;
	HANDLE processHandle;

	inline static Signature baseSig = Signature("F8 01 74 04 83 65", 0x0);
	inline static Signature audioTimeSig = Signature("DB 5C 24 34 8B 44 24 34", 0x2);
};

// [baseSig - 0xc] -> read_ptr = beatmapAddr
// [beatmapAddr + 0x6c] -> read_int -> read_string = beatmapChecksum