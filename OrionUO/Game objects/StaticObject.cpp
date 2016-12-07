/***********************************************************************************
**
** StaticObject.cpp
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#include "StaticObject.h"
#include "../OrionUO.h"
#include "../SelectedObject.h"
#include "../Screen stages/GameScreen.h"
//----------------------------------------------------------------------------------
CStaticObject::CStaticObject(const uint &serial, const ushort &graphic, const ushort &color, const short &x, const short &y, const char &z)
: CRenderStaticObject(ROT_STATIC_OBJECT, serial, graphic, color, x, y, z)
{
	m_OriginalGraphic = graphic;
	UpdateGraphicBySeason();

	m_TextControl->MaxSize = 1;

#if UO_DEBUG_INFO!=0
	g_StaticsObjectsCount++;
#endif //UO_DEBUG_INFO!=0
}
//---------------------------------------------------------------------------
void CStaticObject::UpdateGraphicBySeason()
{
	ushort graphic = m_Graphic;

	m_Graphic = g_Orion.GetSeasonGraphic(m_OriginalGraphic) + 0x4000;

	if (m_Graphic != graphic)
	{
		m_Vegetation = g_Orion.IsVegetation(m_Graphic);

		if (IsWet())
			m_RenderQueueIndex = 1;
		else if (IsBackground())
			m_RenderQueueIndex = 3 - (int)IsSurface();
		else if (IsSurface())
			m_RenderQueueIndex = 4;
		else
			m_RenderQueueIndex = 6;
	}
}
//----------------------------------------------------------------------------------
void CStaticObject::Draw(const int &x, const int &y)
{
	m_RenderGraphic = m_Graphic - 0x4000;

	if (g_DeveloperMode == DM_DEBUGGING && g_SelectedObject.Object() == this)
		m_RenderColor = SELECT_STATIC_COLOR;
	else
		m_RenderColor = m_Color;

	CRenderStaticObject::Draw(x, y);
}
//----------------------------------------------------------------------------------
void CStaticObject::Select(const int &x, const int &y)
{
	m_RenderGraphic = m_Graphic - 0x4000;

	CRenderStaticObject::Select(x, y);
}
//----------------------------------------------------------------------------------
