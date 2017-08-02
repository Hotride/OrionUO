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
//!Класс менеджера подключения к серверу
class CSocket : public WISP_NETWORK::CConnection
{
	SETGET(bool, GameSocket, false);
	SETGET(bool, UseProxy, false);
	SETGET(string, ProxyAddress, "");
	SETGET(int, ProxyPort, 0);
	SETGET(bool, ProxySocks5, false);
	SETGET(string, ProxyAccount, "");
	SETGET(string, ProxyPassword, "");

private:
	CDecompressingCopier m_Decompressor;

public:
	CSocket(bool gameSocket);
	~CSocket();

	virtual bool Connect(const string &address, const int &port);

	virtual UCHAR_LIST Decompression(UCHAR_LIST data);
};
//---------------------------------------------------------------------------
#endif