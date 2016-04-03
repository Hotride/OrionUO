/****************************************************************************
**
** ConnectionManager.cpp
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
#include "stdafx.h"

TConnectionManager ConnectionManager;
//---------------------------------------------------------------------------
TConnectionManager::TConnectionManager()
: m_LastMessageFragment(NULL), m_EncryptionType(ET_NOCRYPT), m_ClientVersion(CV_OLD),
m_SocketType(true)
{
}
//---------------------------------------------------------------------------
TConnectionManager::~TConnectionManager()
{
	//Дисконнектим всё подключенное
	if (m_LoginSocket.Connected())
		m_LoginSocket.Disconnect();
	if (m_GameSocket.Connected())
		m_GameSocket.Disconnect();

	//Удаляем фрагмент, если есть
	if (m_LastMessageFragment != NULL)
		delete m_LastMessageFragment;
	m_LastMessageFragment = NULL;
}
//---------------------------------------------------------------------------
/*!
Инициализаци логин сокета
@param [__in] k1 Ключ шифрования 1
@param [__in] k2 Ключ шифрования 2
@param [__in] k3 Ключ шифрования 3
@param [__in] seed_key Семка для генерации шифрования
@return Код ошибки
*/
int TConnectionManager::Init( __in DWORD k1, __in DWORD k2, __in DWORD k3, __in DWORD seed_key)
{
	//Сокет уже открыт, ошибка
	if (m_LoginSocket.Connected())
		return CE_CONNECTED;

	m_SocketType = true;

	WSADATA wsaData;
	WSAStartup(MAKEWORD(1, 1), &wsaData);

	char HostName[1024] = {0};

	//Получим ключик для логин крипта
	if(!gethostname(HostName, 1024))
	{
		if (LPHOSTENT lphost = gethostbyname(HostName))
		{
			BYTE buf[4] = {0};
			pack32(buf,((LPIN_ADDR)lphost->h_addr)->s_addr);

			BYTE c = buf[0];
			buf[0] = buf[3];
			buf[3] = c;
			c = buf[1];
			buf[1] = buf[2];
			buf[2] = c;

			m_LoginCrypt.Init(buf, k1, k2, k3, seed_key);
		}
	}

	WSACleanup();
		
	return m_LoginSocket.Init();
}
//---------------------------------------------------------------------------
/*!
Инициализация игрового сокета
@param [__in] GameSeed Ключ для игрового шифрования
@return Код ошибки
*/
int TConnectionManager::Init(__in PBYTE GameSeed)
{
	//Гейм сокет уже открыт, ошибка
	if (m_GameSocket.Connected())
		return CE_CONNECTED;

	m_SocketType = false;

	//Инициализация шифрования
	if (m_EncryptionType != ET_NOCRYPT)
		m_BlowfishCrypt.Init();

	if (m_EncryptionType == ET_203 || m_EncryptionType == ET_TFISH)
	{
		m_TwoFishCrypt.Init(GameSeed);

		if (m_EncryptionType == ET_TFISH)
			m_TwoFishCrypt.Init_MD5();
	}

	return m_GameSocket.Init();
}
//---------------------------------------------------------------------------
/*!
Подключение к серверу
@param [__in] IP IP адрес сервера
@param [__in] Port Порт сервера
@param [__in] GameSeed Ключ для шифрования
@return Код ошибки
*/
int TConnectionManager::Connect( __in const char *IP, __in int Port, __in PBYTE GameSeed)
{
	if (m_SocketType) //Логин сокет
	{
		if (m_LoginSocket.Connected())
			return CE_CONNECTED; //Уже подключен куда-то

		int ret = m_LoginSocket.Connect(IP, Port);

		if (ret == CE_NO_ERROR) //Соединение удачно установлено
		{
			//Для старых клиентов посылаем сразу же 4 байта
			g_TotalSendSize = 4;
			g_LastPacketTime = GetTickCount();
			g_LastSendTime = g_LastPacketTime;

			if (m_ClientVersion < CV_6060)
				m_LoginSocket.Send(m_LoginCrypt.m_seed, 4);
			else //В новых клиентах изменилось приветствие логин сокета
			{
				BYTE buf[16] = {0};
				buf[0] = 0xEF;
				m_LoginSocket.Send(buf, 1); //0xEF - приветствие, 1 байт

				m_LoginSocket.Send(m_LoginCrypt.m_seed, 4); //Сид, 4 байта

				buf[0] = 0;
				string str = g_ClientVersionText;
				if (str.length())
				{
					char ver[20] = {0};
					char *ptr = ver;
					strncpy(ver, str.c_str(), str.length());
					int idx = 0;

					for (int i = 0; i < (int)str.length(); i++)
					{
						if (ver[i] == '.')
						{
							ver[i] = 0;
							pack32(buf + (idx * 4), atoi(ptr));
							ptr = ver + (i + 1);
							idx++;

							if (idx > 3)
								break;
						}
					}

					pack32(buf + (idx * 4), atoi(ptr));
				}

				g_TotalSendSize = 21;

				m_LoginSocket.Send(buf, 16); //Версия клиента, 16 байт
			}
		}
		else
			m_LoginSocket.Disconnect();

		return ret;
	}
	else //Гейм сокет
	{
		if (m_GameSocket.Connected())
			return CE_CONNECTED; //Уже подключены

		g_TotalSendSize = 4;
		g_LastPacketTime = GetTickCount();
		g_LastSendTime = g_LastPacketTime;
		int ret = m_GameSocket.Connect(IP, Port);

		if (ret == CE_NO_ERROR)
			m_GameSocket.Send(GameSeed, 4); //Если всё удачно - шлем ключ из пакета релея

		m_LoginSocket.Disconnect(); //Отрубаем логин сокет

		return ret;
	}

	return 0;
}
//---------------------------------------------------------------------------
/*!
Разорвать подключение
@return
*/
void TConnectionManager::Disconnect()
{
	//!Дисконнект всего, что подключено
	if (m_LoginSocket.Connected())
		m_LoginSocket.Disconnect();

	if (m_GameSocket.Connected())
		m_GameSocket.Disconnect();
}
//---------------------------------------------------------------------------
/*!
Получить данные с сервера
@return 
*/
void TConnectionManager::Recv()
{
	if (m_SocketType) //Логин сокет
	{
		if (!m_LoginSocket.Connected())
			return; //Не подключены

		if (!m_LoginSocket.DataReady())
			return; //На сокете в данный момент пусто

		BYTE Data[SOCK_RECV_BUF_SIZE] = {0};
		int size = 0;

		if (m_LoginSocket.Recv(Data, size) > 0) //Если данные получены - обрабатываем
		{
			PBYTE ptr = Data;
			PBYTE end = ptr + size;

			if (m_LastMessageFragment != NULL) //Предыдущее сообщение не было обработано целиком
			{
				ptr += m_LastMessageFragment->Append(ptr, size); //Допишем в фрагмент данные

				if (!m_LastMessageFragment->Complete())
					return; //Пришедших данных оказалось не достаточно, сваливаем

				//Пришедших данных оказалось достаточно, отправляем пакет на анализ, в PacketManager
				PacketManager.ReceiveHandler(m_LastMessageFragment->Get(), m_LastMessageFragment->Size());

				//Фрагмент нам больше не требуется, удалим его
				delete m_LastMessageFragment;
				m_LastMessageFragment = NULL;
			}

			while (ptr < end) //Обработаем всё оставшееся в буффере
			{
				//Размер обрабатываемого пакета
				int msg_size = PacketManager.GetPacketSize(*ptr);

				if (msg_size == -1) //Неизвестная длина (такого уже быть не может)
				{
					EPRINT("Length unknown for message 0x%02X\n", *ptr);
					msg_size = 0;
				}
				else
				{
					//Если это динамическая длина пакета
					if (msg_size == 0 && ptr + 3 <= end)
						msg_size = (ptr[1] << 8) | ptr[2];
				}

				//Если это динамическая длина пакета или пакет не был доставлен целиком
				if (msg_size == 0 || ptr + msg_size > end)
				{
					m_LastMessageFragment = new TMessageFragment(ptr, end - ptr, msg_size);
					ptr = end;
				}
				else //Статичный размер пакета и данных достаточно для обработки
				{
					//Отправляем пакет на анализ, в PacketManager
					PacketManager.ReceiveHandler(ptr, msg_size);
					ptr += msg_size;
				}
			}
		}
		else //Ошибка приема, дисконнектимся
		{
			TPRINT("Failed to Recv()...Disconnecting...\n");
			m_LoginSocket.Disconnect();

			UO->InitScreen(GS_MAIN_CONNECT);
			ConnectionScreen->Type = CST_CONLOST;
		}
	}
	else //Гейм сокет
	{
		if (!m_GameSocket.Connected())
			return; //Не подключены

		if (!m_GameSocket.DataReady())
			return; //На сокете в данный момент пусто

		BYTE Data[SOCK_RECV_BUF_SIZE] = {0};
		int size = 0;

		if (m_GameSocket.Recv(Data, size) > 0) //Если данные получены - обрабатываем
		{
			PBYTE dec_buf = new BYTE[size * 4 + 2];
			int in_bytes = size, out_bytes = 65536;

			m_Decompressor((char*)dec_buf, (char*)Data, out_bytes, in_bytes); //Расжимаем

			if (in_bytes != size) //Буффер декомпрессии недостаточен
			{
				EPRINT("decompression buffer too small\n");
				m_GameSocket.Disconnect();
				delete dec_buf;
				return;
			}

			PBYTE ptr = dec_buf;
			PBYTE end = ptr + out_bytes;
			
			if (m_LastMessageFragment != NULL) //Предыдущее сообщение не было обработано целиком
			{
				ptr += m_LastMessageFragment->Append(ptr, out_bytes); //Допишем в фрагмент данные

				if (!m_LastMessageFragment->Complete()) //Пришедших данных оказалось не достаточно, сваливаем
				{
					delete dec_buf;
					return;
				}

				//Пришедших данных оказалось достаточно, отправляем пакет на анализ, в PacketManager
				PacketManager.ReceiveHandler(m_LastMessageFragment->Get(), m_LastMessageFragment->Size());

				//Фрагмент нам больше не требуется, удалим его
				delete m_LastMessageFragment;
				m_LastMessageFragment = NULL;
			}

			while (ptr < end) //Обработаем всё оставшееся в буффере
			{
				//Размер обрабатываемого пакета
				int msg_size = PacketManager.GetPacketSize(*ptr);

				if (msg_size == -1) //Неизвестная длина (такого уже быть не может)
				{
					EPRINT("Length unknown for message 0x%02X\n", *ptr);
					msg_size = 0;
				}
				else
				{
					//Если это динамическая длина пакета
					if (msg_size == 0 && ptr + 3 <= end)
						msg_size = (ptr[1] << 8) | ptr[2];
				}

				//Если это динамическая длина пакета или пакет не был доставлен целиком
				if (msg_size == 0 || ptr + msg_size > end)
				{
					m_LastMessageFragment = new TMessageFragment(ptr, end - ptr, msg_size);
					ptr = end;
				}
				else //Статичный размер пакета и данных достаточно для обработки
				{
					//Отправляем пакет на анализ, в PacketManager
					PacketManager.ReceiveHandler(ptr, msg_size);
					ptr += msg_size;
				}
			}

			delete dec_buf;
		}
		else //Ошибка приема, дисконнектимся
		{
			TPRINT("Failed to Recv()...Disconnecting...\n");
			m_GameSocket.Disconnect();

			if (g_GameState == GS_GAME || (g_GameState == GS_GAME_BLOCKED && GameBlockedScreen->Code))
			{
				string str = "Disconnected from " + ServerList.GetServerName();
				UO->CreateTextMessage(TT_SYSTEM, 0, 3, 0x21, str);

				int x = g_GameWindowPosX + (g_GameWindowWidth / 2) - 100;
				int y = g_GameWindowPosY + (g_GameWindowHeight / 2) - 62;

				TGumpNotify *gump = new TGumpNotify(0, x, y, 1, 200, 125, "Connection lost");

				GumpManager->AddGump(gump);

				UO->InitScreen(GS_GAME_BLOCKED);
				GameBlockedScreen->Code = 0;
			}
			else
			{
				UO->InitScreen(GS_MAIN_CONNECT);
				ConnectionScreen->Type = CST_CONLOST;
			}
		}
	}
}
//---------------------------------------------------------------------------
/*!
Отправить сообщение серверу
@param [__in] buf Буфер с данными
@param [__in] size Размер данных
@return Размер отправленных данных или код ошибки
*/
int TConnectionManager::Send(PBYTE buf, int size)
{
#if UO_ABYSS_SHARD == 1
	if (buf[0] == 0x34)
		buf[0] = 0x71;
	else if (buf[0] == 0x72)
		buf[0] = 0x6C;
	else if (buf[0] == 0x6C)
		buf[0] = 0x72;
	else if (buf[0] == 0x3B)
		buf[0] = 0x34;
	else if (buf[0] == 0x6F)
		buf[0] = 0x56;
	else if (buf[0] == 0x56)
		buf[0] = 0x6F;
#endif

	if (m_SocketType) //Логин сокет
	{
		if (!m_LoginSocket.Connected())
			return CE_NOT_CONNECTED; //Нет подключения

		PBYTE cbuf = new BYTE[size]; //Буффер для криптованного пакета

		//Шифруем, руководствуясь указанным типом шифрования
		if (m_EncryptionType == ET_OLD_BFISH)
			m_LoginCrypt.Encrypt_Old(buf, cbuf, size);
		else if (m_EncryptionType == ET_1_25_36)
			m_LoginCrypt.Encrypt_1_25_36(buf, cbuf, size);
		else if (m_EncryptionType != ET_NOCRYPT)
			m_LoginCrypt.Encrypt(buf, cbuf, size);
		else
			memcpy(cbuf, buf, size); //Или просто скопируем буффер, если шифрование отсутствует

		int ret = m_LoginSocket.Send(cbuf, size); //Отправляем зашифрованный пакет

		delete cbuf;

		return ret;
	}
	else
	{
		if (!m_GameSocket.Connected()) return CE_NOT_CONNECTED; //Нет подключения

		PBYTE cbuf = new BYTE[size]; //Буффер для криптованного пакета

		if (m_EncryptionType == ET_NOCRYPT)
			memcpy(cbuf, buf, size); //Скопируем буффер, если шифрование отсутствует
		else
		{
			//Шифруем, руководствуясь указанным типом шифрования
			if (m_EncryptionType == ET_203)
			{
				m_BlowfishCrypt.Encrypt(buf, cbuf, size);
				m_TwoFishCrypt.Encrypt(cbuf, cbuf, size);
			}
			else if (m_EncryptionType == ET_TFISH)
				m_TwoFishCrypt.Encrypt(buf, cbuf, size);
			else
				m_BlowfishCrypt.Encrypt(buf, cbuf, size);
		}

		int ret = m_GameSocket.Send(cbuf, size); //Отправляем зашифрованный пакет

		delete cbuf;

		return ret;
	}

	return 0;
}
//---------------------------------------------------------------------------