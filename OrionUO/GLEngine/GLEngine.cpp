/***********************************************************************************
**
** GLEngine.cpp
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#include "stdafx.h"

#pragma comment(lib, "glew32.lib")
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "Glu32.lib")
//----------------------------------------------------------------------------------
CGLEngine g_GL;

BIND_TEXTURE_16_FUNCTION g_GL_BindTexture16_Ptr = &CGLEngine::GL1_BindTexture16;
BIND_TEXTURE_32_FUNCTION g_GL_BindTexture32_Ptr = &CGLEngine::GL1_BindTexture32;

DRAW_LAND_TEXTURE_FUNCTION g_GL_DrawLandTexture_Ptr = &CGLEngine::GL1_DrawLandTexture;
DRAW_TEXTURE_FUNCTION g_GL_Draw_Ptr = &CGLEngine::GL1_Draw;
DRAW_TEXTURE_ROTATED_FUNCTION g_GL_DrawRotated_Ptr = &CGLEngine::GL1_DrawRotated;
DRAW_TEXTURE_MIRRORED_FUNCTION g_GL_DrawMirrored_Ptr = &CGLEngine::GL1_DrawMirrored;
DRAW_TEXTURE_SITTING_FUNCTION g_GL_DrawSitting_Ptr = &CGLEngine::GL1_DrawSitting;
DRAW_TEXTURE_SHADOW_FUNCTION g_GL_DrawShadow_Ptr = &CGLEngine::GL1_DrawShadow;
DRAW_TEXTURE_STRETCHED_FUNCTION g_GL_DrawStretched_Ptr = &CGLEngine::GL1_DrawStretched;
DRAW_TEXTURE_RESIZEPIC_FUNCTION g_GL_DrawResizepic_Ptr = &CGLEngine::GL1_DrawResizepic;
//----------------------------------------------------------------------------------
CGLEngine::CGLEngine()
{
}
//----------------------------------------------------------------------------------
CGLEngine::~CGLEngine()
{
	WISPFUN_DEBUG("c29_f1");
	if (m_PositionBuffer != 0)
	{
		glDeleteBuffers(1, &m_PositionBuffer);
		m_PositionBuffer = 0;
	}

	Uninstall();
}
//----------------------------------------------------------------------------------
bool CGLEngine::GLSetupPixelFormat()
{
	WISPFUN_DEBUG("c29_f2");
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
//----------------------------------------------------------------------------------
bool CGLEngine::Install()
{
	WISPFUN_DEBUG("c29_f3");
	m_OldTexture = -1;

	m_DC = ::GetDC(g_OrionWindow.Handle);
	if (!GLSetupPixelFormat())
		return false;

	m_RC = wglCreateContext(m_DC);
	if (!m_RC)
		return false;

	if (!wglMakeCurrent(m_DC, m_RC))
		return false;
	
	int glewInitResult = glewInit();
	LOG("glewInit() = %i rt=%i fb=%i v(%s) (shader: %i)\n", glewInitResult, WGLEW_ARB_render_texture, GL_ARB_framebuffer_object, glGetString(GL_VERSION), GL_ARB_shader_objects);

	if (!glewInitResult)
	{
		m_CanUseFrameBuffer = (GL_ARB_framebuffer_object &&
			glBindFramebuffer &&
			glDeleteFramebuffers &&
			glFramebufferTexture2D &&
			glGenFramebuffers
			);

		m_CanUseBuffer = (GL_VERSION_1_5 &&
			glBindBuffer &&
			glBufferData &&
			glDeleteBuffers &&
			glGenBuffers
			);

		m_CanUseBuffer = false;

		if (m_CanUseBuffer)
		{
			glGenBuffers(3, &m_PositionBuffer);

			int positionArray[] = { 0, 1, 1, 1, 0, 0, 1, 0 };

			glBindBuffer(GL_ARRAY_BUFFER, m_PositionBuffer);
			glBufferData(GL_ARRAY_BUFFER, sizeof(positionArray), &positionArray[0], GL_STATIC_DRAW);

			g_GL_BindTexture16_Ptr = &CGLEngine::GL2_BindTexture16;
			g_GL_BindTexture32_Ptr = &CGLEngine::GL2_BindTexture32;

			g_GL_DrawLandTexture_Ptr = &CGLEngine::GL2_DrawLandTexture;
			g_GL_Draw_Ptr = &CGLEngine::GL2_Draw;
			g_GL_DrawRotated_Ptr = &CGLEngine::GL2_DrawRotated;
			g_GL_DrawMirrored_Ptr = &CGLEngine::GL2_DrawMirrored;
			g_GL_DrawSitting_Ptr = &CGLEngine::GL2_DrawSitting;
			g_GL_DrawShadow_Ptr = &CGLEngine::GL2_DrawShadow;
			g_GL_DrawStretched_Ptr = &CGLEngine::GL2_DrawStretched;
			g_GL_DrawResizepic_Ptr = &CGLEngine::GL2_DrawResizepic;
		}
	}
	else
		return false;

	LOG("g_UseFrameBuffer = %i; m_CanUseBuffer = %i\n", m_CanUseFrameBuffer, m_CanUseBuffer);

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);		// Black Background
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

	ViewPort(0, 0, g_OrionWindow.Size.Width, g_OrionWindow.Size.Height);
	
	return true;
}
//----------------------------------------------------------------------------------
void CGLEngine::Uninstall()
{
	WISPFUN_DEBUG("c29_f4");
	m_DC = 0;
	wglMakeCurrent(NULL, NULL);

	if (m_RC != 0)
	{
		wglDeleteContext(m_RC);
		m_RC = 0;
	}
}
//----------------------------------------------------------------------------------
void CGLEngine::UpdateRect()
{
	WISPFUN_DEBUG("c29_f5");
	RECT cr;
	GetClientRect(g_OrionWindow.Handle, &cr);
	int width = cr.right - cr.left;
	int height = cr.bottom - cr.top;

	ViewPort(0, 0, width, height);
	//ViewPort(0, 0, g_OrionWindow.Size.Width, g_OrionWindow.Size.Height);

	g_GumpManager.RedrawAll();
}
//----------------------------------------------------------------------------------
void CGLEngine::GL1_BindTexture16(CGLTexture &texture, const int &width, const int &height, pushort pixels)
{
	WISPFUN_DEBUG("c29_f6");
	GLuint tex = 0;
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glGenTextures(1, &tex);
	glBindTexture(GL_TEXTURE_2D, tex);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB5_A1, width, height, 0, GL_BGRA, GL_UNSIGNED_SHORT_1_5_5_5_REV, pixels);

	texture.Width = width;
	texture.Height = height;
	texture.Texture = tex;

	vector<bool> &hitMap = texture.m_HitMap;
	hitMap.resize(width * height);
	int pos = 0;

	IFOR(y, 0, height)
	{
		IFOR(x, 0, width)
		{
			hitMap[pos] = (pixels[pos] != 0);
			pos++;
		}
	}
}
//----------------------------------------------------------------------------------
void CGLEngine::GL1_BindTexture32(CGLTexture &texture, const int &width, const int &height, puint pixels)
{
	WISPFUN_DEBUG("c29_f7");
	GLuint tex = 0;
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glGenTextures(1, &tex);
	glBindTexture(GL_TEXTURE_2D, tex);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA4, width, height, 0, GL_BGRA, GL_UNSIGNED_INT_8_8_8_8, pixels);

	texture.Width = width;
	texture.Height = height;
	texture.Texture = tex;

	vector<bool> &hitMap = texture.m_HitMap;
	hitMap.resize(width * height);
	int pos = 0;

	IFOR(y, 0, height)
	{
		IFOR(x, 0, width)
		{
			hitMap[pos] = (pixels[pos] != 0);
			pos++;
		}
	}
}
//----------------------------------------------------------------------------------
void CGLEngine::GL2_CreateArrays(CGLTexture &texture, const int &width, const int &height)
{
	WISPFUN_DEBUG("c29_f8");
	GLuint vbo[2] = { 0 };
	glGenBuffers(2, &vbo[0]);

	int vertexArray[] = { 0, height, width, height, 0, 0, width, 0 };

	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexArray), &vertexArray[0], GL_STATIC_DRAW);

	int mirroredVertexArray[] = { width, height, 0, height, width, 0, 0, 0 };

	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(mirroredVertexArray), &mirroredVertexArray[0], GL_STATIC_DRAW);

	texture.VertexBuffer = vbo[0];
	texture.MirroredVertexBuffer = vbo[1];
}
//----------------------------------------------------------------------------------
void CGLEngine::GL2_BindTexture16(CGLTexture &texture, const int &width, const int &height, pushort pixels)
{
	WISPFUN_DEBUG("c29_f9");
	GL1_BindTexture16(texture, width, height, pixels);
	GL2_CreateArrays(texture, width, height);
}
//----------------------------------------------------------------------------------
void CGLEngine::GL2_BindTexture32(CGLTexture &texture, const int &width, const int &height, puint pixels)
{
	WISPFUN_DEBUG("c29_f10");
	GL1_BindTexture32(texture, width, height, pixels);
	GL2_CreateArrays(texture, width, height);
}
//----------------------------------------------------------------------------------
void CGLEngine::BeginDraw()
{
	WISPFUN_DEBUG("c29_f11");
	m_Drawing = true;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	glLoadIdentity();
	
	glDisable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glDisable(GL_STENCIL_TEST);
	glDisable(GL_BLEND);

	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER, 0.0f);

	if (m_CanUseBuffer)
	{
		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	}
}
//----------------------------------------------------------------------------------
void CGLEngine::EndDraw()
{
	WISPFUN_DEBUG("c29_f12");
	m_Drawing = false;

	if (m_CanUseBuffer)
	{
		glDisableClientState(GL_VERTEX_ARRAY);
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	}

	glDisable(GL_ALPHA_TEST);

	SwapBuffers(m_DC);
}
//----------------------------------------------------------------------------------
void CGLEngine::BeginStencil()
{
	WISPFUN_DEBUG("c29_f13");
	glEnable(GL_STENCIL_TEST);

	glColorMask(false, false, false, false);

	glStencilFunc(GL_ALWAYS, 1, 1);
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
}
//----------------------------------------------------------------------------------
void CGLEngine::EndStencil()
{
	WISPFUN_DEBUG("c29_f14");
	glColorMask(true, true, true, true);
					
	glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
	glStencilFunc(GL_NOTEQUAL, 1, 1);

	glDisable(GL_STENCIL_TEST);
}
//----------------------------------------------------------------------------------
void CGLEngine::ViewPortScaled(const int &x, const int &y, const int &width, const int &height)
{
	WISPFUN_DEBUG("c29_f15");
	glViewport(x, g_OrionWindow.Size.Height - y - height, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	GLdouble left = (GLdouble)x;
	GLdouble right = (GLdouble)(width + x);
	GLdouble top = (GLdouble)y;
	GLdouble bottom = (GLdouble)(height + y);

	GLdouble newRight = right * g_GlobalScale;
	GLdouble newBottom = bottom * g_GlobalScale;

	left = (left * g_GlobalScale) - (newRight - right);
	top = (top * g_GlobalScale) - (newBottom - bottom);

	glOrtho(left, newRight, newBottom, top, -150.0, 150.0);
	glMatrixMode(GL_MODELVIEW);
}
//----------------------------------------------------------------------------------
void CGLEngine::ViewPort(const int &x, const int &y, const int &width, const int &height)
{
	WISPFUN_DEBUG("c29_f16");
	glViewport(x, g_OrionWindow.Size.Height - y - height, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(x, width + x, height + y, y, -150.0, 150.0);
	glMatrixMode(GL_MODELVIEW);
}
//----------------------------------------------------------------------------------
void CGLEngine::RestorePort()
{
	WISPFUN_DEBUG("c29_f17");
	glViewport(0, 0, g_OrionWindow.Size.Width, g_OrionWindow.Size.Height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.0, g_OrionWindow.Size.Width, g_OrionWindow.Size.Height, 0.0, -150.0, 150.0);
	glMatrixMode(GL_MODELVIEW);
}
//----------------------------------------------------------------------------------
void CGLEngine::PushScissor(const int &x, const int &y, const int &width, const int &height)
{
	WISPFUN_DEBUG("c29_f18");
	PushScissor(WISP_GEOMETRY::CRect(x, y, width, height));
}
//----------------------------------------------------------------------------------
void CGLEngine::PushScissor(const WISP_GEOMETRY::CPoint2Di &position, const int &width, const int &height)
{
	WISPFUN_DEBUG("c29_f19");
	PushScissor(WISP_GEOMETRY::CRect(position, width, height));
}
//----------------------------------------------------------------------------------
void CGLEngine::PushScissor(const int &x, const int &y, const WISP_GEOMETRY::CSize &size)
{
	WISPFUN_DEBUG("c29_f20");
	PushScissor(WISP_GEOMETRY::CRect(x, y, size));
}
//----------------------------------------------------------------------------------
void CGLEngine::PushScissor(const WISP_GEOMETRY::CPoint2Di &position, const WISP_GEOMETRY::CSize &size)
{
	WISPFUN_DEBUG("c29_f21");
	PushScissor(WISP_GEOMETRY::CRect(position, size));
}
//----------------------------------------------------------------------------------
void CGLEngine::PushScissor(const WISP_GEOMETRY::CRect &rect)
{
	WISPFUN_DEBUG("c29_f22");
	m_ScissorList.push_back(rect);

	glEnable(GL_SCISSOR_TEST);

	glScissor(rect.Position.X, rect.Position.Y, rect.Size.Width, rect.Size.Height);
}
//----------------------------------------------------------------------------------
void CGLEngine::PopScissor()
{
	WISPFUN_DEBUG("c29_f23");
	m_ScissorList.pop_back();

	if (m_ScissorList.empty())
		glDisable(GL_SCISSOR_TEST);
	else
	{
		WISP_GEOMETRY::CRect &rect = m_ScissorList.back();
		glScissor(rect.Position.X, rect.Position.Y, rect.Size.Width, rect.Size.Height);
	}
}
//----------------------------------------------------------------------------------
void CGLEngine::ClearScissorList()
{
	WISPFUN_DEBUG("c29_f24");
	m_ScissorList.clear();

	glDisable(GL_SCISSOR_TEST);
}
//----------------------------------------------------------------------------------
inline void CGLEngine::BindTexture(const GLuint &texture)
{
	WISPFUN_DEBUG("c29_f25");
	if (m_OldTexture != texture)
	{
		m_OldTexture = texture;
		glBindTexture(GL_TEXTURE_2D, texture);
	}
}
//----------------------------------------------------------------------------------
void CGLEngine::DrawLine(const int &x, const int &y, const int &targetX, const int &targetY)
{
	WISPFUN_DEBUG("c29_f26");
	glDisable(GL_TEXTURE_2D);

	glBegin(GL_LINES);
		glVertex2i(x, y);
		glVertex2i(targetX, targetY);
	glEnd();

	glEnable(GL_TEXTURE_2D);
}
//----------------------------------------------------------------------------------
void CGLEngine::DrawPolygone(const int &x, const int &y, const int &width, const int &height)
{
	WISPFUN_DEBUG("c29_f27");
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
//----------------------------------------------------------------------------------
void CGLEngine::DrawCircle(const float &x, const float &y, const float &radius, const int &gradientMode)
{
	WISPFUN_DEBUG("c29_f28");
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
//----------------------------------------------------------------------------------
void CGLEngine::GL1_DrawLandTexture(const CGLTexture &texture, const int &x, const int &y, CLandObject *land)
{
	WISPFUN_DEBUG("c29_f29");
	BindTexture(texture.Texture);

	float translateX = x - 22.0f;
	float translateY = y - 22.0f;

	const RECT &rc = land->m_Rect;
	CVector *normals = land->m_Normals;

	glTranslatef(translateX, translateY, 0.0f);

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

	glTranslatef(-translateX, -translateY, 0.0f);
}
//----------------------------------------------------------------------------------
void CGLEngine::GL1_Draw(const CGLTexture &texture, const int &x, const int &y)
{
	WISPFUN_DEBUG("c29_f30");
	BindTexture(texture.Texture);

	int width = texture.Width;
	int height = texture.Height;

	glTranslatef((GLfloat)x, (GLfloat)y, 0.0f);

	glBegin(GL_TRIANGLE_STRIP);
		glTexCoord2i(0, 1); glVertex2i(0, height);
		glTexCoord2i(1, 1); glVertex2i(width, height);
		glTexCoord2i(0, 0); glVertex2i(0, 0);
		glTexCoord2i(1, 0); glVertex2i(width, 0);
	glEnd();

	glTranslatef((GLfloat)-x, (GLfloat)-y, 0.0f);
}
//----------------------------------------------------------------------------------
void CGLEngine::GL1_DrawRotated(const CGLTexture &texture, const int &x, const int &y, const float &angle)
{
	WISPFUN_DEBUG("c29_f31");
	BindTexture(texture.Texture);

	int width = texture.Width;
	int height = texture.Height;

	GLfloat translateY = (GLfloat)(y - height);

	glTranslatef((GLfloat)x, translateY, 0.0f);

	glRotatef(angle, 0.0f, 0.0f, 1.0f);

	glBegin(GL_TRIANGLE_STRIP);
		glTexCoord2i(0, 1); glVertex2i(0, height);
		glTexCoord2i(1, 1); glVertex2i(width, height);
		glTexCoord2i(0, 0); glVertex2i(0, 0);
		glTexCoord2i(1, 0); glVertex2i(width, 0);
	glEnd();

	glRotatef(angle, 0.0f, 0.0f, -1.0f);
	glTranslatef((GLfloat)-x, -translateY, 0.0f);
}
//----------------------------------------------------------------------------------
void CGLEngine::GL1_DrawMirrored(const CGLTexture &texture, const int &x, const int &y, const bool &mirror)
{
	WISPFUN_DEBUG("c29_f32");
	BindTexture(texture.Texture);

	int width = texture.Width;
	int height = texture.Height;

	glTranslatef((GLfloat)x, (GLfloat)y, 0.0f);

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

	glTranslatef((GLfloat)-x, (GLfloat)-y, 0.0f);
}
//----------------------------------------------------------------------------------
void CGLEngine::GL1_DrawSitting(const CGLTexture &texture, const int &x, const int &y, const bool &mirror, const float &h3mod, const float &h6mod, const float &h9mod)
{
	WISPFUN_DEBUG("c29_f33");
	BindTexture(texture.Texture);
	
	glTranslatef((GLfloat)x, (GLfloat)y, 0.0f);

	float width = (float)texture.Width;
	float height = (float)texture.Height;
	
	float h03 = height * h3mod;
	float h06 = height * h6mod;
	float h09 = height * h9mod;

	float widthOffset = (float)(width + m_SittingCharacterOffset);
	glBegin(GL_TRIANGLE_STRIP);

	if (mirror)
	{	
		if (h3mod)
		{
			glTexCoord2f(0.0f, 0.0f); glVertex2f(width, 0);
			glTexCoord2f(1.0f, 0.0f); glVertex2f(0, 0);
			glTexCoord2f(0.0f, h3mod); glVertex2f(width, h03);
			glTexCoord2f(1.0f, h3mod); glVertex2f(0, h03);
		}

		if (h6mod)
		{
			if (!h3mod)
			{
				glTexCoord2f(0.0f, 0.0f); glVertex2f(width, 0);
				glTexCoord2f(1.0f, 0.0f); glVertex2f(0, 0);
			}

			glTexCoord2f(0.0f, h6mod); glVertex2f(widthOffset, h06);
			glTexCoord2f(1.0f, h6mod); glVertex2f(m_SittingCharacterOffset, h06);
		}

		if (h9mod)
		{
			if (!h6mod)
			{
				glTexCoord2f(0.0f, 0.0f); glVertex2f(widthOffset, 0);
				glTexCoord2f(1.0f, 0.0f); glVertex2f(m_SittingCharacterOffset, 0);
			}

			glTexCoord2f(0.0f, 1.0f); glVertex2f(widthOffset, h09);
			glTexCoord2f(1.0f, 1.0f); glVertex2f(m_SittingCharacterOffset, h09);

		}
	}
	else
	{
		if (h3mod)
		{
			glTexCoord2f(0.0f, 0.0f); glVertex2f(m_SittingCharacterOffset, 0);
			glTexCoord2f(1.0f, 0.0f); glVertex2f(widthOffset, 0);
			glTexCoord2f(0.0f, h3mod); glVertex2f(m_SittingCharacterOffset, h03);
			glTexCoord2f(1.0f, h3mod); glVertex2f(widthOffset, h03);
		}

		if (h6mod)
		{
			if (!h3mod)
			{
				glTexCoord2f(0.0f, 0.0f); glVertex2f(m_SittingCharacterOffset, 0);
				glTexCoord2f(1.0f, 0.0f); glVertex2f(width + m_SittingCharacterOffset, 0);
			}

			glTexCoord2f(0.0f, h6mod); glVertex2f(0, h06);
			glTexCoord2f(1.0f, h6mod); glVertex2f(width, h06);
		}

		if (h9mod)
		{
			if (!h6mod)
			{
				glTexCoord2f(0.0f, 0.0f); glVertex2f(0, 0);
				glTexCoord2f(1.0f, 0.0f); glVertex2f(width, 0);
			}

			glTexCoord2f(0.0f, 1.0f); glVertex2f(0, h09);
			glTexCoord2f(1.0f, 1.0f); glVertex2f(width, h09);
		}

	}

	glEnd();

	glTranslatef((GLfloat)-x, (GLfloat)-y, 0.0f);
}
//----------------------------------------------------------------------------------
void CGLEngine::GL1_DrawShadow(const CGLTexture &texture, const int &x, const int &y, const bool &mirror)
{
	WISPFUN_DEBUG("c29_f34");
	BindTexture(texture.Texture);

	float width = (float)texture.Width;
	float height = texture.Height / 2.0f;

	GLfloat translateY = (GLfloat)(y + height * 0.75);

	glTranslatef((GLfloat)x, translateY, 0.0f);

	glBegin(GL_TRIANGLE_STRIP);

		float ratio = height / width;

		if (mirror)
		{
			glTexCoord2f(0, 1); glVertex2f(width, height);
			glTexCoord2f(1, 1); glVertex2f(0, height);
			glTexCoord2f(0, 0); glVertex2f(width * (ratio + 1.0f), 0);
			glTexCoord2f(1, 0); glVertex2f(width * ratio, 0);
		}
		else
		{
			glTexCoord2f(0, 1); glVertex2f(0, height);
			glTexCoord2f(1, 1); glVertex2f(width, height);
			glTexCoord2f(0, 0); glVertex2f(width * ratio, 0);
			glTexCoord2f(1, 0); glVertex2f(width * (ratio + 1.0f), 0);
		}

	glEnd();

	glTranslatef((GLfloat)-x, -translateY, 0.0f);
}
//----------------------------------------------------------------------------------
void CGLEngine::GL1_DrawStretched(const CGLTexture &texture, const int &x, const int &y, const int &drawWidth, const int &drawHeight)
{
	WISPFUN_DEBUG("c29_f35");
	BindTexture(texture.Texture);

	int width = texture.Width;
	int height = texture.Height;

	glTranslatef((GLfloat)x, (GLfloat)y, 0.0f);

	float drawCountX = drawWidth / (float)width;
	float drawCountY = drawHeight / (float)height;

	glBegin(GL_TRIANGLE_STRIP);
		glTexCoord2f(0.0f, drawCountY);			glVertex2i(0, drawHeight);
		glTexCoord2f(drawCountX, drawCountY);	glVertex2i(drawWidth, drawHeight);
		glTexCoord2f(0.0f, 0.0f);				glVertex2i(0, 0);
		glTexCoord2f(drawCountX, 0.0f);			glVertex2i(drawWidth, 0);
	glEnd();

	glTranslatef((GLfloat)-x, (GLfloat)-y, 0.0f);
}
//----------------------------------------------------------------------------------
void CGLEngine::GL1_DrawResizepic(CGLTexture **th, const int &x, const int &y, const int &width, const int &height)
{
	WISPFUN_DEBUG("c29_f36");
	IFOR(i, 0, 9)
	{
		BindTexture(th[i]->Texture);

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

				drawCountY = drawHeight / (float)th[i]->Height;

				break;
			}
			case 4:
			{
				drawX += width - drawWidth;
				drawY += th[2]->Height;

				drawHeight = height - th[2]->Height - th[7]->Height;

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

				drawHeight = height - th[2]->Height - th[7]->Height;

				drawCountX = drawWidth / (float)th[i]->Width;
				drawCountY = drawHeight / (float)th[i]->Height;

				break;
			}
			default:
				break;
		}

		if (drawWidth < 1 || drawHeight < 1)
			continue;

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

























//----------------------------------------------------------------------------------
void CGLEngine::GL2_DrawLandTexture(const CGLTexture &texture, const int &x, const int &y, CLandObject *land)
{
	WISPFUN_DEBUG("c29_f37");
	BindTexture(texture.Texture);

	float translateX = x - 22.0f;
	float translateY = y - 22.0f;

	glTranslatef(translateX, translateY, 0.0f);

	glBindBuffer(GL_ARRAY_BUFFER, land->VertexBuffer);
	glVertexPointer(2, GL_INT, 0, (PVOID)0);

	glBindBuffer(GL_ARRAY_BUFFER, land->PositionBuffer);
	glTexCoordPointer(2, GL_INT, 0, (PVOID)0);

	glBindBuffer(GL_ARRAY_BUFFER, land->NormalBuffer);
	glNormalPointer(GL_FLOAT, 0, (PVOID)0);

	glEnableClientState(GL_NORMAL_ARRAY);

	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	glDisableClientState(GL_NORMAL_ARRAY);

	glTranslatef(-translateX, -translateY, 0.0f);
}
//----------------------------------------------------------------------------------
void CGLEngine::GL2_Draw(const CGLTexture &texture, const int &x, const int &y)
{
	WISPFUN_DEBUG("c29_f38");
	BindTexture(texture.Texture);

	int width = texture.Width;
	int height = texture.Height;

	glTranslatef((GLfloat)x, (GLfloat)y, 0.0f);

	glBindBuffer(GL_ARRAY_BUFFER, texture.VertexBuffer);
	glVertexPointer(2, GL_INT, 0, (PVOID)0);

	glBindBuffer(GL_ARRAY_BUFFER, m_PositionBuffer);
	glTexCoordPointer(2, GL_INT, 0, (PVOID)0);

	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	glTranslatef((GLfloat)-x, (GLfloat)-y, 0.0f);
}
//----------------------------------------------------------------------------------
void CGLEngine::GL2_DrawRotated(const CGLTexture &texture, const int &x, const int &y, const float &angle)
{
	WISPFUN_DEBUG("c29_f39");
	BindTexture(texture.Texture);

	int width = texture.Width;
	int height = texture.Height;

	GLfloat translateY = (GLfloat)(y - height);

	glTranslatef((GLfloat)x, translateY, 0.0f);

	glRotatef(angle, 0.0f, 0.0f, 1.0f);

	glBindBuffer(GL_ARRAY_BUFFER, texture.VertexBuffer);
	glVertexPointer(2, GL_INT, 0, (PVOID)0);

	glBindBuffer(GL_ARRAY_BUFFER, m_PositionBuffer);
	glTexCoordPointer(2, GL_INT, 0, (PVOID)0);

	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	glRotatef(angle, 0.0f, 0.0f, -1.0f);
	glTranslatef((GLfloat)-x, -translateY, 0.0f);
}
//----------------------------------------------------------------------------------
void CGLEngine::GL2_DrawMirrored(const CGLTexture &texture, const int &x, const int &y, const bool &mirror)
{
	WISPFUN_DEBUG("c29_f40");
	BindTexture(texture.Texture);

	int width = texture.Width;
	int height = texture.Height;

	glTranslatef((GLfloat)x, (GLfloat)y, 0.0f);

	if (mirror)
		glBindBuffer(GL_ARRAY_BUFFER, texture.MirroredVertexBuffer);
	else
		glBindBuffer(GL_ARRAY_BUFFER, texture.VertexBuffer);

	glVertexPointer(2, GL_INT, 0, (PVOID)0);

	glBindBuffer(GL_ARRAY_BUFFER, m_PositionBuffer);
	glTexCoordPointer(2, GL_INT, 0, (PVOID)0);

	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	glTranslatef((GLfloat)-x, (GLfloat)-y, 0.0f);
}
//----------------------------------------------------------------------------------
void CGLEngine::GL2_DrawSitting(const CGLTexture &texture, const int &x, const int &y, const bool &mirror, const float &h3mod, const float &h6mod, const float &h9mod)
{
	WISPFUN_DEBUG("c29_f41");
	BindTexture(texture.Texture);
	
	glTranslatef((GLfloat)x, (GLfloat)y, 0.0f);

	float width = (float)texture.Width;
	float height = (float)texture.Height;
	
	float h03 = height * h3mod;
	float h06 = height * h6mod;
	float h09 = height * h9mod;

	float widthOffset = (float)(width + m_SittingCharacterOffset);
	glBegin(GL_TRIANGLE_STRIP);

	if (mirror)
	{	
		if (h3mod)
		{
			glTexCoord2f(0.0f, 0.0f); glVertex2f(width, 0);
			glTexCoord2f(1.0f, 0.0f); glVertex2f(0, 0);
			glTexCoord2f(0.0f, h3mod); glVertex2f(width, h03);
			glTexCoord2f(1.0f, h3mod); glVertex2f(0, h03);
		}

		if (h6mod)
		{
			if (!h3mod)
			{
				glTexCoord2f(0.0f, 0.0f); glVertex2f(width, 0);
				glTexCoord2f(1.0f, 0.0f); glVertex2f(0, 0);
			}

			glTexCoord2f(0.0f, h6mod); glVertex2f(widthOffset, h06);
			glTexCoord2f(1.0f, h6mod); glVertex2f(m_SittingCharacterOffset, h06);
		}

		if (h9mod)
		{
			if (!h6mod)
			{
				glTexCoord2f(0.0f, 0.0f); glVertex2f(widthOffset, 0);
				glTexCoord2f(1.0f, 0.0f); glVertex2f(m_SittingCharacterOffset, 0);
			}

			glTexCoord2f(0.0f, 1.0f); glVertex2f(widthOffset, h09);
			glTexCoord2f(1.0f, 1.0f); glVertex2f(m_SittingCharacterOffset, h09);

		}
	}
	else
	{
		if (h3mod)
		{
			glTexCoord2f(0.0f, 0.0f); glVertex2f(m_SittingCharacterOffset, 0);
			glTexCoord2f(1.0f, 0.0f); glVertex2f(widthOffset, 0);
			glTexCoord2f(0.0f, h3mod); glVertex2f(m_SittingCharacterOffset, h03);
			glTexCoord2f(1.0f, h3mod); glVertex2f(widthOffset, h03);
		}

		if (h6mod)
		{
			if (!h3mod)
			{
				glTexCoord2f(0.0f, 0.0f); glVertex2f(m_SittingCharacterOffset, 0);
				glTexCoord2f(1.0f, 0.0f); glVertex2f(width + m_SittingCharacterOffset, 0);
			}

			glTexCoord2f(0.0f, h6mod); glVertex2f(0, h06);
			glTexCoord2f(1.0f, h6mod); glVertex2f(width, h06);
		}

		if (h9mod)
		{
			if (!h6mod)
			{
				glTexCoord2f(0.0f, 0.0f); glVertex2f(0, 0);
				glTexCoord2f(1.0f, 0.0f); glVertex2f(width, 0);
			}

			glTexCoord2f(0.0f, 1.0f); glVertex2f(0, h09);
			glTexCoord2f(1.0f, 1.0f); glVertex2f(width, h09);
		}

	}

	glEnd();

	glTranslatef((GLfloat)-x, (GLfloat)-y, 0.0f);
}
//----------------------------------------------------------------------------------
void CGLEngine::GL2_DrawShadow(const CGLTexture &texture, const int &x, const int &y, const bool &mirror)
{
	WISPFUN_DEBUG("c29_f42");
	BindTexture(texture.Texture);

	float width = (float)texture.Width;
	float height = texture.Height / 2.0f;

	GLfloat translateY = (GLfloat)(y + height * 0.75);

	glTranslatef((GLfloat)x, translateY, 0.0f);

	float ratio = height / width;
	float verticles[8];

	if (mirror)
	{
		verticles[0] = width;
		verticles[1] = height;
		verticles[2] = 0.0f;
		verticles[3] = height;
		verticles[4] = width * (ratio + 1.0f);
		verticles[5] = 0.0f;
		verticles[6] = width * ratio;
		verticles[7] = 0.0f;
	}
	else
	{
		verticles[0] = 0.0f;
		verticles[1] = height;
		verticles[2] = width;
		verticles[3] = height;
		verticles[4] = width * ratio;
		verticles[5] = 0.0f;
		verticles[6] = width * (ratio + 1.0f);
		verticles[7] = 0.0f;
	}

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glVertexPointer(2, GL_FLOAT, 0, &verticles[0]);

	glBindBuffer(GL_ARRAY_BUFFER, m_PositionBuffer);
	glTexCoordPointer(2, GL_INT, 0, (PVOID)0);

	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	glTranslatef((GLfloat)-x, -translateY, 0.0f);
}
//----------------------------------------------------------------------------------
void CGLEngine::GL2_DrawStretched(const CGLTexture &texture, const int &x, const int &y, const int &drawWidth, const int &drawHeight)
{
	WISPFUN_DEBUG("c29_f43");
	BindTexture(texture.Texture);

	int width = texture.Width;
	int height = texture.Height;

	glTranslatef((GLfloat)x, (GLfloat)y, 0.0f);

	float drawCountX = drawWidth / (float)width;
	float drawCountY = drawHeight / (float)height;

	glBegin(GL_TRIANGLE_STRIP);
		glTexCoord2f(0.0f, drawCountY);			glVertex2i(0, drawHeight);
		glTexCoord2f(drawCountX, drawCountY);	glVertex2i(drawWidth, drawHeight);
		glTexCoord2f(0.0f, 0.0f);				glVertex2i(0, 0);
		glTexCoord2f(drawCountX, 0.0f);			glVertex2i(drawWidth, 0);
	glEnd();

	glTranslatef((GLfloat)-x, (GLfloat)-y, 0.0f);
}
//----------------------------------------------------------------------------------
void CGLEngine::GL2_DrawResizepic(CGLTexture **th, const int &x, const int &y, const int &width, const int &height)
{
	WISPFUN_DEBUG("c29_f44");
	IFOR(i, 0, 9)
	{
		BindTexture(th[i]->Texture);

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

				drawCountY = drawHeight / (float)th[i]->Height;

				break;
			}
			case 4:
			{
				drawX += width - drawWidth;
				drawY += th[2]->Height;

				drawHeight = height - th[2]->Height - th[7]->Height;

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

				drawHeight = height - th[2]->Height - th[7]->Height;

				drawCountX = drawWidth / (float)th[i]->Width;
				drawCountY = drawHeight / (float)th[i]->Height;

				break;
			}
			default:
				break;
		}

		if (drawWidth < 1 || drawHeight < 1)
			continue;

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
//----------------------------------------------------------------------------------
