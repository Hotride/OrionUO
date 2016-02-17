/****************************************************************************
**
** ClilocManager.h
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
#ifndef ClilocManagerH
#define ClilocManagerH
//---------------------------------------------------------------------------
typedef std::map<DWORD, string> CLILOC_MAP;
//---------------------------------------------------------------------------
//Объект с данными клилок-файла
class TCliloc : public ICliloc, public TBaseQueueItem
{
private:
	//Системные клилоки (id < 1000000)
	CLILOC_MAP m_ClilocSystem;

	//Обычные клилоки (id >= 1000000 && id < 3000000)
	CLILOC_MAP m_ClilocRegular;

	//Клилоки для помощи (id >= 3000000)
	CLILOC_MAP m_ClilocSupport;

	//Указатель на проекцию файла в памяти (оно же ридер)
	TMappedHeader m_File;

	//Тип клилока
	string m_Language;

	//Статус загрузки
	bool m_Loaded;

	//Загрузить клилок
	string Load(DWORD &id);
public:
	TCliloc(const char *lang);
	virtual ~TCliloc();

	SETGET(string, Language);
	SETGET(bool, Loaded);

	//Получить ASCII строку по id (и загрузить при необходимости)
	string GetA(DWORD id, string result = "");

	//Получить Unicode строку по id (и загрузить при необходимости)
	wstring GetW(DWORD id, string result = "");
};
//---------------------------------------------------------------------------
//Менеджер клилоков
class TClilocManager : public IClilocManager, public TBaseQueue
{
private:
public:
	TClilocManager();
	virtual ~TClilocManager();

	//Получить ссылку на объект клилока (и загрузить при необходимости)
	TCliloc *Cliloc(const char *lang);
};
//---------------------------------------------------------------------------
extern TClilocManager *ClilocManager;
//---------------------------------------------------------------------------
#endif