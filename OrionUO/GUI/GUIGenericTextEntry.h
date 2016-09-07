/***********************************************************************************
**
** GUIGenericTextEntry.h
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#ifndef GUIGENERICTEXTENTRY_H
#define GUIGENERICTEXTENTRY_H
//----------------------------------------------------------------------------------
#include "GUITextEntry.h"
//----------------------------------------------------------------------------------
class CGUIGenericTextEntry : public CGUITextEntry
{
	SETGET(uint, TextID);

public:
	CGUIGenericTextEntry(const uint &serial, const uint &index, const ushort &color, const int &x, const int &y, const int &maxWidth = 0, const int &maxLength = 0);
	virtual ~CGUIGenericTextEntry();
};
//----------------------------------------------------------------------------------
#endif
//----------------------------------------------------------------------------------
