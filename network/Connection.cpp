/****************************************************************************
**
** Connection.cpp
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
//---------------------------------------------------------------------------
TConnection::TConnection()
: m_Socket(INVALID_SOCKET), m_ReadyData(0), m_Connected(false)
{
}
//---------------------------------------------------------------------------
TConnection::~TConnection()
{
	if (m_Connected)
		closesocket(m_Socket); //Закрываем соединение

	m_Socket = INVALID_SOCKET;
	m_Connected = false;
	m_ReadyData = 0;
}
//---------------------------------------------------------------------------
int TConnection::Init()
{
	TPRINT("TConnection::Init\n");

	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData))
		return CE_INIT; //Ошибка инициализации

	m_Socket = socket(AF_INET, SOCK_STREAM, IPPROTO_IP);

	if (m_Socket == INVALID_SOCKET)
		return CE_CREATE; //Ошибка при создании сокета

	m_Connected = false;
	return CE_NO_ERROR;
}
//---------------------------------------------------------------------------
int TConnection::Connect(const char *IP, int Port)
{
	TPRINT("TConnection::Connect\n");

	if (m_Connected)
		return CE_CONNECTED; //Уже подключены

	if (m_Socket == INVALID_SOCKET)
		return CE_NO_SOCKET; //Паленый сокет

	sockaddr_in caddr;
	memset(&caddr, 0, sizeof(sockaddr_in));
	caddr.sin_family = AF_INET;

	struct hostent *he;

	int rt = inet_addr(IP);

	//Узнаем, куда нам нужно подключиться
	if (rt != -1) caddr.sin_addr.s_addr = rt;
	else
	{
		he = gethostbyname(IP);

		if (he == NULL)
		{
			TPRINT("TConnection::Connect->Host not found\n");
			return CE_HOST;
		}

		memcpy(&caddr.sin_addr, he->h_addr, he->h_length);
	}

	//Укажем порт
	caddr.sin_port = htons(Port);

	//Продуем подключиться на указанный адрес
	if (connect(m_Socket, (struct sockaddr*)&caddr, sizeof(caddr)) == -1)
	{
		TPRINT("TConnection::Connect->Connection failed\n");
		return CE_CONNECT_ERROR; //Не вышло((
	}

	m_Connected = true;
	WSASetLastError(0);

	return CE_NO_ERROR; //Подключение прошло успешно
}
//---------------------------------------------------------------------------
void TConnection::Disconnect()
{
	TPRINT("TConnection::Disconnect\n");

	if (m_Connected)
		closesocket(m_Socket); //Закрываем сокет

	m_Socket = INVALID_SOCKET;
	m_Connected = false;
	m_ReadyData = 0;
}
//---------------------------------------------------------------------------
bool TConnection::DataReady()
{
	//TPRINT("TConnection::DataReady\n");

	if (!m_Connected)
		return false; //Не подключены

	fd_set rfds;
	struct timeval tv = {0, 0};
	FD_ZERO(&rfds);
	FD_SET(m_Socket, &rfds);

	m_ReadyData = select(m_Socket, &rfds, NULL, NULL, &tv); //На сокете что-то есть?

	return (m_ReadyData != 0);
}
//---------------------------------------------------------------------------
int TConnection::Recv(PBYTE buffer, int &size)
{
	//TPRINT("TConnection::Recv\n");

	if (!m_Connected)
		return CE_NOT_CONNECTED; //Нет подключения

	if (m_ReadyData == SOCKET_ERROR) //select завершился с ошибкой, закрываемся
	{
		closesocket(m_Socket);

		m_Socket = INVALID_SOCKET;
		m_Connected = false;
		m_ReadyData = 0;

		TPRINT("TConnection::ReadyData == SOCKET_ERROR\n");

		return CE_RECV_ERROR;
	}

	size = recv(m_Socket, (char*)buffer, SOCK_RECV_BUF_SIZE, 0); //Получаем данные

	return size;
}
//---------------------------------------------------------------------------
int TConnection::Send(PBYTE buffer, int size)
{
	//TPRINT("TConnection::Send\n");

	if (!m_Connected)
		return CE_NOT_CONNECTED; //Нет подключения

	size = send(m_Socket, (char*)buffer, size, 0); //Отправляем данные

	return size;
}
//---------------------------------------------------------------------------