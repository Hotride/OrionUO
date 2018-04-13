// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
/***********************************************************************************
**
** GLTextureCircleOfTransparency.cpp
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#include "stdafx.h"
//----------------------------------------------------------------------------------
CGLTextureCircleOfTransparency g_CircleOfTransparency;
//----------------------------------------------------------------------------------
CGLTextureCircleOfTransparency::CGLTextureCircleOfTransparency()
: CGLTexture()
{
}
//----------------------------------------------------------------------------------
CGLTextureCircleOfTransparency::~CGLTextureCircleOfTransparency()
{
	WISPFUN_DEBUG("c37_f1");
	Clear();
}
//---------------------------------------------------------------------------
void CGLTextureCircleOfTransparency::CreatePixels(const int &radius, short &width, short &height, UINT_LIST &pixels)
{
	WISPFUN_DEBUG("c37_f2");
	int fixRadius = radius + 1;
	int mulRadius = fixRadius * 2;

	pixels.resize(mulRadius * mulRadius);

	width = mulRadius;
	height = mulRadius;

	IFOR(x, -fixRadius, fixRadius)
	{
		intptr_t mulX = x * x;
		int posX = (((int)x + fixRadius) * mulRadius) + fixRadius;

		IFOR(y, -fixRadius, fixRadius)
		{
			int r = (int)sqrt(mulX + (y * y));
			uchar pic = ((r <= radius) ? ((radius - r) & 0xFF) : 0);

			int pos = posX + (int)y;

			pixels[pos] = pic;
		}
	}
}
//---------------------------------------------------------------------------
bool CGLTextureCircleOfTransparency::Create(int radius)
{
	WISPFUN_DEBUG("c37_f3");
	if (radius <= 0)
		return false;
	else if (radius > 200)
		radius = 200;

	if (radius == Radius)
		return true;

	UINT_LIST pixels;

	CreatePixels(radius, Width, Height, pixels);

	Radius = radius;

	if (Texture != NULL)
	{
		glDeleteTextures(1, &Texture);
		Texture = 0;
	}

	g_GL_BindTexture32(*this, Width, Height, &pixels[0]);

	return true;
}
//---------------------------------------------------------------------------
void CGLTextureCircleOfTransparency::Draw(const int &x, const int &y, const bool &checktrans)
{
	WISPFUN_DEBUG("c37_f4");
	if (Texture != 0)
	{
		X = x - Width / 2;
		Y = y - Height / 2;

		glEnable(GL_STENCIL_TEST);

		glColorMask(false, false, false, true);

		glStencilFunc(GL_ALWAYS, 1, 1);
		glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

		g_GL_Draw(*this, X, Y);

		glColorMask(true, true, true, true);

		glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
		glStencilFunc(GL_NOTEQUAL, 1, 1);

		glDisable(GL_STENCIL_TEST);
	}
}
//---------------------------------------------------------------------------
void CGLTextureCircleOfTransparency::Redraw()
{
	WISPFUN_DEBUG("c37_f5");
	glClear(GL_STENCIL_BUFFER_BIT);

	if (g_ConfigManager.UseCircleTrans && Texture != 0)
	{
		glEnable(GL_STENCIL_TEST);

		glColorMask(false, false, false, true);

		glStencilFunc(GL_ALWAYS, 1, 1);
		glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

		g_GL_Draw(*this, X, Y);

		glColorMask(true, true, true, true);

		glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
		glStencilFunc(GL_NOTEQUAL, 1, 1);

		glDisable(GL_STENCIL_TEST);
	}
}
//----------------------------------------------------------------------------------