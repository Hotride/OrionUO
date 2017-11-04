// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
/***********************************************************************************
**
** GUIGenericText.cpp
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#include "stdafx.h"
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
	WISPFUN_DEBUG("c54_f1");
	ushort flags = UOFONT_BLACK_BORDER;

	if (m_MaxWidth)
		flags |= UOFONT_CROPPED;

	CreateTextureW((uchar)(g_PacketManager.ClientVersion >= CV_308Z), str, 30, m_MaxWidth, TS_LEFT, flags);
}
//----------------------------------------------------------------------------------
