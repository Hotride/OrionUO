/****************************************************************************
**
** ImageBounds.cpp
**
** Copyright (C) February 2016 Hotride
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

TImageBounds g_PlayerRect(0, 0, 0, 0);
//---------------------------------------------------------------------------
TImageBounds::TImageBounds(int x, int y, int width, int height)
: m_X(x), m_Y(y), m_Width(width), m_Height(height)
{
}
//---------------------------------------------------------------------------
TImageBounds::~TImageBounds()
{
}
//---------------------------------------------------------------------------
bool TImageBounds::InRect(TImageBounds &ib)
{
	bool inRect = false;

	if (m_X < ib.X)
	{
		if (ib.X < m_X + m_Width)
			inRect = true;
	}
	else
	{
		if (m_X < ib.X + ib.Width)
			inRect = true;
	}

	if (inRect)
	{
		if (m_Y < ib.Y)
		{
			if (ib.Y < m_Y + m_Height)
				inRect = true;
			else
				inRect = false;
		}
		else
		{
			if (m_Y < ib.Y + ib.Height)
				inRect = true;
			else
				inRect = false;
		}
	}

	return inRect;
}
//---------------------------------------------------------------------------
TTextImageBounds::TTextImageBounds(int x, int y, int width, int height, TRenderTextObject *text)
: TImageBounds(x, y, width, height), m_Text(text)
{
}
//---------------------------------------------------------------------------
TTextImageBounds::~TTextImageBounds()
{
	m_Text = NULL;
}
//---------------------------------------------------------------------------