/***********************************************************************************
**
** GUIScissor.cpp
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#include "stdafx.h"
//----------------------------------------------------------------------------------
CGUIScissor::CGUIScissor(const bool &enabled, const int &baseX, const int &baseY, const int &x, const int &y, const int &width, const int &height)
: CGUIPolygonal(GOT_SCISSOR, x, y, width, height), m_BaseX(baseX), m_BaseY(baseY)
{
	m_Enabled = enabled;
}
//----------------------------------------------------------------------------------
CGUIScissor::~CGUIScissor()
{
}
//----------------------------------------------------------------------------------
void CGUIScissor::Draw(const bool &checktrans)
{
	WISPFUN_DEBUG("c70_f1");
	if (m_Enabled)
		g_GL.PushScissor(m_BaseX + m_X, m_BaseY + m_Y, m_Width, m_Height);
	else
		g_GL.PopScissor();
}
//----------------------------------------------------------------------------------
