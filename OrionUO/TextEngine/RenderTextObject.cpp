/***********************************************************************************
**
** RenderTextObject.cpp
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#include "RenderTextObject.h"
#include "TextRenderer.h"
//----------------------------------------------------------------------------------
CRenderTextObject::CRenderTextObject()
: CRenderObject(0, 0, 0, 0, 0), m_Transparent(false), m_NextDraw(NULL), m_PrevDraw(NULL)
{
}
//---------------------------------------------------------------------------
CRenderTextObject::~CRenderTextObject()
{
	UnlinkDraw();
}
//---------------------------------------------------------------------------
void CRenderTextObject::UnlinkDraw()
{
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
