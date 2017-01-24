/***********************************************************************************
**
** GLTextureCircleOfTransparency.cpp
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#include "GLTextureCircleOfTransparency.h"
#include "GLEngine.h"
#include "../Managers/ConfigManager.h"
//----------------------------------------------------------------------------------
CGLTextureCircleOfTransparency g_CircleOfTransparency;
//----------------------------------------------------------------------------------
CGLTextureCircleOfTransparency::CGLTextureCircleOfTransparency()
: CGLTexture(), m_Radius(0), m_X(0), m_Y(0)
{
}
//----------------------------------------------------------------------------------
CGLTextureCircleOfTransparency::~CGLTextureCircleOfTransparency()
{
	Clear();
}
//---------------------------------------------------------------------------
void CGLTextureCircleOfTransparency::CreatePixels(const int &radius, int &width, int &height, UINT_LIST &pixels)
{
	int fixRadius = radius + 1;
	int mulRadius = fixRadius * 2;

	pixels.resize(mulRadius * mulRadius);

	width = mulRadius;
	height = mulRadius;

	IFOR(x, -fixRadius, fixRadius)
	{
		int mulX = x * x;
		int posX = ((x + fixRadius) * mulRadius) + fixRadius;

		IFOR(y, -fixRadius, fixRadius)
		{
			int r = (int)sqrt(mulX + (y * y));

			int pos = posX + y;

			if (r <= radius)
				pixels[pos] = (radius - r) & 0xFF;
			else
				pixels[pos] = 0;
		}
	}
}
//---------------------------------------------------------------------------
bool CGLTextureCircleOfTransparency::Create(int radius)
{
	if (radius <= 0)
		return false;
	else if (radius > 200)
		radius = 200;

	if (radius == m_Radius)
		return true;

	UINT_LIST pixels;

	CreatePixels(radius, m_Width, m_Height, pixels);

	PixelsData.resize(m_Width * m_Height);

	IFOR(i, 0, (int)pixels.size())
		PixelsData[i] = (pixels[i] ? 1 : 0);

	m_Radius = radius;

	if (Texture != NULL)
	{
		glDeleteTextures(1, &Texture);
		Texture = 0;
	}

	g_GL_BindTexture32(*this, m_Width, m_Height, &pixels[0]);

	return true;
}
//---------------------------------------------------------------------------
void CGLTextureCircleOfTransparency::Draw(const int &x, const int &y, const bool &checktrans)
{
	if (Texture != 0)
	{
		m_X = x - m_Width / 2;
		m_Y = y - m_Height / 2;

		glEnable(GL_STENCIL_TEST);

		glColorMask(false, false, false, true);

		glStencilFunc(GL_ALWAYS, 1, 1);
		glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

		g_GL_Draw(*this, m_X, m_Y);

		glColorMask(true, true, true, true);

		glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
		glStencilFunc(GL_NOTEQUAL, 1, 1);

		glDisable(GL_STENCIL_TEST);
	}
}
//---------------------------------------------------------------------------
void CGLTextureCircleOfTransparency::Redraw()
{
	glClear(GL_STENCIL_BUFFER_BIT);

	if (g_ConfigManager.UseCircleTrans && Texture != 0)
	{
		glEnable(GL_STENCIL_TEST);

		glColorMask(false, false, false, true);

		glStencilFunc(GL_ALWAYS, 1, 1);
		glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

		g_GL_Draw(*this, m_X, m_Y);

		glColorMask(true, true, true, true);

		glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
		glStencilFunc(GL_NOTEQUAL, 1, 1);

		glDisable(GL_STENCIL_TEST);
	}
}
//----------------------------------------------------------------------------------