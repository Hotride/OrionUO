/***********************************************************************************
**
** ConnectionManager.cpp
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#include "ConnectionManager.h"
#include "ConfigManager.h"
#include "../Wisp/WispDataStream.h"
#include "../Wisp/WispLogger.h"
#include "../OrionUO.h"
#include "PacketManager.h"
#include "../Screen stages/ConnectionScreen.h"
#include "../Screen stages/GameBlockedScreen.h"
#include "../Managers/GumpManager.h"
#include "../Managers/PacketManager.h"
//----------------------------------------------------------------------------------
CConnectionManager g_ConnectionManager;
//----------------------------------------------------------------------------------
NETWORK_INIT_TYPE *g_NetworkInit = NULL;
NETWORK_ACTION_TYPE *g_NetworkAction = NULL;
//----------------------------------------------------------------------------------
CConnectionManager::CConnectionManager()
: m_IsLoginSocket(true), m_LoginSocket(false), m_GameSocket(true), m_UseProxy(false),
m_ProxyAddress(""), m_ProxyPort(0), m_ProxySocks5(false), m_ProxyAccount(""),
m_ProxyPassword("")
{
}
//----------------------------------------------------------------------------------
CConnectionManager::~CConnectionManager()
{
	//Дисконнектим всё подключенное
	if (m_LoginSocket.Connected)
		m_LoginSocket.Disconnect();

	if (m_GameSocket.Connected)
		m_GameSocket.Disconnect();
}
//----------------------------------------------------------------------------------
void CConnectionManager::OnChangeUseProxy(const bool &val)
{
	m_LoginSocket.UseProxy = val;
	m_GameSocket.UseProxy = val;
}
//----------------------------------------------------------------------------------
void CConnectionManager::OnChangeProxyAddress(const string &val)
{
	m_LoginSocket.ProxyAddress = val;
	m_GameSocket.ProxyAddress = val;
}
//----------------------------------------------------------------------------------
void CConnectionManager::OnChangeProxyPort(const int &val)
{
	m_LoginSocket.ProxyPort = val;
	m_GameSocket.ProxyPort = val;
}
//----------------------------------------------------------------------------------
void CConnectionManager::OnChangeProxySocks5(const bool &val)
{
	m_LoginSocket.ProxySocks5 = val;
	m_GameSocket.ProxySocks5 = val;
}
//----------------------------------------------------------------------------------
void CConnectionManager::OnChangeProxyAccount(const string &val)
{
	m_LoginSocket.ProxyAccount = val;
	m_GameSocket.ProxyAccount = val;
}
//----------------------------------------------------------------------------------
void CConnectionManager::OnChangeProxyPassword(const string &val)
{
	m_LoginSocket.ProxyPassword = val;
	m_GameSocket.ProxyPassword = val;
}
//----------------------------------------------------------------------------------
/*!
Инициализаци логин сокета
@return Код ошибки
*/
void CConnectionManager::Init()
{
	//Сокет уже открыт, ошибка
	if (m_LoginSocket.Connected)
		return;

	m_IsLoginSocket = true;

	WSADATA wsaData;
	WSAStartup(MAKEWORD(1, 1), &wsaData);

	char hostName[1024] = {0};

	//Получим ключик для логин крипта
	if(!gethostname(hostName, 1024))
	{
		if (LPHOSTENT lphost = gethostbyname(hostName))
		{
			WISP_DATASTREAM::CDataWritter stream;
			stream.WriteUInt32BE(((LPIN_ADDR)lphost->h_addr)->s_addr);
			UCHAR_LIST &data = stream.Data();

			memcpy(&m_Seed[0], &data[0], 4);
			g_NetworkInit(true, &data[0]);
		}
	}

	WSACleanup();
}
//---------------------------------------------------------------------------
/*!
Инициализация игрового сокета
@param [__in] GameSeed Ключ для игрового шифрования
@return Код ошибки
*/
void CConnectionManager::Init(puchar gameSeed)
{
	//Гейм сокет уже открыт, ошибка
	if (m_GameSocket.Connected)
		return;

	m_IsLoginSocket = false;

	g_NetworkInit(false, &gameSeed[0]);
}
//---------------------------------------------------------------------------
/*!
Подключение к серверу
@param [__in] IP IP адрес сервера
@param [__in] Port Порт сервера
@param [__in] GameSeed Ключ для шифрования
@return Код ошибки
*/
bool CConnectionManager::Connect(const string &address, int port, puchar gameSeed)
{
	if (m_IsLoginSocket) //Логин сокет
	{
		if (m_LoginSocket.Connected)
			return true; //Уже подключен куда-то

		bool result = m_LoginSocket.Connect(address, port);

		if (result) //Соединение удачно установлено
		{
			//Для старых клиентов посылаем сразу же 4 байта
			g_TotalSendSize = 4;
			g_LastPacketTime = g_Ticks;
			g_LastSendTime = g_LastPacketTime;

			if (g_PacketManager.ClientVersion < CV_6060)
				m_LoginSocket.Send(m_Seed, 4);
			else //В новых клиентах изменилось приветствие логин сокета
			{
				BYTE buf = 0xEF;
				m_LoginSocket.Send(&buf, 1); //0xEF - приветствие, 1 байт

				m_LoginSocket.Send(m_Seed, 4); //Сид, 4 байта

				WISP_DATASTREAM::CDataWritter stream;

				string str = g_Orion.ClientVersionText;

				if (str.length())
				{
					char ver[20] = {0};
					char *ptr = ver;
					strncpy_s(ver, str.c_str(), str.length());
					int idx = 0;

					for (int i = 0; i < (int)str.length(); i++)
					{
						if (ver[i] == '.')
						{
							ver[i] = 0;
							stream.WriteUInt32BE(atoi(ptr));
							ptr = ver + (i + 1);
							idx++;

							if (idx > 3)
								break;
						}
					}

					stream.WriteUInt32BE(atoi(ptr));
				}

				g_TotalSendSize = 21;
				m_LoginSocket.Send(stream.Data()); //Версия клиента, 16 байт
			}
		}
		else
			m_LoginSocket.Disconnect();

		return result;
	}
	else //Гейм сокет
	{
		if (m_GameSocket.Connected)
			return true; //Уже подключены

		g_TotalSendSize = 4;
		g_LastPacketTime = g_Ticks;
		g_LastSendTime = g_LastPacketTime;

		bool result = m_GameSocket.Connect(address, port);

		if (result)
			m_GameSocket.Send(gameSeed, 4); //Если всё удачно - шлем ключ из пакета релея

		m_LoginSocket.Disconnect(); //Отрубаем логин сокет

		return result;
	}

	return false;
}
//---------------------------------------------------------------------------
/*!
Разорвать подключение
@return
*/
void CConnectionManager::Disconnect()
{
	//!Дисконнект всего, что подключено
	if (m_LoginSocket.Connected)
		m_LoginSocket.Disconnect();

	if (m_GameSocket.Connected)
		m_GameSocket.Disconnect();
}
//---------------------------------------------------------------------------
/*!
Получить данные с сервера
@return 
*/
void CConnectionManager::Recv()
{
	if (m_IsLoginSocket) //Логин сокет
	{
		if (!m_LoginSocket.Connected)
			return; //Не подключены

		if (!m_LoginSocket.ReadyRead())
		{
			if (m_LoginSocket.DataReady == SOCKET_ERROR)
			{
				LOG("Failed to Recv()...Disconnecting...\n");

				g_Orion.InitScreen(GS_MAIN_CONNECT);
				g_ConnectionScreen.Type = CST_CONLOST;
			}
			return; //На сокете в данный момент пусто
		}

		g_PacketManager.Read(&m_LoginSocket);
	}
	else //Гейм сокет
	{
		if (!m_GameSocket.Connected)
			return; //Не подключены

		if (!m_GameSocket.ReadyRead())
		{
			if (m_GameSocket.DataReady == SOCKET_ERROR)
			{
				LOG("Failed to Recv()...Disconnecting...\n");

				if (g_GameState == GS_GAME || (g_GameState == GS_GAME_BLOCKED && g_GameBlockedScreen.Code))
					g_Orion.DisconnectGump();
				else
				{
					g_Orion.InitScreen(GS_MAIN_CONNECT);
					g_ConnectionScreen.Type = CST_CONLOST;
				}
			}

			return; //На сокете в данный момент пусто
		}

		g_PacketManager.Read(&m_GameSocket);
	}
}
//---------------------------------------------------------------------------
/*!
Отправить сообщение серверу
@param [__in] buf Буфер с данными
@param [__in] size Размер данных
@return Размер отправленных данных или код ошибки
*/
int CConnectionManager::Send(puchar buf, int size)
{
	if (g_TheAbyss)
	{
		switch (buf[0])
		{
			case 0x34:
				buf[0] = 0x71;
				break;
			case 0x72:
				buf[0] = 0x6C;
				break;
			case 0x6C:
				buf[0] = 0x72;
				break;
			case 0x3B:
				buf[0] = 0x34;
				break;
			case 0x6F:
				buf[0] = 0x56;
				break;
			case 0x56:
				buf[0] = 0x6F;
				break;
			default:
				break;
		}
	}

	if (m_IsLoginSocket) //Логин сокет
	{
		if (!m_LoginSocket.Connected)
			return 0; //Нет подключения

		UCHAR_LIST cbuf(size); //Буффер для криптованного пакета

		g_NetworkAction(true, &buf[0], &cbuf[0], size);

		return m_LoginSocket.Send(cbuf); //Отправляем зашифрованный пакет
	}
	else
	{
		if (!m_GameSocket.Connected)
			return 0; //Нет подключения

		UCHAR_LIST cbuf(size); //Буффер для криптованного пакета

		g_NetworkAction(false, &buf[0], &cbuf[0], size);

		return m_GameSocket.Send(cbuf); //Отправляем зашифрованный пакет
	}

	return 0;
}
//---------------------------------------------------------------------------
int CConnectionManager::Send(const UCHAR_LIST &data)
{
	return Send((puchar)&data[0], data.size());
}
//---------------------------------------------------------------------------