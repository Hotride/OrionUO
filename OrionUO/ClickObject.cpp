// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
/***********************************************************************************
**
** ClickObject.cpp
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#include "stdafx.h"
//----------------------------------------------------------------------------------
CClickObject g_ClickObject;
//----------------------------------------------------------------------------------
void CClickObject::Clear(CRenderObject *obj)
{
	WISPFUN_DEBUG("c185_f1");
	if (obj == Object)
	{
		Object = NULL;
		Gump = NULL;
		Timer = 0;
		X = 0;
		Y = 0;
		Enabled = false;
		Page = 0;
	}
}
//----------------------------------------------------------------------------------
void CClickObject::Clear()
{
	WISPFUN_DEBUG("c185_f2");
	Object = NULL;
	Gump = NULL;
	Timer = 0;
	//m_X = 0;
	//m_Y = 0;
	Enabled = false;
	Page = 0;
}
//----------------------------------------------------------------------------------
void CClickObject::Init(CRenderObject *obj, CGump *gump)
{
	WISPFUN_DEBUG("c185_f3");
	Enabled = true;
	Object = obj;
	Gump = gump;
	Timer = 0;
	X = 0;
	Y = 0;
	Page = 0;
}
//----------------------------------------------------------------------------------
