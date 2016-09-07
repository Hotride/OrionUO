/***********************************************************************************
**
** GumpNotify.cpp
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#include "GumpNotify.h"
#include "../Screen stages/GameScreen.h"
#include "../Managers/GumpManager.h"
#include "../Managers/ConfigManager.h"
#include "../TextEngine/GameConsole.h"
//----------------------------------------------------------------------------------
CGumpNotify::CGumpNotify(uint serial, short x, short y, uchar variant, short width, short height, string text)
: CGump(GT_NOTIFY, serial, x, y), m_Variant(variant), m_Width(width),
m_Height(height), m_Text(text)
{
	Blocked = true;
	g_GrayMenuCount++;

	Add(new CGUIResizepic(0, 0x0A28, 0, 0, m_Width, m_Height));

	CGUIText *obj = (CGUIText*)Add(new CGUIText(0x0386, 40, 45));
	obj->CreateTextureA(1, m_Text, m_Width - 90);

	Add(new CGUIButton(ID_GN_BUTTON_OK, 0x0481, 0x0482, 0x0483, (m_Width / 2) - 13, m_Height - 45));
}
//----------------------------------------------------------------------------------
CGumpNotify::~CGumpNotify()
{
}
//----------------------------------------------------------------------------------
void CGumpNotify::GUMP_BUTTON_EVENT_C
{
	if (serial == ID_GN_BUTTON_OK)
		Process();
}
//----------------------------------------------------------------------------------
void CGumpNotify::OnKeyDown(const WPARAM &wParam, const LPARAM &lParam)
{
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
	if (m_Variant == ID_GN_STATE_LOGOUT)
		g_GameScreen.CreateSmoothAction(CGameScreen::ID_SMOOTH_GS_LOGOUT);
	else if (m_Variant == ID_GN_STATE_NOTIFICATION)
		m_RemoveMark = true;
}
//----------------------------------------------------------------------------------
