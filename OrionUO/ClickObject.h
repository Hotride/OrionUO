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
//!ќбъект клика мышкой
class CClickObject
{
	SETGET(uint, Timer);
	SETGET(ushort, X);
	SETGET(ushort, Y);
	SETGET(bool, Enabled);
	SETGET(uint, Page);

private:
	CRenderObject *m_Object;
	CGump *m_Gump;

public:
	CClickObject();
	~CClickObject() {}

	void Clear(CRenderObject *obj);
	void Clear();

	void Init(CRenderObject *obj, CGump *gump = NULL);

	CRenderObject *Object() const { return m_Object; }
	CGump *Gump() const { return m_Gump; }
};
//----------------------------------------------------------------------------------
extern CClickObject g_ClickObject; //—сылка на объект клика
//----------------------------------------------------------------------------------
#endif
//----------------------------------------------------------------------------------
