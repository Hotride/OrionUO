// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
/***********************************************************************************
**
** PingThread.cpp
**
** Copyright (C) December 2017 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#include "stdafx.h"
//----------------------------------------------------------------------------------
typedef struct tagICMPHDR
{
	u_char Type;
	u_char Code;
	u_short Checksum;
	u_short ID;
	u_short Seq;
	char Data;
} ICMPHDR, *PICMPHDR;
//----------------------------------------------------------------------------------
typedef struct tagECHOREQUEST
{
	ICMPHDR icmpHdr;
	DWORD dwTime;
	char cData[64];
} ECHOREQUEST, *PECHOREQUEST;
//----------------------------------------------------------------------------------
typedef struct tagIPHDR
{
	u_char VIHL;
	u_char TOS;
	short TotLen;
	short ID;
	short FlagOff;
	u_char TTL;
	u_char Protocol;
	u_short Checksum;
	struct in_addr iaSrc;
	struct in_addr iaDst;
} IPHDR, *PIPHDR;
//----------------------------------------------------------------------------------
typedef struct tagECHOREPLY
{
	IPHDR ipHdr;
	ECHOREQUEST echoRequest;
	char cFiller[256];
} ECHOREPLY, *PECHOREPLY;
//----------------------------------------------------------------------------------
CPingThread::CPingThread(const uint &serverID, const string &serverIP, const int &requestsCount)
: WISP_THREAD::CThread(), m_ServerID(serverID), m_ServerIP(serverIP),
m_RequestsCount(requestsCount)
{
	WISPFUN_DEBUG("");
}
//----------------------------------------------------------------------------------
CPingThread::~CPingThread()
{
	WISPFUN_DEBUG("");
}
//----------------------------------------------------------------------------------
ushort CPingThread::CalculateChecksum(pushort addr, int count)
{
	uint checksum = 0;

	while (count > 1)
	{
		checksum += *addr++;
		count -= 2;
	}

	if (count > 0)
		checksum += *(puchar)addr;

	while (checksum >> 16)
		checksum = (checksum & 0xffff) + (checksum >> 16);

	return ~checksum;
}
//----------------------------------------------------------------------------------
int CPingThread::CalculatePing()
{
	int result = 0;

	WSADATA wsa;

	if (!::WSAStartup(MAKEWORD(2, 2), &wsa))
	{
		SOCKET socket = ::socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);

		if (socket != SOCKET_ERROR)
		{
			LPHOSTENT lpHost = gethostbyname(m_ServerIP.c_str());

			if (lpHost != NULL)
			{
				sockaddr_in destAddress;

				destAddress.sin_addr.s_addr = ((in_addr*)lpHost->h_addr_list[0])->s_addr;
				destAddress.sin_family = AF_INET;
				destAddress.sin_port = 0;

				const int ICMP_ECHOREQ = 8;

				ECHOREQUEST request = { 0 };

				request.icmpHdr.Type = ICMP_ECHOREQ;
				request.dwTime = timeGetTime();
				memset(request.cData, 80, 64);
				request.icmpHdr.Checksum = CalculateChecksum((pushort)&request, sizeof(ECHOREQUEST));

				::sendto(socket, (LPSTR)&request, sizeof(ECHOREQUEST), 0, (LPSOCKADDR)&destAddress, sizeof(SOCKADDR_IN));

				timeval tomeoutInfo;
				fd_set readfds;
				readfds.fd_count = 1;
				readfds.fd_array[0] = socket;
				tomeoutInfo.tv_sec = 1;
				tomeoutInfo.tv_usec = 0;

				if (::select(1, &readfds, NULL, NULL, &tomeoutInfo))
				{
					ECHOREPLY answer;
					sockaddr_in sourceAddress;
					int length = sizeof(sockaddr_in);

					if (::recvfrom(socket, (LPSTR)&answer, sizeof(ECHOREPLY), 0, (LPSOCKADDR)&sourceAddress, &length) != SOCKET_ERROR)
						result = timeGetTime() - answer.echoRequest.dwTime;
					else
						result = -1;
				}
				else
					result = -2;
			}
			else
				result = - 3;

			closesocket(socket);
		}
		else
			result = - 4;

		WSACleanup();
	}
	else
		result = - 5;

	return result;
}
//----------------------------------------------------------------------------------
void CPingThread::OnExecute(uint nowTime)
{
	WISPFUN_DEBUG("");

	if (m_ServerIP.empty() || m_RequestsCount < 1)
		return;

	PING_INFO_DATA info = { m_ServerID, 9999, 0, 0, 0 };

	IFOR(i, 0, m_RequestsCount)
	{
		int ping = CalculatePing();

		if (ping < 0)
		{
			if (ping == -1)
				info.Lost++;

			continue;
		}

		info.Min = min(info.Min, ping);
		info.Max = max(info.Max, ping);

		info.Average += (info.Max - info.Min);
	}

	info.Average = info.Min + (info.Average / m_RequestsCount);

	SendMessage(g_OrionWindow.Handle, MessageID, (WPARAM)&info, 0);
}
//----------------------------------------------------------------------------------
