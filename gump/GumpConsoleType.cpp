/****************************************************************************
**
** GumpConsoleType.cpp
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

TTextTexture TGumpConsoleType::m_TextTitle;
TTextTexture TGumpConsoleType::m_TextCheckbox;
TTextTexture TGumpConsoleType::m_Text[2][7];
TGumpConsoleType *g_GumpConsoleType = NULL;
//----------------------------------------------------------------------------
TGumpConsoleType::TGumpConsoleType(bool minimized, bool showFullText)
: TGump(GT_CONSOLE_TYPE, 0, 0, 0), m_SelectedType(0), m_ShowFullText(showFullText)
{
	m_Minimized = minimized;
	m_NoMove = true;
	g_GumpConsoleType = this;
}
//----------------------------------------------------------------------------
TGumpConsoleType::~TGumpConsoleType()
{
	g_GumpConsoleType = NULL;
}
//----------------------------------------------------------------------------
void TGumpConsoleType::InitTextTextures()
{
	FontManager->GenerateA(3, m_TextTitle, "Default entry text mode:");
	FontManager->GenerateA(3, m_TextCheckbox, "Show full mode name");

	FontManager->GenerateA(3, m_Text[0][0], " Normal ");

	IFOR(i, 1, 7)
	{
		string str = " ";
		str += ToString(m_ConsolePrefix[i].c_str());

		if (str.length() < 3)
			str += " ";

		FontManager->GenerateA(3, m_Text[0][i], str.c_str());
	}

	FontManager->GenerateA(3, m_Text[1][0], " Normal ");
	FontManager->GenerateA(3, m_Text[1][1], " Yell ");
	FontManager->GenerateA(3, m_Text[1][2], " Whisper ");
	FontManager->GenerateA(3, m_Text[1][3], " Emote ");
	FontManager->GenerateA(3, m_Text[1][4], " Command ");
	FontManager->GenerateA(3, m_Text[1][5], " Broadcast ");
	FontManager->GenerateA(3, m_Text[1][6], " Party ");
}
//----------------------------------------------------------------------------
void TGumpConsoleType::ReleaseTextTextures()
{
	m_TextTitle.Clear();
	m_TextCheckbox.Clear();

	IFOR(i, 0, 7)
	{
		m_Text[0][i].Clear();
		m_Text[1][i].Clear();
	}
}
//----------------------------------------------------------------------------
bool TGumpConsoleType::ConsoleIsEmpty()
{
	if (GameConsole == NULL)
		return false;

	bool result = (GameConsole->Length() == 0);

	switch (m_SelectedType)
	{
		case GCTT_YELL:
		case GCTT_WHISPER:
		case GCTT_EMOTE:
		case GCTT_C:
		case GCTT_BROADCAST:
		case GCTT_PARTY:
		{	   
			result = (m_ConsolePrefix[m_SelectedType] == GameConsole->Data());
			break;
		}
		default:
			break;
	}

	return result;
}
//----------------------------------------------------------------------------
void TGumpConsoleType::DeleteConsolePrefix()
{
	if (GameConsole == NULL)
		return;

	static const wstring space = L" ";

	switch (m_SelectedType)
	{
		case GCTT_YELL:
		case GCTT_WHISPER:
		case GCTT_EMOTE:
		case GCTT_C:
		case GCTT_BROADCAST:
		case GCTT_PARTY:
		{
			wstring str(GameConsole->Data());

			if (str.find(m_ConsolePrefix[m_SelectedType]) == 0)
			{
				str.erase(str.begin(), str.begin() + m_ConsolePrefix[m_SelectedType].length());
				GameConsole->SetText(str);
			}

			break;
		}
		default:
			break;
	}
}
//----------------------------------------------------------------------------
void TGumpConsoleType::SetConsolePrefix()
{
	if (GameConsole == NULL)
		return;

	switch (m_SelectedType)
	{
		case GCTT_YELL:
		case GCTT_WHISPER:
		case GCTT_EMOTE:
		case GCTT_C:
		case GCTT_BROADCAST:
		case GCTT_PARTY:
		{
			wstring str(GameConsole->Data());
			str = m_ConsolePrefix[m_SelectedType] + str;
			GameConsole->SetText(str);

			break;
		}
		default:
			break;
	}
}
//----------------------------------------------------------------------------
void TGumpConsoleType::PrepareTextures()
{
	Orion->ExecuteGump(0x0984);
	Orion->ExecuteGump(0x0986);
	Orion->ExecuteGumpPart(0x00D2, 2);
}
//----------------------------------------------------------------------------
void TGumpConsoleType::OnToolTip()
{
	DWORD selected = ((g_LastSelectedGump == (DWORD)this) ? g_LastSelectedObject : 0);

	switch (selected)
	{
		case ID_GCT_NORMAL:
		{
			ToolTip.Set(L"Normal entry mode\nno prefix", SOT_NO_OBJECT, selected);
			break;
		}
		case ID_GCT_YELL:
		{
			ToolTip.Set(L"Yell entry mode\nprefix is '! '", SOT_NO_OBJECT, selected);
			break;
		}
		case ID_GCT_WHISPER:
		{
			ToolTip.Set(L"Whisper entry mode\nprefix is '; '", SOT_NO_OBJECT, selected);
			break;
		}
		case ID_GCT_EMOTE:
		{
			ToolTip.Set(L"Emote entry mode\nprefix is ': '", SOT_NO_OBJECT, selected);
			break;
		}
		case ID_GCT_COMMAND:
		{
			ToolTip.Set(L"Command entry mode\nprefix is '. '", SOT_NO_OBJECT, selected);
			break;
		}
		case ID_GCT_BROADCAST:
		{
			ToolTip.Set(L"Broadcast entry mode\nprefix is '? '", SOT_NO_OBJECT, selected);
			break;
		}
		case ID_GCT_PARTY:
		{
			ToolTip.Set(L"Party entry mode\nprefix is '/ '", SOT_NO_OBJECT, selected);
			break;
		}
		case ID_GCT_MINIMIZE:
		{
			ToolTip.Set(L"Show/hide configuration", SOT_NO_OBJECT, selected);
			break;
		}
		case ID_GCT_SHOW_FULL_TEXT:
		{
			ToolTip.Set(L"Show full mode names", SOT_NO_OBJECT, selected);
			break;
		}
		default:
			break;
	}
}
//----------------------------------------------------------------------------
void TGumpConsoleType::GenerateFrame()
{
	if (!g_DrawMode)
	{
		m_FrameRedraw = false;
		m_FrameCreated = false;

		return;
	}

	CalculateGumpState();
	
	glNewList((GLuint)this, GL_COMPILE);

		m_TextTitle.Draw(14, 0);

		WORD gump = 0x0986;
		if (!m_Minimized)
			gump -= 2;

		Orion->DrawGump(gump, 0, 0, 6);

		if (!m_Minimized)
		{
			int offsetX = 0;
			int offsetY = m_TextTitle.Height;

			IFOR(i, 0, 7)
			{
				TTextTexture &text = m_Text[(int)m_ShowFullText][i];

				glEnable(GL_BLEND);
				glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
				glColor4f(0.3f, 0.3f, 0.3f, 0.5f);

				g_GL.DrawPolygone(offsetX + 1, offsetY, text.Width - 2, text.Height);

				glDisable(GL_BLEND);

				if (i == m_SelectedType)
					glColor4f(0.0f, 1.0f, 0.0f, 1.0f);
				else if (i + 1 == g_GumpSelectElement)
					glColor4f(1.0f, 0.5f, 0.15f, 1.0f);
				else
					glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

				text.Draw(offsetX, offsetY);
				offsetX += text.Width;
			}

			glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

			offsetY += m_Text[(int)m_ShowFullText][0].Height + 3;

			Orion->DrawGump(0x00D2 + (int)m_ShowFullText, 0, 0, offsetY + 2);

			m_TextCheckbox.Draw(24, offsetY);
		}

	glEndList();

	m_FrameRedraw = true;
	m_FrameCreated = true;
}
//----------------------------------------------------------------------------
void TGumpConsoleType::CalculateGumpState()
{
	TGump::CalculateGumpState();

	g_GumpMovingOffsetX = 0;
	g_GumpMovingOffsetY = 0;

	g_GumpTranslateX = (float)(g_RenderBounds->GameWindowPosX + 2);
	g_GumpTranslateY = (float)(g_RenderBounds->GameWindowPosY + g_RenderBounds->GameWindowSizeY + 2);
}
//----------------------------------------------------------------------------
int TGumpConsoleType::Draw(bool &mode)
{
	CalculateGumpState();

	DWORD index = (DWORD)this;

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

		if (Orion->PolygonePixelsInXY(0, 6, 14, 14))
			LSG = ID_GCT_MINIMIZE;

		if (!m_Minimized)
		{
			int offsetX = 0;
			int offsetY = m_TextTitle.Height;

			IFOR(i, 0, 7)
			{
				TTextTexture &text = m_Text[(int)m_ShowFullText][i];

				if (Orion->PolygonePixelsInXY(offsetX, offsetY, text.Width, text.Height))
					LSG = i + 1;

				offsetX += text.Width;
			}

			offsetY += m_Text[(int)m_ShowFullText][0].Height + 3;

			if (Orion->GumpPixelsInXY(0x00D2, 0, offsetY + 2))
				LSG = ID_GCT_SHOW_FULL_TEXT;
		}

		g_MouseX = oldMouseX;
		g_MouseY = oldMouseY;

		return LSG;
	}

	return 0;
}
//----------------------------------------------------------------------------
void TGumpConsoleType::OnLeftMouseUp()
{
	if (!g_LastObjectLeftMouseDown || g_LastObjectLeftMouseDown != g_LastSelectedObject)
		return;

	if (g_LastObjectLeftMouseDown == ID_GCT_MINIMIZE)
		m_Minimized = !m_Minimized;
	else if (g_LastObjectLeftMouseDown == ID_GCT_SHOW_FULL_TEXT)
		m_ShowFullText = !m_ShowFullText;
	else
	{
		DeleteConsolePrefix();

		m_SelectedType = g_LastObjectLeftMouseDown - 1;

		SetConsolePrefix();
	}

	m_FrameCreated = false;
}
//----------------------------------------------------------------------------