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
//Базовый класс для работы с шейдерами
class TBaseShader
{
protected:
	//Шейдерная программа
	GLhandleARB m_Shader;

	//Вершинный шейдер
	GLhandleARB m_VertexShader;

	//Фрагментный (пиксельный) шейдер
	GLhandleARB m_FragmentShader;

	//Указатель на текстуру для передачи ее шейдеру (можно не использовать, текстура передается автоматически при glBingTexture)
	GLuint m_TexturePointer;
public:
	TBaseShader(const char *vertexShaderData, const char *fragmentShaderData);
	virtual ~TBaseShader();

	//Использовать шейдер
	virtual bool Use();

	//Остановить использование шейдера
	virtual void Pause();

	//Восстановить работу шейдера
	virtual void Resume();
};
//---------------------------------------------------------------------------
//Класс для работы с шейдером смерти (черно-белый шейдер)
class TDeathShader : public TBaseShader
{
public:
	TDeathShader(const char *vertexShaderData, const char *fragmentShaderData);
};
//---------------------------------------------------------------------------
//Класс для работы с цветным шейдером
class TColorizerShader : public TBaseShader
{
protected:
	//Указатель на палитру цветов для передачи ее шейдеру
	GLuint m_ColorTablePointer;

	//Указатель на метод вывода шейдером
	GLuint m_DrawModePointer;
public:
	TColorizerShader(const char *vertexShaderData, const char *fragmentShaderData);

	//Использовать шейдер
	virtual bool Use();
};
//---------------------------------------------------------------------------
extern TBaseShader *CurrentShader;
extern TDeathShader *DeathShader;
extern TColorizerShader *ColorizerShader;
extern TColorizerShader *FontColorizerShader;
extern TColorizerShader *LightColorizerShader;
//---------------------------------------------------------------------------
#endif