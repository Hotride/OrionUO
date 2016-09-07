/***********************************************************************************
**
** GUIBuff.h
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#ifndef GUIBUFF_H
#define GUIBUFF_H
//----------------------------------------------------------------------------------
#include "GUIDrawObject.h"
//----------------------------------------------------------------------------------
class CGUIBuff : public CGUIDrawObject
{
	SETGET(uint, Timer);
	SETGET(wstring, Text);
	SETGET(uint, TooltipTimer);
	SETGET(bool, DecAlpha);
	SETGET(uchar, Alpha);

public:
	CGUIBuff(const ushort &graphic, const uint &timer, const wstring &text);
	virtual ~CGUIBuff();

	virtual void Draw(const bool &checktrans = false);
};
//----------------------------------------------------------------------------------
#endif
//----------------------------------------------------------------------------------
