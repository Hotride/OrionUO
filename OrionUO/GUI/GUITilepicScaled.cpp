﻿// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
/***********************************************************************************
**
** GUITilepicScaled.cpp
**
** Copyright (C) December 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#include "stdafx.h"
//----------------------------------------------------------------------------------
CGUITilepicScaled::CGUITilepicScaled(ushort graphic, ushort color, int x, int y, int width, int height)
: CGUITilepic(graphic, color, x, y), Width(width), Height(height)
{
}
//----------------------------------------------------------------------------------
CGUITilepicScaled::~CGUITilepicScaled()
{
}
//----------------------------------------------------------------------------------
void CGUITilepicScaled::Draw(bool checktrans)
{
	WISPFUN_DEBUG("c82_f1");
	CGLTexture *th = g_Orion.ExecuteStaticArt(Graphic);

	if (th != NULL)
	{
		SetShaderMode();

		g_GL_Draw(*th, m_X, m_Y);
	}
}
//----------------------------------------------------------------------------------
