/***********************************************************************************
**
** PathFinder.cpp
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#include "PathFinder.h"
#include "../Managers/MapManager.h"
#include "../Managers/ConfigManager.h"
#include "../Managers/GumpManager.h"
#include "../Game objects/GamePlayer.h"
#include "../Game objects/GameWorld.h"
#include "Walker.h"
#include "../OrionUO.h"
//----------------------------------------------------------------------------------
CPathFinder g_PathFinder;
//----------------------------------------------------------------------------------
CPathFinder::CPathFinder()
: CBaseQueue(), m_OnLongStair(false), m_AutoWalking(false), m_PointIndex(0),
m_PathSize(0), /*m_StartX(0), m_StartY(0), m_EndX(0), m_EndY(0),*/ m_GoalNode(0),
m_GoalFound(false), m_ActiveOpenNodes(0), m_ActiveClosedNodes(0),
m_PathFindDistance(0), m_PathFindidngCanBeCancelled(false), m_StartPoint(), m_EndPoint()
{
}
//----------------------------------------------------------------------------------
CPathFinder::~CPathFinder()
{
}
//----------------------------------------------------------------------------------
bool CPathFinder::CreateItemsList(int &x, int &y, char &z)
{
	Clear();

	int blockX = x / 8;
	int blockY = y / 8;

	int blockIndex = (blockX * g_MapBlockSize[g_CurrentMap].Height) + blockY;

	CMapBlock *block = g_MapManager->GetBlock(blockIndex);

	if (block == NULL)
	{
		block = g_MapManager->AddBlock(blockIndex);
		block->X = blockX;
		block->Y = blockY;
		g_MapManager->LoadBlock(block);
		//return false;
	}

	int bx = x % 8;
	int by = y % 8;

	bool ignoreGameObjects = (g_Player->Graphic == 0x03DB);
	bool ignoreDoors = g_Player->Dead();
	bool ignoreGameCharacters = (ignoreDoors || g_Player->IgnoreCharacters() || g_Player->Stam >= g_Player->MaxStam);

	CRenderWorldObject *obj = block->GetRender(bx, by);

	while (obj != NULL)
	{
		ushort graphic = obj->Graphic;

		if (obj->IsLandObject())
		{
			if (graphic != 2)
			{
				uint flags = g_Orion.GetLandFlags(graphic);

				char surface = 0x10 + (char)(!IsImpassable(flags));

				Add(new CPathObject(obj->Z, 0, surface));
			}
		}
		else if (obj->IsStaticGroupObject())
		{
			bool canBeAdd = true;

			if (obj->IsGameObject())
			{
				if (ignoreGameObjects || graphic >= 0x4000 || (ignoreDoors && obj->IsDoor()) || obj->IsInternal()) //GM || isMulti || (ghost && isDoor) || InternalItem
					canBeAdd = false;
				else if (((CGameObject*)obj)->NPC)
				{
					if (!ignoreGameCharacters)
						Add(new CPathObject(obj->Z, DEFAULT_CHARACTER_HEIGHT, 0));

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
					uchar height = obj->StaticGroupObjectPtr()->GetStaticHeight();

					char surface = (char)(obj->IsSurface() && !obj->IsImpassable());

					if (obj->IsBridge() && obj->IsPrefixA() && height >= 10) //long stair
						surface += 0x20;

					if (!height && obj->IsBackground() && obj->IsSurface() && obj->Z >= z + 3)
						surface += 0x40;

					Add(new CPathObject(obj->Z, height, surface));
				}
			}
		}

		obj = obj->m_NextXY;
	}

	return true;
}
//----------------------------------------------------------------------------------
void CPathFinder::CheckLongStairUnderfoot(int &x, int &y, char &z)
{
	m_OnLongStair = false;

	if (CreateItemsList(x, y, z))
	{
		QFOR(po, m_Items, CPathObject*)
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
//----------------------------------------------------------------------------------
bool CPathFinder::CalculateNewZ(int &x, int &y, char &z)
{
	if (!CreateItemsList(x, y, z))
		return false;
	
	int newZ = z;

	if (m_Items != NULL)
	{
		CPathObject *po = (CPathObject*)m_Items;

		while (po != NULL && po->m_Next != NULL)
			po = (CPathObject*)po->m_Next;

		CPathObject *poStart = po;
		bool found = false;

		int c = 0;
		for (; po != NULL; po = (CPathObject*)po->m_Prev)
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
					
					//TPRINT("f1::Found_1::newZ=%i (top=%i)\n", newZ, top);
					found = true;

					if (height > 1)
						break;
				}
				else if ((isLand && top <= newZ + g_MaxClimbMapZ && top >= newZ - g_MaxFallZ) || (top < newZ))
				{
					if (found && abs(z - newZ) < abs(z - top))
						break;
					
					//TPRINT("f1::Found_2::newZ=%i\n", newZ);
					newZ = top;
					found = true;
				}
			}
		}

		if (!found)
		{
			//TPRINT("Failed 1\n");
			Clear();

			return false;
		}

		//found = false;

		//TPRINT("c=%i\n", c);
		c = 0;
		
		for (po = poStart; po != NULL; po = (CPathObject*)po->m_Prev)
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

			//TPRINT("surface=%i nowC=%i z=%i newZ=%i curZ=%i top=%i isLongStair=%i\n", surface, c, z, newZ, curZ, top, isLongStair);

			if (top <= newZ)
				continue;
			
			if (surface < 0x10 && !isLongStair)
			//if (!surface)
			{
				if (top > newZ && top < (newZ + g_MaxBlockZ))
				{
					//TPRINT("f2_1\n");
					found = false;

					break;
				}
				else if (curZ >= z && curZ < (z + (g_MaxBlockZ / 2)))
				{
					//TPRINT("f2_2\n");
					found = false;

					break;
				}
				else if (curZ <= z && top >= (z + (g_MaxBlockZ / 2)))
				{
					//TPRINT("f2_3\n");
					found = false;

					break;
				}
				else if (curZ >= newZ && curZ <= (newZ + g_MaxBlockZ))
				{
					//TPRINT("f2_4\n");
					found = false;

					break;
				}
			}
		}

		if (!found)
		{
			//TPRINT("Failed 2\n");
			Clear();

			return false;
		}
	}

	z = newZ;

	return true;
}
//----------------------------------------------------------------------------------
void CPathFinder::GetNewXY(uchar &direction, int &x, int &y)
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
//----------------------------------------------------------------------------------
bool CPathFinder::CanWalk(uchar &direction, int &x, int &y, char &z)
{
	int newX = x;
	int newY = y;
	char newZ = z;
	uchar newDirection = direction;

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

				uchar testDir = (direction + dirOffset[i]) % 8;
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
//----------------------------------------------------------------------------------
int CPathFinder::GetWalkSpeed(const bool &run, const bool &onMount)
{
	bool mounted = (onMount || (g_SpeedMode == CST_FAST_UNMOUNT || g_SpeedMode == CST_FAST_UNMOUNT_AND_CANT_RUN));

	return CHARACTER_ANIMATION_DELAY_TABLE[mounted][run];
}
//----------------------------------------------------------------------------------
bool CPathFinder::Walk(bool run, uchar direction)
{
	if (g_PendingDelayTime >  g_Ticks || g_WalkRequestCount > 3 || g_Player == NULL || g_DeathScreenTimer || g_GameState != GS_GAME)
		return false;

	if (g_SpeedMode >= CST_CANT_RUN)
		run = false;
	else if (!run)
		run = g_ConfigManager.AlwaysRun;

	int x = g_Player->X;
	int y = g_Player->Y;
	char z = g_Player->Z;
	uchar olddir = g_Player->Direction;

	bool onMount = (g_Player->FindLayer(OL_MOUNT) != NULL);

	bool emptyStack = true;
	CWalkData *walker = g_Player->m_WalkStack.Top();
	if (walker != NULL)
	{
		x = walker->X;
		y = walker->Y;
		z = walker->Z;
		olddir = walker->Direction;
		emptyStack = false;
	}

	ushort walkTime = TURN_DELAY;

	if ((olddir & 7) == (direction & 7)) //Повернуты куда надо
	{
		uchar newDir = direction;
		int newX = x;
		int newY = y;
		char newZ = z;

		if (!CanWalk(newDir, newX, newY, newZ))
			return false;

		if ((direction & 7) != newDir)
			direction = newDir;
		else
		{
			direction = newDir;
			x = newX;
			y = newY;
			z = newZ;

			walkTime = GetWalkSpeed(run, onMount);
		}
	}
	else
	{
		uchar newDir = direction;
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

			walkTime = GetWalkSpeed(run, onMount);
		}

		direction = newDir;
	}

	CGameItem *bank = g_Player->FindLayer(OL_BANK);

	if (bank != NULL)
	{
		bank->Clear();
		bank->Opened = false;

		g_GumpManager.CloseGump(bank->Serial, 0, GT_CONTAINER);
	}

	if (run)
		direction += 0x80;

	CWalkData *wd = new CWalkData();
	wd->X = x;
	wd->Y = y;
	wd->Z = z;
	wd->Direction = direction;

	/*g_RemoveRangeXY.x = x;
	g_RemoveRangeXY.y = y;

	g_UpdateRange--;

	UO->RemoveRangedObjects();

	g_UpdateRange++;*/

	if (emptyStack)
	{
		if (!g_Player->Walking())
			g_Player->SetAnimation(0xFF);

		g_Player->LastStepTime = g_Ticks;
	}

	g_Player->m_WalkStack.Push(wd);

	g_World->MoveToTop(g_Player);

	uchar seq = g_Walker->GetSequence();
	g_Walker->SetSequence(seq, direction);

	uchar buf[7] = { 0 };
	*buf = 0x02;
	buf[1] = direction;
	buf[2] = seq;
	pack32(buf + 3, g_Walker->m_FastWalkStack.Pop());

	g_Orion.Send(buf, 7);

	g_WalkRequestCount++;

	g_Walker->IncSequence();

	static bool lastRun = false;
	static bool lastMount = false;
	static int lastDir = -1;
	static int lastDelta = 0;
	static int lastStepTime = 0;

	/*if (onMount)
		trace_printf("Mounted");

	if (run)
		trace_printf("Run");
	else
		trace_printf("Walk");*/

	//Высчитываем актуальную дельту с помощью разници во времени между прошлым и текущим шагом.
	int nowDelta = 0;
	
	if (lastDir == direction && lastMount == onMount && lastRun == run)
	{
		nowDelta = (g_Ticks - lastStepTime) - walkTime + lastDelta;

		if (abs(nowDelta) > 70)
			nowDelta = 0;

		lastDelta = nowDelta;
	}
	else
		lastDelta = 0;


	//TPRINT("ReqDelta (%i) %i\n", nowDelta, currentTime - lastStepTime);

	lastStepTime = g_Ticks;

	lastDelta = nowDelta;
	lastRun = run;
	lastMount = onMount;
	lastDir = direction;

	g_PendingDelayTime = g_Ticks + walkTime - nowDelta;
	g_Player->GetAnimationGroup();

	return true;
}
//----------------------------------------------------------------------------------
int CPathFinder::GetGoalDistCost(const WISP_GEOMETRY::CPoint2Di &p, int cost)
{
	return (abs(m_EndPoint.X - p.X) + abs(m_EndPoint.Y - p.Y)) * cost;
}
//----------------------------------------------------------------------------------
bool CPathFinder::DoesNotExistOnOpenList(int x, int y, int z)
{
	bool result = false;

	IFOR(i, 0, PATHFINDER_MAX_NODES)
	{
		CPathNode &node = m_OpenList[i];

		if (node.Used && node.X == x && node.Y == y && node.Z == z)
		{
			result = true;
			break;
		}
	}

	return result;
}
//----------------------------------------------------------------------------------
bool CPathFinder::DoesNotExistOnClosedList(int x, int y, int z)
{
	bool result = false;

	IFOR(i, 0, PATHFINDER_MAX_NODES)
	{
		CPathNode &node = m_ClosedList[i];

		if (node.Used && node.X == x && node.Y == y && node.Z == z)
		{
			result = true;
			break;
		}
	}

	return result;
}
//----------------------------------------------------------------------------------
int CPathFinder::AddNodeToList(int list, int direction, int x, int y, int z, CPathNode *parentNode, int cost)
{
	if (!list)
	{
		if (!DoesNotExistOnClosedList(x, y, z))
		{
			if (!DoesNotExistOnOpenList(x, y, z))
			{
				IFOR(i, 0, PATHFINDER_MAX_NODES)
				{
					CPathNode &node = m_OpenList[i];

					if (!node.Used)
					{
						node.Used = true;

						node.Direction = direction;
						node.X = x;
						node.Y = y;
						node.Z = z;
						WISP_GEOMETRY::CPoint2Di p(x, y);

						node.DistFromGoalCost = GetGoalDistCost(p, cost);
						node.DistFromStartCost = parentNode->DistFromStartCost + cost;
						node.Cost = node.DistFromGoalCost + node.DistFromStartCost;

						node.m_Parent = parentNode;

						//if (x == m_EndX && y == m_EndY)
						if (GetDistance(m_EndPoint, p) <= m_PathFindDistance)
						{
							m_GoalFound = true;
							m_GoalNode = i;
						}

						m_ActiveOpenNodes++;

						return i;
					}
				}
			}
			else
			{
				IFOR(i, 0, PATHFINDER_MAX_NODES)
				{
					CPathNode &node = m_OpenList[i];

					if (node.Used)
					{
						if (node.X == x && node.Y == y && node.Z == z)
						{
							int startCost = parentNode->DistFromStartCost + cost;

							if (node.DistFromStartCost > startCost)
							{
								node.m_Parent = parentNode;
								node.DistFromStartCost = startCost + cost;
								node.Cost = node.DistFromGoalCost + node.DistFromStartCost;
							}

							return i;
						}
					}
				}
			}
		}
		else
			return 0;
	}
	else
	{
		parentNode->Used = false;

		IFOR(i, 0, PATHFINDER_MAX_NODES)
		{
			CPathNode &node = m_ClosedList[i];

			if (!node.Used)
			{
				node.Used = true;

				node.DistFromGoalCost = parentNode->DistFromGoalCost;
				node.DistFromStartCost = parentNode->DistFromStartCost;
				node.Cost = node.DistFromGoalCost + node.DistFromStartCost;
				node.Direction = parentNode->Direction;
				node.X = parentNode->X;
				node.Y = parentNode->Y;
				node.Z = parentNode->Z;
				node.m_Parent = parentNode->m_Parent;

				m_ActiveOpenNodes--;
				m_ActiveClosedNodes++;

				return i;
			}
		}
	}

	return -1;
}
//----------------------------------------------------------------------------------
bool CPathFinder::OpenNodes(CPathNode *node)
{
	bool found = false;

	IFOR(i, 0, 8)
	{
		uchar direction = (uchar)i;
		int x = node->X;
		int y = node->Y;
		char z = (char)node->Z;

		uchar oldDirection = direction;

		if (CanWalk(direction, x, y, z))
		{
			if (direction != oldDirection)
				continue;

			int diagonal = i % 2;

			if (diagonal)
			{
				uchar wantDirection = (uchar)i;
				int wantX = node->X;
				int wantY = node->Y;

				GetNewXY(wantDirection, wantX, wantY);

				if (x != wantX || y != wantY)
					diagonal = -1;
			}

			if (diagonal >= 0 && AddNodeToList(0, direction, x, y, z, node, 1 + diagonal) != -1)
				found = true;
			else
				continue;
		}
	}

	return found;
}
//----------------------------------------------------------------------------------
int CPathFinder::FindCheapestNode()
{
	int cheapestCost = 9999999;
	int cheapestNode = -1;

	IFOR(i, 0, PATHFINDER_MAX_NODES)
	{
		if (m_OpenList[i].Used)
		{
			if (m_OpenList[i].Cost < cheapestCost)
			{
				cheapestNode = i;
				cheapestCost = m_OpenList[i].Cost;
			}
		}
	}

	int result = -1;

	if (cheapestNode != -1)
		result = AddNodeToList(1, 0, 0, 0, 0, &m_OpenList[cheapestNode], 2);

	return result;
}
//----------------------------------------------------------------------------------
bool CPathFinder::FindPath(int maxNodes)
{
	int curNode = 0;

	m_ClosedList[0].Used = true;

	m_ClosedList[0].X = m_StartPoint.X;
	m_ClosedList[0].Y = m_StartPoint.Y;
	m_ClosedList[0].Z = g_Player->Z;
	m_ClosedList[0].m_Parent = NULL;

	m_ClosedList[0].DistFromGoalCost = GetGoalDistCost(m_StartPoint, 0);
	m_ClosedList[0].Cost = m_ClosedList[0].DistFromGoalCost;

	while (m_AutoWalking)
	{
		OpenNodes(&m_ClosedList[curNode]);

		if (m_GoalFound)
		{
			int totalNodes = 0;

			CPathNode *GoalNode = &m_OpenList[m_GoalNode];

			while (GoalNode->m_Parent != NULL && GoalNode != GoalNode->m_Parent)
			{
				GoalNode = GoalNode->m_Parent;
				totalNodes++;
			};

			totalNodes++;

			m_PathSize = totalNodes;

			GoalNode = &m_OpenList[m_GoalNode];

			while (totalNodes > 0)
			{
				totalNodes--;
				m_Path[totalNodes] = GoalNode;
				GoalNode = GoalNode->m_Parent;
			};

			break;
		}

		curNode = FindCheapestNode();

		if (curNode == -1)
			return false;

		if (m_ActiveClosedNodes >= maxNodes)
			return false;
	}

	return true;
}
//----------------------------------------------------------------------------------
bool CPathFinder::WalkTo(int x, int y, int z, int distance)
{
	IFOR(i, 0, PATHFINDER_MAX_NODES) //m_ActiveOpenNodes)
		m_OpenList[i].Reset();

	IFOR(i, 0, PATHFINDER_MAX_NODES) //m_ActiveClosedNodes)
		m_ClosedList[i].Reset();

	m_StartPoint.X = g_Player->X;
	m_StartPoint.Y = g_Player->Y;
	m_EndPoint.X = x;
	m_EndPoint.Y = y;
	m_GoalNode = 0;
	m_GoalFound = false;
	m_ActiveOpenNodes = 0;
	m_ActiveClosedNodes = 0;
	m_PathFindDistance = distance;
	m_PathSize = 0;
	m_PathFindidngCanBeCancelled = true;

	StopAutoWalk();
	m_AutoWalking = true;

	if (FindPath(PATHFINDER_MAX_NODES))
	{
		m_PointIndex = 1;
		ProcessAutowalk();
	}
	else
		m_AutoWalking = false;

	return (m_PathSize != 0);
}
//----------------------------------------------------------------------------------
void CPathFinder::ProcessAutowalk()
{
	if (m_AutoWalking && g_Player != NULL && !g_DeathScreenTimer && g_WalkRequestCount <= 3 && g_PendingDelayTime <= g_Ticks)
	{
		if (m_PointIndex >= 0 && m_PointIndex < m_PathSize)
		{
			CPathNode *p = m_Path[m_PointIndex];

			uchar olddir = g_Player->Direction;

			CWalkData *walker = g_Player->m_WalkStack.Top();
			if (walker != NULL)
				olddir = walker->Direction;

			if ((olddir & 7) == p->Direction)
				m_PointIndex++;

			if (!Walk(g_ConfigManager.AlwaysRun, p->Direction))
				StopAutoWalk();
		}
		else
			StopAutoWalk();
	}
}
//----------------------------------------------------------------------------------
void CPathFinder::StopAutoWalk()
{
	m_AutoWalking = false;
	m_PathSize = 0;
}
//----------------------------------------------------------------------------------
