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
	CStaticObject(const uint &serial, const ushort &graphic, const ushort &color, const short &x, const short &y, const char &z);
	virtual ~CStaticObject() {}

	virtual void UpdateGraphicBySeason();

	//Отрисовать объект
	virtual void Draw(const int &x, const int &y);

	//Выбрать объект
	virtual void Select(const int &x, const int &y);

	//Это объект статики
	bool IsStaticObject() {return true;}
};
//----------------------------------------------------------------------------------
#endif
//----------------------------------------------------------------------------------
