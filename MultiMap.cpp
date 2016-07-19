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

	PBYTE map = new BYTE[gump->Width * gump->Height];
	memset(&map[0], 0, gump->Width * gump->Height);

	int startX = m_Width * gump->StartX;
	int calcStartX = startX / 5120;
	int calcStartX2 = calcStartX;

	int endX = m_Width * gump->EndX;
	int calcEndX = endX / 5120;
	int calcEndX2 = endX / 5120 - calcStartX;

	if (endX / 5120 == calcStartX)
		calcEndX2 = 1;

	int startY = m_Height * gump->StartY / 4096;
	int endY = m_Height * gump->EndY;
	int calcEndY = endY / 4096;
	int calcEndY2 = calcEndY - startY;

	if (calcEndY == startY)
		calcEndY2 = 1;

	int width2 = m_Width;

	int gumpWidth = (gump->Width << 8) / calcEndX2;
	int gumpHeight = (gump->Height << 8) / calcEndY2;

	int x = 0;
	int y = 0;

	int v17 = 1;
	int v44 = 1;

	while (address < end)
	{
		BYTE pic = *address++;

		BYTE size = pic & 0x7F;
		BYTE size2 = pic & 0x7F;

		if (pic & 0x80)
		{
			if (size > 0)
			{
				BYTE size3 = pic & 0x7F;

				int v20 = y * gumpHeight;
				int v21 = startY * gumpHeight;

				for (int i = startY * gumpHeight;; v21 = i)
				{
					if (x >= calcStartX && x < calcEndX && y >= startY && y < calcEndY)
					{
						int v22 = gump->Width * ((v20 - v21) >> 8) + (gumpWidth * (x - calcStartX) >> 8);
						BYTE v23 = *(map + v22);
						if (v23 < 255)
						{
							++*(map + v22);
							if (v23 == v44)
								++v44;
							calcStartX = calcStartX2;
						}
					}
					++x;
					if (x == width2)
					{
						x = 0;
						++y;
						v20 += gumpHeight;
					}
					--size3;
					if (!size3)
						break;
				}

				m_Width = width2;
				v17 = v44;
				size = size2;
			}
		}
		else
		{
			x += size;
			if (x >= m_Width)
			{
				x -= m_Width;
				++y;
			}
		}
	}

	PWORD huesData = (PWORD)ColorManager->GetHuesRangePointer() + 30592;

	PWORD mapw = new WORD[gump->Width * gump->Height];
	PWORD mapwPtr = mapw;

	PBYTE mapEnd = map + (gump->Width * gump->Height);
	PBYTE mapPtr = map;

	while (mapPtr < mapEnd)
	{
		BYTE pic = *mapPtr++;
		WORD val = 0;
		if (pic)
			val = 0x8000 | ColorManager->GetColor16(/*pic & 0x1F*/8, 0x015B);
		*mapwPtr++ = val;
	}

	GLuint tex = 0;
	g_GL.BindTexture16(tex, gump->Width, gump->Height, mapw);
	gump->Texture = tex;

	delete map;
	delete mapw;
/*
    if ( v17 >= 1 )
    {
      v24 = &v71;
      v25 = v17;
      v26 = 31 * v17 - 31;
      do
      {
        v27 = v26;
        v26 -= 31;
        v24 += 4;
        --v25;
        *((_DWORD *)v24 - 1) = (unsigned __int16)word_C87110[v27 / v44];
      }
      while ( v25 );
    }
*/



























