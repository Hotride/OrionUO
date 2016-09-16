/***********************************************************************************
**
** GumpScreenMain.cpp
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#include "GumpScreenMain.h"
#include "../Screen stages/MainScreen.h"
#include "../OrionUO.h"
#include "../Managers/ConfigManager.h"
//----------------------------------------------------------------------------------
CGumpScreenMain::CGumpScreenMain()
: CGump(GT_NONE, 0, 0, 0), m_Arrow(NULL), m_PasswordFake(NULL)
{
	m_NoMove = true;
	m_NoClose = true;
}
//----------------------------------------------------------------------------------
CGumpScreenMain::~CGumpScreenMain()
{
}
//----------------------------------------------------------------------------------
void CGumpScreenMain::PrepareContent()
{
	static uint lastArrowTick = 0;
	static bool arrowLighted = false;

	if (lastArrowTick < g_Ticks)
	{
		arrowLighted = !arrowLighted;

		if (arrowLighted)
			m_Arrow->Graphic = 0x15A5;
		else
			m_Arrow->Graphic = 0x15A4;

		lastArrowTick = g_Ticks + 500;

		m_WantRedraw = true;
	}
}
//----------------------------------------------------------------------------------
void CGumpScreenMain::GUMP_SCROLL_BUTTON_EVENT_C
{
	CGUIMinMaxButtons *minmax = (CGUIMinMaxButtons*)g_PressedObject.LeftObject();

	if (minmax == NULL)
		return;

	CGUIHTMLGump *htmlGump = NULL;

	QFOR(item, m_Items, CBaseGUI*)
	{
		if (item->Type == GOT_SHOPITEM && item->Serial == minmax->Serial)
		{
			((CGUIShopItem*)item)->CreateCountText(minmax->Value);
			return;
		}
		else if (item->Type == GOT_HTMLGUMP && item->Serial == 0111)
			htmlGump = (CGUIHTMLGump*)item;
	}

	if (htmlGump == NULL)
		return;

	QFOR(item, htmlGump->m_Items, CBaseGUI*)
	{
		if (item->Type == GOT_SHOPITEM && item->Serial == minmax->Serial)
		{
			((CGUIShopItem*)item)->CreateCountText(minmax->Value);
			return;
		}
	}
}
/*#include "../Game objects/GamePlayer.h"
#include "../Managers/SkillGroupManager.h"
CGUISkillGroup *g_Grp;*/
//----------------------------------------------------------------------------------
void CGumpScreenMain::UpdateContent()
{
	if (m_Items != NULL)
		return;

	//Clear();

	Add(new CGUIGumppicTiled(0x0588, 0, 0, 640, 480));
	Add(new CGUIGumppic(0x0E14, 0, 0));
	Add(new CGUIGumppic(0x157C, 0, 0));
	Add(new CGUIGumppic(0x15A0, 0, 4));
	Add(new CGUIResizepic(0, 0x13BE, 128, 288, 451, 157));
	Add(new CGUIGumppic(0x058A, 286, 45));
	Add(new CGUIButton(ID_MS_QUIT, 0x1589, 0x158A, 0x158B, 555, 4));
	m_Arrow = (CGUIButton*)Add(new CGUIButton(ID_MS_ARROW_NEXT, 0x15A4, 0x15A5, 0x15A6, 610, 445));
	Add(new CGUIResizepic(ID_MS_ACCOUNT, 0x0BB8, 328, 343, 210, 30));
	Add(new CGUIResizepic(ID_MS_PASSWORD, 0x0BB8, 328, 383, 210, 30));
	g_MainScreen.m_SavePassword = (CGUICheckbox*)Add(new CGUICheckbox(ID_MS_SAVEPASSWORD, 0x00D2, 0x00D3, 0x00D2, 328, 417));
	g_MainScreen.m_AutoLogin = (CGUICheckbox*)Add(new CGUICheckbox(ID_MS_AUTOLOGIN, 0x00D2, 0x00D3, 0x00D2, 183, 417));

	CGUIText *text = (CGUIText*)Add(new CGUIText(0x0386, 253, 305));
	text->CreateTextureA(2, "Log in to Ultima Online");
	text = (CGUIText*)Add(new CGUIText(0x0386, 183, 345));
	text->CreateTextureA(2, "Account Name");
	text = (CGUIText*)Add(new CGUIText(0x0386, 183, 385));
	text->CreateTextureA(2, "Password");
	text = (CGUIText*)Add(new CGUIText(0x0386, 351, 421));
	text->CreateTextureA(9, "Save Password");
	text = (CGUIText*)Add(new CGUIText(0x0386, 206, 421));
	text->CreateTextureA(9, "Auto Login");
	text = (CGUIText*)Add(new CGUIText(0x034E, 286, 455));
	text->CreateTextureA(9, string("UO Version " + g_Orion.ClientVersionText + "."));

	CGUITextEntry *entry = (CGUITextEntry*)Add(new CGUITextEntry(ID_MS_ACCOUNT, 0x034F, 0x03E3, 0x0021, 335, 343, 300, false, 5, TS_LEFT, 0, 32));
	entry->CheckOnSerial = true;
	g_MainScreen.m_Account = &entry->m_Entry;
	entry = (CGUITextEntry*)Add(new CGUITextEntry(ID_MS_PASSWORD, 0x034F, 0x03E3, 0x0021, 335, 385, 300, false, 5, TS_LEFT, 0, 32));
	entry->CheckOnSerial = true;
	m_PasswordFake = &entry->m_Entry;

	/*g_Player = new CPlayer(1);
	CSkillGroupObject *gobj = new CSkillGroupObject();
	gobj->Name = "SomeGroup";

	g_Grp = (CGUISkillGroup*)Add(new CGUISkillGroup(2999, 2988, gobj, 100, 100));

	g_Grp->Add(new CGUISkillItem(3000, 4000, 5000, 3, 0, 00));
	g_Grp->Add(new CGUISkillItem(3001, 4001, 5001, 7, 0, 17));
	g_Grp->Add(new CGUISkillItem(3002, 4002, 5002, 14, 0, 34));*/

	/*CGUIMinMaxButtons *minmax = (CGUIMinMaxButtons*)Add(new CGUIMinMaxButtons(123320, 0x0037, 100, 10, 0, 1735, 0));
	minmax->SetTextParameters(true, STP_TOP_CENTER, 3, 0x0021, false);

	Add(new CGUIShopItem(123320, 0x0324, 0x0021, 1735, 164, "WTH bro?!?17171 what is it?!?!? really, what is it???777", 100, 150));*/

	/*CGUIComboBox *combo = NULL;

	combo = (CGUIComboBox*)Add(new CGUIComboBox(200, 0x0BB8, false, 0x0BB8, 10, 110, 200, 5, false));
	IFOR(i, 0, 30)
		combo->Add(new CGUIComboboxText(0x0386, 9, "Some text data chars: " + std::to_string(i + 1)));

	combo = (CGUIComboBox*)Add(new CGUIComboBox(300, 0x098D, true, 0x09B5, 210, 110, 0, 6, true));
	IFOR(i, 0, 30)
		combo->Add(new CGUIComboboxText(0x0386, 9, "Any text data chars: " + std::to_string(i + 1)));*/

	//Add(new CGUIScrollBackground(0, 0x0820, 0, 0, 300)); //Journal
	//Add(new CGUIScrollBackground(0, 0x1F40, 200, 0, 300)); //Skills
	
	/*CGUIHTMLGump *htmlGump = (CGUIHTMLGump*)Add(new CGUIHTMLGump(0111, 0x0BB8, 20, 60, 260, 260, true, true));
	CGUIHTMLGump *htmlGump2 = (CGUIHTMLGump*)Add(new CGUIHTMLGump(0112, 0x0BB8, 320, 60, 260, 260, true, true));

	htmlGump->Add(new CGUIShader(g_ColorizerShader, true));

	int cy = 0;
	int cy2 = 0;
	IFOR(i, 0, 13)
	{
		CGUIShopItem *si = (CGUIShopItem*)htmlGump->Add(new CGUIShopItem(123321 + i, 0x0324 + i, 0x0021 + i, 1735, 164, "WTH bro?!?17171", 0, cy));
		cy += si->GetSize().Height;

		CGUIShopResult *sr = (CGUIShopResult*)htmlGump2->Add(new CGUIShopResult(si, 0, cy2));
		cy2 += sr->GetSize().Height;
	}
	htmlGump->Add(new CGUIShader(g_ColorizerShader, false));
	htmlGump->CalculateDataSize();
	htmlGump2->CalculateDataSize();*/

	/*CGUISlider *slider = new CGUISlider(0, 0x00D8, 0x00D8, 0x00D8, 0x00D5, true, false, 100, 100, 295, -40, 40, 20);
	slider->SetTextParameters(true, STP_RIGHT_CENTER, 3, 0x0021, false);
	htmlGump->Add(slider);

	slider = new CGUISlider(0, 0x00FE, 0x00FE, 0x00FE, 0x0100, false, true, 200, 340, 95, -40, 255, 177);
	slider->SetTextParameters(true, STP_RIGHT_CENTER, 3, 0x0021, false);
	htmlGump->Add(slider);

	htmlGump->CalculateDataSize();*/
}
//----------------------------------------------------------------------------------
void CGumpScreenMain::InitToolTip()
{
	if (!g_ConfigManager.UseToolTips || g_SelectedObject.Object() == NULL)
		return;

	uint id = g_SelectedObject.Serial;

	switch (id)
	{
		case ID_MS_QUIT:
		{
			g_ToolTip.Set(L"Quit Ultima Online", g_SelectedObject.Object(), 80);
			break;
		}
		case ID_MS_ARROW_NEXT:
		{
			g_ToolTip.Set(L"Next screen", g_SelectedObject.Object());
			break;
		}
		case ID_MS_ACCOUNT:
		{
			g_ToolTip.Set(L"Click here to enter your user name", g_SelectedObject.Object(), 150);
			break;
		}
		case ID_MS_PASSWORD:
		{
			g_ToolTip.Set(L"Click here to enter your password", g_SelectedObject.Object(), 150);
			break;
		}
		case ID_MS_SAVEPASSWORD:
		{
			g_ToolTip.Set(L"Save your password in config file", g_SelectedObject.Object(), 150);
			break;
		}
		case ID_MS_AUTOLOGIN:
		{
			g_ToolTip.Set(L"Auto LogIn (without selection screens)", g_SelectedObject.Object(), 150);
			break;
		}
		default:
			break;
	}
}
//----------------------------------------------------------------------------------
void CGumpScreenMain::OnButton(const uint &serial)
{
	if (serial == ID_MS_QUIT) //x button
		g_MainScreen.CreateSmoothAction(CMainScreen::ID_SMOOTH_MS_QUIT);
	else if (serial == ID_MS_ARROW_NEXT) //> button
		g_MainScreen.CreateSmoothAction(CMainScreen::ID_SMOOTH_MS_CONNECT);
	/*else if (serial == 2988)
		g_Grp->Minimized = !g_Grp->Minimized;
	else
	{
		switch (serial)
		{
			case 4000:
			case 4001:
			case 4002:
			{
						 DebugMsg("Skill button pressed: %i\n", serial - 4000);
						 break;
			}
			case 5000:
			case 5001:
			case 5002:
			{
						 DebugMsg("Skill locker pressed: %i\n", serial - 4000);
						 break;
			}
		}
	}*/
}
//----------------------------------------------------------------------------------
void CGumpScreenMain::OnTextEntry(const uint &serial)
{
	if (serial == ID_MS_PASSWORD)
		g_MainScreen.m_Password->SetPos(m_PasswordFake->Pos());
}
//----------------------------------------------------------------------------------
