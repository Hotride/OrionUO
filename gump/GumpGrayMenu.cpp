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
		UO->InitScreen(GS_GAME_BLOCKED);
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
	UO->ExecuteResizepic(0x13EC);
	UO->ExecuteGumpPart(0x138A, 2);
	UO->ExecuteGumpPart(0x1450, 2);
	UO->ExecuteGumpPart(0x13B2, 2);
}
//---------------------------------------------------------------------------
void TGumpGrayMenu::GenerateFrame(int posX, int posY)
{
	if (!g_DrawMode)
	{
		m_FrameRedraw = false;
		m_FrameCreated = false;

		return;
	}

	CalculateGumpState();

	glNewList((GLuint)this, GL_COMPILE);

		UO->DrawResizepicGump(0x13EC, 0, 0, 400, 121 + (m_Count * 21)); //Body
		
		FontManager->DrawA(1, m_Text.c_str(), 0x0386, 20, 16);

		int idx = 1;

		QFOR(go, m_Items, TGumpMenuObject*)
		{
			int offs = 30 + (idx * 21);

			UO->DrawGump(0x138A, 0, 20, offs); //Button

			offs += 2;

			FontManager->DrawA(1, go->GetText().c_str(), 0x0386, 50, offs);

			idx++;
		}

	glEndList();

	m_FrameCreated = true;
}
//----------------------------------------------------------------------------
int TGumpGrayMenu::Draw(bool &mode)
{
	CalculateGumpState();

	int height = 121 + (m_Count * 21);

	if (mode)
	{
		if (!m_FrameCreated)
			GenerateFrame(0, 0);

		glTranslatef(g_GumpTranslateX, g_GumpTranslateY, 0.0f);

		glCallList((GLuint)this);

		int idx = 1;

		QFOR(go, m_Items, TGumpMenuObject*)
		{
			if (go->Pressed)
			{
				UO->DrawGump(0x138B, 0, 20, 30 + (idx * 21)); //Button

				break;
			}

			idx++;
		}

		UO->DrawGump(0x1450 + (int)(g_GumpPressedElement == ID_GGM_CANCEL), 0, 40, height - 44); //CANCEL

		UO->DrawGump(0x13B2 + (int)(g_GumpPressedElement == ID_GGM_CONTINUE), 0, 160, height - 44); //CONTINUE

		glTranslatef(-g_GumpTranslateX, -g_GumpTranslateY, 0.0f);
	}
	else
	{
		int oldMouseX = g_MouseX;
		int oldMouseY = g_MouseY;
		g_MouseX -= (int)g_GumpTranslateX;
		g_MouseY -= (int)g_GumpTranslateY;

		int LSG = 0;

		//Если выбран основной гамп - меняем глобальный указатель на выбранный гамп на него
		if (UO->ResizepicPixelsInXY(0x13EC, 0, 0, 400, height))
		{
			g_LastSelectedObject = 0;
			g_LastSelectedGump = (DWORD)this;
		}
		
		int offs = height - 44;

		if (UO->GumpPixelsInXY(0x1450, 40, offs))
			LSG = ID_GGM_CANCEL; //CANCEL
		else if (UO->GumpPixelsInXY(0x13B2, 160, offs))
			LSG = ID_GGM_CONTINUE; //CONTINUE
		else //Перебор кнопок
		{
			int idx = 1;

			QFOR(go, m_Items, TGumpMenuObject*)
			{
				if (UO->GumpPixelsInXY(0x138A, 20, 30 + (idx * 21)))
				{
					LSG = idx;

					break;
				}

				idx++;
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
		UO->CreateTextMessage(TT_SYSTEM, 0xFFFFFFFF, 3, 0x21, "You must choose the section of menu.");
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