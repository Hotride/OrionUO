﻿// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
/***********************************************************************************
**
** GumpTip.cpp
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#include "stdafx.h"
//----------------------------------------------------------------------------------
CGumpTip::CGumpTip(uint serial, short x, short y, string str, bool updates)
: CGumpBaseScroll(GT_TIP, serial, 0x0820, 250, x, y, true), Updates(updates)
{
	WISPFUN_DEBUG("c131_f1");
	Add(new CGUIPage(1));
	Add(new CGUIGumppic((Updates ? 0x09D3 : 0x09CB), 0, 0));

	Add(new CGUIPage(2));

	if (Updates) //Updates text gump
	{
		Add(new CGUIGumppic(0x09D2, 112, 35));

		m_ButtonPrevGump = NULL;
		m_ButtonNextGump = NULL;
	}
	else //Tips text gump
	{
		Add(new CGUIGumppic(0x09CA, 127, 33));

		m_ButtonPrevGump = (CGUIButton*)Add(new CGUIButton(ID_GT_PREV_GUMP, 0x09CC, 0x09CC, 0x09CC, 35, Height)); //Previous gump
		m_ButtonPrevGump->CheckPolygone = true;
		//m_ButtonRemoveGump = (CGUIButton*)Add(new CGUIButton(ID_GT_REMOVE_GUMP, 0x0823, 0x0823, 0x0823, 18, Height + 34)); //+/- gump
		m_ButtonNextGump = (CGUIButton*)Add(new CGUIButton(ID_GT_NEXT_GUMP, 0x09CD, 0x09CD, 0x09CD, 251, Height)); //Next gump
		m_ButtonNextGump->CheckPolygone = true;
	}

	CGUIText *text = (CGUIText*)m_HTMLGump->Add(new CGUIText(0, 3, 3));
	text->CreateTextureA(6, str, 200);
	m_HTMLGump->CalculateDataSize();
}
//----------------------------------------------------------------------------------
CGumpTip::~CGumpTip()
{
}
//----------------------------------------------------------------------------------
void CGumpTip::UpdateHeight()
{
	WISPFUN_DEBUG("c131_f2");
	CGumpBaseScroll::UpdateHeight();

	if (!Updates)
	{
		m_ButtonPrevGump->SetY(Height);
		//m_ButtonRemoveGump->SetY(Height + 34);
		m_ButtonNextGump->SetY(Height);
	}
}
//----------------------------------------------------------------------------------
void CGumpTip::GUMP_BUTTON_EVENT_C
{
	WISPFUN_DEBUG("c131_f3");
	if (serial == ID_GBS_BUTTON_MINIMIZE)
	{
		Minimized = true;
		Page = 1;
		WantRedraw = true;
	}
	else if (serial == ID_GT_PREV_GUMP)
		SendTipRequest(0);
	else if (serial == ID_GT_NEXT_GUMP)
		SendTipRequest(1);
}
//----------------------------------------------------------------------------------
bool CGumpTip::OnLeftMouseButtonDoubleClick()
{
	WISPFUN_DEBUG("c131_f4");
	if (Minimized)
	{
		Minimized = false;
		Page = 2;
		WantRedraw = true;

		return true;
	}

	return false;
}
//----------------------------------------------------------------------------------
void CGumpTip::SendTipRequest(uchar flag)
{
	WISPFUN_DEBUG("c131_f5");
	//Отправляем запрос диалога Tip/Updates
	CPacketTipRequest((ushort)Serial, flag).Send();

	//Удаляем использованный гамп
	RemoveMark = true;
}
//----------------------------------------------------------------------------------
