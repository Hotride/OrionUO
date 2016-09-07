//---------------------------------------------------------------------------
#ifndef LoginCryptH
#define LoginCryptH
//---------------------------------------------------------------------------
#include <windows.h>
//---------------------------------------------------------------------------
class CLoginCrypt
{
private:
	DWORD m_k1;
	DWORD m_k2;
	DWORD m_k3;
	DWORD m_key[2];

public:
	CLoginCrypt();
	~CLoginCrypt() {}

	BYTE m_seed[4];

	void Init(PBYTE ps, DWORD k1, DWORD k2, DWORD k3, DWORD seed_key);

	void Encrypt(PBYTE in, PBYTE out, int size);
	void Encrypt_Old(PBYTE in, PBYTE out, int size);
	void Encrypt_1_25_36(PBYTE in, PBYTE out, int size);
};
//---------------------------------------------------------------------------
#endif