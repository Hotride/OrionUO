/***********************************************************************************
**
** GUITilepic.cpp
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#include "GUITilepic.h"
#include "../OrionUO.h"
#include "../Managers/ColorManager.h"
#include "../Managers/MouseManager.h"
#include "../Managers/MulReader.h"
//----------------------------------------------------------------------------------
CGUITilepic::CGUITilepic(const ushort &graphic, const ushort &color, const int &x, const int &y)
: CGUIDrawObject(GOT_TILEPIC, 0, graphic, color, x, y)
{
}
//----------------------------------------------------------------------------------
CGUITilepic::~CGUITilepic()
{
}
//----------------------------------------------------------------------------------
WISP_GEOMETRY::CSize CGUITilepic::GetSize()
{
	WISP_GEOMETRY::CSize size;

	CGLTexture *th = g_Orion.ExecuteStaticArt(m_Graphic);

	if (th != NULL)
	{
		size.Width = th->Width;
		size.Height = th->Height;
	}

	return size;
}
//----------------------------------------------------------------------------------
void CGUITilepic::PrepareTextures()
{
	g_Orion.ExecuteStaticArt(m_Graphic);
}
//----------------------------------------------------------------------------------
void CGUITilepic::Draw(const bool &checktrans)
{
	CGLTexture *th = g_Orion.ExecuteStaticArt(m_Graphic);

	if (th != NULL)
	{
		SetShaderMode();

		th->Draw(m_X, m_Y, checktrans);
	}
}
//----------------------------------------------------------------------------------
bool CGUITilepic::Select()
{
	//if (CGUIDrawObject::Select())
	//	return true;

	bool select = false;

	CIndexObject &io = g_Orion.m_StaticDataIndex[m_Graphic];

	CGLTexture *th = io.Texture;

	if (th != NULL)
	{
		int x = g_MouseManager.Position.X - m_X;
		int y = g_MouseManager.Position.Y - m_Y;

		if (x >= 0 && y >= 0 && x < th->Width && y < th->Height)
		{
#if UO_ENABLE_TEXTURE_DATA_SAVING == 1
			select = (m_CheckPolygone || th->PixelsData[(y * th->Width) + x] != 0);
#else
			select = (m_CheckPolygone || g_MulReader->ArtPixelsInXY(false, io, x, y));
#endif
		}
	}

	return select;
}
//----------------------------------------------------------------------------------
