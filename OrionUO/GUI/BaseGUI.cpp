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
CBaseGUI::CBaseGUI(const GUMP_OBJECT_TYPE &type, const uint &serial, const ushort &graphic, const ushort &color, const int &x, const int &y)
: CRenderObject(serial, graphic, color, x, y), Type(type)
{
}
//----------------------------------------------------------------------------------
CBaseGUI::~CBaseGUI()
{
}
//----------------------------------------------------------------------------------
