/***********************************************************************************
**
** GUIButtonTileart.h
**
** Компонента для отрисовки кнопки (и всего ее функционала) с картинкой статики
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#ifndef GUIBUTTONTILEART_H
#define GUIBUTTONTILEART_H
//----------------------------------------------------------------------------------
#include "GUIButton.h"
//----------------------------------------------------------------------------------
class CGUIButtonTileart : public CGUIButton
{
	//!ИД картинки статики
	SETGET(ushort, TileGraphic);

	//!Цвет картинки статики
	SETGET(ushort, TileColor);

	//!Координата по оси X для отображения статики
	SETGET(int, TileX);

	//!Координата по оси Y для отображения статики
	SETGET(int, TileY);

public:
	CGUIButtonTileart(const uint &serial, const ushort &graphic, const ushort &graphicSelected, const ushort &graphicPressed, const int &x, const int &y, const ushort &tileGraphic, const ushort &tileColor, const int &tileX, const int &tileY);
	virtual ~CGUIButtonTileart();

	virtual WISP_GEOMETRY::CSize GetSize();

	virtual void PrepareTextures();

	virtual void Draw(const bool &checktrans = false);
	virtual bool Select();
};
//----------------------------------------------------------------------------------
#endif
//----------------------------------------------------------------------------------
