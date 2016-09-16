/***********************************************************************************
**
** GUIResizeButton.h
**
** Компонента для кнопок изменения размера
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#ifndef GUIRESIZEBUTTON_H
#define GUIRESIZEBUTTON_H
//----------------------------------------------------------------------------------
#include "GUIButton.h"
//----------------------------------------------------------------------------------
class CGUIResizeButton : public CGUIButton
{
public:
	CGUIResizeButton(const uint &serial, const ushort &graphic, const ushort &graphicSelected, const ushort &graphicPressed, const int &x, const int &y);
	virtual ~CGUIResizeButton();

	virtual bool IsPressedOuthit() { return true; }
};
//----------------------------------------------------------------------------------
#endif
//----------------------------------------------------------------------------------
