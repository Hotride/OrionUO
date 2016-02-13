/****************************************************************************
**
** Multi.h
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
#ifndef MultiH
#define MultiH
//---------------------------------------------------------------------------
class TMultiObject : public TRenderStaticObject
{
private:
	DWORD m_MultiFlags;
	char m_CanBeTransparent;

public:
	TMultiObject(WORD graphic, short x, short y, char z, DWORD flags);
	virtual ~TMultiObject();
	
	SETGET(DWORD, MultiFlags);
	SETGET(char, CanBeTransparent);

	virtual int Draw(bool &mode, int &drawX, int &drawY, DWORD &ticks);
	
	virtual bool TranparentTest(int &playerZ);

	bool IsMultiObject() {return true;}
};
//---------------------------------------------------------------------------
class TMulti : public TBaseQueueItem
{
private:
	short m_X;
	short m_Y;
	short m_MinX;
	short m_MinY;
	short m_MaxX;
	short m_MaxY;

public:
	TMulti(short x, short y);
	virtual ~TMulti();
	
	SETGET(short, X);
	SETGET(short, Y);
	SETGET(short, MinX);
	SETGET(short, MinY);
	SETGET(short, MaxX);
	SETGET(short, MaxY);
};
//---------------------------------------------------------------------------
#endif