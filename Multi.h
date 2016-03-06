/****************************************************************************
**
** Multi.h
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
#ifndef MultiH
#define MultiH
//---------------------------------------------------------------------------
//Объект для мульти-объекта
class TMultiObject : public TRenderStaticObject
{
private:
	//Фгали объекта (2 - мульти с таргета)
	DWORD m_MultiFlags;

public:
	TMultiObject(WORD graphic, short x, short y, char z, DWORD flags);
	virtual ~TMultiObject();
	
	SETGET(DWORD, MultiFlags);
	SETGET(char, CanBeTransparent);

	//Отрисовать объект
	virtual int Draw(bool &mode, int &drawX, int &drawY, DWORD &ticks);

	//Это объект мульти
	bool IsMultiObject() {return true;}
};
//---------------------------------------------------------------------------
//Класс мульти-объекта
class TMulti : public TBaseQueueItem
{
private:
	//Координаты центра
	short m_X;
	short m_Y;

	//Минимальные координаты края мульти-объекта
	short m_MinX;
	short m_MinY;

	//Максимальные координаты края мульти-объекта
	short m_MaxX;
	short m_MaxY;

public:
	TMulti(short x, short y);
	virtual ~TMulti();
	
	SETGET(short, X);
	SETGET(short, Y);
	SETGET(short, MinX);
	SETGET(short, MinY);
	SETGET(short, MaxX);
	SETGET(short, MaxY);
};
//---------------------------------------------------------------------------
#endif