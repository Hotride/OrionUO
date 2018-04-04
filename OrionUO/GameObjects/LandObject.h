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
	//Минимальная Z координата
	SETGET(char, MinZ, 0);

	//Средняя Z координата
	SETGET(char, AverageZ, 0);

	//Флаг отображения (true - картинка из texmaps, false - из art.mul)
	SETGET(bool, IsStretched, false);

	//Оригинальный индекс картинки
	SETGET(ushort, OriginalGraphic, 0);

	//!Буфер текстурных координат
	SETGET(GLuint, PositionBuffer, 0);

	//!Буфер вершин
	SETGET(GLuint, VertexBuffer, 0);

	//!Буфер нормалей
	SETGET(GLuint, NormalBuffer, 0);

private:
	int GetDirectionZ(const int &direction);

public:
	CLandObject(const uint &serial, const ushort &graphic, const ushort &color, const short &x, const short &y, const char &z);
	virtual ~CLandObject();

	//Координаты привязки вершин
	RECT m_Rect;

	virtual void UpdateGraphicBySeason();

	int CalculateCurrentAverageZ(const int &z);

	//Векторы нормали
	CVector m_Normals[4];

	//Отрисовать объект
	virtual void Draw(const int &x, const int &y);

	//Выбрать объект
	virtual void Select(const int &x, const int &y);

	//Это объект ландшафта
	bool IsLandObject() {return true;}

	void UpdateZ(const int &zTop, const int &zRight, const int &zBottom);

	//Игнорирование при некоторых расчетах
	bool Ignored() {return (Graphic == 2 || Graphic == 0x1DB || (Graphic >= 0x1AE && Graphic <= 0x1B5));}

	virtual CLandObject *LandObjectPtr() { return this; }
};
//---------------------------------------------------------------------------
#endif