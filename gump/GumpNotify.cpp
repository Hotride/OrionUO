/****************************************************************************
**
** GumpNotify.h
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
//----------------------------------------------------------------------------
TGumpNotify::TGumpNotify(DWORD serial, short x, short y, BYTE variant, short width, short height, string text)
: TGump(GT_NOTIFY, serial, x, y), m_Variant(variant), m_Width(width),
m_Height(height), m_Text(text)
{
	Blocked = true;
	g_GrayMenuCount++;
}
//----------------------------------------------------------------------------
TGumpNotify::~TGumpNotify()
{
}
//----------------------------------------------------------------------------
void TGumpNotify::PrepareTextures()
{
	UO->ExecuteResizepic(0x0A28);
	UO->ExecuteButton(0x0481);
}
//----------------------------------------------------------------------------
void TGumpNotify::GenerateFrame()
{
	if (!g_DrawMode)
	{
		m_FrameRedraw = false;
		m_FrameCreated = false;

		return;
	}

	CalculateGumpState();

	glNewList((GLuint)this, GL_COMPILE);
	
		UO->DrawResizepicGump(0x0A28, 0, 0, m_Width, m_Height); //Background
		
		FontManager->DrawA(1, m_Text.c_str(), 0x0386, 40, 45, m_Width - 90);

		WORD gumpID = 0x0481 + (int)(g_GumpSelectElement == ID_GN_BUTTON_OK); //Button v / light
		if (g_GumpPressedElement == ID_GN_BUTTON_OK)
			gumpID = 0x0483; //Button v pressed

		UO->DrawGump(gumpID, 0, (m_Width / 2) - 13, m_Height - 45); //Button v

	glEndList();

	m_FrameRedraw = true;
	m_FrameCreated = true;
}
//----------------------------------------------------------------------------
int TGumpNotify::Draw(bool &mode)
{
	DWORD index = (DWORD)this;

	CalculateGumpState();

	if (mode)
	{
	}

	if (mode)
	{
		if (g_GumpPressed || !m_FrameCreated || g_GumpSelectElement)
			GenerateFrame();
		else if (m_FrameRedraw)
		{
			GenerateFrame();
			m_FrameRedraw = false;
		}

		glTranslatef(g_GumpTranslateX, g_GumpTranslateY, 0.0f);

		glCallList((GLuint)index);

		glTranslatef(-g_GumpTranslateX, -g_GumpTranslateY, 0.0f);
	}
	else
	{
		int oldMouseX = g_MouseX;
		int oldMouseY = g_MouseY;
		g_MouseX -= (int)g_GumpTranslateX;
		g_MouseY -= (int)g_GumpTranslateY;

		int LSG = 0;
		
		if (UO->ResizepicPixelsInXY(0x0A28, 0, 0, m_Width, m_Height))
		{
			g_LastSelectedObject = 0;
			g_LastSelectedGump = index;

			if (UO->GumpPixelsInXY(0x0481, (m_Width / 2) - 13, m_Height - 45))
			{
				LSG = ID_GN_BUTTON_OK;
				g_LastSelectedObject = ID_GN_BUTTON_OK;
			}
		}

		g_MouseX = oldMouseX;
		g_MouseY = oldMouseY;

		return LSG;
	}

	return 0;
}
//----------------------------------------------------------------------------
void TGumpNotify::OnLeftMouseUp()
{
	if (!g_LastObjectLeftMouseDown || g_LastObjectLeftMouseDown != g_LastSelectedObject)
		return;

	Process();
}
//----------------------------------------------------------------------------
void TGumpNotify::OnKeyPress(WPARAM &wparam, LPARAM &lparam)
{
	if (wparam == VK_RETURN)
	{
		if (ConfigManager.GetConsoleNeedEnter())
			EntryPointer = NULL;
		else
			EntryPointer = GameConsole;

		Process();
	}
}
//----------------------------------------------------------------------------
void TGumpNotify::Process()
{
	switch (m_Variant)
	{
		case 1:
		{
			GameScreen->CreateSmoothAction(TGameScreen::ID_SMOOTH_GS_LOGOUT);

			break;
		}
		case 2:
		{
			GumpManager->RemoveGump(this);

			break;
		}
		default:
			break;
	}
}
//----------------------------------------------------------------------------