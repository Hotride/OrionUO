// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
/***********************************************************************************
**
** GLFrameBuffer.cpp
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#include "stdafx.h"
//----------------------------------------------------------------------------------
CGLFrameBuffer::CGLFrameBuffer()
{
	WISPFUN_DEBUG("c30_f1");
}
//----------------------------------------------------------------------------------
CGLFrameBuffer::~CGLFrameBuffer()
{
	WISPFUN_DEBUG("c30_f2");
	Free();
}
//----------------------------------------------------------------------------------
/*!
Инициализациия буфера
@param [__in] width Ширина буфера
@param [__in] height Высота буфера
@return true в случае успеха
*/
bool CGLFrameBuffer::Init(const int &width, const int &height)
{
	WISPFUN_DEBUG("c30_f3");
	Free();

	bool result = false;

	if (g_GL.CanUseFrameBuffer && width && height)
	{
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glGenTextures(1, &Texture.Texture);
		glBindTexture(GL_TEXTURE_2D, Texture.Texture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_BGRA, GL_UNSIGNED_INT_8_8_8_8, NULL);

		GLint currentFrameBuffer = 0;
		glGetIntegerv(GL_FRAMEBUFFER_BINDING, &currentFrameBuffer);

		glGenFramebuffers(1, &m_FrameBuffer);
		glBindFramebuffer(GL_FRAMEBUFFER, m_FrameBuffer);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, Texture.Texture, 0);

		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE)
		{
			Texture.Width = width;
			Texture.Height = height;

			result = true;
			m_Ready = true;
		}

		glBindFramebuffer(GL_FRAMEBUFFER, currentFrameBuffer);
	}

	return result;
}
//----------------------------------------------------------------------------------
/*!
Очистка фрэймбуфера
@return
*/
void CGLFrameBuffer::Free()
{
	WISPFUN_DEBUG("c30_f4");
	Texture.Clear();

	if (g_GL.CanUseFrameBuffer && m_FrameBuffer != 0)
	{
		glDeleteFramebuffers(1, &m_FrameBuffer);
		m_FrameBuffer = 0;
	}

	m_OldFrameBuffer = 0;
}
//----------------------------------------------------------------------------------
/*!
Завершение использования фрэймбуфера
@return 
*/
void CGLFrameBuffer::Release()
{
	WISPFUN_DEBUG("c30_f5");
	if (g_GL.CanUseFrameBuffer)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_OldFrameBuffer);
	
		glBindTexture(GL_TEXTURE_2D, Texture.Texture);
		glGenerateMipmap(GL_TEXTURE_2D);

		g_GL.RestorePort();
	}
}
//----------------------------------------------------------------------------------
/*!
Проверка готовности буфера с потенциальным пересозданием
@param [__in] width Ширина буфера
@param [__in] height Высота буфера
@return true в случае готовности
*/
bool CGLFrameBuffer::Ready(const int &width, const int &height)
{
	WISPFUN_DEBUG("c30_f6");
	return (g_GL.CanUseFrameBuffer && m_Ready && Texture.Width == width && Texture.Height == height);
}
//----------------------------------------------------------------------------------
bool CGLFrameBuffer::ReadyMinSize(const int &width, const int &height)
{
	WISPFUN_DEBUG("c30_f6");
	return (g_GL.CanUseFrameBuffer && m_Ready && Texture.Width >= width && Texture.Height >= height);
}
//----------------------------------------------------------------------------------
/*!
Использование буфера
@return true в случае успеха
*/
bool CGLFrameBuffer::Use()
{
	WISPFUN_DEBUG("c30_f7");
	bool result = false;

	if (g_GL.CanUseFrameBuffer && m_Ready)
	{
		glEnable(GL_TEXTURE_2D);
		
		glGetIntegerv(GL_FRAMEBUFFER_BINDING, &m_OldFrameBuffer);
		glBindFramebuffer(GL_FRAMEBUFFER, m_FrameBuffer);
		glBindTexture(GL_TEXTURE_2D, Texture.Texture);

		glViewport(0, 0, Texture.Width, Texture.Height);

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		
		glOrtho(0.0, Texture.Width, 0.0, Texture.Height, -150.0, 150.0);

		glMatrixMode(GL_MODELVIEW);
		
		result = true;
	}

	return result;
}
//----------------------------------------------------------------------------------
/*!
Отрисовать текстуру буфера
@param [__in] x Экранная координата X
@param [__in] y Экранная координата Y
@return 
*/
void CGLFrameBuffer::Draw(const int &x, const int &y)
{
	WISPFUN_DEBUG("c30_f8");
	if (g_GL.CanUseFrameBuffer && m_Ready)
	{
		g_GL.OldTexture = 0;
		g_GL.GL1_Draw(Texture, x, y);
	}
}
//----------------------------------------------------------------------------------

