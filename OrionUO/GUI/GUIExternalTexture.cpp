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
CGUIExternalTexture::CGUIExternalTexture(CGLTexture *texture, const bool &deleteTextureOnDestroy, const int &x, const int &y)
: CBaseGUI(GOT_EXTERNALTEXTURE, 0, 0, 0, x, y), m_Texture(texture),
m_DeleteTextureOnDestroy(deleteTextureOnDestroy)
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
		size.Width = m_Texture->Width;
		size.Height = m_Texture->Height;
	}

	return size;
}
//----------------------------------------------------------------------------------
void CGUIExternalTexture::Draw(const bool &checktrans)
{
	WISPFUN_DEBUG("c53_f3");
	if (m_Texture != NULL)
	{
		SetShaderMode();

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
