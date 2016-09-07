/***********************************************************************************
**
** GUILine.h
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#ifndef GUILINE_H
#define GUILINE_H
//----------------------------------------------------------------------------------
#include "BaseGUI.h"
//----------------------------------------------------------------------------------
class CGUILine : public CBaseGUI
{
	SETGET(int, TargetX);
	SETGET(int, TargetY);
	SETGET(uchar, ColorR);
	SETGET(uchar, ColorG);
	SETGET(uchar, ColorB);
	SETGET(uchar, ColorA);

public:
	CGUILine(const int &x, const int &y, const int &targetX, const int &targetY, const uint &polygoneColor);
	virtual ~CGUILine();

	virtual void Draw(const bool &checktrans = false);
};
//----------------------------------------------------------------------------------
#endif
//----------------------------------------------------------------------------------
