/****************************************************************************
**
** Shader.h
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
#ifndef ShaderH
#define ShaderH
//---------------------------------------------------------------------------
class TBaseShader
{
protected:
	GLhandleARB m_Shader;
	GLhandleARB m_VertexShader;
	GLhandleARB m_FragmentShader;
	GLuint m_TexturePointer;
public:
	TBaseShader(const char *vertexShaderData, const char *fragmentShaderData);
	virtual ~TBaseShader();

	virtual bool Use();
};
//---------------------------------------------------------------------------
class TDeathShader : public TBaseShader
{
public:
	TDeathShader(const char *vertexShaderData, const char *fragmentShaderData);
};
//---------------------------------------------------------------------------
class TColorizerShader : public TBaseShader
{
protected:
	GLuint m_ColorTablePointer;
public:
	TColorizerShader(const char *vertexShaderData, const char *fragmentShaderData);

	virtual bool Use();
};
//---------------------------------------------------------------------------
extern TBaseShader *CurrentShader;
extern TDeathShader *DeathShader;
extern TColorizerShader *ColorizerShader;
//---------------------------------------------------------------------------
#endif