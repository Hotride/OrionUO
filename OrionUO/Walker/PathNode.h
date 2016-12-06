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
class CPathObjectTest
{
	//Флаги
	SETGET(uint, Flags);

	//Координата Z точки
	SETGET(int, Z);

	//Координата Z точки (усредненная)
	SETGET(int, AverageZ);

	//Высота точки
	SETGET(int, Height);

public:
	CPathObjectTest(const uint &flags, const int &z, const int &averageZ, const int &height, class CRenderWorldObject *object) : m_Flags(flags), m_Z(z), m_AverageZ(averageZ), m_Height(height), m_Object(object) {}
	virtual ~CPathObjectTest() {}

	class CRenderWorldObject *m_Object;
};
//----------------------------------------------------------------------------------
//Класс объекта в точке пути
class CPathObject : public CBaseQueueItem
{
	//Координата Z точки
	SETGET(int, Z);
	//Высота точки
	SETGET(char, Height);
	//Идентификатор поверхности
	SETGET(BYTE, Surface);

public:
	CPathObject(const int &z, const char &height, const char &surface);
	virtual ~CPathObject();
};
//----------------------------------------------------------------------------------
//Класс объекта в точке пути
class CPathNode
{
	SETGET(int, X);
	SETGET(int, Y);
	SETGET(int, Z);
	SETGET(int, Direction);
	SETGET(bool, Used);
	SETGET(int, Cost);
	SETGET(int, DistFromStartCost);
	SETGET(int, DistFromGoalCost);

public:
	CPathNode();
	virtual ~CPathNode();

	CPathNode *m_Parent;

	void Reset();
};
//----------------------------------------------------------------------------------
#endif
//----------------------------------------------------------------------------------
