/****************************************************************************
**
** MapBlock.h
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
#ifndef MapBlockH
#define MapBlockH
//---------------------------------------------------------------------------
class TMapBlock : public TBaseQueueItem
{
private:
	DWORD m_Index;
	DWORD m_LastAccessTime;
	short m_X;
	short m_Y;
	
	char GetLandZ(int x, int y, int &map);
	char GetRenderZ(TRenderWorldObject *item);
public:
	TMapBlock(DWORD index);
	virtual ~TMapBlock();

	SETGET(DWORD, Index)
	SETGET(DWORD, LastAccessTime)
	SETGET(short, X)
	SETGET(short, Y)

	TMapObject *Block[8][8];

	TMapObject *AddObject(TMapObject *obj, int x, int y);
	TLandObject *GetLand(int &x, int &y);

	void AddRender(TRenderWorldObject *item, int &x, int &y);
	TRenderWorldObject *GetRender(int &x, int &y);

	WORD GetRadarColor(int x, int y);

	void CreateLandTextureRect();
};
//---------------------------------------------------------------------------
#endif