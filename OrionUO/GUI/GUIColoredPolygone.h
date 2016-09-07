/***********************************************************************************
**
** GUIColoredPolygone.h
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
	SETGET(uchar, ColorR);
	SETGET(uchar, ColorG);
	SETGET(uchar, ColorB);
	SETGET(uchar, ColorA);
	SETGET(bool, DrawDot);
	SETGET(bool, Focused);

public:
	CGUIColoredPolygone(const uint &serial, const ushort &color, const int &x, const int &y, const int &width, const int &height, const uint &polygoneColor);
	virtual ~CGUIColoredPolygone();

	void UpdateColor(const ushort &color, const uint &polygoneColor);

	virtual void Draw(const bool &checktrans = false);

	virtual void OnMouseEnter();
	virtual void OnMouseExit();
};
//----------------------------------------------------------------------------------
#endif
//----------------------------------------------------------------------------------
