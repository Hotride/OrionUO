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
	CGUITilepicHightlighted(int serial, ushort graphic, ushort color, ushort selectedColor, int x, int y, bool doubleDraw);
	virtual ~CGUITilepicHightlighted();

	virtual void SetShaderMode();

	virtual void Draw(bool checktrans = false);
	virtual bool Select();

	virtual void OnMouseEnter();
	virtual void OnMouseExit();
};
//----------------------------------------------------------------------------------
#endif
//----------------------------------------------------------------------------------
