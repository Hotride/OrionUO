/****************************************************************************
**
** GumpTip.cpp
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
//----------------------------------------------------------------------------
#include "stdafx.h"
//----------------------------------------------------------------------------
TGumpTip::TGumpTip(DWORD serial, short x, short y, bool updates)
: TGump(GT_TIP, serial, x, y), m_Height(200), m_HeightBuffer(0), m_CurrentLine(0),
m_LastScrollChangeTime(0), m_Updates(updates)
{
}
//----------------------------------------------------------------------------
TGumpTip::~TGumpTip()
{
	m_Text.Clear();
}
//----------------------------------------------------------------------------
void TGumpTip::PrepareTextures()
{
	UO->ExecuteGumpPart(0x09CA, 4);
	UO->ExecuteGumpPart(0x09D2, 2);
	UO->ExecuteGumpPart(0x0820, 6);
	UO->ExecuteGump(0x082B);
	UO->ExecuteGumpPart(0x082D, 3);
	UO->ExecuteGump(0x001F);
}
//----------------------------------------------------------------------------
void TGumpTip::SetHeight(int val)
{
	//Установка и корректировка значения высоты гампа
	if (val < 120)
		val = 120;

	int buf = GetSystemMetrics(SM_CYSCREEN) - 50;
	if (val >= buf)
		val = (GetSystemMetrics(SM_CYSCREEN) - 50);

	m_Height = val;
}
//----------------------------------------------------------------------------
void TGumpTip::ChangeHeight()
{
	//Событие изменения высоты гампа с вложенной корректировкой
	if (m_HeightBuffer)
		SetHeight(m_Height + (g_MouseY - m_HeightBuffer));

	m_HeightBuffer = 0;
	g_ResizedGump = NULL;
}
//---------------------------------------------------------------------------
void TGumpTip::GenerateFrame()
{
	if (!g_DrawMode)
	{
		m_FrameRedraw = false;
		m_FrameCreated = false;

		return;
	}

	int height = m_Height;

	if (!Minimized && m_HeightBuffer)
	{
		height += (g_MouseY - m_HeightBuffer);

		if (height < 120)
			height = 120;

		int buf = (GetSystemMetrics(SM_CYSCREEN) - 50);
		if (height >= buf)
			height = buf;
	}

	CalculateGumpState();

	int scrollerY = 0;

	int heightToScrolling = 0;

	if (!m_Minimized)
	{
		bool canMoveScroller = true;
		if (m_Text.Height < height - 50)
			canMoveScroller = false;
		else
			heightToScrolling = m_Text.Height - (height - 50);
	
		int maxScrollerY = height - 89;

		int visibleLines = heightToScrolling / GUMP_SCROLLING_PIXEL_STEP;

		if (g_GumpPressedScroller && g_LastObjectLeftMouseDown == ID_GT_SCROLLER && canMoveScroller) //Scroller pressed
		{
			int currentY = (g_MouseY - 10) - (int)g_GumpTranslateY - 72; //Scroller position

			scrollerY = CalculateScrollerAndTextPosition(m_CurrentLine, visibleLines, maxScrollerY, currentY);
		}
		else if (m_CurrentLine && canMoveScroller)
			scrollerY = CalculateScrollerY(m_CurrentLine, visibleLines, maxScrollerY);
	}

	//Создание дисплей листа
	glNewList((GLuint)this, GL_COMPILE);

		if (m_Minimized)
		{
			WORD gumpID = 0x09CB;
			if (m_Updates)
				gumpID = 0x09D3;

			UO->DrawGump(gumpID, 0, 0, 0);

			//Завершим создание листа и выйдем из процедуры
			glEndList();

			m_FrameRedraw = true;
			m_FrameCreated = true;

			return;
		}

		UO->DrawGump(0x082D, 0, 137, 0); //Minimize
		UO->DrawGump(0x0820, 0, 0, 23); //Top scroll

		if (m_Updates) //Updates text gump
			UO->DrawGump(0x09D2, 0, 112, 35);
		else //Tips text gump
			UO->DrawGump(0x09CA, 0, 127, 33);
		
		//Tip window body
		int curPosY = 59;

		while (true)
		{
			int deltaHeight = height - (curPosY - 36);
			if (deltaHeight  < 70)
			{
				if (deltaHeight > 0)
					UO->DrawGump(0x0821, 0, 18, curPosY, 0, deltaHeight);

				break;
			}
			else
				UO->DrawGump(0x0821, 0, 18, curPosY);

			curPosY += 70;

			deltaHeight = height - (curPosY - 36);

			if (deltaHeight < 70)
			{
				if (deltaHeight > 0)
					UO->DrawGump(0x0822, 0, 18, curPosY, 0, deltaHeight);

				break;
			}
			else
				UO->DrawGump(0x0822, 0, 18, curPosY);

			curPosY += 70;
		}
		
		//Up
		//UO->DrawGump(0x001F, 0, PosX + 257, PosY + 72); //Scroller
		//Down
		//UO->DrawGump(0x001F, 0, PosX + 257, PosY + Height - 17); //Scroller

		UO->DrawGump(0x001F, 0, 257, 72 + scrollerY); //Scroller
		
		UO->DrawGump(0x082B, 0, 30, 58); //Top line
		UO->DrawGump(0x0824, 0, 249, 56); //^ button
		UO->DrawGump(0x082B, 0, 31, height + 21); //Bottom line
		UO->DrawGump(0x0825, 0, 249, height + 17); //v button

		g_GL.Scissor((int)g_GumpTranslateX + 36, (int)g_GumpTranslateY + 68, 214, height - 50);

		m_Text.Draw(36, 68 - (m_CurrentLine * GUMP_SCROLLING_PIXEL_STEP));

		glDisable(GL_SCISSOR_TEST);

		UO->DrawGump(0x0823, 0, 18, height + 34); //Bottom scroll

		if (!m_Updates)
		{
			UO->DrawGump(0x09CC, 0, 35, height + 45); //Previous gump

			//UO->DrawGump(0x0823, 0, posX + 18, posY + height + 34); //+/- gump

			UO->DrawGump(0x09CD, 0, 251, height + 45); //Next gump
		}
		
		WORD gumpID = 0x082E + (int)(g_GumpSelectElement == 2);
		UO->DrawGump(gumpID, 0, 137, height + 66); //Resize

	//Завершаем запись дисплей листа
	glEndList();

	m_FrameRedraw = true;
	m_FrameCreated = true;
}
//----------------------------------------------------------------------------
int TGumpTip::Draw(bool &mode)
{
	DWORD index = (DWORD)this;

	int height = m_Height;

	if (!m_Minimized && m_HeightBuffer)
	{
		height += (g_MouseY - m_HeightBuffer);

		if (height < 120)
			height = 120;

		int buf = (GetSystemMetrics(SM_CYSCREEN) - 50);
		if (height >= buf)
			height = buf;
	}

	CalculateGumpState();

	DWORD ticks = GetTickCount();

	//Проверим, вдруг необходимо изменить положение отображаемого элемента
	if (g_GumpPressedScroller && m_LastScrollChangeTime < ticks)
	{
		if (g_LastObjectLeftMouseDown == ID_GT_BUTTON_UP) //Скроллинг вверх (гампом-стрелкой)
			ListingList(true);
		else if (g_LastObjectLeftMouseDown == ID_GT_BUTTON_DOWN) //Скроллинг вниз (гампом-стрелкой)
			ListingList(false);
	}

	int scrollerY = 0;

	int heightToScrolling = 0;

	if (!m_Minimized)
	{
		bool canMoveScroller = true;
		if (m_Text.Height < height - 50)
			canMoveScroller = false;
		else
			heightToScrolling = m_Text.Height - (height - 50);
	
		int maxScrollerY = height - 89;

		int visibleLines = heightToScrolling / GUMP_SCROLLING_PIXEL_STEP;

		if (g_GumpPressedScroller && g_LastObjectLeftMouseDown == ID_GT_SCROLLER && canMoveScroller) //Scroller pressed
		{
			int currentY = (g_MouseY - 10) - (int)g_GumpTranslateY - 72; //Scroller position

			scrollerY = CalculateScrollerAndTextPosition(m_CurrentLine, visibleLines, maxScrollerY, currentY);

			if (mode)
				m_FrameCreated = false;
		}
		else if (m_CurrentLine && canMoveScroller)
			scrollerY = CalculateScrollerY(m_CurrentLine, visibleLines, maxScrollerY);
	}

	if (mode) //Отрисовка
	{
		//Если фрэйм не был создан - создаем
		if (!m_FrameCreated || (g_GumpSelectElement && !m_HeightBuffer) || g_GumpMovingOffsetX || g_GumpMovingOffsetY)
			GenerateFrame();
		else if (m_FrameRedraw)
		{
			GenerateFrame();
			m_FrameRedraw = false;
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

		if (m_Minimized)
		{
			WORD gumpID = 0x09CB;
			if (m_Updates)
				gumpID = 0x09D3;

			if (UO->GumpPixelsInXY(gumpID, 0, 0))
			{
				g_LastSelectedObject = 0;
				g_LastSelectedGump = index;
			}

			g_MouseX = oldMouseX;
			g_MouseY = oldMouseY;

			return 0;
		}
		
		int LSG = 0;
		
		if (UO->GumpPixelsInXY(0x082D, 137, 0)) //Minimize
		{
			LSG = ID_GT_BUTTON_MINIMIZE;
			g_LastSelectedGump = index;
		}
		
		if (UO->GumpPixelsInXY(0x0820, 0, 23)) //Top scroll
		{
			g_LastSelectedObject = 0;
			g_LastSelectedGump = index;
		}

		//Tip window body
		int curPosY = 59;

		while (true)
		{
			int deltaHeight = height - (curPosY - 36);

			if (deltaHeight  < 70)
			{
				if (deltaHeight > 0)
				{
					if (UO->GumpPixelsInXY(0x0821, 18, curPosY, 0, deltaHeight))
					{
						g_LastSelectedObject = 0;
						g_LastSelectedGump = index;
					}
				}

				break;
			}
			else if (UO->GumpPixelsInXY(0x0821, 18, curPosY))
			{
				g_LastSelectedObject = 0;
				g_LastSelectedGump = index;

				break;
			}

			curPosY += 70;

			deltaHeight = height - (curPosY - 36);

			if (deltaHeight < 70)
			{
				if (deltaHeight > 0)
				{
					if (UO->GumpPixelsInXY(0x0822, 18, curPosY, 0, deltaHeight))
					{
						g_LastSelectedObject = 0;
						g_LastSelectedGump = index;
					}
				}

				break;
			}
			else if (UO->GumpPixelsInXY(0x0822, 18, curPosY))
			{
				g_LastSelectedObject = 0;
				g_LastSelectedGump = index;

				break;
			}

			curPosY += 70;
		}

		if (UO->GumpPixelsInXY(0x0823, 18, height + 34)) //Bottom scroll
		{
			g_LastSelectedObject = 0;
			g_LastSelectedGump = index;
		}

		if (UO->GumpPixelsInXY(0x0824, 249, 56)) //^ button
			LSG = ID_GT_BUTTON_UP;
		else if (UO->GumpPixelsInXY(0x0825, 249, height + 17)) //v button
			LSG = ID_GT_BUTTON_DOWN;

		if (!m_Updates)
		{
			if (UO->GumpPixelsInXY(0x09CC, 35, height + 45)) //Previous gump
				LSG = ID_GT_PREV_GUMP;
			else if (UO->GumpPixelsInXY(0x09CD, 251, height + 45)) //Next gump
				LSG = ID_GT_NEXT_GUMP;
		}

		if (UO->GumpPixelsInXY(0x001F, 257, 72 + scrollerY)) //Scroller
			LSG = ID_GT_SCROLLER;
		else if (UO->GumpPixelsInXY(0x082E, 137, height + 66)) //Resize
			LSG = ID_GT_BUTTON_RESIZE;

		g_MouseX = oldMouseX;
		g_MouseY = oldMouseY;

		return LSG;
	}

	return 0;
}
//----------------------------------------------------------------------------
void TGumpTip::OnLeftMouseUp()
{
	if (g_LastObjectLeftMouseDown != g_LastSelectedObject)
		return;

	DWORD ticks = GetTickCount();

	if (g_LastObjectLeftMouseDown == ID_GT_BUTTON_MINIMIZE)
	{
		m_Minimized = true;

		m_FrameCreated = false;
	}
	else if (g_LastObjectLeftMouseDown == ID_GT_BUTTON_RESIZE)
	{
		if (!m_HeightBuffer)
		{
			m_HeightBuffer = g_DroppedLeftMouseY;
			g_ResizedGump = this;
		}
		else
		{
			SetHeight(m_Height + (g_MouseY - m_HeightBuffer));
			m_HeightBuffer = 0;
			g_ResizedGump = NULL;
		}
	}
	else if (g_LastObjectLeftMouseDown == ID_GT_BUTTON_UP && m_LastScrollChangeTime < ticks)
	{
		ListingList(true);
		m_FrameCreated = false;
	}
	else if (g_LastObjectLeftMouseDown == ID_GT_BUTTON_DOWN && m_LastScrollChangeTime < ticks)
	{
		ListingList(false);
		m_FrameCreated = false;
	}
	else if (g_LastObjectLeftMouseDown == ID_GT_PREV_GUMP)
		SendTipRequest(0);
	else if (g_LastObjectLeftMouseDown == ID_GT_NEXT_GUMP)
		SendTipRequest(1);
}
//----------------------------------------------------------------------------
bool TGumpTip::OnLeftMouseDoubleClick()
{
	if (m_Minimized)
	{
		m_Minimized = false;
		m_FrameCreated = false;

		return true;
	}

	return false;
}
//----------------------------------------------------------------------------
void TGumpTip::OnMouseWheel(MOUSE_WHEEL_STATE &state)
{
	if (!m_Minimized && !g_LeftMouseDown && !g_RightMouseDown)
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
void TGumpTip::ListingList(bool direction, int divizor)
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
		int maxidx = (m_Text.Height - (m_Height - 50)) / GUMP_SCROLLING_PIXEL_STEP;

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