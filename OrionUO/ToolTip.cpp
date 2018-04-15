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
void CToolTip::CreateTextTexture(CGLTextTexture &texture, const wstring &str, int &width, int minWidth)
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
void CToolTip::Set(const wstring &str, int maxWidth)
{
	WISPFUN_DEBUG("c213_f3");
	if (!str.length())
		return;

	Use = !(Timer > g_Ticks);

	CRenderObject *object = g_SelectedObject.Object;

	if (object == m_Object || object == NULL) //Уже забиндено или нет объекта для бинда
		return;

	Timer = g_Ticks + g_ConfigManager.ToolTipsDelay;
	m_Object = object;
	Use = false;
	Data = str;
	ClilocID = 0;
	MaxWidth = maxWidth;

	Position.X = 0;
	Position.Y = 0;

	CreateTextTexture(Texture, Data, MaxWidth, 0);
}
//----------------------------------------------------------------------------------
void CToolTip::Set(int clilocID, const string &str, int maxWidth, bool toCamelCase)
{
	WISPFUN_DEBUG("c213_f4");
	Set(g_ClilocManager.Cliloc(g_Language)->GetW(clilocID, toCamelCase, str), maxWidth);

	ClilocID = clilocID;
}
//----------------------------------------------------------------------------------
void CToolTip::Draw(int cursorWidth, int cursorHeight)
{
	WISPFUN_DEBUG("c213_f5");
	if (!Use /*|| !g_ConfigManager.UseToolTips*/)
		return;

	if (Texture.Empty())
		CreateTextTexture(Texture, Data, MaxWidth, 0);

	if (!Texture.Empty())
	{
		int x = Position.X;
		int y = Position.Y;

		if (!x)
			x = g_MouseManager.Position.X - (Texture.Width + 8);

		if (!y)
			y = g_MouseManager.Position.Y - (Texture.Height + 8);

		if (y < 0)
		{
			y = Position.Y;
			//y = g_MouseManager.Position.Y + cursorHeight;
		}
		
		if (x < 0)
		{
			x = Position.X;
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

	Use = false;
}
//----------------------------------------------------------------------------------
