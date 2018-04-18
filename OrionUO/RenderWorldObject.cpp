﻿// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
/***********************************************************************************
**
** RenderWorldObject.cpp
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#include "stdafx.h"
//----------------------------------------------------------------------------------
//---------------------------------RenderWorldObject--------------------------------
//----------------------------------------------------------------------------------
CRenderWorldObject::CRenderWorldObject(RENDER_OBJECT_TYPE renderType, int serial, ushort graphic, ushort color, int x, int y, char z)
: CRenderObject(serial, graphic, color, x, y), RenderType(renderType), m_Z(z)
{
	WISPFUN_DEBUG("c202_f1");
	m_DrawTextureColor[0] = 0xFF;
	m_DrawTextureColor[1] = 0xFF;
	m_DrawTextureColor[2] = 0xFF;
	m_DrawTextureColor[3] = 0x00;

	UpdateRealDrawCoordinates();
}
//----------------------------------------------------------------------------------
CRenderWorldObject::~CRenderWorldObject()
{
	WISPFUN_DEBUG("c202_f2");
	RemoveRender();
}
//----------------------------------------------------------------------------------
bool CRenderWorldObject::ProcessAlpha(int maxAlpha)
{
	WISPFUN_DEBUG("c202_f3");
	if (!g_ConfigManager.RemoveOrCreateObjectsWithBlending)
	{
		m_DrawTextureColor[3] = (uchar)maxAlpha;

		return (maxAlpha != 0);
	}

	bool result = false;
	int alpha = (int)m_DrawTextureColor[3];

	if (alpha > maxAlpha)
	{
		alpha -= ALPHA_STEP;

		if (alpha < maxAlpha)
			alpha = maxAlpha;

		result = true;
	}
	else if (alpha < maxAlpha)
	{
		alpha += ALPHA_STEP;

		if (alpha > maxAlpha)
			alpha = maxAlpha;

		result = true;
	}

	m_DrawTextureColor[3] = (uchar)alpha;

	return result;
}
//----------------------------------------------------------------------------------
void CRenderWorldObject::RemoveRender()
{
	WISPFUN_DEBUG("c202_f4");
	if (m_PrevXY != NULL)
		m_PrevXY->m_NextXY = m_NextXY;

	if (m_NextXY != NULL)
		m_NextXY->m_PrevXY = m_PrevXY;

	m_NextXY = NULL;
	m_PrevXY = NULL;
}
//----------------------------------------------------------------------------------
bool CRenderWorldObject::RemovedFromRender()
{
	return m_NextXY == NULL && m_PrevXY == NULL;
}
//----------------------------------------------------------------------------------
CLandObject *CRenderWorldObject::GetLand()
{
	WISPFUN_DEBUG("c202_f5");

	for (CRenderWorldObject *land = this; land != NULL; land = land->m_NextXY)
	{
		if (land->IsLandObject())
			return (CLandObject*)land;
	}

	for (CRenderWorldObject *land = this->m_PrevXY; land != NULL; land = land->m_PrevXY)
	{
		if (land->IsLandObject())
			return (CLandObject*)land;
	}

	return NULL;
}
//----------------------------------------------------------------------------------
void CRenderWorldObject::SetZ(char val)
{
	m_Z = val;
	UpdateRealDrawCoordinates();
}
//----------------------------------------------------------------------------------
void CRenderWorldObject::UpdateRealDrawCoordinates()
{
	RealDrawX = (m_X - m_Y) * 22;
	RealDrawY = (m_X + m_Y) * 22 - (m_Z * 4);
	Changed = true;
}
//----------------------------------------------------------------------------------
void CRenderWorldObject::UpdateDrawCoordinates()
{
	DrawX = RealDrawX - g_RenderBounds.WindowDrawOffsetX;
	DrawY = RealDrawY - g_RenderBounds.WindowDrawOffsetY;
	Changed = false;
	UpdateTextCoordinates();
	FixTextCoordinates();
}
//----------------------------------------------------------------------------------
