/***********************************************************************************
**
** GUIMenuObject.h
**
** Компонента для отображения объекта меню
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#ifndef GUIMENUOBJECT_H
#define GUIMENUOBJECT_H
//----------------------------------------------------------------------------------
class CGUIMenuObject : public CGUITilepic
{
	//!Текст объекта
	string Text = "";

public:
	CGUIMenuObject(int serial, ushort graphic, ushort color, int x, int y, const string &text);
	virtual ~CGUIMenuObject();

	virtual bool Select();

	virtual void OnMouseEnter();
	virtual void OnMouseExit();
};
//----------------------------------------------------------------------------------
#endif
//----------------------------------------------------------------------------------
