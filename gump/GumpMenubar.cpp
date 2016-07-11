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

TTextTexture TGumpMenubar::m_Texture[8];
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

	FontManager->GenerateW(0, m_Texture[0], L"Map", 0, 30, smallWidth, TS_CENTER);
	FontManager->GenerateW(0, m_Texture[1], L"Paperdoll", 0, 30, largeWidth, TS_CENTER);
	FontManager->GenerateW(0, m_Texture[2], L"Inventory", 0, 30, largeWidth, TS_CENTER);
	FontManager->GenerateW(0, m_Texture[3], L"Journal", 0, 30, largeWidth, TS_CENTER);
	FontManager->GenerateW(0, m_Texture[4], L"Chat", 0, 30, smallWidth, TS_CENTER);
	FontManager->GenerateW(0, m_Texture[5], L"Help", 0, 30, smallWidth, TS_CENTER);
	FontManager->GenerateW(0, m_Texture[6], L"World Map", 0, 30, largeWidth, TS_CENTER);
	FontManager->GenerateW(0, m_Texture[7], L"< ? >", 0, 30, smallWidth, TS_CENTER);
}
//---------------------------------------------------------------------------
void TGumpMenubar::ReleaseTextTextures()
{
	IFOR(i, 0, 8)
		m_Texture[i].Clear();
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
void TGumpMenubar::GenerateFrame()
{
	if (!g_DrawMode)
	{
		m_FrameRedraw = true;
		m_FrameCreated = false;

		return;
	}

	CalculateGumpState();

	glNewList((GLuint)this, GL_COMPILE);
	
		if (!m_Opened)
		{
			UO->DrawResizepicGump(0x13BE, 0, 0, 30, 27);

			UO->DrawGump(0x15A1, 0, 5, 3);
		}
		else
		{
			UO->DrawResizepicGump(0x13BE, 0, 0, 718, 27);

			UO->DrawGump(0x15A4, 0, 5, 3);

			const int textPosTable[8][4] =
			{
				{ 0x098B, 30, 32, ID_GMB_MAP },
				{ 0x098D, 93, 96, ID_GMB_PAPERDOLL },
				{ 0x098D, 201, 204, ID_GMB_INVENTORY },
				{ 0x098D, 309, 312, ID_GMB_JOURNAL },
				{ 0x098B, 417, 422, ID_GMB_CHAT },
				{ 0x098B, 480, 482, ID_GMB_HELP },
				{ 0x098D, 543, 546, ID_GMB_WORLD_MAP },
				{ 0x098B, 651, 654, ID_GMB_INFO }
			};

			IFOR(i, 0, 8)
				UO->DrawGump(textPosTable[i][0], 0, textPosTable[i][1], 1);

			FontColorizerShader->Use();

			IFOR(i, 0, 8)
			{
				int drawMode = (int)(g_GumpSelectElement == textPosTable[i][3]) * 4;

				if (drawMode)
				{
					WORD yellowColor = 0x0036;
					ColorManager->SendColorsToShader(yellowColor);
				}

				glUniform1iARB(ShaderDrawMode, drawMode);

				m_Texture[i].Draw(textPosTable[i][2], 2 + (int)(g_GumpPressedElement == textPosTable[i][3] ? 2 : 0));
			}

			UnuseShader();
		}

	glEndList();

	m_FrameRedraw = true;
	m_FrameCreated = true;
}
//----------------------------------------------------------------------------
int TGumpMenubar::Draw(bool &mode)
{
	DWORD index = (DWORD)this;

	CalculateGumpState();

	if (mode) //Отрисовка
	{
		if (!m_FrameCreated || g_GumpSelectElement) // || g_GumpPressed
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
	else //Выбор объектов
	{
		int oldMouseX = g_MouseX;
		int oldMouseY = g_MouseY;
		g_MouseX -= (int)g_GumpTranslateX;
		g_MouseY -= (int)g_GumpTranslateY;

		int LSG = 0;

		if (!m_Opened)
		{
			if (UO->ResizepicPixelsInXY(0x13BE, 0, 0, 30, 27))
			{
				g_LastSelectedGump = index;

				if (UO->GumpPixelsInXY(0x15A1, 5, 3))
					LSG = ID_GMB_MINIMIZE;
			}
		}
		else
		{
			if (UO->ResizepicPixelsInXY(0x13BE, 0, 0, 718, 27))
			{
				g_LastSelectedGump = index;
				g_LastSelectedObject = 0;
			}
				
			if (UO->GumpPixelsInXY(0x15A4, 5, 3))
				LSG = ID_GMB_MINIMIZE;
			else
			{
				if (UO->GumpPixelsInXY(0x098B, 30, 1))
					LSG = ID_GMB_MAP;
				else if (UO->GumpPixelsInXY(0x098D, 93, 1))
					LSG = ID_GMB_PAPERDOLL;
				else if (UO->GumpPixelsInXY(0x098D, 201, 1))
					LSG = ID_GMB_INVENTORY;
				else if (UO->GumpPixelsInXY(0x098D, 309, 1))
					LSG = ID_GMB_JOURNAL;
				else if (UO->GumpPixelsInXY(0x098B, 417, 1))
					LSG = ID_GMB_CHAT;
				else if (UO->GumpPixelsInXY(0x098B, 480, 1))
					LSG = ID_GMB_HELP;
				else if (UO->GumpPixelsInXY(0x098D, 543, 1))
					LSG = ID_GMB_WORLD_MAP;
				else if (UO->GumpPixelsInXY(0x098B, 651, 1))
					LSG = ID_GMB_INFO;
			}
		}

		g_MouseX = oldMouseX;
		g_MouseY = oldMouseY;

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