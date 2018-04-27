﻿// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
/***********************************************************************************
**
** GumpProfile.cpp
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#include "stdafx.h"
#include "GumpProfile.h"
//----------------------------------------------------------------------------------
CGumpProfile::CGumpProfile(uint serial, short x, short y, wstring topText, wstring bottomText, wstring dataText)
	: CGumpBaseScroll(GT_PROFILE, serial, 0x0820, 250, x, y, true)
{
	WISPFUN_DEBUG("c108_f1");
	Changed = false;
	Add(new CGUIPage(1));
	Add(new CGUIGumppic(0x09D4, 0, 0));

	Add(new CGUIPage(2));

	CGUIText *text = new CGUIText(0, 53, 6);
	text->CreateTextureW(0, topText, 30, 140);

	int offsetY = text->m_Texture.Height - 15;

	m_HTMLGump->Add(new CGUIButton(ID_GP_APPLY, 0x005C, 0x005C, 0x005C, 4, offsetY));
	m_HTMLGump->Add(new CGUIGumppicTiled(0x005D, 56, offsetY, 138, 0));
	m_HTMLGump->Add(new CGUIGumppic(0x005E, 194, offsetY));

	m_HTMLGump->Add(text);

	offsetY += 44;

	m_HitBox = (CGUIHitBox*)m_HTMLGump->Add(new CGUIHitBox(ID_GP_TEXT_FIELD, 4, offsetY, 210, 14));

	m_Entry = (CGUITextEntry*)m_HTMLGump->Add(new CGUITextEntry(ID_GP_TEXT_FIELD, 0, 0, 0, 4, offsetY));
	m_Entry->m_Entry.Width = 210;
	m_Entry->CheckOnSerial = true;
	m_Entry->m_Entry.SetText(dataText);
	m_Entry->m_Entry.CreateTextureW(0, dataText, 0, 210, TS_LEFT, 0);

	if (m_Entry->m_Entry.m_Texture.Height > 14)
		m_HitBox->Height = m_Entry->m_Entry.m_Texture.Height;

	offsetY += m_HitBox->Height + 3;

	m_BottomData[0] = (CBaseGUI*)m_HTMLGump->Add(new CGUIGumppic(0x005F, 4, offsetY));
	m_BottomData[1] = (CBaseGUI*)m_HTMLGump->Add(new CGUIGumppicTiled(0x0060, 13, offsetY + 9, 197, 0));
	m_BottomData[2] = (CBaseGUI*)m_HTMLGump->Add(new CGUIGumppic(0x0061, 210, offsetY));

	text = (CGUIText*)m_HTMLGump->Add(new CGUIText(0, 2, offsetY + 26));
	text->CreateTextureW(0, bottomText, 30, 190);
	m_BottomData[3] = text;

	m_HTMLGump->CalculateDataSize();
}
//----------------------------------------------------------------------------------
CGumpProfile::~CGumpProfile()
{
	WISPFUN_DEBUG("c108_f2");
	if (Changed && m_Entry != NULL)
		CPacketProfileUpdate(Serial, m_Entry->m_Entry.Data(), m_Entry->m_Entry.Length()).Send();
}
//----------------------------------------------------------------------------
void CGumpProfile::RecalculateHeight()
{
	WISPFUN_DEBUG("c108_f3");
	int offsetY = m_Entry->GetY();
	m_Entry->m_Entry.CreateTextureW(0, m_Entry->m_Entry.Data(), 0, 210, TS_LEFT, 0);

	if (m_Entry->m_Entry.m_Texture.Height > 14)
		m_HitBox->Height = m_Entry->m_Entry.m_Texture.Height;
	else
		m_HitBox->Height = 14;

	offsetY += m_HitBox->Height + 3;

	m_BottomData[0]->SetY(offsetY);
	m_BottomData[1]->SetY(offsetY + 9);
	m_BottomData[2]->SetY(offsetY);
	m_BottomData[3]->SetY(offsetY + 26);

	m_HTMLGump->CalculateDataSize();
}
//----------------------------------------------------------------------------
void CGumpProfile::GUMP_BUTTON_EVENT_C
{
	WISPFUN_DEBUG("c108_f4");
	if (serial == ID_GBS_BUTTON_MINIMIZE)
	{
		Minimized = true;
		Page = 1;
		WantRedraw = true;
	}
	else if (Serial == g_PlayerSerial && serial == ID_GP_APPLY && m_Entry != NULL)
	{
		Changed = true;
		
		if (g_EntryPointer == &m_Entry->m_Entry)
		{
			if (g_ConfigManager.GetConsoleNeedEnter())
				g_EntryPointer = NULL;
			else
				g_EntryPointer = &g_GameConsole;
		}

		WantRedraw = true;
	}
}
//----------------------------------------------------------------------------
bool CGumpProfile::OnLeftMouseButtonDoubleClick()
{
	WISPFUN_DEBUG("c108_f5");
	if (Minimized)
	{
		Minimized = false;
		Page = 2;
		WantRedraw = true;

		return true;
	}

	return false;
}
//----------------------------------------------------------------------------
#if USE_WISP
void CGumpProfile::OnCharPress(const WPARAM &wParam, const LPARAM &lParam)
{
	WISPFUN_DEBUG("c108_f6");
	g_EntryPointer->Insert((wchar_t)wParam);

	RecalculateHeight();

	WantRedraw = true;
}
//----------------------------------------------------------------------------
void CGumpProfile::OnKeyDown(const WPARAM &wParam, const LPARAM &lParam)
{
	WISPFUN_DEBUG("c108_f7");
	if (wParam == VK_RETURN)
	{
		g_EntryPointer->Insert(0x000D);

		RecalculateHeight();

		WantRedraw = true;
	}
	else
	{
		g_EntryPointer->OnKey(this, wParam);

		if (WantRedraw)
			RecalculateHeight();
	}
}
#else
void CGumpProfile::OnTextInput(const SDL_TextInputEvent &ev)
{
  NOT_IMPLEMENTED; // FIXME
}
void CGumpProfile::OnKeyDown(const SDL_KeyboardEvent &ev)
{
  NOT_IMPLEMENTED; // FIXME
}
#endif