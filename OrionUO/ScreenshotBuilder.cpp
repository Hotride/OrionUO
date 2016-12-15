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
#include "Managers/ConfigManager.h"
#include "FreeImage.h"

#pragma comment(lib, "FreeImage.lib")

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
	SaveScreen(0, 0, g_OrionWindow.Size.Width, g_OrionWindow.Size.Height);
}
//---------------------------------------------------------------------------
void CScreenshotBuilder::SaveScreen(const int &x, const int &y, const int &width, const int &height)
{
	string path = g_App.FilePath("snapshots");
	CreateDirectoryA(path.c_str(), NULL);

	SYSTEMTIME st;
	GetLocalTime(&st);

	char buf[100] = { 0 };

	sprintf_s(buf, "\\snapshot_d(%i.%i.%i)_t(%i.%i.%i_%i)", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);

	path += buf;

	UINT_LIST pixels = GetScenePixels(x, y, width, height);

	FIBITMAP *fBmp = FreeImage_ConvertFromRawBits((puchar)&pixels[0], width, height, width * 4, 32, 0, 0, 0);

	FREE_IMAGE_FORMAT format = FIF_BMP;

	switch (g_ConfigManager.ScreenshotFormat)
	{
		case SF_PNG:
		{
			path += ".png";
			format = FIF_PNG;
			break;
		}
		case SF_TIFF:
		{
			path += ".tiff";
			format = FIF_TIFF;
			break;
		}
		case SF_JPEG:
		{
			path += ".jpeg";
			format = FIF_JPEG;
			break;
		}
		default:
		{
			path += ".bmp";
			format = FIF_BMP;
			break;
		}
	}

	FreeImage_Save(format, fBmp, path.c_str());

	FreeImage_Unload(fBmp);

	if (g_GameState >= GS_GAME)
		g_Orion.CreateTextMessageF(3, 0, "Screenshot saved to: %s", path.c_str());
}
//---------------------------------------------------------------------------
UINT_LIST CScreenshotBuilder::GetScenePixels(const int &x, const int &y, const int &width, const int &height)
{
	UINT_LIST pixels(width * height);

	glReadPixels(x, y, width, height, GL_BGRA, GL_UNSIGNED_INT_8_8_8_8_REV, &pixels[0]);

	return pixels;
}
//---------------------------------------------------------------------------