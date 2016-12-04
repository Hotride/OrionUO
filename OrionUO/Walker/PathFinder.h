/***********************************************************************************
**
** PathFinder.h
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#ifndef PATHFINDER_H
#define PATHFINDER_H
//----------------------------------------------------------------------------------
#include "PathNode.h"
//----------------------------------------------------------------------------------
const int PATHFINDER_MAX_NODES = 10000;

#define TEST_WALK_CHECK_ALG 0
//----------------------------------------------------------------------------------
enum PATH_OBJECT_FLAGS
{
	POF_IMPASSABLE_OR_SURFACE	= 0x00000001,
	POF_SURFACE				= 0x00000002,
	POF_BRIDGE				= 0x00000004
};
//----------------------------------------------------------------------------------
//Класс для поиска пути и теста шага на точку
class CPathFinder : public CBaseQueue
{
	//Автоматической передвижение (не сложными махинациями с мышкой)
	SETGET(bool, AutoWalking);
	SETGET(bool, PathFindidngCanBeCancelled);
	SETGET(bool, BlockMoving);

protected:
	//Вычисление новой Z координаты
	virtual bool CalculateNewZ(int &x, int &y, char &z);

	//Вычисление новых XY координат
	void GetNewXY(uchar &direction, int &x, int &y);

	//Создание списка предметов, участвующих в поиске в указанных координатах
	virtual bool CreateItemsList(int &x, int &y, char &z);

	//Флаг, указывающий. что персонаж стоит на длинной лестнице
	bool m_OnLongStair;

	//Проверка на лестницу под ногами
	void CheckLongStairUnderfoot(int &x, int &y, char &z);

	WISP_GEOMETRY::CPoint2Di m_StartPoint;
	WISP_GEOMETRY::CPoint2Di m_EndPoint;

	/*int m_StartX;
	int m_StartY;

	int m_EndX;
	int m_EndY;*/

	int m_GoalNode;
	bool m_GoalFound;

	int m_ActiveOpenNodes;
	int m_ActiveClosedNodes;

	int m_PathFindDistance;

	CPathNode m_OpenList[PATHFINDER_MAX_NODES];
	CPathNode m_ClosedList[PATHFINDER_MAX_NODES];

	//Список точек пути
	CPathNode *m_Path[PATHFINDER_MAX_NODES];

	//Текущая точка пути
	int m_PointIndex;

	//Размер точек пути
	int m_PathSize;

	int GetGoalDistCost(const WISP_GEOMETRY::CPoint2Di &p, int cost);

	bool DoesNotExistOnOpenList(int x, int y, int z);

	bool DoesNotExistOnClosedList(int x, int y, int z);

	int AddNodeToList(int list, int direction, int x, int y, int z, CPathNode *parentNode, int cost);

	bool OpenNodes(CPathNode *node);

	int FindCheapestNode();

	bool FindPath(int maxNodes);

public:
	CPathFinder();
	virtual ~CPathFinder();

	//Проверка на возможность сделать шаг в указанные координаты
	virtual bool CanWalk(uchar &direction, int &x, int &y, char &z);

	//Пойти в указанные координаты
	virtual bool Walk(bool run, uchar direction);

	//Переместиться в указанные координаты
	bool WalkTo(int x, int y, int z, int distance);

	//Обработка пути
	void ProcessAutowalk();

	//Остановить автоматическую ходилку
	void StopAutoWalk();

	int GetWalkSpeed(const bool &run, const bool &onMount);
};
//----------------------------------------------------------------------------------
class CPathFinderTest : public CPathFinder
{
protected:
	int CalculateMinMaxZ(int &minZ, int &maxZ, int newX, int newY, const int &currentZ, int newDirection, const int &stepState);

	bool CalculateNewZ(const int &x, const int &y, char &z, const int &direction);

	bool CreateItemsList(vector<CPathObjectTest> &list, const int &x, const int &y, const int &stepState);

public:
	CPathFinderTest() : CPathFinder() {}
	virtual ~CPathFinderTest() {}

	virtual bool CanWalk(uchar &direction, int &x, int &y, char &z);

	virtual bool Walk(bool run, uchar direction);
};
//----------------------------------------------------------------------------------
#if TEST_WALK_CHECK_ALG == 1
extern CPathFinderTest g_PathFinder;
#else
extern CPathFinder g_PathFinder;
#endif
//----------------------------------------------------------------------------------
#endif
//----------------------------------------------------------------------------------
