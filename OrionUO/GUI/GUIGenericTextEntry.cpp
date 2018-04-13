// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
/***********************************************************************************
**
** GUITextEntry.cpp
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#include "stdafx.h"
//----------------------------------------------------------------------------------
CGUIGenericTextEntry::CGUIGenericTextEntry(const uint &serial, const uint &index, const ushort &color, const int &x, const int &y, const int &maxWidth, const int &maxLength)
: CGUITextEntry(serial, color, color, color, x, y, maxWidth, true, (uchar)(g_PacketManager.ClientVersion >= CV_305D), TS_LEFT, UOFONT_BLACK_BORDER, maxLength),
TextID(index)
{
}
//----------------------------------------------------------------------------------
CGUIGenericTextEntry::~CGUIGenericTextEntry()
{
}
//----------------------------------------------------------------------------------
