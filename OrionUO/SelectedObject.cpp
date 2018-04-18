// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
/***********************************************************************************
**
** SelectedObject.cpp
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#include "stdafx.h"
//----------------------------------------------------------------------------------
CSelectedObject g_SelectedObject;
CSelectedObject g_LastSelectedObject;
//----------------------------------------------------------------------------------
CSelectedObject::CSelectedObject()
{
}
//----------------------------------------------------------------------------------
CSelectedObject::~CSelectedObject()
{
}
//----------------------------------------------------------------------------------
void CSelectedObject::Clear()
{
	WISPFUN_DEBUG("c205_f1");
	Object = NULL;
	Gump = NULL;
	Serial = 0;
}
//----------------------------------------------------------------------------------
void CSelectedObject::Clear(CRenderObject *obj)
{
	WISPFUN_DEBUG("c205_f2");
	if (obj == Object)
	{
		Object = NULL;
		Gump = NULL;
		Serial = 0;
	}
}
//----------------------------------------------------------------------------------
void CSelectedObject::Init(CRenderObject *obj, CGump *gump)
{
	WISPFUN_DEBUG("c205_f3");
	Object = obj;
	Gump = gump;
	Serial = (obj != NULL ? obj->Serial : 0);
}
//----------------------------------------------------------------------------------
void CSelectedObject::Init(const CSelectedObject &obj)
{
	WISPFUN_DEBUG("c205_f4");
	Object = obj.Object;
	Gump = obj.Gump;
	Serial = (obj.Object != NULL ? obj.Object->Serial : 0);
}
//----------------------------------------------------------------------------------
