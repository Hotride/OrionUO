// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
/***********************************************************************************
**
** GUIGumppic.cpp
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#include "stdafx.h"
//----------------------------------------------------------------------------------
CGUIGumppic::CGUIGumppic(const ushort &graphic, const int &x, const int &y)
: CGUIDrawObject(GOT_GUMPPIC, 0, graphic, 0, x, y)
{
}
//----------------------------------------------------------------------------------
CGUIGumppic::~CGUIGumppic()
{
}
//----------------------------------------------------------------------------------
