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
	NoMove = true;
	Blocked = true;

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
		FrameRedraw = false;
		FrameCreated = false;

		return;
	}

	glNewList((GLuint)this, GL_COMPILE);

		UO->DrawResizepicGump(0x13EC, posX, posY, 400, 121 + (m_Count * 21)); //Body
		
		FontManager->DrawA(1, m_Text.c_str(), 0x0386, posX + 20, posY + 16);

		TGumpMenuObject *go = (TGumpMenuObject*)m_Items;
		int idx = 1;

		while (go != NULL)
		{
			int offs = 30 + (idx * 21);

			UO->DrawGump(0x138A, 0, posX + 20, posY + offs); //Button

			offs += 2;

			FontManager->DrawA(1, go->GetText().c_str(), 0x0386, posX + 50, posY + offs);

			idx++;
			go = (TGumpMenuObject*)go->m_Next;
		}

	glEndList();

	FrameCreated = true;
}
//----------------------------------------------------------------------------
int TGumpGrayMenu::Draw(bool &mode)
{
	DWORD index = (DWORD)this;

	//Нажата ли кнопка в окне?
	bool IsPressed = (g_LeftMouseDown && g_LastGumpLeftMouseDown == index && g_LastSelectedGump == index);

	//Может ли быть нажат элемент?
	int CanPressedButton = 0;
	if (IsPressed && g_LastObjectLeftMouseDown == g_LastSelectedObject)
		CanPressedButton = g_LastObjectLeftMouseDown;

	int posX = X;
	int posY = Y;

	int height = 121 + (m_Count * 21);

	if (mode)
	{
		if (!FrameCreated)
			GenerateFrame(posX, posY);

		glCallList((GLuint)index);

		TGumpMenuObject *go = (TGumpMenuObject*)m_Items;
		int idx = 1;

		while (go != NULL)
		{
			if (go->Pressed)
			{
				UO->DrawGump(0x138B, 0, posX + 20, posY + 30 + (idx * 21)); //Button

				break;
			}

			idx++;
			go = (TGumpMenuObject*)go->m_Next;
		}

		posY += (height - 44);
		
		UO->DrawGump(0x1450 + (int)(CanPressedButton == ID_GGM_CANCEL), 0, posX + 40, posY); //CANCEL

		UO->DrawGump(0x13B2 + (int)(CanPressedButton == ID_GGM_CONTINUE), 0, posX + 160, posY); //CONTINUE
	}
	else
	{
		int LSG = 0;

		//Если выбран основной гамп - меняем глобальный указатель на выбранный гамп на него
		if (UO->ResizepicPixelsInXY(0x13EC, posX, posY, 400, height))
		{
			g_LastSelectedObject = 0;
			g_LastSelectedGump = index;
		}
		
		int offs = posY + height - 44;

		if (UO->GumpPixelsInXY(0x1450, posX + 40, offs))
			LSG = ID_GGM_CANCEL; //CANCEL
		else if (UO->GumpPixelsInXY(0x13B2, posX + 160, offs))
			LSG = ID_GGM_CONTINUE; //CONTINUE
		else //Перебор кнопок
		{
			TGumpMenuObject *go = (TGumpMenuObject*)m_Items;
			int idx = 1;

			posX += 20;
			posY += 30;

			while (go != NULL)
			{
				if (UO->GumpPixelsInXY(0x138A, posX, posY + (idx * 21)))
				{
					LSG = idx;

					break;
				}

				idx++;
				go = (TGumpMenuObject*)go->m_Next;
			}
		}

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
		TGumpMenuObject *go = (TGumpMenuObject*)m_Items;
		int idx = 1;

		while (go != NULL)
		{
			if (go->Pressed)
			{
				SendMenuResponse(idx);

				return;
			}

			idx++;
			go = (TGumpMenuObject*)go->m_Next;
		}

		//Ничего не выбрали
		UO->CreateTextMessage(TT_SYSTEM, 0xFFFFFFFF, 3, 0x21, "You must choose the section of menu.");
	}
	else
	{
		TGumpMenuObject *go = (TGumpMenuObject*)m_Items;
		int idx = 1;

		while (go != NULL)
		{
			go->Pressed = (g_LastObjectLeftMouseDown == idx);

			idx++;
			go = (TGumpMenuObject*)go->m_Next;
		}
	}
}
//---------------------------------------------------------------------------