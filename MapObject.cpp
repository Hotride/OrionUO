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
	double newX = m_Y * v.Z + m_Z * v.Y;
	double newY = m_Z * v.X + m_X * v.Z;
	double newZ = m_X * v.Y + m_Y * v.X;

	m_X = newX;
	m_Y = newY;
	m_Z = newZ;
}
//---------------------------------------------------------------------------
void TVector::Normalize()
{
	double result = m_X * m_X + m_Y * m_Y + m_Z * m_Z;

	if (result)
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
	if (RenderType == ROT_LAND_OBJECT)
		g_LandObjectsCount--;
	else if (RenderType == ROT_STATIC_OBJECT)
		g_StaticsObjectsCount--;
#endif //UO_DEBUG_INFO!=0
}
//---------------------------------------------------------------------------
TLandObject::TLandObject(DWORD serial, WORD graphic, WORD color, short x, short y, char z)
: TMapObject(ROT_LAND_OBJECT, serial, graphic, color, x, y, z), m_DrawZ(z), m_MinZ(z)
{
	memset(&m_Rect, 0, sizeof(RECT));
	memset(&m_Normals[0], 0, sizeof(m_Normals));

#if UO_DEBUG_INFO!=0
	g_LandObjectsCount++;
#endif //UO_DEBUG_INFO!=0
}
//---------------------------------------------------------------------------
TStaticObject::TStaticObject(DWORD serial, WORD graphic, WORD color, short x, short y, char z)
: TMapObject(ROT_STATIC_OBJECT, serial, graphic, color, x, y, z)
{
	m_ObjectFlags = UO->GetStaticFlags(graphic - 0x4000);

	if (IsBackground())
		RenderQueueIndex = 3 - (int)IsSurface();
	else if (IsSurface())
		RenderQueueIndex = 4;
	else
		RenderQueueIndex = 6;

#if UO_DEBUG_INFO!=0
	g_StaticsObjectsCount++;
#endif //UO_DEBUG_INFO!=0
}
//---------------------------------------------------------------------------