// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
/***********************************************************************************
**
** GUIGumppicHightlighted.cpp
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#include "stdafx.h"
//----------------------------------------------------------------------------------
CGUIGumppicHightlighted::CGUIGumppicHightlighted(const uint &serial, const ushort &graphic, const ushort &color, const ushort &selectedColor, const int &x, const int &y)
: CGUIDrawObject(GOT_GUMPPICHIGHTLIGHTED, serial, graphic, color, x, y),
m_SelectedColor(selectedColor)
{
}
//----------------------------------------------------------------------------------
CGUIGumppicHightlighted::~CGUIGumppicHightlighted()
{
}
//----------------------------------------------------------------------------------
void CGUIGumppicHightlighted::SetShaderMode()
{
	WISPFUN_DEBUG("c56_f1");
	int drawMode = (m_Color != 0);

	if (g_SelectedObject.Object == this)
	{
		drawMode = 1;

		g_ColorManager.SendColorsToShader(m_SelectedColor);
	}
	else if (drawMode)
	{
		if (m_PartialHue)
			drawMode = 2;

		g_ColorManager.SendColorsToShader(m_Color);
	}

	glUniform1iARB(g_ShaderDrawMode, drawMode);
}
//----------------------------------------------------------------------------------
void CGUIGumppicHightlighted::OnMouseEnter()
{
	WISPFUN_DEBUG("c56_f2");
	if (g_SelectedObject.Gump != NULL)
		g_SelectedObject.Gump->WantRedraw = true;
}
//----------------------------------------------------------------------------------
void CGUIGumppicHightlighted::OnMouseExit()
{
	WISPFUN_DEBUG("c56_f3");
	if (g_LastSelectedObject.Gump != NULL)
		g_LastSelectedObject.Gump->WantRedraw = true;
}
//----------------------------------------------------------------------------------
