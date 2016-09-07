/***********************************************************************************
**
** GUIComboboxText.cpp
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#include "GUIComboboxText.h"
#include "../SelectedObject.h"
//----------------------------------------------------------------------------------
CGUIComboboxText::CGUIComboboxText(const ushort &color, const uchar &font, const string &str, const int &width, const TEXT_ALIGN_TYPE &align, const ushort &flags)
: CGUIText(color, 0, 0)
{
	m_Serial = 0xFFFFFFFE;
	CreateTextureA(font, str, width, align, flags);
}
//----------------------------------------------------------------------------------
CGUIComboboxText::CGUIComboboxText(const ushort &color, const uchar &font, const wstring &str, const int &width, const TEXT_ALIGN_TYPE &align, const ushort &flags)
: CGUIText(color, 0, 0)
{
	m_Serial = 0xFFFFFFFE;
	CreateTextureW(font, str, 30, width, align, flags);
}
//----------------------------------------------------------------------------------
CGUIComboboxText::~CGUIComboboxText()
{
}
//----------------------------------------------------------------------------------
void CGUIComboboxText::OnMouseEnter()
{
	if (g_SelectedObject.Gump() != NULL)
		g_SelectedObject.Gump()->WantRedraw = true;
}
//----------------------------------------------------------------------------------
void CGUIComboboxText::OnMouseExit()
{
	if (g_LastSelectedObject.Gump() != NULL)
		g_LastSelectedObject.Gump()->WantRedraw = true;
}
//----------------------------------------------------------------------------------
