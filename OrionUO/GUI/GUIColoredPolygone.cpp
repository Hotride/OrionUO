// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
/***********************************************************************************
**
** GUIColoredPolygone.cpp
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#include "stdafx.h"
//----------------------------------------------------------------------------------
CGUIColoredPolygone::CGUIColoredPolygone(int serial, ushort color, int x, int y, int width, int height, int polygoneColor)
: CGUIPolygonal(GOT_COLOREDPOLYGONE, x, y, width, height)
{
	WISPFUN_DEBUG("c48_f1");
	Serial = serial;
	UpdateColor(color, polygoneColor);
}
//----------------------------------------------------------------------------------
CGUIColoredPolygone::~CGUIColoredPolygone()
{
}
//----------------------------------------------------------------------------------
void CGUIColoredPolygone::UpdateColor(ushort color, int polygoneColor)
{
	WISPFUN_DEBUG("c48_f2");
	Color = color;

	ColorR = GetRValue(polygoneColor);
	ColorG = GetGValue(polygoneColor);
	ColorB = GetBValue(polygoneColor);
	ColorA = polygoneColor >> 24;

	if (!ColorA)
		ColorA = 0xFF;
}
//----------------------------------------------------------------------------------
void CGUIColoredPolygone::Draw(bool checktrans)
{
	WISPFUN_DEBUG("c48_f3");
	glColor4ub(ColorR, ColorG, ColorB, ColorA);

	if (ColorA < 0xFF)
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		g_GL.DrawPolygone(m_X, m_Y, Width, Height);

		glDisable(GL_BLEND);
	}
	else
		g_GL.DrawPolygone(m_X, m_Y, Width, Height);

	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

	if (Focused || (DrawDot && g_GumpSelectedElement == this))
		g_GL.DrawPolygone(m_X + (Width / 2) - 1, m_Y + (Height / 2) - 1, 2, 2);
}
//----------------------------------------------------------------------------------
void CGUIColoredPolygone::OnMouseEnter()
{
	WISPFUN_DEBUG("c48_f4");
	if (DrawDot && g_SelectedObject.Gump != NULL)
		g_SelectedObject.Gump->WantRedraw = true;
}
//----------------------------------------------------------------------------------
void CGUIColoredPolygone::OnMouseExit()
{
	WISPFUN_DEBUG("c48_f5");
	if (DrawDot && g_LastSelectedObject.Gump != NULL)
		g_LastSelectedObject.Gump->WantRedraw = true;
}
//----------------------------------------------------------------------------------
