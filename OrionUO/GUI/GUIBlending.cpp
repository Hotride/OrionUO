/***********************************************************************************
**
** GUIBlending.cpp
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#include "GUIBlending.h"
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
	if (m_Enabled)
	{
		glEnable(GL_BLEND);
		glBlendFunc(m_SFactor, m_DFactor);
	}
	else
		glDisable(GL_BLEND);
}
//----------------------------------------------------------------------------------
