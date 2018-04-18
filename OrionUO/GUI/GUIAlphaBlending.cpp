// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
/***********************************************************************************
**
** GUIAlphaBlending.cpp
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#include "stdafx.h"
//----------------------------------------------------------------------------------
CGUIAlphaBlending::CGUIAlphaBlending(bool enabled, float alpha)
: CGUIBlending(enabled, GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA), Alpha(alpha)
{
}
//----------------------------------------------------------------------------------
CGUIAlphaBlending::~CGUIAlphaBlending()
{
}
//----------------------------------------------------------------------------------
void CGUIAlphaBlending::Draw(bool checktrans)
{
	WISPFUN_DEBUG("c40_f1");
	CGUIBlending::Draw(checktrans);

	if (Enabled)
		glColor4f(1.0f, 1.0f, 1.0f, Alpha);
	else
		glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
}
//----------------------------------------------------------------------------------
