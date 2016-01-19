/****************************************************************************
**
** MulReader.cpp
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
//---------------------------------------------------------------------------
#include "stdafx.h"

TMulReader MulReader;
//---------------------------------------------------------------------------
TTextureObject *TMulReader::ReadGump(DWORD Address, DWORD Size, WORD Width, WORD Height)
{
	int DataStart = Address;
	PDWORD LookupList = (PDWORD)DataStart;

	TTextureObject *th = new TTextureObject();
	th->Width = Width;
	th->Height = Height;
	th->Texture = 0;

	int blocksize = Width * Height;

	PWORD pixels = new WORD[blocksize];
	//memset(&pixels[0], 0, blocksize * 4);
	IFOR(Y, 0, Height)
	{
		int GSize = 0;

		if (Y < Height - 1)
			GSize = LookupList[Y + 1] - LookupList[Y];
		else
			GSize = (Size / 4) - LookupList[Y];

		PGUMP_BLOCK gmul = (PGUMP_BLOCK)(DataStart + LookupList[Y] * 4);
		int X = 0;
		IFOR(i, 0, GSize)
		{
			WORD val = gmul[i].Value;
			WORD a = val ? 0x8000 : 0;

			IFOR(j, 0, gmul[i].Run)
			{
				int block = Y * Width + X;

				pixels[block] = a | val;

				X++;
			}
		}
	}

	g_GL.BindTexture16(th->Texture, Width, Height, pixels);

	delete pixels;
	return th;
}
//---------------------------------------------------------------------------
bool TMulReader::GumpPixelsInXY(DWORD Address, DWORD Size, WORD Width, WORD Height, int CheckX, int CheckY)
{
	if (CheckX < 0 || CheckY < 0 || CheckX >= Width || CheckY >= Height)
		return false;

	int DataStart = Address;
	PDWORD LookupList = (PDWORD)DataStart;

	int GSize = 0;
	if (CheckY < Height - 1)
		GSize = LookupList[CheckY + 1] - LookupList[CheckY];
	else
		GSize = (Size / 4) - LookupList[CheckY];

	PGUMP_BLOCK gmul = (PGUMP_BLOCK)(DataStart + LookupList[CheckY] * 4);
	
	int X = 0;
	IFOR(i, 0, GSize)
	{
		IFOR(j, 0, gmul[i].Run)
		{
			if (X == CheckX) return (gmul[i].Value != 0);
			X++;
		}
	}

	return false;
}
//---------------------------------------------------------------------------
TTextureObject *TMulReader::ReadArt(WORD ID, DWORD Address, DWORD Size)
{
	TTextureObject *th = new TTextureObject();
	th->Texture = 0;

	DWORD flag = *(PDWORD)Address;
	WORD h = 44;
	WORD w = 44;
	PWORD P = (PWORD)Address;

	//if (!flag || flag > 0xFFFF) //raw tile
	if (ID < 0x4000) //raw tile
	{
		int blocksize = 44 * 44;

		PWORD pixels = new WORD[blocksize];
		memset(&pixels[0], 0, blocksize * 2);

		IFOR(i, 0, 22)
		{
			IFOR(j, 22 - (i + 1), (22 - (i + 1)) + (i + 1) * 2)
			{
				WORD val = *P;
				WORD a = val ? 0x8000 : 0;
				int block = i * 44 + j;
				pixels[block] = a | val;
				P++;
			}
		}

		IFOR(i, 0, 22)
		{
			IFOR(j, i, i + (22 - i) * 2)
			{
				WORD val = *P;
				WORD a = val ? 0x8000 : 0;
				int block = (i + 22) * 44 + j;
				pixels[block] = a | val;
				P++;
			}
		}

		g_GL.BindTexture16(th->Texture, 44, 44, pixels);

		delete pixels;
	}
	else
	{ //run tile
		PWORD ptr = (PWORD)((DWORD)Address + 4);

		w = *ptr;
		if (!w || w >= 1024)
		{
			delete th;
			return NULL;
		}

		ptr++;

		h = *ptr;

		if (!h || (h * 2) > 5120)
		{
			delete th;
			return NULL;
		}

		ptr++;

		PWORD LineOffsets = ptr;
		PVOID DataStart = (PVOID)((DWORD)ptr + (h * 2));

		int X = 0;
		int Y = 0;
		WORD XOffs = 0;
		WORD Run = 0;

		int blocksize = w * h;

		PWORD pixels = new WORD[blocksize];
		memset(&pixels[0], 0, blocksize * 2);

		ptr = (PWORD)((DWORD)DataStart + (*LineOffsets));
		ID -= 0x4000;

		while (Y < h)
		{
			XOffs = *ptr;
			ptr++;
			Run = *ptr;
			ptr++;
			if (XOffs + Run >= 2048)
			{
				delete pixels;
				delete th;
				return NULL;
			}
			else if (XOffs + Run != 0)
			{
				X += XOffs;

				IFOR(j, 0, Run)
				{
					WORD val = *ptr;
					WORD a = val ? 0x8000 : 0;
					int block = Y * w + X + j;
					pixels[block] = a | val;
					ptr++;
				}

				X += Run;
			}
			else
			{
				X = 0;
				Y++;
				ptr = (PWORD)((DWORD)DataStart + (LineOffsets[Y] * 2));
			}
		}
		if ((ID >= 0x2053 && ID <= 0x2062) || (ID >= 0x206A && ID <= 0x2079)) //Убираем рамку (если это курсор мышки)
		{
			IFOR(i, 0, w)
			{
				pixels[i] = 0;
				pixels[(h - 1)* w + i] = 0;
			}

			IFOR(i, 0, h)
			{
				pixels[i * w] = 0;
				pixels[i* w + w - 1] = 0;
			}
		}

		g_GL.BindTexture16(th->Texture, w, h, pixels);

		delete pixels;
	}

	th->Width = w;
	th->Height = h;

	return th;
}
//---------------------------------------------------------------------------
bool TMulReader::ArtPixelsInXY(WORD ID, DWORD Address, DWORD Size, WORD Width, WORD Height, int CheckX, int CheckY)
{
	if (CheckX < 0 || CheckY < 0 || CheckX >= Width || CheckY >= Height)
		return false;

	DWORD flag = *(PDWORD)Address;
	WORD h = 44;
	WORD w = 44;
	PWORD P = (PWORD)Address;

	//if (!flag || flag > 0xFFFF) //raw tile
	if (ID < 0x4000) //raw tile
	{
		IFOR(i, 0, 22)
		{
			IFOR(j, 22 - (i + 1), (22 - (i + 1)) + (i + 1) * 2)
			{
				if (i == CheckY && j == CheckX) return ((*P) != 0);
				P++;
			}
		}

		IFOR(i, 0, 22)
		{
			IFOR(j, i, i + (22 - i) * 2)
			{
				if ((i + 22) == CheckY && j == CheckX) return ((*P) != 0);
				P++;
			}
		}
	}
	else //run tile
	{
		PWORD ptr = (PWORD)((DWORD)Address + 4);

		w = *ptr;
		if (!w || w >= 1024)
		{
			return false;
		}

		ptr++;

		h = *ptr;

		if (!h || (h * 2) > 5120)
		{
			return false;
		}

		ptr++;

		PWORD LineOffsets = ptr;
		PVOID DataStart = (PVOID)((DWORD)ptr + (h * 2));

		int X = 0;
		int Y = 0;
		WORD XOffs = 0;
		WORD Run = 0;

		ptr = (PWORD)((DWORD)DataStart + (*LineOffsets));

		while (Y < h)
		{
			XOffs = *ptr;
			ptr++;
			Run = *ptr;
			ptr++;
			if (XOffs + Run >= 2048)
				return false;
			else if (XOffs + Run != 0)
			{
				X += XOffs;
				IFOR(j, 0, Run)
				{
					if (Y == CheckY && (X + j) == CheckX)
						return ((*ptr) != 0);
					ptr++;
				}
				X += Run;
			}
			else
			{
				X = 0;
				Y++;
				ptr = (PWORD)((DWORD)DataStart + (LineOffsets[Y] * 2));
			}
		}
	}

	return false;
}
//---------------------------------------------------------------------------
TTextureObject *TMulReader::ReadTexture(WORD ID, DWORD Address, DWORD Size)
{
	TTextureObject *th = new TTextureObject();
	th->Texture = 0;

	WORD w = 64;
	WORD h = 64;
	if (Size == 0x2000)
	{
		w = 64;
		h = 64;
	}
	else if (Size == 0x8000)
	{
		w = 128;
		h = 128;
	}
	else
	{
		delete th;
		return NULL;
	}

	int blocksize = w * h;
	PWORD pixels = new WORD[blocksize];

	PWORD P = (PWORD)Address;
	IFOR(i, 0, h)
	{
		IFOR(j, 0, w)
		{
			WORD val = *P;
			WORD a = val ? 0x8000 : 0;
			int block = i * w + j;
			pixels[block] = a | val;
			P++;
		}
	}

	th->Width = w;
	th->Height = h;

	g_GL.BindTexture16(th->Texture, w, h, pixels);

	delete pixels;

	return th;
}
//---------------------------------------------------------------------------
TTextureObject *TMulReader::ReadLight(WORD id, DWORD address, DWORD size, WORD width, WORD height)
{
	TTextureObject *th = new TTextureObject();
	th->Texture = NULL;

	PWORD pixels = new WORD[width * height];

	PBYTE p = (PBYTE)address;

	IFOR(i, 0, height)
	{
		IFOR(j, 0, width)
		{
			WORD val = *p * 8;
			WORD a = val ? 0x8000 : 0;
			int block = (i * width) + j;
			pixels[block] = a | val;
			p++;
		}
	}

	th->Width = width;
	th->Height = height;

	g_GL.BindTexture16(th->Texture, width, height, pixels);

	delete pixels;

	return th;
}
//---------------------------------------------------------------------------