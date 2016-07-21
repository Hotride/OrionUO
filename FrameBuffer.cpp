/****************************************************************************
**
** FrameBuffer.cpp
**
** Copyright (C) December 2015 Hotride
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

TFrameBuffer g_LightBuffer;
TFrameBuffer g_CharacterBuffer;
//---------------------------------------------------------------------------
TFrameBuffer::TFrameBuffer()
: m_Width(0), m_Height(0), m_Ready(false), m_OldFrameBuffer(0), m_FrameBuffer(0),
m_Texture(0)
{
}
//---------------------------------------------------------------------------
TFrameBuffer::~TFrameBuffer()
{
	Free();
}
//---------------------------------------------------------------------------
/*!
Инициализациия буфера
@param [__in] width Ширина буфера
@param [__in] height Высота буфера
@return true в случае успеха
*/
bool TFrameBuffer::Init( __in int width, __in int height)
{
	Free();

	bool result = false;

	if (g_UseFrameBuffer)
	{
		glGenTextures(1, &m_Texture);
		glBindTexture(GL_TEXTURE_2D, m_Texture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB5_A1, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);

		GLint currentFrameBuffer = 0;
		glGetIntegerv(GL_FRAMEBUFFER_BINDING, &currentFrameBuffer); 

		glGenFramebuffers(1, &m_FrameBuffer);
		glBindFramebuffer(GL_FRAMEBUFFER, m_FrameBuffer);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, m_Texture, 0);

		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE)
		{
			m_Width = width;
			m_Height = height;

			result = true;
			m_Ready = true;
		}

		glBindFramebuffer(GL_FRAMEBUFFER, currentFrameBuffer);
	}

	return result;
}
//---------------------------------------------------------------------------
/*!
Очистка фрэймбуфера
@return
*/
void TFrameBuffer::Free()
{
	if (m_Texture != 0)
	{
		glDeleteTextures(1, &m_Texture);
		m_Texture = 0;
	}

	if (m_FrameBuffer != 0)
	{
		glDeleteFramebuffers(1, &m_FrameBuffer);
		m_FrameBuffer = 0;
	}

	m_OldFrameBuffer = 0;
}
//---------------------------------------------------------------------------
/*!
Завершение использования фрэймбуфера
@return 
*/
void TFrameBuffer::Release()
{
	if (g_UseFrameBuffer)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_OldFrameBuffer);
	
		glBindTexture(GL_TEXTURE_2D, m_Texture);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
}
//---------------------------------------------------------------------------
/*!
Проверка готовности буфера с потенциальным пересозданием
@param [__in] width Ширина буфера
@param [__in] height Высота буфера
@return true в случае готовности
*/
bool TFrameBuffer::Ready( __in int &width, __in int &height)
{
	return (g_UseFrameBuffer && m_Ready && m_Width == width && m_Height == height);
}
//---------------------------------------------------------------------------
/*!
Использование буфера
@return true в случае успеха
*/
bool TFrameBuffer::Use()
{
	bool result = false;

	if (g_UseFrameBuffer && m_Ready)
	{
		glEnable(GL_TEXTURE_2D);
		
		glGetIntegerv(GL_FRAMEBUFFER_BINDING, &m_OldFrameBuffer);
		glBindFramebuffer(GL_FRAMEBUFFER, m_FrameBuffer);
        glBindTexture(GL_TEXTURE_2D, m_Texture);

		glViewport(0, 0, m_Width, m_Height);

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		
		glOrtho(0.0, m_Width, 0.0, m_Height, -150.0, 150.0);

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		
		result = true;
	}

	return result;
}
//---------------------------------------------------------------------------
/*!
Отрисовать текстуру буфера
@param [__in] x Экранная координата X
@param [__in] y Экранная координата Y
@return 
*/
void TFrameBuffer::Draw( __in int x, __in int y)
{
	if (g_UseFrameBuffer && m_Ready)
	{
		g_GL.OldTexture = 0;
		g_GL.Draw(m_Texture, x, y, m_Width, m_Height);
	}
}
//---------------------------------------------------------------------------
/*!
Отрисовать текстуру буфера
@param [__in] x Экранная координата X
@param [__in] y Экранная координата Y
@return
*/
void TFrameBuffer::DrawShadow(__in int x, __in int y, const DRAW_FRAME_INFORMATION &dfInfo)
{
	if (g_UseFrameBuffer && m_Ready)
	{
		g_GL.OldTexture = 0;
		glBindTexture(GL_TEXTURE_2D, m_Texture);

		int offsetY = dfInfo.Height / 2;
		GLfloat translateY = (GLfloat)(y + offsetY);

		float texX = dfInfo.Width / (float)m_Width;
		float texY = dfInfo.Height / (float)m_Height;

		glTranslatef((GLfloat)x, translateY, (GLfloat)g_ZBuffer);

		glBegin(GL_TRIANGLE_STRIP);
			glTexCoord2f(0.0f, texY); glVertex2i(0, dfInfo.Height - offsetY);
			glTexCoord2f(texX, texY); glVertex2i(dfInfo.Width, dfInfo.Height - offsetY);
			glTexCoord2f(0.0f, 0.0f); glVertex2i(dfInfo.Width, 0);
			glTexCoord2f(texX, 0.0f); glVertex2i(dfInfo.Width * 2, 0);
		glEnd();

		glTranslatef((GLfloat)-x, -translateY, (GLfloat)-g_ZBuffer);
	}
}
//---------------------------------------------------------------------------
