/****************************************************************************
**
** ServerGump.cpp
**
** Copyright (C) September 2015 Hotride
**
** This program is free software; you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation; either version 2 of the License, or
** (at your option) any later version.
**
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**
*****************************************************************************
*/
//---------------------------------------------------------------------------
#include "stdafx.h"

TServerScreen *ServerScreen = NULL;
//---------------------------------------------------------------------------
TServerScreen::TServerScreen()
: TBaseScreen(), m_LastScrollChangeTime(0), m_PixelOffset(0),
m_SelectionServerTempValue(0)
{
	TCliloc *cliloc = ClilocManager->Cliloc(g_Language);
	
	FontManager->GenerateA(9, m_Text[0], cliloc->GetA(1044579, "Select which shard to play on:").c_str(), 0x0481);
	FontManager->GenerateA(9, m_Text[1], cliloc->GetA(1044580, "Sort by:").c_str(), 0x0481);
}
//---------------------------------------------------------------------------
TServerScreen::~TServerScreen()
{
	IFOR(i, 0, 2)
		m_Text[i].Clear();
}
//---------------------------------------------------------------------------
void TServerScreen::Init()
{
	char buf[128] = {0};
	sprintf(buf, "Ultima Online - %s", MainScreen->m_Account->c_str());

	SetWindowTextA(g_hWnd, buf);

	SmoothMonitor.UseSunrise();
	m_SmoothScreenAction = 0;

	PopupHelp.SeqIndex = 0;

	g_UpdateRange = 18;

	m_LastScrollChangeTime = 0;
	m_PixelOffset = 0;

	//Prepare textures on Main Screen:
	UO->ExecuteGump(0x0588); //Main Screen background
	UO->ExecuteGump(0x157C); //Main Screen
	UO->ExecuteGump(0x15A0); //Main Screen Notes
	UO->ExecuteResizepic(0x13BE); //ActPwd Container
	UO->ExecuteGump(0x058A); //Main Screen Castle?
	UO->ExecuteGumpPart(0x1589, 3); //X gump
	UO->ExecuteGumpPart(0x15A1, 3); //< gump
	UO->ExecuteGumpPart(0x15A4, 3); //> gump
	UO->ExecuteResizepic(0x0DAC); //Server list field
	UO->ExecuteResizepic(0x00FA); //Server list scroll bar:

	UO->ExecuteGumpPart(0x093B, 3); //TimeZone button
	UO->ExecuteGumpPart(0x093E, 3); //Full button
	UO->ExecuteGumpPart(0x0941, 3); //Connection button
	UO->ExecuteGump(0x0589); //Earth container
	UO->ExecuteGumpPart(0x15E8, 3); //Earth button
}
//---------------------------------------------------------------------------
void TServerScreen::InitPopupHelp()
{
	if (!ConfigManager.PopupHelpEnabled)
		return;

	switch (g_LastSelectedObject)
	{
		case ID_SS_QUIT:
		{
			PopupHelp.Set(L"Quit Ultima Online", SOT_NO_OBJECT, g_LastSelectedObject, 80);
			break;
		}
		case ID_SS_ARROW_PREV:
		{
			PopupHelp.Set(L"Back to main menu", SOT_NO_OBJECT, g_LastSelectedObject);
			break;
		}
		case ID_SS_ARROW_NEXT:
		{
			PopupHelp.Set(L"Play UO on selected shard", SOT_NO_OBJECT, g_LastSelectedObject);
			break;
		}
		case ID_SS_EARTH:
		{
			PopupHelp.Set(L"Select last visited shard", SOT_NO_OBJECT, g_LastSelectedObject, 100);
			break;
		}
		default:
			break;
	}

	if (g_LastSelectedObject >= ID_SS_SERVER_LIST)
	{
		string cstr("Connect to '" + ServerList.GetName(g_LastSelectedObject - 8) + "' server");

		PopupHelp.Set(ToWString(cstr), SOT_NO_OBJECT, g_LastSelectedObject, 100);
	}
}
//---------------------------------------------------------------------------
int TServerScreen::GetServerListHeight()
{
	int height = 10;

	IFOR(i, 0, ServerList.Count)
		height += ServerList.TextTexture[i].Height;

	return height;
}
//---------------------------------------------------------------------------
int TServerScreen::Render(bool mode)
{
	DWORD ticks = GetTickCount();

	if (g_LastRenderTime > ticks)
	{
		if (mode || !g_SelectGumpObjects)
			return 0;
	}

	int CanSelectedButton = g_LastSelectedObject;

	int CanPressedButton = 0;
	if (g_LeftMouseDown && g_LastObjectLeftMouseDown == g_LastSelectedObject)
		CanPressedButton = g_LastObjectLeftMouseDown;

	//Проверим, вдруг необходимо изменить положение отображаемого элемента
	if (g_LeftMouseDown && m_LastScrollChangeTime < ticks)
	{
		if (g_LastObjectLeftMouseDown == ID_SS_SCROLLBAR_UP) //Скроллинг вверх (гампом-стрелкой)
			ListingList(true, 10);
		else if (g_LastObjectLeftMouseDown == ID_SS_SCROLLBAR_DOWN) //Скроллинг вниз (гампом-стрелкой)
			ListingList(false, 10);
	}

	int scrollerY = 0;

	int heightToScrolling = 0;
							
	bool canMoveScroller = true;

	int curHeight = GetServerListHeight();

	if (curHeight < 240)
		canMoveScroller = false;
	else
		heightToScrolling = curHeight - 240;
	
	int maxScrollerY = 200;

	int visibleLines = heightToScrolling / GUMP_SCROLLING_PIXEL_STEP;

	if (g_LeftMouseDown && g_LastObjectLeftMouseDown == ID_SS_SCROLLBAR && canMoveScroller) //Scroller pressed
	{
		int currentY = (g_MouseY - 25) - 100; //Scroller position

		scrollerY = CalculateScrollerAndTextPosition(m_PixelOffset, visibleLines, maxScrollerY, currentY);
	}
	else if (m_PixelOffset && canMoveScroller)
		scrollerY = CalculateScrollerY(m_PixelOffset, visibleLines, maxScrollerY);

	if (mode)
	{
		g_LastRenderTime = ticks + (g_FrameDelay[(int)(GetForegroundWindow() == g_hWnd)]);

		g_GL.BeginDraw();

		if (DrawSmoothMonitor())
			return 0;

		glColor3f(1.0f, 1.0f, 1.0f);

		UO->DrawGump(0x0588, 0, 0, 0, 640, 480); //Main Gump background
		UO->DrawGump(0x157C, 0, 0, 0); //Main Gump
		UO->DrawGump(0x15A0, 0, 0, 4); //Main Gump Notes
	
		WORD GumpID = 0x1589 + (int)(CanSelectedButton == ID_SS_QUIT); //X gump /lighted
		if (CanPressedButton == ID_SS_QUIT)
			GumpID = 0x158B; //X gump (pressed)
		UO->DrawGump(GumpID, 0, 555, 4);
		
		GumpID = 0x15A1 + (int)(CanSelectedButton == ID_SS_ARROW_PREV); //< gump /lighted
		if (CanPressedButton == ID_SS_ARROW_PREV)
			GumpID = 0x15A3; //< gump pressed
		UO->DrawGump(GumpID, 0, 586, 445); //< gump

		GumpID = 0x15A4 + (int)(CanSelectedButton == ID_SS_ARROW_NEXT); //> gump /lighted
		if (CanPressedButton == ID_SS_ARROW_NEXT)
			GumpID = 0x15A6; //> gump pressed
		UO->DrawGump(GumpID, 0, 610, 445); //> gump
		
		m_Text[0].Draw(233, 62);

		UO->DrawResizepicGump(0x0DAC, 150, 90, 380, 271); //Server list field
		
		//Server list scroll bar:
		UO->DrawGump(0x0100, 0, 530, 111, 0, 240); //background
		UO->DrawGump(0x00FA, 0, 530, 91); //^
		UO->DrawGump(0x00FE, 0, 531, 112 + scrollerY); //bar
		UO->DrawGump(0x00FC, 0, 530, 338); //v

		g_GL.ViewPort(170, 100, 346, 250);

		int ofsY = -(m_PixelOffset * GUMP_SCROLLING_PIXEL_STEP);
		
		IFOR(i, 0, ServerList.Count)
		{
			if (g_LastSelectedObject == (ID_SS_SERVER_LIST + i))
				FontManager->DrawA(5, ServerList.GetName(i).c_str(), 0x0021, 224, 100 + ofsY);
			else
				ServerList.TextTexture[i].Draw(224, 100 + ofsY);

			ofsY += 25;

			//FontManager->DrawA(5, ServerList.GetName(i).c_str(), FColor, 224, 100 + (i * 25));
		}

		g_GL.RestorePort();
		
		m_Text[1].Draw(153, 368);
		
		GumpID = 0x093B + (int)(CanSelectedButton == ID_SS_TIME_ZONE ? 2 : 0); //TimeZone button /lighted
		if (CanPressedButton == ID_SS_TIME_ZONE)
			GumpID = 0x093C; //TimeZone button pressed
		UO->DrawGump(GumpID, 0, 230, 366); //TimeZone button

		GumpID = 0x093E + (int)(CanSelectedButton == ID_SS_FULL ? 2 : 0);  //Full button /lighted
		if (CanPressedButton == ID_SS_FULL)
			GumpID = 0x093F; //Full button pressed
		UO->DrawGump(GumpID, 0, 338, 366); //Full button

		GumpID = 0x0941 + (int)(CanSelectedButton == ID_SS_CONNECTION ? 2 : 0); //Connection button /lighted
		if (CanPressedButton == ID_SS_CONNECTION)
			GumpID = 0x0942; //Connection button pressed
		UO->DrawGump(GumpID, 0, 446, 366); //Connection button

		UO->DrawGump(0x0589, 0, 150, 390); //Earth container

		GumpID = 0x15E8 + (int)(CanSelectedButton == ID_SS_EARTH); //Earth button /lighted
		if (CanPressedButton == ID_SS_EARTH)
			GumpID = 0x15EA; //Earth button pressed
		UO->DrawGump(GumpID, 0, 160, 400); //Earth button
		
		if (ServerList.Count > 0)
			FontManager->DrawA(9, ServerList.GetName(0).c_str(), 0x0481, 243, 420);
		
		InitPopupHelp();

		DrawSmoothMonitorEffect();

		MouseManager.Draw(0x2073); //Main Gump mouse cursor

		g_GL.EndDraw();
	}
	else
	{
		g_LastSelectedObject = 0;

		if (UO->GumpPixelsInXY(0x1589, 555, 4))
			g_LastSelectedObject = ID_SS_QUIT; //X gump
		else if (UO->GumpPixelsInXY(0x15A1, 586, 445))
			g_LastSelectedObject = ID_SS_ARROW_PREV; //< gump
		else if (UO->GumpPixelsInXY(0x15A4, 610, 445))
			g_LastSelectedObject = ID_SS_ARROW_NEXT; //> gump
		else if (UO->GumpPixelsInXY(0x093B, 230, 366))
			g_LastSelectedObject = ID_SS_TIME_ZONE; //TimeZone button
		else if (UO->GumpPixelsInXY(0x093E, 338, 366))
			g_LastSelectedObject = ID_SS_FULL; //Full button
		else if (UO->GumpPixelsInXY(0x0941, 446, 366))
			g_LastSelectedObject = ID_SS_CONNECTION; //Connection button
		else if (UO->GumpPixelsInXY(0x15E8, 160, 400))
			g_LastSelectedObject = ID_SS_EARTH; //Earth button
		else if (UO->GumpPixelsInXY(0x00FA, 530, 91))
			g_LastSelectedObject = ID_SS_SCROLLBAR_UP; //^
		else if (UO->GumpPixelsInXY(0x00FC, 530, 338))
			g_LastSelectedObject = ID_SS_SCROLLBAR_DOWN; //v
		else if (UO->GumpPixelsInXY(0x00FE, 531, 112 + scrollerY))
			g_LastSelectedObject = ID_SS_SCROLLBAR; //bar
		else if (UO->GumpPixelsInXY(0x0100, 530, 111, 0, 240))
			g_LastSelectedObject = ID_SS_SCROLLBAR_BACKGROUND; //background
		else if (g_MouseX >= 170 && g_MouseX < 516 && g_MouseY >= 100 && g_MouseY < 350)
		{
			int ofsY = -(m_PixelOffset * GUMP_SCROLLING_PIXEL_STEP);

			IFOR(i, 0, ServerList.Count)
			{
				RECT rc = {0, 0, 300, 25};
				POINT p = {g_MouseX - 224, g_MouseY - (100 + ofsY)};

				ofsY += 25;

				if (PtInRect(&rc, p))
				{
					g_LastSelectedObject = (ID_SS_SERVER_LIST + i);
					break;
				}
			}
		}

		return g_LastSelectedObject;
	}

	return 0;
}
//---------------------------------------------------------------------------
void TServerScreen::OnLeftMouseDown()
{
	Render(false);

	g_LeftMouseDown = true;
	g_LastObjectLeftMouseDown = g_LastSelectedObject;

	if (g_LastSelectedObject == ID_SS_SCROLLBAR_BACKGROUND) //Scrollbar background click
	{
		int drawY = 100;

		int heightToScrolling = GetServerListHeight() - 240;

		if (heightToScrolling > 0)
		{
			int maxScrollerY = 200;

			int visibleLines = heightToScrolling / GUMP_SCROLLING_PIXEL_STEP;

			int jsize = visibleLines;

			float ValPer = (m_PixelOffset / (float)jsize) * 100.0f;

			jsize = (int)((jsize * ValPer) / 100.0f);

			if (jsize < 1)
				jsize = 1;
			else if (jsize > visibleLines)
				jsize = visibleLines;
			
			if (jsize >= visibleLines)
				drawY += maxScrollerY;
			else
				drawY += (int)((maxScrollerY * ValPer) / 100.0f);
		}

		if (g_MouseY < drawY) //^
			g_LastSelectedObject = ID_SS_SCROLLBAR_UP;
		else if (g_MouseY > drawY + 25) //v
			g_LastSelectedObject = ID_SS_SCROLLBAR_DOWN;
	}

	if (g_LastObjectLeftMouseDown >= ID_SS_SERVER_LIST) //Server selection
	{
		m_SelectionServerTempValue = g_LastObjectLeftMouseDown - ID_SS_SERVER_LIST;
		CreateSmoothAction(ID_SMOOTH_SS_SELECT_SERVER);
	}
}
//---------------------------------------------------------------------------
void TServerScreen::OnLeftMouseUp()
{
	Render(false);

	g_LeftMouseDown = false;

	if (g_LastSelectedObject == 0 || g_LastSelectedObject != g_LastObjectLeftMouseDown)
	{
		g_LastObjectLeftMouseDown = 0;
		return;
	}

	if (g_LastObjectLeftMouseDown == ID_SS_QUIT) //x button
		CreateSmoothAction(ID_SMOOTH_SS_QUIT);
	else if (g_LastObjectLeftMouseDown == ID_SS_ARROW_PREV) //< button
		CreateSmoothAction(ID_SMOOTH_SS_GO_SCREEN_MAIN);
	else if (g_LastObjectLeftMouseDown == ID_SS_ARROW_NEXT || g_LastObjectLeftMouseDown == ID_SS_EARTH) //> button
	{
		m_SelectionServerTempValue = 0;
		CreateSmoothAction(ID_SMOOTH_SS_SELECT_SERVER);
	}
	else if (m_LastScrollChangeTime < GetTickCount())
	{
		if (g_LastObjectLeftMouseDown == ID_SS_SCROLLBAR_UP)
			ListingList(true);
		else if (g_LastObjectLeftMouseDown == ID_SS_SCROLLBAR_DOWN)
			ListingList(false);
	}

	g_LastObjectLeftMouseDown = 0;
}
//----------------------------------------------------------------------------
void TServerScreen::OnMouseWheel(MOUSE_WHEEL_STATE state)
{
	if (!g_LeftMouseDown && !g_RightMouseDown && m_LastScrollChangeTime < GetTickCount())
	{
		if (g_MouseX >= 170 && g_MouseX < 516 && g_MouseY >= 100 && g_MouseY < 350)
		{
			if (state == MWS_UP)
				ListingList(true, 25);
			else if (state == MWS_DOWN)
				ListingList(false, 25);
		}
	}
}
//---------------------------------------------------------------------------
void TServerScreen::OnKeyPress(WPARAM wparam, LPARAM lparam)
{
	if (wparam == VK_RETURN)
	{
		m_SelectionServerTempValue = 0;
		CreateSmoothAction(ID_SMOOTH_SS_SELECT_SERVER);
	}
}
//---------------------------------------------------------------------------
void TServerScreen::ProcessSmoothAction(BYTE action)
{
	if (action == 0xFF)
		action = m_SmoothScreenAction;

	if (action == ID_SMOOTH_SS_SELECT_SERVER)
		UO->ServerSelection(m_SelectionServerTempValue);
	else if (action == ID_SMOOTH_SS_QUIT)
		PostMessage(g_hWnd, WM_CLOSE, 0, 0);
	else if (action == ID_SMOOTH_SS_GO_SCREEN_MAIN)
	{
		UO->Disconnect();
		UO->InitScreen(GS_MAIN);
	}
}
//----------------------------------------------------------------------------
void TServerScreen::ListingList(bool direction, int divizor)
{
	if (direction) //Up
	{
		if (m_PixelOffset > 1)
			m_PixelOffset--;
		else
			m_PixelOffset = 0;
	}
	else //Down
	{
		int maxidx = (GetServerListHeight() - 240) / GUMP_SCROLLING_PIXEL_STEP;

		if (maxidx < 0)
			maxidx = 0;

		if (m_PixelOffset < maxidx)
			m_PixelOffset++;
		else
			m_PixelOffset = maxidx;
	}

	m_LastScrollChangeTime = GetTickCount() + (SCROLL_LISTING_DELAY / divizor);
}
//---------------------------------------------------------------------------