/***********************************************************************************
**
** GLTextTexture.cpp
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#include "GLTextTexture.h"
#include "GLEngine.h"
#include "../Managers/MouseManager.h"
//----------------------------------------------------------------------------------
CGLTextTexture::CGLTextTexture()
: CGLTexture(), m_LinesCount(0)
{
}
//----------------------------------------------------------------------------------
CGLTextTexture::~CGLTextTexture()
{
}
//----------------------------------------------------------------------------------
void CGLTextTexture::Clear()
{
	CGLTexture::Clear();
	m_LinesCount = 0;
}
//----------------------------------------------------------------------------------
void CGLTextTexture::Draw(const int &x, const int &y, const bool &checktrans)
{
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
bool CGLTextTexture::UnderMouse(int x, int y)
{
	if (PixelsData.size())
	{
		x = g_MouseManager.Position.X - x;
		y = g_MouseManager.Position.Y - y;

		if (x > 0 && y > 0 && x < m_Width && y < m_Height)
			return (PixelsData[(y * m_Width) + x] != 0);
	}

	return false;
}
//----------------------------------------------------------------------------------