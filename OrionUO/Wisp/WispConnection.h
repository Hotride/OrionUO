//----------------------------------------------------------------------------------
#ifndef WISPCONNECTION_H
#define WISPCONNECTION_H
#include "WispGlobal.h"
#include "WispPacketReader.h"
#include "WispPacketMessage.h"
#include <winsock.h>
#pragma comment(lib, "wsock32.lib")
//----------------------------------------------------------------------------------
namespace WISP_NETWORK
{
//----------------------------------------------------------------------------------
class CConnection
{
protected:
	SOCKET m_Socket;

	SETGET(int, DataReady);
	SETGET(bool, Connected);
	SETGET(int, Port);

	SETGET(int, Af);
	SETGET(int, Type);
	SETGET(int, Protocol);

public:
	CConnection(int af = AF_INET, int type = SOCK_STREAM, int protocol = IPPROTO_IP);
	virtual ~CConnection();

	class CPacketMessage *m_MessageParser;

	bool Connect(const string &address, const int &port);
	void Disconnect();

	bool ReadyRead();

	virtual UCHAR_LIST Decompression(UCHAR_LIST data) { return data; }

	bool Read(const int &maxSize = 0x1000);

	int Send(puchar data, const int &size);
	int Send(const UCHAR_LIST &data);
};
//----------------------------------------------------------------------------------
}; //namespace
//----------------------------------------------------------------------------------
#endif

