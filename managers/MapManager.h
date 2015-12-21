/****************************************************************************
**
** MapManager.h
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
#ifndef MapManagerH
#define MapManagerH
//--------------------------------------------------------------------------
class TMapManager : public TBaseQueue
{
private:
public:
	TMapManager();
	virtual ~TMapManager();
	
	int GetActualMap();

	void LoadBlock(TMapBlock *block);
	
	void GetRadarMapBlock(int blockX, int blockY, MAP_BLOCK &mb);

	int GetWorldMapBlock(int &map, int &blockX, int &blockY, MAP_BLOCK &mb);

	void GetMapZ(int x, int y, int &groundZ, int &staticZ);
	
	void Init();
	TMapBlock *GetBlock(DWORD index);
	TMapBlock *AddBlock(DWORD index);
	void DeleteBlock(DWORD index);
	void ClearUnusedBlocks();
	
	void AddRender(TRenderWorldObject *item);
};

extern TMapManager *MapManager;
//--------------------------------------------------------------------------
#endif