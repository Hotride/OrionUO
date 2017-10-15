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
	{
		if (g_GL.CanUseFrameBuffer)
		{
			int x = m_X;
			int y = m_Y;

			if (m_GumpParent != NULL)
			{
				x -= m_GumpParent->GumpRect.Position.X;
				y -= m_GumpParent->GumpRect.Position.Y;
			}

			g_GL.PushScissor(m_BaseX + x, m_BaseY + y, m_Width, m_Height);
		}
		else
			g_GL.PushScissor((int)g_GumpTranslate.X + m_BaseX + m_X, g_OrionWindow.Size.Height - ((int)g_GumpTranslate.Y + m_BaseY + m_Y) - m_Height, m_Width, m_Height);
	}
	else
		g_GL.PopScissor();
}
//----------------------------------------------------------------------------------
