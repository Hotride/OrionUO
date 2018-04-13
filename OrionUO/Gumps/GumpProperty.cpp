// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
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
	Timer = g_Ticks + 5000;

	int width = 0;
	g_ToolTip.CreateTextTexture(m_Texture, text, width, 0);

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
	if (Timer < g_Ticks)
		RemoveMark = true;
}
//----------------------------------------------------------------------------------
