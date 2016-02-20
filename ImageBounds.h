/****************************************************************************
**
** ImageBounds.h
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
#ifndef ImageBoundsH
#define ImageBoundsH
//---------------------------------------------------------------------------
class TRenderTextObject;
//---------------------------------------------------------------------------
class TImageBounds
{
protected:
	int m_X;
	int m_Y;
	int m_Width;
	int m_Height;

public:
	TImageBounds(int x, int y, int width, int height);
	virtual ~TImageBounds();

	SETGET(int, X);
	SETGET(int, Y);
	SETGET(int, Width);
	SETGET(int, Height);

	bool InRect(TImageBounds &ib);
};
//---------------------------------------------------------------------------
class TTextImageBounds : public TImageBounds
{
protected:
	TRenderTextObject *m_Text;

public:
	TTextImageBounds(int x, int y, int width, int height, TRenderTextObject *text);
	virtual ~TTextImageBounds();

	SETGET(TRenderTextObject*, Text);
};
//---------------------------------------------------------------------------
extern TImageBounds g_PlayerRect;
//---------------------------------------------------------------------------
#endif