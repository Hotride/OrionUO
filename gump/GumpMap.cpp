/****************************************************************************
**
** GumpMap.cpp
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
//---------------------------------------------------------------------------
TGumpMap::TGumpMap(DWORD serial, short x, short y)
: TGump(GT_MAP, serial, x, y), m_PlotState(0), m_StartX(0), m_StartY(0),
m_EndX(0), m_EndY(0), m_Width(0), m_Height(0), Texture(0), m_DoubleImage(false),
m_PinTimer(0), PinOnCursor(NULL)
{
}
//---------------------------------------------------------------------------
TGumpMap::~TGumpMap()
{
}
//---------------------------------------------------------------------------
void TGumpMap::PrepareTextures()
{
	UO->ExecuteResizepic(0x1432);
	UO->ExecuteGumpPart(0x1398, 4);
	UO->ExecuteGump(0x139D);
}
//---------------------------------------------------------------------------
void TGumpMap::GenerateFrame(int posX, int posY)
{
	if (!g_DrawMode)
	{
		FrameRedraw = false;
		FrameCreated = false;

		return;
	}
}
//----------------------------------------------------------------------------
int TGumpMap::LineUnderMouse(__inout int &x1, __inout int &y1, __in int x2, __in int y2)
{
	int tempX = x2 - x1;
	int tempY = y2 - y1;

	float testOfsX = (float)tempX;

	if (testOfsX == 0.0f)
		testOfsX = 1.0f;

	float pi = (float)M_PI;

	float a = -(atan(tempY / testOfsX) * 180.0f / pi);

	int inverseCheck = 0;

	if (x1 >= x2 && y1 <= y2)
		inverseCheck = 1;
	else if (x1 >= x2 && y1 >= y2)
		inverseCheck = 1;

	float sinA = sin(a * pi / 180.f);
	float cosA = cos(a * pi / 180.f);

	int offsX = (int)((tempX * cosA) - (tempY * sinA));
	int offsY = (int)((tempX * sinA) + (tempY * cosA));

	int endX2 = x1 + offsX;
	int endY2 = y1 + offsY;

	tempX = g_MouseX - x1;
	tempY = g_MouseY - y1;

	offsX = (int)((tempX * cosA) - (tempY * sinA));
	offsY = (int)((tempX * sinA) + (tempY * cosA));

	POINT mousePoint = {x1 + offsX, y1 + offsY};

	const int polyOffset = 5;

	int result = 0;

	if (!inverseCheck)
	{
		RECT lineRect = {x1 - polyOffset, y1 - polyOffset, endX2 + polyOffset, endY2 + polyOffset};

		if (PtInRect(&lineRect, mousePoint))
		{
			x1 = x1 + ((x2 - x1) / 2);
			y1 = y1 + ((y2 - y1) / 2);

			result = 1;
		}
	}
	else
	{		
		RECT lineRect = {endX2 - polyOffset, endY2 - polyOffset, x1 + polyOffset, y1 + polyOffset};

		if (PtInRect(&lineRect, mousePoint))
		{
			x1 = x2 + ((x1 - x2) / 2);
			y1 = y2 + ((y1 - y2) / 2);

			result = 2;
		}
	}

	return result;
}
//----------------------------------------------------------------------------
int TGumpMap::Draw(__in bool &mode)
{
	const DWORD lineColor[2] = {0x007F7F7F, 0x006F6F6F};
	DWORD index = (DWORD)this;

	TGameItem *selobj = World->FindWorldItem(Serial);
	if (selobj == NULL)
		return 0; //Объект, к которому привязан ползунок - исчез
	
	//Для быстрого доступа
	int posX = X;
	int posY = Y;

	//Нажата ли кнопка в окне?
	bool IsPressed = (g_LeftMouseDown && g_LastGumpLeftMouseDown == index && g_LastSelectedGump == index);
	
	//Может ли быть подсвечен элемент?
	int CanSelectedButton = ((g_LastSelectedGump == index) ? g_LastSelectedObject : 0);

	//Если окошко захвачено для перемещения - вычислим оффсеты
	if (mode && g_LeftMouseDown && g_LastGumpLeftMouseDown == index)
	{
		int ofsX = (g_MouseX - g_DroppedLeftMouseX);
		int ofsY = (g_MouseY - g_DroppedLeftMouseY);

		if (!g_LastObjectLeftMouseDown)
		{
			posX += ofsX;
			posY += ofsY;
		}
		else if (PinOnCursor == NULL && (ofsX || ofsY) && g_LastObjectLeftMouseDown > ID_GM_PIN_LIST && g_LastObjectLeftMouseDown < ID_GM_PIN_LIST_INSERT && m_PinTimer > GetTickCount())
		{
			int objIndex = g_LastObjectLeftMouseDown - ID_GM_PIN_LIST;

			TGumpObject *go = (TGumpObject*)m_Items;
			int idx = 1;

			for (; go != NULL && idx != objIndex; go = (TGumpObject*)go->m_Next, idx++);

			if (go != NULL)
				PinOnCursor = go;
		}
		else if (PinOnCursor != NULL)
		{
			PinOnCursor->X = g_MouseX - (X + 20);
			PinOnCursor->Y = g_MouseY - (Y + 30);
		}
	}

	if (mode) //Отрисовка
	{
		UO->DrawResizepicGump(0x1432, posX, posY, m_Width + 44, m_Height + 61); //Map Gump

		if (m_PlotState == 0)
			UO->DrawGump(0x1398, 0, posX + ((m_Width - 100) / 2), posY + 5); //Plot Course
		else if (m_PlotState == 1)
		{
			UO->DrawGump(0x1399, 0, posX + ((m_Width - 70) / 2), posY + 5); //Stop Plotting
			UO->DrawGump(0x139A, 0, posX + ((m_Width - 66) / 2), posY + m_Height + 37); //Clear Course
		}
		
		int tempX = posX + 24;
		int tempY = posY + 32;
		g_GL.Draw(Texture, tempX, tempY, (float)m_Width, (float)m_Height);

		if (m_DoubleImage)
		{
			tempX++;
			g_GL.Draw(Texture, tempX, tempY, (float)m_Width, (float)m_Height);
			tempX--;
			tempY++;
			g_GL.Draw(Texture, tempX, tempY, (float)m_Width, (float)m_Height);
		}

		UO->DrawGump(0x139D, 0, posX + 174, posY + 177); //N

		//Отрисовка кнопок
		TGumpObject *go = (TGumpObject*)m_Items;
		int idx = 1;

		while (go != NULL)
		{
			int drawX = posX + go->X + 18;
			int drawY = posY + go->Y + 21;

			TGumpObject *next = (TGumpObject*)go->m_Next;

			if (next != NULL)
			{
				int nextDrawX = posX + next->X + 20;
				int nextDrawY = posY + next->Y + 30;

				g_GL.DrawLine(lineColor[(int)(next == PinOnCursor || go == PinOnCursor)], drawX + 2.0f, drawY + 8.0f, (float)nextDrawX, (float)nextDrawY);
				
				if (PinOnCursor == NULL && CanSelectedButton >= ID_GM_PIN_LIST_INSERT && (CanSelectedButton - ID_GM_PIN_LIST_INSERT) == idx)
				{
					int checkX = drawX + 2;
					int checkY = drawY + 8;

					if (LineUnderMouse(checkX, checkY, nextDrawX, nextDrawY))
						UO->DrawGump(0x139B, 0, checkX - 2, checkY - 8);
				}
			}

			if (go != PinOnCursor)
			{
				UO->DrawGump(0x139B, 0, drawX, drawY);

				char text[10] = {0};
				sprintf(text, "%d", idx);
				FontManager->DrawA(3, text, 0, drawX - 10, drawY - 12);
			}
			else
				idx--;

			go = (TGumpObject*)go->m_Next;
			idx++;
		}
	}
	else //Выбор объектов
	{
		int LSG = 0;

		//Если выбран основной гамп - меняем глобальный указатель на выбранный гамп на него
		if (UO->ResizepicPixelsInXY(0x1432, posX, posY, m_Width + 44, m_Height + 61))
		{
			g_LastSelectedObject = 0;
			g_LastSelectedGump = index;
		}

		if (m_PlotState == 0)
		{
			if (UO->GumpPixelsInXY(0x1398, posX + ((m_Width - 99) / 2), posY + 5)) //Plot Course
				LSG = ID_GM_PLOT_COURSE;
		}
		else if (m_PlotState == 1)
		{
			if (UO->GumpPixelsInXY(0x1398, posX + ((m_Width - 69) / 2), posY + 5)) //Stop Plotting
				LSG = ID_GM_STOP_PLOTTING;
			else if (UO->GumpPixelsInXY(0x1398, posX + ((m_Width - 66) / 2), posY + m_Height + 37)) //Clear Course
				LSG = ID_GM_CLEAR_COURSE;

			TGumpObject *go = (TGumpObject*)m_Items;
			int idx = 1;

			for (; go != NULL; go = (TGumpObject*)go->m_Next, idx++)
			{
				int drawX = posX + go->X + 18;
				int drawY = posY + go->Y + 21;

				if (UO->PolygonePixelsInXY(drawX, drawY, 10, 10))
					LSG = ID_GM_PIN_LIST + idx;
			}

			if (LSG < ID_GM_PIN_LIST)
			{
				go = (TGumpObject*)m_Items;
				idx = 1;

				for (; go != NULL; go = (TGumpObject*)go->m_Next, idx++)
				{
					int drawX = posX + go->X + 18;
					int drawY = posY + go->Y + 21;
					
					TGumpObject *next = (TGumpObject*)go->m_Next;

					if (next != NULL)
					{
						int nextDrawX = posX + next->X + 20;
						int nextDrawY = posY + next->Y + 30;
						
						int checkX = drawX + 2;
						int checkY = drawY + 8;

						if (LineUnderMouse(checkX, checkY, nextDrawX, nextDrawY))
							LSG = ID_GM_PIN_LIST_INSERT + idx;
					}
				}
			}
		}
		
		if (LSG != 0)
		{
			g_LastSelectedObject = LSG; //Если что-то нашлось - выбираем
			g_LastSelectedGump = index;
		}

		return LSG;
	}

	return 0;
}
//----------------------------------------------------------------------------
void TGumpMap::OnLeftMouseDown()
{
	m_PinTimer = GetTickCount() + 300;
}
//----------------------------------------------------------------------------
void TGumpMap::OnLeftMouseUp()
{
	if (g_LastObjectLeftMouseDown == g_LastSelectedObject)
	{
		if (g_LastObjectLeftMouseDown == ID_GM_PLOT_COURSE || g_LastObjectLeftMouseDown == ID_GM_STOP_PLOTTING) //Plot Course /Stop Plotting
		{
			TPacketMapMessage packet(Serial, MM_EDIT, m_PlotState);
			packet.Send();

			GenerateFrame(X, Y);
		}
		else if (g_LastObjectLeftMouseDown == ID_GM_CLEAR_COURSE) //Clear Course
		{
			TPacketMapMessage packet(Serial, MM_CLEAR);
			packet.Send();

			Clear();
		
			GenerateFrame(X, Y);
		}
		else if (m_PlotState && PinOnCursor == NULL)
		{
			if (m_PinTimer > GetTickCount())
			{
				if (g_LastObjectLeftMouseDown >= ID_GM_PIN_LIST_INSERT)
				{
					int idx = g_LastObjectLeftMouseDown - ID_GM_PIN_LIST_INSERT - 1;
					
					TGumpObject *first = (TGumpObject*)Get(idx);

					if (first != NULL)
					{
						int x = X;
						int y = Y;

						int drawX = x + first->X + 18;
						int drawY = y + first->Y + 21;
					
						TGumpObject *next = (TGumpObject*)first->m_Next;

						if (next != NULL)
						{
							int nextDrawX = x + next->X + 20;
							int nextDrawY = y + next->Y + 30;
						
							int checkX = drawX + 2;
							int checkY = drawY + 8;

							if (LineUnderMouse(checkX, checkY, nextDrawX, nextDrawY))
							{
								checkX = checkX - (x + 20);
								checkY = checkY - (y + 29);

								TPacketMapMessage packet(Serial, MM_INSERT, idx + 1, checkX, checkY);
								packet.Send();
					
								TGumpObject *newItem = new TGumpObject(GOT_NONE, 0, 0, checkX, checkY);
								Insert(first, newItem);
							}
						}
					}
				}
				else
				{
					int x = X + 24;
					int y = Y + 32;

					if (UO->PolygonePixelsInXY(x, y, m_Width, m_Height))
					{
						x = g_MouseX - x - 4;
						y = g_MouseY - y - 2;

						TPacketMapMessage packet(Serial, MM_ADD, 0, x, y);
						packet.Send();

						Add(new TGumpObject(GOT_NONE, 0, 0, x, y));
					}
				}
			}
		}
	}

	if (PinOnCursor != NULL)
	{
		int x = X + 24;
		int y = Y + 32;

		int idx = g_LastObjectLeftMouseDown - ID_GM_PIN_LIST - 1;

		if (UO->PolygonePixelsInXY(x, y, m_Width, m_Height))
		{
			x = g_MouseX - (x - 4);
			y = g_MouseY - (y - 2);

			PinOnCursor->X = x;
			PinOnCursor->Y = y;

			TPacketMapMessage packet(Serial, MM_MOVE, idx, x, y);
			packet.Send();
		}
		else
		{
			TPacketMapMessage packet(Serial, MM_REMOVE, idx);
			packet.Send();

			Delete(PinOnCursor);
		}

		PinOnCursor = NULL;
	}
}
//----------------------------------------------------------------------------