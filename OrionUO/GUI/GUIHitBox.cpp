// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
/***********************************************************************************
**
** GUIHitBox.cpp
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#include "stdafx.h"
//----------------------------------------------------------------------------------
CGUIHitBox::CGUIHitBox(const uint &serial, const int &x, const int &y, const int &width, const int &height, const bool &callOnMouseUp)
: CGUIPolygonal(GOT_HITBOX, x, y, width, height, callOnMouseUp)
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
	WISPFUN_DEBUG("c58_f1");
	if (g_SelectedObject.Gump != NULL)
		g_SelectedObject.Gump->WantRedraw = true;
}
//----------------------------------------------------------------------------------
void CGUIHitBox::OnMouseExit()
{
	WISPFUN_DEBUG("c58_f2");
	if (g_LastSelectedObject.Gump != NULL)
		g_LastSelectedObject.Gump->WantRedraw = true;
}
//----------------------------------------------------------------------------------
