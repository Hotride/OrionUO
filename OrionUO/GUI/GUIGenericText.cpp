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
CGUIGenericText::CGUIGenericText(int index, ushort color, int x, int y, int maxWidth)
: CGUIText(color, x, y), TextID(index), MaxWidth(maxWidth)
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

	if (MaxWidth)
		flags |= UOFONT_CROPPED;

	CreateTextureW((uchar)(g_PacketManager.GetClientVersion() >= CV_305D), str, 30, MaxWidth, TS_LEFT, flags);
}
//----------------------------------------------------------------------------------
