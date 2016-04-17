/****************************************************************************
**
** PathFinder.h
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
#ifndef PathFinderH
#define PathFinderH
//---------------------------------------------------------------------------
//Класс объекта в точке пути
class TPathObject : public TBaseQueueItem
{
private:
	//Координата Z точки
	int m_Z;

	//Высота точки
	char m_Height;

	//Идентификатор поверхности
	BYTE m_Surface;
public:
	TPathObject(int z, char height, char surface);
	virtual ~TPathObject();

	SETGET(int, Z);
	SETGET(char, Height);
	SETGET(BYTE, Surface);
};
//---------------------------------------------------------------------------
struct PATH_POINT
{
	int X;
	int Y;
	int Direction;
};
//---------------------------------------------------------------------------
//Класс для поиска пути и теста шага на точку
class TPathFinder : public TBaseQueue
{
private:
	//Вычисление новой Z координаты
	bool CalculateNewZ(int &x, int &y, char &z);

	//Вычисление новых XY координат
	void GetNewXY(BYTE &direction, int &x, int &y);

	//Создание списка предметов, участвующих в поиске в указанных координатах
	bool CreateItemsList(int &x, int &y);

	//Флаг, указывающий. что персонаж стоит на длинной лестнице
	bool m_OnLongStair;

	//Проверка на лестницу под ногами
	void CheckLongStairUnderfoot(int &x, int &y, char &z);

	//Автоматической передвижение (не сложными махинациями с мышкой)
	bool m_AutoWalking;

	//Список точек пути
	PATH_POINT *m_Path;

	//Текущая точка пути
	int m_PointIndex;

	//Размер точек пути
	int m_PathSize;
public:
	TPathFinder();
	virtual ~TPathFinder();

	SETGET(bool, AutoWalking);

	//Проверка на возможность сделать шаг в указанные координаты
	bool CanWalk(BYTE &direction, int &x, int &y, char &z);

	//Пойти в указанные координаты
	bool Walk(bool run, BYTE direction);

	//Вычислить новый путь
	PATH_POINT *CalculatePath(int &size, int x, int y, int z);

	//Переместиться в указанные координаты
	bool WalkTo(int x, int y, int z);

	//Обработка пути
	void ProcessAutowalk();

	//Остановить автоматическую ходилку
	void StopAutoWalk();
};
//---------------------------------------------------------------------------
extern TPathFinder *PathFinder;
//---------------------------------------------------------------------------
#endif