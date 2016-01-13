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

	TGumpMenuObject *go = (TGumpMenuObject*)m_Items;

	while (go != NULL)
	{
		UO->ExecuteStaticArt(go->Graphic);

		go = (TGumpMenuObject*)go->m_Next;
	}
}
//---------------------------------------------------------------------------
void TGumpMenu::GenerateFrame(int posX, int posY)
{
	if (!g_DrawMode)
	{
		FrameRedraw = false;
		FrameCreated = false;

		return;
	}

	DWORD index = (DWORD)this;

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

	glNewList((GLuint)index, GL_COMPILE);
	
		UO->DrawGump(0x0910, 0, posX, posY);

		FontManager->DrawA(1, m_Text.c_str(), 0x0386, posX + 39, posY + 18, 200, TS_LEFT, UOFONT_FIXED);
		
		g_GL.ViewPort(posX + 40, posY + 42, 217, 49);

		TGumpMenuObject *go = (TGumpMenuObject*)m_Items;
		int idx = 1;
		string itemText = "";

		ColorizerShader->Use();

		while (go != NULL)
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

						UO->DrawStaticArtInContainer(go->Graphic, go->Color, posX + 40 + w + posXOffs, posY + 47 + offsH);

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
			if (go != NULL)
				go = (TGumpMenuObject*)go->m_Next;
		}

		UnuseShader();

		g_GL.RestorePort();

		if (itemText.length())
			FontManager->DrawA(1, itemText.c_str(), 0x0386, posX + 42, posY + 105, 200, TS_LEFT, UOFONT_FIXED);

	glEndList();

	FrameRedraw = true;
	FrameCreated = true;
}
//----------------------------------------------------------------------------
int TGumpMenu::Draw(bool &mode)
{
	DWORD index = (DWORD)this;

	int posX = X;
	int posY = Y;

	bool needRedraw = false;
	
	if (m_CurrentOffset)
	{
		needRedraw = true;

		if (m_CurrentOffset < 0) //Prev
			m_CurrentOffset++;
		else //Next
			m_CurrentOffset--;
	}

	if (g_LeftMouseDown && g_LastGumpLeftMouseDown == index && (!g_LastObjectLeftMouseDown || g_LastObjectLeftMouseDown > ID_GM_RIGHT))
	{
		posX += (g_MouseX - g_DroppedLeftMouseX);
		posY += (g_MouseY - g_DroppedLeftMouseY);

		if (mode)
			FrameRedraw = true;
	}

	if (mode)
	{
		if (!m_CurrentOffset && g_LastGumpLeftMouseDown == index)
		{
			MoveItems();
			GenerateFrame(posX, posY);
		}
		else if ((index == g_LastSelectedGump && g_LastSelectedObject >= 10) || needRedraw)
			GenerateFrame(posX, posY);
		else if (FrameRedraw)
		{
			GenerateFrame(posX, posY);
			FrameRedraw = false;
		}
	}


	if (mode)
	{
		if (!FrameCreated)
			GenerateFrame(posX, posY);

		glCallList((GLuint)index);
	}
	else
	{
		if (UO->GumpPixelsInXY(0x0910, posX, posY))
		{
			g_LastSelectedObject = 0;
			g_LastSelectedGump = index;
		}

		if (m_CurrentOffset || g_LastSelectedGump != index)
			return 0;
		
		int LSG = 0;

		RECT rc = {0, 0, 8, 16};
		POINT p = {g_MouseX - (posX + 23), g_MouseY - (posY + 59)};
		if (PtInRect(&rc, p))
			LSG = ID_GM_LEFT;

		p.x = g_MouseX - (posX + 261);
		p.y = g_MouseY - (posY + 59);
		if (PtInRect(&rc, p))
			LSG = ID_GM_RIGHT;
		
		RECT mrc = {0, 0, 217, 49};
		p.x = g_MouseX - (posX + 40);
		p.y = g_MouseY - (posY + 47);

		if (PtInRect(&mrc, p))
		{
			TGumpMenuObject *go = (TGumpMenuObject*)m_Items;
			int idx = 1;

			while (go != NULL)
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
							RECT irc = {0, 0, mp.x, 49};
							POINT ip = {g_MouseX - (posX + 40 + w), g_MouseY - (posY + 47)};

							if (PtInRect(&irc, ip))
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
				if (go != NULL)
					go = (TGumpMenuObject*)go->m_Next;
			}
		}

		if (LSG != 0)
			g_LastSelectedObject = LSG;

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

		while (go != NULL)
		{
			if (idx == m_CurrentIndex)
				break;

			idx++;
			prev = go;
			go = (TGumpMenuObject*)go->m_Next;
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

		while (go != NULL)
		{
			if (idx == m_CurrentIndex)
				break;

			idx++;
			go = (TGumpMenuObject*)go->m_Next;
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