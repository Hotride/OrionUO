/***********************************************************************************
**
** LandObject.cpp
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#include "LandObject.h"
#include "../MulStruct.h"
#include "../OrionUO.h"
#include "../Constants.h"
#include "../SelectedObject.h"
//----------------------------------------------------------------------------------
CLandObject::CLandObject(const uint &serial, const ushort &graphic, const ushort &color, const short &x, const short &y, const char &z)
: CMapObject(ROT_LAND_OBJECT, serial, 0, color, x, y, z), m_MinZ(z)
{
	m_OriginalGraphic = graphic;
	UpdateGraphicBySeason();

	LAND_TILES &tile = g_Orion.m_LandData[graphic / 32].Tiles[graphic % 32];

	m_IsStretched = (!tile.TexID && ::IsWet(tile.Flags));

	memset(&m_Rect, 0, sizeof(RECT));
	memset(&m_Normals[0], 0, sizeof(m_Normals));

#if UO_DEBUG_INFO!=0
	g_LandObjectsCount++;
#endif //UO_DEBUG_INFO!=0
}
//---------------------------------------------------------------------------
void CLandObject::UpdateGraphicBySeason()
{
	m_Graphic = g_Orion.GetLandSeasonGraphic(m_OriginalGraphic);
}
//---------------------------------------------------------------------------
void CLandObject::UpdateZ(const char &zTop, const char &zRight, const char &zBottom)
{
	if (m_IsStretched)
	{
		//Сохраним среднее значение Z-координаты
		m_Serial = ((m_Z + zTop + zRight + zBottom) / 4);

		//Значения для рендера
		m_Rect.left = m_Z * 4;
		m_Rect.top = zTop * 4;
		m_Rect.right = zRight * 4;
		m_Rect.bottom = zBottom * 4;

		//Минимальная Z-координата из всех
		m_MinZ = m_Z;

		if (zTop < m_MinZ)
			m_MinZ = zTop;

		if (zRight < m_MinZ)
			m_MinZ = zRight;

		if (zBottom < m_MinZ)
			m_MinZ = zBottom;
	}
}
//---------------------------------------------------------------------------
void CLandObject::Draw(const int &x, const int &y)
{
	if (m_Z <= g_MaxGroundZ)
	{
		ushort objColor = 0;

		if (g_DeveloperMode == DM_DEBUGGING && g_SelectedObject.Object() == this)
			objColor = SELECT_LAND_COLOR;

#if UO_DEBUG_INFO!=0
		g_RenderedObjectsCountInGameWindow++;
#endif

		if (!m_IsStretched)
			g_Orion.DrawLandArt(m_Graphic, objColor, x, y, m_Z);
		else
		{
			char minZ = m_MinZ + 3;

			if (minZ > m_Z)
				minZ = m_Z;

			g_Orion.DrawLandTexture(this, objColor, x, y);
		}
	}
}
//---------------------------------------------------------------------------
void CLandObject::Select(const int &x, const int &y)
{
	if (m_Z <= g_MaxGroundZ)
	{
		if (!m_IsStretched)
		{
			if (g_Orion.LandPixelsInXY(m_Graphic, x, y, m_Z))
				g_SelectedObject.Init(this);
		}
		else
		{
			if (g_Orion.LandTexturePixelsInXY(x, y, m_Rect))
				g_SelectedObject.Init(this);
		}
	}
}
//----------------------------------------------------------------------------------
