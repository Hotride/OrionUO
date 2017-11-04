// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
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
