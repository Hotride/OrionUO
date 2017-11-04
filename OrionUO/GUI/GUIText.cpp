// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
/***********************************************************************************
**
** GUIText.cpp
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#include "stdafx.h"
//----------------------------------------------------------------------------------
CGUIText::CGUIText(const ushort &color, const int &x, const int &y)
: CBaseGUI(GOT_TEXT, 0, 0, color, x, y)
{
}
//----------------------------------------------------------------------------------
CGUIText::~CGUIText()
{
	WISPFUN_DEBUG("c78_f1");
	m_Texture.Clear();
}
//----------------------------------------------------------------------------------
void CGUIText::CreateTextureA(const uchar &font, const string &str, int width, const TEXT_ALIGN_TYPE &align, const ushort &flags)
{
	WISPFUN_DEBUG("c78_f2");
	g_FontManager.GenerateA(font, m_Texture, str, m_Color, width, align, flags);
}
//----------------------------------------------------------------------------------
void CGUIText::CreateTextureW(const uchar &font, const wstring &str, const uchar &cell, int width, const TEXT_ALIGN_TYPE &align, const ushort &flags)
{
	WISPFUN_DEBUG("c78_f3");
	g_FontManager.GenerateW(font, m_Texture, str, m_Color, cell, width, align, flags);
}
//----------------------------------------------------------------------------------
void CGUIText::Draw(const bool &checktrans)
{
	WISPFUN_DEBUG("c78_f4");
	m_Texture.Draw(m_X, m_Y, checktrans);
}
//----------------------------------------------------------------------------------
bool CGUIText::Select()
{
	WISPFUN_DEBUG("c78_f5");
	int x = g_MouseManager.Position.X - m_X;
	int y = g_MouseManager.Position.Y - m_Y;

	return (x >= 0 && y >= 0 && x < m_Texture.Width && y < m_Texture.Height);
}
//----------------------------------------------------------------------------------
