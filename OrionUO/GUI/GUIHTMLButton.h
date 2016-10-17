/***********************************************************************************
**
** GUIHTMLButton.h
**
** Компонента для кнопок HTMLGump'а
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#ifndef GUIHTMLBUTTON_H
#define GUIHTMLBUTTON_H
//----------------------------------------------------------------------------------
#include "GUIButton.h"
//----------------------------------------------------------------------------------
class CGUIHTMLButton : public CGUIButton
{
private:
	//!Ссылка на гамп-родитель
	class CGUIHTMLGump *m_HTMLGump;

public:
	CGUIHTMLButton(class CGUIHTMLGump *htmlGump, const uint &serial, const ushort &graphic, const ushort &graphicSelected, const ushort &graphicPressed, const int &x, const int &y);
	virtual ~CGUIHTMLButton();

	virtual void SetShaderMode();

	//!Обработка прокрутки
	void Scroll(const bool &up, const uint &delay);

	virtual bool IsControlHTML() { return true; }
};
//----------------------------------------------------------------------------------
#endif
//----------------------------------------------------------------------------------
