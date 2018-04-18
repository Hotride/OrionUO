// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
/***********************************************************************************
**
** GUIBlending.cpp
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#include "stdafx.h"
//----------------------------------------------------------------------------------
CGUIBlending::CGUIBlending(bool enabled, GLenum sFactor, GLenum dFactor)
: CBaseGUI(GOT_BLENDING, 0, 0, 0, 0, 0), SFactor(sFactor), DFactor(dFactor)
{
	Enabled = enabled;
}
//----------------------------------------------------------------------------------
CGUIBlending::~CGUIBlending()
{
}
//----------------------------------------------------------------------------------
void CGUIBlending::Draw(bool checktrans)
{
	WISPFUN_DEBUG("c41_f1");
	if (Enabled)
	{
		glEnable(GL_BLEND);
		glBlendFunc(SFactor, DFactor);
	}
	else
		glDisable(GL_BLEND);
}
//----------------------------------------------------------------------------------
