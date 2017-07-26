/***********************************************************************************
**
** SelectedObject.h
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#ifndef SELECTEDOBJECT_H
#define SELECTEDOBJECT_H
//----------------------------------------------------------------------------------
#include "Globals.h"
#include "Gumps/Gump.h"
#include "GUI/BaseGUI.h"
//----------------------------------------------------------------------------------
class CSelectedObject
{
	SETGET(uint, Serial, 0);
	SETGET(CRenderObject*, Object, 0);
	SETGET(CGump*, Gump, 0);

public:
	CSelectedObject();
	virtual ~CSelectedObject();

	void Clear();
	void Clear(CRenderObject *obj);

	void Init(CRenderObject *obj, CGump *gump = NULL);
	void Init(const CSelectedObject &obj);
};
//----------------------------------------------------------------------------------
extern CSelectedObject g_SelectedObject;
extern CSelectedObject g_LastSelectedObject;
//----------------------------------------------------------------------------------
#endif
