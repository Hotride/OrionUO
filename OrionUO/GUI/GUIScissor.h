/***********************************************************************************
**
** GUIScissor.h
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#ifndef GUISCISSOR_H
#define GUISCISSOR_H
//----------------------------------------------------------------------------------
#include "GUIPolygonal.h"
//----------------------------------------------------------------------------------
class CGUIScissor : public CGUIPolygonal
{
	SETGET(int, BaseX);
	SETGET(int, BaseY);

public:
	CGUIScissor(const bool &enabled, const int &baseX = 0, const int &baseY = 0, const int &x = 0, const int &y = 0, const int &width = 0, const int &height = 0);
	virtual ~CGUIScissor();

	virtual void Draw(const bool &checktrans = false);
};
//----------------------------------------------------------------------------------
#endif
//----------------------------------------------------------------------------------
