/* aes.h */

/* ---------- See examples at end of this file for typical usage -------- */

/* AES Cipher header file for ANSI C Submissions  
    Lawrence E. Bassham III
    Computer Security Division
    National Institute of Standards and Technology

    This sample is to assist implementers developing to the
Cryptographic API Profile for AES Candidate Algorithm Submissions.
Please consult this document as a cross-reference.
    
    ANY CHANGES, WHERE APPROPRIATE, TO INFORMATION PROVIDED IN THIS FILE
MUST BE DOCUMENTED. CHANGES ARE ONLY APPROPRIATE WHERE SPECIFIED WITH
THE STRING "CHANGE POSSIBLE". FUNCTION CALLS AND THEIR PARAMETERS
CANNOT BE CHANGED. STRUCTURES CAN BE ALTERED TO ALLOW IMPLEMENTERS TO
INCLUDE IMPLEMENTATION SPECIFIC INFORMATION.
*/

/* Includes:
    Standard include files
*/
#pragma once

#include <stdio.h>
#include "platform.h"

/*  Defines:
        Add any additional defines you need
*/

#define DIR_ENCRYPT 0 /* Are we encrpyting? */
#define DIR_DECRYPT 1 /* Are we decrpyting? */
#define MODE_ECB 1    /* Are we ciphering in ECB mode? */
#define MODE_CBC 2    /* Are we ciphering in CBC mode? */
#define MODE_CFB1 3   /* Are we ciphering in 1-bit CFB mode? */

#define TRUE 1
#define FALSE 0

#define BAD_KEY_DIR -1      /* Key direction is invalid (unknown value) */
#define BAD_KEY_MAT -2      /* Key material not of correct length */
#define BAD_KEY_INSTANCE -3 /* Key passed is not valid */
#define BAD_CIPHER_MODE -4  /* Params struct passed to cipherInit invalid */
#define BAD_CIPHER_STATE -5 /* Cipher in wrong state (e.g., not initialized) */

/* CHANGE POSSIBLE: inclusion of algorithm specific defines */
/* TWOFISH specific definitions */
#define MAX_KEY_SIZE 64  /* # of ASCII chars needed to represent a key */
#define MAX_IV_SIZE 16   /* # of u8s needed to represent an IV */
#define BAD_INPUT_LEN -6 /* inputLen not a multiple of block size */
#define BAD_PARAMS -7    /* invalid parameters */
#define BAD_IV_MAT -8    /* invalid IV text */
#define BAD_ENDIAN -9    /* incorrect endianness define */
#define BAD_ALIGN32 -10  /* incorrect 32-bit alignment */

#define BLOCK_SIZE 128       /* number of bits per block */
#define MAX_ROUNDS 16        /* max # rounds (for allocating subkey array) */
#define ROUNDS_128 16        /* default number of rounds for 128-bit keys*/
#define ROUNDS_192 16        /* default number of rounds for 192-bit keys*/
#define ROUNDS_256 16        /* default number of rounds for 256-bit keys*/
#define MAX_KEY_BITS 256     /* max number of bits of key */
#define MIN_KEY_BITS 128     /* min number of bits of key (zero pad) */
#define VALID_SIG 0x48534946 /* initialization signature ('FISH') */
#define MCT_OUTER 400        /* MCT outer loop */
#define MCT_INNER 10000      /* MCT inner loop */
#define REENTRANT 1          /* nonzero forces reentrant code (slightly slower) */

#define INPUT_WHITEN 0 /* subkey array indices */
#define OUTPUT_WHITEN (INPUT_WHITEN + BLOCK_SIZE / 32)
#define ROUND_SUBKEYS (OUTPUT_WHITEN + BLOCK_SIZE / 32) /* use 2 * (# rounds) */
#define TOTAL_SUBKEYS (ROUND_SUBKEYS + 2 * MAX_ROUNDS)

