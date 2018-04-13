// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
/***********************************************************************************
**
** GUIRadio.cpp
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#include "stdafx.h"
//----------------------------------------------------------------------------------
CGUIRadio::CGUIRadio(const uint &serial, const ushort &graphic, const ushort &graphicChecked, const ushort &graphicDisabled, const int &x, const int &y)
: CGUICheckbox(serial, graphic, graphicChecked, graphicDisabled, x, y)
{
	Type = GOT_RADIO;
}
//----------------------------------------------------------------------------------
CGUIRadio::~CGUIRadio()
{
}
//----------------------------------------------------------------------------------
