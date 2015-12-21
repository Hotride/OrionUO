/****************************************************************************
**
** MulStrict.cpp
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
//----------------------------------------------------------------------------
void ANIM_FRAME::Reset()
{
	ImageCenterX = 0;
	ImageCenterY = 0;
	Width = 0;
	Height = 0;
	Texture = 0;
	Data = NULL;
}
//----------------------------------------------------------------------------
void ANIM_FRAME::Clear()
{
	if (Texture != 0) glDeleteTextures(1, &Texture);
	Texture = 0;

	if (Data != NULL) delete Data;
	Data = NULL;
}
//----------------------------------------------------------------------------