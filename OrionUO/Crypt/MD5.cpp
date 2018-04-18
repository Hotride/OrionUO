#include "stdafx.h"

// MD5 Definitions

#define T_MASK ((unsigned int)~0)
#define T1 (T_MASK ^ 0x28955b87)
#define T2 (T_MASK ^ 0x173848a9)
#define T3 0x242070db
#define T4 (T_MASK ^ 0x3e423111)
#define T5 (T_MASK ^ 0x0a83f050)
#define T6 0x4787c62a
#define T7 (T_MASK ^ 0x57cfb9ec)
#define T8 (T_MASK ^ 0x02b96afe)
#define T9 0x698098d8
#define T10 (T_MASK ^ 0x74bb0850)
#define T11 (T_MASK ^ 0x0000a44e)
#define T12 (T_MASK ^ 0x76a32841)
#define T13 0x6b901122
#define T14 (T_MASK ^ 0x02678e6c)
#define T15 (T_MASK ^ 0x5986bc71)
#define T16 0x49b40821
#define T17 (T_MASK ^ 0x09e1da9d)
#define T18 (T_MASK ^ 0x3fbf4cbf)
#define T19 0x265e5a51
#define T20 (T_MASK ^ 0x16493855)
#define T21 (T_MASK ^ 0x29d0efa2)
#define T22 0x02441453
#define T23 (T_MASK ^ 0x275e197e)
#define T24 (T_MASK ^ 0x182c0437)
#define T25 0x21e1cde6
#define T26 (T_MASK ^ 0x3cc8f829)
#define T27 (T_MASK ^ 0x0b2af278)
#define T28 0x455a14ed
#define T29 (T_MASK ^ 0x561c16fa)
#define T30 (T_MASK ^ 0x03105c07)
#define T31 0x676f02d9
#define T32 (T_MASK ^ 0x72d5b375)
#define T33 (T_MASK ^ 0x0005c6bd)
#define T34 (T_MASK ^ 0x788e097e)
#define T35 0x6d9d6122
#define T36 (T_MASK ^ 0x021ac7f3)
#define T37 (T_MASK ^ 0x5b4115bb)
#define T38 0x4bdecfa9
#define T39 (T_MASK ^ 0x0944b49f)
#define T40 (T_MASK ^ 0x4140438f)
#define T41 0x289b7ec6
#define T42 (T_MASK ^ 0x155ed805)
#define T43 (T_MASK ^ 0x2b10cf7a)
#define T44 0x04881d05
#define T45 (T_MASK ^ 0x262b2fc6)
#define T46 (T_MASK ^ 0x1924661a)
#define T47 0x1fa27cf8
#define T48 (T_MASK ^ 0x3b53a99a)
#define T49 (T_MASK ^ 0x0bd6ddbb)
#define T50 0x432aff97
#define T51 (T_MASK ^ 0x546bdc58)
#define T52 (T_MASK ^ 0x036c5fc6)
#define T53 0x655b59c3
#define T54 (T_MASK ^ 0x70f3336d)
#define T55 (T_MASK ^ 0x00100b82)
#define T56 (T_MASK ^ 0x7a7ba22e)
#define T57 0x6fa87e4f
#define T58 (T_MASK ^ 0x01d3191f)
#define T59 (T_MASK ^ 0x5cfebceb)
#define T60 0x4e0811a1
#define T61 (T_MASK ^ 0x08ac817d)
#define T62 (T_MASK ^ 0x42c50dca)
#define T63 0x2ad7d2bb
#define T64 (T_MASK ^ 0x14792c6e)

// Constructor / Destructor
MD5Crypt::MD5Crypt()
{
}
MD5Crypt::~MD5Crypt()
{
}

// Public Member Functions

void MD5Crypt::Init(unsigned char* Data, unsigned int Size)
{
    md5_state tmpMd5;

    TableIdx = 0;

    start(&tmpMd5);
    append(&tmpMd5, (const unsigned char*)Data, Size);
    finish(&tmpMd5, Digest);
}

void MD5Crypt::Encrypt(unsigned char* in, unsigned char* out, int len)
{
    for (int i = 0; i < len; i++)
        out[i] = in[i] ^ Digest[TableIdx++ % 16];
}

