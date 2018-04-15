// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
/***********************************************************************************
**
** RenderStaticObject.cpp
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#include "stdafx.h"
//----------------------------------------------------------------------------------
CRenderStaticObject::CRenderStaticObject(const RENDER_OBJECT_TYPE &renderType, const uint &serial, const ushort &graphic, const ushort &color, const short &x, const short &y, const char &z)
: CMapObject(renderType, serial, graphic, color, x, y, z)
{
	WISPFUN_DEBUG("c27_f1");
	m_TiledataPtr = &g_Orion.m_StaticData[graphic];

	if (m_TiledataPtr->Height > 5)
		CanBeTransparent = 1;
	else if (IsRoof() || (IsSurface() && IsBackground()) || IsWall())
		CanBeTransparent = 1;
	else if (m_TiledataPtr->Height == 5 && IsSurface() && !IsBackground())
		CanBeTransparent = 1;
	else
		CanBeTransparent = 0;

	if (renderType == ROT_GAME_OBJECT)
	{
		m_TextControl = new CTextContainer(5);

		if (IsSurface() || (IsBackground() && IsUnknown2()))
			CanBeTransparent |= 0x10;
	}
	else
	{
		if (IsWet())
			m_DrawTextureColor[3] = 0xFF;

		if (IsTranslucent())
			m_DrawTextureColor[3] = TRANSLUCENT_ALPHA;

		m_TextControl = new CTextContainer(1);

		if (IsSurface() || (IsBackground() && IsUnknown2()) || IsRoof())
			CanBeTransparent |= 0x10;
	}
}
//---------------------------------------------------------------------------
CRenderStaticObject::~CRenderStaticObject()
{
	WISPFUN_DEBUG("c27_f2");
	if (m_TextControl != NULL)
	{
		delete m_TextControl;
		m_TextControl = NULL;
	}
}
//---------------------------------------------------------------------------
void CRenderStaticObject::UpdateTextCoordinates()
{
	int offset = 0;

	if (IsGameObject() && ((CGameObject*)this)->Container != 0xFFFFFFFF)
	{
		for (CTextData *item = (CTextData*)m_TextControl->Last(); item != NULL; item = (CTextData*)item->m_Prev)
		{
			CTextData &text = *item;

			if (!offset && text.Timer < g_Ticks)
				continue;

			offset += text.m_Texture.Height;

			text.RealDrawX = text.GetX() - (text.m_Texture.Width / 2);
			text.RealDrawY = text.GetY() - offset;
		}
	}
	else
	{
		int y = DrawY - (m_TiledataPtr->Height + 20);

		for (CTextData *item = (CTextData*)m_TextControl->Last(); item != NULL; item = (CTextData*)item->m_Prev)
		{
			CTextData &text = *item;

			if (!offset && text.Timer < g_Ticks)
				continue;

			offset += text.m_Texture.Height;

			text.RealDrawX = DrawX - (text.m_Texture.Width / 2);
			text.RealDrawY = y - offset;
		}
	}
}
//---------------------------------------------------------------------------
void CRenderStaticObject::FixTextCoordinates()
{
	if (IsGameObject() && ((CGameObject*)this)->Container != 0xFFFFFFFF)
		return;

	int offsetY = 0;

	int minX = g_RenderBounds.GameWindowPosX;
	int maxX = minX + g_RenderBounds.GameWindowWidth;
	int minY = g_RenderBounds.GameWindowPosY;
	int maxY = minY + g_RenderBounds.GameWindowHeight;

	for (CTextData *item = (CTextData*)m_TextControl->m_Items; item != NULL; item = (CTextData*)item->m_Next)
	{
		CTextData &text = *item;

		if (text.Timer < g_Ticks)
			continue;



		int startX = text.RealDrawX;
		int endX = startX + text.m_Texture.Width;

		if (startX < minX)
			text.RealDrawX += minX - startX;

		if (endX > maxX)
			text.RealDrawX -= endX - maxX;



		int startY = text.RealDrawY;
		int endY = startY + text.m_Texture.Height;

		if (startY < minY && !offsetY)
			offsetY = minY - startY;

		if (offsetY)
			text.RealDrawY += offsetY;
	}
}
//---------------------------------------------------------------------------
bool CRenderStaticObject::IsNoDrawTile(const ushort &graphic)
{
	switch (graphic)
	{
		case 0x0001:
		case 0x21BC:
		case 0x9E4C:
		case 0x9E64:
		case 0x9E65:
		case 0x9E7D:
			return true;
		default:
			break;
	}

	if (graphic != 0x63D3)
	{
		if (graphic >= 0x2198 && graphic <= 0x21A4)
			return true;

		long long flags = g_Orion.GetStaticFlags(graphic);

		if (!::IsNoDiagonal(flags) || (::IsAnimated(flags) && g_Player != NULL && g_Player->Race == RT_GARGOYLE))
			return false;
	}

	return true;
}
//----------------------------------------------------------------------------------
void CRenderStaticObject::Draw(const int &x, const int &y)
{
	WISPFUN_DEBUG("c27_f3");
#if UO_DEBUG_INFO!=0
	g_RenderedObjectsCountInGameWindow++;
#endif

	bool useAlpha = (m_DrawTextureColor[3] != 0xFF);

	if (useAlpha)
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glColor4ub(m_DrawTextureColor[0], m_DrawTextureColor[1], m_DrawTextureColor[2], m_DrawTextureColor[3]);
	}

	if (g_UseCircleTrans)
		g_Orion.DrawStaticArtAnimatedTransparent(RenderGraphic, RenderColor, x, y);
	else
		g_Orion.DrawStaticArtAnimated(RenderGraphic, RenderColor, x, y);

	if (useAlpha)
	{
		glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
		glDisable(GL_BLEND);
	}

	if (IsLightSource() && g_GameScreen.UseLight)
		g_GameScreen.AddLight(this, this, x, y);
}
//----------------------------------------------------------------------------------
void CRenderStaticObject::Select(const int &x, const int &y)
{
	WISPFUN_DEBUG("c27_f4");
	if (m_DrawTextureColor[3] != 0xFF)
	{
		if (!IsTranslucent() || m_DrawTextureColor[3] != TRANSLUCENT_ALPHA)
			return;
	}

	if (!g_UseCircleTrans && g_Orion.StaticPixelsInXYAnimated(RenderGraphic, x, y))
		g_SelectedObject.Init(this);
}
//---------------------------------------------------------------------------
void CRenderStaticObject::AddText(CTextData *msg)
{
	WISPFUN_DEBUG("c27_f5");
	if (m_TextControl != NULL)
	{
		msg->Owner = this;
		m_TextControl->Add(msg);
		Changed = true;

		g_Orion.AddJournalMessage(msg, "You see: ");
	}
}
//---------------------------------------------------------------------------
bool CRenderStaticObject::TextCanBeTransparent(CRenderTextObject *text)
{
	WISPFUN_DEBUG("c27_f8");
	bool result = true;

	QFOR(item, m_TextControl->m_Items, CTextData*)
	{
		if (text == item)
		{
			result = false;
			break;
		}
	}

	return result;
}
//---------------------------------------------------------------------------
bool CRenderStaticObject::TranparentTest(const int &playerZPlus5)
{
	WISPFUN_DEBUG("c27_f9");
	bool result = true;

	if (m_Z <= playerZPlus5 - m_TiledataPtr->Height)
		result = false;
	else if (playerZPlus5 < m_Z && !(CanBeTransparent & 0xF))
		result = false;

	return result;
}
//---------------------------------------------------------------------------
bool CRenderStaticObject::CheckDrawFoliage()
{
	if (IsFoliage())
	{
		if (g_Season < ST_WINTER)
		{
			if (g_ConfigManager.GetDrawStumps())
				return g_Orion.InTileFilter(Graphic);

			return true;
		}

		return false;
	}

	return true;
}
//---------------------------------------------------------------------------
bool CRenderStaticObject::CheckDrawVegetation()
{
	if (g_ConfigManager.GetNoVegetation() && Vegetation)
		return g_Orion.InTileFilter(Graphic);

	return true;
}
//----------------------------------------------------------------------------------
