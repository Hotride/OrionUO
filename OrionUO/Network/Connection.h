/***********************************************************************************
**
** Connection.h
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#ifndef CONNECTION_H
#define CONNECTION_H
//----------------------------------------------------------------------------------
#include "../Wisp/WispConnection.h"
#include "UOHuffman.h"
//----------------------------------------------------------------------------------
//!Класс менеджера подключения к серверу
class CSocket : public WISP_NETWORK::CConnection
{
	SETGET(bool, GameSocket);

private:
	CDecompressingCopier m_Decompressor;

public:
	CSocket(bool gameSocket);
	~CSocket();

	virtual UCHAR_LIST Decompression(UCHAR_LIST data);
};
//---------------------------------------------------------------------------
#endif