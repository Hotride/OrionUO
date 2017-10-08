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
CGUIButtonTileart::CGUIButtonTileart(const uint &serial, const ushort &graphic, const ushort &graphicSelected, const ushort &graphicPressed, const int &x, const int &y, const ushort &tileGraphic, const ushort &tileColor, const int &tileX, const int &tileY)
: CGUIButton(serial, graphic, graphicSelected, graphicPressed, x, y),
m_TileGraphic(tileGraphic), m_TileColor(tileColor), m_TileX(tileX), m_TileY(tileY)
{
	m_Type = GOT_BUTTONTILEART;
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

	CGLTexture *th = g_Orion.ExecuteStaticArt(m_TileGraphic);

	if (th != NULL)
	{
		tileSize.Width = th->Width;
		tileSize.Height = th->Height;
	}

	int startX = m_X;
	int endX = m_X + gumpSize.Width;

	if (m_TileX < startX)
		startX = m_TileX;

	if (m_TileX + tileSize.Width > endX)
		endX = m_TileX + tileSize.Width;

	int startY = m_Y;
	int endY = m_Y + gumpSize.Height;

	if (m_TileY < startY)
		startY = m_TileY;

	if (m_TileY + tileSize.Height > endY)
		endY = m_TileY + tileSize.Height;

	return WISP_GEOMETRY::CSize(abs(endX) - abs(startX), abs(endY) - abs(startY));
}
//----------------------------------------------------------------------------------
void CGUIButtonTileart::PrepareTextures()
{
	WISPFUN_DEBUG("c45_f2");
	CGUIButton::PrepareTextures();

	g_Orion.ExecuteStaticArt(m_TileGraphic);
}
//----------------------------------------------------------------------------------
void CGUIButtonTileart::Draw(const bool &checktrans)
{
	WISPFUN_DEBUG("c45_f3");
	CGUIDrawObject::Draw(checktrans);

	CGLTexture *th = g_Orion.ExecuteStaticArt(m_TileGraphic);

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

	bool select = false;

	CIndexObject &io = g_Orion.m_StaticDataIndex[m_TileGraphic];

	CGLTexture *th = g_Orion.ExecuteStaticArt(m_TileGraphic);

	if (th != NULL)
	{
		int x = g_MouseManager.Position.X - m_X;
		int y = g_MouseManager.Position.Y - m_Y;

		if (x >= 0 && y >= 0 && x < th->Width && y < th->Height)
		{
#if UO_ENABLE_TEXTURE_DATA_SAVING == 1
			select = (m_CheckPolygone || th->PixelsData[(y * th->Width) + x] != 0);
#else
			select = (m_CheckPolygone || g_UOFileReader.ArtPixelsInXY(false, io, x, y));
#endif
		}
	}

	return select;
}
//----------------------------------------------------------------------------------
