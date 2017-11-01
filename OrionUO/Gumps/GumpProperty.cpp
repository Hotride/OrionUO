/***********************************************************************************
**
** GumpProperty.cpp
**
** Copyright (C) November 2017 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#include "stdafx.h"
//----------------------------------------------------------------------------------
CGumpProperty::CGumpProperty(const wstring &text)
: CGump(GT_PROPERTY, 0, 0, 0)
{
	m_Timer = g_Ticks + 5000;
	uchar font = (uchar)g_ConfigManager.ToolTipsTextFont;

	g_FontManager.SetUseHTML(true);
	g_FontManager.RecalculateWidthByInfo = true;

	int width = g_FontManager.GetWidthW(font, text);

	if (width > 600)
		width = 600;

	width = g_FontManager.GetWidthExW(font, text, width, TS_CENTER, UOFONT_BLACK_BORDER);

	if (width > 600)
		width = 600;

	g_FontManager.GenerateW(font, m_Texture, text, g_ConfigManager.ToolTipsTextColor, 5, width, TS_CENTER, UOFONT_BLACK_BORDER);

	g_FontManager.RecalculateWidthByInfo = false;
	g_FontManager.SetUseHTML(false);

	m_X = g_MouseManager.Position.X - (m_Texture.Width + 8);

	if (m_X < 0)
		m_X = 0;

	m_Y = g_MouseManager.Position.Y - (m_Texture.Height + 8);

	if (m_Y < 0)
		m_Y = 0;

	Add(new CGUIColoredPolygone(0, 0, 0, 0, m_Texture.Width + 12, m_Texture.Height + 8, 0x7F000000));

	Add(new CGUIExternalTexture(&m_Texture, false, 6, 4));
}
//----------------------------------------------------------------------------------
CGumpProperty::~CGumpProperty()
{
}
//----------------------------------------------------------------------------------
void CGumpProperty::PrepareContent()
{
	if (m_Timer < g_Ticks)
		m_RemoveMark = true;
}
//----------------------------------------------------------------------------------
