// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
/***********************************************************************************
**
** GumpScreenSelectTown.cpp
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#include "stdafx.h"
//----------------------------------------------------------------------------------
CGumpScreenSelectTown::CGumpScreenSelectTown()
: CGump(GT_NONE, 0, 0, 0)
{
	WISPFUN_DEBUG("c118_f1");
	m_NoMove = true;
	m_NoClose = true;

	//!Список точек для отображения кнопок городов
	m_TownButtonText.push_back(WISP_GEOMETRY::CPoint2Di(105, 130));
	m_TownButtonText.push_back(WISP_GEOMETRY::CPoint2Di(245, 90));
	m_TownButtonText.push_back(WISP_GEOMETRY::CPoint2Di(165, 200));
	m_TownButtonText.push_back(WISP_GEOMETRY::CPoint2Di(395, 160));
	m_TownButtonText.push_back(WISP_GEOMETRY::CPoint2Di(200, 305));
	m_TownButtonText.push_back(WISP_GEOMETRY::CPoint2Di(335, 250));
	m_TownButtonText.push_back(WISP_GEOMETRY::CPoint2Di(160, 395));
	m_TownButtonText.push_back(WISP_GEOMETRY::CPoint2Di(100, 250));
	m_TownButtonText.push_back(WISP_GEOMETRY::CPoint2Di(270, 130));
}
//----------------------------------------------------------------------------------
CGumpScreenSelectTown::~CGumpScreenSelectTown()
{
}
//----------------------------------------------------------------------------------
void CGumpScreenSelectTown::UpdateContent()
{
	WISPFUN_DEBUG("c118_f2");
	Clear();

	CCityItem *city = g_SelectTownScreen.m_City;

	if (city == NULL)
		return;

	wstring description = city->m_City.Description;
	int map = 0;

	if (city->IsNewCity())
	{
		//!Получаем строку клилока с описанием города
		description = g_ClilocManager.Cliloc(g_Language)->GetW(((CCityItemNew*)city)->Cliloc);
		map = ((CCityItemNew*)city)->MapIndex;
	}

	Add(new CGUIGumppicTiled(0x0E14, 0, 0, 640, 480));
	Add(new CGUIGumppic(0x157C, 0, 0));
	Add(new CGUIGumppic(0x15A0, 0, 4));

	if (g_PacketManager.ClientVersion >= CV_70130)
	{
		Add(new CGUIGumppic(0x15D9 + map, 62, 54));
		Add(new CGUIGumppic(0x15DF, 57, 49));

		static const wstring townNames[6] = { L"Felucca", L"Trammel", L"Ilshenar", L"Malas", L"Tokuno", L"Ter Mur" };

		CGUIText *mapName = (CGUIText*)Add(new CGUIText(0x0481, 240, 440));
		mapName->CreateTextureW(0, townNames[map], 24, 0, TS_LEFT, UOFONT_BLACK_BORDER);
	}
	else
		Add(new CGUIGumppic(0x1598, 57, 49));

	Add(new CGUIButton(ID_STS_QUIT, 0x1589, 0x158A, 0x158B, 555, 4));
	Add(new CGUIButton(ID_STS_ARROW_PREV, 0x15A1, 0x15A2, 0x15A3, 586, 445));
	Add(new CGUIButton(ID_STS_ARROW_NEXT, 0x15A4, 0x15A5, 0x15A6, 610, 445));

	m_HTMLGump = (CGUIHTMLGump*)Add(new CGUIHTMLGump(ID_STS_HTML_GUMP, 0x0BB8, 452, 60, 175, 367, true, true));

	//!Используем обработку HTML-тэгов при создании текстуры текста
	g_FontManager.SetUseHTML(true);

	m_Description = (CGUIText*)m_HTMLGump->Add(new CGUIText(0, 3, 3));
	m_Description->CreateTextureW(1, description, 30, 150);

	//!Выключаем обработку HTML-тэгов
	g_FontManager.SetUseHTML(false);

	m_HTMLGump->CalculateDataSize();

	IFOR(i, 0, g_CityList.CityCount())
	{
		if (g_PacketManager.ClientVersion >= CV_70130)
			city = g_CityList.GetCity(i);
		else
			city = g_CityList.GetCity(i + 1);

		if (city == NULL)
			continue;

		int x = 0;
		int y = 0;

		if (city->IsNewCity())
		{
			CCityItemNew *newCity = (CCityItemNew*)city;

			uint map = newCity->MapIndex;

			if (map > 5)
				map = 5;

			x = 62 + CalculatePercents(g_MapSize[map].Width - 2048, newCity->X, 383);
			y = 54 + CalculatePercents(g_MapSize[map].Height, newCity->Y, 384);
		}
		else if (i < (int)m_TownButtonText.size())
		{
			x = m_TownButtonText[i].X;
			y = m_TownButtonText[i].Y;
		}

		Add(new CGUIButton(ID_STS_TOWN + i, 0x04B9, 0x04BA, 0x04BA, x, y));

		y -= 20;

		if (i == 3) //Moonglow
			x -= 60;

		CGUITextEntry *entry = (CGUITextEntry*)Add(new CGUITextEntry(ID_STS_TOWN + i, 0x0058, 0x0099, 0x0481, x, y, 0, false, 3));
		entry->m_Entry.SetText(city->Name);
		entry->CheckOnSerial = true;
		entry->ReadOnly = true;
		entry->Focused = (g_SelectTownScreen.m_City->Name == city->Name);
	}
}
//----------------------------------------------------------------------------------
void CGumpScreenSelectTown::GUMP_BUTTON_EVENT_C
{
	WISPFUN_DEBUG("c118_f3");
	if (serial == ID_STS_QUIT) //x button
		g_SelectTownScreen.CreateSmoothAction(CSelectTownScreen::ID_SMOOTH_STS_QUIT);
	else if (serial == ID_STS_ARROW_PREV) //< button
		g_SelectTownScreen.CreateSmoothAction(CSelectTownScreen::ID_SMOOTH_STS_GO_SCREEN_CHARACTER);
	else if (serial == ID_STS_ARROW_NEXT || serial >= ID_STS_TOWN) //> button
		g_SelectTownScreen.CreateSmoothAction(CSelectTownScreen::ID_SMOOTH_STS_GO_SCREEN_GAME_CONNECT);
}
//----------------------------------------------------------------------------------
void CGumpScreenSelectTown::GUMP_TEXT_ENTRY_EVENT_C
{
	WISPFUN_DEBUG("c118_f4");
	QFOR(item, m_Items, CBaseGUI*)
	{
		if (item->Type == GOT_TEXTENTRY)
		{
			if (item->Serial == serial)
			{
				CCityItem *city = g_CityList.GetCity(serial - ID_STS_TOWN + 1);

				if (city == NULL || g_SelectTownScreen.m_City->Name == city->Name)
					break;

				g_SelectTownScreen.m_City = city;

				wstring description = city->m_City.Description;

				if (city->IsNewCity())
				{
					//!Получаем строку клилока с описанием города
					description = g_ClilocManager.Cliloc(g_Language)->GetW(((CCityItemNew*)city)->Cliloc);
				}

				//!Используем обработку HTML-тэгов при создании текстуры текста
				g_FontManager.SetUseHTML(true);

				if (m_Description != NULL)
					m_Description->CreateTextureW(1, description, 30, 150);

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
