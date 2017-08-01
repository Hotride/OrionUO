/***********************************************************************************
**
** PathNode.h
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#ifndef PATHNODE_H
#define PATHNODE_H
//----------------------------------------------------------------------------------
#include "../BaseQueue.h"
#include "../Globals.h"
//----------------------------------------------------------------------------------
struct PATH_POINT
{
	int X;
	int Y;
	int Direction;
};
//----------------------------------------------------------------------------------
//Класс объекта в точке пути
class CPathObject
{
	//Флаги
	SETGET(uint, Flags, 0);

	//Координата Z точки
	SETGET(int, Z, 0);

	//Координата Z точки (усредненная)
	SETGET(int, AverageZ, 0);

	//Высота точки
	SETGET(int, Height, 0);

public:
	CPathObject(const uint &flags, const int &z, const int &averageZ, const int &height, class CRenderWorldObject *object) : m_Flags(flags), m_Z(z), m_AverageZ(averageZ), m_Height(height), m_Object(object) {}
	virtual ~CPathObject() {}

	class CRenderWorldObject *m_Object;
};
//----------------------------------------------------------------------------------
//Класс объекта в точке пути
class CPathNode
{
	SETGET(int, X, 0);
	SETGET(int, Y, 0);
	SETGET(int, Z, 0);
	SETGET(int, Direction, 0);
	SETGET(bool, Used, false);
	SETGET(int, Cost, 0);
	SETGET(int, DistFromStartCost, 0);
	SETGET(int, DistFromGoalCost, 0);

public:
	CPathNode();
	virtual ~CPathNode();

	CPathNode *m_Parent{ NULL };

	void Reset();
};
//----------------------------------------------------------------------------------
#endif
//----------------------------------------------------------------------------------
