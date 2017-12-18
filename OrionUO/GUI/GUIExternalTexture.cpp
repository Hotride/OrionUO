// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
/***********************************************************************************
**
** GUIExternalTexture.cpp
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#include "stdafx.h"
//----------------------------------------------------------------------------------
CGUIExternalTexture::CGUIExternalTexture(CGLTexture *texture, const bool &deleteTextureOnDestroy, const int &x, const int &y, const int &drawWidth, const int &drawHeight)
: CBaseGUI(GOT_EXTERNALTEXTURE, 0, 0, 0, x, y), m_Texture(texture),
m_DeleteTextureOnDestroy(deleteTextureOnDestroy), m_DrawWidth(drawWidth), m_DrawHeight(drawHeight)
{
}
//----------------------------------------------------------------------------------
CGUIExternalTexture::~CGUIExternalTexture()
{
	WISPFUN_DEBUG("c53_f1");
	if (m_DeleteTextureOnDestroy)
	{
		RELEASE_POINTER(m_Texture);
	}
}
//----------------------------------------------------------------------------------
WISP_GEOMETRY::CSize CGUIExternalTexture::GetSize()
{
	WISPFUN_DEBUG("c53_f2");
	WISP_GEOMETRY::CSize size;

	if (m_Texture != NULL)
	{
		if (m_DrawWidth)
			size.Width = m_DrawWidth;
		else
			size.Width = m_Texture->Width;

		if (m_DrawHeight)
			size.Height = m_DrawHeight;
		else
			size.Height = m_Texture->Height;
	}

	return size;
}
//----------------------------------------------------------------------------------
void CGUIExternalTexture::SetShaderMode()
{
	WISPFUN_DEBUG("c53_f2_1");

	if (m_Color != 0)
	{
		if (m_PartialHue)
			glUniform1iARB(g_ShaderDrawMode, SDM_PARTIAL_HUE);
		else
			glUniform1iARB(g_ShaderDrawMode, SDM_COLORED);

		g_ColorManager.SendColorsToShader(m_Color);
	}
	else
		glUniform1iARB(g_ShaderDrawMode, SDM_NO_COLOR);
}
//----------------------------------------------------------------------------------
void CGUIExternalTexture::Draw(const bool &checktrans)
{
	WISPFUN_DEBUG("c53_f3");
	if (m_Texture != NULL)
	{
		SetShaderMode();

		if (m_DrawWidth || m_DrawHeight)
		{
			CGLTexture tex;
			tex.Texture = m_Texture->Texture;

			if (m_DrawWidth)
				tex.Width = m_DrawWidth;
			else
				tex.Width = m_Texture->Width;

			if (m_DrawHeight)
				tex.Height = m_DrawHeight;
			else
				tex.Height = m_Texture->Height;

			g_GL.GL1_Draw(tex, m_X, m_Y);

			tex.Texture = 0;
		}
		else
			m_Texture->Draw(m_X, m_Y, checktrans);
	}
}
//----------------------------------------------------------------------------------
bool CGUIExternalTexture::Select()
{
	WISPFUN_DEBUG("c53_f4");
	if (m_Texture != NULL)
		return m_Texture->Select(m_X, m_Y, !m_CheckPolygone);

	return false;
}
//----------------------------------------------------------------------------------
