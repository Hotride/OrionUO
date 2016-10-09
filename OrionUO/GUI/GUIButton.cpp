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
#include "../Managers/UOFileReader.h"
#include "../SelectedObject.h"
//----------------------------------------------------------------------------------
CGUIButton::CGUIButton(const uint &serial, const ushort &graphic, const ushort &graphicSelected, const ushort &graphicPressed, const int &x, const int &y)
: CGUIDrawObject(GOT_BUTTON, serial, graphic, 0, x, y), m_GraphicSelected(graphicSelected),
m_GraphicPressed(graphicPressed), m_ToPage(-1), m_ProcessPressedState(false)
{
}
//----------------------------------------------------------------------------------
CGUIButton::~CGUIButton()
{
}
//----------------------------------------------------------------------------------
void CGUIButton::PrepareTextures()
{
	g_Orion.ExecuteGump(m_Graphic);
	g_Orion.ExecuteGump(m_GraphicSelected);
	g_Orion.ExecuteGump(m_GraphicPressed);
}
//----------------------------------------------------------------------------------
ushort CGUIButton::GetDrawGraphic()
{
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
	if (g_SelectedObject.Gump() != NULL)
		g_SelectedObject.Gump()->WantRedraw = true;
}
//----------------------------------------------------------------------------------
void CGUIButton::OnMouseExit()
{
	if (g_LastSelectedObject.Gump() != NULL)
		g_LastSelectedObject.Gump()->WantRedraw = true;
}
//----------------------------------------------------------------------------------
