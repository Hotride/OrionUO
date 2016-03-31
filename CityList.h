/****************************************************************************
**
** CityList.h
**
** Copyright (C) October 2015 Hotride
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
#ifndef CityListH
#define CityListH
//--------------------------------------------------------------------------
//!Класс города (для старых клиентов)
class TCityItem : public TBaseQueueItem
{
private:
	//!Индекс города
	BYTE m_LocationIndex;

	//!Имя города
	string m_Name;

	//!Описание территории
	string m_Area;

	//!Выбран ли этот город
	bool m_Selected;

public:
	TCityItem();
	virtual ~TCityItem();

	//!Ссылка на описание города
	class TCity *m_City;

	SETGET(BYTE, LocationIndex);
	SETGET(string, Name);
	SETGET(string, Area);
	SETGET(bool, Selected);

	/*!
	Определение версии класса
	@return Новая или старая
	*/
	virtual bool IsNewCity() {return false;}

	/*!
	Инициализация
	@return 
	*/
	void InitCity();
};
//--------------------------------------------------------------------------
//!Класс города (для новых клиентов)
class TCityItemNew : public TCityItem
{
private:
	//!Координаты корода, зачем?
	DWORD m_X;
	DWORD m_Y;
	DWORD m_Z;

	//!Индекс карты, зачем?
	DWORD m_MapIndex;

	//DWORD m_Cliloc;

public:
	TCityItemNew();
	virtual ~TCityItemNew();

	//!Текстура текста
	TTextTexture m_Texture;

	SETGET(DWORD, X);
	SETGET(DWORD, Y);
	SETGET(DWORD, Z);
	SETGET(DWORD, MapIndex);
	//SETGET(DWORD, Cliloc);

	/*!
	Определение версии класса
	@return Новая или старая
	*/
	bool IsNewCity() { return true; }

	/*!
	Генерация текстуры по ИД клилока
	@param [__in] clilocID Индекс клилока
	@return 
	*/
	void CreateTextTexture(__in DWORD clilocID);
};
//--------------------------------------------------------------------------
//!Класс списка городов
class TCityList : public TBaseQueue
{
public:
	TCityList();
	virtual ~TCityList();

	/*!
	Получить ссылку на город
	@param [__in] index Индекс города
	@return 
	*/
	TCityItem *GetCity(__in int index);
};
//---------------------------------------------------------------------------
//!Ссылка на список городов
extern TCityList *CityList;
//---------------------------------------------------------------------------
#endif