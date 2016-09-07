/***********************************************************************************
**
** GUIButton.h
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#ifndef GUIBUTTON_H
#define GUIBUTTON_H
//----------------------------------------------------------------------------------
#include "GUIDrawObject.h"
//----------------------------------------------------------------------------------
class CGUIButton : public CGUIDrawObject
{
	SETGET(ushort, GraphicSelected);
	SETGET(ushort, GraphicPressed);
	SETGET(int, ToPage);
	SETGET(bool, ProcessPressedState);

public:
	CGUIButton(const uint &serial, const ushort &graphic, const ushort &graphicSelected, const ushort &graphicPressed, const int &x, const int &y);
	virtual ~CGUIButton();

	virtual void PrepareTextures();
	virtual ushort GetDrawGraphic();

	virtual void OnMouseEnter();
	virtual void OnMouseExit();
};
//----------------------------------------------------------------------------------
#endif
//----------------------------------------------------------------------------------
