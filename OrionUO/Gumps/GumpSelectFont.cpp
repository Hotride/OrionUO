// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
/***********************************************************************************
**
** GumpSelectFont.cpp
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#include "stdafx.h"
//----------------------------------------------------------------------------------
CGumpSelectFont::CGumpSelectFont(uint serial, short x, short y, SELECT_FONT_GUMP_STATE state)
: CGump(GT_SELECT_FONT, serial, x, y), m_State(state)
{
}
//----------------------------------------------------------------------------------
CGumpSelectFont::~CGumpSelectFont()
{
}
//----------------------------------------------------------------------------------
void CGumpSelectFont::UpdateContent()
{
	WISPFUN_DEBUG("c122_f1");
	Clear();

	CGUIResizepic *background = (CGUIResizepic*)Add(new CGUIResizepic(0, 0x0A28, 0, 0, 200, 70));

	CGUIText *text = (CGUIText*)Add(new CGUIText(0, 60, 22));
	text->CreateTextureW(0, L"Select font");
	
	int selected = 0;
			
	switch (m_State)
	{
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
		
	int drawY = 46;

	Add(new CGUIGroup(1));
	int count = 0;

	IFOR(i, 0, 20)
	{
		if (g_FontManager.UnicodeFontExists(i))
		{
			CGUIRadio *radio = (CGUIRadio*)Add(new CGUIRadio((int)i + ID_GSF_FONTS, 0x00D0, 0x00D1, 0x00D0, 50, drawY));
			radio->Checked = (i == selected);
			radio->SetTextParameters((uchar)i, L"This font", 0);
			drawY += 22;
			count++;
		}
	}

	background->Height = 70 + (count * 22);

	Add(new CGUIGroup(0));
}
//----------------------------------------------------------------------------------
void CGumpSelectFont::GUMP_RADIO_EVENT_C
{
	WISPFUN_DEBUG("c122_f2");
	if (!state)
		return;

	int realFont = -1;
	int count = serial - ID_GSF_FONTS;

	IFOR(i, 0, 20)
	{
		if (g_FontManager.UnicodeFontExists(i))
		{
			if (!count)
			{
				realFont = i;
				break;
			}

			count--;
		}
	}

	if (realFont == -1)
		return;

	switch (m_State)
	{
		case SFGS_OPT_TOOLTIP:
		{
			g_OptionsConfig.ToolTipsTextFont = realFont;
			m_RemoveMark = true;

			break;
		}
		case SFGS_OPT_CHAT:
		{
			g_OptionsConfig.ChatFont = realFont;
			m_RemoveMark = true;

			break;
		}
		case SFGS_OPT_MISCELLANEOUS:
		{
			g_OptionsConfig.SpeechFont = realFont;
			m_RemoveMark = true;

			break;
		}
		default:
			break;
	}
}
//----------------------------------------------------------------------------------
