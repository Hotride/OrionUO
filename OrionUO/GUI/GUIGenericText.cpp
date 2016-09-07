/***********************************************************************************
**
** GUIGenericText.cpp
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#include "GUIGenericText.h"
//----------------------------------------------------------------------------------
CGUIGenericText::CGUIGenericText(const uint &index, const ushort &color, const int &x, const int &y, const int &maxWidth)
: CGUIText(color, x, y), m_TextID(index), m_MaxWidth(maxWidth)
{
}
//----------------------------------------------------------------------------------
CGUIGenericText::~CGUIGenericText()
{
}
//----------------------------------------------------------------------------------
void CGUIGenericText::CreateTexture(const wstring &str)
{
	ushort flags = UOFONT_BLACK_BORDER;

	if (m_MaxWidth)
		flags |= UOFONT_CROPPED;

	CreateTextureW(0, str, 30, m_MaxWidth, TS_LEFT, flags);
}
//----------------------------------------------------------------------------------
