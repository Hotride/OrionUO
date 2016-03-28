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

	GumpManager->CloseGump(0xFFFFFFFF, m_Serial, GT_BULLETIN_BOARD_ITEM);
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
void TGumpBulletinBoard::GenerateFrame()
{
	if (!g_DrawMode)
	{
		m_FrameRedraw = false;
		m_FrameCreated = false;

		return;
	}

	CalculateGumpState();

	//Проверим, вдруг необходимо изменить положение отображаемого элемента
	if (g_GumpPressed && m_LastScrollChangeTime < GetTickCount())
	{
		if (g_GumpPressedElement == ID_GBB_BUTTON_UP) //Скроллинг вверх (гампом-стрелкой)
			ListingList(true, 10);
		else if (g_GumpPressedElement == ID_GBB_BUTTON_DOWN) //Скроллинг вниз (гампом-стрелкой)
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

	if (g_GumpPressedScroller && g_LastObjectLeftMouseDown == ID_GBB_SCROLLBAR && canMoveScroller) //Scroller pressed
	{
		int currentY = (g_MouseY - 10) - (int)g_GumpTranslateY - 162; //Scroller position

		scrollerY = CalculateScrollerAndTextPosition(m_CurrentLine, visibleLines, maxScrollerY, currentY);
	}
	else if (m_CurrentLine && canMoveScroller)
		scrollerY = CalculateScrollerY(m_CurrentLine, visibleLines, maxScrollerY);

	glNewList((GLuint)this, GL_COMPILE);

		UO->DrawGump(0x087A, 0, 0, 0);

		m_Text.Draw(159, 36);
		
		UO->DrawGump(0x0828, 0, 357, 162 + scrollerY);

		TGumpBulletinBoardObject *item = (TGumpBulletinBoardObject*)m_Items;

		int yPtr = 159;

		g_GL.Scissor((int)g_GumpTranslateX + 127, (int)g_GumpTranslateY + yPtr, 230, 330);

		yPtr -= (m_CurrentLine * GUMP_SCROLLING_PIXEL_STEP);

		while (item != NULL)
		{
			UO->DrawGump(0x1523, 0, 127, yPtr);
			item->Texture.Draw(150, yPtr + 1);

			yPtr += 18;
			item = (TGumpBulletinBoardObject*)item->m_Next;
		}

		glDisable(GL_SCISSOR_TEST);

	glEndList();

	FrameRedraw = true;
	FrameCreated = true;
}
//----------------------------------------------------------------------------
int TGumpBulletinBoard::Draw(bool &mode)
{
	DWORD index = (DWORD)this;

	CalculateGumpState();

	//Проверим, вдруг необходимо изменить положение отображаемого элемента
	if (g_GumpPressed && m_LastScrollChangeTime < GetTickCount())
	{
		if (g_GumpPressedElement == ID_GBB_BUTTON_UP) //Скроллинг вверх (гампом-стрелкой)
			ListingList(true, 10);
		else if (g_GumpPressedElement == ID_GBB_BUTTON_DOWN) //Скроллинг вниз (гампом-стрелкой)
			ListingList(false, 10);
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

	if (g_GumpPressedScroller && g_LastObjectLeftMouseDown == ID_GBB_SCROLLBAR && canMoveScroller) //Scroller pressed
	{
		int currentY = (g_MouseY - 10) - (int)g_GumpTranslateY - 162; //Scroller position

		scrollerY = CalculateScrollerAndTextPosition(m_CurrentLine, visibleLines, maxScrollerY, currentY);

		if (mode)
			m_FrameCreated = false;
	}
	else if (m_CurrentLine && canMoveScroller)
		scrollerY = CalculateScrollerY(m_CurrentLine, visibleLines, maxScrollerY);

	if (mode) //Отрисовка
	{
		//Если фрэйм не был создан - создаем
		if (!m_FrameCreated || g_GumpMovingOffsetX || g_GumpMovingOffsetY)
			GenerateFrame();
		if (m_FrameRedraw)
		{
			GenerateFrame();
			FrameRedraw = false;
		}

		glTranslatef(g_GumpTranslateX, g_GumpTranslateY, 0.0f);

		//Отобразим фрэйм
		glCallList((GLuint)index);

		glTranslatef(-g_GumpTranslateX, -g_GumpTranslateY, 0.0f);
	}
	else //Выбор объектов
	{
		int oldMouseX = g_MouseX;
		int oldMouseY = g_MouseY;
		g_MouseX -= (int)g_GumpTranslateX;
		g_MouseY -= (int)g_GumpTranslateY;

		int LSG = 0;

		//Если выбран основной гамп - меняем глобальный указатель на выбранный гамп на него
		if (UO->GumpPixelsInXY(0x087A, 0, 0))
		{
			g_LastSelectedObject = 0;
			g_LastSelectedGump = index;
			
			if (UO->PolygonePixelsInXY(15, 170, 80, 80))
				LSG = ID_GBB_POST_MESSAGE;
			else if (UO->PolygonePixelsInXY(355, 142, 20, 20))
				LSG = ID_GBB_BUTTON_UP;
			else if (UO->PolygonePixelsInXY(355, 318, 20, 20))
				LSG = ID_GBB_BUTTON_DOWN;
			else if (UO->PolygonePixelsInXY(127, 159, 230, 330))
			{
				int currentIndex = 0;

				TBaseQueueItem *item = m_Items;

				int yPtr = 159 - (m_CurrentLine * GUMP_SCROLLING_PIXEL_STEP);

				while (item != NULL)
				{
					if (UO->PolygonePixelsInXY(127, yPtr, 230, 18))
						LSG = ID_GBB_MESSAGE + currentIndex;

					currentIndex++;
					yPtr += 18;
					item = (TGumpBulletinBoardObject*)item->m_Next;
				}
			}
		}

		g_MouseX = oldMouseX;
		g_MouseY = oldMouseY;

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
		TGumpBulletinBoardItem *gump = new TGumpBulletinBoardItem(0, 0, 0, 0, m_Serial, g_Player->GetName(), "", "Date/Time", "");

		TGump *gumpEntry = GumpManager->GetTextEntryOwner();

		EntryPointer = gump->TextEntrySubject;

		if (gumpEntry != NULL)
			gumpEntry->FrameCreated = false;

		GumpManager->AddGump(gump);
	}
	else if (m_LastScrollChangeTime < GetTickCount())
	{
		if (g_LastObjectLeftMouseDown == ID_GBB_BUTTON_UP)
		{
			ListingList(true);
			m_FrameCreated = false;
		}
		else if (g_LastObjectLeftMouseDown == ID_GBB_BUTTON_DOWN)
		{
			ListingList(false);
			m_FrameCreated = false;
		}
	}
}
//----------------------------------------------------------------------------
bool TGumpBulletinBoard::OnLeftMouseDoubleClick()
{
	bool result = false;

	if (g_LastObjectLeftMouseDown >= ID_GBB_MESSAGE)
	{
		int index = ID_GBB_MESSAGE;

		for (TGumpBulletinBoardObject *item = (TGumpBulletinBoardObject*)m_Items; item != NULL; item = (TGumpBulletinBoardObject*)item->m_Next, index++)
		{
			if (index == g_LastObjectLeftMouseDown)
			{
				TPacketBulletinBoardRequestMessage packet(m_Serial, item->Serial);
				packet.Send();

				result = true;
			}
		}
	}

	return result;
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
				m_FrameCreated = false;
			}
			else if (state == MWS_DOWN)
			{
				ListingList(false, 25);
				m_FrameCreated = false;
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