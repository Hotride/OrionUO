/****************************************************************************
**
** GumpPopupMenu.cpp
**
** Copyright (C) September 2015 Hotride
**
** This program is free software; you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation; either version 2 of the License, or
** (at your option) any later version.
**
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**
*****************************************************************************
*/
//---------------------------------------------------------------------------
#include "stdafx.h"
//---------------------------------------------------------------------------
TGumpPopupMenu *g_PopupMenu = NULL;
//---------------------------------------------------------------------------
TGumpPopupMenu::TGumpPopupMenu(DWORD serial, short x, short y)
: TGump(GT_POPUP_MENU, serial, x, y), m_Width(0), m_Height(0)
{
	m_NoMove = true;
	g_PopupMenu = this;
}
//---------------------------------------------------------------------------
TGumpPopupMenu::~TGumpPopupMenu()
{
	g_PopupMenu = NULL;
}
//---------------------------------------------------------------------------
void TGumpPopupMenu::CalculateGumpState()
{
	TGump::CalculateGumpState();

	g_GumpMovingOffsetX = 0;
	g_GumpMovingOffsetY = 0;

	g_GumpTranslateX = (float)m_X;
	g_GumpTranslateY = (float)m_Y;
}
//---------------------------------------------------------------------------
void TGumpPopupMenu::PrepareTextures()
{
	UO->ExecuteResizepic(0x0A3C);
}
//---------------------------------------------------------------------------
void TGumpPopupMenu::GenerateFrame()
{
	if (!g_DrawMode)
	{
		m_FrameRedraw = false;
		m_FrameCreated = false;

		return;
	}

	CalculateGumpState();

	glNewList((GLuint)this, GL_COMPILE);

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glColor4f(1.0f, 1.0f, 1.0f, 0.5f);

		UO->DrawResizepicGump(0x0A3C, 0, 0, m_Width, m_Height); //Body

		glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
		glDisable(GL_BLEND);
		
		int offsetY = 10;
		int index = 1;

		QFOR(item, m_Items, TGumpText*)
		{
			if (index == g_GumpSelectElement)
			{
				glColor4f(0.4f, 0.4f, 0.4f, 1.0f);
				g_GL.DrawPolygone(10, offsetY, m_Width - 20, item->m_Text.Height);
				glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
			}

			item->m_Text.Draw(10, offsetY);

			offsetY += item->m_Text.Height;
			index++;
		}

	glEndList();

	m_FrameCreated = true;
	m_FrameRedraw = true;
}
//----------------------------------------------------------------------------
int TGumpPopupMenu::Draw(bool &mode)
{
	CalculateGumpState();

	if (mode)
	{
		if (!m_FrameCreated || g_GumpSelectElement)
			GenerateFrame();
		else if (m_FrameRedraw)
		{
			GenerateFrame();
			m_FrameRedraw = false;
		}

		glTranslatef(g_GumpTranslateX, g_GumpTranslateY, 0.0f);

		glCallList((GLuint)this);

		glTranslatef(-g_GumpTranslateX, -g_GumpTranslateY, 0.0f);
	}
	else
	{
		int oldMouseX = g_MouseX;
		int oldMouseY = g_MouseY;
		g_MouseX -= (int)g_GumpTranslateX;
		g_MouseY -= (int)g_GumpTranslateY;

		int LSG = 0;

		int gumpHeight = 121;

		if (UO->ResizepicPixelsInXY(0x0A3C, 0, 0, m_Width, m_Height))
		{
			g_LastSelectedObject = 0;
			g_LastSelectedGump = (DWORD)this;

			int offsetY = 10;
			int width = m_Width - 20;
			int index = 1;

			QFOR(item, m_Items, TGumpText*)
			{
				if (UO->PolygonePixelsInXY(10, offsetY, width, item->m_Text.Height))
				{
					LSG = index;

					break;
				}

				offsetY += item->m_Text.Height;
				index++;
			}
		}
		
		g_MouseX = oldMouseX;
		g_MouseY = oldMouseY;

		if (LSG != 0)
			g_LastSelectedObject = LSG; //Если что-то нашлось - выбираем

		return LSG;
	}

	return 0;
}
//----------------------------------------------------------------------------
void TGumpPopupMenu::OnLeftMouseUp()
{
	if (g_LastObjectLeftMouseDown == g_LastSelectedObject)
	{
		TGumpText *item = (TGumpText*)Get(g_LastObjectLeftMouseDown - 1);

		if (item != NULL)
		{
			TPacketPopupMenuSelection packet(m_Serial, item->TextIndex);
			packet.Send();
		}
	}

	GumpManager->RemoveGump(this);
}
//---------------------------------------------------------------------------