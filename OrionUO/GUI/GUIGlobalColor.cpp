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
: CBaseGUI(GOT_GLOBAL_COLOR, 0, 0, 0, 0, 0), m_R(r), m_G(g), m_B(b), m_A(a)
{
	m_Enabled = enabled;
}
//----------------------------------------------------------------------------------
CGUIGlobalColor::~CGUIGlobalColor()
{
}
//----------------------------------------------------------------------------------
void CGUIGlobalColor::Draw(const bool &checktrans)
{
	WISPFUN_DEBUG("c55_f1");
	if (m_Enabled)
		glColor4f(m_R, m_G, m_B, m_A);
}
//----------------------------------------------------------------------------------
