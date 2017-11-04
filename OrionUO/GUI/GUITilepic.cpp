// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
/***********************************************************************************
**
** GUITilepic.cpp
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#include "stdafx.h"
//----------------------------------------------------------------------------------
CGUITilepic::CGUITilepic(const ushort &graphic, const ushort &color, const int &x, const int &y)
: CGUIDrawObject(GOT_TILEPIC, 0, graphic, color, x, y)
{
}
//----------------------------------------------------------------------------------
CGUITilepic::~CGUITilepic()
{
}
//----------------------------------------------------------------------------------
WISP_GEOMETRY::CSize CGUITilepic::GetSize()
{
	WISPFUN_DEBUG("c80_f1");
	WISP_GEOMETRY::CSize size;

	CGLTexture *th = g_Orion.ExecuteStaticArt(m_Graphic);

	if (th != NULL)
	{
		size.Width = th->Width;
		size.Height = th->Height;
	}

	return size;
}
//----------------------------------------------------------------------------------
void CGUITilepic::PrepareTextures()
{
	WISPFUN_DEBUG("c80_f2");
	g_Orion.ExecuteStaticArt(m_Graphic);
}
//----------------------------------------------------------------------------------
void CGUITilepic::Draw(const bool &checktrans)
{
	WISPFUN_DEBUG("c80_f3");
	CGLTexture *th = g_Orion.ExecuteStaticArt(m_Graphic);

	if (th != NULL)
	{
		SetShaderMode();

		th->Draw(m_X, m_Y, checktrans);
	}
}
//----------------------------------------------------------------------------------
bool CGUITilepic::Select()
{
	WISPFUN_DEBUG("c80_f4");
	//if (CGUIDrawObject::Select())
	//	return true;
	CGLTexture *th = g_Orion.m_StaticDataIndex[m_Graphic].Texture;

	if (th != NULL)
		return th->Select(m_X, m_Y, !m_CheckPolygone);

	return false;
}
//----------------------------------------------------------------------------------
