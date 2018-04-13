/***********************************************************************************
**
** GUITilepicHightlighted.h
**
** Компонента для отображения картинки статики с возможностью подсветки
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#ifndef GUITILEPICHIGHTLIGHTED_H
#define GUITILEPICHIGHTLIGHTED_H
//----------------------------------------------------------------------------------
class CGUITilepicHightlighted : public CGUITilepic
{
	//!Цвет выбранной компоненты
	ushort SelectedColor = 0;

	//!Дублировать картинку при отображении
	bool DoubleDraw = false;

public:
	CGUITilepicHightlighted(const uint &serial, const ushort &graphic, const ushort &color, const ushort &selectedColor, const int &x, const int &y, const bool &doubleDraw);
	virtual ~CGUITilepicHightlighted();

	virtual void SetShaderMode();

	virtual void Draw(const bool &checktrans = false);
	virtual bool Select();

	virtual void OnMouseEnter();
	virtual void OnMouseExit();
};
//----------------------------------------------------------------------------------
#endif
//----------------------------------------------------------------------------------
