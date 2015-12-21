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
: TRenderWorldObject(ROT_MULTI_OBJECT, 0, graphic, 0, x, y, z), m_MultiFlags(multiflags)
{
	m_ObjectFlags = UO->GetStaticFlags(graphic - 0x4000);

	if (IsBackground())
		RenderQueueIndex = 3 - (int)IsSurface();
	else if (IsSurface())
		RenderQueueIndex = 4;
	else
		RenderQueueIndex = 6;

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