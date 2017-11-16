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

	int unicodeFontsCount = g_FileManager.UnicodeFontsCount;

	Add(new CGUIResizepic(0, 0x0A28, 0, 0, 200, 70 + (unicodeFontsCount * 22)));

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
		
	int drawY = 24;

	Add(new CGUIGroup(1));

	IFOR(i, 0, unicodeFontsCount)
	{
		drawY += 22;

		CGUIRadio *radio = (CGUIRadio*)Add(new CGUIRadio((int)i + ID_GSF_FONTS, 0x00D0, 0x00D1, 0x00D0, 50, drawY));
		radio->Checked = (i == selected);

		text = (CGUIText*)Add(new CGUIText(0, 74, drawY));
		text->CreateTextureW((uchar)i, L"This font");
	}

	Add(new CGUIGroup(0));
}
//----------------------------------------------------------------------------------
void CGumpSelectFont::GUMP_RADIO_EVENT_C
{
	WISPFUN_DEBUG("c122_f2");
	if (!state)
		return;

	switch (m_State)
	{
		case SFGS_OPT_TOOLTIP:
		{
			g_OptionsConfig.ToolTipsTextFont = serial - ID_GSF_FONTS;
			m_RemoveMark = true;

			break;
		}
		case SFGS_OPT_CHAT:
		{
			g_OptionsConfig.ChatFont = serial - ID_GSF_FONTS;
			m_RemoveMark = true;

			break;
		}
		case SFGS_OPT_MISCELLANEOUS:
		{
			g_OptionsConfig.SpeechFont = serial - ID_GSF_FONTS;
			m_RemoveMark = true;

			break;
		}
		default:
			break;
	}
}
//----------------------------------------------------------------------------------
