/***********************************************************************************
**
** GUICheckbox.h
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#ifndef GUICHECKBOX_H
#define GUICHECKBOX_H
//----------------------------------------------------------------------------------
#include "GUIDrawObject.h"
//----------------------------------------------------------------------------------
class CGUICheckbox : public CGUIDrawObject
{
	SETGET(ushort, GraphicChecked);
	SETGET(ushort, GraphicSelected);
	SETGET(ushort, GraphicDisabled);
	SETGET(bool, Checked);

public:
	CGUICheckbox(const uint &serial, const ushort &graphic, const ushort &graphicChecked, const ushort &graphicDisabled, const int &x, const int &y);
	virtual ~CGUICheckbox();

	virtual void PrepareTextures();
	virtual ushort GetDrawGraphic();

	virtual void OnMouseEnter();
	virtual void OnMouseExit();
};
//----------------------------------------------------------------------------------
#endif
//----------------------------------------------------------------------------------
