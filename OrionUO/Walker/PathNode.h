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
public:
	//Флаги
	uint Flags = 0;

	//Координата Z точки
	int Z = 0;

	//Координата Z точки (усредненная)
	int AverageZ = 0;

	//Высота точки
	int Height = 0;

	CPathObject(const uint &flags, const int &z, const int &averageZ, const int &height, class CRenderWorldObject *object) : Flags(flags), Z(z), AverageZ(averageZ), Height(height), m_Object(object) {}
	virtual ~CPathObject() {}

	class CRenderWorldObject *m_Object;
};
//----------------------------------------------------------------------------------
//Класс объекта в точке пути
class CPathNode
{
public:
	int X = 0;
	int Y = 0;
	int Z = 0;
	int Direction = 0;
	bool Used = false;
	int Cost = 0;
	int DistFromStartCost = 0;
	int DistFromGoalCost = 0;

	CPathNode();
	virtual ~CPathNode();

	CPathNode *m_Parent{ NULL };

	void Reset();
};
//----------------------------------------------------------------------------------
#endif
//----------------------------------------------------------------------------------
