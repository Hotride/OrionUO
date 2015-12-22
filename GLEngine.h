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

	bool Install(HWND hWnd);

	void Uninstall() {wglMakeCurrent(NULL, NULL); wglDeleteContext(m_RC);}

	void UpdateRect();
	
	void BindTexture(GLuint &Texture, int Width, int Height, PDWORD pixels);

	void BeginDraw();
	void EndDraw();

	void EnableAlpha();
	void DisableAlpha();

	void ViewPort(int X, int Y, int SizeX, int SizeY);
	void RestorePort();

	void Enable(GLenum val) {glEnable(val);}
	void Disable(GLenum val) {glDisable(val);}

	void DrawLine(DWORD Color, float X, float Y, float TargetX, float TargetY);
	void DrawPolygone(DWORD Color, float X, float Y, float Width, float Height);
	void DrawTriangle(DWORD Color, float X, float Y, float radius);

	void DrawLandTexture(GLuint &Texture, float X, float Y, float Width, float Height, RECT &rc, TVector *normals);
	void Draw(GLuint &Texture, float X, float Y, float Width, float Height);
	void Draw(GLuint &Texture, float X, float Y, float Width, float Height, bool Mirror);
	void Draw(GLuint &Texture, float X, float Y, float Width, float Height, float DrawWidth, float DrawHeight);
	void DrawResizepic(TTextureObject **th, float X, float Y, float W, float H);
};

extern TGLEngine g_GL;
//---------------------------------------------------------------------------
#endif