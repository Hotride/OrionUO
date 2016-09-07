/***********************************************************************************
**
** Connection.h
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#include "Connection.h"
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
UCHAR_LIST CSocket::Decompression(UCHAR_LIST data)
{
	if (m_GameSocket)
	{
		UCHAR_LIST decBuf(data.size() * 4 + 2);

		int inSize = data.size();
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
