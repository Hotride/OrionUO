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
		m_FrameRedraw = false;
		m_FrameCreated = false;

		return;
	}

	CalculateGumpState();

	int unicodeFontCount = FileManager.UnicodeFontCount;

	glNewList((GLuint)this, GL_COMPILE);

		UO->DrawResizepicGump(0x0A28, 0, 0, 200, 70 + (unicodeFontCount * 22));
		
		FontManager->DrawW(0, L"Select font", 0, 60, 22);

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
		
		int drawY = 24;

		IFOR(i, 0, unicodeFontCount)
		{
			drawY += 22;

			WORD gumpID = 0x00D0;
			if ((i == selected && !g_GumpPressedElement) || g_GumpPressedElement == (i + ID_GSF_FONTS))
				gumpID = 0x00D1;

			UO->DrawGump(gumpID, 0, 50, drawY);
			FontManager->DrawW(i, L"This font", 0, 74, drawY);
		}

	glEndList();

	FrameRedraw = true;
	FrameCreated = true;
}
//----------------------------------------------------------------------------
int TGumpSelectFont::Draw(bool &mode)
{
	DWORD index = (DWORD)this;

	CalculateGumpState();

	if (mode)
	{
		if (!m_FrameCreated || g_GumpPressedElement) // || g_GumpPressed
			GenerateFrame(0, 0);
		else if (m_FrameRedraw)
		{
			GenerateFrame(0, 0);
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
		
		int unicodeFontCount = FileManager.UnicodeFontCount;

		if (UO->ResizepicPixelsInXY(0x0A28, 0, 0, 200, 70 + (unicodeFontCount * 22)))
		{
			g_LastSelectedObject = 0;
			g_LastSelectedGump = index;

			int drawY = 24;

			IFOR(i, 0, unicodeFontCount)
			{
				drawY += 22;

				if (UO->GumpPixelsInXY(0x00D0, 50, drawY))
				{
					LSG = i + ID_GSF_FONTS;

					break;
				}
			}

			if (LSG != 0)
				g_LastSelectedObject = LSG;
		}

		g_MouseX = oldMouseX;
		g_MouseY = oldMouseY;

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