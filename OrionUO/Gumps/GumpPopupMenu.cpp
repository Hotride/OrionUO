/***********************************************************************************
**
** GumpPopupMenu.cpp
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#include "GumpPopupMenu.h"
#include "../Network/Packets.h"
//----------------------------------------------------------------------------------
CGumpPopupMenu *g_PopupMenu = NULL;
//----------------------------------------------------------------------------------
CGumpPopupMenu::CGumpPopupMenu(uint serial, short x, short y)
: CGump(GT_POPUP_MENU, serial, x, y), m_Width(0), m_Height(0)
{
	m_NoMove = true;
	g_PopupMenu = this;
}
//----------------------------------------------------------------------------------
CGumpPopupMenu::~CGumpPopupMenu()
{
	g_PopupMenu = NULL;
}
//----------------------------------------------------------------------------------
void CGumpPopupMenu::GUMP_BUTTON_EVENT_C
{
	CPacketPopupMenuSelection(m_Serial, serial - 1).Send();

	m_RemoveMark = true;
}
//----------------------------------------------------------------------------------
