#include "stdafx.h"

CLoginCrypt g_LoginCrypt;

CLoginCrypt::CLoginCrypt()
{
    memset(&m_Seed, 0, sizeof(m_Seed));
}

void CLoginCrypt::Init(uint8_t* ps)
{
    memcpy(m_Seed, ps, 4);
    uint seed = (ps[0] << 24) | (ps[1] << 16) | (ps[2] << 8) | ps[3];

    m_Key[0] = (((~seed) ^ SeedKey) << 16) | ((seed ^ 0xffffaaaa) & 0x0000ffff);
    m_Key[1] = ((seed ^ 0x43210000) >> 16) | (((~seed) ^ 0xabcdffff) & 0xffff0000);
}

void CLoginCrypt::Encrypt(uint8_t* in, uint8_t* out, int size)
{
    for (int i = 0; i < size; i++) {
        out[i] = in[i] ^ static_cast<uint8_t>(m_Key[0]);

        uint table0 = m_Key[0];
        uint table1 = m_Key[1];

        m_Key[1] = (((((table1 >> 1) | (table0 << 31)) ^ Key1) >> 1) | (table0 << 31)) ^ Key2;
        m_Key[0] = ((table0 >> 1) | (table1 << 31)) ^ Key3;
    }
}

void CLoginCrypt::Encrypt_Old(uint8_t* in, uint8_t* out, int size)
{
    for (int i = 0; i < size; i++) {
        out[i] = in[i] ^ static_cast<unsigned char>(m_Key[0]);

        unsigned int table0 = m_Key[0];
        unsigned int table1 = m_Key[1];

        m_Key[0] = ((table0 >> 1) | (table1 << 31)) ^ Key2;
        m_Key[1] = ((table1 >> 1) | (table0 << 31)) ^ Key1;
    }
}

void CLoginCrypt::Encrypt_1_25_36(uint8_t* in, uint8_t* out, int size)
{
    for (int i = 0; i < size; i++) {
        out[i] = in[i] ^ static_cast<unsigned char>(m_Key[0]);

        unsigned int table0 = m_Key[0];
        unsigned int table1 = m_Key[1];

        m_Key[0] = ((table0 >> 1) | (table1 << 31)) ^ Key2;
        m_Key[1] = ((table1 >> 1) | (table0 << 31)) ^ Key1;

        m_Key[1] = (Key1 >> ((5 * table1 * table1) & 0xFF)) + (table1 * Key1) + (table0 * table0 * 0x35ce9581) + 0x07afcc37;
        m_Key[0] = (Key2 >> ((3 * table0 * table0) & 0xFF)) + (table0 * Key2) + (m_Key[1] * m_Key[1] * 0x4c3a1353) + 0x16ef783f;
    }
}
