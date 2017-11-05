// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
/***********************************************************************************
**
** GUITilepicHightlighted.cpp
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#include "stdafx.h"
//----------------------------------------------------------------------------------
CGUITilepicHightlighted::CGUITilepicHightlighted(const uint &serial, const ushort &graphic, const ushort &color, const ushort &selectedColor, const int &x, const int &y, const bool &doubleDraw)
: CGUITilepic(graphic, color, x, y), m_SelectedColor(selectedColor),
m_DoubleDraw(doubleDraw)
{
	m_Type = GOT_TILEPICHIGHTLIGHTED;
	m_Serial = serial;
}
//----------------------------------------------------------------------------------
CGUITilepicHightlighted::~CGUITilepicHightlighted()
{
}
//----------------------------------------------------------------------------------
void CGUITilepicHightlighted::SetShaderMode()
{
	WISPFUN_DEBUG("c81_f1");

	if (g_SelectedObject.Object == this)
	{
		glUniform1iARB(g_ShaderDrawMode, SDM_COLORED);

		g_ColorManager.SendColorsToShader(m_SelectedColor);
	}
	else if (m_Color != 0)
	{
		if (m_PartialHue)
			glUniform1iARB(g_ShaderDrawMode, SDM_PARTIAL_HUE);
		else
			glUniform1iARB(g_ShaderDrawMode, SDM_COLORED);

		g_ColorManager.SendColorsToShader(m_Color);
	}
	else
		glUniform1iARB(g_ShaderDrawMode, SDM_NO_COLOR);
}
//----------------------------------------------------------------------------------
void CGUITilepicHightlighted::Draw(const bool &checktrans)
{
	WISPFUN_DEBUG("c81_f2");
	CGLTexture *th = g_Orion.ExecuteStaticArt(m_Graphic);

	if (th != NULL)
	{
		SetShaderMode();

		th->Draw(m_X, m_Y, checktrans);

		if (m_DoubleDraw)
			th->Draw(m_X + 5, m_Y + 5, checktrans);
	}
}
//----------------------------------------------------------------------------------
bool CGUITilepicHightlighted::Select()
{
	WISPFUN_DEBUG("c81_f3");
	CGLTexture *th = g_Orion.m_StaticDataIndex[m_Graphic].Texture;

	if (th != NULL)
	{
		int count = 1 + m_DoubleDraw;
		int offset = 0;

		IFOR(i, 0, count)
		{
			if (th->Select(m_X + offset, m_Y + offset, !m_CheckPolygone))
				return true;

			offset = 5;
		}
	}

	return false;
}
//----------------------------------------------------------------------------------
void CGUITilepicHightlighted::OnMouseEnter()
{
	WISPFUN_DEBUG("c81_f4");
	if (g_SelectedObject.Gump != NULL)
		g_SelectedObject.Gump->WantRedraw = true;
}
//----------------------------------------------------------------------------------
void CGUITilepicHightlighted::OnMouseExit()
{
	WISPFUN_DEBUG("c81_f5");
	if (g_LastSelectedObject.Gump != NULL)
		g_LastSelectedObject.Gump->WantRedraw = true;
}
//----------------------------------------------------------------------------------
