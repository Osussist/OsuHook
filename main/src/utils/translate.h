#pragma once
#include <string>
#include <windows.h>
#include <array>
#include <stdexcept>

class Translate {
public:
    static std::string WcharToChar(const wchar_t* wchar) {
        if (wchar == nullptr) {
            throw std::invalid_argument("Input string cannot be null");
        }

        int bufferSize = WideCharToMultiByte(CP_UTF8, 0, wchar, -1, nullptr, 0, nullptr, nullptr);
        if (bufferSize == 0) {
            throw std::runtime_error("Failed to determine buffer size: " +
                std::to_string(GetLastError()));
        }

        std::string charStr(bufferSize, '\0');
        int result = WideCharToMultiByte(CP_UTF8, 0, wchar, -1, &charStr[0], bufferSize,
            nullptr, nullptr);
        if (result == 0) {
            throw std::runtime_error("Failed to convert string: " +
                std::to_string(GetLastError()));
        }

        charStr.resize(bufferSize - 1);
        return charStr;
    }

    static std::wstring CharToWchar(const char* charStr) {
        if (charStr == nullptr) {
            throw std::invalid_argument("Input string cannot be null");
        }

        int bufferSize = MultiByteToWideChar(CP_UTF8, 0, charStr, -1, nullptr, 0);
        if (bufferSize == 0) {
            throw std::runtime_error("Failed to determine buffer size");
        }

        std::wstring wcharStr(bufferSize, L'\0');
        int result = MultiByteToWideChar(CP_UTF8, 0, charStr, -1, &wcharStr[0], bufferSize);
        if (result == 0) {
            throw std::runtime_error("Failed to convert string");
        }

        wcharStr.resize(bufferSize - 1);
        return wcharStr;
    }

    static std::string CharArrayToString(const std::array<char, 16>& charArray) {
        return std::string(charArray.data(), charArray.size());
    }
};