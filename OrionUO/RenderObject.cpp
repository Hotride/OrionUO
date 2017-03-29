/***********************************************************************************
**
** RenderObject.cpp
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#include "RenderObject.h"
#include "SelectedObject.h"
#include "PressedObject.h"
#include "ClickObject.h"
//----------------------------------------------------------------------------------
CRenderObject::CRenderObject(const uint &serial, const ushort &graphic, const ushort &color, const short &x, const short &y)
: CBaseQueueItem(), m_Serial(serial), m_Graphic(graphic), m_Color(color), m_X(x), m_Y(y)
{
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
