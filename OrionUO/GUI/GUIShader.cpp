// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
/***********************************************************************************
**
** GUIShader.cpp
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#include "stdafx.h"
//----------------------------------------------------------------------------------
CGUIShader::CGUIShader(CGLShader *shader, const bool &enabled)
: CBaseGUI(GOT_SHADER, 0, 0, 0, 0, 0), m_Shader(shader)
{
	Enabled = enabled;
}
//----------------------------------------------------------------------------------
CGUIShader::~CGUIShader()
{
}
//----------------------------------------------------------------------------------
void CGUIShader::Draw(const bool &checktrans)
{
	WISPFUN_DEBUG("c72_f1");
	if (Enabled && m_Shader != NULL)
		m_Shader->Use();
	else
		UnuseShader();
}
//----------------------------------------------------------------------------------
