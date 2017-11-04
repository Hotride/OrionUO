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
CGUIBlending::CGUIBlending(const bool &enabled, const GLenum &sFactor, const GLenum &dFactor)
: CBaseGUI(GOT_BLENDING, 0, 0, 0, 0, 0), m_SFactor(sFactor), m_DFactor(dFactor)
{
	m_Enabled = enabled;
}
//----------------------------------------------------------------------------------
CGUIBlending::~CGUIBlending()
{
}
//----------------------------------------------------------------------------------
void CGUIBlending::Draw(const bool &checktrans)
{
	WISPFUN_DEBUG("c41_f1");
	if (m_Enabled)
	{
		glEnable(GL_BLEND);
		glBlendFunc(m_SFactor, m_DFactor);
	}
	else
		glDisable(GL_BLEND);
}
//----------------------------------------------------------------------------------
