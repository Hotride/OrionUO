/****************************************************************************
**
** GumpBulletinBoardItem.cpp
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
TGumpBulletinBoardItem::TGumpBulletinBoardItem(DWORD serial, short x, short y, BYTE variant, DWORD id, string poster, string subject, string dataTime, string data)
: TGump(GT_BULLETIN_BOARD_ITEM, serial, x, y), m_Variant(variant), m_Height(200),
m_HeightBuffer(0), m_CurrentLine(0), m_LastScrollChangeTime(0)
{
	ID = id;

	TextEntry = new TEntryText(0, 220);
	TextEntry->SetText(data);

	TextEntrySubject = new TEntryText(0, 0, 150);
	TextEntrySubject->SetText(subject);

	FontManager->GenerateA(9, m_Poster, poster.c_str(), 0x0386);
	FontManager->GenerateA(9, m_DataTime, dataTime.c_str(), 0x0386);

	FontManager->GenerateA(6, m_Text[0], "Author:", 0);
	FontManager->GenerateA(6, m_Text[1], "Time:", 0);
	FontManager->GenerateA(6, m_Text[2], "Subject:", 0);
}
//----------------------------------------------------------------------------
TGumpBulletinBoardItem::~TGumpBulletinBoardItem()
{
	if (TextEntrySubject != NULL)
	{
		delete TextEntrySubject;
		TextEntrySubject = NULL;
	}

	if (TextEntry != NULL)
	{
		delete TextEntry;
		TextEntry = NULL;
	}

	m_Poster.Clear();
	m_DataTime.Clear();

	IFOR(i, 0, 3)
		m_Text[i].Clear();
}
//----------------------------------------------------------------------------
void TGumpBulletinBoardItem::PrepareTextures()
{
	UO->ExecuteGumpPart(0x0820, 6);
	UO->ExecuteGump(0x0828);
	UO->ExecuteGump(0x0835);
	UO->ExecuteGump(0x082B);
	UO->ExecuteGumpPart(0x082E, 2);

	switch (m_Variant)
	{
		case 0:
		{
			UO->ExecuteGump(0x0883); //NEW MESSAGE
			UO->ExecuteGump(0x0886); //Pos

			break;
		}
		case 2:
			UO->ExecuteGump(0x0885); //Remove
		case 1:
		{
			UO->ExecuteGump(0x0884); //Reply

			break;
		}
		default:
			break;
	}
}
//----------------------------------------------------------------------------
void TGumpBulletinBoardItem::SetHeight(int val)
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
void TGumpBulletinBoardItem::ChangeHeight()
{
	//Событие изменения высоты гампа с вложенной корректировкой
	if (m_HeightBuffer)
		SetHeight(m_Height + (g_MouseY - m_HeightBuffer));

	m_HeightBuffer = 0;
	g_ResizedGump = NULL;
}
//----------------------------------------------------------------------------
int TGumpBulletinBoardItem::GetHeight()
{
	return TextEntry->m_Texture.Height + 10;
}
//---------------------------------------------------------------------------
void TGumpBulletinBoardItem::CalculateGumpState()
{
	TGump::CalculateGumpState();

	if (g_LeftMouseDown && g_LastGumpLeftMouseDown == (DWORD)this && (!g_LastObjectLeftMouseDown || g_LastObjectLeftMouseDown == ID_GBBI_TEXT_FIELD))
	{
		g_GumpMovingOffsetX = g_MouseX - g_DroppedLeftMouseX;
		g_GumpMovingOffsetY = g_MouseY - g_DroppedLeftMouseY;
	}
	else
	{
		g_GumpMovingOffsetX = 0;
		g_GumpMovingOffsetY = 0;
	}

	if (m_Minimized)
	{
		g_GumpTranslateX = (float)(m_MinimizedX + g_GumpMovingOffsetX);
		g_GumpTranslateY = (float)(m_MinimizedY + g_GumpMovingOffsetY);
	}
	else
	{
		g_GumpTranslateX = (float)(m_X + g_GumpMovingOffsetX);
		g_GumpTranslateY = (float)(m_Y + g_GumpMovingOffsetY);
	}
}
//---------------------------------------------------------------------------
void TGumpBulletinBoardItem::GenerateFrame()
{
	if (!g_DrawMode)
	{
		m_FrameRedraw = false;
		m_FrameCreated = false;

		return;
	}

	CalculateGumpState();

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
	
	int scrollerY = 0;

	int heightToScrolling = 0;

	bool canMoveScroller = true;

	int curHeight = GetHeight();

	if (curHeight < height - 100)
		canMoveScroller = false;
	else
		heightToScrolling = curHeight - (height - 100);
	
	int maxScrollerY = height - 132;
	
	int visibleLines = heightToScrolling / GUMP_SCROLLING_PIXEL_STEP;

	if (g_GumpPressedScroller && g_LastObjectLeftMouseDown == ID_GBBI_SCROLLER && canMoveScroller) //Scroller pressed
	{
		int currentY = (g_MouseY - 10) - (int)g_GumpTranslateY - 120; //Scroller position

		scrollerY = CalculateScrollerAndTextPosition(m_CurrentLine, visibleLines, maxScrollerY, currentY);
	}
	else if (m_CurrentLine && canMoveScroller)
		scrollerY = CalculateScrollerY(m_CurrentLine, visibleLines, maxScrollerY);

	//Создание дисплей листа
	glNewList((GLuint)this, GL_COMPILE);

		UO->DrawGump(0x0820, 0, 0, 0); //Top scroll

		//Profile window body
		int curPosY = 36;

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
		
		UO->DrawGump(0x082B, 0, 30, 34); //Top line
		
		//Author
		m_Text[0].Draw(30, 40);
		m_Poster.Draw(30 + m_Text[0].Width, 46);

		//Time
		m_Text[1].Draw(30, 56);
		m_DataTime.Draw(30 + m_Text[1].Width, 62);

		//Subject
		m_Text[2].Draw(30, 72);
		WORD subjectColor = 0x0386;
		if (!m_Variant)
			subjectColor = 0x0008;
		TextEntrySubject->DrawA(9, subjectColor, 30 + m_Text[2].Width, 78);

		UO->DrawGump(0x0835, 0, 30, 110, 204, 0); //Line
		
		UO->DrawGump(0x0824, 0, 246, 106); //^ button
		UO->DrawGump(0x0828, 0, 255, 120 + scrollerY); //Scroller
		
		g_GL.Scissor((int)g_GumpTranslateX + 30, (int)g_GumpTranslateY + 120, 220, height - 100);

		int yPtr = 120 - (m_CurrentLine * GUMP_SCROLLING_PIXEL_STEP);

		TextEntry->DrawA(9, 0x0386, 30, yPtr, TS_LEFT, 0);
		
		glDisable(GL_SCISSOR_TEST);

		UO->DrawGump(0x0823, 0, 18, height + 34); //Bottom scroll
		UO->DrawGump(0x0825, 0, 246, height + 17); //v button
		UO->DrawGump(0x082B, 0, 30, height + 21); //Bottom line

		switch (m_Variant)
		{
			case 0:
			{
				UO->DrawGump(0x0883, 0, 97, 12); //NEW MESSAGE
				UO->DrawGump(0x0886, 0, 37, height + 45); //Post

				break;
			}
			case 2:
				UO->DrawGump(0x0885, 0, 235, height + 45); //Remove
			case 1:
			{
				UO->DrawGump(0x0884, 0, 37, height + 45); //Reply

				break;
			}
			default:
				break;
		}

		WORD gumpID = 0x082E + (int)(g_GumpSelectElement == ID_GBBI_BUTTON_RESIZE);
		UO->DrawGump(gumpID, 0, 139, height + 66); //Resize

	//Завершаем запись дисплей листа
	glEndList();

	m_FrameRedraw = true;
	m_FrameCreated = true;
}
//----------------------------------------------------------------------------
int TGumpBulletinBoardItem::Draw(bool &mode)
{
	DWORD index = (DWORD)this;

	CalculateGumpState();

	int height = m_Height;

	if (m_HeightBuffer)
	{
		height += (g_MouseY - m_HeightBuffer);

		if (height < 120)
			height = 120;

		int buf = (GetSystemMetrics(SM_CYSCREEN) - 30);
		if (height >= buf)
			height = buf;
		
		if (mode)
			m_FrameCreated = false;
	}
	
	//Проверим, вдруг необходимо изменить положение отображаемого элемента
	if (g_GumpPressed && m_LastScrollChangeTime < timeGetTime())
	{
		if (g_LastObjectLeftMouseDown == ID_GBBI_SCROLLER_UP) //Скроллинг вверх (гампом-стрелкой)
			ListingList(true, 10);
		else if (g_LastObjectLeftMouseDown == ID_GBBI_SCROLLER_DOWN) //Скроллинг вниз (гампом-стрелкой)
			ListingList(false, 10);
	}

	int scrollerY = 0;

	int heightToScrolling = 0;

	bool canMoveScroller = true;

	int curHeight = GetHeight();

	if (curHeight < height - 100)
		canMoveScroller = false;
	else
		heightToScrolling = curHeight - (height - 100);
	
	int maxScrollerY = height - 132;

	int visibleLines = heightToScrolling / GUMP_SCROLLING_PIXEL_STEP;

	if (g_GumpPressedScroller && g_LastObjectLeftMouseDown == ID_GBBI_SCROLLER && canMoveScroller) //Scroller pressed
	{
		int currentY = (g_MouseY - 10) - (int)g_GumpTranslateY - 120; //Scroller position

		scrollerY = CalculateScrollerAndTextPosition(m_CurrentLine, visibleLines, maxScrollerY, currentY);
		
		m_FrameCreated = false;
	}
	else if (m_CurrentLine && canMoveScroller)
		scrollerY = CalculateScrollerY(m_CurrentLine, visibleLines, maxScrollerY);

	if (mode) //Отрисовка
	{
		if (g_GumpSelectElement || !m_FrameCreated || g_GumpMovingOffsetX || g_GumpMovingOffsetY)
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

		int LSG = 0;

		if (UO->GumpPixelsInXY(0x0820, 0, 23)) //Top scroll
		{
			g_LastSelectedObject = 0;
			g_LastSelectedGump = index;
		}
		else if (UO->GumpPixelsInXY(0x0823, 18, height + 34)) //Bottom scroll
		{
			g_LastSelectedObject = 0;
			g_LastSelectedGump = index;
		}
		else if (UO->GumpPixelsInXY(0x0828, 255, 120 + scrollerY)) //Scroller
		{
			g_LastSelectedGump = index;
			LSG = ID_GBBI_SCROLLER;
		}
		else if (UO->GumpPixelsInXY(0x082E, 139, height + 66)) //Resize
		{
			g_LastSelectedGump = index;
			LSG = ID_GBBI_BUTTON_RESIZE;
		}
		else //body
		{
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
		}
		
		if (g_LastSelectedGump == index)
		{
			if (UO->PolygonePixelsInXY(246, 106, 20, 18)) //^
				LSG = ID_GBBI_SCROLLER_UP;
			else if (UO->PolygonePixelsInXY(246, height + 17, 20, 18)) //v
				LSG = ID_GBBI_SCROLLER_DOWN;
			else
			{
				switch (m_Variant)
				{
					case 0:
					{
						if (UO->PolygonePixelsInXY(30 + m_Text[2].Width, 78, 160, 16)) //Subject text
							LSG = ID_GBBI_SUBJECT_TEXT_FIELD;
						else if (LSG != ID_GBBI_SCROLLER && UO->PolygonePixelsInXY(30, 120, 220, height - 100)) //Text
							LSG = ID_GBBI_TEXT_FIELD;
						else if (UO->PolygonePixelsInXY(37, height + 45, 30, 14)) //Post
							LSG = ID_GBBI_POST;

						break;
					}
					case 2:
					{
						if (UO->PolygonePixelsInXY(235, height + 45, 50, 14)) //Remove
							LSG = ID_GBBI_REMOVE;
					}
					case 1:
					{
						if (UO->PolygonePixelsInXY(37, height + 45, 40, 14)) //Reply
							LSG = ID_GBBI_REPLY;

						break;
					}
					default:
						break;
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
void TGumpBulletinBoardItem::OnLeftMouseDown()
{
	if (g_LastSelectedObject == ID_GBBI_SUBJECT_TEXT_FIELD)
	{
		int x = g_MouseX - (m_X + 40);
		int y = g_MouseY - (m_Y + 44);

		TextEntrySubject->OnClick(this, 9, true, x, y);
	}
	else if (g_LastSelectedObject == ID_GBBI_TEXT_FIELD)
	{
		int yPtr = 40 - (m_CurrentLine * GUMP_SCROLLING_PIXEL_STEP);

		int x = g_MouseX - (m_X + 40);
		int y = g_MouseY - (m_Y + yPtr + 44);

		TextEntry->OnClick(this, 1, true, x, y);
	}
}
//----------------------------------------------------------------------------
void TGumpBulletinBoardItem::OnLeftMouseUp()
{
	if (g_LastObjectLeftMouseDown != g_LastSelectedObject)
		return;

	if (g_LastObjectLeftMouseDown == ID_GBBI_BUTTON_RESIZE)
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
	else if (g_LastObjectLeftMouseDown == ID_GBBI_POST)
	{
		TPacketBulletinBoardPostMessage packet(m_ID, 0, TextEntrySubject->c_str(), TextEntry->c_str());
		packet.Send();
		packet.Free();
		
		GumpManager->RemoveGump(this);
	}
	else if (g_LastObjectLeftMouseDown == ID_GBBI_REPLY)
	{
		string subj("RE: ");
		subj += TextEntrySubject->c_str();

		TGumpBulletinBoardItem *gump = new TGumpBulletinBoardItem(0, 0, 0, 0, m_ID, g_Player->GetName(), subj, "Date/Time", "");

		TGump *gumpEntry = GumpManager->GetTextEntryOwner();

		EntryPointer = gump->TextEntrySubject;

		if (gumpEntry != NULL)
			gumpEntry->FrameCreated = false;

		GumpManager->AddGump(gump);
	}
	else if (g_LastObjectLeftMouseDown == ID_GBBI_REMOVE)
	{
		TPacketBulletinBoardRemoveMessage packet(m_ID, m_Serial);
		packet.Send();

		//GumpManager->CloseGump(Serial, ID, GT_BULLETIN_BOARD_ITEM);
	}
}
//----------------------------------------------------------------------------
void TGumpBulletinBoardItem::OnMouseWheel(MOUSE_WHEEL_STATE &state)
{
	if (!g_LeftMouseDown && !g_RightMouseDown)
	{
		if (m_LastScrollChangeTime < timeGetTime())
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
void TGumpBulletinBoardItem::OnCharPress(WPARAM &wparam, LPARAM &lparam)
{
	EntryPointer->Insert(wparam);

	m_FrameCreated = false;
}
//----------------------------------------------------------------------------
void TGumpBulletinBoardItem::OnKeyPress(WPARAM &wparam, LPARAM &lparam)
{
	if (wparam == VK_RETURN && EntryPointer == TextEntry)
	{
		EntryPointer->Insert(L'\n');

		m_FrameCreated = false;
	}
	else
		EntryPointer->OnKey(this, wparam);
}
//----------------------------------------------------------------------------
void TGumpBulletinBoardItem::ListingList(bool direction, int divizor)
{
	if (direction) //Up
	{
		if (m_CurrentLine > 1)
			m_CurrentLine--;
		else
			m_CurrentLine = 0;
	}
	else //Down
	{
		int maxidx = (GetHeight() - (m_Height - 100)) / GUMP_SCROLLING_PIXEL_STEP;

		if (maxidx < 0)
			maxidx = 0;

		if (m_CurrentLine < maxidx)
			m_CurrentLine++;
		else
			m_CurrentLine = maxidx;
	}

	m_LastScrollChangeTime = timeGetTime() + (SCROLL_LISTING_DELAY / divizor);
}
//----------------------------------------------------------------------------