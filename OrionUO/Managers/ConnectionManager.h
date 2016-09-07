/***********************************************************************************
**
** ConnectionManager.h
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#ifndef CONNECTIONMANAGER_H
#define CONNECTIONMANAGER_H
//----------------------------------------------------------------------------------
#include "../Globals.h"
#include "../Network/Connection.h"
#include "../Network/LoginCrypt.h"
#include "../Network/GameCrypt.h"
#include "../EnumList.h"
//----------------------------------------------------------------------------------
//!Класс менеджера подключения к серверу
class CConnectionManager
{
	//!Тип шифрования
	SETGET(ENCRYPTION_TYPE, EncryptionType);

	//!Версия протокола
	SETGET(CLIENT_VERSION, ClientVersion);

	SETGET(uint, CryptKey1);
	SETGET(uint, CryptKey2);
	SETGET(uint, CryptKey3);
	SETGET(ushort, CryptSeed);

private:
	//!Подключение к сокету авторизации
	CSocket m_LoginSocket;

	//!Подключение к сокету сервера
	CSocket m_GameSocket;

	//!Шифрование для сокета авторизации
	CLoginCrypt m_LoginCrypt;

	//!Рыбы
	CBlowfishCrypt m_BlowfishCrypt;
	CTwofishCrypt m_TwoFishCrypt;

	//!Тип сокета. true - Login, false - game
	bool m_IsLoginSocket;

public:
	CConnectionManager();
	~CConnectionManager();

	/*!
	Инициализаци логин сокета
	@return 
	*/
	void Init();

	/*!
	Инициализация игрового сокета
	@param [__in] GameSeed Ключ для игрового шифрования
	@return Код ошибки
	*/
	void Init(puchar GameSeed);

	/*!
	Состояние подключения
	@return true - подключено
	*/
	bool Connected() { return (m_LoginSocket.Connected || m_GameSocket.Connected); }

	/*!
	Подключение к серверу
	@param [__in] IP IP адрес сервера
	@param [__in] Port Порт сервера
	@param [__in] GameSeed Ключ для шифрования
	@return Код ошибки
	*/
	bool Connect(const string &address, int port, puchar gameSeed);

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
	int Send(puchar buf, int size);

	int Send(const UCHAR_LIST &data);

	/*!
	Получить свой IP-адрес
	@return 
	*/
	puchar GetClientIP() const { return (puchar)m_LoginCrypt.m_seed; }
};
//---------------------------------------------------------------------------
//!Менеджер подключения
extern CConnectionManager g_ConnectionManager;
//---------------------------------------------------------------------------
#endif