/***********************************************************************************
**
** GUIHTMLResizepic.h
**
** Компонента для отображения фона HTMLGump'а
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#ifndef GUIHTMLRESIZEPIC_H
#define GUIHTMLRESIZEPIC_H
//----------------------------------------------------------------------------------
#include "GUIResizepic.h"
//----------------------------------------------------------------------------------
class CGUIHTMLResizepic : public CGUIResizepic
{
private:
	//!Гамп-родитель
	class CGUIHTMLGump *m_HTMLGump;

public:
	CGUIHTMLResizepic(class CGUIHTMLGump *htmlGump, const uint &serial, const ushort &graphic, const int &x, const int &y, const int &width, const int &height);
	virtual ~CGUIHTMLResizepic();

	//!Функция прокрутки
	void Scroll(const bool &up, const uint &delay);

	virtual bool IsControlHTML() { return true; }
};
//----------------------------------------------------------------------------------
#endif
//----------------------------------------------------------------------------------
