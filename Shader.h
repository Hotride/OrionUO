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
class TShader
{
protected:
	GLhandleARB m_Shader;
	GLhandleARB m_VertexShader;
	GLhandleARB m_FragmentShader;
	GLuint m_TexturePointer;
public:
	TShader(const char *vertexShaderData, const char *fragmentShaderData);
	virtual ~TShader();

	bool Use();
};
//---------------------------------------------------------------------------
class TDeathShader : public TShader
{
public:
	TDeathShader(const char *vertexShaderData, const char *fragmentShaderData);
};
//---------------------------------------------------------------------------
extern TShader *Shader;
extern TDeathShader *DeathShader;
//---------------------------------------------------------------------------
#endif