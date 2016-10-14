/***********************************************************************************
**
** RenderWorldObject.cpp
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#include "RenderWorldObject.h"
#include "SelectedObject.h"
#include "PressedObject.h"
//----------------------------------------------------------------------------------
//---------------------------------RenderWorldObject--------------------------------
//----------------------------------------------------------------------------------
CRenderWorldObject::CRenderWorldObject(const RENDER_OBJECT_TYPE &renderType, const uint &serial, const ushort &graphic, const ushort &color, const int &x, const int &y, const char &z)
: CRenderObject(serial, graphic, color, x, y), m_RenderType(renderType),
m_Z(z), m_NextXY(NULL), m_PrevXY(NULL), m_RenderQueueIndex(0)
#if UO_RENDER_LIST_SORT == 1
, m_CurrentRenderIndex(0)
#endif
{
}
//---------------------------------------------------------------------------
CRenderWorldObject::~CRenderWorldObject()
{
	RemoveRender();
}
//---------------------------------------------------------------------------
void CRenderWorldObject::RemoveRender()
{
	if (m_PrevXY != NULL)
		m_PrevXY->m_NextXY = m_NextXY;

	if (m_NextXY != NULL)
		m_NextXY->m_PrevXY = m_PrevXY;

	m_NextXY = NULL;
	m_PrevXY = NULL;
}
//---------------------------------------------------------------------------
CLandObject *CRenderWorldObject::GetLand()
{
	CRenderWorldObject *land = this;

	while (land != NULL && !land->IsLandObject())
		land = land->m_NextXY;

	return (CLandObject*)land;
}
//---------------------------------------------------------------------------