// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
/***********************************************************************************
**
** Connection.h
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#include "stdafx.h"
//----------------------------------------------------------------------------------
CSocket::CSocket(bool gameSocket)
: WISP_NETWORK::CConnection(), m_GameSocket(gameSocket)
{
}
//----------------------------------------------------------------------------------
CSocket::~CSocket()
{
}
//----------------------------------------------------------------------------------
bool CSocket::Connect(const string &address, const int &port)
{
	WISPFUN_DEBUG("c158_f1");
	LOG("Connecting...%s:%i\n", address.c_str(), port);

	if (m_UseProxy)
	{
		if (m_Connected)
			return false;

		LOG("using proxy %s:%d\n", m_ProxyAddress.c_str(), m_ProxyPort);

		if (!CConnection::Connect(m_ProxyAddress, m_ProxyPort))
		{
			LOG("Can't connect to proxy\n");
			return WISP_NETWORK::CConnection::Connect(address, port);
		}

		ushort serverPort = htons(port);
		uint serverIP = inet_addr(address.c_str());

		if (serverIP == 0xFFFFFFFF)
		{
			struct hostent *uohe = gethostbyname(address.c_str());

			if (uohe != NULL)
			{
				sockaddr_in caddr;
				memcpy(&caddr.sin_addr, uohe->h_addr, uohe->h_length);
				serverIP = caddr.sin_addr.S_un.S_addr;
			}
		}

		if (serverIP == 0xFFFFFFFF)
		{
			LOG("Unknowm server address\n");
			return WISP_NETWORK::CConnection::Connect(address, port);
		}

		if (!m_ProxySocks5)
		{
			LOG("proxy connection open\n");
			char str[9] = { 0 };
			str[0] = 4;
			str[1] = 1;
			memcpy(&str[2], &serverPort, 2);
			memcpy(&str[4], &serverIP, 4);
			::send(m_Socket, str, 9, 0);
			int recvSize = ::recv(m_Socket, str, 8, 0);

			if (recvSize != 8)
			{
				LOG("proxy error != 8\n");
				closesocket(m_Socket);
				m_Socket = INVALID_SOCKET;
				return WISP_NETWORK::CConnection::Connect(address, port);
			}
		}
		else
		{
			LOG("proxy connection open (auth)\n");
			char str[255] = { 0 };
			str[0] = 5;
			str[1] = 2;
			str[2] = 0;
			str[3] = 2;
			::send(m_Socket, str, 4, 0);
			int num = ::recv(m_Socket, str, 255, 0);
			if ((str[0] != 5) || (num != 2))
			{
				LOG("proxy error != 2\n");
				closesocket(m_Socket);
				m_Socket = INVALID_SOCKET;
				return WISP_NETWORK::CConnection::Connect(address, port);
			}
			else
			{
				if (str[1])
				{
					if (str[1] == 2)
					{
						int totalSize = 3 + (int)m_ProxyAccount.length() + (int)m_ProxyPassword.length();
						vector<char> buffer(totalSize, 0);
						sprintf(&buffer[0], "  %s %s", m_ProxyAccount.c_str(), m_ProxyPassword.c_str());
						buffer[0] = 1;
						buffer[1] = (char)m_ProxyAccount.length();
						buffer[2 + (int)m_ProxyAccount.length()] = (char)m_ProxyPassword.length();
						::send(m_Socket, &buffer[0], totalSize, 0);
						::recv(m_Socket, str, 255, 0);
						if (str[1] != 0)
						{
							LOG("proxy error != 2 (2)\n");
							closesocket(m_Socket);
							m_Socket = INVALID_SOCKET;
							return WISP_NETWORK::CConnection::Connect(address, port);
						}
					}
				}

				memset(str, 0, 10);
				str[0] = 5;
				str[1] = 1;
				str[2] = 0;
				str[3] = 1;
				memcpy(&str[4], &serverIP, 4);
				memcpy(&str[8], &serverPort, 2);
				::send(m_Socket, str, 10, 0);

				num = ::recv(m_Socket, str, 255, 0);

				if (str[1] != 0)
				{
					switch (str[1])
					{
					case 1:
						LOG("general SOCKS server failure\n");
						break;
					case 2:
						LOG("connection not allowed by ruleset\n");
						break;
					case 3:
						LOG("Network unreachable\n");
						break;
					case 4:
						LOG("Host unreachable\n");
						break;
					case 5:
						LOG("Connection refused\n");
						break;
					case 6:
						LOG("TTL expired\n");
						break;	
					case 7:
						LOG("Command not supported\n");
						break;
					case 8:
						LOG("Address type not supported\n");
						break;
					case 9:
						LOG("to X'FF' unassigned\n");
						break;
					default:
						LOG("proxy error != 10 <%d>\n", num);
					}
					
					closesocket(m_Socket);
					m_Socket = INVALID_SOCKET;
					return WISP_NETWORK::CConnection::Connect(address, port);
				}
				LOG("Connected to server via proxy\n");
				m_Connected = true;
				WSASetLastError(0);
				m_MessageParser->Clear();
			}
		}
	}
	else
		return WISP_NETWORK::CConnection::Connect(address, port);

	return true;
}
//----------------------------------------------------------------------------------
UCHAR_LIST CSocket::Decompression(UCHAR_LIST data)
{
	WISPFUN_DEBUG("c158_f2");
	if (m_GameSocket)
	{
		intptr_t inSize = (intptr_t)data.size();

		if (g_NetworkPostAction != NULL)
			g_NetworkPostAction(&data[0], &data[0], (int)inSize);

		UCHAR_LIST decBuf(inSize * 4 + 2);

		int outSize = 65536;

		m_Decompressor((char*)&decBuf[0], (char*)&data[0], outSize, inSize);

		if (inSize != data.size())
		{
			DebugMsg("decompression buffer too small\n");
			Disconnect();
		}
		else
			decBuf.resize(outSize);

		return decBuf;
	}

	return data;
}
//----------------------------------------------------------------------------------
