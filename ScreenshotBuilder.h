/****************************************************************************
**
** ScreenshotBuilder.h
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
#ifndef ScreenshotBuilderH
#define ScreenshotBuilderH
//--------------------------------------------------------------------------
class TScreenshotBuilder
{
public:
	TScreenshotBuilder();
	virtual ~TScreenshotBuilder();

	void SaveScreen();

	PDWORD GetScenePixels(const int &x, const int &y, const int &width, const int &height);
};
//---------------------------------------------------------------------------
extern TScreenshotBuilder g_ScreenshotBuilder;
//---------------------------------------------------------------------------
#endif