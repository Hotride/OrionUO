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
	m_Page = 1;

	m_Polygone = (CGUIColoredPolygone*)Add(new CGUIColoredPolygone(0, 0, 0, 0, 0, 0, 0x7F7F7F7F));
	m_Polygone->DrawOnly = true;
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
	if (g_SelectedObject.Gump() == this && g_SelectedObject.Object() != NULL && ((CBaseGUI*)g_SelectedObject.Object())->Type == GOT_HITBOX)
	{
		CGUIHitBox *box = (CGUIHitBox*)g_SelectedObject.Object();

		if (!m_Polygone->Visible || m_Polygone->Y != box->Y)
		{
			m_WantRedraw = true;

			m_Polygone->X = box->X;
			m_Polygone->Y = box->Y;
			m_Polygone->Width = box->Width;
			m_Polygone->Height = box->Height;
			m_Polygone->Visible = true;
		}
	}
	else if (m_Polygone->Visible)
	{
		m_WantRedraw = true;

		m_Polygone->Visible = false;
	}
}
//----------------------------------------------------------------------------------
void CGumpPopupMenu::GUMP_BUTTON_EVENT_C
{
	if (serial == ID_GPM_MAXIMIZE)
	{
		m_Page = 2;
		CGUIResizepic *resizepic = NULL;
		int width = 0;
		int height = 20;

		QFOR(item, m_Items, CBaseGUI*)
		{
			if (item->Type == GOT_RESIZEPIC)
				resizepic = (CGUIResizepic*)item;
			else if (item->Type == GOT_TEXTENTRY)
			{
				CGLTextTexture &texture = ((CGUITextEntry*)item)->m_Entry.m_Texture;

				if (width < texture.Width)
					width = texture.Width;

				height += texture.Height;
			}
		}

		width += 20;

		QFOR(item, m_Items, CBaseGUI*)
		{
			if (item->Type == GOT_HITBOX)
				((CGUIHitBox*)item)->Width = width - 20;
		}

		if (resizepic != NULL)
		{
			resizepic->Width = width;
			resizepic->Height = height;
		}

		m_WantRedraw = true;
		m_Polygone->Visible = false;
	}
	else
	{
		CPacketPopupMenuSelection(m_Serial, serial - 1).Send();

		m_RemoveMark = true;
	}
}
//----------------------------------------------------------------------------------
