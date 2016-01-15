/****************************************************************************
**
** GLEngine.h
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
#ifndef GLEngineH
#define GLEngineH
//---------------------------------------------------------------------------
class TVector;
//---------------------------------------------------------------------------
class TGLEngine : public IGLEngine
{
private:
	HDC m_DC;
	HGLRC m_RC;

	bool GLSetupPixelFormat();

public:
	TGLEngine() {m_DC = NULL; m_RC = NULL;}
	~TGLEngine() {}
	
	HDC GetDC() const {return m_DC;}
	HGLRC GetRC() const {return m_RC;}
	GLuint tex;
	bool Install(HWND hWnd);

	void Uninstall() {wglMakeCurrent(NULL, NULL); wglDeleteContext(m_RC);}

	void UpdateRect();
	
	void BindTexture(GLuint &texture, int width, int height, PDWORD pixels);

	void BeginDraw();
	void EndDraw();
	
	void BeginStencil();
	void EndStencil();

	void EnableAlpha();
	void DisableAlpha();

	void ViewPort(int x, int y, int sizeX, int sizeY);
	void RestorePort();

	void Enable(GLenum val) {glEnable(val);}
	void Disable(GLenum val) {glDisable(val);}

	void DrawLine(DWORD color, float x, float y, float targetX, float targetY);
	void DrawPolygone(DWORD color, float x, float y, float width, float height);
	void DrawTriangle(DWORD color, float x, float y, float radius);

	void DrawLandTexture(GLuint &texture, float x, float y, float width, float height, RECT &rc, TVector *normals);
	void Draw(GLuint &texture, float x, float y, float width, float height);
	void Draw(GLuint &texture, float x, float y, float width, float height, bool &mirror);
	void Draw(GLuint &texture, float x, float y, float width, float height, float drawWidth, float drawHeight);
	void DrawResizepic(TTextureObject **th, float x, float y, float width, float height);
};

extern TGLEngine g_GL;
//---------------------------------------------------------------------------
#endif