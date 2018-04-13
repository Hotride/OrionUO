// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
/***********************************************************************************
**
** GUIHTMLText.cpp
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#include "stdafx.h"
//----------------------------------------------------------------------------------
CGUIHTMLText::CGUIHTMLText(const uint &index, const uchar &font, const ushort &color, const int &x, const int &y, const int &width, const TEXT_ALIGN_TYPE &align, const ushort &textFlags, const uint &htmlStartColor)
: CBaseGUI(GOT_HTMLTEXT, 0, 0, color, x, y), TextID(index), HTMLStartColor(htmlStartColor),
Text(L""), Font(font), Align(align), TextFlags(textFlags), Width(width)
{
}
//----------------------------------------------------------------------------------
CGUIHTMLText::~CGUIHTMLText()
{
	WISPFUN_DEBUG("c64_f1");
	m_Texture.Clear();
}
//----------------------------------------------------------------------------------
void CGUIHTMLText::CreateTexture(const bool &backgroundCanBeColored)
{
	WISPFUN_DEBUG("c64_f2");
	g_FontManager.SetUseHTML(true, HTMLStartColor, backgroundCanBeColored);

	g_FontManager.GenerateW(Font, m_Texture, Text, Color, 30, Width, Align, TextFlags);

	g_FontManager.SetUseHTML(false);
}
//----------------------------------------------------------------------------------
void CGUIHTMLText::Draw(const bool &checktrans)
{
	WISPFUN_DEBUG("c64_f3");
	m_Texture.Draw(m_X, m_Y, checktrans);
}
//----------------------------------------------------------------------------------
bool CGUIHTMLText::Select()
{
	WISPFUN_DEBUG("c64_f4");
	int x = g_MouseManager.Position.X - m_X;
	int y = g_MouseManager.Position.Y - m_Y;

	return (x >= 0 && y >= 0 && x < m_Texture.Width && y < m_Texture.Height);
}
//----------------------------------------------------------------------------------
