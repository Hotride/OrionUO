/****************************************************************************
**
** RenderObject.cpp
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
//---------------------------------------------------------------------------
#include "stdafx.h"
//---------------------------------------------------------------------------
TVector::TVector(double x, double y, double z)
: m_X(x), m_Y(y), m_Z(z)
{
}
//---------------------------------------------------------------------------
void TVector::Link(TVector &v)
{
	m_X = v.X;
	m_Y = v.Y;
	m_Z = v.Z;
}
//---------------------------------------------------------------------------
void TVector::Link(double x, double y, double z)
{
	m_X = x;
	m_Y = y;
	m_Z = z;
}
//---------------------------------------------------------------------------
void TVector::Add(TVector &v)
{
	m_X += v.X;
	m_Y += v.Y;
	m_Z += v.Z;
}
//---------------------------------------------------------------------------
void TVector::Add(double x, double y, double z)
{
	m_X += x;
	m_Y += y;
	m_Z += z;
}
//---------------------------------------------------------------------------
void TVector::Merge(TVector &v)
{
	double newX = m_Y * v.Z - m_Z * v.Y;
	double newY = m_Z * v.X - m_X * v.Z;
	double newZ = m_X * v.Y - m_Y * v.X;

	m_X = newX;
	m_Y = newY;
	m_Z = newZ;
}
//---------------------------------------------------------------------------
void TVector::Merge(double x, double y, double z)
{
	double newX = m_Y * z - m_Z * y;
	double newY = m_Z * x - m_X * z;
	double newZ = m_X * y - m_Y * x;

	m_X = newX;
	m_Y = newY;
	m_Z = newZ;
}
//---------------------------------------------------------------------------
void TVector::Normalize()
{
	if (!m_X && !m_Y && !m_Z)
			return;

	double result = m_X * m_X + m_Y * m_Y + m_Z * m_Z;

	//if (result)
	{
		result = sqrtl(result);
		
		m_X /= result;
		m_Y /= result;
		m_Z /= result;
	}
}
//---------------------------------------------------------------------------
TMapObject::TMapObject(RENDER_OBJECT_TYPE type, DWORD serial, WORD graphic, WORD color, short x, short y, char z)
: TRenderWorldObject(type, serial, graphic, color, x, y, z)
{
}
//---------------------------------------------------------------------------
TMapObject::~TMapObject()
{
#if UO_DEBUG_INFO!=0
	if (m_RenderType == ROT_LAND_OBJECT)
		g_LandObjectsCount--;
	else if (m_RenderType == ROT_STATIC_OBJECT)
		g_StaticsObjectsCount--;
#endif //UO_DEBUG_INFO!=0
}
//---------------------------------------------------------------------------
TLandObject::TLandObject(DWORD serial, WORD graphic, WORD color, short x, short y, char z)
: TMapObject(ROT_LAND_OBJECT, serial, graphic, color, x, y, z), m_MinZ(z),
m_IsStretched(false)
{
	//m_ObjectFlags = UO->GetLandFlags(graphic);

	memset(&m_Rect, 0, sizeof(RECT));
	memset(&m_Normals[0], 0, sizeof(m_Normals));

#if UO_DEBUG_INFO!=0
	g_LandObjectsCount++;
#endif //UO_DEBUG_INFO!=0
}
//---------------------------------------------------------------------------
int TLandObject::Draw(bool &mode, int &drawX, int &drawY, DWORD &ticks)
{
	if (m_Z <= g_MaxGroundZ)
	{
		if (mode)
		{
			WORD objColor = 0;

			if (this == g_SelectedObject)
				objColor = g_SelectLandColor;

#if UO_DEBUG_INFO!=0
g_RenderedObjectsCountInGameWindow++;
#endif
			glEnable(GL_DEPTH_TEST);

			if (!m_IsStretched)
				UO->DrawLandArt(m_Graphic, objColor, drawX, drawY, m_Z);
			else
			{
				char minZ = m_MinZ + 3;

				if (minZ > m_Z)
					minZ = m_Z;

				g_ZBuffer = minZ;

				UO->DrawLandTexture(m_Graphic, objColor, drawX, drawY, m_Rect, m_Normals);
			}

			glDisable(GL_DEPTH_TEST);
		}
		else
		{
			if (!m_IsStretched)
			{
				if (UO->LandPixelsInXY(m_Graphic, drawX, drawY, m_Z))
				{
					g_LastObjectType = SOT_LAND_OBJECT;
					g_LastSelectedObject = 2;
					g_SelectedObject = this;
				}
			}
			else
			{
				if (UO->LandTexturePixelsInXY(drawX, drawY, m_Rect))
				{
					g_LastObjectType = SOT_LAND_OBJECT;
					g_LastSelectedObject = 2;
					g_SelectedObject = this;
				}
			}
		}
	}

	return 0;
}
//---------------------------------------------------------------------------
TStaticObject::TStaticObject(DWORD serial, WORD graphic, WORD color, short x, short y, char z)
: TMapObject(ROT_STATIC_OBJECT, serial, graphic, color, x, y, z)
{
	m_ObjectFlags = UO->GetStaticFlags(graphic - 0x4000);
	
	if (IsWet())
		m_RenderQueueIndex = 0;
	else if (IsBackground())
		m_RenderQueueIndex = 3 - (int)IsSurface();
	else if (IsSurface())
		m_RenderQueueIndex = 4;
	else
		m_RenderQueueIndex = 6;

#if UO_DEBUG_INFO!=0
	g_StaticsObjectsCount++;
#endif //UO_DEBUG_INFO!=0
}
//---------------------------------------------------------------------------
int TStaticObject::Draw(bool &mode, int &drawX, int &drawY, DWORD &ticks)
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
			objColor = g_SelectStaticColor;

		if (IsFoliage()) // && playerX - 2 < x && playerY - 2 < y)
		{
			if (!g_GrayedPixels)
			{
				POINT fp = { 0, 0 };
				UO->GetArtDimension(m_Graphic, fp);

				IMAGE_BOUNDS fib = { drawX - fp.x / 2, drawY - fp.y - (m_Z * 4), fp.x, fp.y };

				//g_GL.DrawPolygone(0x7F7F7F7F, fib.X, fib.Y, fib.Width, fib.Height);

				if (fib.InRect(g_PlayerRect))
				{
					glEnable(GL_BLEND);
					//glBlendFunc(GL_SRC_COLOR, GL_ONE_MINUS_SRC_COLOR);
					glBlendFunc(GL_SRC_COLOR, GL_DST_COLOR);

					UO->DrawStaticArtAnimated(objGraphic, objColor, drawX, drawY, m_Z);

					glDisable(GL_BLEND);
				}
				else
					UO->DrawStaticArtAnimated(objGraphic, objColor, drawX, drawY, m_Z);
			}
		}
		else
		{
			if (IsSurface() || (IsBackground() && IsUnknown2()) || IsRoof())
				glEnable(GL_DEPTH_TEST);

			UO->DrawStaticArtAnimated(objGraphic, objColor, drawX, drawY, m_Z);

			glDisable(GL_DEPTH_TEST);
		}

		if (IsLightSource() && !IsWall())
		{
			STATIC_TILES &tile = UO->m_StaticData[objGraphic / 32].Tiles[objGraphic % 32];

			LIGHT_DATA light = { tile.Quality, 0, X, Y, m_Z, drawX, drawY - (m_Z * 4) };

			if (ConfigManager.ColoredLighting)
				light.Color = UO->GetLightColor(objGraphic);

			GameScreen->AddLight(light);
		}
	}
	else
	{
		if (IsFoliage()) // && playerX - 2 < mX && playerY - 2 < mY)
		{
			if (!g_GrayedPixels)
			{
				POINT fp = { 0, 0 };
				UO->GetArtDimension(m_Graphic, fp);

				IMAGE_BOUNDS fib = { drawX - fp.x / 2, drawY - fp.y - (m_Z * 4), fp.x, fp.y };

				if (!fib.InRect(g_PlayerRect))
				{
					if (UO->StaticPixelsInXYAnimated(objGraphic, drawX, drawY, m_Z))
					{
						g_LastObjectType = SOT_STATIC_OBJECT;
						g_LastSelectedObject = 3;
						g_SelectedObject = this;
					}
				}
			}
		}
		else if (UO->StaticPixelsInXYAnimated(objGraphic, drawX, drawY, m_Z))
		{
			g_LastObjectType = SOT_STATIC_OBJECT;
			g_LastSelectedObject = 3;
			g_SelectedObject = this;
		}
	}

	return 0;
}
//---------------------------------------------------------------------------