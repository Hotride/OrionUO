//----------------------------------------------------------------------------------
#ifndef WISPCONNECTION_H
#define WISPCONNECTION_H
#include <winsock.h>
#pragma comment(lib, "wsock32.lib")
//----------------------------------------------------------------------------------
namespace WISP_NETWORK
{
//----------------------------------------------------------------------------------
class CConnection
{
public:
	SOCKET m_Socket;

	int DataReady = 0;
	int Port = 0;

	int Af = 0;
	int Type = 0;
	int Protocol = 0;

	bool Connected = 0;

	CConnection(int af = AF_INET, int type = SOCK_STREAM, int protocol = IPPROTO_IP);
	virtual ~CConnection();

	class CPacketMessage *m_MessageParser;

	virtual bool Connect(const string &address, int port);
	void Disconnect();

	bool ReadyRead();

	virtual UCHAR_LIST Decompression(UCHAR_LIST data) { return data; }

	bool Read(int maxSize = 0x1000);

	int Send(puchar data, int size);
	int Send(const UCHAR_LIST &data);
};
//----------------------------------------------------------------------------------
}; //namespace
//----------------------------------------------------------------------------------
#endif

