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
class TTooltip
{
private:
	DWORD m_Timer;
	DWORD m_SeqIndex;
	SELECT_OBJECT_TYPE m_Type;
	wstring m_Data;
	DWORD m_ClilocID;
	int m_MaxWidth;
	int m_X;
	int m_Y;
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

	TTextTexture Texture;

	void Set(wstring str, SELECT_OBJECT_TYPE type, DWORD seqIndex, int maxWidth = 0, int x = 0, int y = 0);
	void Set(DWORD clilocID, string str, SELECT_OBJECT_TYPE type, DWORD seqIndex, int maxWidth = 0, int x = 0, int y = 0);
	void Draw(int cursorWidth = 0, int cursorHeight = 0);
};

extern TTooltip Tooltip;
//---------------------------------------------------------------------------
#endif