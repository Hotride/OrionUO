/****************************************************************************
**
** ScreenshotBuilder.cpp
**
** Copyright (C) October 2015 Hotride
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

TScreenshotBuilder g_ScreenshotBuilder;
//---------------------------------------------------------------------------
TScreenshotBuilder::TScreenshotBuilder()
{
}
//---------------------------------------------------------------------------
TScreenshotBuilder::~TScreenshotBuilder()
{
}
//---------------------------------------------------------------------------
void TScreenshotBuilder::SaveScreen()
{
	string path = FilePath("snapshots");
	CreateDirectoryA(path.c_str(), NULL);

	SYSTEMTIME st;
	GetLocalTime(&st);

	char buf[100] = { 0 };

	sprintf_s(buf, "\\snapshot_d(%i.%i.%i)_t(%i.%i.%i_%i).bmp", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);

	path += buf;

	int width = g_ClientWidth;
	int height = g_ClientHeight;

	BITMAPINFO bmInfo;
	BITMAPFILEHEADER bmFileHeader;

	bmInfo.bmiHeader.biSize = 40;
	bmInfo.bmiHeader.biWidth = width;
	bmInfo.bmiHeader.biHeight = height;
	bmInfo.bmiHeader.biPlanes = 1;
	bmInfo.bmiHeader.biBitCount = 32;
	bmInfo.bmiHeader.biCompression = 0;
	bmInfo.bmiHeader.biSizeImage = (width * height * 4) + 40 + 14;
	bmInfo.bmiHeader.biXPelsPerMeter = 0;
	bmInfo.bmiHeader.biYPelsPerMeter = 0;
	bmInfo.bmiHeader.biClrUsed = 0;
	bmInfo.bmiHeader.biClrImportant = 0;

	bmFileHeader.bfType = 0x4d42;
	bmFileHeader.bfSize = (width * height * 4) + 40 + 14;
	bmFileHeader.bfReserved1 = 0;
	bmFileHeader.bfReserved2 = 0;
	bmFileHeader.bfOffBits = 54;

	PDWORD pixels = GetScenePixels(0, 0, width, height);

	FILE *file = fopen(path.c_str(), "wb");

	if (file != NULL)
	{
		fwrite(&bmFileHeader, sizeof(bmFileHeader), 1, file);
		fwrite(&bmInfo, sizeof(bmInfo), 1, file);
		fwrite(&pixels[0], bmInfo.bmiHeader.biSizeImage, 1, file);
		fclose(file);
	}

	delete pixels;

	if (g_GameState >= GS_GAME)
		Orion->CreateTextMessageF(3, 0, "Screenshot saved to: %s", path.c_str());

	/*TPRINT("Saving screen to:\n");
	TPRINT(path.c_str());
	TPRINT("\n");*/
}
//---------------------------------------------------------------------------
PDWORD TScreenshotBuilder::GetScenePixels(const int &x, const int &y, const int &width, const int &height)
{
	PDWORD pixels = new DWORD[width * height];

	glReadPixels(x, y, width, height, GL_BGRA, GL_UNSIGNED_INT_8_8_8_8_REV, &pixels[0]);

	return pixels;
}
//---------------------------------------------------------------------------