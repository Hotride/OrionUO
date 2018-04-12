// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
/***********************************************************************************
**
** UOFileReader.cpp
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#include "stdafx.h"

UOFileReader g_UOFileReader;
//---------------------------------------------------------------------------
//----------------------------------CUOFileReader-------------------------------
//---------------------------------------------------------------------------
/*!
Получить массив пикселей гампа
@param [__in] io Ссылка на данные о гампе
@return Массив пикселей или NULL
*/
USHORT_LIST UOFileReader::GetGumpPixels(CIndexObject &io)
{
	WISPFUN_DEBUG("c148_f1");
	size_t dataStart = io.Address;
	puint lookupList = (puint)dataStart;

	int blocksize = io.Width * io.Height;

	USHORT_LIST pixels;

	if (!blocksize)
	{
		LOG("UOFileReader::GetGumpPixels bad size:%i, %i\n", io.Width, io.Height);
		return pixels;
	}

	pixels.resize(blocksize);

	if (pixels.size() != blocksize)
	{
		LOG("Allocation pixels memory for GetGumpPixels failed (want size: %i)\n", blocksize);
		return pixels;
	}

	ushort color = io.Color;

	IFOR(y, 0, io.Height)
	{
		int gSize = 0;

		if (y < io.Height - 1)
			gSize = lookupList[y + 1] - lookupList[y];
		else
			gSize = (io.DataSize / 4) - lookupList[y];

		PGUMP_BLOCK gmul = (PGUMP_BLOCK)(dataStart + lookupList[y] * 4);
		int pos = (int)y * io.Width;

		IFOR(i, 0, gSize)
		{
			ushort val = gmul[i].Value;

			if (color && val)
				val = g_ColorManager.GetColor16(val, color);

			ushort a = (val ? 0x8000 : 0) | val;

			int count = gmul[i].Run;

			IFOR(j, 0, count)
				pixels[pos++] = a;
		}
	}

	return pixels;
}
//---------------------------------------------------------------------------
/*!
Прочитать гамп и сгенерировать текстуру
@param [__in] io Ссылка на данные о гампе
@return Ссылка на данные о текстуре
*/
CGLTexture *UOFileReader::ReadGump(CIndexObject &io)
{
	WISPFUN_DEBUG("c148_f2");
	CGLTexture *th = NULL;

	USHORT_LIST pixels = GetGumpPixels(io);

	if (pixels.size())
	{
		th = new CGLTexture();
		g_GL_BindTexture16(*th, io.Width, io.Height, &pixels[0]);
	}

	return th;
}
//---------------------------------------------------------------------------
USHORT_LIST UOFileReader::GetArtPixels(ushort id, CIndexObject &io, bool run, short &width, short &height)
{
	WISPFUN_DEBUG("c148_f4");

	uint flag = *(puint)io.Address;
	pushort P = (pushort)io.Address;
	ushort color = io.Color;

	USHORT_LIST pixels;

	if (!run) //raw tile
	{
		width = 44;
		height = 44;
		pixels.resize(44 * 44, 0);

		IFOR(i, 0, 22)
		{
			int start = (22 - ((int)i + 1));
			int pos = (int)i * 44 + start;
			int end = start + ((int)i + 1) * 2;

			IFOR(j, start, end)
			{
				ushort val = *P++;

				if (color && val)
					val = g_ColorManager.GetColor16(val, color);

				if (val)
					val = 0x8000 | val;

				pixels[pos++] = val;
			}
		}

		IFOR(i, 0, 22)
		{
			int pos = ((int)i + 22) * 44 + (int)i;
			int end = (int)i + (22 - (int)i) * 2;

			IFOR(j, i, end)
			{
				ushort val = *P++;

				if (color && val)
					val = g_ColorManager.GetColor16(val, color);

				if (val)
					val = 0x8000 | val;

				pixels[pos++] = val;
			}
		}
	}
	else //run tile
	{
		int stumpIndex = 0;

		if (g_Orion.IsTreeTile(id, stumpIndex))
		{
			pushort ptr = NULL;

			if (stumpIndex == g_StumpHatchedID)
			{
				width = g_StumpHatchedWidth;
				height = g_StumpHatchedHeight;
				ptr = (pushort)g_StumpHatched;
			}
			else
			{
				width = g_StumpWidth;
				height = g_StumpHeight;
				ptr = (pushort)g_Stump;
			}

			int blocksize = width * height;

			pixels.resize(blocksize);

			if (pixels.size() != blocksize)
			{
				LOG("Allocation pixels memory for ReadArt::LandTile failed (want size: %i)\n", blocksize);
				pixels.clear();
				return pixels;
			}

			IFOR(i, 0, blocksize)
				pixels[i] = ptr[i];
		}
		else
		{
			pushort ptr = (pushort)(io.Address + 4);

			width = *ptr;
			if (!width || width >= 1024)
			{
				LOG("UOFileReader::ReadArt bad width:%i\n", width);
				return pixels;
			}

			ptr++;

			height = *ptr;

			if (!height || (height * 2) > 5120)
			{
				LOG("UOFileReader::ReadArt bad height:%i\n", height);
				return pixels;
			}

			ptr++;

			pushort lineOffsets = ptr;
			puchar dataStart = (puchar)ptr + (height * 2);

			int X = 0;
			int Y = 0;
			ushort XOffs = 0;
			ushort Run = 0;

			int blocksize = width * height;

			pixels.resize(blocksize, 0);

			if (pixels.size() != blocksize)
			{
				LOG("Allocation pixels memory for ReadArt::StaticTile failed (want size: %i)\n", blocksize);
				pixels.clear();
				return pixels;
			}

			ptr = (pushort)(dataStart + (lineOffsets[0] * 2));

			while (Y < height)
			{
				XOffs = *ptr;
				ptr++;
				Run = *ptr;
				ptr++;
				if (XOffs + Run >= 2048)
				{
					LOG("UOFileReader::ReadArt bad offset:%i, %i\n", XOffs, Run);
					pixels.clear();
					return pixels;
				}
				else if (XOffs + Run != 0)
				{
					X += XOffs;
					int pos = Y * width + X;

					IFOR(j, 0, Run)
					{
						ushort val = *ptr++;

						if (val)
						{
							if (color)
								val = g_ColorManager.GetColor16(val, color);

							val = 0x8000 | val;
						}

						pixels[pos++] = val;
					}

					X += Run;
				}
				else
				{
					X = 0;
					Y++;
					ptr = (pushort)(dataStart + (lineOffsets[Y] * 2));
				}
			}

			if ((id >= 0x2053 && id <= 0x2062) || (id >= 0x206A && id <= 0x2079)) //Убираем рамку (если это курсор мышки)
			{
				IFOR(i, 0, width)
				{
					pixels[i] = 0;
					pixels[(height - 1) * width + i] = 0;
				}

				IFOR(i, 0, height)
				{
					pixels[i * width] = 0;
					pixels[i * width + width - 1] = 0;
				}
			}
			else if (g_Orion.IsCaveTile(id))
			{
				IFOR(y, 0, height)
				{
					int startY = (y ? -1 : 0);
					int endY = (y + 1 < height ? 2 : 1);

					IFOR(x, 0, width)
					{
						ushort &pixel = pixels[y * width + x];

						if (pixel)
						{
							int startX = (x ? -1 : 0);
							int endX = (x + 1 < width ? 2 : 1);

							IFOR(i, startY, endY)
							{
								int currentY = (int)y + (int)i;

								IFOR(j, startX, endX)
								{
									int currentX = (int)x + (int)j;

									ushort &currentPixel = pixels[currentY * width + currentX];

									if (!currentPixel)
										pixel = 0x8000;
								}
							}
						}
					}
				}
			}
		}
	}

	return pixels;
}
//---------------------------------------------------------------------------
/*!
Прочитать арт и сгенерировать текстуру
@param [__in] ID Индекс арта
@param [__in] io Ссылка на данные о арте
@return Ссылка на данные о текстуре
*/
CGLTexture *UOFileReader::ReadArt(ushort id, CIndexObject &io, bool run)
{
	WISPFUN_DEBUG("c148_f2");
	CGLTexture *texture = NULL;
	short width = 0;
	short height = 0;

	USHORT_LIST pixels = GetArtPixels(id, io, run, width, height);

	if (pixels.size())
	{
		int minX = width;
		int minY = height;
		int maxX = 0;
		int maxY = 0;

		if (!run)
		{
			maxX = 44;
			maxY = 44;
			bool allBlack = true;
			int pos = 0;

			IFOR(i, 0, 44)
			{
				IFOR(j, 0, 44)
				{
					if (pixels[pos++])
					{
						i = 44;
						allBlack = false;
						break;
					}
				}
			}

			((CIndexObjectLand*)&io)->AllBlack = allBlack;

			if (allBlack)
			{
				IFOR(i, 0, 22)
				{
					int start = (22 - ((int)i + 1));
					int pos = (int)i * 44 + start;
					int end = start + ((int)i + 1) * 2;

					IFOR(j, start, end)
						pixels[pos++] = 0x8000;
				}

				IFOR(i, 0, 22)
				{
					int pos = ((int)i + 22) * 44 + (int)i;
					int end = (int)i + (22 - (int)i) * 2;

					IFOR(j, i, end)
						pixels[pos++] = 0x8000;
				}
			}
		}
		else
		{
			int pos = 0;

			IFOR(y, 0, height)
			{
				IFOR(x, 0, width)
				{
					if (pixels[pos++])
					{
						minX = min(minX, int(x));
						maxX = max(maxX, int(x));
						minY = min(minY, int(y));
						maxY = max(maxY, int(y));
					}
				}
			}
		}

		texture = new CGLTexture();
		g_GL_BindTexture16(*texture, width, height, &pixels[0]);

		texture->ImageOffsetX = minX;
		texture->ImageOffsetY = minY;

		texture->ImageWidth = maxX - minX;
		texture->ImageHeight = maxY - minY;
	}

	return texture;
}
//---------------------------------------------------------------------------
/*!
Прочитать текстуру ландшафта и сгенерировать тексруту
@param [__in] io Ссылка на данные о текстуре ландшафта
@return Ссылка на данные о текстуре
*/
CGLTexture *UOFileReader::ReadTexture(CIndexObject &io)
{
	WISPFUN_DEBUG("c148_f7");
	CGLTexture *th = new CGLTexture();
	th->Texture = 0;
	ushort color = io.Color;

	ushort w = 64;
	ushort h = 64;

	if (io.DataSize == 0x2000)
	{
		w = 64;
		h = 64;
	}
	else if (io.DataSize == 0x8000)
	{
		w = 128;
		h = 128;
	}
	else
	{
		LOG("UOFileReader::ReadTexture bad data size\n", io.DataSize);
		delete th;
		return NULL;
	}

	USHORT_LIST pixels(w * h);

	pushort P = (pushort)io.Address;

	IFOR(i, 0, h)
	{
		int pos = (int)i * w;

		IFOR(j, 0, w)
		{
			ushort val = *P++;

			if (color)
				val = g_ColorManager.GetColor16(val, color);

			pixels[pos + j] = 0x8000 | val;
		}
	}

	g_GL.IgnoreHitMap = true;
	g_GL_BindTexture16(*th, w, h, &pixels[0]);
	g_GL.IgnoreHitMap = false;

	return th;
}
//---------------------------------------------------------------------------
/*!
Прочитать освещение и сгенерировать текстуру
@param [__in] io Ссылка на данные о освещении
@return Ссылка на данные о текстуре
*/
CGLTexture *UOFileReader::ReadLight(CIndexObject &io)
{
	WISPFUN_DEBUG("c148_f8");
	CGLTexture *th = new CGLTexture();
	th->Texture = NULL;

	USHORT_LIST pixels(io.Width * io.Height);

	puchar p = (puchar)io.Address;

	IFOR(i, 0, io.Height)
	{
		int pos = (int)i * io.Width;

		IFOR(j, 0, io.Width)
		{
			ushort val = (*p << 10) | (*p << 5) | *p;
			p++;
			pixels[pos + j] = (val ? 0x8000 : 0) | val;
		}
	}

	g_GL.IgnoreHitMap = true;
	g_GL_BindTexture16(*th, io.Width, io.Height, &pixels[0]);
	g_GL.IgnoreHitMap = false;

	return th;
}
//---------------------------------------------------------------------------
