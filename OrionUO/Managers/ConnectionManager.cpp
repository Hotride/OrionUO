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
#include "../ServerList.h"
#include "../Managers/GumpManager.h"
#include "../Gumps/GumpNotify.h"
//----------------------------------------------------------------------------------
CConnectionManager g_ConnectionManager;
//----------------------------------------------------------------------------------
CConnectionManager::CConnectionManager()
: m_EncryptionType(ET_NOCRYPT), m_ClientVersion(CV_OLD), m_IsLoginSocket(true),
m_LoginSocket(false), m_GameSocket(true), m_CryptKey1(0), m_CryptKey2(0), m_CryptKey3(0),
m_CryptSeed(1357)
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
/*!
Инициализаци логин сокета
@return Код ошибки
*/
void CConnectionManager::Init()
{
	//Сокет уже открыт, ошибка
	if (m_LoginSocket.Connected)
	{
		LOG("Socket's already open. Return\n");
		return;
	}

	m_IsLoginSocket = true;

	WSADATA wsaData;
	WSAStartup(MAKEWORD(1, 1), &wsaData);

	char hostName[1024] = {0};
	LOG("Getting login crypt key\n");
	//Получим ключик для логин крипта
	if(!gethostname(hostName, 1024))
	{
		LOG("gethostname call\n");
		if (LPHOSTENT lphost = gethostbyname(hostName))
		{
			WISP_DATASTREAM::CDataWritter stream;
			stream.WriteUInt32BE(((LPIN_ADDR)lphost->h_addr)->s_addr);
			UCHAR_LIST &data = stream.Data();

			m_LoginCrypt.Init(&data[0], m_CryptKey1, m_CryptKey2, m_CryptKey3, m_CryptSeed);
		}
	}

	LOG("WSACleanup\n");
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

	//Инициализация шифрования
	if (m_EncryptionType != ET_NOCRYPT)
		m_BlowfishCrypt.Init();

	if (m_EncryptionType == ET_203 || m_EncryptionType == ET_TFISH)
	{
		m_TwoFishCrypt.Init(gameSeed);

		if (m_EncryptionType == ET_TFISH)
			m_TwoFishCrypt.Init_MD5();
	}
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

		LOG("Connecting to login socket\n");
		bool result = m_LoginSocket.Connect(address, port);

		if (result) //Соединение удачно установлено
		{
			LOG("Successfully connected to login socket\n");
			//Для старых клиентов посылаем сразу же 4 байта
			g_TotalSendSize = 4;
			g_LastPacketTime = g_Ticks;
			g_LastSendTime = g_LastPacketTime;

			if (m_ClientVersion < CV_6060)
			{
				LOG("Old client login, sending 4 bytes with seed\n");
				m_LoginSocket.Send(m_LoginCrypt.m_seed, 4);
			}
			else //В новых клиентах изменилось приветствие логин сокета
			{
				BYTE buf = 0xEF;
				m_LoginSocket.Send(&buf, 1); //0xEF - приветствие, 1 байт
				LOG("New client login, sending 1 byte\n");

				m_LoginSocket.Send(m_LoginCrypt.m_seed, 4); //Сид, 4 байта
				LOG("New client login, sending 4 bytes seed\n");

				WISP_DATASTREAM::CDataWritter stream;

				string str = g_Orion.ClientVersionText;
				LOG("Orion.CLientVersionText(str)\n", str);

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
					LOG("Writing client version to stream\n", str);
				}

				g_TotalSendSize = 21;
				LOG("Sending client version, 16 byte\n", str);
				m_LoginSocket.Send(stream.Data()); //Версия клиента, 16 байт
			}
		}
		else
		{
			LOG("Failed to connect to login socket.\n");
			m_LoginSocket.Disconnect();
		}

		return result;
	}
	else //Гейм сокет
	{
		LOG("Game socket?\n");
		if (m_GameSocket.Connected)
			return true; //Уже подключены

		g_TotalSendSize = 4;
		g_LastPacketTime = g_Ticks;
		g_LastSendTime = g_LastPacketTime;

		LOG("Game socket connecting...\n");
		bool result = m_GameSocket.Connect(address, port);

		if (result)
		{
			LOG("Game socket connected. Sending key\n");
			m_GameSocket.Send(gameSeed, 4); //Если всё удачно - шлем ключ из пакета релея
		}


		LOG("Login socket disconnect\n");
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
				{
					string str = "Disconnected from " + g_ServerList.GetSelectedServer()->Name;
					g_Orion.CreateTextMessage(TT_SYSTEM, 0, 3, 0x21, str);

					int x = g_ConfigManager.GameWindowX + (g_ConfigManager.GameWindowWidth / 2) - 100;
					int y = g_ConfigManager.GameWindowY + (g_ConfigManager.GameWindowHeight / 2) - 62;

					CGumpNotify *gump = new CGumpNotify(0, x, y, CGumpNotify::ID_GN_STATE_LOGOUT, 200, 125, "Connection lost");

					g_GumpManager.AddGump(gump);

					g_Orion.InitScreen(GS_GAME_BLOCKED);
					g_GameBlockedScreen.Code = 0;
				}
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
#if UO_ABYSS_SHARD == 1
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
#endif

	if (m_IsLoginSocket) //Логин сокет
	{
		if (!m_LoginSocket.Connected)
			return 0; //Нет подключения

		UCHAR_LIST cbuf(size); //Буффер для криптованного пакета

		//Шифруем, руководствуясь указанным типом шифрования
		if (m_EncryptionType == ET_OLD_BFISH)
			m_LoginCrypt.Encrypt_Old(buf, &cbuf[0], size);
		else if (m_EncryptionType == ET_1_25_36)
			m_LoginCrypt.Encrypt_1_25_36(buf, &cbuf[0], size);
		else if (m_EncryptionType != ET_NOCRYPT)
			m_LoginCrypt.Encrypt(buf, &cbuf[0], size);
		else
			memcpy(&cbuf[0], buf, size); //Или просто скопируем буффер, если шифрование отсутствует

		return m_LoginSocket.Send(cbuf); //Отправляем зашифрованный пакет
	}
	else
	{
		if (!m_GameSocket.Connected)
			return 0; //Нет подключения

		UCHAR_LIST cbuf(size); //Буффер для криптованного пакета

		if (m_EncryptionType == ET_NOCRYPT)
			memcpy(&cbuf[0], buf, size); //Скопируем буффер, если шифрование отсутствует
		else
		{
			//Шифруем, руководствуясь указанным типом шифрования
			if (m_EncryptionType == ET_203)
			{
				m_BlowfishCrypt.Encrypt(buf, &cbuf[0], size);
				m_TwoFishCrypt.Encrypt(&cbuf[0], &cbuf[0], size);
			}
			else if (m_EncryptionType == ET_TFISH)
				m_TwoFishCrypt.Encrypt(buf, &cbuf[0], size);
			else
				m_BlowfishCrypt.Encrypt(buf, &cbuf[0], size);
		}

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