// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
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
: CGUIPolygonal(GOT_SCISSOR, x, y, width, height), BaseX(baseX), BaseY(baseY)
{
	Enabled = enabled;
}
//----------------------------------------------------------------------------------
CGUIScissor::~CGUIScissor()
{
}
//----------------------------------------------------------------------------------
void CGUIScissor::Draw(const bool &checktrans)
{
	WISPFUN_DEBUG("c70_f1");
	if (Enabled)
	{
		if (!g_ConfigManager.GetUseGLListsForInterface())
		{
			int x = m_X;
			int y = m_Y;

			if (GumpParent != NULL)
			{
				x -= GumpParent->GumpRect.Position.X;
				y -= GumpParent->GumpRect.Position.Y;
			}

			g_GL.PushScissor(BaseX + x, BaseY + y, Width, Height);
		}
		else
			g_GL.PushScissor((int)g_GumpTranslate.X + BaseX + m_X, g_OrionWindow.GetSize().Height - ((int)g_GumpTranslate.Y + BaseY + m_Y) - Height, Width, Height);
	}
	else
		g_GL.PopScissor();
}
//----------------------------------------------------------------------------------
