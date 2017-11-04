// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
/***********************************************************************************
**
** GUIWorldMapTexture.cpp
**
** Copyright (C) September 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#include "stdafx.h"
//----------------------------------------------------------------------------------
CGUIWorldMapTexture::CGUIWorldMapTexture(const int &x, const int &y)
: CBaseGUI(GOT_EXTERNALTEXTURE, 0, 0, 0, x, y)
{
}
//----------------------------------------------------------------------------------
CGUIWorldMapTexture::~CGUIWorldMapTexture()
{
}
//----------------------------------------------------------------------------------
void CGUIWorldMapTexture::Draw(const bool &checktrans)
{
	WISPFUN_DEBUG("c83_f1");
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
	WISPFUN_DEBUG("c83_f2");
	bool select = false;

	int x = g_MouseManager.Position.X - m_X;
	int y = g_MouseManager.Position.Y - m_Y;

	if (x >= 0 && y >= 0 && x < m_Width && y < m_Height)
		select = true;

	return select;
}
//----------------------------------------------------------------------------------
