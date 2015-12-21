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
class TCity;
//--------------------------------------------------------------------------
class TCityItem : public TBaseQueueItem
{
private:
	BYTE m_LocationIndex; //Индекс города
	string m_Name; //Имя города
	string m_Area; //Описание территории
	bool m_Selected; //Выбран ли этот город

public:
	TCityItem();
	virtual ~TCityItem();

	//Ссылка на описание города
	TCity *m_City;

	SETGET(BYTE, LocationIndex)
	SETGET(string, Name)
	SETGET(string, Area)
	SETGET(bool, Selected)

	//Это старая версия класса
	virtual bool IsNewCity() {return false;}

	//Инициализация
	void InitCity();
};
//--------------------------------------------------------------------------
class TCityItemNew : public TCityItem
{
private:
	//Координаты корода, зачем?
	DWORD m_X;
	DWORD m_Y;
	DWORD m_Z;
	//Индекс карты, зачем?
	DWORD m_MapIndex;
	//DWORD m_Cliloc;

public:
	TCityItemNew();
	virtual ~TCityItemNew();

	//Это новая версия класса	
	bool IsNewCity() {return true;}

	//Текстура текста
	TTextTexture m_Texture;

	SETGET(DWORD, X)
	SETGET(DWORD, Y)
	SETGET(DWORD, Z)
	SETGET(DWORD, MapIndex)
	//SETGET(DWORD, Cliloc)

	//Генерация текстуры по ИД клилока
	void CreateTextTexture(DWORD clilocID);
};
//--------------------------------------------------------------------------
class TCityList : public TBaseQueue
{
private:

public:
	TCityList();
	virtual ~TCityList();

	//Получить ссылку на город
	TCityItem *GetCity(int index);
};
//---------------------------------------------------------------------------
extern TCityList *CityList; //Ссылка на список городов
//---------------------------------------------------------------------------
#endif