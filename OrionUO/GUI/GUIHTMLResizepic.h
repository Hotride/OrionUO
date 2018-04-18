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
class CGUIHTMLResizepic : public CGUIResizepic
{
private:
	//!Гамп-родитель
	class CGUIHTMLGump *m_HTMLGump{ NULL };

public:
	CGUIHTMLResizepic(class CGUIHTMLGump *htmlGump, int serial, ushort graphic, int x, int y, int width, int height);
	virtual ~CGUIHTMLResizepic();

	//!Функция прокрутки
	void Scroll(bool up, int delay);

	virtual bool IsControlHTML() { return true; }
};
//----------------------------------------------------------------------------------
#endif
//----------------------------------------------------------------------------------
