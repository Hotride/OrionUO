/****************************************************************************
**
** MinimapGump.cpp
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
TGumpMinimap::TGumpMinimap(DWORD serial, short x, short y, bool minimized)
: TGump(GT_MINIMAP, serial, x, y), m_Count(0), m_Texture(0), m_LastX(0), m_LastY(0)
{
	m_Minimized = minimized;
	GenerateMap();
}
//----------------------------------------------------------------------------
TGumpMinimap::~TGumpMinimap()
{
	if (m_Texture != 0)
	{
		glDeleteTextures(1, &m_Texture);
		m_Texture = 0;
	}
}
//---------------------------------------------------------------------------
void TGumpMinimap::PrepareTextures()
{
	UO->ExecuteGumpPart(0x1392, 2);
}
//---------------------------------------------------------------------------
void TGumpMinimap::GenerateMap()
{
	if (g_Player != NULL)
	{
		m_LastX = g_Player->X;
		m_LastY = g_Player->Y;
	}

	if (m_Texture != 0)
	{
		glDeleteTextures(1, &m_Texture);
		m_Texture = 0;
	}
	
	WORD gumpID = 0x1393 - (int)m_Minimized;
	TIndexObject *io = UO->GetGumpPointer(gumpID);

	if (io == NULL)
		return;

	int gumpWidth = io->Width;
	int gumpHeight = io->Height;

	PWORD data = MulReader.GetGumpPixels(*io);
	
	if (data == NULL)
		return;

	int blockOffsetX = gumpWidth / 4;
	int blockOffsetY = gumpHeight / 4;

	int gumpCenterX = gumpWidth / 2;
	int gumpCenterY = gumpHeight / 2;

	//0xFF080808 - pixel32
	//0x8421 - pixel16

	int minBlockX = (m_LastX - blockOffsetX) / 8 - 1;
	int minBlockY = (m_LastY - blockOffsetY) / 8 - 1;
	int maxBlockX = ((m_LastX + blockOffsetX) / 8) + 1;
	int maxBlockY = ((m_LastY + blockOffsetY) / 8) + 1;

	if (minBlockX < 0)
		minBlockX = 0;

	if (minBlockY < 0)
		minBlockY = 0;

	for (int i = minBlockX; i <= maxBlockX; i++)
	{
		for (int j = minBlockY; j <= maxBlockY; j++)
		{
			int blockIndex = (i * g_MapBlockY[g_CurrentMap]) + j;

			MAP_BLOCK mb = { 0 };
			MapManager->GetRadarMapBlock(i, j, mb);

			IFOR(x, 0, 8)
			{
				IFOR(y, 0, 8)
				{
					int px = (((i * 8) + x) - m_LastX) + gumpCenterX;
					int py = ((j * 8) + y) - m_LastY;

					int gx = px - py;
					int gy = px + py;

					if (gy < 0 || gy >= gumpHeight)
						continue;

					IFOR(i1, 0, 2)
					{
						gx += i1;

						if (gx < 0 || gx >= gumpWidth)
							continue;

						int block = gy * gumpWidth + gx;

						if (data[block] == 0x8421)
						{
							WORD color = mb.Cells[(y * 8) + x].TileID;
							data[block] = 0x8000 | ColorManager->GetRadarColorData(color);
						}
					}
				}
			}
		}
	}

	g_GL.BindTexture16(m_Texture, gumpWidth, gumpHeight, data);

	delete data;
}
//---------------------------------------------------------------------------
void TGumpMinimap::GenerateFrame(int posX, int posY)
{
	if (!g_DrawMode)
	{
		m_FrameRedraw = false;
		m_FrameCreated = false;

		return;
	}

	int PlayerX = g_Player->X;
	int PlayerY = g_Player->Y;

	if (PlayerX != m_LastX || PlayerY != m_LastY || m_Texture == 0)
		GenerateMap();

	TTextureObject *th = UO->ExecuteGump(0x1393 - (int)Minimized);
	if (th == NULL)
		return;

	WORD gumpWidth = th->Width;
	WORD gumpHeight = th->Height;

	WORD gumpCenterX = gumpWidth / 2;
	WORD gumpCenterY = gumpHeight / 2;

	glNewList((GLuint)this, GL_COMPILE);

		g_GL.OldTexture = 0;
		g_GL.Draw(m_Texture, posX, posY, gumpWidth, gumpHeight);
		
		if (m_Count < 6)
		{
			posX += (gumpCenterX - 1);
			posY += (gumpCenterY - 1);

			TGameObject *go = World->m_Items;

			while (go != NULL)
			{
				if (go->Container != 0xFFFFFFFF || go->Graphic >= 0x4000)
				{
					go = (TGameObject*)go->m_Next;
					continue; //Custom multi
				}

				if (go->NPC && !go->IsPlayer())
				{
					WORD color = ConfigManager.GetColorByNotoriety(((TGameCharacter*)go)->Notoriety);

					if (color)
					{
						DWORD pcl = ColorManager->GetPolygoneColor(16, color);
						glColor3ub(GetRValue(pcl), GetGValue(pcl), GetBValue(pcl));

						int x = go->X - PlayerX;
						int y = go->Y - PlayerY;

						int gx = x - y;
						int gy = x + y;

						g_GL.DrawPolygone(posX + gx, posY + gy, 2, 2);
					}
				}

				go = (TGameObject*)go->m_Next;
			}

			glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
			g_GL.DrawPolygone(posX, posY, 2, 2);
		}

	glEndList();

	FrameCreated = true;
}
//----------------------------------------------------------------------------
int TGumpMinimap::Draw(bool &mode)
{
	DWORD index = (DWORD)this;
	static DWORD ticks = GetTickCount();

	int posX = X;
	int posY = Y;

	if (CanBeMoved() && g_LeftMouseDown && g_LastGumpLeftMouseDown == index && !g_LastObjectLeftMouseDown)
	{
		posX += g_MouseX - g_DroppedLeftMouseX;
		posY += g_MouseY - g_DroppedLeftMouseY;
		
		if (mode)
			GenerateFrame(posX, posY);
	}
	else if (mode)
	{
		if (!m_Count || m_Count == 6)
			GenerateFrame(posX, posY);
		else if (g_Player->X != m_LastX || g_Player->Y != m_LastY)
			GenerateFrame(posX, posY);
	}

	if (mode)
	{
		if (!FrameCreated || m_Texture == 0)
			GenerateFrame(posX, posY);

		if (m_Texture != 0)
		{
			glCallList((GLuint)index);

			DrawLocker(posX, posY);
		}

		if (ticks < GetTickCount())
		{
			m_Count++;
			ticks = GetTickCount() + 50;
		}

		if (m_Count > 12)
			m_Count = 0;
	}
	else
	{
		if (UO->GumpPixelsInXY(0x1393 - (int)Minimized, posX, posY))
		{
			g_LastSelectedObject = 0;
			g_LastSelectedGump = index;
		}

		if (g_ShowGumpLocker && UO->PolygonePixelsInXY(posX, posY, 10, 14))
		{
			g_LastSelectedObject = ID_GMM_LOCK_MOVING;
			g_LastSelectedGump = index;
		}
	}

	return 0;
}
//----------------------------------------------------------------------------
void TGumpMinimap::OnLeftMouseUp()
{
	if (g_LastObjectLeftMouseDown == g_LastSelectedObject)
	{
		if (g_LastObjectLeftMouseDown == ID_GMM_LOCK_MOVING)
		{
			LockMoving = !LockMoving;
			g_CancelDoubleClick = true;
		}
	}
}
//----------------------------------------------------------------------------