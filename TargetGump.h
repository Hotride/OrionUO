/****************************************************************************
**
** TargetGump.h
**
** Copyright (C) February 2016 Hotride
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
#ifndef TargetGumpH
#define TargetGumpH
//---------------------------------------------------------------------------
class TTargetGump
{
protected:
	int m_X;
	int m_Y;
	int m_Hits;
	WORD m_Color;

public:
	TTargetGump();
	~TTargetGump();

	SETGET(int, X);
	SETGET(int, Y);
	SETGET(int, Hits);
	SETGET(WORD, Color);

	void Draw();
};
//---------------------------------------------------------------------------
class TNewTargetSystem
{
protected:
	DWORD m_Serial;
	int m_X;
	int m_TopY;
	int m_BottomY;
	int m_GumpX;
	int m_GumpY;
	int m_Hits;
	WORD m_GumpTop;
	WORD m_GumpBottom;
	WORD m_ColorGump;

public:
	TNewTargetSystem();
	~TNewTargetSystem();

	SETGET(DWORD, Serial);
	SETGET(int, X);
	SETGET(int, TopY);
	SETGET(int, BottomY);
	SETGET(int, GumpX);
	SETGET(int, GumpY);
	SETGET(int, Hits);
	SETGET(WORD, GumpTop);
	SETGET(WORD, GumpBottom);
	SETGET(WORD, ColorGump);

	void Draw();
};
//---------------------------------------------------------------------------
extern TTargetGump TargetGump;
extern TTargetGump AttackTargetGump;
extern TNewTargetSystem NewTargetSystem;
//---------------------------------------------------------------------------
#endif