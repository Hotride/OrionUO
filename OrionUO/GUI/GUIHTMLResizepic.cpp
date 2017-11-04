// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
/***********************************************************************************
**
** GUIHTMLResizepic.cpp
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#include "stdafx.h"
//----------------------------------------------------------------------------------
CGUIHTMLResizepic::CGUIHTMLResizepic(class CGUIHTMLGump *htmlGump, const uint &serial, const ushort &graphic, const int &x, const int &y, const int &width, const int &height)
: CGUIResizepic(serial, graphic, x, y, width, height), m_HTMLGump(htmlGump)
{
}
//----------------------------------------------------------------------------------
CGUIHTMLResizepic::~CGUIHTMLResizepic()
{
}
//----------------------------------------------------------------------------------
void CGUIHTMLResizepic::Scroll(const bool &up, const uint &delay)
{
	WISPFUN_DEBUG("c62_f1");
	if (m_HTMLGump != NULL)
		m_HTMLGump->Scroll(up, delay);
}
//----------------------------------------------------------------------------------
