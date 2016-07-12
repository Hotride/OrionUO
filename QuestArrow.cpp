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
void TQuestArrow::Draw(const RENDER_VARIABLES_FOR_GAME_WINDOW &bounds)
{
	if (m_Enabled)
	{
		int dir = MouseManager.GetFacing(g_Player->X, g_Player->Y, m_X, m_Y, 0);

		WORD gumpID = m_Gump + ((dir + 1) % 8);

		POINT p = { 0 };
		UO->GetGumpDimension(gumpID, p);

		int gox = m_X - g_Player->X;
		int goy = m_Y - g_Player->Y;

		int x = bounds.GameWindowCenterX + ((gox - goy) * 22) - (p.x / 2);
		int y = bounds.GameWindowCenterY + ((gox + goy) * 22) - (p.y / 2);

		if (x < bounds.GameWindowPosX)
			x = bounds.GameWindowPosX;
		else if (x > bounds.GameWindowPosX + bounds.GameWindowSizeX - p.x)
			x = bounds.GameWindowPosX + bounds.GameWindowSizeX - p.x;

		if (y < bounds.GameWindowPosY)
			y = bounds.GameWindowPosY;
		else if (y > bounds.GameWindowPosY + bounds.GameWindowSizeY - p.y)
			y = bounds.GameWindowPosY + bounds.GameWindowSizeY - p.y;

		DWORD ticks = GetTickCount();

		if (m_Timer < ticks)
		{
			if (m_Timer + 120 < ticks)
				m_Timer = ticks + 1000;

			ColorizerShader->Use();

			UO->DrawGump(gumpID, 0x0021, x, y);

			UnuseShader();
		}
		else
			UO->DrawGump(gumpID, 0, x, y);
	}
}
//----------------------------------------------------------------------------