// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
/***********************************************************************************
**
** TextRenderer.cpp
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#include "stdafx.h"
//----------------------------------------------------------------------------------
CTextRenderer g_WorldTextRenderer;
//----------------------------------------------------------------------------------
CTextRenderer::CTextRenderer()
: CRenderTextObject(), m_TextItems(this), m_DrawPointer(NULL)
{
}
//----------------------------------------------------------------------------------
CTextRenderer::~CTextRenderer()
{
	WISPFUN_DEBUG("c175_f1");
	m_TextItems = NULL;
}
//---------------------------------------------------------------------------
CRenderTextObject *CTextRenderer::AddText(CRenderTextObject *obj)
{
	WISPFUN_DEBUG("c175_f2");
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
	WISPFUN_DEBUG("c175_f3");
	obj->UnlinkDraw();

	CRenderTextObject *next = m_TextItems->m_NextDraw;

	m_TextItems->m_NextDraw = obj;
	obj->m_PrevDraw = m_TextItems;
	obj->m_NextDraw = next;

	if (next != NULL)
		next->m_PrevDraw = obj;
}
//----------------------------------------------------------------------------------
bool CTextRenderer::InRect(CTextData *text, CRenderWorldObject *rwo)
{
	WISPFUN_DEBUG("c175_f4");
	bool result = false;
	CTextImageBounds rect(text);

	for (std::deque<CTextImageBounds>::iterator it = m_TextRect.begin(); it != m_TextRect.end(); ++it)
	{
		if (it->InRect(rect))
		{
			if (rwo == NULL || rwo->TextCanBeTransparent(it->m_Text))
			{
				result = true;
				break;
			}
		}
	}

	AddRect(rect);

	return result;
}
//----------------------------------------------------------------------------------
bool CTextRenderer::ProcessTextRemoveBlending(CTextData &text)
{
	if (g_ConfigManager.RemoveTextWithBlending)
	{
		int delta = text.Timer - g_Ticks;

		if (delta >= 0 && delta <= 1000)
		{
			delta = delta / 10;

			if (delta > 100)
				delta = 100;

			if (delta < 1)
				delta = 0;

			delta = (255 * delta) / 100;

			if (!text.Transparent || delta <= 0x7F)
				text.Alpha = (uchar)delta;

			text.Transparent = true;

			return true;
		}
	}

	return false;
}
//----------------------------------------------------------------------------------
bool CTextRenderer::CalculatePositions(const bool &noCalculate)
{
	WISPFUN_DEBUG("c175_f5");
	bool changed = false;

	if (!noCalculate)
		ClearRect();

	for (m_DrawPointer = m_TextItems; m_DrawPointer != NULL; m_DrawPointer = m_DrawPointer->m_NextDraw)
	{
		if (!noCalculate && m_DrawPointer->IsText())
		{
			CTextData &text = *(CTextData*)m_DrawPointer;

			if (text.Timer >= g_Ticks)
			{
				CRenderWorldObject *rwo = NULL;

				if (text.Type == TT_OBJECT)
					rwo = g_World->FindWorldObject(text.Serial);

				bool transparent = InRect((CTextData*)m_DrawPointer, rwo);

				if (text.Transparent != transparent)
					changed = true;

				text.Transparent = transparent;

				ProcessTextRemoveBlending(text);
			}
		}

		if (m_DrawPointer->m_NextDraw == NULL)
			break;
	}

	return changed;
}
//----------------------------------------------------------------------------------
void CTextRenderer::Draw()
{
	WISPFUN_DEBUG("c175_f6");
	CalculatePositions(true);

	for (CRenderTextObject *item = m_DrawPointer; item != NULL; item = item->m_PrevDraw)
	{
		if (!item->IsText())
			continue;

		CTextData &text = *(CTextData*)item;

		if (text.Timer >= g_Ticks)
		{
			ushort textColor = text.Color;

			int drawMode = 0;

			if (textColor)
			{
				g_ColorManager.SendColorsToShader(textColor);

				if (text.Unicode)
					drawMode = 3;
				else if (text.Font != 5 && text.Font != 8)
					drawMode = 2;
				else
					drawMode = 1;
			}

			glUniform1iARB(g_ShaderDrawMode, drawMode);

			if (text.Transparent)
			{
				uchar alpha = text.Alpha;

				if (alpha == 0xFF)
					alpha = 0x7F;

				glEnable(GL_BLEND);
				glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
				glColor4ub(0xFF, 0xFF, 0xFF, alpha);

				text.m_Texture.Draw(text.RealDrawX, text.RealDrawY);

				glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
				glDisable(GL_BLEND);
			}
			else
				text.m_Texture.Draw(text.RealDrawX, text.RealDrawY);
		}
	}
}
//----------------------------------------------------------------------------------
void CTextRenderer::Select(CGump *gump)
{
	WISPFUN_DEBUG("c175_f7");
	if (gump != NULL)
		CalculatePositions(true);
	else
		CalculateWorldPositions(true);

	int renderIndex = g_GameScreen.RenderIndex - 1;

	if (renderIndex < 1)
		renderIndex = 99;

	for (CRenderTextObject *item = m_DrawPointer; item != NULL; item = item->m_PrevDraw)
	{
		if (!item->IsText())
			continue;

		CTextData &text = *(CTextData*)item;

		if (text.Timer >= g_Ticks)
		{
			if (gump == NULL && (text.Owner == NULL || text.Owner->UseInRender != renderIndex))
				continue;

			if (text.m_Texture.Select(text.RealDrawX, text.RealDrawY))
				g_SelectedObject.Init(item, gump);
		}
	}
}
//----------------------------------------------------------------------------------
bool CTextRenderer::CalculateWorldPositions(const bool &noCalculate)
{
	WISPFUN_DEBUG("c175_f8");
	bool changed = false;

	if (!noCalculate)
		ClearRect();

	for (m_DrawPointer = m_TextItems; m_DrawPointer != NULL; m_DrawPointer = m_DrawPointer->m_NextDraw)
	{
		if (!noCalculate && m_DrawPointer->IsText())
		{
			CTextData &text = *(CTextData*)m_DrawPointer;

			if (text.Timer >= g_Ticks)
			{
				if (text.Owner != NULL)
				{
					text.Transparent = InRect((CTextData*)m_DrawPointer, text.Owner);

					ProcessTextRemoveBlending(text);
				}
			}
		}

		if (m_DrawPointer->m_NextDraw == NULL)
			break;
	}

	return changed;
}
//----------------------------------------------------------------------------------
void CTextRenderer::WorldDraw()
{
	WISPFUN_DEBUG("c175_f9");
	CalculateWorldPositions(true);

	int renderIndex = g_GameScreen.RenderIndex - 1;

	if (renderIndex < 1)
		renderIndex = 99;

	for (CRenderTextObject *item = m_DrawPointer; item != NULL; item = item->m_PrevDraw)
	{
		if (!item->IsText())
			continue;

		CTextData &text = *(CTextData*)item;

		if (text.Type != TT_SYSTEM && text.Timer >= g_Ticks)
		{
			CRenderWorldObject *rwo = text.Owner;

			if (rwo == NULL || rwo->UseInRender != renderIndex)
				continue;

			ushort textColor = text.Color;

			if (text.Type == TT_OBJECT && g_SelectedObject.Object == item && (((CGameObject*)rwo)->NPC || ((CGameObject*)rwo)->IsCorpse()))
				textColor = 0x0035;

			int drawMode = 0;

			if (textColor)
			{
				g_ColorManager.SendColorsToShader(textColor);

				if (text.Unicode)
					drawMode = 3;
				else if (text.Font != 5 && text.Font != 8)
					drawMode = 2;
				else
					drawMode = 1;
			}

			glUniform1iARB(g_ShaderDrawMode, drawMode);

			if (text.Transparent)
			{
				uchar alpha = text.Alpha;

				if (alpha == 0xFF)
					alpha = 0x7F;

				glEnable(GL_BLEND);
				glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
				glColor4ub(0xFF, 0xFF, 0xFF, alpha);

				text.m_Texture.Draw(text.RealDrawX, text.RealDrawY);

				glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
				glDisable(GL_BLEND);
			}
			else
				text.m_Texture.Draw(text.RealDrawX, text.RealDrawY);
		}
	}
}
//----------------------------------------------------------------------------------
