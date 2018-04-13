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
const int PATHFINDER_MAX_NODES = 10000;
//----------------------------------------------------------------------------------
enum PATH_STEP_STATE
{
	PSS_NORMAL = 0,
	PSS_DEAD_OR_GM,
	PSS_ON_SEA_HORSE,
	PSS_FLYING
};
//----------------------------------------------------------------------------------
enum PATH_OBJECT_FLAGS
{
	POF_IMPASSABLE_OR_SURFACE	= 0x00000001,
	POF_SURFACE					= 0x00000002,
	POF_BRIDGE					= 0x00000004,
	POF_NO_DIAGONAL				= 0x00000008
};
//----------------------------------------------------------------------------------
//Класс для поиска пути и теста шага на точку
class CPathFinder
{
public:
	//Автоматической передвижение (не сложными махинациями с мышкой)
	bool AutoWalking = false;
	bool PathFindidngCanBeCancelled = false;
	bool BlockMoving = false;
	bool FastRotation = false;
	bool IgnoreStaminaCheck = false;

protected:
	//Вычисление новой Z координаты
	bool CalculateNewZ(const int &x, const int &y, char &z, const int &direction);

	//Вычисление минимальной и максимальной Z координат
	int CalculateMinMaxZ(int &minZ, int &maxZ, int newX, int newY, const int &currentZ, int newDirection, const int &stepState);

	//Создание списка предметов, участвующих в поиске в указанных координатах
	bool CreateItemsList(vector<CPathObject> &list, const int &x, const int &y, const int &stepState);

	WISP_GEOMETRY::CPoint2Di m_StartPoint{ WISP_GEOMETRY::CPoint2Di() };
	WISP_GEOMETRY::CPoint2Di m_EndPoint{ WISP_GEOMETRY::CPoint2Di() };

	int m_GoalNode{ 0 };
	bool m_GoalFound{ 0 };

	int m_ActiveOpenNodes{ 0 };
	int m_ActiveClosedNodes{ 0 };

	int m_PathFindDistance{ 0 };

	CPathNode m_OpenList[PATHFINDER_MAX_NODES];
	CPathNode m_ClosedList[PATHFINDER_MAX_NODES];

	//Список точек пути
	CPathNode *m_Path[PATHFINDER_MAX_NODES];

	//Текущая точка пути
	int m_PointIndex{ 0 };

	//Размер точек пути
	int m_PathSize{ 0 };

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

	//Вычисление новых XY координат
	void GetNewXY(const uchar &direction, int &x, int &y);

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
