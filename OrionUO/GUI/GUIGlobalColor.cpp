/***********************************************************************************
**
** GUIGlobalColor.cpp
**
** Copyright (C) December 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#include "GUIGlobalColor.h"
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
	if (m_Enabled)
		glColor4f(m_R, m_G, m_B, m_A);
}
//----------------------------------------------------------------------------------
