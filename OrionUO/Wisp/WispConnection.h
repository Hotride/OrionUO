﻿//----------------------------------------------------------------------------------
#ifndef WISPCONNECTION_H
#define WISPCONNECTION_H
#if !defined(ORION_LINUX) // FIXME: define on windows only
#include <winsock.h>
#pragma comment(lib, "wsock32.lib")
#endif
//----------------------------------------------------------------------------------
namespace WISP_NETWORK
{
//----------------------------------------------------------------------------------
class CConnection
{
protected:
	SOCKET m_Socket;

	SETGET(int, DataReady, 0);
	SETGET(bool, Connected, 0);
	SETGET(int, Port, 0);

	SETGET(int, Af, 0);
	SETGET(int, Type, 0);
	SETGET(int, Protocol, 0);

public:
	CConnection(int af = AF_INET, int type = SOCK_STREAM, int protocol = IPPROTO_IP);
	virtual ~CConnection();

	class CPacketMessage *m_MessageParser;

	virtual bool Connect(const string &address, const int &port);
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

