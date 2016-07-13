/****************************************************************************
**
** GumpProfile.cpp
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
TGumpProfile::TGumpProfile(DWORD serial, short x, short y, wstring topText, wstring bottomText, wstring dataText)
: TGump(GT_PROFILE, serial, x, y), m_Height(200), m_HeightBuffer(0), m_CurrentLine(0),
m_LastScrollChangeTime(0), m_Changed(false)
{
	FontManager->GenerateW(0, m_TopText, topText.c_str(), 0, 30, 140);
	FontManager->GenerateW(0, m_BottomText, bottomText.c_str(), 0, 30, 190);

	TextEntry = new TEntryText(0, 210);
	TextEntry->SetText(dataText);
}
//----------------------------------------------------------------------------
TGumpProfile::~TGumpProfile()
{
	m_TopText.Clear();
	m_BottomText.Clear();

	if (m_Changed)
	{
		TPacketProfileUpdate packet(m_Serial, TextEntry->Data(), TextEntry->Length());
		packet.Send();
		packet.Free();
	}

	if (TextEntry != NULL)
	{
		delete TextEntry;
		TextEntry = NULL;
	}
}
//----------------------------------------------------------------------------
void TGumpProfile::PrepareTextures()
{
	Orion->ExecuteGump(0x001F);
	Orion->ExecuteGumpPart(0x005C, 6);
	Orion->ExecuteGumpPart(0x0820, 4);
	Orion->ExecuteButton(0x082D);
	Orion->ExecuteGump(0x09D4);
}
//----------------------------------------------------------------------------
void TGumpProfile::SetHeight(int val)
{
	//Установка и корректировка значения высоты гампа
	if (val < 120)
		val = 120;

	int buf = GetSystemMetrics(SM_CYSCREEN) - 30;
	if (val >= buf)
		val = (GetSystemMetrics(SM_CYSCREEN) - 30);

	m_Height = val;
}
//----------------------------------------------------------------------------
void TGumpProfile::ChangeHeight()
{
	//Событие изменения высоты гампа с вложенной корректировкой
	if (m_HeightBuffer)
		SetHeight(m_Height + (g_MouseY - m_HeightBuffer));

	m_HeightBuffer = 0;
	g_ResizedGump = NULL;
}
//----------------------------------------------------------------------------
int TGumpProfile::GetHeight()
{
	int height = m_TopText.Height;

	if (TextEntry->Length())
		height += TextEntry->m_Texture.Height;
	else
		height += 14;

	height += 73;
		
	height += m_BottomText.Height;
	
	return height;
}
//---------------------------------------------------------------------------
void TGumpProfile::GenerateFrame()
{
	if (!g_DrawMode)
	{
		m_FrameRedraw = false;
		m_FrameCreated = false;

		return;
	}

	int height = m_Height;

	if (m_HeightBuffer)
	{
		height += (g_MouseY - m_HeightBuffer);

		if (height < 120)
			height = 120;

		int buf = (GetSystemMetrics(SM_CYSCREEN) - 30);
		if (height >= buf)
			height = buf;
	}
	
	CalculateGumpState();

	int scrollerY = 0;

	int heightToScrolling = 0;

	if (!m_Minimized)
	{
		bool canMoveScroller = true;

		int curHeight = GetHeight();

		if (curHeight < height - 30)
			canMoveScroller = false;
		else
			heightToScrolling = curHeight - (height - 30);
	
		int maxScrollerY = height - 89;

		int visibleLines = heightToScrolling / GUMP_SCROLLING_PIXEL_STEP;

		if (g_GumpPressedScroller && g_LastObjectLeftMouseDown == ID_GP_SCROLLER && canMoveScroller) //Scroller pressed
		{
			int currentY = (g_MouseY - 10) - (int)g_GumpTranslateY - 72; //Scroller position

			scrollerY = CalculateScrollerAndTextPosition(m_CurrentLine, visibleLines, maxScrollerY, currentY);
		}
		else if (m_CurrentLine && canMoveScroller)
			scrollerY = CalculateScrollerY(m_CurrentLine, visibleLines, maxScrollerY);
	}

	if (TextEntry->Changed)
		TextEntry->DrawW(0, 0, 40, /*yPtr +*/ 44, TS_LEFT, 0);

	//Создание дисплей листа
	glNewList((GLuint)this, GL_COMPILE);

		if (m_Minimized)
		{
			Orion->DrawGump(0x09D4, 0, 0, 0);

			//Завершим создание листа и выйдем из процедуры
			glEndList();

			m_FrameRedraw = true;
			m_FrameCreated = true;

			return;
		}

		Orion->DrawGump(0x082D, 0, 137, 0); //Minimize
		Orion->DrawGump(0x0820, 0, 0, 23); //Top scroll

		//Profile window body
		int curPosY = 59;

		while (true)
		{
			int deltaHeight = height - (curPosY - 36);
			if (deltaHeight  < 70)
			{
				if (deltaHeight > 0)
					Orion->DrawGump(0x0821, 0, 18, curPosY, 0, deltaHeight);

				break;
			}
			else
				Orion->DrawGump(0x0821, 0, 18, curPosY);

			curPosY += 70;

			deltaHeight = height - (curPosY - 36);

			if (deltaHeight < 70)
			{
				if (deltaHeight > 0)
					Orion->DrawGump(0x0822, 0, 18, curPosY, 0, deltaHeight);

				break;
			}
			else
				Orion->DrawGump(0x0822, 0, 18, curPosY);

			curPosY += 70;
		}

		Orion->DrawGump(0x001F, 0, 257, 72 + scrollerY); //Scroller
		
		int yPtr = m_TopText.Height + 40;

		g_GL.Scissor((int)g_GumpTranslateX + 36, (int)g_GumpTranslateY + yPtr, 220, height - 30);
		
		yPtr -= (m_CurrentLine * GUMP_SCROLLING_PIXEL_STEP);

		Orion->DrawGump(0x005C, 0, 40, yPtr); //top text ornament
		Orion->DrawGump(0x005D, 0, 92, yPtr, 138, 0); //top text ornament
		Orion->DrawGump(0x005E, 0, 230, yPtr); //top text ornament
		m_TopText.Draw(89, yPtr - m_TopText.Height + 22);

		TextEntry->DrawW(0, 0, 40, yPtr + 44, TS_LEFT, 0);

		if (TextEntry->Length())
			yPtr += TextEntry->m_Texture.Height;
		else
			yPtr += 14;

		yPtr += 47;
		
		Orion->DrawGump(0x005F, 0, 40, yPtr); //bottom text ornament
		Orion->DrawGump(0x0060, 0, 49, yPtr + 9, 197, 0); //bottom text ornament
		Orion->DrawGump(0x0061, 0, 246, yPtr); //bottom text ornament

		m_BottomText.Draw(38, yPtr + 26);
		
		glDisable(GL_SCISSOR_TEST);

		Orion->DrawGump(0x0823, 0, 18, height + 34); //Bottom scroll

		WORD gumpID = 0x082E + (int)(g_GumpSelectElement == ID_GP_BUTTON_RESIZE);
		Orion->DrawGump(gumpID, 0, 137, height + 66); //Resize

	//Завершаем запись дисплей листа
	glEndList();

	m_FrameRedraw = true;
	m_FrameCreated = true;
}
//----------------------------------------------------------------------------
int TGumpProfile::Draw(bool &mode)
{
	DWORD index = (DWORD)this;

	if (m_Minimized)
		m_HeightBuffer = 0;

	int height = m_Height;

	if (m_HeightBuffer)
	{
		height += (g_MouseY - m_HeightBuffer);

		if (height < 120)
			height = 120;

		int buf = (GetSystemMetrics(SM_CYSCREEN) - 30);
		if (height >= buf)
			height = buf;
		
		//Если это режим рисования - слепим фрэйм гампа
		if (mode)
			m_FrameCreated = false;
	}

	CalculateGumpState();

	int scrollerY = 0;

	int heightToScrolling = 0;

	if (!Minimized)
	{
		bool canMoveScroller = true;

		int curHeight = GetHeight();

		if (curHeight < height - 30)
			canMoveScroller = false;
		else
			heightToScrolling = curHeight - (height - 30);
	
		int maxScrollerY = height - 89;

		int visibleLines = heightToScrolling / GUMP_SCROLLING_PIXEL_STEP;

		if (g_GumpPressedScroller && g_LastObjectLeftMouseDown == ID_GP_SCROLLER && canMoveScroller) //Scroller pressed
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
		if (!m_FrameCreated || g_GumpMovingOffsetX || g_GumpMovingOffsetY)
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
			if (Orion->GumpPixelsInXY(0x09D4, 0, 0))
			{
				g_LastSelectedObject = 0;
				g_LastSelectedGump = index;
			}

			g_MouseX = oldMouseX;
			g_MouseY = oldMouseY;

			return 0;
		}
		
		int LSG = 0;
		
		if (Orion->GumpPixelsInXY(0x082D, 137, 0)) //Minimize
			LSG = ID_GP_BUTTON_MINIMIZE;
		
		if (Orion->GumpPixelsInXY(0x0820, 0, 23)) //Top scroll
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
					if (Orion->GumpPixelsInXY(0x0821, 18, curPosY, 0, deltaHeight))
					{
						g_LastSelectedObject = 0;
						g_LastSelectedGump = index;
					}
				}

				break;
			}
			else if (Orion->GumpPixelsInXY(0x0821, 18, curPosY))
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
					if (Orion->GumpPixelsInXY(0x0822, 18, curPosY, 0, deltaHeight))
					{
						g_LastSelectedObject = 0;
						g_LastSelectedGump = index;
					}
				}

				break;
			}
			else if (Orion->GumpPixelsInXY(0x0822, 18, curPosY))
			{
				g_LastSelectedObject = 0;
				g_LastSelectedGump = index;

				break;
			}

			curPosY += 70;
		}
		
		if (g_MouseY > 58 && g_MouseY < 58 + height - 30) //В рабочей зоне
		{
			int yPtr = m_TopText.Height + 40 - (m_CurrentLine * GUMP_SCROLLING_PIXEL_STEP);

			if (Orion->GumpPixelsInXY(0x005C, 40, yPtr))
				LSG = ID_GP_APPLY;
			
			int textHeight = 14;

			if (TextEntry->Length())
				textHeight = TextEntry->m_Texture.Height;

			if (Orion->PolygonePixelsInXY(40, yPtr + 44, 210, textHeight))
				LSG = ID_GP_TEXT_FIELD;
		}

		if (Orion->GumpPixelsInXY(0x0823, 18, height + 34)) //Bottom scroll
		{
			g_LastSelectedObject = 0;
			g_LastSelectedGump = index;
		}

		if (Orion->GumpPixelsInXY(0x001F, 257, 72 + scrollerY)) //Scroller
			LSG = ID_GP_SCROLLER;
		
		if (Orion->GumpPixelsInXY(0x082E, 137, height + 66)) //Resize
			LSG = ID_GP_BUTTON_RESIZE;

		g_MouseX = oldMouseX;
		g_MouseY = oldMouseY;

		return LSG;
	}

	return 0;
}
//----------------------------------------------------------------------------
void TGumpProfile::OnLeftMouseDown()
{
	if (m_Serial == g_PlayerSerial && g_LastSelectedObject == ID_GP_TEXT_FIELD)
	{
		int yPtr = m_TopText.Height + 40 - (m_CurrentLine * GUMP_SCROLLING_PIXEL_STEP);

		int x = g_MouseX - (m_X + 40);
		int y = g_MouseY - (m_Y + yPtr + 44);

		TextEntry->OnClick(this, 1, true, x, y);
	}
}
//----------------------------------------------------------------------------
void TGumpProfile::OnLeftMouseUp()
{
	if (g_LastObjectLeftMouseDown != g_LastSelectedObject)
		return;

	DWORD ticks = GetTickCount();

	if (g_LastObjectLeftMouseDown == ID_GP_BUTTON_MINIMIZE)
	{
		m_Minimized = true;

		m_FrameCreated = false;
	}
	else if (g_LastObjectLeftMouseDown == ID_GP_BUTTON_RESIZE)
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
	else if (m_Serial == g_PlayerSerial && g_LastObjectLeftMouseDown == ID_GP_APPLY)
	{
		m_Changed = true;
		
		if (EntryPointer == TextEntry)
		{
			if (ConfigManager.GetConsoleNeedEnter())
				EntryPointer = NULL;
			else
				EntryPointer = GameConsole;
		}

		m_FrameCreated = false;
	}
}
//----------------------------------------------------------------------------
bool TGumpProfile::OnLeftMouseDoubleClick()
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
void TGumpProfile::OnMouseWheel(MOUSE_WHEEL_STATE &state)
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
void TGumpProfile::ListingList(bool direction, int divizor)
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
		int maxidx = (GetHeight() - (m_Height - 30)) / GUMP_SCROLLING_PIXEL_STEP;

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
void TGumpProfile::OnCharPress(WPARAM &wparam, LPARAM &lparam)
{
	if (wparam == VK_RETURN || wparam == VK_BACK || wparam == VK_ESCAPE || EntryPointer == NULL)
		return; //Ignore no print keys

	wchar_t wstr[2] = {0};
	wstr[0] = wparam;

	wstring str(wstr);
	if (!str.length())
		return;

	EntryPointer->Insert(wparam);

	m_FrameCreated = false;
}
//----------------------------------------------------------------------------
void TGumpProfile::OnKeyPress(WPARAM &wparam, LPARAM &lparam)
{
	if (wparam == VK_RETURN)
	{
		EntryPointer->Insert(0x000D);

		m_FrameCreated = false; //Перерисуем
	}
	else
		EntryPointer->OnKey(this, wparam);
}
//----------------------------------------------------------------------------