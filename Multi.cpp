/****************************************************************************
**
** Multi.cpp
**
** Copyright (C) September 2015 Hotride
**
** This program is free software; you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation; either version 2 of the License, or
** (at your option) any later version.
**
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**
*****************************************************************************
*/
//----------------------------------------------------------------------------
#include "stdafx.h"
//----------------------------------------------------------------------------
//--------------------------------TMultiObject--------------------------------
//----------------------------------------------------------------------------
TMultiObject::TMultiObject(WORD graphic, short x, short y, char z, DWORD multiflags)
: TRenderStaticObject(ROT_MULTI_OBJECT, 0, graphic, 0, x, y, z), m_MultiFlags(multiflags)
{
	m_Graphic += 0x4000;

	if (IsWet())
		m_RenderQueueIndex = 1;
	else if (IsBackground())
		m_RenderQueueIndex = 3 - (int)IsSurface();
	else if (IsSurface())
		m_RenderQueueIndex = 4;
	else
		m_RenderQueueIndex = 6;

#if UO_DEBUG_INFO!=0
	g_MultiObjectsCount++;
#endif //UO_DEBUG_INFO!=0
}
//----------------------------------------------------------------------------
TMultiObject::~TMultiObject()
{
#if UO_DEBUG_INFO!=0
	g_MultiObjectsCount--;
#endif //UO_DEBUG_INFO!=0
}
//---------------------------------------------------------------------------
int TMultiObject::Draw(bool &mode, int &drawX, int &drawY, DWORD &ticks)
{
	if (g_NoDrawRoof && IsRoof())
		return 0;

	WORD objGraphic = m_Graphic - 0x4000;

	if (mode)
	{
#if UO_DEBUG_INFO!=0
		g_RenderedObjectsCountInGameWindow++;
#endif

		WORD objColor = m_Color;

		if (this == g_SelectedObject)
			objColor = SELECT_MULTI_COLOR;

		if (m_MultiFlags == 2) //Мульти на таргете
		{
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_COLOR, GL_ONE_MINUS_SRC_COLOR);

			UO->DrawStaticArt(objGraphic, objColor, drawX, drawY, m_Z);

			glDisable(GL_BLEND);
		}
		else
		{
#if UO_DEPTH_TEST == 1
			if (m_CanBeTransparent & 0x10)
				glEnable(GL_DEPTH_TEST);
#endif
			
			if (g_UseCircleTrans)
				UO->DrawStaticArtTransparent(objGraphic, objColor, drawX, drawY, m_Z);
			else
				UO->DrawStaticArt(objGraphic, objColor, drawX, drawY, m_Z);

#if UO_DEPTH_TEST == 1
			glDisable(GL_DEPTH_TEST);
#endif

			if (IsLightSource() && GameScreen->UseLight)
				GameScreen->AddLight(this, this, drawX, drawY - (m_Z * 4));
		}
	}
	else
	{
		if (UO->StaticPixelsInXY(objGraphic, drawX, drawY, m_Z) && !g_UseCircleTrans)
		{
			g_LastObjectType = SOT_STATIC_OBJECT;
			g_LastSelectedObject = 3;
			g_SelectedObject = this;
		}
	}

	return 0;
}
//----------------------------------------------------------------------------
//-----------------------------------TMulti-----------------------------------
//----------------------------------------------------------------------------
TMulti::TMulti(short x, short y)
: TBaseQueueItem(), m_X(x), m_Y(y), m_MinX(0), m_MinY(0), m_MaxX(0), m_MaxY(0)
{
}
//----------------------------------------------------------------------------
TMulti::~TMulti()
{
}
//----------------------------------------------------------------------------