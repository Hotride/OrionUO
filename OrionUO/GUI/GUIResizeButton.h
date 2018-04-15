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
class CGUIResizeButton : public CGUIButton
{
public:
	CGUIResizeButton(int serial, ushort graphic, ushort graphicSelected, ushort graphicPressed, int x, int y);
	virtual ~CGUIResizeButton();

	virtual bool IsPressedOuthit() { return true; }
};
//----------------------------------------------------------------------------------
#endif
//----------------------------------------------------------------------------------
