/****************************************************************************
**
** PopupHelp.cpp
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
//---------------------------------------------------------------------------
#include "stdafx.h"

TPopupHelp PopupHelp;
//---------------------------------------------------------------------------
TPopupHelp::TPopupHelp()
: m_SeqIndex(0), m_Data(L""), m_ClilocID(0), m_MaxWidth(0), m_X(0), m_Y(0),
m_Use(false), m_Type(SOT_NO_OBJECT), m_Timer(0)
{
	Texture.Clear();
}
//---------------------------------------------------------------------------
TPopupHelp::~TPopupHelp()
{
	Texture.Clear();
}
//---------------------------------------------------------------------------
void TPopupHelp::Set(wstring str, SELECT_OBJECT_TYPE type, DWORD seqIndex, int maxWidth, int x, int y)
{
	m_Use = !(m_Timer > GetTickCount());

	if (type == m_Type && seqIndex == m_SeqIndex) //Уже забиндено
		return;

	m_Timer = GetTickCount() + ConfigManager.PopupHelpDelay;
	m_Use = false;
	m_Type = type;
	m_SeqIndex = seqIndex;
	m_Data = str;
	m_ClilocID = 0;
	m_MaxWidth = maxWidth;
	m_X = x;
	m_Y = y;

	Texture.Clear();

	FontManager->GenerateW((BYTE)ConfigManager.PopupHelpTextFont, Texture, m_Data.c_str(), ConfigManager.PopupHelpTextColor, 5, m_MaxWidth, TS_CENTER, UOFONT_BLACK_BORDER);
	//FontManager->GenerateW(1, Texture, m_Data.c_str(), 0x0482, 30, m_MaxWidth, TS_CENTER, UOFONT_BLACK_BORDER);
}
//---------------------------------------------------------------------------
void TPopupHelp::Set(DWORD clilocID, string str, SELECT_OBJECT_TYPE type, DWORD seqIndex, int maxWidth, int x, int y)
{
	Set(ClilocManager->Cliloc(g_Language)->GetW(clilocID, str), type, seqIndex, maxWidth, x, y);

	m_ClilocID = clilocID;
}
//---------------------------------------------------------------------------
void TPopupHelp::Draw(int cursorWidth, int cursorHeight)
{
	if (!m_Use || !ConfigManager.PopupHelpEnabled)
		return;

	if (!m_MaxWidth)
		m_MaxWidth = FontManager->GetWidthW(1, m_Data.c_str(), m_Data.length());

	if (Texture.Empty())
		FontManager->GenerateW(1, Texture, m_Data.c_str(), 0x0482, 30, m_MaxWidth, TS_CENTER, UOFONT_BLACK_BORDER);

	if (!Texture.Empty())
	{
		int x = m_X;
		int y = m_Y;

		if (!x)
			x = g_MouseX - (Texture.Width + 8);

		if (!y)
			y = g_MouseY - (Texture.Height + 8);

		if (y < 0)
		{
			y = m_Y;
			y = g_MouseY + cursorHeight;
		}
		
		if (x < 0)
		{
			x = m_X;
			x = g_MouseX + cursorWidth;
		}
		
		glEnable(GL_BLEND);
		//glBlendFunc(GL_ONE_MINUS_SRC_ALPHA, GL_SRC_COLOR);
		//glBlendFunc(GL_SRC_ALPHA, GL_SRC_ALPHA);
		//glBlendFunc(GL_DST_ALPHA, GL_ONE_MINUS_DST_COLOR);
		glBlendFunc(GL_SRC_COLOR, GL_DST_COLOR);

		g_GL.DrawPolygone(0, (float)x, (float)y, Texture.Width + 8.0f, Texture.Height + 8.0f);
		
		glDisable(GL_BLEND);

		//g_GL.Draw(g_TransparntBackground, (float)x, (float)y, 2.0f, 2.0f, Texture.Width + 8.0f, Texture.Height + 8.0f);

		GLuint tex = Texture.Texture;
		int tempX = x + 6;
		int tempY = y + 4;

		g_GL.Draw(tex, tempX, tempY, Texture.Width, Texture.Height);
	}

	m_Use = false;
}
//---------------------------------------------------------------------------