// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
/***********************************************************************************
**
** GUIHTMLHitBox.cpp
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#include "stdafx.h"
//----------------------------------------------------------------------------------
CGUIHTMLHitBox::CGUIHTMLHitBox(CGUIHTMLGump *htmlGump, const uint &serial, const int &x, const int &y, const int &width, const int &height, const bool &callOnMouseUp)
: CGUIHitBox(serial, x, y, width, height, callOnMouseUp), m_HTMLGump(htmlGump)
{
}
//----------------------------------------------------------------------------------
CGUIHTMLHitBox::~CGUIHTMLHitBox()
{
}
//----------------------------------------------------------------------------------
void CGUIHTMLHitBox::Scroll(const bool &up, const uint &delay)
{
	WISPFUN_DEBUG("c61_f1");
	if (m_HTMLGump != NULL)
		m_HTMLGump->Scroll(up, delay);
}
//----------------------------------------------------------------------------------
