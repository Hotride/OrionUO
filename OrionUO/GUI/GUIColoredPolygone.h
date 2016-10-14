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
	SETGET(uchar, ColorR);

	//!G-компонента цвета
	SETGET(uchar, ColorG);

	//!B-компонента цвета
	SETGET(uchar, ColorB);

	//!Значение альфа-канала
	SETGET(uchar, ColorA);

	//!Отрисовывать ли белую точку
	SETGET(bool, DrawDot);

	//!Флаг выбора текущей компоненты
	SETGET(bool, Focused);

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
