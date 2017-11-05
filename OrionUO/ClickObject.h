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
//!Объект клика мышкой
class CClickObject
{
	SETGET(uint, Timer, 0);
	SETGET(ushort, X, 0);
	SETGET(ushort, Y, 0);
	SETGET(bool, Enabled, false);
	SETGET(uint, Page, 0);
	SETGETP(CRenderObject*, Object, NULL);
	SETGETP(CGump*, Gump, NULL);

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
