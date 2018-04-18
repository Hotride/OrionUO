// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
/***********************************************************************************
**
** MultiMap.cpp
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#include "stdafx.h"
//----------------------------------------------------------------------------------
CMultiMap g_MultiMap;
//----------------------------------------------------------------------------------
CMultiMap::CMultiMap()
{
}
//----------------------------------------------------------------------------------
CMultiMap::~CMultiMap()
{
}
//----------------------------------------------------------------------------------
void CMultiMap::LoadMap(CGumpMap *gump, CGUIExternalTexture *mapObject)
{
	WISPFUN_DEBUG("c192_f1");
	WISP_FILE::CMappedFile &file = g_FileManager.m_MultiMap;

	if (!file.Size)
	{
		LOG("MultiMap.rle is not loaded!\n");
		return;
	}

	file.ResetPtr();
	
	int Width = file.ReadInt32LE();
	int Height = file.ReadInt32LE();

	if (Width < 1 || Height < 1)
	{
		LOG("Failed to load bounds from MultiMap.rle\n");
		return;
	}

	int mapSize = gump->Width * gump->Height;
	UCHAR_LIST byteMap(mapSize, 0);

	int startX = gump->StartX / 2;
	int endX = gump->EndX / 2;

	int widthDivizor = endX - startX;

	if (!widthDivizor)
		widthDivizor++;

	int startY = gump->StartY / 2;
	int endY = gump->EndY / 2;

	int heightDivizor = endY - startY;

	if (!heightDivizor)
		heightDivizor++;

	int width = (gump->Width << 8) / widthDivizor;
	int height = (gump->Height << 8) / heightDivizor;

	int x = 0;
	int y = 0;

	int maxPixelValue = 1;

	while (!file.IsEOF())
	{
		BYTE pic = file.ReadUInt8();
		BYTE size = pic & 0x7F;

		bool colored = (bool)(pic & 0x80);

		int startHeight = startY * height;
		int currentHeight = y * height;
		int posY = gump->Width * ((currentHeight - startHeight) >> 8);

		IFOR(i, 0, size)
		{
			if (colored && x >= startX && x < endX && y >= startY && y < endY)
			{
				int position = posY + ((width * (x - startX)) >> 8);

				BYTE &pixel = byteMap[position];

				if (pixel < 0xFF)
				{
					if (pixel == maxPixelValue)
						maxPixelValue++;

					pixel++;
				}
			}

			x++;

			if (x >= Width)
			{
				x = 0;
				y++;
				currentHeight += height;
				posY = gump->Width * ((currentHeight - startHeight) >> 8);
			}

			//if (y >= Height)
			//	break;
		}

		//if (y >= Height)
		//	break;
	}

	if (maxPixelValue >= 1)
	{
		pushort huesData = (pushort)((puchar)g_ColorManager.GetHuesRangePointer() + 30800); // color = 0x015C

		USHORT_LIST colorTable(maxPixelValue);
		int colorOffset = 31 * maxPixelValue;

		IFOR(i, 0, maxPixelValue)
		{
			colorOffset -= 31;
			colorTable[i] = 0x8000 | huesData[colorOffset / maxPixelValue];
		}

		USHORT_LIST wordMap(mapSize);

		IFOR(i, 0, mapSize)
		{
			BYTE &pic = byteMap[i];

			wordMap[i] = (pic ? colorTable[pic - 1] : 0);
		}

		g_GL_BindTexture16(*mapObject->m_Texture, gump->Width, gump->Height, &wordMap[0]);
	}
}
//----------------------------------------------------------------------------------
bool CMultiMap::LoadFacet(CGumpMap *gump, CGUIExternalTexture *mapObject, int facet)
{
	WISPFUN_DEBUG("c192_f2");
	if (facet < 0 || facet > 5)
	{
		LOG("Invalid facet index: %i\n", facet);

		return false;
	}

	WISP_FILE::CMappedFile &file = g_FileManager.m_FacetMul[facet];
	file.ResetPtr();

	if (!file.Size)
	{
		LOG("Facet %i is not loaded!\n", facet);

		return false;
	}

	int mapWidth = file.ReadInt16LE();
	int mapHeight = file.ReadInt16LE();

	//DebugMsg("Facet w:%i h:%i\n", mapWidth, mapHeight);

	int startX = gump->StartX;
	int endX = gump->EndX;

	int startY = gump->StartY;
	int endY = gump->EndY;

	int width = endX - startX;
	int height = endY - startY;

	USHORT_LIST map(width * height);

	IFOR(y, 0, mapHeight)
	{
		int x = 0;
		int colorCount = file.ReadInt32LE() / 3;

		IFOR(i, 0, colorCount)
		{
			int size = file.ReadUInt8();
			ushort color = 0x8000 | file.ReadInt16LE();

			IFOR(j, 0, size)
			{
				if ((x >= startX && x < endX) && (y >= startY && y < endY))
					map[((y - startY) * width) + (x - startX)] = color;

				x++;
			}
		}
	}

	g_GL_BindTexture16(*mapObject->m_Texture, width, height, &map[0]);

	//Или не надо...?
	mapObject->m_Texture->Width = gump->Width;
	mapObject->m_Texture->Height = gump->Height;

	return true;
}
//----------------------------------------------------------------------------------
