#if !USE_ORIONDLL
//----------------------------------------------------------------------------------
#include "LoginCrypt.h"
#include "../Wisp/WispGlobal.h"
#include "../Wisp/WispDataStream.h"
extern UCHAR_LIST g_RawData;
CLoginCrypt g_LoginCrypt;
//----------------------------------------------------------------------------------
CLoginCrypt::CLoginCrypt()
{
	memset(&m_seed, 0, sizeof(m_seed));
}
//----------------------------------------------------------------------------------
void CLoginCrypt::Init(puchar ps)
{
	memcpy(m_seed, ps, 4);
	uint seed = (ps[0] << 24) | (ps[1] << 16) | (ps[2] << 8) | ps[3];

	WISP_DATASTREAM::CDataReader reader(&g_RawData[0], (int)g_RawData.size());
	reader.Move(3);
	int len = reader.ReadUInt8();

	m_k1 = reader.ReadUInt32LE(len);
	m_k2 = reader.ReadUInt32LE(len);
	m_k3 = reader.ReadUInt32LE(len);
	uint seed_key = reader.ReadUInt16LE(len);

	m_key[0] = (((~seed) ^ seed_key) << 16) | ((seed ^ 0xffffaaaa) & 0x0000ffff);
	m_key[1] = ((seed ^ 0x43210000) >> 16) | (((~seed) ^ 0xabcdffff) & 0xffff0000);
}
//----------------------------------------------------------------------------------
void CLoginCrypt::Encrypt(puchar in, puchar out, int size)
{
	for (int i = 0; i < size; i++)
	{
		out[i] = in[i] ^ static_cast<uint8_t>(m_key[0]);

		uint table0 = m_key[0];
		uint table1 = m_key[1];

		m_key[1] = (((((table1 >> 1) | (table0 << 31)) ^ m_k1) >> 1) | (table0 << 31)) ^ m_k2;
		m_key[0] = ((table0 >> 1) | (table1 << 31)) ^ m_k3;
	}
}
//----------------------------------------------------------------------------------
void CLoginCrypt::Encrypt_Old(puchar in, puchar out, int size)
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
//----------------------------------------------------------------------------------
void CLoginCrypt::Encrypt_1_25_36(puchar in, puchar out, int size)
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
//----------------------------------------------------------------------------------
#endif
