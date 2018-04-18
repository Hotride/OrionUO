﻿// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
/***********************************************************************************
**
** GUIResizepic.cpp
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#include "stdafx.h"
//----------------------------------------------------------------------------------
CGUIResizepic::CGUIResizepic(int serial, ushort graphic, int x, int y, int width, int height)
: CGUIPolygonal(GOT_RESIZEPIC, x, y, width, height)
{
	Serial = serial;
	Graphic = graphic;
}
//----------------------------------------------------------------------------------
CGUIResizepic::~CGUIResizepic()
{
}
//----------------------------------------------------------------------------------
void CGUIResizepic::PrepareTextures()
{
	WISPFUN_DEBUG("c69_f1");
	g_Orion.ExecuteResizepic(Graphic);
}
//----------------------------------------------------------------------------------
void CGUIResizepic::Draw(bool checktrans)
{
	WISPFUN_DEBUG("c69_f2");
	CGLTexture *th[9] = { NULL };

	IFOR(i, 0, 9)
	{
		CGLTexture *pth = g_Orion.ExecuteGump(Graphic + (int)i);

		if (pth == NULL)
			return;

		if (i == 4)
			th[8] = pth;
		else if (i > 4)
			th[i - 1] = pth;
		else
			th[i] = pth;
	}

	glUniform1iARB(g_ShaderDrawMode, SDM_NO_COLOR);

	if (checktrans)
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		g_GL_DrawResizepic(th, m_X, m_Y, Width, Height);

		glDisable(GL_BLEND);

		glEnable(GL_STENCIL_TEST);

		g_GL_DrawResizepic(th, m_X, m_Y, Width, Height);

		glDisable(GL_STENCIL_TEST);
	}
	else
		g_GL_DrawResizepic(th, m_X, m_Y, Width, Height);
}
//----------------------------------------------------------------------------------
bool CGUIResizepic::Select()
{
	WISPFUN_DEBUG("c69_f3");
	if (CheckPolygone)
		return CGUIPolygonal::Select();

	return g_Orion.ResizepicPixelsInXY(Graphic, m_X, m_Y, Width, Height);
}
//----------------------------------------------------------------------------------
