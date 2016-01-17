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
//---------------------------------------------------------------------------
TFrameBuffer::TFrameBuffer()
: m_Width(0), m_Height(0), m_Ready(false), m_OldFrameBuffer(0), m_FrameBuffer(0),
m_Texture(0)
{
}
//---------------------------------------------------------------------------
TFrameBuffer::~TFrameBuffer()
{
}
//---------------------------------------------------------------------------
bool TFrameBuffer::Init(int width, int height)
{
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

		//GLenum DrawBuffers[1] = { GL_COLOR_ATTACHMENT0 };
		//glDrawBuffers(1, DrawBuffers);
	
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
void TFrameBuffer::Free()
{
	if (m_Texture != 0)
	{
		glDeleteTextures(1, &m_Texture);
		m_Texture = 0;
	}

	if (m_FrameBuffer != 0)
	{
		glDeleteBuffers(1, &m_FrameBuffer);
		m_FrameBuffer = 0;
	}

	m_OldFrameBuffer = 0;
}
//---------------------------------------------------------------------------
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
bool TFrameBuffer::Ready(int &width, int &height)
{
	return (g_UseFrameBuffer && m_Ready && m_Width == width && m_Height == height);
}
//---------------------------------------------------------------------------
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
		glPushMatrix();
		glLoadIdentity();
		
		glOrtho(0.0, m_Width, 0.0, m_Height, -100.0, 100.0);

		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glLoadIdentity();
		
		result = true;
	}

	return result;
}
//---------------------------------------------------------------------------
void TFrameBuffer::Draw(float x, float y)
{
	if (g_UseFrameBuffer && m_Ready)
	{
		glBindTexture(GL_TEXTURE_2D, m_Texture);

		glLoadIdentity();
		glTranslatef(x, y, 0.0f);
		
		float width = (float)m_Width;
		float height = (float)m_Height;

		glBegin(GL_QUADS);
			glTexCoord2f(0.0f, 1.0f); glVertex2f(0.0f, height);
			glTexCoord2f(1.0f, 1.0f); glVertex2f(width, height);
			glTexCoord2f(1.0f, 0.0f); glVertex2f(width, 0.0f);
			glTexCoord2f(0.0f, 0.0f); glVertex2f(0.0f, 0.0f);
		glEnd();
	}
}
//---------------------------------------------------------------------------
void TFrameBuffer::DrawShadow(float x, float y, float width, float height, bool mirror)
{
	if (g_UseFrameBuffer && m_Ready)
	{
		glBindTexture(GL_TEXTURE_2D, m_Texture);
		
		glLoadIdentity();
		glTranslatef(x, y, 0.0f);
		
		width = (float)m_Width;
		height = (float)m_Height;
		
		glBegin(GL_QUADS);
			glTexCoord2f(0.0f, 1.0f); glVertex2f(0.0f, height);
			glTexCoord2f(1.0f, 1.0f); glVertex2f(width, height);
			glTexCoord2f(1.0f, 0.0f); glVertex2f(width, 0.0f);
			glTexCoord2f(0.0f, 0.0f); glVertex2f(0.0f, 0.0f);
		glEnd();
	}
}
//---------------------------------------------------------------------------