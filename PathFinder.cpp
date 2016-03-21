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
TPathObject::TPathObject(int z, char height, char surface)
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
: TBaseQueue(), m_OnLongStair(false), m_AutoWalking(false), m_Path(NULL),
m_PointIndex(0), m_PathSize(0)
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

	int blockIndex = (blockX * g_MapBlockY[g_CurrentMap]) + blockY;

	TMapBlock *block = MapManager->GetBlock(blockIndex);

	if (block == NULL)
		return false;

	int bx = x % 8;
	int by = y % 8;

	bool ignoreGameObjects = (g_Player->Graphic == 0x03DB);
	bool ignoreDoors = g_Player->Dead();
	bool ignoreGameCharacters = (g_Player->Stam >= g_Player->MaxStam || g_Player->Dead());

	TRenderWorldObject *obj = block->GetRender(bx, by);

	while (obj != NULL)
	{
		WORD graphic = obj->Graphic;

		if (obj->IsLandObject())
		{
			if (graphic != 2)
			{
				DWORD flags = UO->GetLandFlags(graphic);

				char surface = 0x10 + (char)(!IsImpassable(flags));

				Add(new TPathObject(obj->Z, 0, surface));
			}
		}
		else if (obj->IsStaticGroupObject())
		{
			bool canBeAdd = true;

			if (obj->IsGameObject())
			{
				if (ignoreGameObjects || graphic >= 0x4000 || (ignoreDoors && obj->IsDoor()) || obj->IsInternal()) //GM || isMulti || (ghost && isDoor) || InternalItem
					canBeAdd = false;
				else if (((TGameObject*)obj)->NPC)
				{
					if (!ignoreGameCharacters)
						Add(new TPathObject(obj->Z, DEFAULT_CHARACTER_HEIGHT, 0));

					canBeAdd = false;
				}
			}
			else
				graphic -= 0x4000;

			if (canBeAdd)
			{
				bool impSurf = (graphic > 1 && (obj->IsImpassable() || obj->IsSurface() || obj->IsBridge()));

				//if (graphic == 1 || !impSurf || (obj->IsBackground() && !impSurf)) {} else

				if (impSurf)
				{
					BYTE height = ((TRenderStaticObject*)obj)->GetStaticHeight();

					char surface = (char)(obj->IsSurface() && !obj->IsImpassable());

					if (obj->IsBridge() && obj->IsPrefixA() && height >= 10) //long stair
						surface += 0x20;

					if (!height && obj->IsBackground() && obj->IsSurface() && obj->Z >= g_Player->Z + 3)
						surface += 0x40;

					Add(new TPathObject(obj->Z, height, surface));
				}
			}
		}

		obj = obj->m_NextXY;
	}

	return true;
}
//---------------------------------------------------------------------------
void TPathFinder::CheckLongStairUnderfoot(int &x, int &y, char &z)
{
	m_OnLongStair = false;

	if (CreateItemsList(x, y))
	{
		for (TPathObject *po = (TPathObject*)m_Items; po != NULL; po = (TPathObject*)po->m_Next)
		{
			char surface = po->Surface;
			int isLongStair = 1;
			
			if (surface >= 0x40)
				surface -= 0x40;

			if (surface >= 0x20)
			{
				surface -= 0x20;
				isLongStair++;
			}

			if (surface > 1)
				surface -= 0x10;

			if (surface)
			{
				char top = po->Z + (char)(po->Height / isLongStair);

				if (top == z)
					m_OnLongStair = (isLongStair > 1);
			}
		}

		Clear();
	}
}
//---------------------------------------------------------------------------
bool TPathFinder::CalculateNewZ(int &x, int &y, char &z)
{
	if (!CreateItemsList(x, y))
		return false;
	
	z = g_Player->Z;
	
	int newZ = z;

	if (m_Items != NULL)
	{
		TPathObject *po = (TPathObject*)m_Items;

		while (po != NULL && po->m_Next != NULL)
			po = (TPathObject*)po->m_Next;

		TPathObject *poStart = po;
		bool found = false;

		int c = 0;
		for (; po != NULL; po = (TPathObject*)po->m_Prev)
		{
			c++;
			int height = po->Height;
			int top = po->Z + height;
			char surface = po->Surface;
			bool isLand = false;
			bool isLongStair = false;
			
			if (surface >= 0x40)
			{
				surface -= 0x40;
				height = 3;
			}

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
					
					TPRINT("f1::Found_1::newZ=%i (top=%i)\n", newZ, top);
					found = true;

					if (height > 1)
						break;
				}
				else if ((isLand && top <= newZ + g_MaxClimbMapZ && top >= newZ - g_MaxFallZ) || (top < newZ))
				{
					if (found && abs(z - newZ) < abs(z - top))
						break;
					
					TPRINT("f1::Found_2::newZ=%i\n", newZ);
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

		TPRINT("c=%i\n", c);
		c = 0;
		
		for (po = poStart; po != NULL; po = (TPathObject*)po->m_Prev)
		{
			c++;
			int curZ = po->Z;
			int top = curZ + po->Height;
			char surface = po->Surface;
			bool isLongStair = false;
			
			if (surface >= 0x40)
			{
				surface -= 0x40;
				top += 3;
			}

			if (surface >= 0x20)
			{
				surface -= 0x20;
				isLongStair = (curZ <= (z + 6));
			}

			TPRINT("surface=%i nowC=%i z=%i newZ=%i curZ=%i top=%i isLongStair=%i\n", surface, c, z, newZ, curZ, top, isLongStair);

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
				else if (curZ >= newZ && curZ <= (newZ + g_MaxBlockZ))
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

	CheckLongStairUnderfoot(x, y, z);

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
void TPathFinder::ProcessAutowalk()
{
	if (m_AutoWalking && g_Player != NULL && !g_DeathScreenTimer && g_WalkRequestCount <= 1 && g_LastStepTime <= GetTickCount())
	{
		if (m_PointIndex >= 0 && m_PointIndex < m_PathSize)
		{
			PATH_POINT &p = m_Path[m_PointIndex];
			m_PointIndex++;

			BYTE newDir = rand() % 8;

			if (!Walk(ConfigManager.AlwaysRun, newDir))
				StopAutoWalk();
		}
		else
			StopAutoWalk();
	}
}
//---------------------------------------------------------------------------
bool TPathFinder::Walk(bool run, BYTE direction)
{
	if (g_LastStepTime > GetTickCount() || g_WalkRequestCount > 1 || g_Player == NULL || g_DeathScreenTimer)
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
			correctDirWD->X = x;
			correctDirWD->Y = y;
			correctDirWD->Z = z;
			correctDirWD->Direction = direction;

			g_Player->m_WalkStack.Push(correctDirWD);

			Walker->m_SendStack.push_back(correctDirWSI);
		}
		
		direction = newDir;
		x = newX;
		y = newY;
		z = newZ;

		bool onMount = (g_Player->FindLayer(OL_MOUNT) != NULL);

		wsi.Time = CHARACTER_ANIMATION_DELAY_TABLE[onMount][run];
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

			wsi.Time = CHARACTER_ANIMATION_DELAY_TABLE[onMount][run];
		}

		direction = newDir;
	}

	if (run)
		direction += 0x80;

	wsi.Dir = direction;

	TWalkData *wd = new TWalkData();
	wd->X = x;
	wd->Y = y;
	wd->Z = z;
	wd->Direction = direction;

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
PATH_POINT *TPathFinder::CalculatePath(int &size, int x, int y, int z)
{
	size = 16;

	return new PATH_POINT[size];
}
//---------------------------------------------------------------------------
bool TPathFinder::WalkTo(int x, int y, int z)
{
	StopAutoWalk();

	m_Path = CalculatePath(m_PathSize, x, y, z);

	if (m_Path != NULL)
	{
		m_AutoWalking = true;
		m_PointIndex = 0;
		ProcessAutowalk();

		//delete m_Path;
		//m_Path = NULL;
	}

	return (m_PathSize != 0);
}
//---------------------------------------------------------------------------
void TPathFinder::StopAutoWalk()
{
	m_AutoWalking = false;
	m_PathSize = 0;

	if (m_Path != NULL)
	{
		delete m_Path;
		m_Path = NULL;
	}
}
//---------------------------------------------------------------------------