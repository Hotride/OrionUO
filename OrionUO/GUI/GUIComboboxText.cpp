// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
/***********************************************************************************
**
** GUIComboboxText.cpp
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#include "stdafx.h"
//----------------------------------------------------------------------------------
CGUIComboboxText::CGUIComboboxText(ushort color, uchar font, const string &str, int width, TEXT_ALIGN_TYPE align, ushort flags)
: CGUIText(color, 0, 0)
{
	WISPFUN_DEBUG("c50_f1");
	Serial = 0xFFFFFFFE;
	CreateTextureA(font, str, width, align, flags);
}
//----------------------------------------------------------------------------------
CGUIComboboxText::CGUIComboboxText(ushort color, uchar font, const wstring &str, int width, TEXT_ALIGN_TYPE align, ushort flags)
: CGUIText(color, 0, 0)
{
	WISPFUN_DEBUG("c50_f2");
	Serial = 0xFFFFFFFE;
	CreateTextureW(font, str, 30, width, align, flags);
}
//----------------------------------------------------------------------------------
CGUIComboboxText::~CGUIComboboxText()
{
}
//----------------------------------------------------------------------------------
void CGUIComboboxText::OnMouseEnter()
{
	WISPFUN_DEBUG("c50_f3");
	if (g_SelectedObject.Gump != NULL)
		g_SelectedObject.Gump->WantRedraw = true;
}
//----------------------------------------------------------------------------------
void CGUIComboboxText::OnMouseExit()
{
	WISPFUN_DEBUG("c50_f4");
	if (g_LastSelectedObject.Gump != NULL)
		g_LastSelectedObject.Gump->WantRedraw = true;
}
//----------------------------------------------------------------------------------
