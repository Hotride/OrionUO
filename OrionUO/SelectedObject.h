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
	SETGET(uint, Serial);

private:
	CRenderObject *m_Object;
	CGump *m_Gump;

public:
	CSelectedObject();
	virtual ~CSelectedObject();

	void Clear();
	void Clear(CRenderObject *obj);

	void Init(CRenderObject *obj, CGump *gump = NULL);
	void Init(const CSelectedObject &obj);

	CRenderObject *Object() const { return m_Object; }
	CGump *Gump() const { return m_Gump; }
};
//----------------------------------------------------------------------------------
extern CSelectedObject g_SelectedObject;
extern CSelectedObject g_LastSelectedObject;
//----------------------------------------------------------------------------------
#endif
