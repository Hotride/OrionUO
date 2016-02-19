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
//Класс по работе с функциями OpenGL
class TGLEngine : public IGLEngine
{
private:
	//Контекст устройства
	HDC m_DC;

	//Контекст OpenGL
	HGLRC m_RC;

	//Установка формата вывода
	bool GLSetupPixelFormat();

	//Индекс последней использованной текстуры
	GLuint m_OldTexture;

public:
	TGLEngine() {m_DC = NULL; m_RC = NULL;}
	~TGLEngine() {}
	
	HDC GetDC() const {return m_DC;}
	HGLRC GetRC() const {return m_RC;}
	GLuint tex;

	//Инициализация OpenGL
	bool Install(HWND hWnd);

	//Завершение работы с OpenGL
	void Uninstall() {wglMakeCurrent(NULL, NULL); wglDeleteContext(m_RC);}

	//Обновление области вывода
	void UpdateRect();

	//Загрузка текстур 16 и 32 бит
	void BindTexture16(GLuint &texture, int width, int height, PWORD pixels);
	void BindTexture32(GLuint &texture, int width, int height, PDWORD pixels);

	//Очистить экран и начать рисование
	void BeginDraw();

	//Завершить рисование и вывести все нарисованное на экран
	void EndDraw();

	//Включить буфер трафарета
	void BeginStencil();

	//Выключить буфер трафарета
	void EndStencil();

	//Включить альфа-тест
	void EnableAlpha();

	//Выключить альфа-тест
	void DisableAlpha();

	//Указать область рисования
	void ViewPort(int x, int y, int sizeX, int sizeY);

	//Восстановить стандартную область рисования
	void RestorePort();

	//Включить режим
	void Enable(GLenum val) {glEnable(val);}

	//Выключить режим
	void Disable(GLenum val) {glDisable(val);}



	Нарисовать линию
	void DrawLine(DWORD color, float x, float y, float targetX, float targetY);

	//Нарисовать прямоугольник
	void DrawPolygone(DWORD color, float x, float y, float width, float height);

	//Нарисовать круг
	void DrawTriangle(DWORD color, float x, float y, float radius);



	//Нарисовать текстуру ландшафта
	void DrawLandTexture(GLuint &texture, int &x, int &y, RECT &rc, TVector *normals);

	//Нарисовать текстуру
	void Draw(GLuint &texture, int &x, int &y, int width, int height);

	//Нарисовать текстуру с возможностью зеркального отражения
	void Draw(GLuint &texture, int &x, int &y, int width, int height, bool &mirror);

	//Нарисовать текстуру сидячего персонажа
	void DrawSitting(GLuint &texture, int &x, int &y, int width, int height, bool &mirror);

	//Нарисовать тень
	void DrawShadow(GLuint &texture, int &x, int &y, int width, int height, bool &mirror);

	//Замостить текстуру на заданные габариты
	void Draw(GLuint &texture, int &x, int &y, int width, int height, int &drawWidth, int &drawHeight);

	//Нарисовать фон
	void DrawResizepic(TTextureObject **th, int &x, int &y, int &width, int &height);
};
//---------------------------------------------------------------------------
extern TGLEngine g_GL;
//---------------------------------------------------------------------------
#endif