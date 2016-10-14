/***********************************************************************************
**
** GUIHitBox.cpp
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#include "GUIHitBox.h"
#include "../SelectedObject.h"
//----------------------------------------------------------------------------------
CGUIHitBox::CGUIHitBox(const uint &serial, const int &x, const int &y, const int &width, const int &height, const bool &callOnMouseUp)
: CGUIPolygonal(GOT_HITBOX, x, y, width, height, callOnMouseUp), m_ToPage(-1)
{
	m_Serial = serial;
}
//----------------------------------------------------------------------------------
CGUIHitBox::~CGUIHitBox()
{
}
//----------------------------------------------------------------------------------
void CGUIHitBox::OnMouseEnter()
{
	if (g_SelectedObject.Gump() != NULL)
		g_SelectedObject.Gump()->WantRedraw = true;
}
//----------------------------------------------------------------------------------
void CGUIHitBox::OnMouseExit()
{
	if (g_LastSelectedObject.Gump() != NULL)
		g_LastSelectedObject.Gump()->WantRedraw = true;
}
//----------------------------------------------------------------------------------
