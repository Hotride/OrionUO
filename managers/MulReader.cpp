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
TTextureObject *TMulReader::ReadGump(TIndexObject &io)
{
	int DataStart = io.Address;
	PDWORD LookupList = (PDWORD)DataStart;

	TTextureObject *th = new TTextureObject();
	th->Width = io.Width;
	th->Height = io.Height;
	th->Texture = 0;

	int blocksize = io.Width * io.Height;

	PWORD pixels = new WORD[blocksize];
	//memset(&pixels[0], 0, blocksize * 2);

#if UO_ENABLE_TEXTURE_DATA_SAVING == 1
	PBYTE data = new BYTE[blocksize];
#endif

	IFOR(Y, 0, io.Height)
	{
		int GSize = 0;

		if (Y < io.Height - 1)
			GSize = LookupList[Y + 1] - LookupList[Y];
		else
			GSize = (io.Size / 4) - LookupList[Y];

		PGUMP_BLOCK gmul = (PGUMP_BLOCK)(DataStart + LookupList[Y] * 4);
		int X = 0;
		WORD color = io.Color;

		IFOR(i, 0, GSize)
		{
			WORD val = gmul[i].Value;

			if (color)
				val = ColorManager->GetColor16(val, color);

			WORD a = val ? 0x8000 : 0;

			IFOR(j, 0, gmul[i].Run)
			{
				int block = Y * io.Width + X;

				pixels[block] = a | val;

#if UO_ENABLE_TEXTURE_DATA_SAVING == 1
				data[block] = (BYTE)(a ? 1 : 0);
#endif

				X++;
			}
		}
	}

	g_GL.BindTexture16(th->Texture, io.Width, io.Height, pixels);

#if UO_ENABLE_TEXTURE_DATA_SAVING == 1
	th->Data = data;
#endif

	delete pixels;
	return th;
}
//---------------------------------------------------------------------------
bool TMulReader::GumpPixelsInXY(TIndexObject &io, int checkX, int checkY)
{
	TTextureObject *th = io.Texture;

	//if (th == NULL)
	//	return false;

	int width = th->Width;
	int height = th->Height;

	if (checkX < 0 || checkY < 0 || checkX >= width || checkY >= height)
		return false;

	int dataStart = io.Address;
	PDWORD lookupList = (PDWORD)dataStart;

	int gSize = 0;
	if (checkY < height - 1)
		gSize = lookupList[checkY + 1] - lookupList[checkY];
	else
		gSize = (io.Size / 4) - lookupList[checkY];

	PGUMP_BLOCK gmul = (PGUMP_BLOCK)(dataStart + lookupList[checkY] * 4);
	
	int x = 0;
	IFOR(i, 0, gSize)
	{
		IFOR(j, 0, gmul[i].Run)
		{
			if (x == checkX)
				return (gmul[i].Value != 0);

			x++;
		}
	}

	return false;
}
//---------------------------------------------------------------------------
TTextureObject *TMulReader::ReadArt(WORD ID, TIndexObject &io)
{
	TTextureObject *th = new TTextureObject();
	th->Texture = 0;

	DWORD flag = *(PDWORD)io.Address;
	WORD h = 44;
	WORD w = 44;
	PWORD P = (PWORD)io.Address;
	WORD color = io.Color;

	//if (!flag || flag > 0xFFFF) //raw tile
	if (ID < 0x4000) //raw tile
	{
		const int blocksize = 44 * 44;

		WORD pixels[blocksize] = { 0 };
		
#if UO_ENABLE_TEXTURE_DATA_SAVING == 1
	PBYTE data = new BYTE[blocksize];
	memset(&data[0], 0, blocksize);
#endif

		IFOR(i, 0, 22)
		{
			IFOR(j, 22 - (i + 1), (22 - (i + 1)) + (i + 1) * 2)
			{
				WORD val = *P;

				if (color)
					val = ColorManager->GetColor16(val, color);

				WORD a = val ? 0x8000 : 0;
				int block = i * 44 + j;
				pixels[block] = a | val;
#if UO_ENABLE_TEXTURE_DATA_SAVING == 1
				data[block] = (BYTE)(a ? 1 : 0);
#endif
				P++;
			}
		}

		IFOR(i, 0, 22)
		{
			IFOR(j, i, i + (22 - i) * 2)
			{
				WORD val = *P;

				if (color)
					val = ColorManager->GetColor16(val, color);

				WORD a = val ? 0x8000 : 0;
				int block = (i + 22) * 44 + j;
				pixels[block] = a | val;
#if UO_ENABLE_TEXTURE_DATA_SAVING == 1
				data[block] = (BYTE)(a ? 1 : 0);
#endif
				P++;
			}
		}
		
#if UO_ENABLE_TEXTURE_DATA_SAVING == 1
		th->Data = data;
#endif

		g_GL.BindTexture16(th->Texture, 44, 44, pixels);
	}
	else
	{ //run tile
		PWORD ptr = (PWORD)((DWORD)io.Address + 4);

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
		
#if UO_ENABLE_TEXTURE_DATA_SAVING == 1
		PBYTE data = new BYTE[blocksize];
		memset(&data[0], 0, blocksize);
#endif

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

					if (color)
						val = ColorManager->GetColor16(val, color);

					WORD a = val ? 0x8000 : 0;
					int block = Y * w + X + j;
					pixels[block] = a | val;
#if UO_ENABLE_TEXTURE_DATA_SAVING == 1
				data[block] = (BYTE)(a ? 1 : 0);
#endif
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
		
#if UO_ENABLE_TEXTURE_DATA_SAVING == 1
		th->Data = data;
#endif

		delete pixels;
	}

	th->Width = w;
	th->Height = h;

	return th;
}
//---------------------------------------------------------------------------
bool TMulReader::ArtPixelsInXY(WORD ID, TIndexObject &io, int checkX, int checkY)
{
	TTextureObject *th = io.Texture;

	//if (th == NULL)
	//	return false;

	int width = th->Width;
	int height = th->Height;

	if (checkX < 0 || checkY < 0 || checkX >= width || checkY >= height)
		return false;

	DWORD flag = *(PDWORD)io.Address;
	WORD h = 44;
	WORD w = 44;
	PWORD P = (PWORD)io.Address;

	//if (!flag || flag > 0xFFFF) //raw tile
	if (ID < 0x4000) //raw tile
	{
		IFOR(i, 0, 22)
		{
			IFOR(j, 22 - (i + 1), (22 - (i + 1)) + (i + 1) * 2)
			{
				if (i == checkY && j == checkX)
					return ((*P) != 0);

				P++;
			}
		}

		IFOR(i, 0, 22)
		{
			IFOR(j, i, i + (22 - i) * 2)
			{
				if ((i + 22) == checkY && j == checkX)
					return ((*P) != 0);

				P++;
			}
		}
	}
	else //run tile
	{
		PWORD ptr = (PWORD)((DWORD)io.Address + 4);

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

		PWORD lineOffsets = ptr;
		PVOID dataStart = (PVOID)((DWORD)ptr + (h * 2));

		int x = 0;
		int y = 0;
		WORD xOffs = 0;
		WORD run = 0;

		ptr = (PWORD)((DWORD)dataStart + (*lineOffsets));

		while (y < h)
		{
			xOffs = *ptr;
			ptr++;
			run = *ptr;
			ptr++;

			if (xOffs + run >= 2048)
				return false;
			else if (xOffs + run != 0)
			{
				x += xOffs;

				IFOR(j, 0, run)
				{
					if (y == checkY && (x + j) == checkX)
						return ((*ptr) != 0);
					ptr++;
				}

				x += run;
			}
			else
			{
				x = 0;
				y++;
				ptr = (PWORD)((DWORD)dataStart + (lineOffsets[y] * 2));
			}
		}
	}

	return false;
}
//---------------------------------------------------------------------------
TTextureObject *TMulReader::ReadTexture(WORD ID, TIndexObject &io)
{
	TTextureObject *th = new TTextureObject();
	th->Texture = 0;
	WORD color = io.Color;

	WORD w = 64;
	WORD h = 64;
	if (io.Size == 0x2000)
	{
		w = 64;
		h = 64;
	}
	else if (io.Size == 0x8000)
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

	PWORD P = (PWORD)io.Address;
	IFOR(i, 0, h)
	{
		IFOR(j, 0, w)
		{
			WORD val = *P;

			if (color)
				val = ColorManager->GetColor16(val, color);

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
TTextureObject *TMulReader::ReadLight(WORD id, TIndexObject &io)
{
	TTextureObject *th = new TTextureObject();
	th->Texture = NULL;

	PWORD pixels = new WORD[io.Width * io.Height];

	PBYTE p = (PBYTE)io.Address;

	IFOR(i, 0, io.Height)
	{
		IFOR(j, 0, io.Width)
		{
			WORD val = (*p << 10) | (*p << 5) | *p;
			WORD a = val ? 0x8000 : 0;
			int block = (i * io.Width) + j;
			pixels[block] = a | val;
			p++;
		}
	}

	th->Width = io.Width;
	th->Height = io.Height;

	g_GL.BindTexture16(th->Texture, io.Width, io.Height, pixels);

	delete pixels;

	return th;
}
//---------------------------------------------------------------------------