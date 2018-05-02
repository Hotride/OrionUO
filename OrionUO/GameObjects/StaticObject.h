/***********************************************************************************
**
** StaticObject.h
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#ifndef STATICOBJECT_H
#define STATICOBJECT_H
//----------------------------------------------------------------------------------
//Класс объекта статики
class CStaticObject : public CRenderStaticObject
{
    //Оригинальный индекс картинки
    ushort OriginalGraphic = 0;

public:
    CStaticObject(int serial, ushort graphic, ushort color, short x, short y, char z);
    virtual ~CStaticObject() {}

    virtual void UpdateGraphicBySeason();

    //Отрисовать объект
    virtual void Draw(int x, int y);

    //Выбрать объект
    virtual void Select(int x, int y);

    //Это объект статики
    bool IsStaticObject() { return true; }
};
//----------------------------------------------------------------------------------
#endif
//----------------------------------------------------------------------------------
