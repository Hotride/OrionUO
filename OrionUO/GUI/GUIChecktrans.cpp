/***********************************************************************************
**
** GUIChecktrans.cpp
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#include "GUIChecktrans.h"
//----------------------------------------------------------------------------------
CGUIChecktrans::CGUIChecktrans(const int &x, const int &y, const int &width, const int &height)
: CGUIPolygonal(GOT_CHECKTRANS, x, y, width, height)
{
}
//----------------------------------------------------------------------------------
CGUIChecktrans::~CGUIChecktrans()
{
}
//----------------------------------------------------------------------------------
void CGUIChecktrans::Draw(const bool &checktrans)
{
	glColorMask(false, false, false, false);

	glStencilFunc(GL_ALWAYS, 1, 1);
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

	g_GL.DrawPolygone(m_X, m_Y, m_Width, m_Height);

	glColorMask(true, true, true, true);

	glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
	glStencilFunc(GL_NOTEQUAL, 1, 1);
}
//----------------------------------------------------------------------------------
