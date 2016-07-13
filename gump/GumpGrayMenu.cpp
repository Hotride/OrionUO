/****************************************************************************
**
** GumpGrayMenu.cpp
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
TGumpGrayMenu::TGumpGrayMenu(DWORD serial, short x, short y, string text)
: TGump(GT_GRAY_MENU, serial, x, y), m_Count(0), m_Text(text)
{
	m_NoMove = true;
	m_Blocked = true;

	if (!g_GrayMenuCount)
	{
		Orion->InitScreen(GS_GAME_BLOCKED);
		GameBlockedScreen->Code = 1;
	}

	g_GrayMenuCount++;
}
//---------------------------------------------------------------------------
TGumpGrayMenu::~TGumpGrayMenu()
{
}
//---------------------------------------------------------------------------
void TGumpGrayMenu::PrepareTextures()
{
	Orion->ExecuteResizepic(0x13EC);
	Orion->ExecuteGumpPart(0x138A, 2);
	Orion->ExecuteGumpPart(0x1450, 2);
	Orion->ExecuteGumpPart(0x13B2, 2);
}
//---------------------------------------------------------------------------
void TGumpGrayMenu::CalculateGumpState()
{
	TGump::CalculateGumpState();

	g_GumpMovingOffsetX = 0;
	g_GumpMovingOffsetY = 0;

	g_GumpTranslateX = (float)m_X;
	g_GumpTranslateY = (float)m_Y;
}
//---------------------------------------------------------------------------
void TGumpGrayMenu::GenerateFrame()
{
	if (!g_DrawMode)
	{
		m_FrameRedraw = false;
		m_FrameCreated = false;

		return;
	}

	CalculateGumpState();

	glNewList((GLuint)this, GL_COMPILE);

		int gumpHeight = 121;

		QFOR(go, m_Items, TGumpMenuObject*)
		{
			int addHeight = go->Texture.Height;

			if (addHeight < 21)
				addHeight = 21;

			gumpHeight += addHeight;
		}

		Orion->DrawResizepicGump(0x13EC, 0, 0, 400, gumpHeight); //Body
		
		FontManager->DrawA(1, m_Text.c_str(), 0x0386, 20, 16);

		int offsetY = 51;

		QFOR(go, m_Items, TGumpMenuObject*)
		{
			Orion->DrawGump(0x138A + (int)go->Pressed, 0, 20, offsetY); //Button

			offsetY += 2;

			go->Texture.Draw(50, offsetY);

			int addHeight = go->Texture.Height;

			if (addHeight < 21)
				addHeight = 21;

			offsetY += addHeight;
		}

		offsetY += 5;

		Orion->DrawGump(0x1450 + (int)(g_GumpPressedElement == ID_GGM_CANCEL), 0, 70, offsetY); //CANCEL

		Orion->DrawGump(0x13B2 + (int)(g_GumpPressedElement == ID_GGM_CONTINUE), 0, 200, offsetY); //CONTINUE

	glEndList();

	m_FrameCreated = true;
	m_FrameRedraw = true;
}
//----------------------------------------------------------------------------
int TGumpGrayMenu::Draw(bool &mode)
{
	CalculateGumpState();

	if (mode)
	{
		if (!m_FrameCreated || g_GumpPressedElement)
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

		QFOR(go, m_Items, TGumpMenuObject*)
		{
			int addHeight = go->Texture.Height;

			if (addHeight < 21)
				addHeight = 21;

			gumpHeight += addHeight;
		}

		//Если выбран основной гамп - меняем глобальный указатель на выбранный гамп на него
		if (Orion->ResizepicPixelsInXY(0x13EC, 0, 0, 400, gumpHeight))
		{
			g_LastSelectedObject = 0;
			g_LastSelectedGump = (DWORD)this;
		}

		//Перебор кнопок
		int idx = 1;
		int offsetY = 51;

		QFOR(go, m_Items, TGumpMenuObject*)
		{
			if (Orion->GumpPixelsInXY(0x138A, 20, offsetY))
			{
				LSG = idx;

				break;
			}

			offsetY += 2;

			int addHeight = go->Texture.Height;

			if (addHeight < 21)
				addHeight = 21;

			offsetY += addHeight;

			idx++;
		}

		if (!LSG)
		{
			offsetY += 5;

			if (Orion->GumpPixelsInXY(0x1450, 70, offsetY))
				LSG = ID_GGM_CANCEL; //CANCEL
			else if (Orion->GumpPixelsInXY(0x13B2, 200, offsetY))
				LSG = ID_GGM_CONTINUE; //CONTINUE
		}
		
		g_MouseX = oldMouseX;
		g_MouseY = oldMouseY;

		return LSG;
	}

	return 0;
}
//----------------------------------------------------------------------------
void TGumpGrayMenu::OnLeftMouseUp()
{
	if (g_LastObjectLeftMouseDown != g_LastSelectedObject)
		return;
	
	if (g_LastObjectLeftMouseDown == ID_GGM_CANCEL)
		SendMenuResponse(0);
	else if (g_LastObjectLeftMouseDown == ID_GGM_CONTINUE)
	{
		int idx = 1;

		QFOR(go, m_Items, TGumpMenuObject*)
		{
			if (go->Pressed)
			{
				SendMenuResponse(idx);

				return;
			}

			idx++;
		}

		//Ничего не выбрали
		Orion->CreateTextMessage(TT_SYSTEM, 0xFFFFFFFF, 3, 0x21, "You must choose the section of menu.");
	}
	else
	{
		int idx = 1;

		QFOR(go, m_Items, TGumpMenuObject*)
		{
			go->Pressed = (g_LastObjectLeftMouseDown == idx);

			idx++;
		}
	}
}
//---------------------------------------------------------------------------