/***********************************************************************************
**
** GUITilepicHightlighted.cpp
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#include "stdafx.h"
//----------------------------------------------------------------------------------
CGUITilepicHightlighted::CGUITilepicHightlighted(const uint &serial, const ushort &graphic, const ushort &color, const ushort &selectedColor, const int &x, const int &y, const bool &doubleDraw)
: CGUITilepic(graphic, color, x, y), m_SelectedColor(selectedColor),
m_DoubleDraw(doubleDraw)
{
	m_Type = GOT_TILEPICHIGHTLIGHTED;
	m_Serial = serial;
}
//----------------------------------------------------------------------------------
CGUITilepicHightlighted::~CGUITilepicHightlighted()
{
}
//----------------------------------------------------------------------------------
void CGUITilepicHightlighted::SetShaderMode()
{
	WISPFUN_DEBUG("c81_f1");
	int drawMode = (m_Color != 0);

	if (g_SelectedObject.Object == this)
	{
		drawMode = 1;

		g_ColorManager.SendColorsToShader(m_SelectedColor);
	}
	else if (drawMode)
	{
		if (m_PartialHue)
			drawMode = 2;

		g_ColorManager.SendColorsToShader(m_Color);
	}

	glUniform1iARB(g_ShaderDrawMode, drawMode);
}
//----------------------------------------------------------------------------------
void CGUITilepicHightlighted::Draw(const bool &checktrans)
{
	WISPFUN_DEBUG("c81_f2");
	CGLTexture *th = g_Orion.ExecuteStaticArt(m_Graphic);

	if (th != NULL)
	{
		SetShaderMode();

		th->Draw(m_X, m_Y, checktrans);

		if (m_DoubleDraw)
			th->Draw(m_X + 5, m_Y + 5, checktrans);
	}
}
//----------------------------------------------------------------------------------
bool CGUITilepicHightlighted::Select()
{
	WISPFUN_DEBUG("c81_f3");
	bool select = false;

	CIndexObject &io = g_Orion.m_StaticDataIndex[m_Graphic];

	CGLTexture *th = io.Texture;

	if (th != NULL)
	{
		int count = 1 + m_DoubleDraw;
		static const int offset[2] = { 0, 5 };

		IFOR(i, 0, count && !select)
		{
			int x = g_MouseManager.Position.X - (m_X + offset[i]);
			int y = g_MouseManager.Position.Y - (m_Y + offset[i]);

			if (x >= 0 && y >= 0 && x < th->Width && y < th->Height)
			{
#if UO_ENABLE_TEXTURE_DATA_SAVING == 1
				select = (m_CheckPolygone || th->PixelsData[(y * th->Width) + x] != 0);
#else
				select = (m_CheckPolygone || g_UOFileReader.ArtPixelsInXY(false, io, x, y));
#endif
			}
		}
	}

	return select;
}
//----------------------------------------------------------------------------------
void CGUITilepicHightlighted::OnMouseEnter()
{
	WISPFUN_DEBUG("c81_f4");
	if (g_SelectedObject.Gump != NULL)
		g_SelectedObject.Gump->WantRedraw = true;
}
//----------------------------------------------------------------------------------
void CGUITilepicHightlighted::OnMouseExit()
{
	WISPFUN_DEBUG("c81_f5");
	if (g_LastSelectedObject.Gump != NULL)
		g_LastSelectedObject.Gump->WantRedraw = true;
}
//----------------------------------------------------------------------------------
