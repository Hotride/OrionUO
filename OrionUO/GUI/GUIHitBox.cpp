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
CGUIHitBox::CGUIHitBox(int serial, int x, int y, int width, int height, bool callOnMouseUp)
: CGUIPolygonal(GOT_HITBOX, x, y, width, height, callOnMouseUp)
{
	Serial = serial;
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
