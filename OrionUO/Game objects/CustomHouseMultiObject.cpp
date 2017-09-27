/***********************************************************************************
**
** CustomHouseMultiObject.h
**
** Copyright (C) September 2017 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#include "stdafx.h"
//----------------------------------------------------------------------------------
CCustomHouseMultiObject::CCustomHouseMultiObject(const ushort &graphic, const ushort &color, const short &x, const short &y, const char &z, const uint &flags)
: CMultiObject(graphic, x, y, z, flags)
{
	WISPFUN_DEBUG("");
	m_Color = color;
}
//----------------------------------------------------------------------------------
CCustomHouseMultiObject::~CCustomHouseMultiObject()
{
}
//----------------------------------------------------------------------------------
