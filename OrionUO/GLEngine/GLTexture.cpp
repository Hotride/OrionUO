/***********************************************************************************
**
** GLTexture.cpp
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#include "GLTexture.h"
#include "GLEngine.h"
//----------------------------------------------------------------------------------
CGLTexture::CGLTexture()
: m_Width(0), m_Height(0), Texture(0), m_VertexBuffer(0), m_MirroredVertexBuffer(0)
{
}
//----------------------------------------------------------------------------------
CGLTexture::~CGLTexture()
{
	Clear();
}
//----------------------------------------------------------------------------------
void CGLTexture::Draw(const int &x, const int &y, const bool &checktrans)
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
void CGLTexture::Draw(const int &x, const int &y, int width, int height, const bool &checktrans)
{
	if (Texture != 0)
	{
		if (!width)
			width = m_Width;

		if (!height)
			height = m_Height;

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
void CGLTexture::DrawRotated(const int &x, const int &y, const float &angle)
{
	if (Texture != 0)
		g_GL_DrawRotated(*this, x, y, angle);
}
//----------------------------------------------------------------------------------
void CGLTexture::DrawTransparent(const int &x, const int &y, const bool &stencil)
{
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
	m_Width = 0;
	m_Height = 0;

	if (Texture != 0)
	{
		glDeleteTextures(1, &Texture);
		Texture = 0;
	}

	if (m_VertexBuffer != 0)
	{
		glDeleteBuffers(1, &m_VertexBuffer);
		m_VertexBuffer = 0;
	}

	if (m_MirroredVertexBuffer != 0)
	{
		glDeleteBuffers(1, &m_MirroredVertexBuffer);
		m_MirroredVertexBuffer = 0;
	}

#if UO_ENABLE_TEXTURE_DATA_SAVING == 1
	PixelsData.clear();
#endif
}
//----------------------------------------------------------------------------------