#ifndef MD5_H
#define MD5_H

// Struct for MD5

typedef struct tagmd5_state {
    unsigned int count[2];
    unsigned int abcd[4];
    unsigned char buf[64];
} md5_state;

class MD5Crypt {

    // Constructor / Destructor
public:
    MD5Crypt();
    ~MD5Crypt();

    // Member Functions

public:
    void Init(unsigned char* Data, unsigned int Size);
    void Encrypt(unsigned char* in, unsigned char* out, int len);
    unsigned char* GetMD5() { return Digest; }

protected:
    void process(md5_state* pms, const unsigned char* data);
    void start(md5_state* pms);
    void append(md5_state* pms, const unsigned char* data, int nbytes);
    void finish(md5_state* pms, unsigned char digest[16]);

    unsigned int TableIdx;
    unsigned char Digest[16];
};

#endif //__MD5_H__
