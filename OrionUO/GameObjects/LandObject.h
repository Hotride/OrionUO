/***********************************************************************************
**
** LandObject.h
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#ifndef LANDOBJECT_H
#define LANDOBJECT_H
//----------------------------------------------------------------------------------
//Класс ландшафта
class CLandObject : public CMapObject
{
public:
    //Минимальная Z координата
    char MinZ = 0;

    //Средняя Z координата
    char AverageZ = 0;

    //Флаг отображения (true - картинка из texmaps, false - из art.mul)
    bool IsStretched = false;

    //Оригинальный индекс картинки
    ushort OriginalGraphic = 0;

    //!Буфер текстурных координат
    GLuint PositionBuffer = 0;

    //!Буфер вершин
    GLuint VertexBuffer = 0;

    //!Буфер нормалей
    GLuint NormalBuffer = 0;

private:
    int GetDirectionZ(int direction);

public:
    CLandObject(int serial, ushort graphic, ushort color, short x, short y, char z);
    virtual ~CLandObject();

    //Координаты привязки вершин
    RECT m_Rect;

    virtual void UpdateGraphicBySeason();

    int CalculateCurrentAverageZ(int z);

    //Векторы нормали
    CVector m_Normals[4];

    //Отрисовать объект
    virtual void Draw(int x, int y);

    //Выбрать объект
    virtual void Select(int x, int y);

    //Это объект ландшафта
    bool IsLandObject() { return true; }

    void UpdateZ(int zTop, int zRight, int zBottom);

    //Игнорирование при некоторых расчетах
    bool Ignored()
    {
        return (Graphic == 2 || Graphic == 0x1DB || (Graphic >= 0x1AE && Graphic <= 0x1B5));
    }

    virtual CLandObject *LandObjectPtr() { return this; }
};
//---------------------------------------------------------------------------
#endif