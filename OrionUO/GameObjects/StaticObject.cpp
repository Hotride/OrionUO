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
	OriginalGraphic = graphic;
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
	//ushort graphic = Graphic;

	Graphic = g_Orion.GetSeasonGraphic(OriginalGraphic);

	//if (Graphic != graphic)
	{
		Vegetation = g_Orion.IsVegetation(Graphic);
	}

	NoDrawTile = IsNoDrawTile(Graphic);
}
//----------------------------------------------------------------------------------
void CStaticObject::Draw(const int &x, const int &y)
{
	WISPFUN_DEBUG("c28_f3");
	RenderGraphic = Graphic;

	if (g_DeveloperMode == DM_DEBUGGING && g_SelectedObject.Object == this)
		RenderColor = SELECT_STATIC_COLOR;
	else
		RenderColor = Color;

	CRenderStaticObject::Draw(x, y);
}
//----------------------------------------------------------------------------------
void CStaticObject::Select(const int &x, const int &y)
{
	WISPFUN_DEBUG("c28_f4");
	RenderGraphic = Graphic;

	CRenderStaticObject::Select(x, y);
}
//----------------------------------------------------------------------------------
