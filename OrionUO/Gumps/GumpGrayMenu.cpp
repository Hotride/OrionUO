/***********************************************************************************
**
** GumpGrayMenu.cpp
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#include "GumpGrayMenu.h"
#include "../OrionUO.h"
#include "../Screen stages/GameBlockedScreen.h"
#include "../Network/Packets.h"
//----------------------------------------------------------------------------------
CGumpGrayMenu::CGumpGrayMenu(uint serial, uint id, short x, short y)
: CGump(GT_GRAY_MENU, serial, x, y)
{
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
	//Ответ на меню
	CPacketGrayMenuResponse(this, index).Send();

	//Удаляем использованный гамп
	m_RemoveMark = true;
}
//----------------------------------------------------------------------------------
