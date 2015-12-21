/****************************************************************************
**
** PathFinder.cpp
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
#include "stdafx.h"

TPathFinder *PathFinder = NULL;
//---------------------------------------------------------------------------
//--------------------------------TPathObject--------------------------------
//---------------------------------------------------------------------------
TPathObject::TPathObject(char z, char height, char surface)
: TBaseQueueItem(), m_Z(z), m_Height(height), m_Surface(surface)
{
}
//---------------------------------------------------------------------------
TPathObject::~TPathObject()
{
}
//---------------------------------------------------------------------------
//---------------------------------TPathFinder-------------------------------
//---------------------------------------------------------------------------
TPathFinder::TPathFinder()
: TBaseQueue()
{
}
//---------------------------------------------------------------------------
TPathFinder::~TPathFinder()
{
}
//---------------------------------------------------------------------------
bool TPathFinder::CreateItemsList(int &x, int &y)
{
	Clear();

	int blockX = x / 8;
	int blockY = y / 8;

	int blockIndex = (blockX * 512) + blockY;

	TMapBlock *block = MapManager->GetBlock(blockIndex);

	if (block == NULL)
		return false;

	int bx = x % 8;
	int by = y % 8;

	bool ignoreGameObjects = (g_Player->Graphic == 0x03DB);
	bool ignoreDoors = g_Player->Dead();
	bool ignoreGameCharacters = (g_Player->Stam >= g_Player->MaxStam);

	TRenderWorldObject *obj = block->GetRender(bx, by);

	while (obj != NULL)
	{
		switch (obj->RenderType)
		{
			case ROT_LAND_OBJECT:
			{
				WORD graphic = obj->Graphic;

				if (graphic == 2)
					break;

				char landZ = obj->Z;

				DWORD flags = UO->GetLandFlags(graphic);

				char surface = 0x10 + (char)(!IsImpassable(flags));

				Add(new TPathObject(landZ, 0, surface));

				break;
			}
			case ROT_STATIC_OBJECT:
			case ROT_MULTI_OBJECT:
			case ROT_GAME_OBJECT:
			{
				WORD graphic = obj->Graphic;

				if (obj->IsGameObject())
				{
					if (ignoreGameObjects || graphic >= 0x4000 || (ignoreDoors && obj->IsDoor()) || obj->IsInternal()) //GM || isMulti || (ghost && isDoor) || InternalItem
						break;
					else if (((TGameObject*)obj)->NPC)
					{
						if (!ignoreGameCharacters)
							Add(new TPathObject(obj->Z, g_CharacterHeight, false));

						break;
					}
				}
				else
					graphic -= 0x4000;

				bool impSurf = (obj->IsImpassable() || obj->IsSurface() || obj->IsBridge());

				if (graphic == 1 || (obj->IsBackground() && !impSurf) || !impSurf)
					break;

				STATIC_TILES &st = UO->m_StaticData[graphic / 32].Tiles[graphic % 32];

				char height = st.Height;

				char surface = (char)(obj->IsSurface() && !obj->IsImpassable());

				if (obj->IsBridge() && obj->IsPrefixA() && height >= 10)
					surface += 0x20;

				Add(new TPathObject(obj->Z, height, surface));

				break;
			}
			default:
				break;
		}

		obj = obj->m_NextXY;
	}

	return true;
}
//---------------------------------------------------------------------------
bool TPathFinder::CalculateNewZ(int &x, int &y, char &z)
{
	if (!CreateItemsList(x, y))
		return false;
	
	z = g_Player->Z;
	
	char newZ = z;

	if (m_Items != NULL)
	{
		TPathObject *po = (TPathObject*)m_Items;

		while (po != NULL && po->m_Next != NULL)
			po = (TPathObject*)po->m_Next;

		TPathObject *poStart = po;
		bool found = false;

		int c = 0;
		for(; po != NULL; po = (TPathObject*)po->m_Prev)
		{
			c++;
			char height = po->Height;
			int top = po->Z + height;
			char surface = po->Surface;
			bool isLand = false;
			bool isLongStair = false;

			if (surface >= 0x20)
			{
				surface -= 0x20;
				isLongStair = true;
			}

			if (surface > 1)
			{
				surface -= 0x10;
				isLand = true;
			}

			if (found && (top > (newZ - g_MaxBlockZ) || !surface))
				break;

			if (!surface && top < newZ) // top <= newZ
			{
				found = false;
				break;
			}

			if (surface)
			{
				if (top < newZ + g_MaxClimbZ + (isLongStair ? height : 0) && top >= newZ - g_MaxFallZ)
				{
					if (found && abs(z - newZ) <= abs(z - top)) // <
						break;
					
					if (isLongStair && top > newZ + g_MaxClimbZ)
						newZ = po->Z + (height / 2);
					else
						newZ = top;
					
					found = true;

					if (height > 1)
						break;
				}
				else if ((isLand && top <= newZ + g_MaxClimbMapZ && top >= newZ - g_MaxFallZ) || (top < newZ))
				{
					if (found && abs(z - newZ) < abs(z - top))
						break;

					newZ = top;
					found = true;
				}
			}
		}

		if (!found)
		{
			TPRINT("Failed 1\n");
			Clear();

			return false;
		}

		//found = false;

		trace_printf("c=%i\n", c);
		c = 0;
		
		for(po = poStart; po != NULL; po = (TPathObject*)po->m_Prev)
		{
			c++;
			char curZ = po->Z;
			int top = curZ + po->Height;
			char surface = po->Surface;
			bool isLongStair = false;
			
			if (surface >= 0x20)
			{
				surface -= 0x20;
				isLongStair = true;
			}

			trace_printf("nowC=%i z=%i newZ=%i curZ=%i top=%i isLongStair=%i\n", c, z, newZ, curZ, top, isLongStair);

			if (top <= newZ)
				continue;
			
			if (surface < 0x10 && !isLongStair)
			//if (!surface)
			{
				if (top > newZ && top < (newZ + g_MaxBlockZ))
				{
					TPRINT("f2_1\n");
					found = false;

					break;
				}
				else if (curZ >= z && curZ < (z + (g_MaxBlockZ / 2)))
				{
					TPRINT("f2_2\n");
					found = false;

					break;
				}
				else if (curZ <= z && top >= (z + (g_MaxBlockZ / 2)))
				{
					TPRINT("f2_3\n");
					found = false;

					break;
				}
				else if (curZ >= newZ && curZ < (newZ + g_MaxBlockZ))
				{
					TPRINT("f2_4\n");
					found = false;

					break;
				}
			}
		}

		if (!found)
		{
			TPRINT("Failed 2\n");
			Clear();

			return false;
		}
	}

	z = newZ;

	return true;
}
//---------------------------------------------------------------------------
void TPathFinder::GetNewXY(BYTE &direction, int &x, int &y)
{
	switch (direction & 7)
	{
		case 0:
		{
			y--;
			break;
		}
		case 1:
		{
			x++;
			y--;
			break;
		}
		case 2:
		{
			x++;
			break;
		}
		case 3:
		{
			x++;
			y++;
			break;
		}
		case 4:
		{
			y++;
			break;
		}
		case 5:
		{
			x--;
			y++;
			break;
		}
		case 6:
		{
			x--;
			break;
		}
		case 7:
		{
			x--;
			y--;
			break;
		}
	}
}
//---------------------------------------------------------------------------
bool TPathFinder::CanWalk(BYTE &direction, int &x, int &y, char &z)
{
	int newX = x;
	int newY = y;
	char newZ = z;
	BYTE newDirection = direction;

	GetNewXY(direction, newX, newY);

	bool passed = CalculateNewZ(newX, newY, newZ);

	if ((char)direction % 2) //diagonal
	{
		const char dirOffset[2] = {1, -1};

		if (passed) //test angleowner tiles
		{
			IFOR(i, 0, 2 && passed)
			{
				int testX = x;
				int testY = y;
				char testZ = z;
		
				BYTE testDir = (direction + dirOffset[i]) % 8;
				GetNewXY(testDir, testX, testY);

				passed = CalculateNewZ(testX, testY, testZ);
			}
		}

		if (!passed) //test neary tiles
		{
			IFOR(i, 0, 2 && !passed)
			{
				newX = x;
				newY = y;
				newZ = z;

				newDirection = (direction + dirOffset[i]) % 8;
				GetNewXY(newDirection, newX, newY);

				passed = CalculateNewZ(newX, newY, newZ);
			}
		}
	}

	if (passed)
	{
		x = newX;
		y = newY;
		z = newZ;
		direction = newDirection;
	}

	return passed;
}
//---------------------------------------------------------------------------
bool TPathFinder::Walk(bool run, BYTE direction)
{
	if (g_LastStepTime > GetTickCount() || g_WalkRequestCount > 1 || g_Player == NULL)
		return false;

	if (!run)
		run = ConfigManager.AlwaysRun;

	int x = g_Player->X;
	int y = g_Player->Y;
	char z = g_Player->Z;
	BYTE olddir = g_Player->Direction;

	bool emptyStack = true;
	TWalkData *walker = g_Player->m_WalkStack.Top();
	if (walker != NULL)
	{
		x = walker->X;
		y = walker->Y;
		z = walker->Z;
		olddir = walker->Direction;
		emptyStack = false;
	}

	WALKER_SEND_ITEM wsi;
	wsi.Time = 100;

	if ((olddir & 7) == (direction & 7)) //Повернуты куда надо
	{
		BYTE newDir = direction;
		int newX = x;
		int newY = y;
		char newZ = z;

		if (!CanWalk(newDir, newX, newY, newZ))
			return false;

		if ((direction & 7) != newDir)
		{
			WALKER_SEND_ITEM correctDirWSI = {newDir, 100};
			
			TWalkData *correctDirWD = new TWalkData();
			correctDirWD->SetX(x);
			correctDirWD->SetY(y);
			correctDirWD->SetZ(z);
			correctDirWD->SetDirection(direction);

			g_Player->m_WalkStack.Push(correctDirWD);

			Walker->m_SendStack.push_back(correctDirWSI);
		}
		
		direction = newDir;
		x = newX;
		y = newY;
		z = newZ;

		bool onMount = (g_Player->FindLayer(OL_MOUNT) != NULL);

		wsi.Time = g_CharacterAnimationDelayTable[onMount][run];
	}
	else
	{
		BYTE newDir = direction;
		int newX = x;
		int newY = y;
		char newZ = z;

		if (!CanWalk(newDir, newX, newY, newZ))
		{
			if ((olddir & 7) == newDir)
				return false;
		}

		if ((olddir & 7) == newDir)
		{
			x = newX;
			y = newY;
			z = newZ;

			bool onMount = (g_Player->FindLayer(OL_MOUNT) != NULL);

			wsi.Time = g_CharacterAnimationDelayTable[onMount][run];
		}

		direction = newDir;
	}

	if (run)
		direction += 0x80;

	wsi.Dir = direction;

	TWalkData *wd = new TWalkData();
	wd->SetX(x);
	wd->SetY(y);
	wd->SetZ(z);
	wd->SetDirection(direction);

	g_RemoveRangeXY.x = x;
	g_RemoveRangeXY.y = y;

	g_UpdateRange--;

	UO->RemoveRangedObjects();

	g_UpdateRange++;

	if (emptyStack)
	{
		if (!g_Player->Walking())
			g_Player->SetAnimation(0xFF);

		g_Player->LastStepTime = GetTickCount();
	}

	g_Player->m_WalkStack.Push(wd);

	World->MoveToTop(g_Player);

	Walker->m_SendStack.push_back(wsi);
	g_Player->GetAnimationGroup();

	return true;
}
//---------------------------------------------------------------------------