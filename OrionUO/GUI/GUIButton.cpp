// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
/***********************************************************************************
**
** GUIButton.cpp
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#include "stdafx.h"
//----------------------------------------------------------------------------------
CGUIButton::CGUIButton(int serial, ushort graphic, ushort graphicSelected, ushort graphicPressed, int x, int y)
: CGUIDrawObject(GOT_BUTTON, serial, graphic, 0, x, y), GraphicSelected(graphicSelected),
GraphicPressed(graphicPressed)
{
}
//----------------------------------------------------------------------------------
CGUIButton::~CGUIButton()
{
}
//----------------------------------------------------------------------------------
void CGUIButton::PrepareTextures()
{
	WISPFUN_DEBUG("c44_f1");
	g_Orion.ExecuteGump(Graphic);
	g_Orion.ExecuteGump(GraphicSelected);
	g_Orion.ExecuteGump(GraphicPressed);
}
//----------------------------------------------------------------------------------
ushort CGUIButton::GetDrawGraphic()
{
	WISPFUN_DEBUG("c44_f2");
	ushort graphic = Graphic;

	if (g_GumpPressedElement == this)
		graphic = GraphicPressed;
	else if (g_GumpSelectedElement == this)
		graphic = GraphicSelected;

	return graphic;
}
//----------------------------------------------------------------------------------
void CGUIButton::OnMouseEnter()
{
	WISPFUN_DEBUG("c44_f3");
	if (g_SelectedObject.Gump != NULL)
		g_SelectedObject.Gump->WantRedraw = true;
}
//----------------------------------------------------------------------------------
void CGUIButton::OnMouseExit()
{
	WISPFUN_DEBUG("c44_f4");
	if (g_LastSelectedObject.Gump != NULL)
		g_LastSelectedObject.Gump->WantRedraw = true;
}
//----------------------------------------------------------------------------------
