/***********************************************************************************
**
** GUIGumppicHightlighted.h
**
** Компонента для отображения гампа с возможностью подсветки (для шахматных фигур)
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#ifndef GUIGUMPPICHIGHTLIGHTED_H
#define GUIGUMPPICHIGHTLIGHTED_H
//----------------------------------------------------------------------------------
#include "GUIDrawObject.h"
//----------------------------------------------------------------------------------
class CGUIGumppicHightlighted : public CGUIDrawObject
{
	//!Цвет подсветки
	SETGET(ushort, SelectedColor);

public:
	CGUIGumppicHightlighted(const uint &serial, const ushort &graphic, const ushort &color, const ushort &electedColor, const int &x, const int &y);
	virtual ~CGUIGumppicHightlighted();

	virtual void SetShaderMode();

	virtual void OnMouseEnter();
	virtual void OnMouseExit();
};
//----------------------------------------------------------------------------------
#endif
//----------------------------------------------------------------------------------
