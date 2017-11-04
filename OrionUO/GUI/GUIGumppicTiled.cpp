// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
/***********************************************************************************
**
** GUIGumppicTiled.cpp
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#include "stdafx.h"
//----------------------------------------------------------------------------------
CGUIGumppicTiled::CGUIGumppicTiled(const ushort &graphic, const int &x, const int &y, const int &width, const int &height)
: CGUIDrawObject(GOT_GUMPPICTILED, 0, graphic, 0, x, y), m_Width(width),
m_Height(height)
{
}
//----------------------------------------------------------------------------------
CGUIGumppicTiled::~CGUIGumppicTiled()
{
}
//----------------------------------------------------------------------------------
void CGUIGumppicTiled::Draw(const bool &checktrans)
{
	WISPFUN_DEBUG("c57_f1");
	CGLTexture *th = g_Orion.ExecuteGump(m_Graphic);

	if (th != NULL)
	{
		SetShaderMode();

		th->Draw(m_X, m_Y, m_Width, m_Height, checktrans);
	}
}
//----------------------------------------------------------------------------------
bool CGUIGumppicTiled::Select()
{
	WISPFUN_DEBUG("c57_f2");
	int x = g_MouseManager.Position.X - m_X;
	int y = g_MouseManager.Position.Y - m_Y;

	if (x < 0 || y < 0 || (m_Width > 0 && x >= m_Width) || (m_Height > 0 && y >= m_Height))
		return false;

	CGLTexture *th = g_Orion.ExecuteGump(m_Graphic);

	if (th != NULL)
	{
		int width = m_Width;
		int height = m_Height;

		if (width == 0)
			width = th->Width;

		if (height == 0)
			height = th->Height;

		while (x > th->Width && width > th->Width)
		{
			x -= th->Width;
			width -= th->Width;
		}

		while (y > th->Height && height > th->Height)
		{
			y -= th->Height;
			height -= th->Height;
		}

		if (x > width || y > height)
			return false;

		if (x >= 0 && y >= 0 && x < th->Width && y < th->Height)
		{
			if (m_CheckPolygone)
				return true;

			int pos = (y * th->Width) + x;

			if (pos < (int)th->m_HitMap.size())
				return (th->m_HitMap[pos] != 0);
		}
	}

	return false;
}
//----------------------------------------------------------------------------------
