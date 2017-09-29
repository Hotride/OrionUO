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
//Объект для мульти-объекта
class CCustomHouseMultiObject : public CMultiObject
{
public:
	CCustomHouseMultiObject(const ushort &graphic, const ushort &color, const short &x, const short &y, const char &z, const uint &flags);
	virtual ~CCustomHouseMultiObject();

	//Это объект мульти для кастомных домов
	virtual bool IsCustomHouseMulti() { return true; }
};
//----------------------------------------------------------------------------------
#endif //CUSTOMHOUSEMULTIOBJECT_H
//----------------------------------------------------------------------------------
