// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
/***********************************************************************************
**
** GumpMenu.cpp
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#include "stdafx.h"
//----------------------------------------------------------------------------------
CGumpMenu::CGumpMenu(uint serial, uint id, short x, short y)
: CGump(GT_MENU, serial, x, y)
{
	m_ID = id;
}
//----------------------------------------------------------------------------------
CGumpMenu::~CGumpMenu()
{
}
//----------------------------------------------------------------------------------
void CGumpMenu::CalculateGumpState()
{
	WISPFUN_DEBUG("c100_f1");
	CGump::CalculateGumpState();

	if (g_GumpPressed)
		m_FrameCreated = false;
}
//----------------------------------------------------------------------------------
void CGumpMenu::PrepareContent()
{
	WISPFUN_DEBUG("c100_f2");
	if (m_TextChanged)
	{
		m_TextChanged = false;

		if (m_TextObject != NULL)
		{
			if (m_Text.length())
				m_TextObject->CreateTextureA(1, m_Text, 200, TS_LEFT, UOFONT_FIXED);
			else
				m_TextObject->m_Texture.Clear();

			m_WantRedraw = true;
		}
	}
}
//----------------------------------------------------------------------------------
bool CGumpMenu::OnLeftMouseButtonDoubleClick()
{
	WISPFUN_DEBUG("c100_f3");
	if (g_PressedObject.LeftSerial && g_PressedObject.LeftSerial != ID_GM_HTMLGUMP)
	{
		SendMenuResponse(g_PressedObject.LeftSerial);

		return true;
	}

	return false;
}
//----------------------------------------------------------------------------------
void CGumpMenu::SendMenuResponse(int index)
{
	WISPFUN_DEBUG("c100_f4");
	//Ответ на меню
	CPacketMenuResponse(this, index).Send();

	//Удаляем использованный гамп
	m_RemoveMark = true;
}
//----------------------------------------------------------------------------------
