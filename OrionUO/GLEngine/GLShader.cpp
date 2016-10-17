/***********************************************************************************
**
** GLShader.cpp
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#include "GLShader.h"
//----------------------------------------------------------------------------------
CGLShader *g_CurrentShader = NULL;
CDeathShader *g_DeathShader = NULL;
CColorizerShader *g_ColorizerShader = NULL;
CColorizerShader *g_FontColorizerShader = NULL;
CColorizerShader *g_LightColorizerShader = NULL;
//----------------------------------------------------------------------------------
void UnuseShader()
{
	glUseProgramObjectARB(0);
	ShaderColorTable = 0;
	g_ShaderDrawMode = 0;
	g_CurrentShader = NULL;
}
//----------------------------------------------------------------------------------
//-----------------------------------CGLShader--------------------------------------
//----------------------------------------------------------------------------------
CGLShader::CGLShader(const char *vertexShaderData, const char *fragmentShaderData)
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
//----------------------------------------------------------------------------------
CGLShader::~CGLShader()
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

	m_TexturePointer = 0;
}
//----------------------------------------------------------------------------------
bool CGLShader::Use()
{
	CGLShader *oldShader = g_CurrentShader;
	UnuseShader();
	g_CurrentShader = oldShader;

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
	glUseProgramObjectARB(0);
}
//----------------------------------------------------------------------------------
void CGLShader::Resume()
{
	glUseProgramObjectARB(m_Shader);
}
//----------------------------------------------------------------------------------
//-----------------------------------CDeathShader-----------------------------------
//----------------------------------------------------------------------------------
CDeathShader::CDeathShader(const char *vertexShaderData, const char *fragmentShaderData)
: CGLShader(vertexShaderData, fragmentShaderData)
{
	if (m_Shader != 0)
		m_TexturePointer = glGetUniformLocationARB(m_Shader, "usedTexture");
	else
		LOG("Failed to create DeathShader\n");
}
//----------------------------------------------------------------------------------
//----------------------------------CColorizerShader--------------------------------
//----------------------------------------------------------------------------------
CColorizerShader::CColorizerShader(const char *vertexShaderData, const char *fragmentShaderData)
: CGLShader(vertexShaderData, fragmentShaderData), m_ColorTablePointer(0),
m_DrawModePointer(0)
{
	if (m_Shader != 0)
	{
		m_TexturePointer = glGetUniformLocationARB(m_Shader, "usedTexture");
		m_ColorTablePointer = glGetUniformLocationARB(m_Shader, "colors");
		m_DrawModePointer = glGetUniformLocationARB(m_Shader, "drawMode");
	}
	else
		LOG("Failed to create ColorizerShader\n");
}
//----------------------------------------------------------------------------------
bool CColorizerShader::Use()
{
	bool result = CGLShader::Use();

	if (result)
	{
		ShaderColorTable = m_ColorTablePointer;
		g_ShaderDrawMode = m_DrawModePointer;
		glUniform1iARB(g_ShaderDrawMode, 0);
	}

	return result;
}
//----------------------------------------------------------------------------------
