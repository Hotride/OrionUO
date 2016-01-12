/****************************************************************************
**
** Shader.cpp
**
** Copyright (C) January 2016 Hotride
**
** This program is free software; you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation; either version 2 of the License, or
** (at your option) any later version.
**
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**
*****************************************************************************
*/
//---------------------------------------------------------------------------
#include "stdafx.h"

TShader *Shader = NULL;
TDeathShader *DeathShader = NULL;
//---------------------------------------------------------------------------
//--------------------------------TShader------------------------------------
//---------------------------------------------------------------------------
TShader::TShader(const char *vertexShaderData, const char *fragmentShaderData)
: m_Shader(0), m_VertexShader(0), m_FragmentShader(0), m_TexturePointer(0)
{
	if (vertexShaderData != NULL && fragmentShaderData != NULL)
	{
		m_Shader = glCreateProgramObjectARB();

		m_VertexShader = glCreateShaderObjectARB(GL_VERTEX_SHADER_ARB);
		glShaderSourceARB(m_VertexShader, 1, (const GLcharARB**)&vertexShaderData, NULL);
		glCompileShaderARB(m_VertexShader);
		glAttachObjectARB(m_Shader, m_VertexShader);

		m_FragmentShader = glCreateShaderObjectARB(GL_FRAGMENT_SHADER_ARB);
		glShaderSourceARB(m_FragmentShader, 1, (const GLcharARB**)&fragmentShaderData, NULL);
		glCompileShaderARB(m_FragmentShader);
		glAttachObjectARB(m_Shader, m_FragmentShader);

		glLinkProgramARB(m_Shader);
		glValidateProgramARB(m_Shader);
	}
}
//---------------------------------------------------------------------------
TShader::~TShader()
{
	if (m_Shader != 0)
	{
		glDeleteObjectARB(m_Shader);
		m_Shader = 0;
	}

	if (m_VertexShader != 0)
	{
		glDeleteObjectARB(m_VertexShader);
		m_VertexShader = 0;
	}

	if (m_FragmentShader != 0)
	{
		glDeleteObjectARB(m_FragmentShader);
		m_FragmentShader = 0;
	}
}
//---------------------------------------------------------------------------
bool TShader::Use()
{
	bool result = false;

	if (m_Shader != 0)
	{
		glUseProgramObjectARB(m_Shader);
		ShaderTexture = m_TexturePointer;
		result = true;
	}

	return result;
}
//---------------------------------------------------------------------------
//-----------------------------TDeathShader----------------------------------
//---------------------------------------------------------------------------
TDeathShader::TDeathShader(const char *vertexShaderData, const char *fragmentShaderData)
: TShader(vertexShaderData, fragmentShaderData)
{
	if (m_Shader != 0)
		m_TexturePointer = glGetUniformLocationARB(m_Shader, "usedTexture");
}
//---------------------------------------------------------------------------