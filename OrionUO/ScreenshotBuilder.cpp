/***********************************************************************************
**
** ScreenshotBuilder.h
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#include "ScreenshotBuilder.h"
#include "Wisp/WispApplication.h"
#include "OrionWindow.h"
#include "OrionUO.h"

CScreenshotBuilder g_ScreenshotBuilder;
//---------------------------------------------------------------------------
CScreenshotBuilder::CScreenshotBuilder()
{
}
//---------------------------------------------------------------------------
CScreenshotBuilder::~CScreenshotBuilder()
{
}
//---------------------------------------------------------------------------
void CScreenshotBuilder::SaveScreen()
{
	string path = g_App.FilePath("snapshots");
	CreateDirectoryA(path.c_str(), NULL);

	SYSTEMTIME st;
	GetLocalTime(&st);

	char buf[100] = { 0 };

	sprintf_s(buf, "\\snapshot_d(%i.%i.%i)_t(%i.%i.%i_%i).bmp", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);

	path += buf;

	int width = g_OrionWindow.Size.Width;
	int height = g_OrionWindow.Size.Height;

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

	UINT_LIST pixels = GetScenePixels(0, 0, width, height);

	FILE *file = NULL;
	fopen_s(&file, path.c_str(), "wb");

	if (file != NULL)
	{
		fwrite(&bmFileHeader, sizeof(bmFileHeader), 1, file);
		fwrite(&bmInfo, sizeof(bmInfo), 1, file);
		fwrite(&pixels[0], bmInfo.bmiHeader.biSizeImage, 1, file);
		fclose(file);
	}

	if (g_GameState >= GS_GAME)
		g_Orion.CreateTextMessageF(3, 0, "Screenshot saved to: %s", path.c_str());

	/*LOG("Saving screen to:\n");
	LOG(path.c_str());
	LOG("\n");*/
}
//---------------------------------------------------------------------------
UINT_LIST CScreenshotBuilder::GetScenePixels(const int &x, const int &y, const int &width, const int &height)
{
	UINT_LIST pixels(width * height);

	glReadPixels(x, y, width, height, GL_BGRA, GL_UNSIGNED_INT_8_8_8_8_REV, &pixels[0]);

	return pixels;
}
//---------------------------------------------------------------------------