// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
/***********************************************************************************
**
** GUIGlobalColor.cpp
**
** Copyright (C) December 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#include "stdafx.h"
//----------------------------------------------------------------------------------
CGUIGlobalColor::CGUIGlobalColor(const bool &enabled, const GLfloat &r, const GLfloat &g, const GLfloat &b, const GLfloat &a)
: CBaseGUI(GOT_GLOBAL_COLOR, 0, 0, 0, 0, 0), R(r), G(g), B(b), A(a)
{
	Enabled = enabled;
}
//----------------------------------------------------------------------------------
CGUIGlobalColor::~CGUIGlobalColor()
{
}
//----------------------------------------------------------------------------------
void CGUIGlobalColor::Draw(const bool &checktrans)
{
	WISPFUN_DEBUG("c55_f1");
	if (Enabled)
		glColor4f(R, G, B, A);
}
//----------------------------------------------------------------------------------
