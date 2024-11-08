#pragma once
#include <string>
#include <windows.h>
#include <array>

class Translate {
public:
	static std::string WcharToChar(const wchar_t* wchar) {
		int bufferSize = WideCharToMultiByte(CP_UTF8, 0, wchar, -1, nullptr, 0, nullptr, nullptr);
		std::string charStr(bufferSize, 0);
		WideCharToMultiByte(CP_UTF8, 0, wchar, -1, &charStr[0], bufferSize, nullptr, nullptr);
		return charStr;
	}

    static std::string CharArrayToString(std::array<char, 16> charArray) {
		return std::string(charArray.data(), charArray.size());
    }
};