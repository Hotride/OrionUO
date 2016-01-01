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
void TGumpNotify::GenerateFrame(int posX, int posY)
{
	if (!g_DrawMode)
	{
		FrameRedraw = false;
		FrameCreated = false;

		return;
	}

	DWORD index = (DWORD)this;

	//Нажата ли кнопка в окне?
	bool IsPressed = (g_LeftMouseDown && g_LastGumpLeftMouseDown == index && g_LastSelectedGump == index);
	
	//Может ли быть подсвечен элемент?
	int CanSelectedButton = ((g_LastSelectedGump == index) ? g_LastSelectedObject : 0);

	//Может ли быть нажат элемент?
	int CanPressedButton = 0;
	if (IsPressed && g_LastObjectLeftMouseDown == g_LastSelectedObject)
		CanPressedButton = g_LastObjectLeftMouseDown;

	glNewList((GLuint)index, GL_COMPILE);
	
		UO->DrawResizepicGump(0x0A28, posX, posY, m_Width, m_Height); //Background
		
		FontManager->DrawA(1, m_Text.c_str(), 0x0386, posX + 40, posY + 45, m_Width - 90);

		WORD gumpID = 0x0481 + (int)(CanSelectedButton == ID_GN_BUTTON_OK); //Button v / light
		if (CanPressedButton == ID_GN_BUTTON_OK)
			gumpID = 0x0483; //Button v pressed

		UO->DrawGump(gumpID, 0, posX + (m_Width / 2) - 13, posY + m_Height - 45); //Button v

	glEndList();

	FrameRedraw = true;
	FrameCreated = true;
}
//----------------------------------------------------------------------------
int TGumpNotify::Draw(bool &mode)
{
	DWORD index = (DWORD)this;

	int posX = X;
	int posY = Y;
	
	//Нажата ли кнопка в окне?
	bool IsPressed = (g_LeftMouseDown && g_LastGumpLeftMouseDown == index && g_LastSelectedGump == index);
	
	//Может ли быть подсвечен элемент?
	int CanSelectedButton = ((g_LastSelectedGump == index) ? g_LastSelectedObject : 0);

	//Может ли быть нажат элемент?
	int CanPressedButton = 0;
	if (IsPressed && g_LastObjectLeftMouseDown == g_LastSelectedObject)
		CanPressedButton = g_LastObjectLeftMouseDown;

	if (g_LeftMouseDown && g_LastGumpLeftMouseDown == index && !g_LastObjectLeftMouseDown)
	{
		posX += (g_MouseX - g_DroppedLeftMouseX);
		posY += (g_MouseY - g_DroppedLeftMouseY);

		if (mode)
			GenerateFrame(posX, posY);
	}
	else if (mode)
	{
		if (IsPressed || CanSelectedButton)
			GenerateFrame(posX, posY);
		else if (FrameRedraw)
		{
			GenerateFrame(posX, posY);
			FrameRedraw = false;
		}
	}

	if (mode)
	{
		if (!FrameCreated)
			GenerateFrame(posX, posY);

		glCallList((GLuint)index);
	}
	else
	{
		int LSG = 0;
		
		if (UO->ResizepicPixelsInXY(0x0A28, posX, posY, m_Width, m_Height))
		{
			g_LastSelectedObject = 0;
			g_LastSelectedGump = index;

			if (UO->GumpPixelsInXY(0x0481, posX + (m_Width / 2) - 13, posY + m_Height - 45))
			{
				LSG = ID_GN_BUTTON_OK;
				g_LastSelectedObject = ID_GN_BUTTON_OK;
			}
		}

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
	switch (wparam)
	{
		case VK_RETURN:
		{
			if (ConfigManager.GetConsoleNeedEnter())
				EntryPointer = NULL;
			else
				EntryPointer = GameConsole;

			Process();

			break;
		}
		default:
			break;
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