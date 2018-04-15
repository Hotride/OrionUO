// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
/***********************************************************************************
**
** GUIButtonTileart.cpp
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#include "stdafx.h"
//----------------------------------------------------------------------------------
CGUIButtonTileart::CGUIButtonTileart(int serial, ushort graphic, ushort graphicSelected, ushort graphicPressed, int x, int y, ushort tileGraphic, ushort tileColor, int tileX, int tileY)
: CGUIButton(serial, graphic, graphicSelected, graphicPressed, x, y),
TileGraphic(tileGraphic), TileColor(tileColor), TileX(tileX), TileY(tileY)
{
	Type = GOT_BUTTONTILEART;
}
//----------------------------------------------------------------------------------
CGUIButtonTileart::~CGUIButtonTileart()
{
}
//----------------------------------------------------------------------------------
WISP_GEOMETRY::CSize CGUIButtonTileart::GetSize()
{
	WISPFUN_DEBUG("c45_f1");
	WISP_GEOMETRY::CSize gumpSize = CGUIDrawObject::GetSize();
	WISP_GEOMETRY::CSize tileSize;

	CGLTexture *th = g_Orion.ExecuteStaticArt(TileGraphic);

	if (th != NULL)
	{
		tileSize.Width = th->Width;
		tileSize.Height = th->Height;
	}

	int startX = m_X;
	int endX = m_X + gumpSize.Width;

	if (TileX < startX)
		startX = TileX;

	if (TileX + tileSize.Width > endX)
		endX = TileX + tileSize.Width;

	int startY = m_Y;
	int endY = m_Y + gumpSize.Height;

	if (TileY < startY)
		startY = TileY;

	if (TileY + tileSize.Height > endY)
		endY = TileY + tileSize.Height;

	return WISP_GEOMETRY::CSize(abs(endX) - abs(startX), abs(endY) - abs(startY));
}
//----------------------------------------------------------------------------------
void CGUIButtonTileart::PrepareTextures()
{
	WISPFUN_DEBUG("c45_f2");
	CGUIButton::PrepareTextures();

	g_Orion.ExecuteStaticArt(TileGraphic);
}
//----------------------------------------------------------------------------------
void CGUIButtonTileart::Draw(bool checktrans)
{
	WISPFUN_DEBUG("c45_f3");
	CGUIDrawObject::Draw(checktrans);

	CGLTexture *th = g_Orion.ExecuteStaticArt(TileGraphic);

	if (th != NULL)
	{
		SetShaderMode();

		th->Draw(m_X, m_Y, checktrans);
	}
}
//----------------------------------------------------------------------------------
bool CGUIButtonTileart::Select()
{
	WISPFUN_DEBUG("c45_f4");
	if (CGUIDrawObject::Select())
		return true;

	CGLTexture *th = g_Orion.ExecuteStaticArt(TileGraphic);

	if (th != NULL)
		return th->Select(m_X, m_Y, !CheckPolygone);

	return false;
}
//----------------------------------------------------------------------------------
