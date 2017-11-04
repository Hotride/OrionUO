// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
/***********************************************************************************
**
** ToolTip.cpp
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#include "stdafx.h"
//----------------------------------------------------------------------------------
CToolTip g_ToolTip;
//----------------------------------------------------------------------------------
CToolTip::CToolTip()
{
}
//----------------------------------------------------------------------------------
CToolTip::~CToolTip()
{
	WISPFUN_DEBUG("c213_f1");
	Reset();
}
//----------------------------------------------------------------------------------
void CToolTip::Reset()
{
	WISPFUN_DEBUG("c213_f2");
	Texture.Clear();
	m_Object = NULL;
}
//----------------------------------------------------------------------------------
void CToolTip::CreateTextTexture(CGLTextTexture &texture, const wstring &str, int &width, const int &minWidth)
{
	g_FontManager.SetUseHTML(true);
	g_FontManager.RecalculateWidthByInfo = true;

	texture.Clear();

	uchar font = (uchar)g_ConfigManager.ToolTipsTextFont;

	if (!width)
	{
		width = g_FontManager.GetWidthW(font, str);

		if (width > 600)
			width = 600;

		width = g_FontManager.GetWidthExW(font, str, width, TS_CENTER, UOFONT_BLACK_BORDER);

		if (width > 600)
			width = 600;
	}

	if (width < minWidth)
		width = minWidth;

	g_FontManager.GenerateW(font, texture, str, g_ConfigManager.ToolTipsTextColor, 5, width, TS_CENTER, UOFONT_BLACK_BORDER);

	g_FontManager.RecalculateWidthByInfo = false;
	g_FontManager.SetUseHTML(false);
}
//----------------------------------------------------------------------------------
void CToolTip::Set(const wstring &str, const int &maxWidth)
{
	WISPFUN_DEBUG("c213_f3");
	if (!str.length())
		return;

	m_Use = !(m_Timer > g_Ticks);

	CRenderObject *object = g_SelectedObject.Object;

	if (object == m_Object || object == NULL) //Уже забиндено или нет объекта для бинда
		return;

	m_Timer = g_Ticks + g_ConfigManager.ToolTipsDelay;
	m_Object = object;
	m_Use = false;
	m_Data = str;
	m_ClilocID = 0;
	m_MaxWidth = maxWidth;

	m_Position.X = 0;
	m_Position.Y = 0;

	CreateTextTexture(Texture, m_Data, m_MaxWidth, 0);
}
//----------------------------------------------------------------------------------
void CToolTip::Set(const uint &clilocID, const string &str, const int &maxWidth, const bool &toCamelCase)
{
	WISPFUN_DEBUG("c213_f4");
	Set(g_ClilocManager.Cliloc(g_Language)->GetW(clilocID, toCamelCase, str), maxWidth);

	m_ClilocID = clilocID;
}
//----------------------------------------------------------------------------------
void CToolTip::Draw(const int &cursorWidth, const int &cursorHeight)
{
	WISPFUN_DEBUG("c213_f5");
	if (!m_Use /*|| !g_ConfigManager.UseToolTips*/)
		return;

	if (Texture.Empty())
		CreateTextTexture(Texture, m_Data, m_MaxWidth, 0);

	if (!Texture.Empty())
	{
		int x = m_Position.X;
		int y = m_Position.Y;

		if (!x)
			x = g_MouseManager.Position.X - (Texture.Width + 8);

		if (!y)
			y = g_MouseManager.Position.Y - (Texture.Height + 8);

		if (y < 0)
		{
			y = m_Position.Y;
			//y = g_MouseManager.Position.Y + cursorHeight;
		}
		
		if (x < 0)
		{
			x = m_Position.X;
			//x = g_MouseManager.Position.X + cursorWidth;
		}
		
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glColor4f(0.0f, 0.0f, 0.0f, 0.5f);

		g_GL.DrawPolygone(x, y, Texture.Width + 8, Texture.Height + 8);

		glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
		
		glDisable(GL_BLEND);

		g_GL_Draw(Texture, x + 6, y + 4);
	}

	m_Use = false;
}
//----------------------------------------------------------------------------------
