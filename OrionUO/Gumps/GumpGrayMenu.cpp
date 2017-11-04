// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
/***********************************************************************************
**
** GumpGrayMenu.cpp
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#include "stdafx.h"
//----------------------------------------------------------------------------------
CGumpGrayMenu::CGumpGrayMenu(uint serial, uint id, short x, short y)
: CGump(GT_GRAY_MENU, serial, x, y)
{
	WISPFUN_DEBUG("c97_f1");
	m_NoMove = true;
	m_Blocked = true;

	m_ID = id;

	if (!g_GrayMenuCount)
	{
		g_Orion.InitScreen(GS_GAME_BLOCKED);
		g_GameBlockedScreen.Code = 1;
	}

	g_GrayMenuCount++;
}
//----------------------------------------------------------------------------------
CGumpGrayMenu::~CGumpGrayMenu()
{
}
//----------------------------------------------------------------------------------
void CGumpGrayMenu::GUMP_BUTTON_EVENT_C
{
	WISPFUN_DEBUG("c97_f2");
	if (serial == ID_GGM_CANCEL)
		SendMenuResponse(0);
	else if (serial == ID_GGM_CONTINUE)
	{
		QFOR(item, m_Items, CBaseGUI*)
		{
			if (item->Type == GOT_RADIO)
			{
				if (((CGUIRadio*)item)->Checked)
				{
					SendMenuResponse(item->Serial);

					return;
				}
			}
		}

		//Ничего не выбрали
		g_Orion.CreateTextMessage(TT_SYSTEM, 0xFFFFFFFF, 3, 0x0021, "You must choose the section of menu.");
	}
}
//----------------------------------------------------------------------------------
void CGumpGrayMenu::SendMenuResponse(int index)
{
	WISPFUN_DEBUG("c97_f3");
	//Ответ на меню
	CPacketGrayMenuResponse(this, index).Send();

	//Удаляем использованный гамп
	m_RemoveMark = true;
}
//----------------------------------------------------------------------------------