/* Typedefs:
    Typedef'ed data storage elements. Add any algorithm specific
    parameters at the bottom of the structs as appropriate.
*/

typedef unsigned char u8;
typedef unsigned long u32; /* 32-bit unsigned quantity */
typedef u32 fullSbox[4][256];

/* The structure for key information */
typedef struct
{
    u8 direction; /* Key used for encrypting or decrypting? */
#if ALIGN32
    u8 dummyAlign[3]; /* keep 32-bit alignment */
#endif
    int keyLen;                         /* Length of the key */
    char keyMaterial[MAX_KEY_SIZE + 4]; /* Raw key data in ASCII */

    /* Twofish-specific parameters: */
    u32 keySig;                      /* set to VALID_SIG by makeKey() */
    int numRounds;                   /* number of rounds in cipher */
    u32 key32[MAX_KEY_BITS / 32];    /* actual key bits, in u32s */
    u32 sboxKeys[MAX_KEY_BITS / 64]; /* key bits used for S-boxes */
    u32 subKeys[TOTAL_SUBKEYS];      /* round subkeys, input/output whitening bits */
#if REENTRANT
    fullSbox sBox8x32; /* fully expanded S-box */
#if defined(COMPILE_KEY) && defined(USE_ASM)
#undef VALID_SIG
#define VALID_SIG 0x504D4F43 /* 'COMP':  C is compiled with -DCOMPILE_KEY */
    u32 cSig1;               /* set after first "compile" (zero at "init") */
    void *encryptFuncPtr;    /* ptr to asm encrypt function */
    void *decryptFuncPtr;    /* ptr to asm decrypt function */
    u32 codeSize;            /* size of compiledCode */
    u32 cSig2;               /* set after first "compile" */
    u8 compiledCode[5000];   /* make room for the code itself */
#endif
#endif
} keyInstance;

/* The structure for cipher information */
typedef struct
{
    u8 mode; /* MODE_ECB, MODE_CBC, or MODE_CFB1 */
#if ALIGN32
    u8 dummyAlign[3]; /* keep 32-bit alignment */
#endif
    u8 IV[MAX_IV_SIZE]; /* CFB1 iv u8s  (CBC uses iv32) */

    /* Twofish-specific parameters: */
    u32 cipherSig;             /* set to VALID_SIG by cipherInit() */
    u32 iv32[BLOCK_SIZE / 32]; /* CBC IV u8s arranged as u32s */
} cipherInstance;

/* Function protoypes */
int makeKey(keyInstance *key, u8 direction, int keyLen, char *keyMaterial);

int cipherInit(cipherInstance *cipher, u8 mode, char *IV);

int blockEncrypt(cipherInstance *cipher, keyInstance *key, u8 *input, int inputLen, u8 *outBuffer);

int blockDecrypt(cipherInstance *cipher, keyInstance *key, u8 *input, int inputLen, u8 *outBuffer);

int reKey(keyInstance *key); /* do key schedule using modified key.keyu32s */

/* API to check table usage, for use in ECB_TBL KAT */
#define TAB_DISABLE 0
#define TAB_ENABLE 1
#define TAB_RESET 2
#define TAB_QUERY 3
#define TAB_MIN_QUERY 50
int TableOp(int op);

#ifndef CONST
#define CONST /* helpful C++ syntax sugar, NOP for ANSI C */
#endif

#if BLOCK_SIZE == 128 /* optimize block copies */
#define Copy1(d, s, N) ((u32 *)(d))[N] = ((u32 *)(s))[N]
#define BlockCopy(d, s)                                                                            \
    {                                                                                              \
        Copy1(d, s, 0);                                                                            \
        Copy1(d, s, 1);                                                                            \
        Copy1(d, s, 2);                                                                            \
        Copy1(d, s, 3);                                                                            \
    }
#else
#define BlockCopy(d, s)                                                                            \
    {                                                                                              \
        memcpy(d, s, BLOCK_SIZE / 8);                                                              \
    }
#endif