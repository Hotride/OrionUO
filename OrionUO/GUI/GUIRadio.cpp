/***********************************************************************************
**
** GUIRadio.cpp
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#include "GUIRadio.h"
//----------------------------------------------------------------------------------
CGUIRadio::CGUIRadio(const uint &serial, const ushort &graphic, const ushort &graphicChecked, const ushort &graphicDisabled, const int &x, const int &y)
: CGUICheckbox(serial, graphic, graphicChecked, graphicDisabled, x, y)
{
	m_Type = GOT_RADIO;
}
//----------------------------------------------------------------------------------
CGUIRadio::~CGUIRadio()
{
}
//----------------------------------------------------------------------------------
