/***********************************************************************************
**
** GUITilepicScaled.cpp
**
** Copyright (C) December 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#include "GUITilepicScaled.h"
#include "../OrionUO.h"
//----------------------------------------------------------------------------------
CGUITilepicScaled::CGUITilepicScaled(const ushort &graphic, const ushort &color, const int &x, const int &y, const int &width, const int &height)
: CGUITilepic(graphic, color, x, y), m_Width(width), m_Height(height)
{
}
//----------------------------------------------------------------------------------
CGUITilepicScaled::~CGUITilepicScaled()
{
}
//----------------------------------------------------------------------------------
void CGUITilepicScaled::Draw(const bool &checktrans)
{
	CGLTexture *th = g_Orion.ExecuteStaticArt(m_Graphic);

	if (th != NULL)
	{
		SetShaderMode();

		g_GL.Draw(th->Texture, m_X, m_Y, m_Width, m_Height);
	}
}
//----------------------------------------------------------------------------------
