/****************************************************************************
**
** Connection.h
**
** Copyright (C) September 2015 Hotride
**
** This program is free software; you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation; either version 2 of the License, or
** (at your option) any later version.
**
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**
*****************************************************************************
*/
//---------------------------------------------------------------------------
#ifndef ConnectionH
#define ConnectionH
//---------------------------------------------------------------------------
#define SOCK_RECV_BUF_SIZE 0x1000
//---------------------------------------------------------------------------
class TConnection
{
private:
	SOCKET m_Socket;
	int m_ReadyData;

	bool m_Connected;

public:
	TConnection();
	~TConnection();

	bool Connected() const {return m_Connected;}

	int Init();

	int Connect(const char *IP, int Port);
	void Disconnect();

	bool DataReady();

	int Recv(PBYTE buf, int &size);
	int Send(PBYTE buf, int size);
};
//---------------------------------------------------------------------------
#endif