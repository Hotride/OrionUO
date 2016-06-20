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
	DWORD selected = ((g_LastSelectedGump == (DWORD)this) ? g_LastSelectedObject : 0);

	if (selected)
	{
		if (selected == ID_GB_NEXT_WINDOW_DIRECTION)
		{
		}
		else
		{
			DWORD ticks = GetTickCount();
			int index = 0;

			QFOR(item, m_Items, TGumpBuffObject*)
			{
				if (selected == ID_GB_BUFF_ITEM + index)
				{
					if (item->TooltipTimer < ticks)
					{
						item->TooltipTimer = ticks + ((item->Timer - ticks) % 1000);
						ToolTip.SeqIndex = 0;
					}

					ToolTip.Set(item->Text, SOT_GAME_OBJECT, selected);

					break;
				}

				index++;
			}
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
		case 0x757F: //^
		{
			decY = true;
			decX = false;
			useX = false;
			break;
		}
		case 0x7580: //>
		{
			decY = false;
			decX = false;
			useX = true;
			break;
		}
		case 0x7581: //v
		{
			decY = false;
			decX = false;
			useX = false;
			break;
		}
		case 0x7582: //<
		{
			decY = false;
			decX = true;
			useX = true;
			break;
		}
		default:
			break;
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
				itemsOfs.x -= gumpDim.x;
		}
		else if (decY)
			itemsOfs.y -= gumpDim.y;

		int addValX = 0;
		int addValY = 0;

		if (useX)
		{
			if (decX)
				addValX = -BUFF_ITEM_STEP_OFFSET_X;
			else
				addValX = gumpDim.x + BUFF_ITEM_STEP_OFFSET_X;
		}
		else
		{
			if (decY)
				addValY = -BUFF_ITEM_STEP_OFFSET_Y;
			else
				addValY = gumpDim.y + BUFF_ITEM_STEP_OFFSET_Y;
		}

		itemsOfs.x += addValX;
		itemsOfs.y += addValY;
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

		g_GL.DrawPolygone(startGump.x - 10, startGump.y - 10, abs(startGump.x) + endGump.x + 20, abs(startGump.y) + endGump.y + 20);

		UO->DrawGump(m_Graphic, 0, 0, 0); //Body

		UO->DrawGump(m_Graphic, 0, ballCoordinates.x, ballCoordinates.y); //Crystal ball

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		DWORD ticks = GetTickCount();

		QFOR(item, m_Items, TGumpBuffObject*)
		{
			if (item->Timer < ticks)
				continue;

			POINT gumpDim = { 0, 0 };
			UO->GetGumpDimension(item->Graphic, gumpDim);

			if (useX)
			{
				if (decX)
					startCoordinates.x -= gumpDim.x;
			}
			else if (decY)
				startCoordinates.y -= gumpDim.y;

			glColor4ub(0xFF, 0xFF, 0xFF, item->Alpha);
			UO->DrawGump(item->Graphic, 0, startCoordinates.x, startCoordinates.y); //Buff item

			int decValX = 0;
			int decValY = 0;

			if (useX)
			{
				if (decX)
					decValX = -BUFF_ITEM_STEP_OFFSET_X;
				else
					decValX = gumpDim.x + BUFF_ITEM_STEP_OFFSET_X;
			}
			else
			{
				if (decY)
					decValY = -BUFF_ITEM_STEP_OFFSET_Y;
				else
					decValY = gumpDim.y + BUFF_ITEM_STEP_OFFSET_Y;
			}

			startCoordinates.x += decValX;
			startCoordinates.y += decValY;
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

			if (UO->GumpPixelsInXY(m_Graphic, ballCoordinates.x, ballCoordinates.y))
				LSG = ID_GB_NEXT_WINDOW_DIRECTION; //Crystal ball
			else
			{
				int index = 0;

				QFOR(item, m_Items, TGumpBuffObject*)
				{
					POINT gumpDim = { 0, 0 };
					UO->GetGumpDimension(item->Graphic, gumpDim);

					if (useX)
					{
						if (decX)
							startCoordinates.x -= gumpDim.x;
					}
					else if (decY)
						startCoordinates.y -= gumpDim.y;

					if (UO->GumpPixelsInXY(m_Graphic, ballCoordinates.x, ballCoordinates.y))
					{
						LSG = ID_GB_BUFF_ITEM + index; //Buff item
						break;
					}

					int decValX = 0;
					int decValY = 0;

					if (useX)
					{
						if (decX)
							decValX = -BUFF_ITEM_STEP_OFFSET_X;
						else
							decValX = gumpDim.x + BUFF_ITEM_STEP_OFFSET_X;
					}
					else
					{
						if (decY)
							decValY = -BUFF_ITEM_STEP_OFFSET_Y;
						else
							decValY = gumpDim.y + BUFF_ITEM_STEP_OFFSET_Y;
					}

					startCoordinates.x += decValX;
					startCoordinates.y += decValY;

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
		m_Graphic++;

		if (m_Graphic > 0x7582)
			m_Graphic = 0x757F;
	}
	else if (g_LastObjectLeftMouseDown == ID_GB_LOCK_MOVING)
		m_LockMoving = !m_LockMoving;
}
//----------------------------------------------------------------------------