/***********************************************************************************
**
** LandObject.cpp
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#include "stdafx.h"
//----------------------------------------------------------------------------------
CLandObject::CLandObject(const uint &serial, const ushort &graphic, const ushort &color, const short &x, const short &y, const char &z)
: CMapObject(ROT_LAND_OBJECT, serial, 0, color, x, y, z), m_MinZ(z), m_AverageZ(z)
{
	WISPFUN_DEBUG("c23_f1");
	m_OriginalGraphic = graphic;
	UpdateGraphicBySeason();

	LAND_TILES &tile = g_Orion.m_LandData[graphic];

	m_IsStretched = (!tile.TexID && ::IsWet(tile.Flags));

	memset(&m_Rect, 0, sizeof(RECT));
	memset(&m_Normals[0], 0, sizeof(m_Normals));

#if UO_DEBUG_INFO!=0
	g_LandObjectsCount++;
#endif //UO_DEBUG_INFO!=0
}
//---------------------------------------------------------------------------
CLandObject::~CLandObject()
{
	WISPFUN_DEBUG("c23_f2");
	if (m_PositionBuffer != 0)
	{
		glDeleteBuffers(1, &m_PositionBuffer);
		m_PositionBuffer = 0;
	}

	if (m_VertexBuffer != 0)
	{
		glDeleteBuffers(1, &m_VertexBuffer);
		m_VertexBuffer = 0;
	}

	if (m_NormalBuffer != 0)
	{
		glDeleteBuffers(1, &m_NormalBuffer);
		m_NormalBuffer = 0;
	}
}
//---------------------------------------------------------------------------
void CLandObject::UpdateGraphicBySeason()
{
	WISPFUN_DEBUG("c23_f3");
	m_Graphic = g_Orion.GetLandSeasonGraphic(m_OriginalGraphic);
	m_NoDrawTile = (m_Graphic == 2);
}
//---------------------------------------------------------------------------
int CLandObject::GetDirectionZ(const int &direction)
{
	WISPFUN_DEBUG("c23_f4");
	switch (direction)
	{
		case 1:
			return (m_Rect.bottom / 4);
		case 2:
			return (m_Rect.right / 4);
		case 3:
			return (m_Rect.top / 4);
		default:
			break;
	}

	return m_Z;
}
//---------------------------------------------------------------------------
int CLandObject::CalculateCurrentAverageZ(const int &direction)
{
	WISPFUN_DEBUG("c23_f5");
	int result = GetDirectionZ(((uchar)(direction >> 1) + 1) & 3);

	if (direction & 1)
		return result;

	return (result + GetDirectionZ(direction >> 1)) >> 1;
}
//---------------------------------------------------------------------------
void CLandObject::UpdateZ(const int &zTop, const int &zRight, const int &zBottom)
{
	WISPFUN_DEBUG("c23_f6");
	if (m_IsStretched)
	{
		//Сохраним среднее значение Z-координаты
		m_Serial = ((m_Z + zTop + zRight + zBottom) / 4);

		//Значения для рендера
		m_Rect.left = m_Z * 4 + 1;
		m_Rect.top = zTop * 4;
		m_Rect.right = zRight * 4;
		m_Rect.bottom = zBottom * 4 + 1;

		if (abs(m_Z - zRight) <= abs(zBottom - zTop))
			AverageZ = (m_Z + zRight) >> 1;
		else
			AverageZ = (zBottom + zTop) >> 1;

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
	WISPFUN_DEBUG("c23_f7");
	if (m_Z <= g_MaxGroundZ)
	{
		ushort objColor = 0;

		if (g_DeveloperMode == DM_DEBUGGING && g_SelectedObject.Object == this)
			objColor = SELECT_LAND_COLOR;

#if UO_DEBUG_INFO!=0
		g_RenderedObjectsCountInGameWindow++;
#endif

		if (!m_IsStretched)
			g_Orion.DrawLandArt(m_Graphic, objColor, x, y);
		else
			g_Orion.DrawLandTexture(this, objColor, x, y);
	}
}
//---------------------------------------------------------------------------
void CLandObject::Select(const int &x, const int &y)
{
	WISPFUN_DEBUG("c23_f8");
	if (m_Z <= g_MaxGroundZ)
	{
		if (!m_IsStretched)
		{
			if (g_Orion.LandPixelsInXY(m_Graphic, x, y))
				g_SelectedObject.Init(this);
		}
		else
		{
			if (g_Orion.LandTexturePixelsInXY(x, y + (m_Z * 4), m_Rect))
				g_SelectedObject.Init(this);
		}
	}
}
//----------------------------------------------------------------------------------
