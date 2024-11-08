#ifndef CRYPTO_H
#define CRYPTO_H

#include <array>
#include <string>
#include <fstream>
#include <sstream>
#include <cstdint>

class Crypto {
public:
    Crypto() = default;

    static std::array<char, 16> get_md5(const std::string& input) {
        return md5(input.c_str());
    }

    static std::array<char, 16> get_md5_from_file(const std::string& filename) {
        std::ifstream file(filename, std::ios::binary);
        if (!file) {
            return {};
        }
        std::ostringstream oss;
        oss << file.rdbuf();
        std::string content = oss.str();
        return get_md5(content);
    }

private:
    // MD5 operations
    static constexpr uint32_t f(uint32_t x, uint32_t y, uint32_t z) {
        return z ^ (x & (y ^ z));
    }

    static constexpr uint32_t g(uint32_t x, uint32_t y, uint32_t z) {
        return y ^ (z & (x ^ y));
    }

    static constexpr uint32_t h(uint32_t x, uint32_t y, uint32_t z) {
        return x ^ y ^ z;
    }

    static constexpr uint32_t i(uint32_t x, uint32_t y, uint32_t z) {
        return y ^ (x | ~z);
    }

    static uint32_t step_helper(uint32_t fun_val, uint32_t s, uint32_t b) {
        return ((fun_val << s) | (fun_val >> (32 - s))) + b;
    }

    template<typename Functor>
    static uint32_t step(Functor fun, uint32_t a, uint32_t b, uint32_t c, uint32_t d,
        uint32_t x, uint32_t t, uint32_t s) {
        return step_helper(a + fun(b, c, d) + x + t, s, b);
    }

    static uint32_t data32(const char* data, size_t n) {
        return (static_cast<uint32_t>(data[n * 4]) & 0xff) |
            ((static_cast<uint32_t>(data[n * 4 + 1]) << 8) & 0xff00) |
            ((static_cast<uint32_t>(data[n * 4 + 2]) << 16) & 0xff0000) |
            ((static_cast<uint32_t>(data[n * 4 + 3]) << 24) & 0xff000000);
    }

    static constexpr std::array<uint32_t, 64> md5_constants = { {
        0xd76aa478,0xe8c7b756,0x242070db,0xc1bdceee,0xf57c0faf,0x4787c62a,
        0xa8304613,0xfd469501,0x698098d8,0x8b44f7af,0xffff5bb1,0x895cd7be,
        0x6b901122,0xfd987193,0xa679438e,0x49b40821,0xf61e2562,0xc040b340,
        0x265e5a51,0xe9b6c7aa,0xd62f105d,0x02441453,0xd8a1e681,0xe7d3fbc8,
        0x21e1cde6,0xc33707d6,0xf4d50d87,0x455a14ed,0xa9e3e905,0xfcefa3f8,
        0x676f02d9,0x8d2a4c8a,0xfffa3942,0x8771f681,0x6d9d6122,0xfde5380c,
        0xa4beea44,0x4bdecfa9,0xf6bb4b60,0xbebfbc70,0x289b7ec6,0xeaa127fa,
        0xd4ef3085,0x04881d05,0xd9d4d039,0xe6db99e5,0x1fa27cf8,0xc4ac5665,
        0xf4292244,0x432aff97,0xab9423a7,0xfc93a039,0x655b59c3,0x8f0ccc92,
        0xffeff47d,0x85845dd1,0x6fa87e4f,0xfe2ce6e0,0xa3014314,0x4e0811a1,
        0xf7537e82,0xbd3af235,0x2ad7d2bb,0xeb86d391
    } };

    static constexpr std::array<size_t, 64> md5_shift = { {
        7,12,17,22,7,12,17,22,7,12,17,22,7,12,17,22,
        5,9,14,20,5,9,14,20,5,9,14,20,5,9,14,20,
        4,11,16,23,4,11,16,23,4,11,16,23,4,11,16,23,
        6,10,15,21,6,10,15,21,6,10,15,21,6,10,15,21
    } };

    static constexpr std::array<size_t, 64> md5_indexes = { {
        0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,
        1,6,11,0,5,10,15,4,9,14,3,8,13,2,7,12,
        5,8,11,14,1,4,7,10,13,0,3,6,9,12,15,2,
        0,7,14,5,12,3,10,1,8,15,6,13,4,11,2,9
    } };

    static std::array<char, 16> md5(const char* data) {
        uint32_t a = 0x67452301;
        uint32_t b = 0xefcdab89;
        uint32_t c = 0x98badcfe;
        uint32_t d = 0x10325476;
        uint32_t aa = a, bb = b, cc = c, dd = d;

        for (size_t n = 0; n < 64; ++n) {
            size_t idx = md5_indexes[n];
            uint32_t x = data32(data, idx);
            uint32_t t = md5_constants[n];
            size_t s = md5_shift[n];

            if (n < 16) {
                a = step(f, a, b, c, d, x, t, s);
            }
            else if (n < 32) {
                a = step(g, a, b, c, d, x, t, s);
            }
            else if (n < 48) {
                a = step(h, a, b, c, d, x, t, s);
            }
            else {
                a = step(i, a, b, c, d, x, t, s);
            }

            uint32_t temp = d;
            d = c;
            c = b;
            b = a;
            a = temp;
        }

        a += aa;
        b += bb;
        c += cc;
        d += dd;

        return {
            static_cast<char>(a & 0xff), static_cast<char>((a >> 8) & 0xff),
            static_cast<char>((a >> 16) & 0xff), static_cast<char>((a >> 24) & 0xff),
            static_cast<char>(b & 0xff), static_cast<char>((b >> 8) & 0xff),
            static_cast<char>((b >> 16) & 0xff), static_cast<char>((b >> 24) & 0xff),
            static_cast<char>(c & 0xff), static_cast<char>((c >> 8) & 0xff),
            static_cast<char>((c >> 16) & 0xff), static_cast<char>((c >> 24) & 0xff),
            static_cast<char>(d & 0xff), static_cast<char>((d >> 8) & 0xff),
            static_cast<char>((d >> 16) & 0xff), static_cast<char>((d >> 24) & 0xff)
        };
    }
};

#endif // CRYPTO_H