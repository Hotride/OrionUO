/***********************************************************************************
**
** GUIColoredPolygone.h
**
** Компонента для отображения цветного прямоугольника
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#ifndef GUICOLOREDPOLYGONE_H
#define GUICOLOREDPOLYGONE_H
//----------------------------------------------------------------------------------
#include "GUIPolygonal.h"
//----------------------------------------------------------------------------------
class CGUIColoredPolygone : public CGUIPolygonal
{
	//!R-компонента цвета
	SETGET(uchar, ColorR, 0);

	//!G-компонента цвета
	SETGET(uchar, ColorG, 0);

	//!B-компонента цвета
	SETGET(uchar, ColorB, 0);

	//!Значение альфа-канала
	SETGET(uchar, ColorA, 0);

	//!Отрисовывать ли белую точку
	SETGET(bool, DrawDot, false);

	//!Флаг выбора текущей компоненты
	SETGET(bool, Focused, false);

public:
	CGUIColoredPolygone(const uint &serial, const ushort &color, const int &x, const int &y, const int &width, const int &height, const uint &polygoneColor);
	virtual ~CGUIColoredPolygone();

	//!Обновить цвет
	void UpdateColor(const ushort &color, const uint &polygoneColor);

	virtual void Draw(const bool &checktrans = false);

	virtual void OnMouseEnter();
	virtual void OnMouseExit();
};
//----------------------------------------------------------------------------------
#endif
//----------------------------------------------------------------------------------
