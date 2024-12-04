#ifndef CRYPTO_H
#define CRYPTO_H

#include <array>
#include <string>
#include <fstream>
#include <sstream>
#include <cstdint>
#include <Windows.h>
#include <Wincrypt.h>
#include <iomanip>

class Crypto {
public:
	Crypto() = default;

	static std::string GetMD5FromString(const std::string& input) {
		auto hash = md5(input.c_str(), input.size());
		return to_hex_string(hash);
	}

	static std::string GetMD5FromFile(const std::string& filename) {
		std::ifstream file(filename, std::ios::binary);
		if (!file) {
			return {};
		}
		std::ostringstream oss;
		oss << file.rdbuf();
		std::string content = oss.str();
		return GetMD5FromString(content);
	}

private:
	static std::array<BYTE, 16> md5(const char* data, size_t length) {
		HCRYPTPROV hProv = 0;
		HCRYPTHASH hHash = 0;
		BYTE hash[16];
		DWORD hashLen = 16;

		if (!CryptAcquireContext(&hProv, NULL, NULL, PROV_RSA_FULL, CRYPT_VERIFYCONTEXT)) {
			return {};
		}

		if (!CryptCreateHash(hProv, CALG_MD5, 0, 0, &hHash)) {
			CryptReleaseContext(hProv, 0);
			return {};
		}

		if (!CryptHashData(hHash, reinterpret_cast<const BYTE*>(data), length, 0)) {
			CryptDestroyHash(hHash);
			CryptReleaseContext(hProv, 0);
			return {};
		}

		if (!CryptGetHashParam(hHash, HP_HASHVAL, hash, &hashLen, 0)) {
			CryptDestroyHash(hHash);
			CryptReleaseContext(hProv, 0);
			return {};
		}

		CryptDestroyHash(hHash);
		CryptReleaseContext(hProv, 0);

		std::array<BYTE, 16> result;
		std::copy(hash, hash + 16, result.begin());
		return result;
	}

	static std::string to_hex_string(const std::array<BYTE, 16>& hash) {
		std::ostringstream oss;
		for (BYTE byte : hash) {
			oss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(byte);
		}
		return oss.str();
	}
};

#endif // CRYPTO_H
