// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
/***********************************************************************************
**
** RenderObject.cpp
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#include "stdafx.h"
//----------------------------------------------------------------------------------
CRenderObject::CRenderObject(const uint &serial, const ushort &graphic, const ushort &color, const short &x, const short &y)
: CBaseQueueItem(), Serial(serial), Graphic(graphic), Color(color), m_X(x), m_Y(y)
{
	UpdateRealDrawCoordinates();
}
//----------------------------------------------------------------------------------
CRenderObject::~CRenderObject()
{
	WISPFUN_DEBUG("c201_f1");
	g_ClickObject.Clear(this);
	g_SelectedObject.Clear(this);
	g_LastSelectedObject.Clear(this);
	g_PressedObject.Clear(this);
}
//----------------------------------------------------------------------------------
void CRenderObject::SetX(const int &val)
{
	m_X = val;
	UpdateRealDrawCoordinates();
}
//----------------------------------------------------------------------------------
void CRenderObject::SetY(const int &val)
{
	m_Y = val;
	UpdateRealDrawCoordinates();
}
//----------------------------------------------------------------------------------
