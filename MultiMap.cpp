/****************************************************************************
**
** MultiMap.cpp
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

TMultiMap *MultiMap = NULL;
//----------------------------------------------------------------------------
TMultiMap::TMultiMap()
{
	PBYTE address = (PBYTE)FileManager.MultiMap.Address;
	
	m_Width = *(PDWORD)address;
	address += 4;

	m_Height = *(PDWORD)address;
	address += 4;

	if (m_Width < 1 || m_Height < 1)
	{
		TPRINT("Failed to load bounds from MultiMap.rle\n");
		m_Width = 0;
		m_Height = 0;
	}
}
//----------------------------------------------------------------------------
TMultiMap::~TMultiMap()
{
}
//----------------------------------------------------------------------------
void TMultiMap::LoadMap(TGumpMap *gump)
{
	if (m_Width < 1 || m_Height < 1)
	{
		TPRINT("Failed to load bounds from MultiMap.rle\n");
		return;
	}

	PBYTE address = (PBYTE)FileManager.MultiMap.Address;
	PBYTE end = address + FileManager.MultiMap.Size;
	
	address += 8;
	
	int sx = gump->StartX / 2;
	int sy = gump->StartY / 2;

	int ex = gump->EndX / 2;
	int ey = gump->EndY / 2;

	int width = ex - sx;
	int height = ey - sy;

	PDWORD map = new DWORD[width * height * 4];
	
	int x = 0;
	int y = 0;

	//WORD color = 0x1461;
	WORD color = 0x1881;

	DWORD data_color = 0; //ColorManager->Color16To32(color);

	if (width > 200 || height > 200)
	{
		data_color = 0x00295A73; //0x004284A5; //ColorManager->Color16To32(color);
		//data_color = (color & 0x1F) | (2 * (color & 0x7FE0));
		gump->DoubleImage = true;
	}

	data_color = (0xFF << 24) | (GetBValue(data_color) << 16) | (GetGValue(data_color) << 8) | GetRValue(data_color);

	while (address < end)
	{
		BYTE pic = *address;
		address++;

		BYTE size = pic & 0x7F;

		DWORD color = 0;
		if (pic & 0x80)
			color = data_color;

		IFOR(i, 0, size)
		{
			if ((x >= sx && x < ex) && (y >= sy && y < ey))
				map[((y - sy) * width) + (x - sx)] = color;

			x++;

			if (x >= m_Width)
			{
				x = 0;
				y++;
			}

			if (y >= m_Height)
				break;
		}

		if (y >= m_Height)
			break;
	}

	GLuint tex = 0;
	g_GL.BindTexture(tex, width, height, map);
	gump->Texture = tex;

	delete map;
}
//----------------------------------------------------------------------------