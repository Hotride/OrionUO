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
		{
			m_RenderQueueIndex = 1;
			m_IsWaterTile = true;
		}
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
#if UO_DEBUG_INFO!=0
	g_RenderedObjectsCountInGameWindow++;
#endif

	ushort objGraphic = m_Graphic - 0x4000;
	ushort objColor = m_Color;

	if (g_DeveloperMode == DM_DEBUGGING && g_SelectedObject.Object() == this)
		objColor = SELECT_STATIC_COLOR;

	if (IsFoliage())
	{
		if (m_FoliageTransparentIndex == g_FoliageIndex)
		{
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			glColor4f(1.0f, 1.0f, 1.0f, 0.3f);

			g_Orion.DrawStaticArtAnimated(objGraphic, objColor, x, y, m_Z);

			glDisable(GL_BLEND);
		}
		else
			g_Orion.DrawStaticArtAnimated(objGraphic, objColor, x, y, m_Z);
	}
	else
	{
		if (g_UseCircleTrans)
			g_Orion.DrawStaticArtAnimatedTransparent(objGraphic, objColor, x, y, m_Z);
		else
			g_Orion.DrawStaticArtAnimated(objGraphic, objColor, x, y, m_Z);
	}

	if (IsLightSource() && g_GameScreen.UseLight)
		g_GameScreen.AddLight(this, this, x, y - (m_Z * 4));
}
//----------------------------------------------------------------------------------
void CStaticObject::Select(const int &x, const int &y)
{
	if (IsFoliage())
	{
		if (m_FoliageTransparentIndex != g_FoliageIndex)
		{
			if (g_Orion.StaticPixelsInXYAnimated(m_Graphic - 0x4000, x, y, m_Z))
				g_SelectedObject.Init(this);
		}
	}
	else if (!g_UseCircleTrans && g_Orion.StaticPixelsInXYAnimated(m_Graphic - 0x4000, x, y, m_Z))
		g_SelectedObject.Init(this);
}
//----------------------------------------------------------------------------------
