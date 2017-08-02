/***********************************************************************************
**
** GUIResizeButton.cpp
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#include "stdafx.h"
//----------------------------------------------------------------------------------
CGUIResizeButton::CGUIResizeButton(const uint &serial, const ushort &graphic, const ushort &graphicSelected, const ushort &graphicPressed, const int &x, const int &y)
: CGUIButton(serial, graphic, graphicSelected, graphicPressed, x, y)
{
	m_Type = GOT_RESIZEBUTTON;
}
//----------------------------------------------------------------------------------
CGUIResizeButton::~CGUIResizeButton()
{
}
//----------------------------------------------------------------------------------
