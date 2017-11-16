// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
/***********************************************************************************
**
** GUIScrollBackground.cpp
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#include "stdafx.h"
//----------------------------------------------------------------------------------
CGUIScrollBackground::CGUIScrollBackground(const uint &serial, const ushort &graphic, const int &x, const int &y, const int &height)
: CBaseGUI(GOT_SCROLLBACKGROUND, serial, graphic, 0, x, y), m_Height(height)
{
	WISPFUN_DEBUG("c71_f1");
	m_OffsetX = 0;
	m_BottomOffsetX = 0;
	m_Width = 0;
	int width = 0;

	CGLTexture *th[4] = { NULL };
	bool isValid = true;

	IFOR(i, 0, 4)
	{
		th[i] = g_Orion.ExecuteGump(m_Graphic + (int)i);

		if (th[i] != NULL)
		{
			if (width < th[i]->Width)
				width = th[i]->Width;
		}
		else
			isValid = false;
	}

	if (isValid)
	{
		m_OffsetX = (width - th[1]->Width) / 2;
		int offset = th[0]->Width - th[3]->Width;
		m_BottomOffsetX = (offset / 2) + (offset / 4);
		m_Width = width; // m_OffsetX + th[3]->Width;
	}

	UpdateHeight(m_Height);
}
//----------------------------------------------------------------------------------
CGUIScrollBackground::~CGUIScrollBackground()
{
}
//----------------------------------------------------------------------------------
void CGUIScrollBackground::UpdateHeight(const int &height)
{
	WISPFUN_DEBUG("c71_f2");
	m_Height = height;

	CGLTexture *th[4] = { NULL };

	IFOR(i, 0, 4)
	{
		th[i] = g_Orion.ExecuteGump(m_Graphic + (int)i);

		if (th[i] == NULL)
			return;
	}

	m_WorkSpace = WISP_GEOMETRY::CRect(m_OffsetX + 10, th[0]->Height, th[1]->Width - 20, m_Height - (th[0]->Height + th[3]->Height));
}
//----------------------------------------------------------------------------------
void CGUIScrollBackground::PrepareTextures()
{
	WISPFUN_DEBUG("c71_f3");
	g_Orion.ExecuteGumpPart(m_Graphic, 4);
}
//----------------------------------------------------------------------------------
void CGUIScrollBackground::Draw(const bool &checktrans)
{
	WISPFUN_DEBUG("c71_f4");
	CGLTexture *th[4] = { NULL };

	IFOR(i, 0, 4)
	{
		th[i] = g_Orion.ExecuteGump(m_Graphic + (int)i);

		if (th[i] == NULL)
			return;
	}

	th[0]->Draw(m_X, m_Y, checktrans); //Top scroll

	int currentY = th[0]->Height;
	int height = m_Height - th[3]->Height;

	int bodyX = m_X + m_OffsetX;

	bool exit = false;

	while (!exit)
	{
		IFOR(i, 1, 3)
		{
			int deltaHeight = height - currentY;

			if (deltaHeight < th[i]->Height)
			{
				if (deltaHeight > 0)
					th[i]->Draw(bodyX, m_Y + currentY, 0, deltaHeight, checktrans);

				exit = true;
				break;
			}
			else
				th[i]->Draw(bodyX, m_Y + currentY, checktrans);

			currentY += th[i]->Height;
		}
	}

	th[3]->Draw(m_X + m_BottomOffsetX, m_Y + m_Height - th[3]->Height, checktrans); //Bottom scroll
}
//----------------------------------------------------------------------------------
bool CGUIScrollBackground::Select()
{
	WISPFUN_DEBUG("c71_f5");
	int x = g_MouseManager.Position.X - m_X;
	int y = g_MouseManager.Position.Y - m_Y;

	if (!(x >= 0 && y >= 0 && x < m_Width && y < m_Height))
		return false;

	if (m_CheckPolygone)
		return true;

	bool select = false;

	CGLTexture *th[4] = { NULL };

	IFOR(i, 0, 4)
	{
		th[i] = g_Orion.ExecuteGump(m_Graphic + (int)i);

		if (th[i] == NULL)
			return false;
	}

	y = m_Y;

	select = g_Orion.GumpPixelsInXY(m_Graphic, m_X, y) || g_Orion.GumpPixelsInXY(m_Graphic + 3, m_X + m_BottomOffsetX, y + m_Height - th[3]->Height); //Top/Bottom scrolls

	x = m_X + m_OffsetX;

	int currentY = th[0]->Height;
	int height = m_Height - th[3]->Height;

	while (!select)
	{
		IFOR(i, 1, 3 && !select)
		{
			int deltaHeight = height - currentY;

			if (deltaHeight < th[i]->Height)
			{
				if (deltaHeight > 0)
					select = g_Orion.GumpPixelsInXY(m_Graphic + (int)i, x, y + currentY, 0, deltaHeight);

				return select;
			}
			else
				select = g_Orion.GumpPixelsInXY(m_Graphic + (int)i, x, y + currentY);

			currentY += th[i]->Height;
		}
	}

	return select;
}
//----------------------------------------------------------------------------------
