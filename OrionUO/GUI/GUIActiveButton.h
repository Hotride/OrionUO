/***********************************************************************************
**
** GUIActiveButton.h
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#ifndef GUIACTIVEBUTTON_H
#define GUIACTIVEBUTTON_H
//----------------------------------------------------------------------------------
#include "GUIButton.h"
//----------------------------------------------------------------------------------
class CGUIActiveButton : public CGUIButton
{
public:
	CGUIActiveButton(const uint &serial, const ushort &graphic, const ushort &graphicSelected, const ushort &graphicPressed, const int &x, const int &y);
	virtual ~CGUIActiveButton();

	virtual bool IsPressedOuthit() { return true; }
};
//----------------------------------------------------------------------------------
#endif
//----------------------------------------------------------------------------------
