// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
/***********************************************************************************
**
** GumpNotify.cpp
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#include "stdafx.h"
//----------------------------------------------------------------------------------
CGumpNotify::CGumpNotify(short x, short y, uchar variant, short width, short height, string text)
: CGump(GT_NOTIFY, 0, x, y), Variant(variant), Width(width),
Height(height), Text(text)
{
	WISPFUN_DEBUG("c103_f1");
	Blocked = true;
	g_GrayMenuCount++;

	Add(new CGUIResizepic(0, 0x0A28, 0, 0, Width, Height));

	CGUIText *obj = (CGUIText*)Add(new CGUIText(0x0386, 40, 45));
	obj->CreateTextureA(1, Text, Width - 90);

	Add(new CGUIButton(ID_GN_BUTTON_OK, 0x0481, 0x0482, 0x0483, (Width / 2) - 13, Height - 45));
}
//----------------------------------------------------------------------------------
CGumpNotify::~CGumpNotify()
{
}
//----------------------------------------------------------------------------------
void CGumpNotify::GUMP_BUTTON_EVENT_C
{
	WISPFUN_DEBUG("c103_f2");
	if (serial == ID_GN_BUTTON_OK)
		Process();
}
//----------------------------------------------------------------------------------
void CGumpNotify::OnKeyDown(const WPARAM &wParam, const LPARAM &lParam)
{
	WISPFUN_DEBUG("c103_f3");
	if (wParam == VK_RETURN)
	{
		if (g_ConfigManager.ConsoleNeedEnter)
			g_EntryPointer = NULL;
		else
			g_EntryPointer = &g_GameConsole;

		Process();
	}
}
//----------------------------------------------------------------------------------
void CGumpNotify::Process()
{
	WISPFUN_DEBUG("c103_f4");
	if (Variant == ID_GN_STATE_LOGOUT)
		g_GameScreen.CreateSmoothAction(CGameScreen::ID_SMOOTH_GS_LOGOUT);
	else if (Variant == ID_GN_STATE_NOTIFICATION)
		RemoveMark = true;
}
//----------------------------------------------------------------------------------
