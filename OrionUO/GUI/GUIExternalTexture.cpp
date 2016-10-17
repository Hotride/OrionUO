/***********************************************************************************
**
** GUIExternalTexture.cpp
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#include "GUIExternalTexture.h"
#include "../OrionUO.h"
#include "../Managers/ColorManager.h"
#include "../Managers/MouseManager.h"
#include "../Managers/UOFileReader.h"
//----------------------------------------------------------------------------------
CGUIExternalTexture::CGUIExternalTexture(CGLTexture *texture, const bool &deleteTextureOnDestroy, const int &x, const int &y)
: CBaseGUI(GOT_EXTERNALTEXTURE, 0, 0, 0, x, y), m_Texture(texture),
m_DeleteTextureOnDestroy(deleteTextureOnDestroy)
{
}
//----------------------------------------------------------------------------------
CGUIExternalTexture::~CGUIExternalTexture()
{
	if (m_DeleteTextureOnDestroy)
	{
		RELEASE_POINTER(m_Texture);
	}
}
//----------------------------------------------------------------------------------
WISP_GEOMETRY::CSize CGUIExternalTexture::GetSize()
{
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
	if (m_Texture != NULL)
	{
		SetShaderMode();

		m_Texture->Draw(m_X, m_Y, checktrans);
	}
}
//----------------------------------------------------------------------------------
bool CGUIExternalTexture::Select()
{
	bool select = false;

	if (m_Texture != NULL)
	{
		int x = g_MouseManager.Position.X - m_X;
		int y = g_MouseManager.Position.Y - m_Y;

		if (x >= 0 && y >= 0 && x < m_Texture->Width && y < m_Texture->Height)
		{
#if UO_ENABLE_TEXTURE_DATA_SAVING == 1
			select = (m_CheckPolygone || th->PixelsData[(y * th->Width) + x] != 0);
#else
			select = m_CheckPolygone;
#endif
		}
	}

	return select;
}
//----------------------------------------------------------------------------------
