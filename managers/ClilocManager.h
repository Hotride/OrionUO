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
//!Класс с данными клилок-файла
class TCliloc : public TBaseQueueItem
{
private:
	//!Системные клилоки (id < 1000000)
	CLILOC_MAP m_ClilocSystem;

	//!Обычные клилоки (id >= 1000000 && id < 3000000)
	CLILOC_MAP m_ClilocRegular;

	//!Клилоки для помощи (id >= 3000000)
	CLILOC_MAP m_ClilocSupport;

	//!Указатель на проекцию файла в памяти (оно же ридер)
	TMappedHeader m_File;

	//!Тип клилока
	string m_Language;

	//!Статус загрузки
	bool m_Loaded;

	/*!
	Загрузить клилок
	@param [__in] id Индекс клилока
	@return Результат загрузки или сообщение с ошибкой
	*/
	string Load(__in DWORD &id);

public:
	TCliloc(string lang);
	virtual ~TCliloc();

	SETGET(string, Language);
	SETGET(bool, Loaded);

	/*!
	Получить ASCII строку по id (и загрузить при необходимости)
	@param [__in] id Индекс клилока
	@param [__in] result Стандартное сообщение, если клилок не был найден
	@return Полученный результат, замена или сообщение с ошибкой
	*/
	string GetA(__in DWORD id, __in string result = "");

	/*!
	Получить Unicode строку по id (и загрузить при необходимости)
	@param [__in] id Индекс клилока
	@param [__in] result Стандартное сообщение, если клилок не был найден
	@return Полученный результат, замена или сообщение с ошибкой
	*/
	wstring GetW(__in DWORD id, __in string result = "");
};
//---------------------------------------------------------------------------
//!Класс менеджера клилоков
class TClilocManager : public TBaseQueue
{
private:
	//!Ссылка на последний использованный клилок (для более быстрого доступа)
	TCliloc *m_LastCliloc;

public:
	TClilocManager();
	virtual ~TClilocManager();

	/*!
	Получить ссылку на объект клилока (и загрузить при необходимости)
	@param [__in] lang Расширение клилока
	@return Ссылка на клилок
	*/
	TCliloc *Cliloc(__in string lang);

	wstring ParseArgumentsToClilocString(DWORD cliloc, wstring args);
};
//---------------------------------------------------------------------------
//!Ссылка на менеджер клилоков
extern TClilocManager *ClilocManager;
//---------------------------------------------------------------------------
#endif