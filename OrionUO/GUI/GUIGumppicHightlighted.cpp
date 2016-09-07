/***********************************************************************************
**
** GUIGumppicHightlighted.cpp
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#include "GUIGumppicHightlighted.h"
#include "../SelectedObject.h"
#include "../Managers/ColorManager.h"
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
	int drawMode = (m_Color != 0);

	if (g_SelectedObject.Object() == this)
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
	if (g_SelectedObject.Gump() != NULL)
		g_SelectedObject.Gump()->WantRedraw = true;
}
//----------------------------------------------------------------------------------
void CGUIGumppicHightlighted::OnMouseExit()
{
	if (g_LastSelectedObject.Gump() != NULL)
		g_LastSelectedObject.Gump()->WantRedraw = true;
}
//----------------------------------------------------------------------------------
