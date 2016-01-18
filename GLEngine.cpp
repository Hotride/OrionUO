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
		PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER, //dwFlags
		PFD_TYPE_RGBA, //iPixelType
		24, //cColorBits
		0, 0, //cRedBits, cRedShift
		0, 0, //cGreenBits, cGreenShift
		0, 0, //cBlueBits, cBlueShift
		0, 0, //cAlphaBits, cAlphaShift
		0, 0, 0, 0, 0, //cAccumBits, cAccumRedBits, cAccumGreenBits, cAccumBlueBits, cAccumAlphaBits
		32, //cDepthBits
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

	TPRINT("g_UseFrameBuffer = %i\n", g_UseFrameBuffer);

	glClearColor(0.0, 0.0, 0.0, 1.0);		// Black Background
	glShadeModel(GL_SMOOTH);				// Enables Smooth Color Shading
	glClearDepth(1.0);						// Depth Buffer Setup
	glEnable(GL_DEPTH_TEST);				// Enable Depth Buffer
	glDepthFunc(GL_LESS);					// The Type Of Depth Test To Do

	//glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);   //Realy Nice perspective calculations

	glEnable(GL_TEXTURE_2D);

	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

	glClearStencil(0);
	glStencilMask(1);
	
	GLfloat lightPosition[]= { -100.0f, 300.0f, 5.0f, 0.0f };
	glLightfv(GL_LIGHT0, GL_POSITION, &lightPosition[0]);
	
	GLfloat lightAmbient[]= { 2.0f, 2.0f, 2.0f, 1.0f };
	glLightfv(GL_LIGHT0, GL_AMBIENT, &lightAmbient[0]);
	
	GLfloat lav = 2.2f;
	GLfloat lightAmbientValues[]= { lav, lav, lav, lav };
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, &lightAmbientValues[0]);

	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_FALSE);

	glEnable(GL_LIGHT0);

	glViewport(0, 0, g_ClientWidth, g_ClientHeight);
	glOrtho(0.0, g_ClientWidth, g_ClientHeight, 0.0, -100.0, 100.0);
	
	return true;
}
//---------------------------------------------------------------------------
void TGLEngine::UpdateRect()
{
	RECT cr;
	GetClientRect(g_hWnd, &cr);
	g_ClientWidth = cr.right - cr.left;
	g_ClientHeight = cr.bottom - cr.top;

	glViewport(0, 0, g_ClientWidth, g_ClientHeight);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glOrtho(0.0, g_ClientWidth, g_ClientHeight, 0.0, -100.0, 100.0);

	glMatrixMode(GL_MODELVIEW);

	if (GumpManager != NULL)
		GumpManager->RedrawAll();
}
//---------------------------------------------------------------------------
void TGLEngine::BindTexture(GLuint &Texture, int Width, int Height, PDWORD pixels)
{
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glGenTextures(1, &Texture);
	glBindTexture(GL_TEXTURE_2D, Texture);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA4, Width, Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
}
//---------------------------------------------------------------------------
void TGLEngine::BeginDraw()
{
	g_DrawMode = true;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	glLoadIdentity();
	
	glDisable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glDisable(GL_LIGHTING);
	glDisable(GL_STENCIL_TEST);
	glDisable(GL_BLEND);
	g_ZBuffer = 0.0f;

	EnableAlpha();
}
//---------------------------------------------------------------------------
void TGLEngine::EndDraw()
{
	g_DrawMode = false;

	DisableAlpha();

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
void TGLEngine::EnableAlpha()
{
	glEnable(GL_ALPHA_TEST);
    glAlphaFunc(GL_GREATER, 0.0f);
}
//---------------------------------------------------------------------------
void TGLEngine::DisableAlpha()
{
	glDisable(GL_ALPHA_TEST);
}
//---------------------------------------------------------------------------
void TGLEngine::ViewPort(int x, int y, int sizeX, int sizeY)
{
	glViewport(x, g_ClientHeight - y - sizeY, sizeX, sizeY);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(x, sizeX + x, sizeY + y, y, -100.0, 100.0);
	glMatrixMode(GL_MODELVIEW);
}
//---------------------------------------------------------------------------
void TGLEngine::RestorePort()
{
	glViewport(0, 0, g_ClientWidth, g_ClientHeight);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.0, g_ClientWidth, g_ClientHeight, 0.0, -100.0, 100.0);
	glMatrixMode(GL_MODELVIEW);
}
//---------------------------------------------------------------------------
void TGLEngine::DrawLine(DWORD color, float x, float y, float targetX, float targetY)
{
	glDisable(GL_TEXTURE_2D);

	glLoadIdentity();

	glColor3b(GetRValue(color), GetGValue(color), GetBValue(color));

	glBegin(GL_LINES);
		glVertex2f(x, y);
		glVertex2f(targetX, targetY);
	glEnd();

	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	glEnable(GL_TEXTURE_2D);
}
//---------------------------------------------------------------------------
void TGLEngine::DrawPolygone(DWORD color, float x, float y, float width, float height)
{
	glDisable(GL_TEXTURE_2D);

	glLoadIdentity();
	glTranslatef(x, y, 0.0f);

	glColor3b(GetRValue(color), GetGValue(color), GetBValue(color));

	glBegin(GL_QUADS);
		glVertex2f(0.0f, height);
		glVertex2f(width, height);
		glVertex2f(width, 0.0f);
		glVertex2f(0.0f, 0.0f);
	glEnd();

	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	glEnable(GL_TEXTURE_2D);
}
//---------------------------------------------------------------------------
void TGLEngine::DrawTriangle(DWORD color, float x, float y, float radius)
{
	glDisable(GL_TEXTURE_2D);

	glLoadIdentity();
	glTranslatef(x, y, 0.0f);

	glColor3b(GetRValue(color), GetGValue(color), GetBValue(color));

	glBegin( GL_TRIANGLE_FAN );

		glVertex2f(0.0f, 0.0f);

		float pi = (float)M_PI * 2.0f;

		for (int i = 0; i <= 360; i++)
		{
			float a = i / 180.0f * pi;
			glVertex2f(cos(a) * radius, sin(a) * radius);
		}

	glEnd();
	
	glColor3f(1.0f, 1.0f, 1.0f);
	glEnable(GL_TEXTURE_2D);
}
//---------------------------------------------------------------------------
void TGLEngine::DrawLandTexture(GLuint &texture, float x, float y, float width, float height, RECT &rc, TVector *normals)
{
	//glUniform1iARB(ShaderTexture, texture);

	glBindTexture(GL_TEXTURE_2D, texture);

	glLoadIdentity();
	glTranslatef(x, y - rc.left, g_ZBuffer);
	
	glBegin(GL_QUADS);
		glNormal3f((GLfloat)normals[3].X, (GLfloat)normals[3].Y, (GLfloat)normals[3].Z);
		glTexCoord2f(0.0f, 1.0f); glVertex2f(0.0f, rc.top + 22.0f); //<
		
		glNormal3f((GLfloat)normals[2].X, (GLfloat)normals[2].Y, (GLfloat)normals[2].Z);
		glTexCoord2f(1.0f, 1.0f); glVertex2f(22.0f, 44.0f + rc.right); //v
		
		glNormal3f((GLfloat)normals[1].X, (GLfloat)normals[1].Y, (GLfloat)normals[1].Z);
		glTexCoord2f(1.0f, 0.0f); glVertex2f(44.0f, rc.bottom + 22.0f); //>
		
		glNormal3f((GLfloat)normals[0].X, (GLfloat)normals[0].Y, (GLfloat)normals[0].Z);
		glTexCoord2f(0.0f, 0.0f); glVertex2f(22.0f, 0.0f); //^
	glEnd();
}
//---------------------------------------------------------------------------
void TGLEngine::Draw(GLuint &texture, float x, float y, float width, float height)
{
	//glUniform1iARB(ShaderTexture, texture);

	glBindTexture(GL_TEXTURE_2D, texture);

	glLoadIdentity();
	glTranslatef(x, y, g_ZBuffer);

	glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 1.0f); glVertex2f(0.0f, height);
		glTexCoord2f(1.0f, 1.0f); glVertex2f(width, height);
		glTexCoord2f(1.0f, 0.0f); glVertex2f(width, 0.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex2f(0.0f, 0.0f);
	glEnd();
}
//---------------------------------------------------------------------------
void TGLEngine::Draw(GLuint &texture, float x, float y, float width, float height, bool &mirror)
{
	//glUniform1iARB(ShaderTexture, texture);

	glBindTexture(GL_TEXTURE_2D, texture);

	glLoadIdentity();
	glTranslatef(x, y, g_ZBuffer);

	if (mirror)
	{
		glBegin(GL_QUADS);
			glTexCoord2f(0.0f, 1.0f); glVertex2f(width, height);
			glTexCoord2f(1.0f, 1.0f); glVertex2f(0.0f, height);
			glTexCoord2f(1.0f, 0.0f); glVertex2f(0.0f, 0.0f);
			glTexCoord2f(0.0f, 0.0f); glVertex2f(width, 0.0f);
		glEnd();
	}
	else
	{
		glBegin(GL_QUADS);
			glTexCoord2f(0.0f, 1.0f); glVertex2f(0.0f, height);
			glTexCoord2f(1.0f, 1.0f); glVertex2f(width, height);
			glTexCoord2f(1.0f, 0.0f); glVertex2f(width, 0.0f);
			glTexCoord2f(0.0f, 0.0f); glVertex2f(0.0f, 0.0f);
		glEnd();
	}
}
//---------------------------------------------------------------------------
void TGLEngine::Draw(GLuint &texture, float x, float y, float width, float height, float drawWidth, float drawHeight)
{
	//glUniform1iARB(ShaderTexture, texture);

	glBindTexture(GL_TEXTURE_2D, texture);

	glLoadIdentity();
	glTranslatef(x, y, g_ZBuffer);

	float drawCountX = drawWidth / width;
	float drawCountY = drawHeight / height;

	glBegin(GL_QUADS);
		glTexCoord2f(0.0f, drawCountY);			glVertex2f(0.0f, drawHeight);
		glTexCoord2f(drawCountX, drawCountY);	glVertex2f(drawWidth, drawHeight);
		glTexCoord2f(drawCountX, 0.0f);			glVertex2f(drawWidth, 0.0f);
		glTexCoord2f(0.0f, 0.0f);				glVertex2f(0.0f, 0.0f);
	glEnd();
}
//---------------------------------------------------------------------------
void TGLEngine::DrawResizepic(TTextureObject **th, float x, float y, float width, float height)
{
	IFOR(i, 0, 9)
	{
		//glUniform1iARB(ShaderTexture, th[i]->Texture);

		glBindTexture(GL_TEXTURE_2D, th[i]->Texture);

		float drawWidth = (float)th[i]->Width;
		float drawHeight = (float)th[i]->Height;
		float drawCountX = 1.0f;
		float drawCountY = 1.0f;
		float X = x;
		float Y = y;

		switch (i)
		{
			case 1:
			{
				X += th[0]->Width;

				drawWidth = width - th[0]->Width - th[2]->Width;

				if (drawWidth < 1)
					break;

				drawCountX = drawWidth / th[i]->Width;

				break;
			}
			case 2:
			{
				X += width - drawWidth;

				break;
			}
			case 3:
			{
				Y += th[0]->Height;

				drawHeight = height - th[0]->Height - th[5]->Height;

				if (drawHeight < 1)
					break;

				drawCountY = drawHeight / th[i]->Height;

				break;
			}
			case 4:
			{
					  X += width - drawWidth;
					  Y += th[2]->Height;

				drawHeight = height - th[2]->Height - th[7]->Height;

				if (drawHeight < 1)
					break;

				drawCountY = drawHeight / th[i]->Height;

				break;
			}
			case 5:
			{
				Y += height - drawHeight;

				break;
			}
			case 6:
			{
				X += th[5]->Width;
				Y += height - drawHeight;

				drawWidth = width - th[5]->Width - th[7]->Width;

				if (drawWidth < 1)
					break;

				drawCountX = drawWidth / th[i]->Width;

				break;
			}
			case 7:
			{
				X += width - drawWidth;
				Y += height - drawHeight;

				break;
			}
			case 8:
			{
				X += th[0]->Width;
				Y += th[0]->Height;

				drawWidth = width - th[0]->Width - th[2]->Width;

				if (drawWidth < 1)
					break;

				drawHeight = height - th[2]->Height - th[7]->Height;

				if (drawHeight < 1)
					break;

				drawCountX = drawWidth / th[i]->Width;
				drawCountY = drawHeight / th[i]->Height;

				break;
			}
			default: break;
		}

		glLoadIdentity();
		glTranslatef(X, Y, g_ZBuffer);

		glBegin(GL_QUADS);
			glTexCoord2f(0.0f, drawCountY);			glVertex2f(0.0f, drawHeight);
			glTexCoord2f(drawCountX, drawCountY);	glVertex2f(drawWidth, drawHeight);
			glTexCoord2f(drawCountX, 0.0f);			glVertex2f(drawWidth, 0.0f);
			glTexCoord2f(0.0f, 0.0f);				glVertex2f(0.0f, 0.0f);
		glEnd();
	}
}
//---------------------------------------------------------------------------