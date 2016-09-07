/***********************************************************************************
**
** GUIButtonTileart.h
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
	SETGET(ushort, TileGraphic);
	SETGET(ushort, TileColor);
	SETGET(int, TileX);
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