/*
  v1 = this;
  v2 = *(_DWORD *)(this + 84);
  *(_DWORD *)(this + 200) = 0;
  if ( v2 == 5021 )
  {
    sub_42C700(5170, *(_DWORD *)(*(_DWORD *)(this + 60) + 4), 0);
    v3 = malloc(*(_DWORD *)(v1 + 300) * *(_DWORD *)(v1 + 296));
    v4 = *(_DWORD *)(v1 + 300) * *(_DWORD *)(v1 + 296);
    lpMem = v3;
    v47 = v3;
    memset(v3, 0, v4);
    v5 = operator new(0x14u);
    size2? = v5;
    v72 = 0;
    if ( v5 )
    {
      v6 = sub_47F9D0(aMultimap_rle);
      v50 = sub_4D2AD0(v5, v6);
    }
    else
    {
      v50 = 0;
    }
    v72 = -1;
    file = sub_4D2C10((int)v50);
    m_Height = *(_DWORD *)(file + 4);
    m_Width = *(_DWORD *)file;
    mapSize = *(_DWORD *)file * *(_DWORD *)(file + 4);
    StartX = m_Width * *(_DWORD *)(v1 + 280);
    dataPtr = file + 8;
    filePosition = 0;
    y = 0;
    calcStartX = StartX / 5120;
    x = 0;
    EndX = m_Width * *(_DWORD *)(v1 + 288);
    StartY = m_Height * *(_DWORD *)(v1 + 284) / 4096;
    m_Width2 = m_Width;
    EndY = m_Height * *(_DWORD *)(v1 + 292);
    calcEndX = EndX / 5120;
    calcStartX2 = calcStartX;
    calcEndX2 = EndX / 5120 - calcStartX;
    calcEndY = EndY / 4096;
    if ( EndX / 5120 == calcStartX )
      calcEndX2 = 1;
    gumpWidth? = (*(_DWORD *)(v1 + 296) << 8) / calcEndX2;
    calcEndY2 = calcEndY - StartY;
    if ( calcEndY == StartY )
      calcEndY2 = 1;
    v17 = 1;
    v44 = 1;
    gumpHeight = (*(_DWORD *)(v1 + 300) << 8) / calcEndY2;
    if ( mapSize > 0 )
    {
      do
      {
        pic = *(_BYTE *)dataPtr++;
        size = pic & 0x7F;
        size2? = (void *)(pic & 0x7F);
        if ( pic & 0x80 )
        {
          if ( size > 0 )
          {
            size3? = pic & 0x7F;
            v20 = y * gumpHeight;
            v21 = StartY * gumpHeight;
            for ( i = StartY * gumpHeight; ; v21 = i )
            {
              if ( x >= calcStartX && x < calcEndX && y >= StartY && y < calcEndY )
              {
                v22 = *(_DWORD *)(v1 + 296) * ((v20 - v21) >> 8) + (gumpWidth? * (x - calcStartX) >> 8);
                v23 = *((_BYTE *)lpMem + v22);
                if ( v23 < 255 )
                {
                  ++*((_BYTE *)lpMem + v22);
                  if ( v23 == v44 )
                    ++v44;
                  calcStartX = calcStartX2;
                }
              }
              ++x;
              if ( x == m_Width2 )
              {
                x = 0;
                ++y;
                v20 += gumpHeight;
              }
              --size3?;
              if ( !size3? )
                break;
            }
            m_Width = m_Width2;
            v17 = v44;
            size = (int)size2?;
          }
        }
        else
        {
          x += size;
          if ( x >= m_Width )
          {
            x -= m_Width;
            ++y;
          }
        }
        filePosition += size;
      }
      while ( filePosition < mapSize );
    }
    if ( v17 >= 1 )
    {
      v24 = &v71;
      v25 = v17;
      v26 = 31 * v17 - 31;
      do
      {
        v27 = v26;
        v26 -= 31;
        v24 += 4;
        --v25;
        *((_DWORD *)v24 - 1) = (unsigned __int16)word_C87110[v27 / v44];
      }
      while ( v25 );
    }
*/
}
//----------------------------------------------------------------------------
void TMultiMap::LoadMapEx(TGumpMap *gump)
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

	PWORD map = new WORD[width * height];
	
	int x = 0;
	int y = 0;

	//WORD color = 0x1461;
	WORD color = 0x1881;

	WORD data_color = 0x8000; //ColorManager->Color16To32(color);

	if (width > 200 || height > 200)
	{
		data_color = 0x9881; // 0x00295A73; //0x004284A5; //ColorManager->Color16To32(color);
		//data_color = (color & 0x1F) | (2 * (color & 0x7FE0));
		gump->DoubleImage = true;
	}

	//data_color = (0xFF << 24) | (GetBValue(data_color) << 16) | (GetGValue(data_color) << 8) | GetRValue(data_color);

	while (address < end)
	{
		BYTE pic = *address;
		address++;

		BYTE size = pic & 0x7F;

		WORD color = 0;
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
	g_GL.BindTexture16(tex, width, height, map);
	gump->Texture = tex;

	delete map;
}
//----------------------------------------------------------------------------