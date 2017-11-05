// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
/***********************************************************************************
**
** GLShader.cpp
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#include "stdafx.h"
//----------------------------------------------------------------------------------
CDeathShader g_DeathShader;
CColorizerShader g_ColorizerShader;
CColorizerShader g_FontColorizerShader;
CColorizerShader g_LightColorizerShader;
//----------------------------------------------------------------------------------
void UnuseShader()
{
	WISPFUN_DEBUG("c_uns_sdr");
	glUseProgramObjectARB(0);
	ShaderColorTable = 0;
	g_ShaderDrawMode = 0;
}
//----------------------------------------------------------------------------------
//-----------------------------------CGLShader--------------------------------------
//----------------------------------------------------------------------------------
CGLShader::CGLShader()
{
	WISPFUN_DEBUG("c32_f1");
}
//----------------------------------------------------------------------------------
bool CGLShader::Init(const char *vertexShaderData, const char *fragmentShaderData)
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

	return (m_Shader != 0);
}
//----------------------------------------------------------------------------------
CGLShader::~CGLShader()
{
	WISPFUN_DEBUG("c32_f2");
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

	m_TexturePointer = 0;
}
//----------------------------------------------------------------------------------
bool CGLShader::Use()
{
	WISPFUN_DEBUG("c32_f3");
	UnuseShader();

	bool result = false;

	if (m_Shader != 0)
	{
		glUseProgramObjectARB(m_Shader);
		result = true;
	}

	return result;
}
//----------------------------------------------------------------------------------
void CGLShader::Pause()
{
	WISPFUN_DEBUG("c32_f4");
	glUseProgramObjectARB(0);
}
//----------------------------------------------------------------------------------
void CGLShader::Resume()
{
	WISPFUN_DEBUG("c32_f5");
	glUseProgramObjectARB(m_Shader);
}
//----------------------------------------------------------------------------------
//-----------------------------------CDeathShader-----------------------------------
//----------------------------------------------------------------------------------
CDeathShader::CDeathShader()
: CGLShader()
{
	WISPFUN_DEBUG("c33_f1");
}
//----------------------------------------------------------------------------------
bool CDeathShader::Init(const char *vertexShaderData, const char *fragmentShaderData)
{
	if (CGLShader::Init(vertexShaderData, fragmentShaderData))
		m_TexturePointer = glGetUniformLocationARB(m_Shader, "usedTexture");
	else
		LOG("Failed to create DeathShader\n");

	return (m_Shader != 0);
}
//----------------------------------------------------------------------------------
//----------------------------------CColorizerShader--------------------------------
//----------------------------------------------------------------------------------
CColorizerShader::CColorizerShader()
: CGLShader()
{
	WISPFUN_DEBUG("c34_f1");
}
//----------------------------------------------------------------------------------
bool CColorizerShader::Init(const char *vertexShaderData, const char *fragmentShaderData)
{
	if (CGLShader::Init(vertexShaderData, fragmentShaderData))
	{
		m_TexturePointer = glGetUniformLocationARB(m_Shader, "usedTexture");
		m_ColorTablePointer = glGetUniformLocationARB(m_Shader, "colors");
		m_DrawModePointer = glGetUniformLocationARB(m_Shader, "drawMode");
	}
	else
		LOG("Failed to create ColorizerShader\n");

	return (m_Shader != 0);
}
//----------------------------------------------------------------------------------
bool CColorizerShader::Use()
{
	WISPFUN_DEBUG("c34_f2");
	bool result = CGLShader::Use();

	if (result)
	{
		ShaderColorTable = m_ColorTablePointer;
		g_ShaderDrawMode = m_DrawModePointer;
		glUniform1iARB(g_ShaderDrawMode, SDM_NO_COLOR);
	}

	return result;
}
//----------------------------------------------------------------------------------
