/***********************************************************************************
**
** GUICheckbox.cpp
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#include "GUICheckbox.h"
#include "../OrionUO.h"
#include "../SelectedObject.h"
//----------------------------------------------------------------------------------
CGUICheckbox::CGUICheckbox(const uint &serial, const ushort &graphic, const ushort &graphicChecked, const ushort &graphicDisabled, const int &x, const int &y)
: CGUIDrawObject(GOT_CHECKBOX, serial, graphic, 0, x, y), m_GraphicChecked(graphicChecked),
m_GraphicSelected(graphic), m_GraphicDisabled(graphicDisabled), m_Checked(false)
{
}
//----------------------------------------------------------------------------------
CGUICheckbox::~CGUICheckbox()
{
}
//----------------------------------------------------------------------------------
void CGUICheckbox::PrepareTextures()
{
	g_Orion.ExecuteGump(m_Graphic);
	g_Orion.ExecuteGump(m_GraphicChecked);
	g_Orion.ExecuteGump(m_GraphicDisabled);
}
//----------------------------------------------------------------------------------
ushort CGUICheckbox::GetDrawGraphic()
{
	ushort graphic = m_Graphic;

	if (!m_Enabled)
		graphic = m_GraphicDisabled;
	else if (m_Checked)
		graphic = m_GraphicChecked;
	else if (g_SelectedObject.Object() == this)
		graphic = m_GraphicSelected;

	return graphic;
}
//----------------------------------------------------------------------------------
void CGUICheckbox::OnMouseEnter()
{
	if (m_Graphic != m_GraphicSelected && g_SelectedObject.Gump() != NULL)
		g_SelectedObject.Gump()->WantRedraw = true;
}
//----------------------------------------------------------------------------------
void CGUICheckbox::OnMouseExit()
{
	if (m_Graphic != m_GraphicSelected && g_LastSelectedObject.Gump() != NULL)
		g_LastSelectedObject.Gump()->WantRedraw = true;
}
//----------------------------------------------------------------------------------
