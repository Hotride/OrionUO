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
		else if (m_Socket == INVALID_SOCKET)
		{
			m_Socket = socket(m_Af, m_Type, m_Protocol);

			if (m_Socket == INVALID_SOCKET)
				return false;
		}



		sockaddr_in caddr;
		memset(&caddr, 0, sizeof(sockaddr_in));
		caddr.sin_family = AF_INET;

		struct hostent *uohe;

		int rt = inet_addr(address.c_str());

		if (rt != -1)
			caddr.sin_addr.s_addr = rt;
		else
		{
			uohe = gethostbyname(address.c_str());

			if (uohe == NULL)
				return false;

			memcpy(&caddr.sin_addr, uohe->h_addr, uohe->h_length);
		}

		caddr.sin_port = htons(port);



		struct sockaddr_in sin;
		struct hostent *he;

		memset(&sin, 0, sizeof(sin));
		sin.sin_family = AF_INET;
		sin.sin_port = htons(m_ProxyPort);

		LOG("using proxy %s:%d\n", m_ProxyAddress.c_str(), m_ProxyPort);

		int r = inet_addr(m_ProxyAddress.c_str());

		if (r != -1)
			sin.sin_addr.s_addr = r;
		else
		{
			he = gethostbyname(m_ProxyAddress.c_str());

			if (he == NULL)
				return false;

			memcpy(&sin.sin_addr, he->h_addr, he->h_length);
		}

		if (connect(m_Socket, (struct sockaddr*)&sin, sizeof(sin)) == -1)
			return WISP_NETWORK::CConnection::Connect(address, port);

		if (!m_ProxySocks5)
		{
			LOG("proxy connection open\n");
			char str[9] = { 0 };
			str[0] = 4;
			str[1] = 1;
			memcpy(&str[2], &caddr.sin_port, 2);
			memcpy(&str[4], &caddr.sin_addr.S_un.S_addr, 4);
			::send(m_Socket, str, 9, 0);

			if (::recv(m_Socket, str, 8, 0) != 8)
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
			char str[10] = { 0 };
			str[0] = 5;
			str[1] = 2;
			str[2] = 2;
			::send(m_Socket, str, 4, 0);

			if (::recv(m_Socket, str, 2, 0) != 2)
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
						int totalSize = 3 + m_ProxyAccount.length() + m_ProxyPassword.length();
						vector<char> buffer(totalSize, 0);
						sprintf(&buffer[0], "  %s %s", m_ProxyAccount.c_str(), m_ProxyPassword.c_str());
						buffer[0] = 1;
						buffer[1] = m_ProxyAccount.length();
						buffer[2 + m_ProxyAccount.length()] = m_ProxyPassword.length();
						::send(m_Socket, &buffer[0], totalSize, 0);

						if (::recv(m_Socket, str, 2, 0) != 2)
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
				memcpy(&str[4], &caddr.sin_addr.S_un.S_addr, 4);
				memcpy(&str[8], &caddr.sin_port, 2);
				::send(m_Socket, str, 10, 0);

				int num = ::recv(m_Socket, str, 10, 0);

				if (num != 10)
				{
					LOG("proxy error != 10 <%d>\n", num);
					closesocket(m_Socket);
					m_Socket = INVALID_SOCKET;
					return WISP_NETWORK::CConnection::Connect(address, port);
				}
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
		int inSize = data.size();

		if (g_NetworkPostAction != NULL)
			g_NetworkPostAction(&data[0], &data[0], inSize);

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
