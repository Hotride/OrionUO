// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
/***********************************************************************************
**
** GUIDrawObject.cpp
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#include "stdafx.h"
//----------------------------------------------------------------------------------
CGUIDrawObject::CGUIDrawObject(const GUMP_OBJECT_TYPE &type, const uint &serial, const ushort &graphic, const ushort &color, const int &x, const int &y)
: CBaseGUI(type, serial, graphic, color, x, y)
{
}
//----------------------------------------------------------------------------------
CGUIDrawObject::~CGUIDrawObject()
{
}
//----------------------------------------------------------------------------------
WISP_GEOMETRY::CSize CGUIDrawObject::GetSize()
{
	WISPFUN_DEBUG("c52_f1");
	WISP_GEOMETRY::CSize size;
	CGLTexture *th = g_Orion.ExecuteGump(GetDrawGraphic());

	if (th != NULL)
	{
		size.Width = th->Width;
		size.Height = th->Height;
	}

	return size;
}
//----------------------------------------------------------------------------------
void CGUIDrawObject::SetShaderMode()
{
	WISPFUN_DEBUG("c52_f2");

	if (m_Color != 0)
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
void CGUIDrawObject::PrepareTextures()
{
	WISPFUN_DEBUG("c52_f3");
	g_Orion.ExecuteGump(m_Graphic);
}
//----------------------------------------------------------------------------------
void CGUIDrawObject::Draw(const bool &checktrans)
{
	WISPFUN_DEBUG("c52_f4");
	CGLTexture *th = g_Orion.ExecuteGump(GetDrawGraphic());

	if (th != NULL)
	{
		SetShaderMode();

		th->Draw(m_X, m_Y, checktrans);
	}
}
//----------------------------------------------------------------------------------
bool CGUIDrawObject::Select()
{
	WISPFUN_DEBUG("c52_f5");
	CGLTexture *th = g_Orion.ExecuteGump(m_Graphic);

	if (th != NULL)
		return th->Select(m_X, m_Y, !m_CheckPolygone);

	return false;
}
//----------------------------------------------------------------------------------
