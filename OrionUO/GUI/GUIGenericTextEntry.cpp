/***********************************************************************************
**
** GUITextEntry.cpp
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#include "GUIGenericTextEntry.h"
//----------------------------------------------------------------------------------
CGUIGenericTextEntry::CGUIGenericTextEntry(const uint &serial, const uint &index, const ushort &color, const int &x, const int &y, const int &maxWidth, const int &maxLength)
: CGUITextEntry(serial, color, 0, 0, x, y, maxWidth, true, 0, TS_LEFT, UOFONT_BLACK_BORDER, maxLength),
m_TextID(index)
{
}
//----------------------------------------------------------------------------------
CGUIGenericTextEntry::~CGUIGenericTextEntry()
{
}
//----------------------------------------------------------------------------------
