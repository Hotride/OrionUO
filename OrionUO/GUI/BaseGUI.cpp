// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
/***********************************************************************************
**
** BaseGUI.cpp
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#include "stdafx.h"
//----------------------------------------------------------------------------------
CBaseGUI::CBaseGUI(GUMP_OBJECT_TYPE type, int serial, ushort graphic, ushort color, int x, int y)
: CRenderObject(serial, graphic, color, x, y), Type(type)
{
}
//----------------------------------------------------------------------------------
CBaseGUI::~CBaseGUI()
{
}
//----------------------------------------------------------------------------------
