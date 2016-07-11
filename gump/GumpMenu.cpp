/****************************************************************************
**
** MenuGump.cpp
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
TGumpMenu::TGumpMenu(DWORD serial, short x, short y, string text)
: TGump(GT_MENU, serial, x, y), m_Text(text), m_Count(0), m_CurrentIndex(1),
m_CurrentItemOffset(0), m_CurrentOffset(0)
{
}
//---------------------------------------------------------------------------
TGumpMenu::~TGumpMenu()
{
}
//---------------------------------------------------------------------------
void TGumpMenu::PrepareTextures()
{
	UO->ExecuteGump(0x0910);

	QFOR(go, m_Items, TGumpMenuObject*)
		UO->ExecuteStaticArt(go->Graphic);
}
//---------------------------------------------------------------------------
void TGumpMenu::CalculateGumpState()
{
	TGump::CalculateGumpState();

	if (g_LeftMouseDown && g_LastGumpLeftMouseDown == (DWORD)this && (!g_LastObjectLeftMouseDown || g_LastObjectLeftMouseDown > ID_GM_RIGHT))
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
//---------------------------------------------------------------------------
void TGumpMenu::GenerateFrame()
{
	if (!g_DrawMode)
	{
		m_FrameRedraw = false;
		m_FrameCreated = false;

		return;
	}

	CalculateGumpState();

	int posXOffs = 0;
	int pageOffs = 0;
	
	if (m_CurrentOffset)
	{
		if (m_CurrentOffset < 0) //Prev
		{
			posXOffs = GUMP_MENU_PIXEL_STEP * m_CurrentOffset;
			pageOffs = 0;
		}
		else //Next
		{
			posXOffs = -(m_CurrentItemOffset - (m_CurrentOffset * GUMP_MENU_PIXEL_STEP));
			pageOffs = -1;
		}
	}

	glNewList((GLuint)this, GL_COMPILE);
	
		UO->DrawGump(0x0910, 0, 0, 0);

		FontManager->DrawA(1, m_Text.c_str(), 0x0386, 39, 18, 200, TS_LEFT, UOFONT_FIXED);
		
		g_GL.Scissor((int)g_GumpTranslateX + 40, (int)g_GumpTranslateY + 42, 217, 49);

		int idx = 1;
		string itemText = "";

		ColorizerShader->Use();

		QFOR(go, m_Items, TGumpMenuObject*)
		{
			if (idx == m_CurrentIndex + pageOffs)
			{
				int w = 0;

				IFOR(i, 0, 7)
				{
					if (go == NULL || w >= 254) //214 + 40 = 254 (40 - offset)
						break;

					POINT p = {0, 0};
					UO->GetArtDimension(go->Graphic + 0x4000, p);

					if (p.x && p.y)
					{
						int offsH = p.y;

						if (offsH >= 47)
							offsH = 0;
						else
							offsH = ((47 - p.y) / 2);

						UO->DrawStaticArtInContainer(go->Graphic, go->Color, 40 + w + posXOffs, 47 + offsH);

						if (g_LastSelectedObject == idx + ID_GM_ITEMS)
							itemText = go->GetText();

						w += p.x;
					}

					idx++;
					go = (TGumpMenuObject*)go->m_Next;
				}

				break;
			}

			idx++;

			if (go == NULL)
				break;
		}

		UnuseShader();

		glDisable(GL_SCISSOR_TEST);

		if (itemText.length())
			FontManager->DrawA(1, itemText.c_str(), 0x0386, 42, 105, 200, TS_LEFT, UOFONT_FIXED);

	glEndList();

	m_FrameRedraw = true;
	m_FrameCreated = true;
}
//----------------------------------------------------------------------------
int TGumpMenu::Draw(bool &mode)
{
	DWORD index = (DWORD)this;

	CalculateGumpState();

	if (m_CurrentOffset)
	{
		m_FrameCreated = false;

		if (m_CurrentOffset < 0) //Prev
			m_CurrentOffset++;
		else //Next
			m_CurrentOffset--;
	}

	if (mode)
	{
		if (!m_CurrentOffset && g_LastGumpLeftMouseDown == index)
		{
			MoveItems();
			m_FrameCreated = false;
		}

		if (!m_FrameCreated || (index == g_LastSelectedGump && g_LastSelectedObject >= 10) || g_GumpMovingOffsetX || g_GumpMovingOffsetY)
			GenerateFrame();
		else if (m_FrameRedraw)
		{
			GenerateFrame();
			m_FrameRedraw = false;
		}

		glTranslatef(g_GumpTranslateX, g_GumpTranslateY, 0.0f);

		glCallList((GLuint)index);

		glTranslatef(-g_GumpTranslateX, -g_GumpTranslateY, 0.0f);
	}
	else
	{
		int oldMouseX = g_MouseX;
		int oldMouseY = g_MouseY;
		g_MouseX -= (int)g_GumpTranslateX;
		g_MouseY -= (int)g_GumpTranslateY;

		if (UO->GumpPixelsInXY(0x0910, 0, 0))
		{
			g_LastSelectedObject = 0;
			g_LastSelectedGump = index;
		}

		if (m_CurrentOffset || g_LastSelectedGump != index)
		{
			g_MouseX = oldMouseX;
			g_MouseY = oldMouseY;

			return 0;
		}
		
		int LSG = 0;

		if (UO->PolygonePixelsInXY(23, 59, 8, 16))
			LSG = ID_GM_LEFT;
		else if (UO->PolygonePixelsInXY(261, 59, 8, 16))
			LSG = ID_GM_RIGHT;
		
		if (UO->PolygonePixelsInXY(40, 47, 217, 49))
		{
			int idx = 1;

			QFOR(go, m_Items, TGumpMenuObject*)
			{
				if (idx == m_CurrentIndex)
				{
					int w = 0;

					IFOR(i, 0, 7)
					{
						if (go == NULL || w >= 214)
							break;

						POINT mp = {0, 0};
						UO->GetArtDimension(go->Graphic + 0x4000, mp);

						if (mp.x && mp.y)
						{
							if (UO->PolygonePixelsInXY(40 + w, 47, mp.x, 49))
							{
								LSG = idx + ID_GM_ITEMS;

								break;
							}

							w += mp.x;
						}

						idx++;
						go = (TGumpMenuObject*)go->m_Next;
					}
				}

				if (LSG)
					break;

				idx++;

				if (go == NULL)
					break;
			}
		}

		g_MouseX = oldMouseX;
		g_MouseY = oldMouseY;

		return LSG;
	}

	return 0;
}
//----------------------------------------------------------------------------
void TGumpMenu::OnLeftMouseUp()
{
	if (g_LastObjectLeftMouseDown != g_LastSelectedObject || !g_LastObjectLeftMouseDown /*|| gump->GetMinimized()*/ || !g_LastSelectedGump || m_CurrentOffset)
		return;

	MoveItems();
}
//----------------------------------------------------------------------------
bool TGumpMenu::OnLeftMouseDoubleClick()
{
	if (!g_LastSelectedGump || m_CurrentOffset)
		return false;

	if (g_LastObjectLeftMouseDown >= ID_GM_ITEMS)
	{
		SendMenuResponse(g_LastObjectLeftMouseDown - ID_GM_ITEMS);

		return true;
	}

	return false;
}
//----------------------------------------------------------------------------
void TGumpMenu::MoveItems()
{
	if (g_LastObjectLeftMouseDown == ID_GM_LEFT) //Prev
	{
		TGumpMenuObject *go = (TGumpMenuObject*)m_Items;
		TGumpMenuObject *prev = NULL;
		int idx = 1;

		for (; go != NULL; go = (TGumpMenuObject*)go->m_Next, idx++)
		{
			if (idx == m_CurrentIndex)
				break;

			prev = go;
		}

		m_CurrentItemOffset = 0;

		if (idx == m_CurrentIndex && go != NULL && go != m_Items && prev != NULL)
		{
			POINT p = {0, 0};
			UO->GetArtDimension(prev->Graphic + 0x4000, p);
			m_CurrentItemOffset = p.x;
			m_CurrentOffset += -(p.x / GUMP_MENU_PIXEL_STEP);
		}

		m_CurrentIndex--;
	}
	else if (g_LastObjectLeftMouseDown == ID_GM_RIGHT) //Next
	{
		TGumpMenuObject *go = (TGumpMenuObject*)m_Items;
		int idx = 1;

		for (; go != NULL; go = (TGumpMenuObject*)go->m_Next, idx++)
		{
			if (idx == m_CurrentIndex)
				break;
		}

		m_CurrentItemOffset = 0;

		if (idx == m_CurrentIndex && go != NULL && go->m_Next != NULL)
		{
			POINT p = {0, 0};
			UO->GetArtDimension(go->Graphic + 0x4000, p);
			m_CurrentItemOffset = p.x;
			m_CurrentOffset = p.x / GUMP_MENU_PIXEL_STEP;
		}

		m_CurrentIndex++;
	}

	if (m_CurrentIndex < 1)
	{
		m_CurrentItemOffset = 0;
		m_CurrentOffset = 0;
		m_CurrentIndex = 1;
	}

	if (m_CurrentIndex > m_Count)
	{
		m_CurrentItemOffset = 0;
		m_CurrentOffset = 0;
		m_CurrentIndex = m_Count;
	}
}
//----------------------------------------------------------------------------