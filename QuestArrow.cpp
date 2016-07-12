/****************************************************************************
**
** QuestArrow.cpp
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
//----------------------------------------------------------------------------
#include "stdafx.h"

TQuestArrow QuestArrow;
//----------------------------------------------------------------------------
TQuestArrow::TQuestArrow()
: m_X(0), m_Y(0), m_Enabled(false)
{
}
//----------------------------------------------------------------------------
TQuestArrow::~TQuestArrow()
{
}
//--------------------------------------------------------------------------
void TQuestArrow::Draw(int &x, int &y)
{
	UO->DrawGump(0x0194, 0, 100, 100);

	if (m_Enabled)
	{
		int dir = MouseManager.GetFacing(g_Player->X, g_Player->Y, m_X, m_Y, 0);

		WORD gumpID = m_Gump + dir;
		POINT p;

		UO->GetGumpDimension(gumpID, p);

		UO->DrawGump(gumpID, 0, x, y);
		//UO->DrawGump(gumpID, 0, x + (p.x / 2), y + (p.y / 2));
	}
}
//----------------------------------------------------------------------------