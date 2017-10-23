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
: CBaseQueueItem(), m_Serial(serial), m_Graphic(graphic), m_Color(color), m_X(x), m_Y(y)
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
void CRenderObject::OnChangeX(const int &val)
{
	m_X = val;
	UpdateRealDrawCoordinates();
}
//----------------------------------------------------------------------------------
void CRenderObject::OnChangeY(const int &val)
{
	m_Y = val;
	UpdateRealDrawCoordinates();
}
//----------------------------------------------------------------------------------
