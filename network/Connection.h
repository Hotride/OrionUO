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
//Максимальный размер принимаемых данных
#define SOCK_RECV_BUF_SIZE 0x1000
//---------------------------------------------------------------------------
//Класс подключения к сокету
class TConnection
{
private:
	//Сокет
	SOCKET m_Socket;

	//Есть ли данные на сокете
	int m_ReadyData;

	//Флаг подключения
	bool m_Connected;

public:
	TConnection();
	~TConnection();

	//Подключены или нет
	bool Connected() const {return m_Connected;}

	//Инициализация
	int Init();

	//Подключение
	int Connect(const char *IP, int Port);

	//Разрыв соединения
	void Disconnect();

	//Проверка готовности данных
	bool DataReady();

	//Функция приема
	int Recv(PBYTE buf, int &size);

	//Отправить сообщение
	int Send(PBYTE buf, int size);
};
//---------------------------------------------------------------------------
#endif