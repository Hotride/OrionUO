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
//Менеджер подключения к серверу
class TConnectionManager
{
private:
	//Подключение к сокету авторизации
	TConnection m_LoginSocket;

	//Подключение к сокету сервера
	TConnection m_GameSocket;

	//Фрагментированное сообщение
	TMessageFragment *m_LastMessageFragment;

	//Тип шифрования
	ENCRYPTION_TYPE m_EncryptionType;

	//Версия протокола
	CLIENT_VERSION m_ClientVersion;

	//Шифрование для сокета авторизации
	TLoginCrypt m_LoginCrypt;

	//Рыбы
	TBlowfishCrypt m_BlowfishCrypt;
	TTwofishCrypt m_TwoFishCrypt;

	//Буфер декомпрессии
	TDecompressingCopier m_Decompressor;

	//Тип сокета. true - Login, false - game
	bool m_SocketType;

public:
	TConnectionManager();
	~TConnectionManager();

	//Инициализаци логин сокета
	int Init(DWORD k1, DWORD k2, DWORD k3, DWORD seed_key);

	//Инициализация игрового сокета
	int Init(PBYTE GameSeed);

	SETGET(ENCRYPTION_TYPE, EncryptionType);
	SETGET(CLIENT_VERSION, ClientVersion);

	//Состояние подключения
	bool Connected() {return (m_LoginSocket.Connected() || m_GameSocket.Connected());}

	//Подключение к серверу
	int Connect(const char *IP, int Port, PBYTE GameSeed);

	//Разорвать подключение
	void Disconnect();

	//Получить данные с сервера
	void Recv();

	//Отправить сообщение серверу
	int Send(PBYTE buf, int size);

	//Получить свой ИП
	PBYTE GetClientIP() const {return (PBYTE)m_LoginCrypt.m_seed;}
};
//---------------------------------------------------------------------------
extern TConnectionManager ConnectionManager;
//---------------------------------------------------------------------------
#endif