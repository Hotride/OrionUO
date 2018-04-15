// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
/***********************************************************************************
**
** GumpScreenMain.cpp
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#include "stdafx.h"
//----------------------------------------------------------------------------------
CGumpScreenMain::CGumpScreenMain()
: CGump(GT_NONE, 0, 0, 0)
{
	NoMove = true;
	NoClose = true;
}
//----------------------------------------------------------------------------------
CGumpScreenMain::~CGumpScreenMain()
{
}
//----------------------------------------------------------------------------------
void CGumpScreenMain::PrepareContent()
{
	WISPFUN_DEBUG("c116_f1");
	static uint lastArrowTick = 0;
	static bool arrowLighted = false;

	if (lastArrowTick < g_Ticks && m_Arrow != NULL)
	{
		arrowLighted = !arrowLighted;

		if (arrowLighted)
			m_Arrow->Graphic = 0x15A5;
		else
			m_Arrow->Graphic = 0x15A4;

		lastArrowTick = g_Ticks + 500;

		WantRedraw = true;
	}
}
//----------------------------------------------------------------------------------
void CGumpScreenMain::UpdateContent()
{
	WISPFUN_DEBUG("c116_f2");
	if (m_Items != NULL)
		return;

	//Clear();

	Add(new CGUIGumppicTiled(0x0E14, 0, 0, 640, 480));

	Add(new CGUIGumppic(0x157C, 0, 0));

	if (g_PacketManager.GetClientVersion() >= CV_500A)
		Add(new CGUIGumppic(0x2329, 0, 0));

	Add(new CGUIGumppic(0x15A0, 0, 4));
	Add(new CGUIResizepic(0, 0x13BE, 128, 288, 451, 157));

	if (g_PacketManager.GetClientVersion() < CV_500A)
		Add(new CGUIGumppic(0x058A, 286, 45));

	Add(new CGUIButton(ID_MS_QUIT, 0x1589, 0x158A, 0x158B, 555, 4));

	m_Arrow = (CGUIButton*)Add(new CGUIButton(ID_MS_ARROW_NEXT, 0x15A4, 0x15A5, 0x15A6, 610, 445));
	Add(new CGUIResizepic(ID_MS_ACCOUNT, 0x0BB8, 328, 343, 210, 30));
	Add(new CGUIResizepic(ID_MS_PASSWORD, 0x0BB8, 328, 383, 210, 30));

	g_MainScreen.m_SavePassword = (CGUICheckbox*)Add(new CGUICheckbox(ID_MS_SAVEPASSWORD, 0x00D2, 0x00D3, 0x00D2, 328, 417));
	g_MainScreen.m_SavePassword->SetTextParameters(9, "Save Password", 0x0386, STP_RIGHT_CENTER);

	g_MainScreen.m_AutoLogin = (CGUICheckbox*)Add(new CGUICheckbox(ID_MS_AUTOLOGIN, 0x00D2, 0x00D3, 0x00D2, 183, 417));
	g_MainScreen.m_AutoLogin->SetTextParameters(9, "Auto Login", 0x0386, STP_RIGHT_CENTER);

	CGUIText *text = (CGUIText*)Add(new CGUIText(0x0386, 253, 305));
	text->CreateTextureA(2, "Log in to Ultima Online");

	text = (CGUIText*)Add(new CGUIText(0x0386, 183, 345));
	text->CreateTextureA(2, "Account Name");

	text = (CGUIText*)Add(new CGUIText(0x0386, 183, 385));
	text->CreateTextureA(2, "Password");

	text = (CGUIText*)Add(new CGUIText(0x034E, 286, 455));
	text->CreateTextureA(9, string("UO Version " + g_Orion.ClientVersionText + "."));

	text = (CGUIText*)Add(new CGUIText(0x034E, 286, 467));
	text->CreateTextureA(9, string("Orion beta v") + g_App.GetFileVersion());

	CGUITextEntry *entry = (CGUITextEntry*)Add(new CGUITextEntry(ID_MS_ACCOUNT, 0x034F, 0x03E3, 0x0021, 335, 343, 190, false, 5, TS_LEFT, 0, 32));
	entry->CheckOnSerial = true;
	g_MainScreen.m_Account = &entry->m_Entry;

	entry = (CGUITextEntry*)Add(new CGUITextEntry(ID_MS_PASSWORD, 0x034F, 0x03E3, 0x0021, 335, 385, 190, false, 5, TS_LEFT, 0, 32));
	entry->CheckOnSerial = true;
	m_PasswordFake = &entry->m_Entry;
}
//----------------------------------------------------------------------------------
void CGumpScreenMain::InitToolTip()
{
	WISPFUN_DEBUG("c116_f3");
	if (!g_ConfigManager.UseToolTips || g_SelectedObject.Object == NULL)
		return;

	uint id = g_SelectedObject.Serial;

	switch (id)
	{
		case ID_MS_QUIT:
		{
			g_ToolTip.Set(L"Quit Ultima Online", 80);
			break;
		}
		case ID_MS_ARROW_NEXT:
		{
			g_ToolTip.Set(L"Next screen");
			break;
		}
		case ID_MS_ACCOUNT:
		{
			g_ToolTip.Set(L"Click here to enter your user name", 150);
			break;
		}
		case ID_MS_PASSWORD:
		{
			g_ToolTip.Set(L"Click here to enter your password", 150);
			break;
		}
		case ID_MS_SAVEPASSWORD:
		{
			g_ToolTip.Set(L"Save your password in config file", 150);
			break;
		}
		case ID_MS_AUTOLOGIN:
		{
			g_ToolTip.Set(L"Auto LogIn (without selection screens)", 150);
			break;
		}
		default:
			break;
	}
}
//----------------------------------------------------------------------------------
void CGumpScreenMain::GUMP_BUTTON_EVENT_C
{
	WISPFUN_DEBUG("c116_f4");
	if (serial == ID_MS_QUIT) //x button
		g_MainScreen.CreateSmoothAction(CMainScreen::ID_SMOOTH_MS_QUIT);
	else if (serial == ID_MS_ARROW_NEXT) //> button
		g_MainScreen.CreateSmoothAction(CMainScreen::ID_SMOOTH_MS_CONNECT);
}
//----------------------------------------------------------------------------------
void CGumpScreenMain::GUMP_TEXT_ENTRY_EVENT_C
{
	WISPFUN_DEBUG("c116_f5");
	if (serial == ID_MS_PASSWORD)
		g_MainScreen.m_Password->SetPos(m_PasswordFake->Pos());
}
//----------------------------------------------------------------------------------
