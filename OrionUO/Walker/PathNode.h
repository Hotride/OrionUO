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
