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
class TGLEngine
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

	SETGET(GLuint, OldTexture);

	//Инициализация OpenGL
	bool Install(HWND hWnd);

	//Завершение работы с OpenGL
	void Uninstall() {wglMakeCurrent(NULL, NULL); wglDeleteContext(m_RC);}

	//Обновление области вывода
	void UpdateRect();

	//Загрузка текстур 16 и 32 бит
	void BindTexture16(GLuint &texture, const int &width, const int &height, PWORD pixels);
	void BindTexture32(GLuint &texture, const int &width, const int &height, PDWORD pixels);

	//Очистить экран и начать рисование
	void BeginDraw();

	//Завершить рисование и вывести все нарисованное на экран
	void EndDraw();

	//Включить буфер трафарета
	void BeginStencil();

	//Выключить буфер трафарета
	void EndStencil();

	//Указать область рисования
	void ViewPort(const int &x, const int &y, const int &width, const int &height);

	//Восстановить стандартную область рисования
	void RestorePort();

	//Указать область рисования (ножницами, сохраняет мартицу)
	void Scissor(const int &x, const int &y, const int &width, const int &height);



	//Нарисовать линию
	void DrawLine(const int &x, const int &y, const int &targetX, const int &targetY);

	//Нарисовать прямоугольник
	void DrawPolygone(const int &x, const int &y, const int &width, const int &height);

	//Нарисовать круг
	void DrawCircle(const float &x, const float &y, const float &radius, const int &gradientMode = 0);



	//Нарисовать текстуру ландшафта
	void DrawLandTexture(GLuint &texture, const int &x, const int &y, RECT &rc, TVector *normals);

	//Нарисовать текстуру
	void Draw(GLuint &texture, const int &x, const int &y, const int &width, const int &height);

	//Нарисовать повернутую текстуру
	void DrawRotated(GLuint &texture, const int &x, const int &y, const int &width, const int &height, const float &angle);

	//Нарисовать текстуру с возможностью зеркального отражения
	void Draw(GLuint &texture, const int &x, const int &y, const int &width, const int &height, const bool &mirror);

	//Нарисовать текстуру сидячего персонажа
	void DrawSitting(GLuint &texture, const int &x, const int &y, const int &width, const int &height, const bool &mirror, bool twistTexture = true);

	//Нарисовать тень
	void DrawShadow(GLuint &texture, const int &x, const int &y, const int &width, const int &height, const bool &mirror);

	//Замостить текстуру на заданные габариты
	void Draw(GLuint &texture, const int &x, const int &y, const int &width, const int &height, const int &drawWidth, const int &drawHeight);

	//Нарисовать фон
	void DrawResizepic(TTextureObject **th, const int &x, const int &y, const int &width, const int &height);
};
//---------------------------------------------------------------------------
extern TGLEngine g_GL;
//---------------------------------------------------------------------------
#endif