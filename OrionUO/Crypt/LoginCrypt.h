//----------------------------------------------------------------------------------
#ifndef LOGINCRYPT_H
#define LOGINCRYPT_H
//----------------------------------------------------------------------------------
#include "../Wisp/WispGlobal.h"
//----------------------------------------------------------------------------------
class CLoginCrypt
{
private:
    uint m_k1;
    uint m_k2;
    uint m_k3;
    uint m_key[2];

public:
    CLoginCrypt();
    ~CLoginCrypt() {}

    uchar m_seed[4];

    void Init(puchar ps);

    void Encrypt(puchar in, puchar out, int size);
    void Encrypt_Old(puchar in, puchar out, int size);
    void Encrypt_1_25_36(puchar in, puchar out, int size);
};
//----------------------------------------------------------------------------------
extern CLoginCrypt g_LoginCrypt;
//----------------------------------------------------------------------------------
#endif
//----------------------------------------------------------------------------------
