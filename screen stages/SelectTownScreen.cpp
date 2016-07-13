/****************************************************************************
**
** CreateCharacterSelectionTownGump.cpp
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

TSelectTownScreen *SelectTownScreen = NULL;
//---------------------------------------------------------------------------
//!Список точек для отображения кнопок городов
POINT TSelectTownScreen::m_TownButtonText[9] =
{
	//Button     Text
	{105, 130}, //Yew
	{245, 90}, //Minoc
	{165, 200}, //Britain
	{395, 160}, //Moonglow
	{200, 305}, //Trinsic
	{335, 250}, //Magincia
	{160, 395}, //Jhelom
	{100, 250}, //Skara Brae
	{270, 130} //Vesper
};
//---------------------------------------------------------------------------
TSelectTownScreen::TSelectTownScreen()
: TBaseScreen(), m_PixelOffset(0), m_LastScrollChangeTime(0), m_City(NULL)
{
}
//---------------------------------------------------------------------------
TSelectTownScreen::~TSelectTownScreen()
{
}
//---------------------------------------------------------------------------
/*!
Инициализация
@return 
*/
void TSelectTownScreen::Init()
{
	m_PixelOffset = 0;
	m_LastScrollChangeTime = 0;
	m_City = CityList->GetCity(3);

	SmoothMonitor.UseSunrise();
	m_SmoothScreenAction = 0;

	ToolTip.SeqIndex = 0;
	
	//Prepare textures:
	Orion->ExecuteGump(0x0588); //Main Screen background
	Orion->ExecuteGump(0x157C); //Main Screen
	Orion->ExecuteGump(0x15A0); //Main Screen Notes
	Orion->ExecuteGumpPart(0x1589, 3); //X gump
	Orion->ExecuteGumpPart(0x15A1, 3); //< gump
	Orion->ExecuteGumpPart(0x15A4, 3); //> gump
	Orion->ExecuteResizepic(0x0A28); //scroll bar field
	Orion->ExecuteResizepic(0x00FA); //scroll bar
}
//---------------------------------------------------------------------------
/*!
Обработка события после плавного затемнения экрана
@param [__in_opt] action Идентификатор действия
@return 
*/
void TSelectTownScreen::ProcessSmoothAction( __in_opt BYTE action)
{
	if (action == 0xFF)
		action = m_SmoothScreenAction;

	if (action == ID_SMOOTH_STS_QUIT)
		PostMessage(g_hWnd, WM_CLOSE, 0, 0);
	else if (action == ID_SMOOTH_STS_GO_SCREEN_CHARACTER)
		Orion->InitScreen(GS_CHARACTER);
	else if (action == ID_SMOOTH_STS_GO_SCREEN_GAME_CONNECT)
	{
		TPacketCreateCharacter packet(string(CreateCharacterManager.m_Name->c_str()));
		packet.Send();
		Orion->InitScreen(GS_GAME_CONNECT);
	}
}
//---------------------------------------------------------------------------
/*!
Инициализация всплывающих подсказок
@return 
*/
void TSelectTownScreen::InitToolTip()
{
	if (!ConfigManager.UseToolTips)
		return;

}
//---------------------------------------------------------------------------
/*!
Отобразить информацию о городе
@param [__in] x Координата X
@param [__in] y Координата Y
@return 
*/
void TSelectTownScreen::DrawTownInfo( __in int x, __in int y)
{
	if (m_City != NULL)
	{
		if (m_City->IsNewCity())
		{
			TCityItemNew *newCity = (TCityItemNew*)m_City;
			newCity->m_Texture.Draw(x, y);
		}
		else
		{
			TCity *city = m_City->m_City;

			if (city != NULL)
				city->m_Texture.Draw(x, y);
		}
	}
}
//---------------------------------------------------------------------------
/*!
Отрисовка/выбор объектов
@param [__in] mode true - отрисовка, false - выбор
@return При выборе объектов - идентификатор выбранного объекта
*/
int TSelectTownScreen::Render(__in bool mode)
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
		if (g_LastObjectLeftMouseDown == ID_STS_SCROLLBAR_UP) //Скроллинг вверх (гампом-стрелкой)
			ListingList(true, 10);
		else if (g_LastObjectLeftMouseDown == ID_STS_SCROLLBAR_DOWN) //Скроллинг вниз (гампом-стрелкой)
			ListingList(false, 10);
	}

	int scrollerY = 0;

	int heightToScrolling = 0;

	bool canMoveScroller = true;

	int curHeight = GetScrollBoxHeight();

	if (curHeight < 350)
		canMoveScroller = false;
	else
		heightToScrolling = curHeight - 350;
	
	int maxScrollerY = 300;

	int visibleLines = heightToScrolling / GUMP_SCROLLING_PIXEL_STEP;

	if (g_LeftMouseDown && g_LastObjectLeftMouseDown == ID_STS_SCROLLBAR && canMoveScroller) //Scroller pressed
	{
		int currentY = (g_MouseY - 25) - 69; //Scroller position

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

		Orion->DrawGump(0x0588, 0, 0, 0, 640, 480); //Main Gump background

		Orion->DrawGump(0x1598, 0, 57, 49); //Map gump
	
		Orion->DrawGump(0x157C, 0, 0, 0); //Main Gump
		Orion->DrawGump(0x15A0, 0, 0, 4); //Main Gump Notes

		WORD GumpID = 0x1589 + (int)(CanSelectedButton == 1); //X gump /lighted
		if (CanPressedButton == 1)
			GumpID = 0x158B; //X gump (pressed)
		Orion->DrawGump(GumpID, 0, 555, 4);
		
		GumpID = 0x15A1 + (int)(CanSelectedButton == 2); //< gump /lighted
		if (CanPressedButton == 2)
			GumpID = 0x15A3; //< gump pressed
		Orion->DrawGump(GumpID, 0, 586, 445); //< gump

		GumpID = 0x15A4 + (int)(CanSelectedButton == 3); //> gump /lighted
		if (CanPressedButton == 3)
			GumpID = 0x15A6; //> gump pressed
		Orion->DrawGump(GumpID, 0, 610, 445); //> gump

		Orion->DrawResizepicGump(0xBB8, 452, 60, 158, 367); //Description text field

		//Description scroll bar:
		Orion->DrawGump(0x0100, 0, 610, 66, 0, 355); //background
		Orion->DrawGump(0x00FA, 0, 610, 60); //^
		Orion->DrawGump(0x00FE, 0, 611, 81 + scrollerY); //bar
		Orion->DrawGump(0x00FC, 0, 610, 406); //v

		g_GL.ViewPort(452, 60, 158, 367);

		int yPtr = 62 - (m_PixelOffset * GUMP_SCROLLING_PIXEL_STEP);
		DrawTownInfo(455, yPtr);

		g_GL.RestorePort();

		IFOR(i, 0, 9)
		{
			TCityItem *city = CityList->GetCity(i + 1);

			if (city != NULL)
			{
				int bx = m_TownButtonText[i].x;
				int by = m_TownButtonText[i].y;
				
				WORD textColor = 0x0058;
				WORD gumpID = 0x04B9;

				if (city == m_City)
					textColor = 0x0481;
				else if (CanSelectedButton == ID_STS_TOWN + i)
				{
					gumpID++;
					textColor = 0x0099;
				}

				Orion->DrawGump(gumpID, 0, bx, by);

				by -= 20;

				if (i == 3) //Moonglow
					bx -= 60;

				FontManager->DrawA(3, city->GetName().c_str(), textColor, bx, by);
			}
		}
		
		InitToolTip();

		DrawSmoothMonitorEffect();

		MouseManager.Draw(0x2073); //Main Gump mouse cursor
		
		g_GL.EndDraw();
	}
	else
	{
		g_LastSelectedObject = 0;
		
		if (Orion->GumpPixelsInXY(0x1589, 555, 4))
			g_LastSelectedObject = ID_STS_QUIT; //X gump
		else if (Orion->GumpPixelsInXY(0x15A1, 586, 445))
			g_LastSelectedObject = ID_STS_ARROW_PREV; //< gump
		else if (Orion->GumpPixelsInXY(0x15A4, 610, 445))
			g_LastSelectedObject = ID_STS_ARROW_NEXT; //> gump
		else if (Orion->GumpPixelsInXY(0x00FA, 610, 60))
			g_LastSelectedObject = ID_STS_SCROLLBAR_UP; //^
		else if (Orion->GumpPixelsInXY(0x00FC, 610, 406))
			g_LastSelectedObject = ID_STS_SCROLLBAR_DOWN; //v
		else if (Orion->GumpPixelsInXY(0x00FE, 611, 81 + scrollerY))
			g_LastSelectedObject = ID_STS_SCROLLBAR; //bar
		else if (Orion->GumpPixelsInXY(0x0100, 610, 66, 0, 355))
			g_LastSelectedObject = ID_STS_SCROLLBAR_BACKGROUND; //background
		else
		{
			IFOR(i, 0, 9)
			{
				TCityItem *city = CityList->GetCity(i + 1);

				if (city != NULL)
				{
					int bx = m_TownButtonText[i].x;
					int by = m_TownButtonText[i].y;

					if (Orion->GumpPixelsInXY(0x0837, bx, by))
						g_LastSelectedObject = ID_STS_TOWN + i;
					else
					{
						by -= 20;

						if (i == 3) //Moonglow
							bx -= 60;

						int tw = FontManager->GetWidthA(3, city->GetName().c_str());
						
						if (Orion->PolygonePixelsInXY(bx, by, tw, 16))
							g_LastSelectedObject = ID_STS_TOWN + i;
					}
				}
			}
		}

		return g_LastSelectedObject;
	}

	return 0;
}
//---------------------------------------------------------------------------
/*!
Нажатие левой кнопки мыши
@return 
*/
void TSelectTownScreen::OnLeftMouseDown()
{
	Render(false);

	g_LeftMouseDown = true;
	g_LastObjectLeftMouseDown = g_LastSelectedObject;
	
	if (g_LastSelectedObject == ID_STS_SCROLLBAR_BACKGROUND) //Scrollbar background click
	{
		int drawY = 69;

		int heightToScrolling = GetScrollBoxHeight() - 350;

		if (heightToScrolling > 0)
		{
			int maxScrollerY = 300;

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
			g_LastSelectedObject = ID_STS_SCROLLBAR_UP;
		else if (g_MouseY > drawY + 25) //v
			g_LastSelectedObject = ID_STS_SCROLLBAR_DOWN;
	}
	
	if (g_LastSelectedObject >= ID_STS_TOWN)
	{
		IFOR(i, 0, 9)
		{
			TCityItem *city = CityList->GetCity(i + 1);

			if (city != NULL)
			{
				if (g_LastSelectedObject == ID_STS_TOWN + i && m_City != city)
				{
					m_PixelOffset = 0;
					m_City = city;

					break;
				}
			}
		}
	}
}
//---------------------------------------------------------------------------
/*!
Отпускание левой кнопки мыши
@return 
*/
void TSelectTownScreen::OnLeftMouseUp()
{
	Render(false);

	g_LeftMouseDown = false;

	if (g_LastSelectedObject == 0 || g_LastSelectedObject != g_LastObjectLeftMouseDown)
	{
		g_LastObjectLeftMouseDown = 0;
		return;
	}

	if (g_LastObjectLeftMouseDown == ID_STS_QUIT) //x button
		CreateSmoothAction(ID_SMOOTH_STS_QUIT);
	else if (g_LastObjectLeftMouseDown == ID_STS_ARROW_PREV) //< button
		CreateSmoothAction(ID_SMOOTH_STS_GO_SCREEN_CHARACTER);
	else if (g_LastObjectLeftMouseDown == ID_STS_ARROW_NEXT) //> button
		CreateSmoothAction(ID_SMOOTH_STS_GO_SCREEN_GAME_CONNECT);

	g_LastObjectLeftMouseDown = 0;
}
//----------------------------------------------------------------------------
/*!
Обработка средней кнопки (колесика) мыши
@param [__in] state Состояние колесика
@return 
*/
void TSelectTownScreen::OnMouseWheel( __in MOUSE_WHEEL_STATE state)
{
	if (!g_LeftMouseDown && !g_RightMouseDown && m_LastScrollChangeTime < GetTickCount())
	{
		if (g_MouseX >= 452 && g_MouseX < 610 && g_MouseY >= 60 && g_MouseY < 427)
		{
			if (state == MWS_UP)
				ListingList(true, 25);
			else if (state == MWS_DOWN)
				ListingList(false, 25);
		}
	}
}
//----------------------------------------------------------------------------
/*!
Получить высоту скроллбокса
@return Высота скроллбокса
*/
int TSelectTownScreen::GetScrollBoxHeight()
{
	int result = 10;

	if (m_City != NULL)
	{
		if (m_City->IsNewCity())
		{
			TCityItemNew *newCity = (TCityItemNew*)m_City;
			result += newCity->m_Texture.Height;
		}
		else
		{
			TCity *city = m_City->m_City;

			if (city != NULL)
				result += city->m_Texture.Height;
		}
	}

	return result;
}
//----------------------------------------------------------------------------
/*!
Пролистать список
@param [__in] direction Направление
@param [__in] divizor Делитель
@return 
*/
void TSelectTownScreen::ListingList( __in bool direction, __in int divizor)
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
		int maxidx = (GetScrollBoxHeight() - 350) / GUMP_SCROLLING_PIXEL_STEP;

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