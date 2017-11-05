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
typedef void __cdecl NETWORK_INIT_TYPE(const bool&, BYTE*);
typedef void __cdecl NETWORK_ACTION_TYPE(const bool&, BYTE*, BYTE*, const int&);
typedef void __cdecl NETWORK_POST_ACTION_TYPE(BYTE*, BYTE*, const int&);
//----------------------------------------------------------------------------------
extern NETWORK_INIT_TYPE *g_NetworkInit;
extern NETWORK_ACTION_TYPE *g_NetworkAction;
extern NETWORK_POST_ACTION_TYPE *g_NetworkPostAction;
//----------------------------------------------------------------------------------
//!Класс менеджера подключения к серверу
class CConnectionManager
{
	SETGETE(bool, UseProxy, false);
	SETGETE(string, ProxyAddress, "");
	SETGETE(int, ProxyPort, 0);
	SETGETE(bool, ProxySocks5, false);
	SETGETE(string, ProxyAccount, "");
	SETGETE(string, ProxyPassword, "");

private:
	//!Подключение к сокету авторизации
	CSocket m_LoginSocket{ CSocket(false) };

	//!Подключение к сокету сервера
	CSocket m_GameSocket{ CSocket(true) };

	//!Тип сокета. true - Login, false - game
	bool m_IsLoginSocket{ true };

	uchar m_Seed[4];

	void SendIP(CSocket &socket, puchar seed);

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
	puchar GetClientIP() const { return (puchar)m_Seed; }
};
//----------------------------------------------------------------------------------
//!Менеджер подключения
extern CConnectionManager g_ConnectionManager;
//----------------------------------------------------------------------------------
#endif //CONNECTIONMANAGER_H
//----------------------------------------------------------------------------------
