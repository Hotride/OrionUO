/****************************************************************************
**
** QuestArrow.h
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
#ifndef QuestArrowH
#define QuestArrowH
//---------------------------------------------------------------------------
class TQuestArrow
{
private:
	WORD m_X;
	WORD m_Y;
	bool m_Enabled;

public:
	TQuestArrow();
	~TQuestArrow();

	static const WORD m_Gump = 0x0194;

	SETGET(WORD, X)
	SETGET(WORD, Y)
	SETGET(bool, Enabled)

	void Draw(int &x, int &y);
};

extern TQuestArrow QuestArrow;
//---------------------------------------------------------------------------
#endif