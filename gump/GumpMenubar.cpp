/****************************************************************************
**
** MenubarGump.cpp
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
//----------------------------------------------------------------------------
#include "stdafx.h"

TTextTexture TGumpMenubar::m_Texture[8][2];
//---------------------------------------------------------------------------
TGumpMenubar::TGumpMenubar(DWORD serial, short x, short y)
: TGump(GT_MENUBAR, serial, x, y), m_Opened(true)
{
}
//---------------------------------------------------------------------------
TGumpMenubar::~TGumpMenubar()
{
}
//---------------------------------------------------------------------------
void TGumpMenubar::InitTextTextures()
{
	TTextureObject *th1 = UO->ExecuteGump(0x098B);
	TTextureObject *th2 = UO->ExecuteGump(0x098D);
	
	int smallWidth = 50;
	if (th1 != NULL)
		smallWidth = th1->Width;

	int largeWidth = 100;
	if (th2 != NULL)
		largeWidth = th2->Width;

	WORD yellowTextColor = 0x0036;

	wstring str = L"Map";
	FontManager->GenerateW(1, m_Texture[0][0], str.c_str(), 0, 30, smallWidth, TS_CENTER);
	FontManager->GenerateW(1, m_Texture[0][1], str.c_str(), yellowTextColor, 30, smallWidth, TS_CENTER);
	
	str = L"Paperdoll";
	FontManager->GenerateW(1, m_Texture[1][0], str.c_str(), 0, 30, largeWidth, TS_CENTER);
	FontManager->GenerateW(1, m_Texture[1][1], str.c_str(), yellowTextColor, 30, largeWidth, TS_CENTER);
	
	str = L"Inventory";
	FontManager->GenerateW(1, m_Texture[2][0], str.c_str(), 0, 30, largeWidth, TS_CENTER);
	FontManager->GenerateW(1, m_Texture[2][1], str.c_str(), yellowTextColor, 30, largeWidth, TS_CENTER);
	
	str = L"Journal";
	FontManager->GenerateW(1, m_Texture[3][0], str.c_str(), 0, 30, largeWidth, TS_CENTER);
	FontManager->GenerateW(1, m_Texture[3][1], str.c_str(), yellowTextColor, 30, largeWidth, TS_CENTER);
	
	str = L"Chat";
	FontManager->GenerateW(1, m_Texture[4][0], str.c_str(), 0, 30, smallWidth, TS_CENTER);
	FontManager->GenerateW(1, m_Texture[4][1], str.c_str(), yellowTextColor, 30, smallWidth, TS_CENTER);
	
	str = L"Help";
	FontManager->GenerateW(1, m_Texture[5][0], str.c_str(), 0, 30, smallWidth, TS_CENTER);
	FontManager->GenerateW(1, m_Texture[5][1], str.c_str(), yellowTextColor, 30, smallWidth, TS_CENTER);
	
	str = L"World Map";
	FontManager->GenerateW(1, m_Texture[6][0], str.c_str(), 0, 30, largeWidth, TS_CENTER);
	FontManager->GenerateW(1, m_Texture[6][1], str.c_str(), yellowTextColor, 30, largeWidth, TS_CENTER);
	
	str = L"< ? >";
	FontManager->GenerateW(1, m_Texture[7][0], str.c_str(), 0, 30, smallWidth, TS_CENTER);
	FontManager->GenerateW(1, m_Texture[7][1], str.c_str(), yellowTextColor, 30, smallWidth, TS_CENTER);
}
//---------------------------------------------------------------------------
void TGumpMenubar::ReleaseTextTextures()
{
	IFOR(i, 0, 8)
	{
		IFOR(j, 0, 2)
		{
			m_Texture[i][j].Clear();
		}
	}
}
//---------------------------------------------------------------------------
void TGumpMenubar::PrepareTextures()
{
	UO->ExecuteResizepic(0x13BE);
	UO->ExecuteGump(0x15A1);
	UO->ExecuteGump(0x15A4);
	UO->ExecuteGump(0x098B);
	UO->ExecuteGump(0x098D);
}
//---------------------------------------------------------------------------
void TGumpMenubar::GenerateFrame(int posX, int posY)
{
	if (!g_DrawMode)
	{
		FrameRedraw = true;
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
	
		if (!m_Opened)
		{
			UO->DrawResizepicGump(0x13BE, posX, posY, 30, 27);

			UO->DrawGump(0x15A1, 0, posX + 5, posY + 3);
		}
		else
		{
			UO->DrawResizepicGump(0x13BE, posX, posY, 718, 27);

			UO->DrawGump(0x15A4, 0, posX + 5, posY + 3);

			posY++;
			int posYT = posY + 1;
			
			UO->DrawGump(0x098B, 0, posX + 30, posY);
			m_Texture[0][(int)(CanSelectedButton == ID_GMB_MAP)].Draw(posX + 32, posYT + (int)(CanPressedButton == ID_GMB_MAP ? 2 : 0));
			
			UO->DrawGump(0x098D, 0, posX + 93, posY);
			m_Texture[1][(int)(CanSelectedButton == ID_GMB_PAPERDOLL)].Draw(posX + 96, posYT + (int)(CanPressedButton == ID_GMB_PAPERDOLL ? 2 : 0));
			
			UO->DrawGump(0x098D, 0, posX + 201, posY);
			m_Texture[2][(int)(CanSelectedButton == ID_GMB_INVENTORY)].Draw(posX + 204, posYT + (int)(CanPressedButton == ID_GMB_INVENTORY ? 2 : 0));
			
			UO->DrawGump(0x098D, 0, posX + 309, posY);
			m_Texture[3][(int)(CanSelectedButton == ID_GMB_JOURNAL)].Draw(posX + 312, posYT + (int)(CanPressedButton == ID_GMB_JOURNAL ? 2 : 0));
			
			UO->DrawGump(0x098B, 0, posX + 417, posY);
			m_Texture[4][(int)(CanSelectedButton == ID_GMB_CHAT)].Draw(posX + 422, posYT + (int)(CanPressedButton == ID_GMB_CHAT ? 2 : 0));
			
			UO->DrawGump(0x098B, 0, posX + 480, posY);
			m_Texture[5][(int)(CanSelectedButton == ID_GMB_HELP)].Draw(posX + 482, posYT + (int)(CanPressedButton == ID_GMB_HELP ? 2 : 0));
			
			UO->DrawGump(0x098D, 0, posX + 543, posY);
			m_Texture[6][(int)(CanSelectedButton == ID_GMB_WORLD_MAP)].Draw(posX + 546, posYT + (int)(CanPressedButton == ID_GMB_WORLD_MAP ? 2 : 0));
			
			UO->DrawGump(0x098B, 0, posX + 651, posY);
			m_Texture[7][(int)(CanSelectedButton == ID_GMB_INFO)].Draw(posX + 654, posYT + (int)(CanPressedButton == ID_GMB_INFO ? 2 : 0));
		}

	glEndList();

	FrameRedraw = true;
	FrameCreated = true;
}
//----------------------------------------------------------------------------
int TGumpMenubar::Draw(bool &mode)
{
	DWORD index = (DWORD)this;

	//Для быстрого доступа
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

	//Если окошко захвачено для перемещения - вычислим оффсеты
	if (mode && CanBeMoved() && g_LeftMouseDown && g_LastGumpLeftMouseDown == index && !g_LastObjectLeftMouseDown)
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

	if (mode) //Отрисовка
	{
		if (!FrameCreated)
			GenerateFrame(posX, posY);

		glCallList((GLuint)index);
	}
	else //Выбор объектов
	{
		int LSG = 0;

		if (!m_Opened)
		{
			if (UO->ResizepicPixelsInXY(0x13BE, posX, posY, 30, 27))
			{
				g_LastSelectedGump = index;

				if (UO->GumpPixelsInXY(0x15A1, posX + 5, posY + 3))
					LSG = ID_GMB_MINIMIZE;
			}
		}
		else
		{
			if (UO->ResizepicPixelsInXY(0x13BE, posX, posY, 718, 27))
			{
				g_LastSelectedGump = index;
				g_LastSelectedObject = 0;
			}
				
			if (UO->GumpPixelsInXY(0x15A4, posX + 5, posY + 3))
				LSG = ID_GMB_MINIMIZE;
			else
			{
				posY++;

				if (UO->GumpPixelsInXY(0x098B, posX + 30, posY))
					LSG = ID_GMB_MAP;
				else if (UO->GumpPixelsInXY(0x098D, posX + 93, posY))
					LSG = ID_GMB_PAPERDOLL;
				else if (UO->GumpPixelsInXY(0x098D, posX + 201, posY))
					LSG = ID_GMB_INVENTORY;
				else if (UO->GumpPixelsInXY(0x098D, posX + 309, posY))
					LSG = ID_GMB_JOURNAL;
				else if (UO->GumpPixelsInXY(0x098B, posX + 417, posY))
					LSG = ID_GMB_CHAT;
				else if (UO->GumpPixelsInXY(0x098B, posX + 480, posY))
					LSG = ID_GMB_HELP;
				else if (UO->GumpPixelsInXY(0x098D, posX + 543, posY))
					LSG = ID_GMB_WORLD_MAP;
				else if (UO->GumpPixelsInXY(0x098B, posX + 651, posY))
					LSG = ID_GMB_INFO;
			}
		}

		if (LSG != 0)
		{
			g_LastSelectedObject = LSG; //Если что-то нашлось - выбираем
			g_LastSelectedGump = index;
		}

		return LSG;
	}

	return 0;
}
//----------------------------------------------------------------------------
void TGumpMenubar::OnLeftMouseUp()
{
	if (g_LastObjectLeftMouseDown != g_LastSelectedObject)
		return;

	switch (g_LastObjectLeftMouseDown)
	{
		case ID_GMB_MINIMIZE:
		{
			m_Opened = !m_Opened;

			break;
		}
		case ID_GMB_MAP:
		{
			UO->OpenMinimap();

			break;
		}
		case ID_GMB_PAPERDOLL:
		{
			UO->OpenPaperdoll();

			break;
		}
		case ID_GMB_INVENTORY:
		{
			UO->OpenBackpack();

			break;
		}
		case ID_GMB_JOURNAL:
		{
			UO->OpenJournal();

			break;
		}
		case ID_GMB_CHAT:
		{
			UO->OpenChat();

			break;
		}
		case ID_GMB_HELP:
		{
			UO->HelpRequest();

			break;
		}
		case ID_GMB_WORLD_MAP:
		{
			UO->OpenWorldMap();

			break;
		}
		case ID_GMB_INFO:
		{
			break;
		}
		default:
			break;
	}
}
//----------------------------------------------------------------------------