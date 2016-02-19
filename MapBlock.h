/****************************************************************************
**
** MapBlock.h
**
** Copyright (C) September 2015 Hotride
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
#ifndef MapBlockH
#define MapBlockH
//---------------------------------------------------------------------------
//Класс для работы с блоками карты 8х8
class TMapBlock : public TBaseQueueItem
{
private:
	//Индекс блока
	DWORD m_Index;

	//Время последнего доступа в блок
	DWORD m_LastAccessTime;

	//Координаты блока (в карте блоков)
	short m_X;
	short m_Y;

	//Получить Z координату ландшафта
	char GetLandZ(int x, int y, int &map);

	//Получить Z координату объекта рендера
	char GetRenderZ(TRenderWorldObject *item);

	//Проверить объект ландшафта на "растягиваемость" при рендере
	bool TestStretched(int x, int y, char &z, int &map, bool recurse);
public:
	TMapBlock(DWORD index);
	virtual ~TMapBlock();

	SETGET(DWORD, Index);
	SETGET(DWORD, LastAccessTime);
	SETGET(short, X);
	SETGET(short, Y);

	//Данные блока
	TMapObject *Block[8][8];

	//Добавить объект
	TMapObject *AddObject(TMapObject *obj, int x, int y);

	//Получить указатель на объект ландшафта
	TLandObject *GetLand(int &x, int &y);

	//Добавить объект в очередь рендера
	void AddRender(TRenderWorldObject *item, int &x, int &y);

	//Получить начало очереди рендера
	TRenderWorldObject *GetRender(int &x, int &y);

	//Получить цвет точки для радара
	WORD GetRadarColor(int x, int y);

	//Сконструировать вершины текстур ландшафта
	void CreateLandTextureRect();
};
//---------------------------------------------------------------------------
#endif