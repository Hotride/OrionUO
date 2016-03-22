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

TRenderWorldObject *g_SelectedObject = NULL;
//---------------------------------------------------------------------------
//---------------------------------TRenderObject-----------------------------
//---------------------------------------------------------------------------
TRenderObject::TRenderObject(DWORD serial, WORD graphic, WORD color, short x, short y)
: TBaseQueueItem(), m_Graphic(graphic), m_Color(color), m_X(x), m_Y(y),
m_Serial(serial)
{
}
//---------------------------------------------------------------------------
TRenderObject::~TRenderObject()
{
}
//---------------------------------------------------------------------------
//-------------------------------TRenderWorldObject--------------------------
//---------------------------------------------------------------------------
TRenderWorldObject::TRenderWorldObject(RENDER_OBJECT_TYPE renderType, DWORD serial, WORD graphic, WORD color, short x, short y, char z)
: TRenderObject(serial, graphic, color, x, y), m_RenderType(renderType),
m_Z(z), m_NextXY(NULL), m_PrevXY(NULL), m_RenderQueueIndex(0)
{
}
//---------------------------------------------------------------------------
TRenderWorldObject::~TRenderWorldObject()
{
	RemoveRender();
}
//---------------------------------------------------------------------------
void TRenderWorldObject::RemoveRender()
{
	if (m_PrevXY != NULL)
		m_PrevXY->m_NextXY = m_NextXY;

	if (m_NextXY != NULL)
		m_NextXY->m_PrevXY = m_PrevXY;

	m_NextXY = NULL;
	m_PrevXY = NULL;
}
//---------------------------------------------------------------------------
TLandObject *TRenderWorldObject::GetLand()
{
	TRenderWorldObject *land = this;

	while (land != NULL && !land->IsLandObject())
		land = land->m_NextXY;

	return (TLandObject*)land;
}
//---------------------------------------------------------------------------