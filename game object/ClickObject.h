/****************************************************************************
**
** ClickObject.h
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
#ifndef ClickObjectH
#define ClickObjectH
//---------------------------------------------------------------------------
class TClickObject
{
private:
	CLICK_OBJECT_TYPE m_Type; //Тип объекта клика
	DWORD m_Serial; //Серийник
	DWORD m_GumpID; //ИД гампп
	DWORD m_Timer; //Таймер клика
	GUMP_TYPE m_GumpType; //Тип гампа (если это гамп)
	WORD m_X; //Координата X
	WORD m_Y; //Координата Y
	char m_Z; //Координата Z
	int m_GumpButtonID; //Индекс кнопки в гампе для клика (если это гамп)

public:
	TClickObject();
	~TClickObject() {}

	SETGET(CLICK_OBJECT_TYPE, Type);
	SETGET(DWORD, Serial);
	SETGET(DWORD, GumpID);
	SETGET(DWORD, Timer);
	SETGET(GUMP_TYPE, GumpType);
	SETGET(WORD, X);
	SETGET(WORD, Y);
	SETGET(char, Z);
	SETGET(int, GumpButtonID);

	//Инициализация под тип объекта
	void Init(CLICK_OBJECT_TYPE type);
};
//---------------------------------------------------------------------------
extern TClickObject g_ClickObject; //Ссылка на объект клика
//---------------------------------------------------------------------------
#endif