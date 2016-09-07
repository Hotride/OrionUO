/***********************************************************************************
**
** GUIHTMLResizepic.cpp
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#include "GUIHTMLResizepic.h"
#include "GUIHTMLGump.h"
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
	if (m_HTMLGump != NULL)
		m_HTMLGump->Scroll(up, delay);
}
//----------------------------------------------------------------------------------
