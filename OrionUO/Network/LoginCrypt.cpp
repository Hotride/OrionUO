//---------------------------------------------------------------------------
#include "LoginCrypt.h"
//---------------------------------------------------------------------------
CLoginCrypt::CLoginCrypt()
{
	memset(&m_seed, 0, sizeof(m_seed));
}
//---------------------------------------------------------------------------
void CLoginCrypt::Init(PBYTE ps, DWORD k1, DWORD k2, DWORD k3, DWORD seed_key)
{
	memcpy(m_seed, ps, 4);
	DWORD seed = (ps[0] << 24) | (ps[1] << 16) | (ps[2] << 8) | ps[3];

	m_k1 = k1;
	m_k2 = k2;
	m_k3 = k3;

	m_key[0]= (((~seed) ^ seed_key) << 16) | ((seed ^ 0xffffaaaa) & 0x0000ffff);
#if UO_FW_SHARD == 1
	m_key[1]= ((seed ^ 0x12340000) >> 16) | (((~seed) ^ 0xabcdffff) & 0xffff0000);
#else
	m_key[1]= ((seed ^ 0x43210000) >> 16) | (((~seed) ^ 0xabcdffff) & 0xffff0000);
#endif
}
//---------------------------------------------------------------------------
void CLoginCrypt::Encrypt(PBYTE in, PBYTE out, int size)
{
	for (int i = 0; i < size; i++)
	{
		out[i] = in[i] ^ static_cast<BYTE>(m_key[0]);

		DWORD table0 = m_key[0];
		DWORD table1 = m_key[1];

		m_key[1] = (((((table1 >> 1) | (table0 << 31)) ^ m_k1) >> 1) | (table0 << 31)) ^ m_k2;
		m_key[0] = ((table0 >> 1) | (table1 << 31)) ^ m_k3;
	}
}
//---------------------------------------------------------------------------
void CLoginCrypt::Encrypt_Old(PBYTE in, PBYTE out, int size)
{
    for (int i = 0; i < size; i++)
    {
        out[i] = in[i] ^ static_cast<unsigned char>(m_key[0]);

        unsigned int table0 = m_key[0];
        unsigned int table1 = m_key[1];
		
        m_key[0] = ((table0 >> 1) | (table1 << 31)) ^ m_k2;
        m_key[1] = ((table1 >> 1) | (table0 << 31)) ^ m_k1;
    }
}
//---------------------------------------------------------------------------
void CLoginCrypt::Encrypt_1_25_36(PBYTE in, PBYTE out, int size)
{
    for (int i = 0; i < size; i++)
    {
        out[i] = in[i] ^ static_cast<unsigned char>(m_key[0]);

        unsigned int table0 = m_key[0];
        unsigned int table1 = m_key[1];
		
        m_key[0] = ((table0 >> 1) | (table1 << 31)) ^ m_k2;
        m_key[1] = ((table1 >> 1) | (table0 << 31)) ^ m_k1;

		m_key[1] = (m_k1 >> ((5 * table1 * table1) & 0xFF)) + (table1 * m_k1) + (table0 * table0 * 0x35ce9581) + 0x07afcc37;
		m_key[0] = (m_k2 >> ((3 * table0 * table0) & 0xFF)) + (table0 * m_k2) + (m_key[1] * m_key[1] * 0x4c3a1353) + 0x16ef783f;
    }
}
//---------------------------------------------------------------------------
