/****************************************************************************
**
** ConnectionManager.h
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
#ifndef ConnectionManagerH
#define ConnectionManagerH
//---------------------------------------------------------------------------
class TConnectionManager
{
private:
	TConnection m_LoginSocket;
	TConnection m_GameSocket;

	TMessageFragment *m_LastMessageFragment;

	ENCRYPTION_TYPE m_EncryptionType;
	CLIENT_VERSION m_ClientVersion;

	TLoginCrypt m_LoginCrypt;

	TBlowfishCrypt m_BlowfishCrypt;
	TTwofishCrypt m_TwoFishCrypt;

	TDecompressingCopier m_Decompressor;

	bool m_SocketType; //true - Login, false - game

public:
	TConnectionManager();
	~TConnectionManager();
	
	int Init(DWORD k1, DWORD k2, DWORD k3, DWORD seed_key); //Login
	int Init(PBYTE GameSeed); //Game

	SETGET(ENCRYPTION_TYPE, EncryptionType);
	SETGET(CLIENT_VERSION, ClientVersion);

	bool Connected() {return (m_LoginSocket.Connected() || m_GameSocket.Connected());}

	int Connect(const char *IP, int Port, PBYTE GameSeed);
	void Disconnect();

	void Recv();
	int Send(PBYTE buf, int size);

	PBYTE GetClientIP() const {return (PBYTE)m_LoginCrypt.m_seed;}
};

extern TConnectionManager ConnectionManager;
//---------------------------------------------------------------------------
#endif