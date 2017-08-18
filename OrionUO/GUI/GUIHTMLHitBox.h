/***********************************************************************************
**
** GUIHTMLHitBox.h
**
** Компонента выбора зоны для HTMLGump'а
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#ifndef GUIHTMLHITBOX_H
#define GUIHTMLHITBOX_H
//----------------------------------------------------------------------------------
class CGUIHTMLHitBox : public CGUIHitBox
{
private:
	//!Гамп-родитель
	class CGUIHTMLGump *m_HTMLGump{ NULL };

public:
	CGUIHTMLHitBox(class CGUIHTMLGump *htmlGump, const uint &serial, const int &x, const int &y, const int &width, const int &height, const bool &callOnMouseUp = false);
	virtual ~CGUIHTMLHitBox();

	//!Функция прокрутки
	void Scroll(const bool &up, const uint &delay);

	virtual bool IsControlHTML() { return true; }
};
//----------------------------------------------------------------------------------
#endif
//----------------------------------------------------------------------------------