// Protected Member Functions

void MD5Crypt::process(md5_state* pms, const unsigned char* data)
{
    unsigned int a = pms->abcd[0], b = pms->abcd[1], c = pms->abcd[2], d = pms->abcd[3];
    unsigned int t = 0;
    unsigned int X[16];
    const unsigned char* xp = data;

    for (int i = 0; i < 16; ++i, xp += 4)
        X[i] = xp[0] + (xp[1] << 8) + (xp[2] << 16) + (xp[3] << 24);

#define ROTATE_LEFT(x, n) (((x) << (n)) | ((x) >> (32 - (n))))

        // Round 1
        // Let [abcd k s i] denote the operation
        // a = b + ((a + F(b,c,d) + X[k] + T[i]) <<< s)
#define F(x, y, z) (((x) & (y)) | (~(x) & (z)))
#define SET(a, b, c, d, k, s, Ti)   \
    t = a + F(b, c, d) + X[k] + Ti; \
    a = ROTATE_LEFT(t, s) + b
    // Do the following 16 operations
    SET(a, b, c, d, 0, 7, T1);
    SET(d, a, b, c, 1, 12, T2);
    SET(c, d, a, b, 2, 17, T3);
    SET(b, c, d, a, 3, 22, T4);
    SET(a, b, c, d, 4, 7, T5);
    SET(d, a, b, c, 5, 12, T6);
    SET(c, d, a, b, 6, 17, T7);
    SET(b, c, d, a, 7, 22, T8);
    SET(a, b, c, d, 8, 7, T9);
    SET(d, a, b, c, 9, 12, T10);
    SET(c, d, a, b, 10, 17, T11);
    SET(b, c, d, a, 11, 22, T12);
    SET(a, b, c, d, 12, 7, T13);
    SET(d, a, b, c, 13, 12, T14);
    SET(c, d, a, b, 14, 17, T15);
    SET(b, c, d, a, 15, 22, T16);
#undef SET

    // Round 2
    // Let [abcd k s i] denote the operation
    // a = b + ((a + G(b,c,d) + X[k] + T[i]) <<< s)
#define G(x, y, z) (((x) & (z)) | ((y) & ~(z)))
#define SET(a, b, c, d, k, s, Ti)   \
    t = a + G(b, c, d) + X[k] + Ti; \
    a = ROTATE_LEFT(t, s) + b
    // Do the following 16 operations
    SET(a, b, c, d, 1, 5, T17);
    SET(d, a, b, c, 6, 9, T18);
    SET(c, d, a, b, 11, 14, T19);
    SET(b, c, d, a, 0, 20, T20);
    SET(a, b, c, d, 5, 5, T21);
    SET(d, a, b, c, 10, 9, T22);
    SET(c, d, a, b, 15, 14, T23);
    SET(b, c, d, a, 4, 20, T24);
    SET(a, b, c, d, 9, 5, T25);
    SET(d, a, b, c, 14, 9, T26);
    SET(c, d, a, b, 3, 14, T27);
    SET(b, c, d, a, 8, 20, T28);
    SET(a, b, c, d, 13, 5, T29);
    SET(d, a, b, c, 2, 9, T30);
    SET(c, d, a, b, 7, 14, T31);
    SET(b, c, d, a, 12, 20, T32);
#undef SET

    // Round 3
    // Let [abcd k s t] denote the operation
    // a = b + ((a + H(b,c,d) + X[k] + T[i]) <<< s)
#define H(x, y, z) ((x) ^ (y) ^ (z))
#define SET(a, b, c, d, k, s, Ti)   \
    t = a + H(b, c, d) + X[k] + Ti; \
    a = ROTATE_LEFT(t, s) + b
    // Do the following 16 operations
    SET(a, b, c, d, 5, 4, T33);
    SET(d, a, b, c, 8, 11, T34);
    SET(c, d, a, b, 11, 16, T35);
    SET(b, c, d, a, 14, 23, T36);
    SET(a, b, c, d, 1, 4, T37);
    SET(d, a, b, c, 4, 11, T38);
    SET(c, d, a, b, 7, 16, T39);
    SET(b, c, d, a, 10, 23, T40);
    SET(a, b, c, d, 13, 4, T41);
    SET(d, a, b, c, 0, 11, T42);
    SET(c, d, a, b, 3, 16, T43);
    SET(b, c, d, a, 6, 23, T44);
    SET(a, b, c, d, 9, 4, T45);
    SET(d, a, b, c, 12, 11, T46);
    SET(c, d, a, b, 15, 16, T47);
    SET(b, c, d, a, 2, 23, T48);
#undef SET

    // Round 4
    // Let [abcd k s t] denote the operation
    // a = b + ((a + I(b,c,d) + X[k] + T[i]) <<< s)
#define I(x, y, z) ((y) ^ ((x) | ~(z)))
#define SET(a, b, c, d, k, s, Ti)   \
    t = a + I(b, c, d) + X[k] + Ti; \
    a = ROTATE_LEFT(t, s) + b
    // Do the following 16 operations
    SET(a, b, c, d, 0, 6, T49);
    SET(d, a, b, c, 7, 10, T50);
    SET(c, d, a, b, 14, 15, T51);
    SET(b, c, d, a, 5, 21, T52);
    SET(a, b, c, d, 12, 6, T53);
    SET(d, a, b, c, 3, 10, T54);
    SET(c, d, a, b, 10, 15, T55);
    SET(b, c, d, a, 1, 21, T56);
    SET(a, b, c, d, 8, 6, T57);
    SET(d, a, b, c, 15, 10, T58);
    SET(c, d, a, b, 6, 15, T59);
    SET(b, c, d, a, 13, 21, T60);
    SET(a, b, c, d, 4, 6, T61);
    SET(d, a, b, c, 11, 10, T62);
    SET(c, d, a, b, 2, 15, T63);
    SET(b, c, d, a, 9, 21, T64);
#undef SET

    // Then perform the following additions (That is increment each
    // of the four registers by the value it had before this block
    // was started)
    pms->abcd[0] += a;
    pms->abcd[1] += b;
    pms->abcd[2] += c;
    pms->abcd[3] += d;
}

