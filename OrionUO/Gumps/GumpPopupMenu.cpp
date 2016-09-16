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
#include "../SelectedObject.h"
//----------------------------------------------------------------------------------
CGumpPopupMenu *g_PopupMenu = NULL;
//----------------------------------------------------------------------------------
CGumpPopupMenu::CGumpPopupMenu(uint serial, short x, short y)
: CGump(GT_POPUP_MENU, serial, x, y), m_Width(0), m_Height(0)
{
	m_NoMove = true;
	g_PopupMenu = this;

	m_Polygone = (CGUIColoredPolygone*)Add(new CGUIColoredPolygone(0, 0, 0, 0, 0, 0, 0x7F7F7F7F));
	m_Polygone->Visible = false;
}
//----------------------------------------------------------------------------------
CGumpPopupMenu::~CGumpPopupMenu()
{
	g_PopupMenu = NULL;
}
//----------------------------------------------------------------------------------
void CGumpPopupMenu::PrepareContent()
{
	m_WantRedraw = true;

	if (g_SelectedObject.Gump() == this && g_SelectedObject.Object() != NULL && ((CBaseGUI*)g_SelectedObject.Object())->Type == GOT_HITBOX)
	{
		CGUIHitBox *box = (CGUIHitBox*)g_SelectedObject.Object();

		m_Polygone->X = box->X;
		m_Polygone->Y = box->Y;
		m_Polygone->Width = box->Width;
		m_Polygone->Height = box->Height;
		m_Polygone->Visible = true;
	}
	else
		m_Polygone->Visible = false;
}
//----------------------------------------------------------------------------------
void CGumpPopupMenu::GUMP_BUTTON_EVENT_C
{
	CPacketPopupMenuSelection(m_Serial, serial - 1).Send();

	m_RemoveMark = true;
}
//----------------------------------------------------------------------------------
