/****************************************************************************
**
** GumpBulletinBoard.cpp
**
** Copyright (C) October 2015 Hotride
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
//----------------------------------------------------------------------------
#include "stdafx.h"
//----------------------------------------------------------------------------
TGumpBulletinBoard::TGumpBulletinBoard(DWORD serial, short x, short y, string name)
: TGump(GT_BULLETIN_BOARD, serial, x, y), m_CurrentLine(0), m_LastScrollChangeTime(0)
{
	FontManager->GenerateA(2, m_Text, name.c_str(), 0x0386, 170, TS_CENTER);
}
//----------------------------------------------------------------------------
TGumpBulletinBoard::~TGumpBulletinBoard()
{
	m_Text.Clear();

	GumpManager->CloseGump(0xFFFFFFFF, Serial, GT_BULLETIN_BOARD_ITEM);
}
//----------------------------------------------------------------------------
void TGumpBulletinBoard::PrepareTextures()
{
	UO->ExecuteGump(0x087A);
	UO->ExecuteGump(0x0828);
	UO->ExecuteGump(0x1523);
}
//----------------------------------------------------------------------------
int TGumpBulletinBoard::GetHeight()
{
	return (GetItemsCount() * 18);
}
//---------------------------------------------------------------------------
void TGumpBulletinBoard::GenerateFrame(int posX, int posY)
{
	if (!g_DrawMode)
	{
		FrameRedraw = false;
		FrameCreated = false;

		return;
	}
	
	DWORD index = (DWORD)this;
	
	//Нажата ли кнопка в окне?
	bool IsPressed = (g_LeftMouseDown && g_LastGumpLeftMouseDown == index && g_LastSelectedGump == index);
	
	//Нажата ли кнопка в окне?
	bool IsScrollerPressed = (g_LeftMouseDown && g_LastGumpLeftMouseDown == index);

	//Может ли быть подсвечен элемент?
	int CanSelectedButton = ((g_LastSelectedGump == index) ? g_LastSelectedObject : 0);

	//Может ли быть нажат элемент?
	int CanPressedButton = 0;
	if (IsPressed && g_LastObjectLeftMouseDown == g_LastSelectedObject)
		CanPressedButton = g_LastObjectLeftMouseDown;
	
	//Проверим, вдруг необходимо изменить положение отображаемого элемента
	if (IsPressed && m_LastScrollChangeTime < GetTickCount())
	{
		if (CanPressedButton == ID_GBB_BUTTON_UP) //Скроллинг вверх (гампом-стрелкой)
			ListingList(true, 10);
		else if (CanPressedButton == ID_GBB_BUTTON_DOWN) //Скроллинг вниз (гампом-стрелкой)
			ListingList(false, 10);
	}

	int scrollerY = 0;

	int heightToScrolling = 0;

	bool canMoveScroller = true;

	int curHeight = GetHeight();

	if (curHeight < 164)
		canMoveScroller = false;
	else
		heightToScrolling = curHeight - 164;

	int maxScrollerY = 147;

	int visibleLines = heightToScrolling / GUMP_SCROLLING_PIXEL_STEP;

	if (IsScrollerPressed && g_LastObjectLeftMouseDown == ID_GBB_SCROLLBAR && canMoveScroller) //Scroller pressed
	{
		int currentY = (g_MouseY - 10) - posY - 162; //Scroller position

		scrollerY = CalculateScrollerAndTextPosition(m_CurrentLine, visibleLines, maxScrollerY, currentY);
	}
	else if (m_CurrentLine && canMoveScroller)
		scrollerY = CalculateScrollerY(m_CurrentLine, visibleLines, maxScrollerY);

	glNewList((GLuint)index, GL_COMPILE);

		UO->DrawGump(0x087A, 0, posX, posY);

		m_Text.Draw(posX + 159, posY + 36);
		
		UO->DrawGump(0x0828, 0, posX + 357, posY + 162 + scrollerY);

		TGumpBulletinBoardObject *item = (TGumpBulletinBoardObject*)m_Items;

		int yPtr = posY + 159;
		int xGump = posX + 127;
		int xText = posX + 150;

		g_GL.ViewPort(xGump, yPtr, 230, 330);

		yPtr -= (m_CurrentLine * GUMP_SCROLLING_PIXEL_STEP);

		while (item != NULL)
		{
			UO->DrawGump(0x1523, 0, xGump, yPtr);
			item->Texture.Draw(xText, yPtr + 1);

			yPtr += 18;
			item = (TGumpBulletinBoardObject*)item->m_Next;
		}

		g_GL.RestorePort();

	glEndList();

	FrameRedraw = true;
	FrameCreated = true;
}
//----------------------------------------------------------------------------
int TGumpBulletinBoard::Draw(bool &mode)
{
	DWORD index = (DWORD)this;

	//Для быстрого доступа
	int posX = X;
	int posY = Y;

	//Нажата ли кнопка в окне?
	bool IsPressed = (g_LeftMouseDown && g_LastGumpLeftMouseDown == index && g_LastSelectedGump == index);
	
	//Нажата ли кнопка в окне?
	bool IsScrollerPressed = (g_LeftMouseDown && g_LastGumpLeftMouseDown == index);

	//Может ли быть подсвечен элемент?
	int CanSelectedButton = ((g_LastSelectedGump == index) ? g_LastSelectedObject : 0);

	//Может ли быть нажат элемент?
	int CanPressedButton = 0;
	if (IsPressed && g_LastObjectLeftMouseDown == g_LastSelectedObject)
		CanPressedButton = g_LastObjectLeftMouseDown;
	
	//Проверим, вдруг необходимо изменить положение отображаемого элемента
	if (IsPressed && m_LastScrollChangeTime < GetTickCount())
	{
		if (CanPressedButton == ID_GBB_BUTTON_UP) //Скроллинг вверх (гампом-стрелкой)
			ListingList(true, 10);
		else if (CanPressedButton == ID_GBB_BUTTON_DOWN) //Скроллинг вниз (гампом-стрелкой)
			ListingList(false, 10);
	}

	//Если окошко захвачено для перемещения - вычислим оффсеты
	if (mode && g_LeftMouseDown && g_LastGumpLeftMouseDown == index && !g_LastObjectLeftMouseDown)
	{
		posX += (g_MouseX - g_DroppedLeftMouseX);
		posY += (g_MouseY - g_DroppedLeftMouseY);
	}

	int scrollerY = 0;

	int heightToScrolling = 0;
	bool noRedraw = false;

	bool canMoveScroller = true;

	int curHeight = GetHeight();

	if (curHeight < 164)
		canMoveScroller = false;
	else
		heightToScrolling = curHeight - 164;
	
	int maxScrollerY = 147;

	int visibleLines = heightToScrolling / GUMP_SCROLLING_PIXEL_STEP;

	if (IsScrollerPressed && g_LastObjectLeftMouseDown == ID_GBB_SCROLLBAR && canMoveScroller) //Scroller pressed
	{
		int currentY = (g_MouseY - 10) - posY - 162; //Scroller position

		scrollerY = CalculateScrollerAndTextPosition(m_CurrentLine, visibleLines, maxScrollerY, currentY);

		if (mode)
			GenerateFrame(posX, posY);

		noRedraw = true;
	}
	else if (m_CurrentLine && canMoveScroller)
		scrollerY = CalculateScrollerY(m_CurrentLine, visibleLines, maxScrollerY);

	//Если окошко захвачено для перемещения
	if (mode && g_LeftMouseDown && g_LastGumpLeftMouseDown == index && !g_LastObjectLeftMouseDown)
	{
		if (!noRedraw)
			GenerateFrame(posX, posY);
	}
	else if (mode && !noRedraw && FrameRedraw) //Перерисовка фрэйма, при соблюдении необходимых условий
	{
		GenerateFrame(posX, posY);
		FrameRedraw = false;
	}

	if (mode) //Отрисовка
	{
		//Если фрэйм не был создан - создаем
		if (!FrameCreated)
			GenerateFrame(posX, posY);

		//Отобразим фрэйм
		glCallList((GLuint)index);
	}
	else //Выбор объектов
	{
		int LSG = 0;

		//Если выбран основной гамп - меняем глобальный указатель на выбранный гамп на него
		if (UO->GumpPixelsInXY(0x087A, posX, posY))
		{
			g_LastSelectedObject = 0;
			g_LastSelectedGump = index;
			
			if (UO->PolygonePixelsInXY(posX + 15, posY + 170, 80, 80))
				LSG = ID_GBB_POST_MESSAGE;
			else if (UO->PolygonePixelsInXY(posX + 355, posY + 142, 20, 20))
				LSG = ID_GBB_BUTTON_UP;
			else if (UO->PolygonePixelsInXY(posX + 355, posY + 318, 20, 20))
				LSG = ID_GBB_BUTTON_DOWN;
			else if (UO->PolygonePixelsInXY(posX + 127, posY + 159, 230, 330))
			{
				int currentIndex = 0;

				TBaseQueueItem *item = m_Items;

				int yPtr = posY + 159 - (m_CurrentLine * GUMP_SCROLLING_PIXEL_STEP);
				int xPtr = posX + 127;

				while (item != NULL)
				{
					if (UO->PolygonePixelsInXY(xPtr, yPtr, 230, 18))
						LSG = ID_GBB_MESSAGE + currentIndex;

					currentIndex++;
					yPtr += 18;
					item = (TGumpBulletinBoardObject*)item->m_Next;
				}
			}
		}

		if (LSG != 0)
			g_LastSelectedObject = LSG; //Если что-то нашлось - выбираем

		return LSG;
	}

	return 0;
}
//----------------------------------------------------------------------------
void TGumpBulletinBoard::OnLeftMouseUp()
{
	if (g_LastObjectLeftMouseDown != g_LastSelectedObject)
		return;

	if (g_LastObjectLeftMouseDown == ID_GBB_POST_MESSAGE)
	{
		TGumpBulletinBoardItem *gump = new TGumpBulletinBoardItem(0, 0, 0, 0, Serial, g_Player->GetName(), "", "Date/Time",  "");

		TGump *gumpEntry = GumpManager->GetTextEntryOwner();

		EntryPointer = gump->TextEntrySubject;

		if (gumpEntry != NULL)
			gumpEntry->UpdateFrame();

		GumpManager->AddGump(gump);
	}
	else if (m_LastScrollChangeTime < GetTickCount())
	{
		if (g_LastObjectLeftMouseDown == ID_GBB_BUTTON_UP)
		{
			ListingList(true);
			GenerateFrame(X, Y);
		}
		else if (g_LastObjectLeftMouseDown == ID_GBB_BUTTON_DOWN)
		{
			ListingList(false);
			GenerateFrame(X, Y);
		}
	}
}
//----------------------------------------------------------------------------
bool TGumpBulletinBoard::OnLeftMouseDoubleClick()
{
	if (g_LastObjectLeftMouseDown >= ID_GBB_MESSAGE)
	{
		int index = 0;

		TGumpBulletinBoardObject *item = (TGumpBulletinBoardObject*)m_Items;

		while (item != NULL)
		{
			if (ID_GBB_MESSAGE + index == g_LastObjectLeftMouseDown)
			{
				TPacketBulletinBoardRequestMessage packet(Serial, item->Serial);
				packet.Send();

				return true;
			}

			index++;
			item = (TGumpBulletinBoardObject*)item->m_Next;
		}
	}

	return false;
}
//----------------------------------------------------------------------------
void TGumpBulletinBoard::OnMouseWheel(MOUSE_WHEEL_STATE &state)
{
	if (!g_LeftMouseDown && !g_RightMouseDown)
	{
		DWORD ticks = GetTickCount();

		if (m_LastScrollChangeTime < ticks)
		{
			if (state == MWS_UP)
			{
				ListingList(true, 25);
				GenerateFrame(X, Y);
			}
			else if (state == MWS_DOWN)
			{
				ListingList(false, 25);
				GenerateFrame(X, Y);
			}
		}
	}
}
//----------------------------------------------------------------------------
void TGumpBulletinBoard::ListingList(bool direction, int divizor)
{
	DWORD ticks = GetTickCount();

	if (direction) //Up
	{
		if (m_CurrentLine > 1)
			m_CurrentLine--;
		else
			m_CurrentLine = 0;
	}
	else //Down
	{
		int maxidx = (GetHeight() - 164) / GUMP_SCROLLING_PIXEL_STEP;

		if (maxidx < 0)
			maxidx = 0;

		if (m_CurrentLine < maxidx)
			m_CurrentLine++;
		else
			m_CurrentLine = maxidx;
	}

	m_LastScrollChangeTime = ticks + (SCROLL_LISTING_DELAY / divizor);
}
//----------------------------------------------------------------------------