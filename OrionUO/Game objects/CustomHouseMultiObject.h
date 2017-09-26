/***********************************************************************************
**
** CustomHouseMultiObject.h
**
** Copyright (C) September 2017 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#ifndef CUSTOMHOUSEMULTIOBJECT_H
#define CUSTOMHOUSEMULTIOBJECT_H
//----------------------------------------------------------------------------------
enum CUSTOM_HOUSE_MULTI_OBJECT_FLAGS
{
	CHMOF_INTERNAL = 0x01,
	CHMOF_TRANSPARENT = 0x02,
	CHMOF_STAIR = 0x04
};
//----------------------------------------------------------------------------------
//Объект для мульти-объекта
class CCustomHouseMultiObject : public CMultiObject
{
	//Прозрачная тесткра
	SETGET(int, State, 0);
	SETGET(int, Dbg, 0);

public:
	CCustomHouseMultiObject(const ushort &graphic, const ushort &color, const short &x, const short &y, const char &z, const uint &flags);
	virtual ~CCustomHouseMultiObject();

	//Отрисовать объект
	virtual void Draw(const int &x, const int &y);

	//Это объект мульти для кастомных домов
	virtual bool IsCustomHouseMulti() { return true; }
};
//----------------------------------------------------------------------------------
#endif //CUSTOMHOUSEMULTIOBJECT_H
//----------------------------------------------------------------------------------
