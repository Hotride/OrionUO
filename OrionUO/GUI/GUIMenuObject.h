/***********************************************************************************
**
** GUIMenuObject.h
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#ifndef GUIMENUOBJECT_H
#define GUIMENUOBJECT_H
//----------------------------------------------------------------------------------
#include "GUITilepic.h"
//----------------------------------------------------------------------------------
class CGUIMenuObject : public CGUITilepic
{
	SETGET(string, Text);

public:
	CGUIMenuObject(const uint &serial, const ushort &graphic, const ushort &color, const int &x, const int &y, const string &text);
	virtual ~CGUIMenuObject();

	virtual bool Select();

	virtual void OnMouseEnter();
	virtual void OnMouseExit();
};
//----------------------------------------------------------------------------------
#endif
//----------------------------------------------------------------------------------
