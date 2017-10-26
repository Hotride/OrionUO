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
: CBaseGUI(GOT_HTMLTEXT, 0, 0, color, x, y), m_TextID(index), m_HTMLStartColor(htmlStartColor),
m_Text(L""), m_Font(font), m_Align(align), m_TextFlags(textFlags), m_Width(width)
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
	g_FontManager.SetUseHTML(true, m_HTMLStartColor, backgroundCanBeColored);

	g_FontManager.GenerateW(m_Font, m_Texture, m_Text, m_Color, 30, m_Width, m_Align, m_TextFlags);

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
