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
void TGumpTip::GenerateFrame(int posX, int posY)
{
	if (!g_DrawMode)
	{
		FrameRedraw = false;
		FrameCreated = false;

		return;
	}

	DWORD index = (DWORD)this;

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

	int scrollerY = 0;

	int heightToScrolling = 0;

	if (!Minimized)
	{
		bool canMoveScroller = true;
		if (m_Text.Height < height - 50)
			canMoveScroller = false;
		else
			heightToScrolling = m_Text.Height - (height - 50);
	
		int maxScrollerY = height - 89;

		int visibleLines = heightToScrolling / GUMP_SCROLLING_PIXEL_STEP;

		if (IsScrollerPressed && g_LastObjectLeftMouseDown == ID_GT_SCROLLER && canMoveScroller) //Scroller pressed
		{
			int currentY = (g_MouseY - 10) - posY - 72; //Scroller position

			scrollerY = CalculateScrollerAndTextPosition(m_CurrentLine, visibleLines, maxScrollerY, currentY);
		}
		else if (m_CurrentLine && canMoveScroller)
			scrollerY = CalculateScrollerY(m_CurrentLine, visibleLines, maxScrollerY);
	}

	//Создание дисплей листа
	glNewList((GLuint)index, GL_COMPILE);

		if (Minimized)
		{
			WORD gumpID = 0x09CB;
			if (m_Updates)
				gumpID = 0x09D3;

			UO->DrawGump(gumpID, 0, posX, posY);

			//Завершим создание листа и выйдем из процедуры
			glEndList();

			FrameRedraw = true;
			FrameCreated = true;

			return;
		}

		UO->DrawGump(0x082D, 0, posX + 137, posY); //Minimize
		UO->DrawGump(0x0820, 0, posX, posY + 23); //Top scroll

		if (m_Updates) //Updates text gump
			UO->DrawGump(0x09D2, 0, posX + 112, posY + 35);
		else //Tips text gump
			UO->DrawGump(0x09CA, 0, posX + 127, posY + 33);
		
		//Tip window body
		int curPosY = posY + 59;

		while (true)
		{
			int deltaHeight = (height + posY) - (curPosY - 36);
			if (deltaHeight  < 70)
			{
				if (deltaHeight > 0)
					UO->DrawGump(0x0821, 0, posX + 18, curPosY, 0, deltaHeight);

				break;
			}
			else
				UO->DrawGump(0x0821, 0, posX + 18, curPosY);

			curPosY += 70;

			deltaHeight = (height + posY) - (curPosY - 36);

			if (deltaHeight < 70)
			{
				if (deltaHeight > 0)
					UO->DrawGump(0x0822, 0, posX + 18, curPosY, 0, deltaHeight);

				break;
			}
			else
				UO->DrawGump(0x0822, 0, posX + 18, curPosY);

			curPosY += 70;
		}
		
		//Up
		//UO->DrawGump(0x001F, 0, PosX + 257, PosY + 72); //Scroller
		//Down
		//UO->DrawGump(0x001F, 0, PosX + 257, PosY + Height - 17); //Scroller

		UO->DrawGump(0x001F, 0, posX + 257, posY + 72 + scrollerY); //Scroller
		
		UO->DrawGump(0x082B, 0, posX + 30, posY + 58); //Top line
		UO->DrawGump(0x0824, 0, posX + 249, posY + 56); //^ button
		UO->DrawGump(0x082B, 0, posX + 31, posY + height + 21); //Bottom line
		UO->DrawGump(0x0825, 0, posX + 249, posY + height + 17); //v button

		g_GL.ViewPort(posX + 36, posY + 68, 214, height - 50);

		m_Text.Draw(posX + 36, posY + 68 - (m_CurrentLine * GUMP_SCROLLING_PIXEL_STEP));

		g_GL.RestorePort();

		UO->DrawGump(0x0823, 0, posX + 18, posY + height + 34); //Bottom scroll

		if (!m_Updates)
		{
			UO->DrawGump(0x09CC, 0, posX + 35, posY + height + 45); //Previous gump

			//UO->DrawGump(0x0823, 0, posX + 18, posY + height + 34); //+/- gump

			UO->DrawGump(0x09CD, 0, posX + 251, posY + height + 45); //Next gump
		}
		
		WORD gumpID = 0x082E + (int)(CanSelectedButton == 2);
		UO->DrawGump(gumpID, 0, posX + 137, posY + height + 66); //Resize

	//Завершаем запись дисплей листа
	glEndList();

	FrameRedraw = true;
	FrameCreated = true;
}
//----------------------------------------------------------------------------
int TGumpTip::Draw(bool &mode)
{
	DWORD index = (DWORD)this;

	//Для быстрого доступа
	int posX = X;
	int posY = Y;

	if (Minimized)
	{
		posX = MinimizedX;
		posY = MinimizedY;
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

	DWORD ticks = GetTickCount();

	//Проверим, вдруг необходимо изменить положение отображаемого элемента
	if (IsScrollerPressed && m_LastScrollChangeTime < ticks)
	{
		if (g_LastObjectLeftMouseDown == ID_GT_BUTTON_UP) //Скроллинг вверх (гампом-стрелкой)
			ListingList(true);
		else if (g_LastObjectLeftMouseDown == ID_GT_BUTTON_DOWN) //Скроллинг вниз (гампом-стрелкой)
			ListingList(false);
	}

	int scrollerY = 0;

	int heightToScrolling = 0;
	bool noRedraw = false;

	if (!Minimized)
	{
		bool canMoveScroller = true;
		if (m_Text.Height < height - 50)
			canMoveScroller = false;
		else
			heightToScrolling = m_Text.Height - (height - 50);
	
		int maxScrollerY = height - 89;

		int visibleLines = heightToScrolling / GUMP_SCROLLING_PIXEL_STEP;

		if (IsScrollerPressed && g_LastObjectLeftMouseDown == ID_GT_SCROLLER && canMoveScroller) //Scroller pressed
		{
			int currentY = (g_MouseY - 10) - posY - 72; //Scroller position

			scrollerY = CalculateScrollerAndTextPosition(m_CurrentLine, visibleLines, maxScrollerY, currentY);

			if (mode)
				GenerateFrame(posX, posY);

			noRedraw = true;
		}
		else if (m_CurrentLine && canMoveScroller)
			scrollerY = CalculateScrollerY(m_CurrentLine, visibleLines, maxScrollerY);
	}

	//Если окошко захвачено для перемещения - вычислим оффсеты
	if (mode && g_LeftMouseDown && g_LastGumpLeftMouseDown == index && !g_LastObjectLeftMouseDown)
	{
		posX += (g_MouseX - g_DroppedLeftMouseX);
		posY += (g_MouseY - g_DroppedLeftMouseY);

		//А так же, если это режим рисования - перерисуем фрэйм
		if (mode && !m_HeightBuffer)
			GenerateFrame(posX, posY);
	}
	else if (mode && !noRedraw) //Перерисовка фрэйма, при соблюдении необходимых условий
	{
		if (CanSelectedButton && !m_HeightBuffer)
			GenerateFrame(posX, posY);
		else if (FrameRedraw)
		{
			GenerateFrame(posX, posY);
			FrameRedraw = false;
		}
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
		if (Minimized)
		{
			WORD gumpID = 0x09CB;
			if (m_Updates)
				gumpID = 0x09D3;

			if (UO->GumpPixelsInXY(gumpID, posX, posY))
			{
				g_LastSelectedObject = 0;
				g_LastSelectedGump = index;
			}

			return 0;
		}
		
		int LSG = 0;
		
		if (UO->GumpPixelsInXY(0x082D, posX + 137, posY)) //Minimize
		{
			LSG = ID_GT_BUTTON_MINIMIZE;
			g_LastSelectedGump = index;
		}
		
		if (UO->GumpPixelsInXY(0x0820, posX, posY + 23)) //Top scroll
		{
			g_LastSelectedObject = 0;
			g_LastSelectedGump = index;
		}

		//Tip window body
		int curPosY = posY + 59;

		while (true)
		{
			int deltaHeight = (height + posY) - (curPosY - 36);

			if (deltaHeight  < 70)
			{
				if (deltaHeight > 0)
				{
					if (UO->GumpPixelsInXY(0x0821, posX + 18, curPosY, 0, deltaHeight))
					{
						g_LastSelectedObject = 0;
						g_LastSelectedGump = index;
					}
				}

				break;
			}
			else if (UO->GumpPixelsInXY(0x0821, posX + 18, curPosY))
			{
				g_LastSelectedObject = 0;
				g_LastSelectedGump = index;

				break;
			}

			curPosY += 70;

			deltaHeight = (height + posY) - (curPosY - 36);

			if (deltaHeight < 70)
			{
				if (deltaHeight > 0)
				{
					if (UO->GumpPixelsInXY(0x0822, posX + 18, curPosY, 0, deltaHeight))
					{
						g_LastSelectedObject = 0;
						g_LastSelectedGump = index;
					}
				}

				break;
			}
			else if (UO->GumpPixelsInXY(0x0822, posX + 18, curPosY))
			{
				g_LastSelectedObject = 0;
				g_LastSelectedGump = index;

				break;
			}

			curPosY += 70;
		}

		if (UO->GumpPixelsInXY(0x0823, posX + 18, posY + height + 34)) //Bottom scroll
		{
			g_LastSelectedObject = 0;
			g_LastSelectedGump = index;
		}

		if (g_LastSelectedGump == index)
		{
			if (UO->GumpPixelsInXY(0x0824, posX + 249, posY + 56)) //^ button
				LSG = ID_GT_BUTTON_UP;
			else if (UO->GumpPixelsInXY(0x0825, posX + 249, posY + height + 17)) //v button
				LSG = ID_GT_BUTTON_DOWN;

			if (!m_Updates)
			{
				if (UO->GumpPixelsInXY(0x09CC, posX + 35, posY + height + 45)) //Previous gump
					LSG = ID_GT_PREV_GUMP;
				else if (UO->GumpPixelsInXY(0x09CD, posX + 251, posY + height + 45)) //Next gump
					LSG = ID_GT_NEXT_GUMP;
			}
		}

		if (UO->GumpPixelsInXY(0x001F, posX + 257, posY + 72 + scrollerY)) //Scroller
		{
			g_LastSelectedGump = index;
			LSG = ID_GT_SCROLLER;
		}
		
		if (UO->GumpPixelsInXY(0x082E, posX + 137, posY + height + 66)) //Resize
		{
			g_LastSelectedGump = index;
			LSG = ID_GT_BUTTON_RESIZE;
		}

		if (LSG != 0)
			g_LastSelectedObject = LSG; //Если что-то нашлось - выбираем

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
		Minimized = true;

		GenerateFrame(MinimizedX, MinimizedY);
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
		UpdateFrame();
	}
	else if (g_LastObjectLeftMouseDown == ID_GT_BUTTON_DOWN && m_LastScrollChangeTime < ticks)
	{
		ListingList(false);
		UpdateFrame();
	}
	else if (g_LastObjectLeftMouseDown == ID_GT_PREV_GUMP)
		SendTipRequest(0);
	else if (g_LastObjectLeftMouseDown == ID_GT_NEXT_GUMP)
		SendTipRequest(1);
}
//----------------------------------------------------------------------------
bool TGumpTip::OnLeftMouseDoubleClick()
{
	if (Minimized)
	{
		Minimized = false;
		GenerateFrame(X, Y);

		return true;
	}

	return false;
}
//----------------------------------------------------------------------------
void TGumpTip::OnMouseWheel(MOUSE_WHEEL_STATE &state)
{
	if (!Minimized && !g_LeftMouseDown && !g_RightMouseDown)
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