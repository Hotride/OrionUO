/****************************************************************************
**
** Tooltip.h
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
#ifndef TooltipH
#define TooltipH
//---------------------------------------------------------------------------
//Класс для работы с тултипами
class TTooltip
{
private:
	//Время фокусировки
	DWORD m_Timer;

	//Индекс объекта, на который навели мышкой
	DWORD m_SeqIndex;

	//Тип объекта
	SELECT_OBJECT_TYPE m_Type;

	//Данные в Unicode строки
	wstring m_Data;

	//Индекс клилока
	DWORD m_ClilocID;

	//Максимальная ширина
	int m_MaxWidth;

	//Экранные координаты
	int m_X;
	int m_Y;

	//Флаг использования тултипа
	bool m_Use;
public:
	TTooltip();
	~TTooltip();

	SETGET(DWORD, Timer);
	SETGET(DWORD, SeqIndex);
	SETGET(SELECT_OBJECT_TYPE, Type);
	SETGET(wstring, Data);
	SETGET(DWORD, ClilocID);
	SETGET(int, MaxWidth);
	SETGET(int, X);
	SETGET(int, Y);
	SETGET(bool, Use);

	//Текстура тулпита
	TTextTexture Texture;

	//Установить тултип из строки
	void Set(wstring str, SELECT_OBJECT_TYPE type, DWORD seqIndex, int maxWidth = 0, int x = 0, int y = 0);

	//Установить тултип из клилока
	void Set(DWORD clilocID, string str, SELECT_OBJECT_TYPE type, DWORD seqIndex, int maxWidth = 0, int x = 0, int y = 0);

	//Отрисовать тултип
	void Draw(int cursorWidth = 0, int cursorHeight = 0);
};
//---------------------------------------------------------------------------
extern TTooltip Tooltip;
//---------------------------------------------------------------------------
#endif