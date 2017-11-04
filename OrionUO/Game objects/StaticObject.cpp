// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
/***********************************************************************************
**
** StaticObject.cpp
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#include "stdafx.h"
//----------------------------------------------------------------------------------
CStaticObject::CStaticObject(const uint &serial, const ushort &graphic, const ushort &color, const short &x, const short &y, const char &z)
: CRenderStaticObject(ROT_STATIC_OBJECT, serial, graphic, color, x, y, z)
{
	WISPFUN_DEBUG("c28_f1");
	m_OriginalGraphic = graphic;
	UpdateGraphicBySeason();

	//if (!color)
	//	m_Color = m_TiledataPtr->Hue;

	m_TextControl->MaxSize = 1;

#if UO_DEBUG_INFO!=0
	g_StaticsObjectsCount++;
#endif //UO_DEBUG_INFO!=0
}
//---------------------------------------------------------------------------
void CStaticObject::UpdateGraphicBySeason()
{
	WISPFUN_DEBUG("c28_f2");
	//ushort graphic = m_Graphic;

	m_Graphic = g_Orion.GetSeasonGraphic(m_OriginalGraphic);

	//if (m_Graphic != graphic)
	{
		m_Vegetation = g_Orion.IsVegetation(m_Graphic);
	}

	m_NoDrawTile = IsNoDrawTile(m_Graphic);
}
//----------------------------------------------------------------------------------
void CStaticObject::Draw(const int &x, const int &y)
{
	WISPFUN_DEBUG("c28_f3");
	m_RenderGraphic = m_Graphic;

	if (g_DeveloperMode == DM_DEBUGGING && g_SelectedObject.Object == this)
		m_RenderColor = SELECT_STATIC_COLOR;
	else
		m_RenderColor = m_Color;

	CRenderStaticObject::Draw(x, y);
}
//----------------------------------------------------------------------------------
void CStaticObject::Select(const int &x, const int &y)
{
	WISPFUN_DEBUG("c28_f4");
	m_RenderGraphic = m_Graphic;

	CRenderStaticObject::Select(x, y);
}
//----------------------------------------------------------------------------------
