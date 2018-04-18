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
class CGUIColoredPolygone : public CGUIPolygonal
{
public:
	//!R-компонента цвета
	uchar ColorR = 0;

	//!G-компонента цвета
	uchar ColorG = 0;

	//!B-компонента цвета
	uchar ColorB = 0;

	//!Значение альфа-канала
	uchar ColorA = 0;

	//!Отрисовывать ли белую точку
	bool DrawDot = false;

	//!Флаг выбора текущей компоненты
	bool Focused = false;

	CGUIColoredPolygone(int serial, ushort color, int x, int y, int width, int height, int polygoneColor);
	virtual ~CGUIColoredPolygone();

	//!Обновить цвет
	void UpdateColor(ushort color, int polygoneColor);

	virtual void Draw(bool checktrans = false);

	virtual void OnMouseEnter();
	virtual void OnMouseExit();
};
//----------------------------------------------------------------------------------
#endif
//----------------------------------------------------------------------------------
