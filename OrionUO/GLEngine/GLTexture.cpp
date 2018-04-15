// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
/***********************************************************************************
**
** GLTexture.cpp
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#include "stdafx.h"
//----------------------------------------------------------------------------------
CGLTexture::CGLTexture()
{
}
//----------------------------------------------------------------------------------
CGLTexture::~CGLTexture()
{
	WISPFUN_DEBUG("c36_f1");
	Clear();
}
//----------------------------------------------------------------------------------
void CGLTexture::Draw(int x, int y, bool checktrans)
{
	WISPFUN_DEBUG("c36_f2");
	if (Texture != 0)
	{
		if (checktrans)
		{
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

			g_GL_Draw(*this, x, y);

			glDisable(GL_BLEND);

			glEnable(GL_STENCIL_TEST);

			g_GL_Draw(*this, x, y);

			glDisable(GL_STENCIL_TEST);
		}
		else
			g_GL_Draw(*this, x, y);
	}
}
//----------------------------------------------------------------------------------
void CGLTexture::Draw(int x, int y, int width, int height, bool checktrans)
{
	WISPFUN_DEBUG("c36_f3");
	if (Texture != 0)
	{
		if (!width)
			width = Width;

		if (!height)
			height = Height;

		if (checktrans)
		{
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

			g_GL_DrawStretched(*this, x, y, width, height);

			glDisable(GL_BLEND);

			glEnable(GL_STENCIL_TEST);

			g_GL_DrawStretched(*this, x, y, width, height);

			glDisable(GL_STENCIL_TEST);
		}
		else
			g_GL_DrawStretched(*this, x, y, width, height);
	}
}
//----------------------------------------------------------------------------------
void CGLTexture::DrawRotated(int x, int y, float angle)
{
	WISPFUN_DEBUG("c36_f4");
	if (Texture != 0)
		g_GL_DrawRotated(*this, x, y, angle);
}
//----------------------------------------------------------------------------------
void CGLTexture::DrawTransparent(int x, int y, bool stencil)
{
	WISPFUN_DEBUG("c36_f5");
	if (Texture != 0)
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glColor4f(1.0f, 1.0f, 1.0f, 0.25f);

		g_GL_Draw(*this, x, y);

		glDisable(GL_BLEND);

		if (stencil)
		{
			glEnable(GL_STENCIL_TEST);

			g_GL_Draw(*this, x, y);

			glDisable(GL_STENCIL_TEST);
		}
	}
}
//----------------------------------------------------------------------------------
void CGLTexture::Clear()
{
	WISPFUN_DEBUG("c36_f6");
	Width = 0;
	Height = 0;
	m_HitMap.clear();

	if (Texture != 0)
	{
		glDeleteTextures(1, &Texture);
		Texture = 0;
	}

	if (VertexBuffer != 0)
	{
		glDeleteBuffers(1, &VertexBuffer);
		VertexBuffer = 0;
	}

	if (MirroredVertexBuffer != 0)
	{
		glDeleteBuffers(1, &MirroredVertexBuffer);
		MirroredVertexBuffer = 0;
	}
}
//----------------------------------------------------------------------------------
bool CGLTexture::Select(int x, int y, bool pixelCheck)
{
	x = g_MouseManager.Position.X - x;
	y = g_MouseManager.Position.Y - y;

	if (x >= 0 && y >= 0 && x < Width && y < Height)
	{
		if (!pixelCheck)
			return true;

		int pos = (y * Width) + x;

		if (pos < (int)m_HitMap.size())
			return (m_HitMap[pos] != 0);
	}

	return false;
}
//----------------------------------------------------------------------------------
