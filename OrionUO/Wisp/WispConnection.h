#pragma once
#include "../Sockets.h"
namespace WISP_NETWORK
{
class CConnection
{
public:
    tcp_socket m_Socket = nullptr;

    int DataReady = 0;
    int Port = 0;

    bool Connected = 0;

    CConnection();
    virtual ~CConnection();

    class CPacketMessage *m_MessageParser;

    virtual bool Connect(const string &address, uint16_t port);
    void Disconnect();

    bool ReadyRead();

    virtual UCHAR_LIST Decompression(UCHAR_LIST data) { return data; }

    bool Read(int maxSize = 0x1000);

    int Send(puchar data, int size);
    int Send(const UCHAR_LIST &data);
};
}; // namespace WISP_NETWORK