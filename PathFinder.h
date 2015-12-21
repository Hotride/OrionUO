/****************************************************************************
**
** PathFinder.h
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
#ifndef PathFinderH
#define PathFinderH
//---------------------------------------------------------------------------
class TPathObject : public TBaseQueueItem
{
private:
	char m_Z;
	char m_Height;
	BYTE m_Surface;
public:
	TPathObject(char z, char height, char surface);
	virtual ~TPathObject();

	SETGET(char, Z)
	SETGET(char, Height)
	SETGET(BYTE, Surface)
};
//---------------------------------------------------------------------------
class TPathFinder : public TBaseQueue
{
private:
	bool CalculateNewZ(int &x, int &y, char &z);
	bool CanWalk(BYTE &direction, int &x, int &y, char &z);

	void GetNewXY(BYTE &direction, int &x, int &y);
	bool CreateItemsList(int &x, int &y);

	char m_OnLongStair;

public:
	TPathFinder();
	virtual ~TPathFinder();

	bool Walk(bool run, BYTE direction);
};
//---------------------------------------------------------------------------
extern TPathFinder *PathFinder;
//---------------------------------------------------------------------------
#endif