/***********************************************************************************
**
** GUIButton.h
**
** Компонента для отображения кнопки
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#ifndef GUIBUTTON_H
#define GUIBUTTON_H
//----------------------------------------------------------------------------------
#include "GUIDrawObject.h"
//----------------------------------------------------------------------------------
class CGUIButton : public CGUIDrawObject
{
	//!ИД картинки для состояния, когда мышка находится над кнопкой
	SETGET(ushort, GraphicSelected, 0);

	//!ИД картинки для зажатого состояния
	SETGET(ushort, GraphicPressed, 0);

	//!Индекс страницы, если указан -1 то используется событие OnButton для гампа с идентификатором кнопки, если отличное значение - переход на страницу без вызова события
	SETGET(int, ToPage, -1);

	//!Флаг обработки зажатого состояния кнопки каждую итерацию рендера
	SETGET(bool, ProcessPressedState, false);

public:
	CGUIButton(const uint &serial, const ushort &graphic, const ushort &graphicSelected, const ushort &graphicPressed, const int &x, const int &y);
	virtual ~CGUIButton();

	virtual void PrepareTextures();
	virtual ushort GetDrawGraphic();

	virtual void OnMouseEnter();
	virtual void OnMouseExit();
};
//----------------------------------------------------------------------------------
#endif
//----------------------------------------------------------------------------------
