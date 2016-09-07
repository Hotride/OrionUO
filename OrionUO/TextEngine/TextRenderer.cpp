/***********************************************************************************
**
** TextRenderer.cpp
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#include "TextRenderer.h"
//----------------------------------------------------------------------------------
CTextRenderer g_WorldTextRenderer;
//----------------------------------------------------------------------------------
CTextRenderer::CTextRenderer()
: CRenderTextObject(), m_TextItems(this)
{
}
//----------------------------------------------------------------------------------
CTextRenderer::~CTextRenderer()
{
	m_TextItems = NULL;
}
//---------------------------------------------------------------------------
CRenderTextObject *CTextRenderer::AddText(CRenderTextObject *obj)
{
	if (obj != NULL)
	{
		CRenderTextObject *item = m_TextItems;

		if (item != NULL)
		{
			if (item->m_NextDraw != NULL)
			{
				CRenderTextObject *next = item->m_NextDraw;

				item->m_NextDraw = obj;
				obj->m_PrevDraw = item;
				obj->m_NextDraw = next;
				next->m_PrevDraw = obj;
			}
			else
			{
				item->m_NextDraw = obj;
				obj->m_PrevDraw = item;
				obj->m_NextDraw = NULL;
			}
		}
	}

	return obj;
}
//---------------------------------------------------------------------------
void CTextRenderer::ToTop(CRenderTextObject *obj)
{
	obj->UnlinkDraw();

	CRenderTextObject *next = m_TextItems->m_NextDraw;

	m_TextItems->m_NextDraw = obj;
	obj->m_PrevDraw = m_TextItems;
	obj->m_NextDraw = next;

	if (next != NULL)
		next->m_PrevDraw = obj;
}
//----------------------------------------------------------------------------------
bool CTextRenderer::InRect(CTextImageBounds &rect, CRenderWorldObject *rwo)
{
	bool result = false;

	for (std::deque<CTextImageBounds>::iterator it = m_TextRect.begin(); it != m_TextRect.end(); it++)
	{
		if (it->InRect(rect))
		{
			if (rwo == NULL || (rwo != NULL && rwo->TextCanBeTransparent(it->m_Text)))
			{
				result = true;
				break;
			}
		}
	}

	return result;
}
//----------------------------------------------------------------------------------
