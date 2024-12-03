#include "memory.h"

template <typename T>
T ReadBytes(uintptr_t address, int bytes) {
	std::vector<byte> buffer(bytes);
	ReadProcessMemory(processHandle, (LPCVOID)address, buffer.data(), bytes, NULL);
	return *reinterpret_cast<T*>(buffer.data());
}

Memory::Memory(Logger sdkLogger, HANDLE processHandle) : logger(sdkLogger), processHandle(processHandle) {
	logger.info(__FUNCTION__, "Initializing Memory");
	baseAddr = pattern_scan(baseSig);
	logger.debug(__FUNCTION__, "Found base address: " + std::to_string(baseAddr));
	audioTimeAddr = pattern_scan(audioTimeSig);
	logger.debug(__FUNCTION__, "Found audio time address: " + std::to_string(audioTimeAddr));
	logger.info(__FUNCTION__, "Memory initialized");
}

uintptr_t Memory::read_ptr(uintptr_t address) {
	return ReadBytes<uintptr_t>(address, sizeof(uintptr_t));
}

int Memory::read_int(uintptr_t address) {
	return ReadBytes<int>(address, sizeof(int));
}

double Memory::read_double(uintptr_t address) {
	return ReadBytes<double>(address, sizeof(double));
}

std::string Memory::read_string(uintptr_t address, size_t length) {
	std::vector<char> buffer(length);
	ReadProcessMemory(processHandle, (LPCVOID)address, buffer.data(), length, NULL);
	return std::string(buffer.data(), length);
}

uintptr_t Memory::pattern_scan(Signature signature) {
	logger.debug(__FUNCTION__, "Scanning for pattern: " + signature.pattern);
	MODULEINFO moduleInfo;
	GetModuleInformation(processHandle, GetModuleHandle(NULL), &moduleInfo, sizeof(MODULEINFO));
	uintptr_t startAddr = reinterpret_cast<uintptr_t>(moduleInfo.lpBaseOfDll);
	uintptr_t endAddr = startAddr + moduleInfo.SizeOfImage;
	uintptr_t patternLength = signature.pattern.length();
	std::vector<uint8_t> patternBytes;
	std::vector<bool> patternMask;
	for (size_t i = 0; i < patternLength; i += 3) {
		if (signature.pattern[i] == ' ') {
			i++;
		}
		if (signature.pattern[i] == '?') {
			patternBytes.push_back(0x0);
			patternMask.push_back(false);
		}
		else {
			patternBytes.push_back(std::stoi(signature.pattern.substr(i, 2), nullptr, 16));
			patternMask.push_back(true);
		}
	}
	for (uintptr_t i = startAddr; i < endAddr; i++) {
		bool found = true;
		for (size_t j = 0; j < patternLength / 3; j++) {
			if (patternMask[j] && *reinterpret_cast<uint8_t*>(i + j) != patternBytes[j]) {
				found = false;
				break;
			}
		}
		if (found) {
			logger.debug(__FUNCTION__, "Found pattern at: " + std::to_string(i));
			return i + signature.offset;
		}
	}
	logger.error(__FUNCTION__, "Failed to find pattern");
	return 0;
}