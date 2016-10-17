/***********************************************************************************
**
** GLShader.h
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#ifndef GLSHADER_H
#define GLSHADER_H
//----------------------------------------------------------------------------------
#include "GLEngine.h"
//----------------------------------------------------------------------------------
void UnuseShader();
//----------------------------------------------------------------------------------
//Базовый класс для работы с шейдерами
class CGLShader
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
	CGLShader(const char *vertexShaderData, const char *fragmentShaderData);
	virtual ~CGLShader();

	//Использовать шейдер
	virtual bool Use();

	//Остановить использование шейдера
	virtual void Pause();

	//Восстановить работу шейдера
	virtual void Resume();
};
//----------------------------------------------------------------------------------
//Класс для работы с шейдером смерти (черно-белый шейдер)
class CDeathShader : public CGLShader
{
public:
	CDeathShader(const char *vertexShaderData, const char *fragmentShaderData);
};
//----------------------------------------------------------------------------------
//Класс для работы с цветным шейдером
class CColorizerShader : public CGLShader
{
protected:
	//Указатель на палитру цветов для передачи ее шейдеру
	GLuint m_ColorTablePointer;

	//Указатель на метод вывода шейдером
	GLuint m_DrawModePointer;

public:
	CColorizerShader(const char *vertexShaderData, const char *fragmentShaderData);

	//Использовать шейдер
	virtual bool Use();
};
//----------------------------------------------------------------------------------
extern CGLShader *g_CurrentShader;
extern CDeathShader *g_DeathShader;
extern CColorizerShader *g_ColorizerShader;
extern CColorizerShader *g_FontColorizerShader;
extern CColorizerShader *g_LightColorizerShader;
//----------------------------------------------------------------------------------
#endif
//----------------------------------------------------------------------------------