void MD5Crypt::start(md5_state* pms)
{
    pms->count[0] = pms->count[1] = 0;
    pms->abcd[0] = 0x67452301;
    pms->abcd[1] = T_MASK ^ 0x10325476;
    pms->abcd[2] = T_MASK ^ 0x67452301;
    pms->abcd[3] = 0x10325476;
}

void MD5Crypt::append(md5_state* pms, const unsigned char* data, int nbytes)
{
    const unsigned char* p = data;
    unsigned int nbits = (unsigned int)(nbytes << 3);
    int left = nbytes, offset = (pms->count[0] >> 3) & 63;

    if (nbytes <= 0)
        return;

    // Update the message length
    pms->count[1] += nbytes >> 29;
    pms->count[0] += nbits;
    if (pms->count[0] < nbits)
        pms->count[1]++;

    // Process an initial partial block
    if (offset) {
        int copy = (offset + nbytes > 64 ? 64 - offset : nbytes);
        memcpy(pms->buf + offset, p, copy);

        if (offset + copy < 64)
            return;

        p += copy;
        left -= copy;
        process(pms, pms->buf);
    }

    // Process full blocks
    for (; left >= 64; p += 64, left -= 64)
        process(pms, p);

    // Process a final partial block
    if (left) {
        if (left > 63)
            left = 63;
        memcpy(pms->buf, p, left);
    }
}

void MD5Crypt::finish(md5_state* pms, unsigned char digest[16])
{
    static const unsigned char pad[64] = {
        0x80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
    };

    unsigned char data[8];

    // Save the length before padding
    for (int i = 0; i < 8; ++i)
        data[i] = (unsigned char)(pms->count[i >> 2] >> ((i & 3) << 3));

    // Pad to 56 bytes mod 64
    append(pms, pad, ((55 - (pms->count[0] >> 3)) & 63) + 1);

    // Append the length
    append(pms, data, 8);
    for (int i = 0; i < 16; ++i)
        digest[i] = (unsigned char)(pms->abcd[i >> 2] >> ((i & 3) << 3));
}
