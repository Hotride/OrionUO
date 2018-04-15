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
//!Custom house component object class
class CCustomHouseMultiObject : public CMultiObject
{
public:
	/*!
	Constructor
	@param [__in] graphic Graphic for item
	@param [__in] color Color for item
	@param [__in] x Coordinate X in the world
	@param [__in] y Coordinate Y in the world
	@param [__in] z Coordinate Z in the world
	@param [__in] flags Multi component flags
	@return
	*/
	CCustomHouseMultiObject(ushort graphic, ushort color, short x, short y, char z, int flags);

	/*!
	Destructor
	@return
	*/
	virtual ~CCustomHouseMultiObject();

	/*!
	Test on Custom House component object class
	@return Always true for this class
	*/
	virtual bool IsCustomHouseMulti() { return true; }
};
//----------------------------------------------------------------------------------
#endif //CUSTOMHOUSEMULTIOBJECT_H
//----------------------------------------------------------------------------------
