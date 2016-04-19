/****************************************************************************
**
** GumpSelectColor.cpp
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
TGumpSelectColor::TGumpSelectColor(DWORD serial, short x, short y, SELECT_COLOR_GUMP_STATE state)
: TGump(GT_SELECT_COLOR, serial, x, y), m_State(state), m_ColorRef(1),
m_SliderPos(72), m_SelectedIndex(10)
{
}
//----------------------------------------------------------------------------
TGumpSelectColor::~TGumpSelectColor()
{
}
//---------------------------------------------------------------------------
void TGumpSelectColor::PrepareTextures()
{
	UO->ExecuteGump(0x0906);
	UO->ExecuteButton(0x0907);
	UO->ExecuteGumpPart(0x0845, 2);
}
//---------------------------------------------------------------------------
void TGumpSelectColor::GenerateFrame()
{
	if (!g_DrawMode)
	{
		m_FrameRedraw = false;
		m_FrameCreated = false;

		return;
	}

	if (m_GumpType != GT_DYE)
	{
		CalculateGumpState();

		glNewList((GLuint)this, GL_COMPILE);
	}

		const int cellWidthX = 8;
		const int cellWidthY = 8;

		UO->DrawGump(0x0906, 0, 0, 0); //Body
		
		WORD gumpID = 0x0907;
		if (g_GumpPressedElement == ID_GSC_BUTTON_OKAY)
			gumpID = 0x0908;
		else if (g_GumpSelectElement == ID_GSC_BUTTON_OKAY)
			gumpID = 0x0909;

		UO->DrawGump(gumpID, 0, 208, 138); //Okay button
		
		gumpID = 0x0845 + (int)(g_GumpSelectElement == ID_GSC_SLIDER); //Scroll button / selected
		UO->DrawGump(gumpID, 0, m_SliderPos, 142);

		WORD startColor = m_ColorRef + 1;

		PBYTE huesData = (PBYTE)ColorManager->GetHuesRangePointer() + 32 + 4;
		int colorOffsetDivider = sizeof(HUES_GROUP) - 4;

		IFOR(y, 0, 10)
		{
			IFOR(x, 0, 20)
			{
				int colorIndex = (startColor + ((startColor + (startColor << 2)) << 1)) << 3;

				colorIndex += (colorIndex / colorOffsetDivider) << 2;
				WORD color = *(PWORD)(huesData + colorIndex);

				DWORD clr = ColorManager->Color16To32(color);

				glColor3ub((GetRValue(clr)), GetGValue(clr), GetBValue(clr));
				g_GL.DrawPolygone(34 + (x * cellWidthX), 34 + (y * cellWidthY), cellWidthX, cellWidthY);

				if (m_SelectedIndex == ID_GSC_COLORS + (x * 30 + y))
				{
					glColor3ub(0xFF, 0xFF, 0xFF);
					g_GL.DrawPolygone(34 + (x * cellWidthX) + (cellWidthX / 2) - 1, 34 + (y * cellWidthY) + (cellWidthY / 2) - 1, 2, 2);
				}

				startColor += 5;
			}
		}

		glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

	if (m_GumpType != GT_DYE)
	{
		glEndList();

		m_FrameRedraw = true;
		m_FrameCreated = true;
	}
}
//----------------------------------------------------------------------------
int TGumpSelectColor::Draw(bool &mode)
{
	CalculateGumpState();

	//Если схватили ползунок - вычислим его позицию и значение в текстовом поле
	if (mode && g_GumpPressed && g_LastObjectLeftMouseDown == ID_GSC_SLIDER)
	{
		int currentX = g_MouseX - (int)g_GumpTranslateX - 5; //Текущая позиция ползунка

		if (currentX < 39)
			currentX = 39; //Выход за допустимый предел, корректируем на минимум
		else if (currentX > 172)
			currentX = 172; //Выход за допустимый предел, корректируем на максимум

		m_SliderPos = currentX; //Устанавливаем значение ползунка

		int count = m_ColorRef; //Количество предметов в стеке
		if (currentX > 39) //Позиция ползунка больше минимума
		{
			if (currentX < 172) //Позиция ползунка меньше максимума, вычисляем
			{
				float ValPer = ((currentX - 39.0f) / 133.0f) * 100.0f;

				if (ValPer == 0.0f)
					count = 0;
				else
					count = (int)((5.0f * ValPer) / 100.0f);
			}
		}
		else
			count = 0; //Позиция равна минимуму, выставляем количество 0

		m_ColorRef = count;
	}
	
	DWORD index = (DWORD)this;

	if (mode)
	{
		if (g_GumpPressed || g_GumpSelectElement || !m_FrameCreated)
			GenerateFrame();
		else if (FrameRedraw)
		{
			GenerateFrame();
			FrameRedraw = false;
		}

		glTranslatef(g_GumpTranslateX, g_GumpTranslateY, 0.0f);

		glCallList((GLuint)index);

		glTranslatef(-g_GumpTranslateX, -g_GumpTranslateY, 0.0f);
	}
	else
	{
		int LSG = 0;

		int oldMouseX = g_MouseX;
		int oldMouseY = g_MouseY;
		g_MouseX -= (int)g_GumpTranslateX;
		g_MouseY -= (int)g_GumpTranslateY;

		if (UO->GumpPixelsInXY(0x0906, 0, 0))
		{
			g_LastSelectedObject = 0;
			g_LastSelectedGump = index;
			
			if (UO->GumpPixelsInXY(0x0907, 208, 138))
				LSG = ID_GSC_BUTTON_OKAY; //Okay button
			else if (UO->GumpPixelsInXY(0x0845, m_SliderPos, 142))
				LSG = ID_GSC_SLIDER; //Scroll button
		
			const int cellWidthX = 8;
			const int cellWidthY = 8;

			bool found = false;
			RECT rc = {0, 0, cellWidthX, cellWidthY};
		
			IFOR(y, 0, 10)
			{
				IFOR(x, 0, 20)
				{
					POINT p = {g_MouseX - (34 + (x * cellWidthX)), g_MouseY - (34 + (y * cellWidthY))};

					if (PtInRect(&rc, p))
					{
						LSG = (ID_GSC_COLORS + (x * 30) + y);
						found = true;

						break;
					}
				}

				if (found)
					break;
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
void TGumpSelectColor::OnLeftMouseUp()
{
	if (g_LastObjectLeftMouseDown == g_LastSelectedObject)
	{
		if (g_LastSelectedObject == ID_GSC_BUTTON_OKAY)
		{
			WORD color = 0;

			WORD startColor = m_ColorRef + 2;

			IFOR(y, 0, 10 && !color)
			{
				IFOR(x, 0, 20)
				{
					if (m_SelectedIndex == ID_GSC_COLORS + (x * 30 + y))
					{
						color = startColor + (x * 5 + (y * 100));
						break;
					}
				}
			}

			if (color)
				OnSelectColor(color);
		}
		else if (g_LastSelectedObject >= ID_GSC_COLORS)
			m_SelectedIndex = g_LastSelectedObject;
	}
}
//----------------------------------------------------------------------------
void TGumpSelectColor::OnSelectColor(WORD &color)
{
	switch (m_State)
	{
		case SCGS_OPT_TOOLTIP_TEXT:
		{
			g_OptionsConfig.ToolTipsTextColor = color;
			break;
		}
		case SCGS_OPT_CHAT_INPUT_TEXT:
		{
			g_OptionsConfig.ChatColorInputText = color;
			break;
		}
		case SCGS_OPT_CHAT_MENU_OPTION:
		{
			g_OptionsConfig.ChatColorMenuOption = color;
			break;
		}
		case SCGS_OPT_CHAT_PLAYER_IN_MEMBER_LIST:
		{
			g_OptionsConfig.ChatColorPlayerInMemberList = color;
			break;
		}
		case SCGS_OPT_CHAT_TEXT:
		{
			g_OptionsConfig.ChatColorText = color;
			break;
		}
		case SCGS_OPT_CHAT_PLAYER_WITHOUT_PRIV:
		{
			g_OptionsConfig.ChatColorPlayerNameWithout = color;
			break;
		}
		case SCGS_OPT_CHAT_MUTED_TEXT:
		{
			g_OptionsConfig.ChatColorMuted = color;
			break;
		}
		case SCGS_OPT_CHAT_CHAN_MODER_NAME:
		{
			g_OptionsConfig.ChatColorChannelModeratorName = color;
			break;
		}
		case SCGS_OPT_CHAT_CHAN_MODER_TEXT:
		{
			g_OptionsConfig.ChatColorChannelModeratorText = color;
			break;
		}
		case SCGS_OPT_CHAT_MY_NAME:
		{
			g_OptionsConfig.ChatColorMyName = color;
			break;
		}
		case SCGS_OPT_CHAT_MY_TEXT:
		{
			g_OptionsConfig.ChatColorMyText = color;
			break;
		}
		case SCGS_OPT_CHAT_SYSTEM_MESSAGE:
		{
			g_OptionsConfig.ChatColorSystemMessage = color;
			break;
		}
		case SCGS_OPT_CHAT_BG_OUTPUT:
		{
			g_OptionsConfig.ChatColorBGOutputText = color;
			break;
		}
		case SCGS_OPT_CHAT_BG_INPUT:
		{
			g_OptionsConfig.ChatColorBGInputText = color;
			break;
		}
		case SCGS_OPT_CHAT_BG_USER_LIST:
		{
			g_OptionsConfig.ChatColorBGUserList = color;
			break;
		}
		case SCGS_OPT_CHAT_BG_CONF_LIST:
		{
			g_OptionsConfig.ChatColorBGCommandList = color;
			break;
		}
		case SCGS_OPT_CHAT_BG_COMMAND_LIST:
		{
			g_OptionsConfig.ChatColorBGCommandList = color;
			break;
		}
		case SCGS_OPT_DISPLAY_SPEECH:
		{
			g_OptionsConfig.SpeechColor = color;
			break;
		}
		case SCGS_OPT_DISPLAY_EMOTE:
		{
			g_OptionsConfig.EmoteColor = color;
			break;
		}
		case SCGS_OPT_DISPLAY_PARTY_MESSAGE:
		{
			g_OptionsConfig.PartyMessageColor = color;
			break;
		}
		case SCGS_OPT_DISPLAY_GUILD_MESSAGE:
		{
			g_OptionsConfig.GuildMessageColor = color;
			break;
		}
		case SCGS_OPT_DISPLAY_ALLIANCE_MESSAGE:
		{
			g_OptionsConfig.AllianceMessageColor = color;
			break;
		}
		case SCGS_OPT_REPSYS_INNOCENT:
		{
			g_OptionsConfig.InnocentColor = color;
			break;
		}
		case SCGS_OPT_REPSYS_FRIENDLY:
		{
			g_OptionsConfig.FriendlyColor = color;
			break;
		}
		case SCGS_OPT_REPSYS_SOMEONE:
		{
			g_OptionsConfig.SomeoneColor = color;
			break;
		}
		case SCGS_OPT_REPSYS_CRIMINAL:
		{
			g_OptionsConfig.CriminalColor = color;
			break;
		}
		case SCGS_OPT_REPSYS_ENEMY:
		{
			g_OptionsConfig.EnemyColor = color;
			break;
		}
		case SCGS_OPT_REPSYS_MURDERER:
		{
			g_OptionsConfig.MurdererColor = color;
			break;
		}
		default:
			break;
	}

	GumpManager->CloseGump(g_PlayerSerial, 0, GT_SELECT_COLOR);

	GumpManager->UpdateGump(g_PlayerSerial, 0, GT_OPTIONS);
}
//----------------------------------------------------------------------------