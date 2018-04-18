// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
/***********************************************************************************
**
** GUILine.cpp
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#include "stdafx.h"
//----------------------------------------------------------------------------------
CGUILine::CGUILine(int startX, int startY, int targetX, int targetY, int polygoneColor)
: CBaseGUI(GOT_LINE, 0, 0, 0, startX, startY), TargetX(targetX), TargetY(targetY)
{
	WISPFUN_DEBUG("c65_f1");
	ColorR = GetRValue(polygoneColor);
	ColorG = GetGValue(polygoneColor);
	ColorB = GetBValue(polygoneColor);
	ColorA = polygoneColor >> 24;

	if (!ColorA)
		ColorA = 0xFF;
}
//----------------------------------------------------------------------------------
CGUILine::~CGUILine()
{
}
//----------------------------------------------------------------------------------
void CGUILine::Draw(bool checktrans)
{
	WISPFUN_DEBUG("c65_f2");
	glColor4ub(ColorR, ColorG, ColorB, ColorA);

	if (ColorA < 0xFF)
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		g_GL.DrawLine(m_X, m_Y, TargetX, TargetY);

		glDisable(GL_BLEND);
	}
	else
		g_GL.DrawLine(m_X, m_Y, TargetX, TargetY);

	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
}
//----------------------------------------------------------------------------------
