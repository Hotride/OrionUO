//---------------------------------------------------------------------------
#ifndef GameCryptH
#define GameCryptH
#include <windows.h>
#include "md5.h"
//---------------------------------------------------------------------------
#define CRYPT_AUTO_VALUE        0x80

#define CRYPT_GAMEKEY_LENGTH    6
#define CRYPT_GAMEKEY_COUNT     25

#define CRYPT_GAMESEED_LENGTH   8
#define CRYPT_GAMESEED_COUNT    25

#define CRYPT_GAMETABLE_START   1
#define CRYPT_GAMETABLE_STEP    3
#define CRYPT_GAMETABLE_MODULO  11
#define CRYPT_GAMETABLE_TRIGGER 21036
//---------------------------------------------------------------------------
class TBlowfishCrypt
{
private:
	static bool m_tables_ready;

	unsigned char m_seed[CRYPT_GAMESEED_LENGTH];
	int m_table_index;
	int m_block_pos;
	int m_stream_pos;

	void InitTables();
	void RawEncrypt(PDWORD values, int table);
public:
	TBlowfishCrypt() {}
	~TBlowfishCrypt() {}

	void Encrypt(PBYTE in, PBYTE out, int len);
	void Init();
};
//---------------------------------------------------------------------------
class TTwofishCrypt
{
private:
	unsigned long m_IP;
	int m_pos;
	keyInstance ki;
	cipherInstance ci;
	DWORD dwIndex;
	BYTE m_subData3[256];
	MD5Crypt *m_md5;
	bool m_use_md5;
	BYTE sm_bData[16];

public:
	TTwofishCrypt() {}
	~TTwofishCrypt() {}
	
	void Init(BYTE IP[4]);
	void Init_MD5();
	
	void Encrypt(PBYTE in, PBYTE out, int size);
	void Decrypt(PBYTE in, PBYTE out, int size);
};
//---------------------------------------------------------------------------
#endif