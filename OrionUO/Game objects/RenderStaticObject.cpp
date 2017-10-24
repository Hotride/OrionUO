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
		m_CanBeTransparent = 1;
	else if (IsRoof() || (IsSurface() && IsBackground()) || IsWall())
		m_CanBeTransparent = 1;
	else if (m_TiledataPtr->Height == 5 && IsSurface() && !IsBackground())
		m_CanBeTransparent = 1;
	else
		m_CanBeTransparent = 0;

	if (renderType == ROT_GAME_OBJECT)
	{
		m_TextControl = new CTextContainer(3);

		if (IsSurface() || (IsBackground() && IsUnknown2()))
			m_CanBeTransparent |= 0x10;
	}
	else
	{
		if (IsTranslucent())
			m_DrawTextureColor[3] = TRANSLUCENT_ALPHA;

		m_TextControl = new CTextContainer(1);

		if (IsSurface() || (IsBackground() && IsUnknown2()) || IsRoof())
			m_CanBeTransparent |= 0x10;
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
		/*uint container = ((CGameObject*)this)->Container;
		g_GumpManager.UpdateGump(container, 0, GT_CONTAINER);
		g_GumpManager.UpdateGump(container, 0, GT_PAPERDOLL);
		g_GumpManager.UpdateGump(container, 0, GT_TRADE);*/

		for (CTextData *text = (CTextData*)m_TextControl->Last(); text != NULL; text = (CTextData*)text->m_Prev)
		{
			CTextData &td = *text;
			offset += td.m_Texture.Height;

			td.RealDrawX = td.X - (td.m_Texture.Width / 2);
			td.RealDrawY = td.Y - offset;
		}
	}
	else
	{
		int y = m_DrawY - (m_TiledataPtr->Height + 20);

		for (CTextData *text = (CTextData*)m_TextControl->Last(); text != NULL; text = (CTextData*)text->m_Prev)
		{
			CTextData &td = *text;
			offset += td.m_Texture.Height;

			td.RealDrawX = m_DrawX - (td.m_Texture.Width / 2);
			td.RealDrawY = y - offset;
		}
	}
}
//---------------------------------------------------------------------------
void CRenderStaticObject::FixTextCoordinates()
{
	int offsetY = 0;

	int minX = g_RenderBounds.GameWindowPosX;
	int maxX = minX + g_RenderBounds.GameWindowWidth;
	int minY = g_RenderBounds.GameWindowPosY;
	int maxY = minY + g_RenderBounds.GameWindowHeight;

	for (CTextData *item = (CTextData*)m_TextControl->m_Items; item != NULL; item = (CTextData*)item->m_Next)
	{
		CTextData &text = *item;

		if (text.Timer < g_Ticks || text.Owner == NULL || text.Owner->UseInRender == 0xFF)
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
			item->RealDrawY += offsetY;

		/*if (endY > maxY)
		{
			//if (startY >= maxY)
			//	continue;

			item->RealDrawY -= endY - maxY;
		}*/
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
		g_Orion.DrawStaticArtAnimatedTransparent(m_RenderGraphic, m_RenderColor, x, y);
	else
		g_Orion.DrawStaticArtAnimated(m_RenderGraphic, m_RenderColor, x, y);

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

	if (!g_UseCircleTrans && g_Orion.StaticPixelsInXYAnimated(m_RenderGraphic, x, y))
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
		UpdateTextCoordinates();

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
bool CRenderStaticObject::TranparentTest(int &playerZPlus5)
{
	WISPFUN_DEBUG("c27_f9");
	bool result = true;

	if (m_Z <= playerZPlus5 - m_TiledataPtr->Height)
		result = false;
	else if (playerZPlus5 < m_Z && !(m_CanBeTransparent & 0xF))
		result = false;

	return result;
}
//----------------------------------------------------------------------------------
