// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
/***********************************************************************************
**
** RenderTextObject.cpp
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#include "stdafx.h"
//----------------------------------------------------------------------------------
CRenderTextObject::CRenderTextObject()
: CRenderObject(0, 0, 0, 0, 0)
{
}
//---------------------------------------------------------------------------
CRenderTextObject::~CRenderTextObject()
{
	WISPFUN_DEBUG("c172_f1");
	UnlinkDraw();
}
//---------------------------------------------------------------------------
void CRenderTextObject::UnlinkDraw()
{
	WISPFUN_DEBUG("c172_f2");
	if (m_NextDraw != NULL)
		m_NextDraw->m_PrevDraw = m_PrevDraw;

	if (m_PrevDraw != NULL)
		m_PrevDraw->m_NextDraw = m_NextDraw;

	m_NextDraw = NULL;
	m_PrevDraw = NULL;
}
//---------------------------------------------------------------------------
void CRenderTextObject::ToTop()
{
	WISPFUN_DEBUG("c172_f3");
	CRenderTextObject *obj = this;

	while (obj != NULL)
	{
		if (obj->m_PrevDraw == NULL)
			break;

		obj = obj->m_PrevDraw;
	}

	CTextRenderer *tr = (CTextRenderer*)obj;
	tr->ToTop(this);
}
//----------------------------------------------------------------------------------
