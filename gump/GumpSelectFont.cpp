/****************************************************************************
**
** GumpSelectFont.cpp
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
//----------------------------------------------------------------------------
TGumpSelectFont::TGumpSelectFont(DWORD serial, short x, short y, SELECT_FONT_GUMP_STATE state)
: TGump(GT_SELECT_FONT, serial, x, y), m_State(state)
{
}
//----------------------------------------------------------------------------
TGumpSelectFont::~TGumpSelectFont()
{
}
//---------------------------------------------------------------------------
void TGumpSelectFont::PrepareTextures()
{
	UO->ExecuteResizepic(0x0A28);
	UO->ExecuteGumpPart(0x00D0, 2);
}
//---------------------------------------------------------------------------
void TGumpSelectFont::GenerateFrame(int posX, int posY)
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

	//Может ли быть нажат элемент?
	int CanPressedButton = 0;
	if (IsPressed && g_LastObjectLeftMouseDown == g_LastSelectedObject)
		CanPressedButton = g_LastObjectLeftMouseDown;

	int unicodeFontCount = FileManager.UnicodeFontCount;

	glNewList((GLuint)index, GL_COMPILE);

		UO->DrawResizepicGump(0x0A28, posX, posY, 200, 70 + (unicodeFontCount * 22));
		
		FontManager->DrawW(0, L"Select font", 0, posX + 60, posY + 22);

		int selected = 0;
			
		switch (m_State)
		{
			case SFGS_OPT_POPUP:
			{
				selected = g_OptionsConfig.PopupHelpTextFont;
				break;
			}
			case SFGS_OPT_TOOLTIP:
			{
				selected = g_OptionsConfig.ToolTipsTextFont;
				break;
			}
			case SFGS_OPT_CHAT:
			{
				selected = g_OptionsConfig.ChatFont;
				break;
			}
			case SFGS_OPT_MISCELLANEOUS:
			{
				selected = g_OptionsConfig.SpeechFont;
				break;
			}
			default:
				break;
		}
		
		posX += 50;
		posY += 24;

		IFOR(i, 0, unicodeFontCount)
		{
			posY += 22;

			WORD gumpID = 0x00D0;
			if ((i == selected && !CanPressedButton) || CanPressedButton == (i + ID_GSF_FONTS))
				gumpID = 0x00D1;

			UO->DrawGump(gumpID, 0, posX, posY);
			FontManager->DrawW(i, L"This font", 0, posX + 24, posY);
		}

	glEndList();

	FrameRedraw = true;
	FrameCreated = true;
}
//----------------------------------------------------------------------------
int TGumpSelectFont::Draw(bool &mode)
{
	DWORD index = (DWORD)this;

	int posX = X;
	int posY = Y;
	
	//Нажата ли кнопка в окне?
	bool IsPressed = (g_LeftMouseDown && g_LastGumpLeftMouseDown == index && g_LastSelectedGump == index);

	//Может ли быть нажат элемент?
	int CanPressedButton = 0;
	if (IsPressed && g_LastObjectLeftMouseDown == g_LastSelectedObject)
		CanPressedButton = g_LastObjectLeftMouseDown;

	if (g_LeftMouseDown && g_LastGumpLeftMouseDown == index && !g_LastObjectLeftMouseDown)
	{
		posX += g_MouseX - g_DroppedLeftMouseX;
		posY += g_MouseY - g_DroppedLeftMouseY;
		
		if (mode)
			GenerateFrame(posX, posY);
	}
	else if (mode)
	{
		if (IsPressed || CanPressedButton)
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
		
		int unicodeFontCount = FileManager.UnicodeFontCount;

		if (UO->ResizepicPixelsInXY(0x0A28, posX, posY, 200, 70 + (unicodeFontCount * 22)))
		{
			g_LastSelectedObject = 0;
			g_LastSelectedGump = index;

			posX += 50;
			posY += 24;

			IFOR(i, 0, unicodeFontCount)
			{
				posY += 22;

				if (UO->GumpPixelsInXY(0x00D0, posX, posY))
				{
					LSG = i + ID_GSF_FONTS;

					break;
				}
			}

			if (LSG != 0)
				g_LastSelectedObject = LSG;
		}

		return LSG;
	}

	return 0;
}
//----------------------------------------------------------------------------
void TGumpSelectFont::OnLeftMouseUp()
{
	if (!g_LastObjectLeftMouseDown || g_LastObjectLeftMouseDown != g_LastSelectedObject)
		return;

	switch (m_State)
	{
		case SFGS_OPT_POPUP:
		{
			g_OptionsConfig.PopupHelpTextFont = g_LastObjectLeftMouseDown - ID_GSF_FONTS;
			GumpManager->CloseGump(g_PlayerSerial, 0, GT_SELECT_FONT);

			break;
		}
		case SFGS_OPT_TOOLTIP:
		{
			g_OptionsConfig.ToolTipsTextFont = g_LastObjectLeftMouseDown - ID_GSF_FONTS;
			GumpManager->CloseGump(g_PlayerSerial, 0, GT_SELECT_FONT);

			break;
		}
		case SFGS_OPT_CHAT:
		{
			g_OptionsConfig.ChatFont = g_LastObjectLeftMouseDown - ID_GSF_FONTS;
			GumpManager->CloseGump(g_PlayerSerial, 0, GT_SELECT_FONT);

			break;
		}
		case SFGS_OPT_MISCELLANEOUS:
		{
			g_OptionsConfig.SpeechFont = g_LastObjectLeftMouseDown - ID_GSF_FONTS;
			GumpManager->CloseGump(g_PlayerSerial, 0, GT_SELECT_FONT);

			break;
		}
		default:
			break;
	}
}
//----------------------------------------------------------------------------