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
TMultiObject::TMultiObject(WORD graphic, short x, short y, char z, DWORD multiflags, STATIC_TILES &staticTile)
: TRenderWorldObject(ROT_MULTI_OBJECT, 0, graphic, 0, x, y, z), m_MultiFlags(multiflags),
m_StaticTile(staticTile)
{
	WORD id = graphic - 0x4000;

	m_ObjectFlags = m_StaticTile.Flags;
	
	if (IsBackground())
		m_RenderQueueIndex = 3 - (int)IsSurface();
	else if (IsSurface())
		m_RenderQueueIndex = 4;
	else
		m_RenderQueueIndex = 6;
	
	if (m_StaticTile.Height > 5)
		m_CanBeTransparent = 1;
	else if (IsRoof() || (IsSurface() && IsBackground()) || IsWall())
		m_CanBeTransparent = 1;
	else if (m_StaticTile.Height == 5 && IsSurface() && !IsBackground())
		m_CanBeTransparent = 1;
	else
		m_CanBeTransparent = 0;

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
bool TMultiObject::TranparentTest(int &playerZ)
{
	bool result = true;

	if (m_Z < playerZ - (m_StaticTile.Height - 5))
		result = false;
	else if (m_StaticTile.Height == 5 && m_Z > playerZ - 5 && m_Z < playerZ + 5)
		result = false;
	else if (playerZ + 5 < m_Z && !m_CanBeTransparent)
		result = false;

	return result;
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
			objColor = g_SelectMultiColor;

		if (m_MultiFlags == 2) //Мульти на таргете
		{
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_COLOR, GL_ONE_MINUS_SRC_COLOR);

			UO->DrawStaticArt(objGraphic, objColor, drawX, drawY, m_Z);

			glDisable(GL_BLEND);
		}
		else
		{
			if (IsSurface() || (IsBackground() && IsUnknown2()) || IsRoof())
				glEnable(GL_DEPTH_TEST);
			
			if (g_UseCircleTrans)
			{
				glEnable(GL_BLEND);
				glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
				glColor4f(1.0f, 1.0f, 1.0f, 0.25f);
				
				UO->DrawStaticArt(objGraphic, objColor, drawX, drawY, m_Z);
		
				glDisable(GL_BLEND);
		
				glEnable(GL_STENCIL_TEST);
		
				UO->DrawStaticArt(objGraphic, objColor, drawX, drawY, m_Z);

				glDisable(GL_STENCIL_TEST);
			}
			else
				UO->DrawStaticArt(objGraphic, objColor, drawX, drawY, m_Z);
			
			glDisable(GL_DEPTH_TEST);

			if (IsLightSource())
			{
				STATIC_TILES &tile = UO->m_StaticData[objGraphic / 32].Tiles[objGraphic % 32];

				LIGHT_DATA light = { tile.Quality, tile.Hue, X, Y, m_Z, drawX, drawY - (m_Z * 4) };

				if (ConfigManager.ColoredLighting)
					light.Color = UO->GetLightColor(objGraphic);

				GameScreen->AddLight(light);
			}
		}
	}
	else
	{
		if ((!g_UseCircleTrans || (g_UseCircleTrans && !UO->StaticPixelsInCircleTrans(objGraphic, drawX, drawY, m_Z))) && UO->StaticPixelsInXY(objGraphic, drawX, drawY, m_Z))
		{
			g_LastObjectType = SOT_STATIC_OBJECT;
			g_LastSelectedObject = 3;
			g_SelectedObject = this;
		}
	}

	return 0;
}
#if UO_ENABLE_DATA_TEST == 1
//---------------------------------------------------------------------------
TTextureObject *TMultiObject::GetRenderTexture()
{
	if (g_NoDrawRoof && IsRoof())
		return NULL;

	return UO->ExecuteStaticArt(m_Graphic - 0x4000);
}
#endif
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