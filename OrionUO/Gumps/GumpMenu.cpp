/***********************************************************************************
**
** GumpMenu.cpp
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#include "GumpMenu.h"
#include "../PressedObject.h"
#include "../Network/Packets.h"
//----------------------------------------------------------------------------------
CGumpMenu::CGumpMenu(uint serial, uint id, short x, short y)
: CGump(GT_MENU, serial, x, y), m_Text(""), m_TextChanged(false), m_TextObject(NULL)
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
	CGump::CalculateGumpState();

	if (g_GumpPressed)
		m_FrameCreated = false;
}
//----------------------------------------------------------------------------------
void CGumpMenu::PrepareContent()
{
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
	//Ответ на меню
	CPacketMenuResponse(this, index).Send();

	//Удаляем использованный гамп
	m_RemoveMark = true;
}
//----------------------------------------------------------------------------------
