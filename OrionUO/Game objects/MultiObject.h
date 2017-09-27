/***********************************************************************************
**
** MultiObject.h
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#ifndef MULTIOBJECT_H
#define MULTIOBJECT_H
//----------------------------------------------------------------------------------
//Объект для мульти-объекта
class CMultiObject : public CRenderStaticObject
{
	//Мульти с таргета
	SETGET(bool, OnTarget, false);

	//Оригинальный индекс картинки
	SETGET(ushort, OriginalGraphic, 0);

	//Состояние объекта (для конструктора кастомных домов)
	SETGET(int, State, 0);

public:
	CMultiObject(const ushort &graphic, const short &x, const short &y, const char &z, const uint &flags);
	virtual ~CMultiObject();

	virtual void UpdateGraphicBySeason();

	//Отрисовать объект
	virtual void Draw(const int &x, const int &y);

	//Выбрать объект
	virtual void Select(const int &x, const int &y);

	//Это объект мульти
	bool IsMultiObject() { return true; }

	//Это объект мульти для кастомных домов
	virtual bool IsCustomHouseMulti() { return false; }
};
//----------------------------------------------------------------------------------
#endif
//----------------------------------------------------------------------------------
