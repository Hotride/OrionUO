// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
/***********************************************************************************
**
** GumpBulletinBoard.cpp
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#include "stdafx.h"
//----------------------------------------------------------------------------------
CGumpBulletinBoard::CGumpBulletinBoard(uint serial, short x, short y, string name)
: CGump(GT_BULLETIN_BOARD, serial, x, y)
{
	WISPFUN_DEBUG("c89_f1");
	Add(new CGUIGumppic(0x087A, 0, 0));

	CGUIText *text = (CGUIText*)Add(new CGUIText(0x0386, 159, 36));
	text->CreateTextureA(2, name, 170, TS_CENTER);

	Add(new CGUIHitBox(ID_GBB_POST_MESSAGE, 15, 170, 80, 80, true));

	m_HTMLGump = (CGUIHTMLGump*)Add(new CGUIHTMLGump(ID_GBS_HTMLGUMP, 0, 127, 159, 241, 195, false, true));

	if (m_HTMLGump->m_Background != NULL)
		m_HTMLGump->m_Background->Height -= 30;

	if (m_HTMLGump->m_Scissor != NULL)
		m_HTMLGump->m_Scissor->Height -= 30;

	CGUIHTMLButton *button = m_HTMLGump->m_ButtonUp;

	if (button != NULL)
	{
		button->Graphic = 0x0824;
		button->GraphicSelected = 0x0824;
		button->GraphicPressed = 0x0824;
		button->SelectOnly = true;
		button->CheckPolygone = true;
		button->Y -= 14;
	}

	button = m_HTMLGump->m_ButtonDown;

	if (button != NULL)
	{
		button->Graphic = 0x0825;
		button->GraphicSelected = 0x0825;
		button->GraphicPressed = 0x0825;
		button->SelectOnly = true;
		button->CheckPolygone = true;
		button->Y -= 14;
	}

	CGUIHTMLSlider *slider = m_HTMLGump->m_Slider;

	if (slider != NULL)
	{
		slider->Graphic = 0x001F;
		slider->GraphicSelected = 0x001F;
		slider->GraphicPressed = 0x001F;
		slider->BackgroundGraphic = 0;
		slider->Y -= 14;
	}
}
//----------------------------------------------------------------------------------
CGumpBulletinBoard::~CGumpBulletinBoard()
{
	WISPFUN_DEBUG("c89_f2");
	g_GumpManager.CloseGump(0xFFFFFFFF, m_Serial, GT_BULLETIN_BOARD_ITEM);
}
//----------------------------------------------------------------------------------
void CGumpBulletinBoard::GUMP_BUTTON_EVENT_C
{
	WISPFUN_DEBUG("c89_f3");
	if (serial == ID_GBB_POST_MESSAGE)
	{
		CGumpBulletinBoardItem *gump = new CGumpBulletinBoardItem(0, 0, 0, 0, m_Serial, ToWString(g_Player->Name), L"", L"Date/Time", L"");

		g_GumpManager.AddGump(gump);
	}
}
//----------------------------------------------------------------------------------
bool CGumpBulletinBoard::OnLeftMouseButtonDoubleClick()
{
	WISPFUN_DEBUG("c89_f4");
	bool result = false;

	if (g_PressedObject.LeftObject != NULL && g_PressedObject.LeftObject->IsGUI())
	{
		CBaseGUI *gui = (CBaseGUI*)g_PressedObject.LeftObject;

		if (gui->Type == GOT_BB_OBJECT)
		{
			CPacketBulletinBoardRequestMessage(m_Serial, gui->Serial).Send();

			result = true;
		}
	}

	return result;
}
//----------------------------------------------------------------------------------
