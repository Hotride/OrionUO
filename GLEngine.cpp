/****************************************************************************
**
** GLEngine.cpp
**
** Copyright (C) September 2015 Hotride
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

TGLEngine g_GL;
//---------------------------------------------------------------------------
bool TGLEngine::GLSetupPixelFormat()
{
	PIXELFORMATDESCRIPTOR pfd =
	{
		sizeof(PIXELFORMATDESCRIPTOR), //nSize
		1, //nVersion
		/*PFD_DRAW_TO_WINDOW |*/ PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER, //dwFlags
		PFD_TYPE_RGBA, //iPixelType
		16, //cColorBits
		0, 0, //cRedBits, cRedShift
		0, 0, //cGreenBits, cGreenShift
		0, 0, //cBlueBits, cBlueShift
		0, 0, //cAlphaBits, cAlphaShift
		0, 0, 0, 0, 0, //cAccumBits, cAccumRedBits, cAccumGreenBits, cAccumBlueBits, cAccumAlphaBits
		16, //cDepthBits
		1, //cStencilBits
		0, //cAuxBuffers
		PFD_MAIN_PLANE, //iLayerType
		0, //bReserved
		0, //dwLayerMask
		0, //dwVisibleMask
		0 //dwDamageMask
	};

	int pixelformat = ChoosePixelFormat(m_DC, &pfd);

	if (!pixelformat)
	{
		MessageBox(NULL, L"ChoosePixelFormat failed", L"Error", MB_OK);
		return false;
	}

	if (!SetPixelFormat(m_DC, pixelformat, &pfd))
	{
		MessageBox(NULL, L"SetPixelFormat failed", L"Error", MB_OK);
		return false;
	}

	return true;
}
//---------------------------------------------------------------------------
bool TGLEngine::Install(HWND hWnd)
{
	m_OldTexture = -1;

	m_DC = ::GetDC(hWnd);
	if (!GLSetupPixelFormat())
		return false;

	m_RC = wglCreateContext(m_DC);
	if (!m_RC)
		return false;

	if (!wglMakeCurrent(m_DC, m_RC))
		return false;
	
	int glewInitResult = glewInit();
	TPRINT("glewInit() = %i rt=%i fb=%i v(%s)\n", glewInitResult, WGLEW_ARB_render_texture, GL_ARB_framebuffer_object, glGetString(GL_VERSION));

	if (!glewInitResult)
	{
		g_UseFrameBuffer = (GL_ARB_framebuffer_object &&
			glBindFramebuffer &&
			glDeleteFramebuffers &&
			glFramebufferTexture2D &&
			glGenFramebuffers
			);
	}
	else
		return false;

	wglSwapIntervalEXT(FALSE);

	TPRINT("g_UseFrameBuffer = %i\n", g_UseFrameBuffer);

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);		// Black Background
	glShadeModel(GL_SMOOTH);				// Enables Smooth Color Shading
	glClearDepth(1.0);						// Depth Buffer Setup
	glDisable(GL_DITHER);

	//glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);   //Realy Nice perspective calculations
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_FASTEST);

	glEnable(GL_TEXTURE_2D);

	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

	glClearStencil(0);
	glStencilMask(1);
	
	glEnable(GL_LIGHT0);

	GLfloat lightPosition[]= { -1.0f, -1.0f, 0.5f, 0.0f };
	glLightfv(GL_LIGHT0, GL_POSITION, &lightPosition[0]);
	
	GLfloat lightAmbient[]= { 2.0f, 2.0f, 2.0f, 1.0f };
	glLightfv(GL_LIGHT0, GL_AMBIENT, &lightAmbient[0]);
	
	GLfloat lav = 0.8f;
	GLfloat lightAmbientValues[]= { lav, lav, lav, lav };
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, &lightAmbientValues[0]);

	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_FALSE);

	ViewPort(0, 0, g_ClientWidth, g_ClientHeight);
	
	return true;
}
//---------------------------------------------------------------------------
void TGLEngine::UpdateRect()
{
	RECT cr;
	GetClientRect(g_hWnd, &cr);
	g_ClientWidth = cr.right - cr.left;
	g_ClientHeight = cr.bottom - cr.top;

	ViewPort(0, 0, g_ClientWidth, g_ClientHeight);

	if (GumpManager != NULL)
		GumpManager->RedrawAll();
}
//---------------------------------------------------------------------------
void TGLEngine::BindTexture16(GLuint &texture, const int &width, const int &height, PWORD pixels)
{
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB5_A1, width, height, 0, GL_BGRA, GL_UNSIGNED_SHORT_1_5_5_5_REV, pixels);
}
//---------------------------------------------------------------------------
void TGLEngine::BindTexture32(GLuint &texture, const int &width, const int &height, PDWORD pixels)
{
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA4, width, height, 0, GL_BGRA, GL_UNSIGNED_INT_8_8_8_8, pixels);
}
//---------------------------------------------------------------------------
void TGLEngine::BeginDraw()
{
	g_DrawMode = true;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	glLoadIdentity();
	
	glDisable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glDisable(GL_STENCIL_TEST);
	glDisable(GL_BLEND);
	g_ZBuffer = 0;

	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER, 0.0f);
}
//---------------------------------------------------------------------------
void TGLEngine::EndDraw()
{
	g_DrawMode = false;

	glDisable(GL_ALPHA_TEST);

	SwapBuffers(m_DC);
}
//---------------------------------------------------------------------------
void TGLEngine::BeginStencil()
{
	glEnable(GL_STENCIL_TEST);

	glColorMask(false, false, false, false);

	glStencilFunc(GL_ALWAYS, 1, 1);
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
}
//---------------------------------------------------------------------------
void TGLEngine::EndStencil()
{
	glColorMask(true, true, true, true);
					
	glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
	glStencilFunc(GL_NOTEQUAL, 1, 1);

	glDisable(GL_STENCIL_TEST);
}
//---------------------------------------------------------------------------
void TGLEngine::ViewPort(const int &x, const int &y, const int &width, const int &height)
{
	glViewport(x, g_ClientHeight - y - height, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(x, width + x, height + y, y, -150.0, 150.0);
	glMatrixMode(GL_MODELVIEW);
}
//---------------------------------------------------------------------------
void TGLEngine::RestorePort()
{
	glViewport(0, 0, g_ClientWidth, g_ClientHeight);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.0, g_ClientWidth, g_ClientHeight, 0.0, -150.0, 150.0);
	glMatrixMode(GL_MODELVIEW);
}
//---------------------------------------------------------------------------
void TGLEngine::Scissor(const int &x, const int &y, const int &width, const int &height)
{
	glEnable(GL_SCISSOR_TEST);
	glScissor(x, g_ClientHeight - y - height, width, height);
}
//---------------------------------------------------------------------------
void TGLEngine::DrawLine(const int &x, const int &y, const int &targetX, const int &targetY)
{
	glDisable(GL_TEXTURE_2D);

	glBegin(GL_LINES);
		glVertex2i(x, y);
		glVertex2i(targetX, targetY);
	glEnd();

	glEnable(GL_TEXTURE_2D);
}
//---------------------------------------------------------------------------
void TGLEngine::DrawPolygone(const int &x, const int &y, const int &width, const int &height)
{
	glDisable(GL_TEXTURE_2D);

	glTranslatef((GLfloat)x, (GLfloat)y, 0.0f);

	glBegin(GL_TRIANGLE_STRIP);
		glVertex2i(0, height);
		glVertex2i(width, height);
		glVertex2i(0, 0);
		glVertex2i(width, 0);
	glEnd();

	glTranslatef((GLfloat)-x, (GLfloat)-y, 0.0f);

	glEnable(GL_TEXTURE_2D);
}
//---------------------------------------------------------------------------
void TGLEngine::DrawCircle(const float &x, const float &y, const float &radius, const int &gradientMode)
{
	glDisable(GL_TEXTURE_2D);

	glTranslatef(x, y, 0.0f);

	glBegin(GL_TRIANGLE_FAN);

		glVertex2i(0, 0);

		if (gradientMode)
			glColor4f(0.0f, 0.0f, 0.0f, 0.0f);

		float pi = (float)M_PI * 2.0f;

		for (int i = 0; i <= 360; i++)
		{
			float a = (i / 180.0f) * pi;
			glVertex2f(cos(a) * radius, sin(a) * radius);
		}

	glEnd();

	glTranslatef(-x, -y, 0.0f);

	glEnable(GL_TEXTURE_2D);
}
//---------------------------------------------------------------------------
void TGLEngine::DrawLandTexture(GLuint &texture, const int &x, const int &y, RECT &rc, TVector *normals)
{
	if (m_OldTexture != texture)
	{
		m_OldTexture = texture;
		glBindTexture(GL_TEXTURE_2D, texture);
	}

	float translateX = x - 23.0f;
	float translateY = y - 23.0f;

	glTranslatef(translateX, translateY, (GLfloat)g_ZBuffer);

	glBegin(GL_TRIANGLE_STRIP);
		glNormal3f((GLfloat)normals[0].X, (GLfloat)normals[0].Y, (GLfloat)normals[0].Z);
		glTexCoord2i(0, 0); glVertex2i(22, -rc.left); //^
		
		glNormal3f((GLfloat)normals[3].X, (GLfloat)normals[3].Y, (GLfloat)normals[3].Z);
		glTexCoord2i(0, 1); glVertex2i(0, 22 - rc.top); //<

		glNormal3f((GLfloat)normals[1].X, (GLfloat)normals[1].Y, (GLfloat)normals[1].Z);
		glTexCoord2i(1, 0); glVertex2i(44, 22 - rc.bottom); //>
		
		glNormal3f((GLfloat)normals[2].X, (GLfloat)normals[2].Y, (GLfloat)normals[2].Z);
		glTexCoord2i(1, 1); glVertex2i(22, 44 - rc.right); //v
	glEnd();

	glTranslatef(-translateX, -translateY, (GLfloat)-g_ZBuffer);
}
//---------------------------------------------------------------------------
void TGLEngine::Draw(GLuint &texture, const int &x, const int &y, const int &width, const int &height)
{
	if (m_OldTexture != texture)
	{
		m_OldTexture = texture;
		glBindTexture(GL_TEXTURE_2D, texture);
	}

	glTranslatef((GLfloat)x, (GLfloat)y, (GLfloat)g_ZBuffer);

	glBegin(GL_TRIANGLE_STRIP);
		glTexCoord2i(0, 1); glVertex2i(0, height);
		glTexCoord2i(1, 1); glVertex2i(width, height);
		glTexCoord2i(0, 0); glVertex2i(0, 0);
		glTexCoord2i(1, 0); glVertex2i(width, 0);
	glEnd();

	glTranslatef((GLfloat)-x, (GLfloat)-y, (GLfloat)-g_ZBuffer);
}
//---------------------------------------------------------------------------
void TGLEngine::DrawRotated(GLuint &texture, const int &x, const int &y, const int &width, const int &height, const float &angle)
{
	if (m_OldTexture != texture)
	{
		m_OldTexture = texture;
		glBindTexture(GL_TEXTURE_2D, texture);
	}

	GLfloat translateY = (GLfloat)(y - height);

	glTranslatef((GLfloat)x, translateY, 0.0f);

	glRotatef(angle, 0.0f, 0.0f, 1.0f);

	glBegin(GL_TRIANGLE_STRIP);
		glTexCoord2i(0, 1); glVertex2i(0, -height);
		glTexCoord2i(1, 1); glVertex2i(width, -height);
		glTexCoord2i(0, 0); glVertex2i(0, height);
		glTexCoord2i(1, 0); glVertex2i(width, height);
	glEnd();

	glRotatef(angle, 0.0f, 0.0f, -1.0f);
	glTranslatef((GLfloat)-x, -translateY, 0.0f);
}
//---------------------------------------------------------------------------
void TGLEngine::Draw(GLuint &texture, const int &x, const int &y, const int &width, const int &height, const bool &mirror)
{
	if (m_OldTexture != texture)
	{
		m_OldTexture = texture;
		glBindTexture(GL_TEXTURE_2D, texture);
	}

	glTranslatef((GLfloat)x, (GLfloat)y, (GLfloat)g_ZBuffer);

	glBegin(GL_TRIANGLE_STRIP);

		if (mirror)
		{
			glTexCoord2i(0, 1); glVertex2i(width, height);
			glTexCoord2i(1, 1); glVertex2i(0, height);
			glTexCoord2i(0, 0); glVertex2i(width, 0);
			glTexCoord2i(1, 0); glVertex2i(0, 0);
		}
		else
		{
			glTexCoord2i(0, 1); glVertex2i(0, height);
			glTexCoord2i(1, 1); glVertex2i(width, height);
			glTexCoord2i(0, 0); glVertex2i(0, 0);
			glTexCoord2i(1, 0); glVertex2i(width, 0);
		}

	glEnd();

	glTranslatef((GLfloat)-x, (GLfloat)-y, (GLfloat)-g_ZBuffer);
}
//---------------------------------------------------------------------------
void TGLEngine::DrawSitting(GLuint &texture, const int &x, const int &y, const int &width, const int &height, const bool &mirror, float h3mod, float h6mod, float h9mod)
{
	if (m_OldTexture != texture)
	{
		m_OldTexture = texture;
		glBindTexture(GL_TEXTURE_2D, texture);
	}
	
	glTranslatef((GLfloat)x, (GLfloat)y, (GLfloat)g_ZBuffer);
	
	int h03 = (int)(height * 0.35f * h3mod);
	int h06 = (int)(height * 0.6f * h6mod);
	int h09 = (int)(height * 0.94f * h9mod);

	int widthOffset = width + SITTING_OFFSET_X;
	
	glBegin(GL_TRIANGLE_STRIP);

	if (mirror)
	{	
			glTexCoord2f(0.0f, 0.0f); glVertex2i(width, 0);
			glTexCoord2f(1.0f, 0.0f); glVertex2i(0, 0);
			glTexCoord2f(0.0f, 0.35f); glVertex2i(width, h03);
			glTexCoord2f(1.0f, 0.35f); glVertex2i(0, h03);
			
			if (h6mod)
			{
				glTexCoord2f(0.0f, 0.65f); glVertex2i(widthOffset, h06);
				glTexCoord2f(1.0f, 0.65f); glVertex2i(SITTING_OFFSET_X, h06);
			}
			
			if (h9mod)
			{
				glTexCoord2f(0.0f, 1.0f); glVertex2i(widthOffset, h09);
				glTexCoord2f(1.0f, 1.0f); glVertex2i(SITTING_OFFSET_X, h09);
			}
	}
	else
	{
			glTexCoord2f(0.0f, 0.0f); glVertex2i(SITTING_OFFSET_X, 0);
			glTexCoord2f(1.0f, 0.0f); glVertex2i(widthOffset, 0);
			glTexCoord2f(0.0f, 0.35f); glVertex2i(SITTING_OFFSET_X, h03);
			glTexCoord2f(1.0f, 0.35f); glVertex2i(widthOffset, h03);
			
			if (h6mod)
			{
				glTexCoord2f(0.0f, 0.65f); glVertex2i(0, h06);
				glTexCoord2f(1.0f, 0.65f); glVertex2i(width, h06);
			}

			
			if (h9mod)
			{
				glTexCoord2f(0.0f, 1.0f); glVertex2i(0, h09);
				glTexCoord2f(1.0f, 1.0f); glVertex2i(width, h09);
			}
	
	}

	glEnd();

	glTranslatef((GLfloat)-x, (GLfloat)-y, (GLfloat)-g_ZBuffer);
}
//---------------------------------------------------------------------------
void TGLEngine::DrawShadow(GLuint &texture, const int &x, const int &y, const int &width, const int &height, const bool &mirror)
{
	if (m_OldTexture != texture)
	{
		m_OldTexture = texture;
		glBindTexture(GL_TEXTURE_2D, texture);
	}

	GLfloat translateY = (GLfloat)(y + height);

	glTranslatef((GLfloat)x, translateY, (GLfloat)g_ZBuffer);

	glBegin(GL_TRIANGLE_STRIP);

		if (mirror)
		{
			glTexCoord2i(0, 1); glVertex2i(width, height);
			glTexCoord2i(1, 1); glVertex2i(0, height);
			glTexCoord2i(0, 0); glVertex2i(width * 2, 0);
			glTexCoord2i(1, 0); glVertex2i(width, 0);
		}
		else
		{
			glTexCoord2i(0, 1); glVertex2i(0, height);
			glTexCoord2i(1, 1); glVertex2i(width, height);
			glTexCoord2i(0, 0); glVertex2i(width, 0);
			glTexCoord2i(1, 0); glVertex2i(width * 2, 0);
		}

	glEnd();

	glTranslatef((GLfloat)-x, -translateY, (GLfloat)-g_ZBuffer);
}
//---------------------------------------------------------------------------
void TGLEngine::Draw(GLuint &texture, const int &x, const int &y, const int &width, const int &height, const int &drawWidth, const int &drawHeight)
{
	if (m_OldTexture != texture)
	{
		m_OldTexture = texture;
		glBindTexture(GL_TEXTURE_2D, texture);
	}

	glTranslatef((GLfloat)x, (GLfloat)y, (GLfloat)g_ZBuffer);

	float drawCountX = drawWidth / (float)width;
	float drawCountY = drawHeight / (float)height;

	glBegin(GL_TRIANGLE_STRIP);
		glTexCoord2f(0.0f, drawCountY);			glVertex2i(0, drawHeight);
		glTexCoord2f(drawCountX, drawCountY);	glVertex2i(drawWidth, drawHeight);
		glTexCoord2f(0.0f, 0.0f);				glVertex2i(0, 0);
		glTexCoord2f(drawCountX, 0.0f);			glVertex2i(drawWidth, 0);
	glEnd();

	glTranslatef((GLfloat)-x, (GLfloat)-y, (GLfloat)-g_ZBuffer);
}
//---------------------------------------------------------------------------
void TGLEngine::DrawResizepic(TTextureObject **th, const int &x, const int &y, const int &width, const int &height)
{
	IFOR(i, 0, 9)
	{
		glBindTexture(GL_TEXTURE_2D, th[i]->Texture);

		int drawWidth = th[i]->Width;
		int drawHeight = th[i]->Height;
		float drawCountX = 1.0f;
		float drawCountY = 1.0f;
		int drawX = x;
		int drawY = y;

		switch (i)
		{
			case 1:
			{
				drawX += th[0]->Width;

				drawWidth = width - th[0]->Width - th[2]->Width;

				if (drawWidth < 1)
					break;

				drawCountX = drawWidth / (float)th[i]->Width;

				break;
			}
			case 2:
			{
				drawX += width - drawWidth;

				break;
			}
			case 3:
			{
				drawY += th[0]->Height;

				drawHeight = height - th[0]->Height - th[5]->Height;

				if (drawHeight < 1)
					break;

				drawCountY = drawHeight / (float)th[i]->Height;

				break;
			}
			case 4:
			{
				drawX += width - drawWidth;
				drawY += th[2]->Height;

				drawHeight = height - th[2]->Height - th[7]->Height;

				if (drawHeight < 1)
					break;

				drawCountY = drawHeight / (float)th[i]->Height;

				break;
			}
			case 5:
			{
				drawY += height - drawHeight;

				break;
			}
			case 6:
			{
				drawX += th[5]->Width;
				drawY += height - drawHeight;

				drawWidth = width - th[5]->Width - th[7]->Width;

				if (drawWidth < 1)
					break;

				drawCountX = drawWidth / (float)th[i]->Width;

				break;
			}
			case 7:
			{
				drawX += width - drawWidth;
				drawY += height - drawHeight;

				break;
			}
			case 8:
			{
				drawX += th[0]->Width;
				drawY += th[0]->Height;

				drawWidth = width - th[0]->Width - th[2]->Width;

				if (drawWidth < 1)
					break;

				drawHeight = height - th[2]->Height - th[7]->Height;

				if (drawHeight < 1)
					break;

				drawCountX = drawWidth / (float)th[i]->Width;
				drawCountY = drawHeight / (float)th[i]->Height;

				break;
			}
			default:
				break;
		}

		glTranslatef((GLfloat)drawX, (GLfloat)drawY, 0.0f);

		glBegin(GL_TRIANGLE_STRIP);
			glTexCoord2f(0.0f, drawCountY);			glVertex2i(0, drawHeight);
			glTexCoord2f(drawCountX, drawCountY);	glVertex2i(drawWidth, drawHeight);
			glTexCoord2f(0.0f, 0.0f);				glVertex2i(0, 0);
			glTexCoord2f(drawCountX, 0.0f);			glVertex2i(drawWidth, 0);
		glEnd();

		glTranslatef((GLfloat)-drawX, (GLfloat)-drawY, 0.0f);
	}
}
//---------------------------------------------------------------------------