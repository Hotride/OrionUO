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
#include "GUIButton.h"
#include "../OrionUO.h"
#include "../Managers/ColorManager.h"
#include "../Managers/MouseManager.h"
#include "../Managers/MulReader.h"
#include "../SelectedObject.h"
//----------------------------------------------------------------------------------
CGUIButton::CGUIButton(const uint &serial, const ushort &graphic, const ushort &graphicSelected, const ushort &graphicPressed, const int &x, const int &y)
: CGUIDrawObject(GOT_BUTTON, serial, graphic, 0, x, y), m_GraphicSelected(graphicSelected),
m_GraphicPressed(graphicPressed), m_ToPage(-1)
{
}
//----------------------------------------------------------------------------------
CGUIButton::~CGUIButton()
{
}
//----------------------------------------------------------------------------------
void CGUIButton::PrepareTextures()
{
	WISPFUN_DEBUG("c39_f1");
	g_Orion.ExecuteGump(m_Graphic);
	g_Orion.ExecuteGump(m_GraphicSelected);
	g_Orion.ExecuteGump(m_GraphicPressed);
}
//----------------------------------------------------------------------------------
ushort CGUIButton::GetDrawGraphic()
{
	WISPFUN_DEBUG("c39_f2");
	ushort graphic = m_Graphic;

	if (g_GumpPressedElement == this)
		graphic = m_GraphicPressed;
	else if (g_GumpSelectedElement == this)
		graphic = m_GraphicSelected;

	return graphic;
}
//----------------------------------------------------------------------------------
void CGUIButton::OnMouseEnter()
{
	WISPFUN_DEBUG("c39_f3");
	if (g_SelectedObject.Gump() != NULL)
		g_SelectedObject.Gump()->WantRedraw = true;
}
//----------------------------------------------------------------------------------
void CGUIButton::OnMouseExit()
{
	WISPFUN_DEBUG("c39_f4");
	if (g_LastSelectedObject.Gump() != NULL)
		g_LastSelectedObject.Gump()->WantRedraw = true;
}
//----------------------------------------------------------------------------------
