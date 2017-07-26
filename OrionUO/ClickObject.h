/***********************************************************************************
**
** ClickObject.h
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#ifndef CLICKOBJECT_H
#define CLICKOBJECT_H
//----------------------------------------------------------------------------------
#include "Globals.h"
#include "RenderObject.h"
#include "Gumps/Gump.h"
//----------------------------------------------------------------------------------
//!Объект клика мышкой
class CClickObject
{
	SETGET(uint, Timer, 0);
	SETGET(ushort, X, 0);
	SETGET(ushort, Y, 0);
	SETGET(bool, Enabled, false);
	SETGET(uint, Page, 0);
	SETGET(CRenderObject*, Object, 0);
	SETGET(CGump*, Gump, 0);

public:
	CClickObject() {}
	~CClickObject() {}

	void Clear(CRenderObject *obj);
	void Clear();

	void Init(CRenderObject *obj, CGump *gump = NULL);
};
//----------------------------------------------------------------------------------
extern CClickObject g_ClickObject; //Ссылка на объект клика
//----------------------------------------------------------------------------------
#endif
//----------------------------------------------------------------------------------
