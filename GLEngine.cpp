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
	
	/*glEnable(GL_LIGHT0);
	glEnable(GL_LIGHTING);
	
	GLfloat lightKa[] = {0.2f, 0.2f, 0.2f, 1.0f};  // ambient light
	GLfloat lightKd[] = {0.7f, 0.7f, 0.7f, 1.0f};  // diffuse light
	GLfloat lightKs[] = {1.0f, 1.0f, 1.0f, 1.0f};  // specular light
	glLightfv(GL_LIGHT0, GL_AMBIENT, lightKa);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightKd);
	glLightfv(GL_LIGHT0, GL_SPECULAR, lightKs);

	// position the light
	float lightPos[4] = {0, 600, 100, 0.5f}; // positional light
	glLightfv(GL_LIGHT0, GL_POSITION, lightPos);

	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_FALSE);*/




	GLfloat lightPosition[]= { -100.0f, 300.0f, 5.0f, 0.0f };
	glLightfv(GL_LIGHT0, GL_POSITION, &lightPosition[0]);
	
	GLfloat lightAmbient[]= { 2.0f, 2.0f, 2.0f, 1.0f };
	glLightfv(GL_LIGHT0, GL_AMBIENT, &lightAmbient[0]);
	
	GLfloat lav = 2.2f;
	GLfloat lightAmbientValues[]= { lav, lav, lav, lav };
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, &lightAmbientValues[0]);

	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_FALSE);

	glEnable(GL_LIGHT0);



	/*int Level = 30; //31 max
	GLfloat lv = 10.0f; //5.0 - (Level * 0.1f);

	GLfloat LightAmbient[4] = { lv, lv, lv, lv };

	GLfloat LightDiffuse[]= { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat LightPosition[]= { 0.0f, 0.0f, 0.1f, 0.1f };
	
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	
	glLightfv(GL_LIGHT0, GL_AMBIENT, LightAmbient);

	glLightfv(GL_LIGHT0, GL_DIFFUSE, LightDiffuse);

	glLightfv(GL_LIGHT0, GL_POSITION, LightPosition);*/
	
	/*int idx = 0;

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0 + idx);

	float p[4] = {1, 1, 10000, 1};
	float d[3] = {0, 10, 0};

	glLightfv(GL_LIGHT0 + idx, GL_POSITION, p);
	glLightfv(GL_LIGHT0 + idx, GL_SPOT_DIRECTION, d);*/

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
	glPushMatrix();
	glLoadIdentity();

	glOrtho(0.0, g_ClientWidth, g_ClientHeight, 0.0, -100.0, 100.0);

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

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
void TGLEngine::ViewPort(int X, int Y, int SizeX, int SizeY)
{
	glViewport(X, g_ClientHeight - Y - SizeY, SizeX, SizeY);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(X, SizeX + X, SizeY + Y, Y, -100.0, 100.0);
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
void TGLEngine::DrawLine(DWORD Color, float X, float Y, float TargetX, float TargetY)
{
	glDisable(GL_TEXTURE_2D);

	glPushMatrix();
	glLoadIdentity();
	
	glColor3b(GetRValue(Color), GetGValue(Color), GetBValue(Color));

	glBegin(GL_LINES);
		glVertex2f(X, Y);
		glVertex2f(TargetX, TargetY);
	glEnd();
	
	glPopMatrix();

	glColor3f(1.0f, 1.0f, 1.0f);
	glEnable(GL_TEXTURE_2D);
}
//---------------------------------------------------------------------------
void TGLEngine::DrawPolygone(DWORD Color, float X, float Y, float Width, float Height)
{
	glDisable(GL_TEXTURE_2D);

	glPushMatrix();
	glLoadIdentity();
	glTranslatef(X, Y, 0.0f);
	
	glColor3b(GetRValue(Color), GetGValue(Color), GetBValue(Color));

	glBegin(GL_QUADS);
		glVertex2f(0.0f, Height);
		glVertex2f(Width, Height);
		glVertex2f(Width, 0.0f);
		glVertex2f(0.0f, 0.0f);
	glEnd();
	
	glPopMatrix();
	glTranslatef(0.0f, 0.0f, 0.1f);
	
	glColor3f(1.0f, 1.0f, 1.0f);
	glEnable(GL_TEXTURE_2D);
}
//---------------------------------------------------------------------------
void TGLEngine::DrawTriangle(DWORD Color, float X, float Y, float radius)
{
	glDisable(GL_TEXTURE_2D);

	glPushMatrix();
	glLoadIdentity();
	glTranslatef(X, Y, 0.0f);
	
	glColor3b(GetRValue(Color), GetGValue(Color), GetBValue(Color));

	glBegin( GL_TRIANGLE_FAN );

	glVertex2f( 0.0f, 0.0f );

	float pi = (float)M_PI * 2.0f;

	IFOR(i, 0, 360)
	{
		float a = i / 180.0f * pi;
		glVertex2f(cos(a) * radius, sin(a) * radius);
	}
	glEnd();
	
	glPopMatrix();
	glTranslatef(0.0f, 0.0f, 0.1f);
	
	glColor3f(1.0f, 1.0f, 1.0f);
	glEnable(GL_TEXTURE_2D);
}
//---------------------------------------------------------------------------
void TGLEngine::DrawLandTexture(GLuint &Texture, float X, float Y, float Width, float Height, RECT &rc, TVector *normals)
{
	glBindTexture(GL_TEXTURE_2D, Texture);

	glPushMatrix();
	glLoadIdentity();

	glTranslatef(X + 22, Y + 22 - rc.left, 0.0f);
	
	glBegin(GL_QUADS);
		glNormal3f((GLfloat)normals[3].X, (GLfloat)normals[3].Y, (GLfloat)normals[3].Z);
		glTexCoord2f(0.0f, 1.0f); glVertex2f(-22.0f, (GLfloat)rc.top); //<
		
		glNormal3f((GLfloat)normals[2].X, (GLfloat)normals[2].Y, (GLfloat)normals[2].Z);
		glTexCoord2f(1.0f, 1.0f); glVertex2f(0.0f, 22.0f + rc.right); //v
		
		glNormal3f((GLfloat)normals[1].X, (GLfloat)normals[1].Y, (GLfloat)normals[1].Z);
		glTexCoord2f(1.0f, 0.0f); glVertex2f(22.0f, (GLfloat)rc.bottom); //>
		
		glNormal3f((GLfloat)normals[0].X, (GLfloat)normals[0].Y, (GLfloat)normals[0].Z);
		glTexCoord2f(0.0f, 0.0f); glVertex2f(0.0f, -22.0f); //^
	glEnd();

	glPopMatrix();
}
//---------------------------------------------------------------------------
void TGLEngine::Draw(GLuint &Texture, float X, float Y, float Width, float Height)
{
	glBindTexture(GL_TEXTURE_2D, Texture);

	glLoadIdentity();
	glTranslatef(X, Y, 0.0f);

	glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 1.0f); glVertex2f(0.0f, Height);
		glTexCoord2f(1.0f, 1.0f); glVertex2f(Width, Height);
		glTexCoord2f(1.0f, 0.0f); glVertex2f(Width, 0.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex2f(0.0f, 0.0f);
	glEnd();
}
//---------------------------------------------------------------------------
void TGLEngine::Draw(GLuint &Texture, float X, float Y, float Width, float Height, bool Mirror)
{
	glBindTexture(GL_TEXTURE_2D, Texture);

	glLoadIdentity();
	glTranslatef(X, Y, 0.0f);

	if (Mirror)
	{
		glBegin(GL_QUADS);
			glTexCoord2f(0.0f, 1.0f); glVertex2f(Width, Height);
			glTexCoord2f(1.0f, 1.0f); glVertex2f(0.0f, Height);
			glTexCoord2f(1.0f, 0.0f); glVertex2f(0.0f, 0.0f);
			glTexCoord2f(0.0f, 0.0f); glVertex2f(Width, 0.0f);
		glEnd();
	}
	else
	{
		glBegin(GL_QUADS);
			glTexCoord2f(0.0f, 1.0f); glVertex2f(0.0f, Height);
			glTexCoord2f(1.0f, 1.0f); glVertex2f(Width, Height);
			glTexCoord2f(1.0f, 0.0f); glVertex2f(Width, 0.0f);
			glTexCoord2f(0.0f, 0.0f); glVertex2f(0.0f, 0.0f);
		glEnd();
	}
}
//---------------------------------------------------------------------------
void TGLEngine::Draw(GLuint &Texture, float X, float Y, float Width, float Height, float DrawWidth, float DrawHeight)
{
	glBindTexture(GL_TEXTURE_2D, Texture);

	glLoadIdentity();
	glTranslatef(X, Y, 0.0f);

	float draw_countX = DrawWidth / Width;
	float draw_countY = DrawHeight / Height;

	glBegin(GL_QUADS);
		glTexCoord2f(0.0f, draw_countY); glVertex2f(0.0f, DrawHeight);
		glTexCoord2f(draw_countX, draw_countY); glVertex2f(DrawWidth, DrawHeight);
		glTexCoord2f(draw_countX, 0.0f); glVertex2f(DrawWidth, 0.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex2f(0.0f, 0.0f);
	glEnd();
}
//---------------------------------------------------------------------------
void TGLEngine::DrawResizepic(TTextureObject **th, float x, float y, float width, float height)
{
	IFOR(i, 0, 9)
	{
		glLoadIdentity();

		glBindTexture(GL_TEXTURE_2D, th[i]->Texture);

		float drawWidth = (float)th[i]->Width;
		float drawHeight = (float)th[i]->Height;
		float drawCountX = 1.0f;
		float drawCountY = 1.0f;

		switch (i)
		{
			case 0:
			{
				glTranslatef(x, y, 0.0f);

				break;
			}
			case 1:
			{
				glTranslatef(x + th[0]->Width, y, 0.0f);

				drawWidth = width - th[0]->Width - th[2]->Width;

				if (drawWidth < 1)
					break;

				drawCountX = drawWidth / th[i]->Width;

				break;
			}
			case 2:
			{
				glTranslatef(x + width - drawWidth, y, 0.0f);

				break;
			}
			case 3:
			{
				glTranslatef(x, y + th[0]->Height, 0.0f);

				drawHeight = height - th[0]->Height - th[5]->Height;

				if (drawHeight < 1)
					break;

				drawCountY = drawHeight / th[i]->Height;

				break;
			}
			case 4:
			{
				glTranslatef(x + width - drawWidth, y + th[2]->Height, 0.0f);

				drawHeight = height - th[2]->Height - th[7]->Height;

				if (drawHeight < 1)
					break;

				drawCountY = drawHeight / th[i]->Height;

				break;
			}
			case 5:
			{
				glTranslatef(x, y + height - drawHeight, 0.0f);

				break;
			}
			case 6:
			{
				glTranslatef(x + th[5]->Width, y + height - drawHeight, 0.0f);

				drawWidth = width - th[5]->Width - th[7]->Width;

				if (drawWidth < 1)
					break;

				drawCountX = drawWidth / th[i]->Width;

				break;
			}
			case 7:
			{
				glTranslatef(x + width - drawWidth, y + height - drawHeight, 0.0f);

				break;
			}
			case 8:
			{
				glTranslatef(x + th[0]->Width, y + th[0]->Height, 0.0f);

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

		glBegin(GL_QUADS);
			glTexCoord2f(0.0f, drawCountY); glVertex2f(0.0f, drawHeight);
			glTexCoord2f(drawCountX, drawCountY); glVertex2f(drawWidth, drawHeight);
			glTexCoord2f(drawCountX, 0.0f); glVertex2f(drawWidth, 0.0f);
			glTexCoord2f(0.0f, 0.0f); glVertex2f(0.0f, 0.0f);
		glEnd();
	}
}
//---------------------------------------------------------------------------