// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
//----------------------------------------------------------------------------------
#include "WispConnection.h"
#include "WispDefinitions.h"
#include "WispPacketMessage.h"

namespace WISP_NETWORK
{
//----------------------------------------------------------------------------------
CConnection::CConnection()
{
    WISPFUN_DEBUG("c3_f1");

    m_Socket = tcp_open();
    m_MessageParser = new CPacketMessage();
}
//----------------------------------------------------------------------------------
CConnection::~CConnection()
{
    WISPFUN_DEBUG("c3_f2");
    Disconnect();

    if (m_MessageParser != nullptr)
    {
        delete m_MessageParser;
        m_MessageParser = nullptr;
    }
}
//----------------------------------------------------------------------------------
bool CConnection::Connect(const string &address, uint16_t port)
{
    WISPFUN_DEBUG("c3_f3");
    if (Connected)
        return false;

    if (m_Socket == nullptr)
        m_Socket = tcp_open();

    if (m_Socket == nullptr)
        return false;

    if (!tcp_connect(m_Socket, address.c_str(), port))
        return false;

    Port = port;
    Connected = true;
    m_MessageParser->Clear();

    return true;
}
//----------------------------------------------------------------------------------
void CConnection::Disconnect()
{
    WISPFUN_DEBUG("c3_f4");
    if (Connected && m_Socket != nullptr)
    {
        tcp_close(m_Socket);
        m_Socket = nullptr;
        Connected = false;
        DataReady = 0;
        Port = 0;
        m_MessageParser->Clear();
    }
}
//----------------------------------------------------------------------------------
bool CConnection::ReadyRead()
{
    WISPFUN_DEBUG("c3_f5");
    if (!Connected || m_Socket == nullptr)
        return false;

    DataReady = tcp_select(m_Socket);
    if (DataReady == -1)
    {
        LOG("CConnection::ReadyRead SOCKET_ERROR\n");
        Disconnect();
    }

    return (DataReady != 0);
}
//----------------------------------------------------------------------------------
bool CConnection::Read(int maxSize)
{
    WISPFUN_DEBUG("c3_f6");
    if (DataReady == -1)
    {
        LOG("CConnection::Read, m_DataReady=%i\n", DataReady);
        Disconnect();
    }
    else if (Connected && m_Socket != nullptr)
    {
        UCHAR_LIST data(maxSize);
        const int size = tcp_recv(m_Socket, &data[0], maxSize);

        if (size > 0)
        {
            //LOG("CConnection::Read size=%i\n", size);
            data.resize(size);
            data = Decompression(data);
            m_MessageParser->Append(data);
            return true;
        }
        else
            LOG("CConnection::Read, bad size=%i\n", size);
    }
    else
        LOG("CConnection::Read, unknown state, m_Connected=%i\n", Connected);

    return false;
}
//----------------------------------------------------------------------------------
int CConnection::Send(puchar data, int size)
{
    WISPFUN_DEBUG("c3_f7");
    if (!Connected || m_Socket == nullptr)
        return 0;

    const int sent = tcp_send(m_Socket, data, size);
    //LOG("CConnection::Send=>%i\n", sent);
    return sent;
}
//----------------------------------------------------------------------------------
int CConnection::Send(const UCHAR_LIST &data)
{
    WISPFUN_DEBUG("c3_f8");
    if (!data.size())
        return 0;

    const int sent = Send((puchar)&data[0], (int)data.size());
    LOG("CConnection::Send=>%i\n", sent);
    return sent;
}
}; // namespace WISP_NETWORK
