// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
/***********************************************************************************
**
** GUIChecktrans.cpp
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#include "stdafx.h"
//----------------------------------------------------------------------------------
CGUIChecktrans::CGUIChecktrans(int x, int y, int width, int height)
: CGUIPolygonal(GOT_CHECKTRANS, x, y, width, height)
{
}
//----------------------------------------------------------------------------------
CGUIChecktrans::~CGUIChecktrans()
{
}
//----------------------------------------------------------------------------------
void CGUIChecktrans::Draw(bool checktrans)
{
	WISPFUN_DEBUG("c47_f1");
	glColorMask(false, false, false, false);

	glStencilFunc(GL_ALWAYS, 1, 1);
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

	g_GL.DrawPolygone(m_X, m_Y, Width, Height);

	glColorMask(true, true, true, true);

	glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
	glStencilFunc(GL_NOTEQUAL, 1, 1);
}
//----------------------------------------------------------------------------------
