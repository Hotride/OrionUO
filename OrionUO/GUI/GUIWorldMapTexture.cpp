/***********************************************************************************
**
** GUIWorldMapTexture.cpp
**
** Copyright (C) September 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#include "GUIWorldMapTexture.h"
#include "../OrionUO.h"
#include "../Managers/ColorManager.h"
#include "../Managers/MouseManager.h"
#include "../Managers/MulReader.h"
//----------------------------------------------------------------------------------
CGUIWorldMapTexture::CGUIWorldMapTexture(const int &x, const int &y)
: CBaseGUI(GOT_EXTERNALTEXTURE, 0, 0, 0, x, y), m_Index(0), m_Width(0), m_Height(0),
m_OffsetX(0), m_OffsetY(0)
{
}
//----------------------------------------------------------------------------------
CGUIWorldMapTexture::~CGUIWorldMapTexture()
{
}
//----------------------------------------------------------------------------------
void CGUIWorldMapTexture::Draw(const bool &checktrans)
{
	if (g_MapTexture[m_Index].Texture != 0)
	{
		CGLTexture tex;
		tex.Texture = g_MapTexture[m_Index].Texture;
		tex.Width = m_Width;
		tex.Height = m_Height;

		g_GL.GL1_Draw(tex, m_X + m_OffsetX, m_Y + m_OffsetY);

		tex.Texture = 0;

		//g_MapTexture[m_Index].Draw(m_X + m_OffsetX, m_Y + m_OffsetY, m_Width, m_Height, checktrans);
	}
}
//----------------------------------------------------------------------------------
bool CGUIWorldMapTexture::Select()
{
	bool select = false;

	int x = g_MouseManager.Position.X - m_X;
	int y = g_MouseManager.Position.Y - m_Y;

	if (x >= 0 && y >= 0 && x < m_Width && y < m_Height)
		select = true;

	return select;
}
//----------------------------------------------------------------------------------
