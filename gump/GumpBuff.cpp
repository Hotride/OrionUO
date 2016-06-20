/****************************************************************************
**
** GumpBuff.cpp
**
** Copyright (C) June 2016 Hotride
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
TGumpBuff::TGumpBuff(DWORD serial, short x, short y)
: TGump(GT_BUFF, serial, x, y)
{
	m_Graphic = 0x7580;
}
//----------------------------------------------------------------------------
TGumpBuff::~TGumpBuff()
{
}
//----------------------------------------------------------------------------
void TGumpBuff::PrepareTextures()
{
	UO->ExecuteGump(m_Graphic);

	//Crustal balls
	UO->ExecuteGump(0x7589);
	UO->ExecuteGump(0x7585);

	QFOR(item, m_Items, TGumpBuffObject*)
		UO->ExecuteGump(item->Graphic);
}
//----------------------------------------------------------------------------
void TGumpBuff::UpdateBuffIcons()
{
	DWORD ticks = GetTickCount();

	for (TGumpBuffObject *item = (TGumpBuffObject*)m_Items; item != NULL;)
	{
		TGumpBuffObject *next = (TGumpBuffObject*)item->m_Next;

		int delta = (int)(item->Timer - ticks);

		if (delta < USE_ALPHA_BLENDING_WHEN_TIMER_LESS)
		{
			m_FrameCreated = false;

			if (delta <= 0)
				Delete(item);
			else
			{
				int alpha = item->Alpha;
				int addVal = (USE_ALPHA_BLENDING_WHEN_TIMER_LESS - delta) / ALPHA_CHANGE_KOEFF;

				if (item->DecAlpha)
				{
					alpha -= addVal;

					if (alpha <= MUNIMUM_ICON_ALPHA)
					{
						item->DecAlpha = false;
						alpha = MUNIMUM_ICON_ALPHA;
					}
				}
				else
				{
					alpha += addVal;

					if (alpha >= 255)
					{
						item->DecAlpha = true;
						alpha = 255;
					}
				}

				item->Alpha = alpha;
			}
		}

		item = next;
	}
}
//----------------------------------------------------------------------------
void TGumpBuff::OnToolTip()
{
	if (g_LastSelectedGump == (DWORD)this)
	{
		if (g_LastSelectedObject == ID_GB_NEXT_WINDOW_DIRECTION)
		{
		}
		else if (g_LastSelectedObject)
		{
			DWORD ticks = GetTickCount();
			int index = 0;

			QFOR(item, m_Items, TGumpBuffObject*)
			{
				if (g_LastSelectedObject == ID_GB_BUFF_ITEM + index)
				{
					if (item->TooltipTimer < ticks)
					{
						item->TooltipTimer = ticks + ((item->Timer - ticks) % 1000);
						ToolTip.SeqIndex = 0xFFFFFFFF;

						ToolTip.Set(item->Text, SOT_GAME_OBJECT, g_LastSelectedObject);

						ToolTip.Timer = 0;
						ToolTip.Use = true;
					}
					else
						ToolTip.Set(item->Text, SOT_GAME_OBJECT, g_LastSelectedObject);

					break;
				}

				index++;
			}
		}
		else
		{
			ToolTip.Set(L"Buffs and Debuffs will appear here.", SOT_GAME_OBJECT, g_LastSelectedObject, 120);
		}
	}
}
//---------------------------------------------------------------------------
void TGumpBuff::GetGumpStatus(POINT &ball, POINT &items, bool &useX, bool &decX, bool &decY, POINT &startGump, POINT &endGump)
{
	startGump.x = 0;
	startGump.y = 0;

	UO->GetGumpDimension(m_Graphic, endGump);

	switch (m_Graphic)
	{
		case 0x757F: //v
		{
			ball.x = 0;
			ball.y = 0;
			items.x = 25;
			items.y = 25;
			decY = false;
			decX = false;
			useX = false;
			break;
		}
		case 0x7581: //^
		{
			ball.x = 34;
			ball.y = 78;
			items.x = 7;
			items.y = 52;
			decY = true;
			decX = false;
			useX = false;
			break;
		}
		case 0x7582: //<
		{
			ball.x = 76;
			ball.y = 36;
			items.x = 52;
			items.y = 7;
			decY = false;
			decX = true;
			useX = true;
			break;
		}
		case 0x7580: //>
		default:
		{
			ball.x = -2;
			ball.y = 36;
			items.x = 20;
			items.y = 7;
			decY = false;
			decX = false;
			useX = true;
			break;
		}
	}

	POINT itemsOfs = items;

	QFOR(item, m_Items, TGumpBuffObject*)
	{
		bool moved = false;

		POINT gumpDim = { 0, 0 };
		UO->GetGumpDimension(item->Graphic, gumpDim);

		if (useX)
		{
			if (decX)
				itemsOfs.x -= gumpDim.x + BUFF_ITEM_STEP_OFFSET_X;
			else
				itemsOfs.x += gumpDim.x + BUFF_ITEM_STEP_OFFSET_X;
		}
		else
		{
			if (decY)
				itemsOfs.y -= gumpDim.y + BUFF_ITEM_STEP_OFFSET_Y;
			else
				itemsOfs.y += gumpDim.y + BUFF_ITEM_STEP_OFFSET_Y;
		}
	}

	if (useX)
	{
		if (decX)
			itemsOfs.x -= 20;
		else
			itemsOfs.x += 20;
	}
	else
	{
		if (decY)
			itemsOfs.y -= 20;
		else
			itemsOfs.y += 20;
	}

	if (itemsOfs.x < startGump.x)
		startGump.x = itemsOfs.x;

	if (itemsOfs.y < startGump.y)
		startGump.y = itemsOfs.y;

	if (itemsOfs.x > endGump.x)
		endGump.x = itemsOfs.x;

	if (itemsOfs.y > endGump.y)
		endGump.y = itemsOfs.y;
}
//---------------------------------------------------------------------------
void TGumpBuff::GenerateFrame()
{
	if (!g_DrawMode)
	{
		m_FrameRedraw = false;
		m_FrameCreated = false;

		return;
	}

	CalculateGumpState();

	glNewList((GLuint)this, GL_COMPILE);

		bool decX = false;
		bool decY = false;
		bool useX = true;

		POINT ballCoordinates = { 0, 0 };
		POINT startCoordinates = { 0, 0 };
		POINT startGump = { 0, 0 };
		POINT endGump = { 0, 0 };

		GetGumpStatus(ballCoordinates, startCoordinates, useX, decX, decY, startGump, endGump);

		//g_GL.DrawPolygone(startGump.x - 10, startGump.y - 10, abs(startGump.x) + endGump.x + 20, abs(startGump.y) + endGump.y + 20);

		UO->DrawGump(m_Graphic, 0, 0, 0); //Body

		WORD gumpID = ((g_GumpSelectElement == ID_GB_NEXT_WINDOW_DIRECTION) ? 0x7589 : 0x7585);

		UO->DrawGump(gumpID, 0, ballCoordinates.x, ballCoordinates.y); //Crystal ball

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		DWORD ticks = GetTickCount();

		int ii = 0;

		QFOR(item, m_Items, TGumpBuffObject*)
		{
			if (item->Timer < ticks)
				continue;

			POINT gumpDim = { 0, 0 };
			UO->GetGumpDimension(item->Graphic, gumpDim);

			glColor4ub(0xFF, 0xFF, 0xFF, item->Alpha);

			gumpID = item->Graphic;

			if (ii == 1)
				gumpID = 0x7563;
			if (ii == 2)
				gumpID = 0x7542;
			UO->DrawGump(gumpID, 0, startCoordinates.x, startCoordinates.y); //Buff item
			ii++;
			if (useX)
			{
				if (decX)
					startCoordinates.x -= gumpDim.x + BUFF_ITEM_STEP_OFFSET_X;
				else
					startCoordinates.x += gumpDim.x + BUFF_ITEM_STEP_OFFSET_X;
			}
			else
			{
				if (decY)
					startCoordinates.y -= gumpDim.y + BUFF_ITEM_STEP_OFFSET_Y;
				else
					startCoordinates.y += gumpDim.y + BUFF_ITEM_STEP_OFFSET_Y;
			}
		}

		glDisable(GL_BLEND);
		glColor4ub(0xFF, 0xFF, 0xFF, 0xFF);

	glEndList();

	m_FrameCreated = true;
	m_FrameRedraw = true;
}
//---------------------------------------------------------------------------
void TGumpBuff::CalculateGumpState()
{
	TGump::CalculateGumpState();

	if (g_LeftMouseDown && g_LastGumpLeftMouseDown == (DWORD)this)
	{
		g_GumpMovingOffsetX = g_MouseX - g_DroppedLeftMouseX;
		g_GumpMovingOffsetY = g_MouseY - g_DroppedLeftMouseY;
	}
	else
	{
		g_GumpMovingOffsetX = 0;
		g_GumpMovingOffsetY = 0;
	}

	g_GumpTranslateX = (float)(m_X + g_GumpMovingOffsetX);
	g_GumpTranslateY = (float)(m_Y + g_GumpMovingOffsetY);
}
//----------------------------------------------------------------------------
int TGumpBuff::Draw(bool &mode)
{
	if (!ConfigManager.ToggleBufficonWindow)
		return 0;

	if (m_Graphic < 0x757F || m_Graphic > 0x7582)
		m_Graphic = 0x7580;

	DWORD index = (DWORD)this;

	CalculateGumpState();

	if (mode) //Отрисовка
	{
		if (!m_FrameCreated || g_GumpSelectElement || g_GumpMovingOffsetX || g_GumpMovingOffsetY)
			GenerateFrame();
		else if (m_FrameRedraw)
		{
			GenerateFrame();
			m_FrameRedraw = false;
		}

		glTranslatef(g_GumpTranslateX, g_GumpTranslateY, 0.0f);

		glCallList((GLuint)this);

		glTranslatef(-g_GumpTranslateX, -g_GumpTranslateY, 0.0f);

		DrawLocker();
	}
	else //Выбор объектов
	{
		int oldMouseX = g_MouseX;
		int oldMouseY = g_MouseY;
		g_MouseX -= (int)g_GumpTranslateX;
		g_MouseY -= (int)g_GumpTranslateY;

		int LSG = 0;

		bool decX = false;
		bool decY = false;
		bool useX = true;

		POINT ballCoordinates = { 0, 0 };
		POINT startCoordinates = { 0, 0 };
		POINT startGump = { 0, 0 };
		POINT endGump = { 0, 0 };

		GetGumpStatus(ballCoordinates, startCoordinates, useX, decX, decY, startGump, endGump);

		if ((g_MouseX >= startGump.x && g_MouseX <= endGump.x && g_MouseY >= startGump.y && g_MouseY <= endGump.y))
		{
			g_LastSelectedGump = (DWORD)this;

			if (UO->GumpPixelsInXY(0x7585, ballCoordinates.x, ballCoordinates.y))
				LSG = ID_GB_NEXT_WINDOW_DIRECTION; //Crystal ball
			else
			{
				int index = 0;

				QFOR(item, m_Items, TGumpBuffObject*)
				{
					POINT gumpDim = { 0, 0 };
					UO->GetGumpDimension(item->Graphic, gumpDim);

					if (UO->GumpPixelsInXY(item->Graphic, startCoordinates.x, startCoordinates.y))
					{
						LSG = ID_GB_BUFF_ITEM + index; //Buff item
						break;
					}

					if (useX)
					{
						if (decX)
							startCoordinates.x -= gumpDim.x + BUFF_ITEM_STEP_OFFSET_X;
						else
							startCoordinates.x += gumpDim.x + BUFF_ITEM_STEP_OFFSET_X;
					}
					else
					{
						if (decY)
							startCoordinates.y -= gumpDim.y + BUFF_ITEM_STEP_OFFSET_Y;
						else
							startCoordinates.y += gumpDim.y + BUFF_ITEM_STEP_OFFSET_Y;
					}

					index++;
				}
			}
		}

		if (g_ShowGumpLocker && UO->PolygonePixelsInXY(0, 0, 10, 14))
			LSG = ID_GB_LOCK_MOVING;

		g_MouseX = oldMouseX;
		g_MouseY = oldMouseY;

		if (LSG != 0)
			g_LastSelectedObject = LSG; //Если что-то нашлось - выбираем

		return LSG;
	}

	return 0;
}
//----------------------------------------------------------------------------
void TGumpBuff::OnLeftMouseUp()
{
	//Проверим валидность переменных
	if (g_LastObjectLeftMouseDown != g_LastSelectedObject || !g_LastObjectLeftMouseDown || !g_LastSelectedGump)
		return;

	if (g_LastObjectLeftMouseDown == ID_GB_NEXT_WINDOW_DIRECTION)
	{
		switch (m_Graphic)
		{
			case 0x7580:
			{
				m_Graphic = 0x7582;
				break;
			}
			case 0x7581:
			{
				m_Graphic = 0x757F;
				break;
			}
			case 0x7582:
			{
				m_Graphic = 0x7581;
				break;
			}
			case 0x757F:
			default:
			{
				m_Graphic = 0x7580;
				break;
			}
		}
	}
	else if (g_LastObjectLeftMouseDown == ID_GB_LOCK_MOVING)
		m_LockMoving = !m_LockMoving;
}
//----------------------------------------------------------------------------