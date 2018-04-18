#ifndef LOGINCRYPT_H
#define LOGINCRYPT_H

#include "Wisp/WispDefinitions.h"

class CLoginCrypt {
private:
    uint32_t m_Key[2];
	uint8_t m_Seed[4];

public:
    CLoginCrypt();
    ~CLoginCrypt() {}

	uint32_t Key1 = 0;
	uint32_t Key2 = 0;
	uint32_t Key3 = 0;
	uint32_t SeedKey = 0;

    void Init(uint8_t* ps);

    void Encrypt(uint8_t* in, uint8_t* out, int size);
    void Encrypt_Old(uint8_t* in, uint8_t* out, int size);
    void Encrypt_1_25_36(uint8_t* in, uint8_t* out, int size);
};

extern CLoginCrypt g_LoginCrypt;

#endif
