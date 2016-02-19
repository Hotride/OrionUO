/****************************************************************************
**
** ServerList.h
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
#ifndef ServerListH
#define ServerListH
//---------------------------------------------------------------------------
//Класс для хранения списка серверов
class TServerList
{
private:
	//Количество серверов в списке
	int m_Count;

	//Выбранный сервер
	int m_SelectedServer;

	//Список индексов серверов
	PWORD m_Index;

	//Список имен серверов
	string *m_Name;
public:
	TServerList();
	~TServerList();

	SETGET(int, Count);
	SETGET(int, SelectedServer);

	//Выделить память под сервера
	void Init(int count);

	//Очистить списки серверов
	void Clear();

	//Указатель на текстуру текста
	TTextTexture *TextTexture;

	void SetServer(int pos, WORD index, string name);
	string GetServerName();

	WORD GetIndex(int pos) const;
	string GetName(int pos) const;
};
//---------------------------------------------------------------------------
extern TServerList ServerList;
//---------------------------------------------------------------------------
#endif