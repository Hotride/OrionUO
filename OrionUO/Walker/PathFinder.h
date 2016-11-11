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
//----------------------------------------------------------------------------------
//Класс для поиска пути и теста шага на точку
class CPathFinder : public CBaseQueue
{
	//Автоматической передвижение (не сложными махинациями с мышкой)
	SETGET(bool, AutoWalking);
	SETGET(bool, PathFindidngCanBeCancelled);
	SETGET(bool, BlockMoving);

private:
	//Вычисление новой Z координаты
	bool CalculateNewZ(int &x, int &y, char &z);

	//Вычисление новых XY координат
	void GetNewXY(uchar &direction, int &x, int &y);

	//Создание списка предметов, участвующих в поиске в указанных координатах
	bool CreateItemsList(int &x, int &y, char &z);

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
	bool CanWalk(uchar &direction, int &x, int &y, char &z);

	//Пойти в указанные координаты
	bool Walk(bool run, uchar direction);

	//Переместиться в указанные координаты
	bool WalkTo(int x, int y, int z, int distance);

	//Обработка пути
	void ProcessAutowalk();

	//Остановить автоматическую ходилку
	void StopAutoWalk();

	int GetWalkSpeed(const bool &run, const bool &onMount);
};
//----------------------------------------------------------------------------------
extern CPathFinder g_PathFinder;
//----------------------------------------------------------------------------------
#endif
//----------------------------------------------------------------------------------
