// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
/***********************************************************************************
**
** GUIHTMLButton.cpp
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#include "stdafx.h"
//----------------------------------------------------------------------------------
CGUIHTMLButton::CGUIHTMLButton(CGUIHTMLGump *htmlGump, const uint &serial, const ushort &graphic, const ushort &graphicSelected, const ushort &graphicPressed, const int &x, const int &y)
: CGUIButton(serial, graphic, graphicSelected, graphicPressed, x, y),
m_HTMLGump(htmlGump)
{
}
//----------------------------------------------------------------------------------
CGUIHTMLButton::~CGUIHTMLButton()
{
}
//----------------------------------------------------------------------------------
void CGUIHTMLButton::SetShaderMode()
{
	WISPFUN_DEBUG("c59_f1");
	glUniform1iARB(g_ShaderDrawMode, SDM_NO_COLOR);
}
//----------------------------------------------------------------------------------
void CGUIHTMLButton::Scroll(const bool &up, const uint &delay)
{
	WISPFUN_DEBUG("c59_f2");
	if (m_HTMLGump != NULL)
		m_HTMLGump->Scroll(up, delay);
}
//----------------------------------------------------------------------------------
