// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
/***********************************************************************************
**
** QuestArrow.cpp
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#include "stdafx.h"
//----------------------------------------------------------------------------------
CQuestArrow g_QuestArrow;
//----------------------------------------------------------------------------------
CQuestArrow::CQuestArrow()
{
}
//----------------------------------------------------------------------------------
CQuestArrow::~CQuestArrow()
{
}
//----------------------------------------------------------------------------------
void CQuestArrow::Draw()
{
	WISPFUN_DEBUG("c200_f1");
	if (Enabled)
	{
		int dir = g_MouseManager.GetFacing(g_Player->X, g_Player->Y, X, Y, 0);

		ushort gumpID = m_Gump + ((dir + 1) % 8);

		WISP_GEOMETRY::CSize size = g_Orion.GetGumpDimension(gumpID);

		int gox = X - g_Player->X;
		int goy = Y - g_Player->Y;

		int x = g_RenderBounds.GameWindowCenterX + ((gox - goy) * 22) - (size.Width / 2);
		int y = g_RenderBounds.GameWindowCenterY + ((gox + goy) * 22) - (size.Height / 2);

		if (x < g_RenderBounds.GameWindowPosX)
			x = g_RenderBounds.GameWindowPosX;
		else if (x > g_RenderBounds.GameWindowPosX + g_RenderBounds.GameWindowWidth - size.Width)
			x = g_RenderBounds.GameWindowPosX + g_RenderBounds.GameWindowWidth - size.Width;

		if (y < g_RenderBounds.GameWindowPosY)
			y = g_RenderBounds.GameWindowPosY;
		else if (y > g_RenderBounds.GameWindowPosY + g_RenderBounds.GameWindowHeight - size.Height)
			y = g_RenderBounds.GameWindowPosY + g_RenderBounds.GameWindowHeight - size.Height;

		if (Timer < g_Ticks)
		{
			if (Timer + 120 < g_Ticks)
				Timer = g_Ticks + 1000;

			g_ColorizerShader.Use();

			g_Orion.DrawGump(gumpID, 0x0021, x, y);

			UnuseShader();
		}
		else
			g_Orion.DrawGump(gumpID, 0, x, y);
	}
}
//----------------------------------------------------------------------------------
