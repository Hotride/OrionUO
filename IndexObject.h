/****************************************************************************
**
** IndexObject.h
**
** Copyright (C) January 2016 Hotride
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
#ifndef IndexObjectH
#define IndexObjectH
//---------------------------------------------------------------------------
//Класс для хранения информации о графических данных
class TIndexObject
{
private:
	//Адрес в памяти
	DWORD m_Address;

	//Размер данных
	DWORD m_Size;

	//Время последнего доступа
	DWORD m_LastAccessTime;

	//Габариты изображения
	short m_Width;
	short m_Height;

	//Стандартный цвет
	WORD m_Color;
public:
	TIndexObject();
	virtual ~TIndexObject();

	SETGET(DWORD, Address);
	SETGET(DWORD, Size);
	SETGET(DWORD, LastAccessTime);
	SETGET(short, Width);
	SETGET(short, Height);
	SETGET(WORD, Color);

	//Указатель на объект текстуры
	TTextureObject *Texture;
};
//---------------------------------------------------------------------------
//Класс для хранения информации о данных анимации статики
class TIndexObjectStatic : public TIndexObject
{
private:
	//Стартовый индекс картинки
	WORD m_Index;

	//Смещение относительно старта
	char m_Increment;

	//Индекс анимации
	char m_AnimIndex;

	//Время следующего изменения
	DWORD m_NextChange;

	//Цвет света (если это является источником света)
	WORD m_LightColor;
public:
	TIndexObjectStatic();

	SETGET(WORD, Index);
	SETGET(char, Increment);
	SETGET(char, AnimIndex);
	SETGET(DWORD, NextChange);
	SETGET(WORD, LightColor);
};
//---------------------------------------------------------------------------
//Класс для хранения информации о звуковых данных
class TIndexSound
{
private:
	//Адрес в памяти
	DWORD m_Address;

	//Размер данных
	DWORD m_Size;

	//Время выполнения
	DWORD m_Timer;

	//Время последнего доступа
	DWORD m_LastAccessTime;
public:
	TIndexSound();
	virtual ~TIndexSound();

	SETGET(DWORD, Address);
	SETGET(DWORD, Size);
	SETGET(DWORD, Timer);
	SETGET(DWORD, LastAccessTime);

	//Указатели на звук
	WaveFile waveFile;
	HSTREAM hStream;
};
//---------------------------------------------------------------------------
//Класс для хранения информации о анимации
class TIndexAnimation
{
private:
	//Адрес в памяти
	DWORD m_Address;

	//Смещение
	DWORD m_Offset;

	//Индекс картинки
	WORD m_Graphic;

	//Стандартный цвет
	WORD m_Color;
public:
	TIndexAnimation();
	virtual ~TIndexAnimation();

	//Указатель на группу анимаций
	TTextureAnimation *Group;

	SETGET(DWORD, Address);
	SETGET(DWORD, Offset);
	SETGET(WORD, Graphic);
	SETGET(WORD, Color);
};
//---------------------------------------------------------------------------
//Класс для хранения информации о мульти-объекте
class TIndexMulti
{
private:
	//Адрес в памяти
	DWORD m_Address;

	//Размер данных
	DWORD m_Size;

	//Количество объектов в мульти-объекте
	WORD m_Count;
public:
	TIndexMulti();
	virtual ~TIndexMulti();

	SETGET(DWORD, Address);
	SETGET(DWORD, Size);
	SETGET(WORD, Count);
};
//---------------------------------------------------------------------------
#endif