﻿// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
//----------------------------------------------------------------------------------
#include "stdafx.h"

namespace WISP_NETWORK
{
//----------------------------------------------------------------------------------
CConnection::CConnection(int af, int type, int protocol)
: Af(af), Type(type), Protocol(protocol)
{
	WISPFUN_DEBUG("c3_f1");
	WSADATA wsaData = { 0 };

	if (!WSAStartup(MAKEWORD(2, 2), &wsaData))
		m_Socket = socket(af, type, protocol);
	else
		m_Socket = INVALID_SOCKET;

	m_MessageParser = new CPacketMessage();
}
//----------------------------------------------------------------------------------
CConnection::~CConnection()
{
	WISPFUN_DEBUG("c3_f2");
	Disconnect();

	if (m_MessageParser != NULL)
	{
		delete m_MessageParser;
		m_MessageParser = NULL;
	}
}
//----------------------------------------------------------------------------------
bool CConnection::Connect(const string &address, int port)
{
	WISPFUN_DEBUG("c3_f3");
	if (Connected)
		return false;
	else if (m_Socket == INVALID_SOCKET)
	{
		m_Socket = socket(Af, Type, Protocol);

		if (m_Socket == INVALID_SOCKET)
			return false;
	}

	sockaddr_in caddr;
	memset(&caddr, 0, sizeof(sockaddr_in));
	caddr.sin_family = AF_INET;

	struct hostent *he;

	int rt = inet_addr(address.c_str());

	if (rt != -1)
		caddr.sin_addr.s_addr = rt;
	else
	{
		he = gethostbyname(address.c_str());

		if (he == NULL)
			return false;

		memcpy(&caddr.sin_addr, he->h_addr, he->h_length);
	}

	caddr.sin_port = htons(port);

	if (connect(m_Socket, (struct sockaddr*)&caddr, sizeof(caddr)) == -1)
		return false;

	Port = port;
	Connected = true;
	WSASetLastError(0);
	m_MessageParser->Clear();

	return true;
}
//----------------------------------------------------------------------------------
void CConnection::Disconnect()
{
	WISPFUN_DEBUG("c3_f4");
	if (Connected && m_Socket != INVALID_SOCKET)
	{
		closesocket(m_Socket);

		m_Socket = INVALID_SOCKET;
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
	if (!Connected || m_Socket == INVALID_SOCKET)
		return false;

	fd_set rfds;
	struct timeval tv = { 0, 0 };
	FD_ZERO(&rfds);
	FD_SET(m_Socket, &rfds);

	DataReady = select((int)m_Socket, &rfds, NULL, NULL, &tv);

	if (DataReady == SOCKET_ERROR)
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
	if (DataReady == SOCKET_ERROR)
	{
		LOG("CConnection::Read, m_DataReady=%i\n", DataReady);
		Disconnect();
	}
	else if (Connected && m_Socket != INVALID_SOCKET)
	{
		UCHAR_LIST data(maxSize);
		int size = recv(m_Socket, (char*)&data[0], maxSize, 0);

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
		LOG("CConnection::Read, unknown state, m_Connected=%i, m_Socket=%i\n", Connected, m_Socket);

	return false;
}
//----------------------------------------------------------------------------------
int CConnection::Send(puchar data, int size)
{
	WISPFUN_DEBUG("c3_f7");
	if (!Connected || m_Socket == INVALID_SOCKET)
		return 0;

	int sent = send(m_Socket, (char*)data, size, 0);

	//LOG("CConnection::Send=>%i\n", sent);

	return sent;
}
//----------------------------------------------------------------------------------
int CConnection::Send(const UCHAR_LIST &data)
{
	WISPFUN_DEBUG("c3_f8");
	if (data.size())
	{
		int sent = Send((puchar)&data[0], (int)data.size());

		//LOG("CConnection::Send=>%i\n", sent);

		return sent;
	}
	
	return 0;
}
//----------------------------------------------------------------------------------
}; //namespace
//----------------------------------------------------------------------------------
