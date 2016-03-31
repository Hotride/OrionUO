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
//!Класс менеджера подключения к серверу
class TConnectionManager
{
private:
	//!Подключение к сокету авторизации
	TConnection m_LoginSocket;

	//!Подключение к сокету сервера
	TConnection m_GameSocket;

	//!Фрагментированное сообщение
	TMessageFragment *m_LastMessageFragment;

	//!Тип шифрования
	ENCRYPTION_TYPE m_EncryptionType;

	//!Версия протокола
	CLIENT_VERSION m_ClientVersion;

	//!Шифрование для сокета авторизации
	TLoginCrypt m_LoginCrypt;

	//!Рыбы
	TBlowfishCrypt m_BlowfishCrypt;
	TTwofishCrypt m_TwoFishCrypt;

	//!Буфер декомпрессии
	TDecompressingCopier m_Decompressor;

	//!Тип сокета. true - Login, false - game
	bool m_SocketType;

public:
	TConnectionManager();
	~TConnectionManager();

	SETGET(ENCRYPTION_TYPE, EncryptionType);
	SETGET(CLIENT_VERSION, ClientVersion);

	/*!
	Инициализаци логин сокета
	@param [__in] k1 Ключ шифрования 1
	@param [__in] k2 Ключ шифрования 2
	@param [__in] k3 Ключ шифрования 3
	@param [__in] seed_key Семка для генерации шифрования
	@return Код ошибки
	*/
	int Init(__in DWORD k1, __in DWORD k2, __in DWORD k3, __in DWORD seed_key);

	/*!
	Инициализация игрового сокета
	@param [__in] GameSeed Ключ для игрового шифрования
	@return Код ошибки
	*/
	int Init(__in PBYTE GameSeed);

	/*!
	Состояние подключения
	@return true - подключено
	*/
	bool Connected() {return (m_LoginSocket.Connected() || m_GameSocket.Connected());}

	/*!
	Подключение к серверу
	@param [__in] IP IP адрес сервера
	@param [__in] Port Порт сервера
	@param [__in] GameSeed Ключ для шифрования
	@return Код ошибки
	*/
	int Connect(__in const char *IP, __in int Port, __in PBYTE GameSeed);

	/*!
	Разорвать подключение
	@return 
	*/
	void Disconnect();

	/*!
	Получить данные с сервера
	@return 
	*/
	void Recv();

	/*!
	Отправить сообщение серверу
	@param [__in] buf Буфер с данными
	@param [__in] size Размер данных
	@return Размер отправленных данных или код ошибки
	*/
	int Send(__in PBYTE buf, __in int size);

	/*!
	Получить свой IP-адрес
	@return 
	*/
	PBYTE GetClientIP() const {return (PBYTE)m_LoginCrypt.m_seed;}
};
//---------------------------------------------------------------------------
//!Менеджер подключения
extern TConnectionManager ConnectionManager;
//---------------------------------------------------------------------------
#endif