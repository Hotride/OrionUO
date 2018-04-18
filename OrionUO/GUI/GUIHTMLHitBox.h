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
	CGUIHTMLHitBox(class CGUIHTMLGump *htmlGump, int serial, int x, int y, int width, int height, bool callOnMouseUp = false);
	virtual ~CGUIHTMLHitBox();

	//!Функция прокрутки
	void Scroll(bool up, int delay);

	virtual bool IsControlHTML() { return true; }
};
//----------------------------------------------------------------------------------
#endif
//----------------------------------------------------------------------------------
