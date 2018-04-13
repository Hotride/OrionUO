/***********************************************************************************
**
** MapBlock.h
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#ifndef MAPBLOCK_H
#define MAPBLOCK_H
//----------------------------------------------------------------------------------
//Класс для работы с блоками карты 8х8
class CMapBlock : public CBaseQueueItem
{
public:
	uint Index = 0;
	uint LastAccessTime = 0;
	short X = 0;
	short Y = 0;

private:
	//Получить Z координату ландшафта
	char GetLandZ(const int &x, const int &y, const int &map);

	//Проверить объект ландшафта на "растягиваемость" при рендере
	bool TestStretched(const int &x, const int &y, const char &z, const int &map, const bool &recurse);

public:
	CMapBlock(const uint &index);
	virtual ~CMapBlock();

	//Данные блока
	CMapObject *Block[8][8];

	//Добавить объект
	CMapObject *AddObject(CMapObject *obj, const int &x, const int &y);

	//Получить указатель на объект ландшафта
	CLandObject *GetLand(const int &x, const int &y);

	//Добавить объект в очередь рендера
	void AddRender(CRenderWorldObject *item, const int &x, const int &y);

	//Получить начало очереди рендера
	CRenderWorldObject *GetRender(const int &x, const int &y);

	//Получить цвет точки для радара
	ushort GetRadarColor(const int &x, const int &y);

	//Сконструировать вершины текстур ландшафта
	void CreateLandTextureRect();

	bool HasNoExternalData();
};
//---------------------------------------------------------------------------
#endif