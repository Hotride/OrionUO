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
PWORD TGumpMinimap::GetTextureData(WORD id, int &width, int &height)
{
	TIndexObject *io = UO->GetGumpPointer(id);

	if (io == NULL)
		return NULL;

	DWORD size = io->Size;
	int DataStart = io->Address;
	PDWORD LookupList = (PDWORD)DataStart;

	width = io->Width;
	height = io->Height;

	int blocksize = width * height;

	PWORD pixels = new WORD[blocksize];
	//memset(&pixels[0], 0, blocksize * 2);

	IFOR(y, 0, height)
	{
		int GSize = 0;

		if (y < height - 1)
			GSize = LookupList[y + 1] - LookupList[y];
		else
			GSize = (size / 4) - LookupList[y];

		PGUMP_BLOCK gmul = (PGUMP_BLOCK)(DataStart + LookupList[y] * 4);
		int x = 0;
		IFOR(i, 0, GSize)
		{
			WORD val = gmul[i].Value;
			WORD a = val ? 0x8000 : 0;

			IFOR(j, 0, gmul[i].Run)
			{
				int block = y * width + x;

				pixels[block] = a | val;

				x++;
			}
		}
	}

	return pixels;
}
//---------------------------------------------------------------------------
void TGumpMinimap::GenerateMap()
{
	/*if (!g_DrawMode)
	{
		m_FrameRedraw = false;
		m_FrameCreated = false;

		return;
	}*/

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
	
	int gumpWidth = 0;
	int gumpHeight = 0;

	PWORD data = GetTextureData(0x1393 - (int)m_Minimized, gumpWidth, gumpHeight);
	
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

			TMapBlock *mapBlock = MapManager->GetBlock(blockIndex);
			MAP_BLOCK mb = { 0 };

			if (mapBlock == NULL)
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
							WORD color = (mapBlock != NULL ? mapBlock->GetRadarColor(x, y) : mb.Cells[(y * 8) + x].TileID);
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

	WORD blockOffsetX = gumpWidth / 4;
	WORD blockOffsetY = gumpHeight / 4;

	WORD gumpCenterX = gumpWidth / 2;
	WORD gumpCenterY = gumpHeight / 2;

	glNewList((GLuint)this, GL_COMPILE);

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
					int X = go->X - PlayerX;
					int Y = go->Y - PlayerY;

					int gx = X - Y;
					int gy = X + Y;

					WORD color = ConfigManager.GetColorByNotoriety(((TGameCharacter*)go)->Notoriety);;

					if (color)
					{
						WORD cell = 5;
						DWORD pcl = ColorManager->GetPolygoneColor(cell, color);
						pcl = (0xFF << 24) | (GetBValue(pcl) << 16) | (GetGValue(pcl) << 8) | GetRValue(pcl);
						
						g_GL.DrawPolygone(pcl, (float)(posX + gx), (float)(posY + gy), 2, 2);
					}
				}

				go = (TGameObject*)go->m_Next;
			}

			g_GL.DrawPolygone(0xFF7F7F7F, (float)(posX), (float)(posY), 2, 2);

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

		glCallList((GLuint)index);

		DrawLocker(posX, posY);

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