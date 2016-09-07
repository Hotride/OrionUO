/***********************************************************************************
**
** GumpScreenSelectTown.cpp
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#include "GumpScreenSelectTown.h"
#include "../Screen stages/SelectTownScreen.h"
#include "../OrionUO.h"
#include "../Managers/ConfigManager.h"
#include "../Managers/CityManager.h"
#include "../Managers/ClilocManager.h"
#include "../Managers/FontsManager.h"
#include "../CityList.h"
//----------------------------------------------------------------------------------
//!Список точек для отображения кнопок городов
WISP_GEOMETRY::CPoint2Di CGumpScreenSelectTown::m_TownButtonText[9] =
{
	//Button     Text
	{ 105, 130 }, //Yew
	{ 245, 90 }, //Minoc
	{ 165, 200 }, //Britain
	{ 395, 160 }, //Moonglow
	{ 200, 305 }, //Trinsic
	{ 335, 250 }, //Magincia
	{ 160, 395 }, //Jhelom
	{ 100, 250 }, //Skara Brae
	{ 270, 130 } //Vesper
};
//----------------------------------------------------------------------------------
CGumpScreenSelectTown::CGumpScreenSelectTown()
: CGump(GT_NONE, 0, 0, 0), m_HTMLGump(NULL), m_Description(NULL)
{
	m_NoMove = true;
	m_NoClose = true;
}
//----------------------------------------------------------------------------------
CGumpScreenSelectTown::~CGumpScreenSelectTown()
{
}
//----------------------------------------------------------------------------------
void CGumpScreenSelectTown::UpdateContent()
{
	Clear();

	Add(new CGUIGumppicTiled(0x0588, 0, 0, 640, 480));
	Add(new CGUIGumppic(0x1598, 57, 49));
	Add(new CGUIGumppic(0x157C, 0, 0));
	Add(new CGUIGumppic(0x15A0, 0, 4));

	Add(new CGUIButton(ID_STS_QUIT, 0x1589, 0x158A, 0x158B, 555, 4));
	Add(new CGUIButton(ID_STS_ARROW_PREV, 0x15A1, 0x15A2, 0x15A3, 586, 445));
	Add(new CGUIButton(ID_STS_ARROW_NEXT, 0x15A4, 0x15A5, 0x15A6, 610, 445));

	m_HTMLGump = (CGUIHTMLGump*)Add(new CGUIHTMLGump(ID_STS_HTML_GUMP, 0x0BB8, 452, 60, 175, 367, true, true));

	CCityItem city = g_SelectTownScreen.m_City;

	wstring description = city.m_City.Description;

	if (city.IsNewCity())
	{
		CCityItemNew *newCity = (CCityItemNew*)&city;

		//!Получаем строку клилока с описанием города
		description = g_ClilocManager.Cliloc(g_Language)->GetW(newCity->Cliloc);
	}

	//!Используем обработку HTML-тэгов при создании текстуры текста
	g_FontManager.SetUseHTML(true);

	m_Description = (CGUIText*)m_HTMLGump->Add(new CGUIText(0, 3, 3));
	m_Description->CreateTextureW(0, description, 30, 150);

	//!Выключаем обработку HTML-тэгов
	g_FontManager.SetUseHTML(false);

	m_HTMLGump->CalculateDataSize();

	IFOR(i, 0, 9)
	{
		city = g_CityList.GetCity(i + 1);

		int x = m_TownButtonText[i].X;
		int y = m_TownButtonText[i].Y;

		Add(new CGUIButton(ID_STS_TOWN + i, 0x04B9, 0x04BA, 0x04BA, x, y));

		y -= 20;

		if (i == 3) //Moonglow
			x -= 60;

		CGUITextEntry *entry = (CGUITextEntry*)Add(new CGUITextEntry(ID_STS_TOWN + i, 0x0058, 0x0099, 0x0481, x, y, 0, false, 3));
		entry->m_Entry.SetText(city.Name);
		entry->CheckOnSerial = true;
		entry->ReadOnly = true;
		entry->Focused = (g_SelectTownScreen.m_City.Name == city.Name);
	}
}
//----------------------------------------------------------------------------------
void CGumpScreenSelectTown::OnButton(const uint &serial)
{
	if (serial == ID_STS_QUIT) //x button
		g_SelectTownScreen.CreateSmoothAction(CSelectTownScreen::ID_SMOOTH_STS_QUIT);
	else if (serial == ID_STS_ARROW_PREV) //< button
		g_SelectTownScreen.CreateSmoothAction(CSelectTownScreen::ID_SMOOTH_STS_GO_SCREEN_CHARACTER);
	else if (serial == ID_STS_ARROW_NEXT || serial >= ID_STS_TOWN) //> button
		g_SelectTownScreen.CreateSmoothAction(CSelectTownScreen::ID_SMOOTH_STS_GO_SCREEN_GAME_CONNECT);
}
//----------------------------------------------------------------------------------
void CGumpScreenSelectTown::OnTextEntry(const uint &serial)
{
	QFOR(item, m_Items, CBaseGUI*)
	{
		if (item->Type == GOT_TEXTENTRY)
		{
			if (item->Serial == serial)
			{
				CCityItem city = g_CityList.GetCity(serial - ID_STS_TOWN + 1);

				if (g_SelectTownScreen.m_City.Name == city.Name)
					break;

				g_SelectTownScreen.m_City = city;

				wstring description = city.m_City.Description;

				if (city.IsNewCity())
				{
					CCityItemNew *newCity = (CCityItemNew*)&city;

					//!Получаем строку клилока с описанием города
					description = g_ClilocManager.Cliloc(g_Language)->GetW(newCity->Cliloc);
				}

				//!Используем обработку HTML-тэгов при создании текстуры текста
				g_FontManager.SetUseHTML(true);

				if (m_Description != NULL)
					m_Description->CreateTextureW(0, description, 30, 150);

				//!Выключаем обработку HTML-тэгов
				g_FontManager.SetUseHTML(false);

				if (m_HTMLGump != NULL)
				{
					m_HTMLGump->ResetDataOffset();
					m_HTMLGump->CalculateDataSize();
				}

				((CGUITextEntry*)item)->Focused = true;
			}
			else
				((CGUITextEntry*)item)->Focused = false;
		}
	}
}
//----------------------------------------------------------------------------------
