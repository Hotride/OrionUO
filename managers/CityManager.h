/****************************************************************************
**
** CityManager.h
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
#ifndef CityManagerH
#define CityManagerH
//---------------------------------------------------------------------------
//!Класс с данными о городе
class TCity : public TBaseQueueItem
{
private:
	//!Название города
	string m_Name;

public:
	TCity(string name);
	virtual ~TCity();

	SETGET(string, Name);

	//!Указатель на текстуру текста
	TTextTexture m_Texture;
};
//---------------------------------------------------------------------------
//!Класс менеджера списка городов
class TCityManager : public TBaseQueue
{
public:
	TCityManager();
	virtual ~TCityManager();

	/*!
	Получить указатель на город
	@param [__in] name Имя города
	@return Ссылка на город или NULL
	*/
	TCity *GetCity(__in string name);
};
//---------------------------------------------------------------------------
//!Ссылка на менеджер городов
extern TCityManager *CityManager;
//---------------------------------------------------------------------------
#endif