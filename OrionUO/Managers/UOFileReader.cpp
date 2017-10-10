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
	int dataStart = io.Address;
	puint lookupList = (puint)dataStart;

	int blocksize = io.Width * io.Height;

	USHORT_LIST pixels;

	if (!blocksize)
		return pixels;

	pixels.resize(blocksize);

	ushort color = io.Color;

	IFOR(y, 0, io.Height)
	{
		int gSize = 0;

		if (y < io.Height - 1)
			gSize = lookupList[y + 1] - lookupList[y];
		else
			gSize = (io.DataSize / 4) - lookupList[y];

		PGUMP_BLOCK gmul = (PGUMP_BLOCK)(dataStart + lookupList[y] * 4);
		int pos = y * io.Width;

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
/*!
Прочитать арт и сгенерировать текстуру
@param [__in] ID Индекс арта
@param [__in] io Ссылка на данные о арте
@return Ссылка на данные о текстуре
*/
CGLTexture *UOFileReader::ReadArt(const ushort &id, CIndexObject &io, const bool &run)
{
	WISPFUN_DEBUG("c148_f4");
	CGLTexture *th = new CGLTexture();
	th->Texture = 0;

	uint flag = *(puint)io.Address;
	int h = 44;
	int w = 44;
	pushort P = (pushort)io.Address;
	ushort color = io.Color;

	//if (!flag || flag > 0xFFFF) //raw tile
	if (!run) //raw tile
	{
		const int blocksize = 44 * 44;

		bool allBlack = true;
		ushort pixels[blocksize] = { 0 };
		
		IFOR(i, 0, 22)
		{
			int start = (22 - (i + 1));
			int pos = i * 44 + start;
			int end = start + (i + 1) * 2;

			IFOR(j, start, end)
			{
				ushort val = *P++;

				if (color && val)
					val = g_ColorManager.GetColor16(val, color);

				if (val)
				{
					allBlack = false;
					val = 0x8000 | val;
				}

				pixels[pos++] = val;
			}
		}

		IFOR(i, 0, 22)
		{
			int pos = (i + 22) * 44 + i;
			int end = i + (22 - i) * 2;

			IFOR(j, i, end)
			{
				ushort val = *P++;

				if (color && val)
					val = g_ColorManager.GetColor16(val, color);

				if (val)
				{
					allBlack = false;
					val = 0x8000 | val;
				}

				pixels[pos++] = val;
			}
		}

		((CIndexObjectLand*)&io)->AllBlack = allBlack;

		if (allBlack)
		{
			IFOR(i, 0, 22)
			{
				int start = (22 - (i + 1));
				int pos = i * 44 + start;
				int end = start + (i + 1) * 2;

				IFOR(j, start, end)
					pixels[pos++] = 0x8000;
			}

			IFOR(i, 0, 22)
			{
				int pos = (i + 22) * 44 + i;
				int end = i + (22 - i) * 2;

				IFOR(j, i, end)
					pixels[pos++] = 0x8000;
			}
		}

		g_GL_BindTexture16(*th, 44, 44, &pixels[0]);
	}
	else //run tile
	{
		USHORT_LIST pixels;
		int stumpIndex = 0;

		if (g_Orion.IsTreeTile(id, stumpIndex))
		{
			pushort ptr = NULL;

			if (stumpIndex == g_StumpHatchedID)
			{
				w = g_StumpHatchedWidth;
				h = g_StumpHatchedHeight;
				ptr = (pushort)g_StumpHatched;
			}
			else
			{
				w = g_StumpWidth;
				h = g_StumpHeight;
				ptr = (pushort)g_Stump;
			}

			int blocksize = w * h;

			pixels.resize(blocksize);

			IFOR(i, 0, blocksize)
				pixels[i] = ptr[i];
		}
		else
		{
			pushort ptr = (pushort)(io.Address + 4);

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

			pushort lineOffsets = ptr;
			puchar dataStart = (puchar)ptr + (h * 2);

			int X = 0;
			int Y = 0;
			ushort XOffs = 0;
			ushort Run = 0;

			int blocksize = w * h;

			pixels.resize(blocksize, 0);

			ptr = (pushort)(dataStart + (lineOffsets[0] * 2));

			while (Y < h)
			{
				XOffs = *ptr;
				ptr++;
				Run = *ptr;
				ptr++;
				if (XOffs + Run >= 2048)
				{
					delete th;
					return NULL;
				}
				else if (XOffs + Run != 0)
				{
					X += XOffs;
					int pos = Y * w + X;

					IFOR(j, 0, Run)
					{
						ushort val = *ptr++;

						if (color && val)
							val = g_ColorManager.GetColor16(val, color);

						val = (val ? 0x8000 : 0) | val;
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
				IFOR(i, 0, w)
				{
					pixels[i] = 0;
					pixels[(h - 1) * w + i] = 0;
				}

				IFOR(i, 0, h)
				{
					pixels[i * w] = 0;
					pixels[i * w + w - 1] = 0;
				}
			}
			else if (g_Orion.IsCaveTile(id))
			{
				IFOR(y, 0, h)
				{
					int startY = (y ? -1 : 0);
					int endY = (y + 1 < h ? 2 : 1);

					IFOR(x, 0, w)
					{
						ushort &pixel = pixels[y * w + x];

						if (pixel)
						{
							int startX = (x ? -1 : 0);
							int endX = (x + 1 < w ? 2 : 1);

							IFOR(i, startY, endY)
							{
								int currentY = y + i;

								IFOR(j, startX, endX)
								{
									int currentX = x + j;

									ushort &currentPixel = pixels[currentY * w + currentX];

									if (!currentPixel)
										pixel = 0x8000;
								}
							}
						}
					}
				}
			}
		}

		g_GL_BindTexture16(*th, w, h, &pixels[0]);
	}

	return th;
}
//---------------------------------------------------------------------------
/*!
Прочитать арт и вычислить реальные пииксельные границы картинки
@param [__in] io Ссылка на данные о арте
@param [__out] r Структура с габаритами на выходе
@return Ссылка на данные о текстуре
*/
WISP_GEOMETRY::CRect UOFileReader::ReadStaticArtPixelDimension(CIndexObject &io)
{
	WISPFUN_DEBUG("c148_f5");
	WISP_GEOMETRY::CRect r;

	int stumpIndex = 0;

	ushort h = 44;
	ushort w = 44;

	int minX = 0;
	int minY = 0;
	int maxX = 0;
	int maxY = 0;

	if (g_Orion.IsTreeTile(io.ID, stumpIndex))
	{
		pushort ptr = NULL;

		if (stumpIndex == g_StumpHatchedID)
		{
			w = g_StumpHatchedWidth;
			h = g_StumpHatchedHeight;
			ptr = (pushort)g_StumpHatched;
		}
		else
		{
			w = g_StumpWidth;
			h = g_StumpHeight;
			ptr = (pushort)g_Stump;
		}
	}
	else
	{
		uint flag = *(PDWORD)io.Address;
		pushort P = (pushort)io.Address;

		pushort ptr = (pushort)(io.Address + 4);

		w = *ptr;
		if (!w || w >= 1024)
			return r;

		ptr++;

		h = *ptr;

		if (!h || (h * 2) > 5120)
			return r;

		ptr++;

		pushort lineOffsets = ptr;
		PVOID dataStart = (PVOID)((uint)ptr + (h * 2));

		int X = 0;
		int Y = 0;
		ushort XOffs = 0;
		ushort Run = 0;

		ptr = (pushort)((uint)dataStart + (lineOffsets[0] * 2));

		minX = w;
		minY = h;

		while (Y < h)
		{
			XOffs = *ptr;
			ptr++;
			Run = *ptr;
			ptr++;
			if (XOffs + Run >= 2048)
				return r;
			else if (XOffs + Run != 0)
			{
				X += XOffs;

				IFOR(j, 0, Run)
				{
					if (*ptr)
					{
						int testX = X + j;

						if (testX < minX)
							minX = testX;

						if (testX > maxX)
							maxX = testX;

						if (Y < minY)
							minY = Y;

						if (Y > maxY)
							maxY = Y;
					}

					ptr++;
				}

				X += Run;
			}
			else
			{
				X = 0;
				Y++;
				ptr = (pushort)((uint)dataStart + (lineOffsets[Y] * 2));
			}
		}
	}

	r.Position = WISP_GEOMETRY::CPoint2Di(minX, minY);
	r.Size = WISP_GEOMETRY::CSize(maxX - minX, maxY - minY);

	return r;
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
		delete th;
		return NULL;
	}

	USHORT_LIST pixels(w * h);

	pushort P = (pushort)io.Address;

	IFOR(i, 0, h)
	{
		int pos = i * w;

		IFOR(j, 0, w)
		{
			ushort val = *P++;

			if (color)
				val = g_ColorManager.GetColor16(val, color);

			pixels[pos + j] = 0x8000 | val;
		}
	}

	g_GL_BindTexture16(*th, w, h, &pixels[0]);

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
		int pos = i * io.Width;

		IFOR(j, 0, io.Width)
		{
			ushort val = (*p << 10) | (*p << 5) | *p;
			p++;
			pixels[pos + j] = (val ? 0x8000 : 0) | val;
		}
	}

	g_GL_BindTexture16(*th, io.Width, io.Height, &pixels[0]);

	return th;
}
//---------------------------------------------------------------------------
