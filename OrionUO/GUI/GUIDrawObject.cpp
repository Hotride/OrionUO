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
	int drawMode = (m_Color != 0);

	if (drawMode)
	{
		if (m_PartialHue)
			drawMode = 2;

		g_ColorManager.SendColorsToShader(m_Color);
	}

	glUniform1iARB(g_ShaderDrawMode, drawMode);
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
	bool select = false;

	CGLTexture *th = g_Orion.ExecuteGump(m_Graphic);

	if (th != NULL)
	{
		int x = g_MouseManager.Position.X - m_X;
		int y = g_MouseManager.Position.Y - m_Y;

		if (x >= 0 && y >= 0 && x < th->Width && y < th->Height)
		{
#if UO_ENABLE_TEXTURE_DATA_SAVING == 1
			select = (m_CheckPolygone || th->PixelsData[(y * th->Width) + x] != 0);
#else
			select = (m_CheckPolygone || g_UOFileReader.GumpPixelsInXY(g_Orion.m_GumpDataIndex[m_Graphic], x, y));
#endif
		}
	}

	return select;
}
//----------------------------------------------------------------------------------
