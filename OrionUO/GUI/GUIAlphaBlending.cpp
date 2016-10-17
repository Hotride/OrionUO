/***********************************************************************************
**
** GUIAlphaBlending.cpp
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#include "GUIAlphaBlending.h"
//----------------------------------------------------------------------------------
CGUIAlphaBlending::CGUIAlphaBlending(const bool &enabled, const float &alpha)
: CGUIBlending(enabled, GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA), m_Alpha(alpha)
{
}
//----------------------------------------------------------------------------------
CGUIAlphaBlending::~CGUIAlphaBlending()
{
}
//----------------------------------------------------------------------------------
void CGUIAlphaBlending::Draw(const bool &checktrans)
{
	CGUIBlending::Draw(checktrans);

	if (m_Enabled)
		glColor4f(1.0f, 1.0f, 1.0f, m_Alpha);
	else
		glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
}
//----------------------------------------------------------------------------------
