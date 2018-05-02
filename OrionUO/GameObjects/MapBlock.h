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
    char GetLandZ(int x, int y, int map);

    //Проверить объект ландшафта на "растягиваемость" при рендере
    bool TestStretched(int x, int y, char z, int map, bool recurse);

public:
    CMapBlock(int index);
    virtual ~CMapBlock();

    //Данные блока
    CMapObject *Block[8][8];

    //Добавить объект
    CMapObject *AddObject(CMapObject *obj, int x, int y);

    //Получить указатель на объект ландшафта
    CLandObject *GetLand(int x, int y);

    //Добавить объект в очередь рендера
    void AddRender(CRenderWorldObject *item, int x, int y);

    //Получить начало очереди рендера
    CRenderWorldObject *GetRender(int x, int y);

    //Получить цвет точки для радара
    ushort GetRadarColor(int x, int y);

    //Сконструировать вершины текстур ландшафта
    void CreateLandTextureRect();

    bool HasNoExternalData();
};
//---------------------------------------------------------------------------
#endif