/***********************************************************************************
**
** GLEngine.h
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#ifndef GLENGINE_H
#define GLENGINE_H
//----------------------------------------------------------------------------------
#include "../Globals.h"
#include "../Wisp/WispLogger.h"
#include "../Wisp/WispGeometry.h"
#include "GLTexture.h"
#include "GLVector.h"
//----------------------------------------------------------------------------------
typedef deque<WISP_GEOMETRY::CRect> SCISSOR_LIST;
//----------------------------------------------------------------------------------
class CGLEngine
{
	SETGET(HDC, DC);
	SETGET(HGLRC, RC);
	SETGET(GLuint, OldTexture);
	SETGET(bool, Drawing);
	SETGET(bool, CanUseFrameBuffer);
	SETGET(float, SittingCharacterOffset);

private:
	bool GLSetupPixelFormat();

	SCISSOR_LIST m_ScissorList;

public:
	CGLEngine();
	virtual ~CGLEngine();
	
	//Инициализация OpenGL
	bool Install();

	//Завершение работы с OpenGL
	void Uninstall();

	//Обновление области вывода
	void UpdateRect();

	//Загрузка текстур 16 и 32 бит
	void BindTexture16(CGLTexture &texture, const int &width, const int &height, const pushort pixels);
	void BindTexture32(CGLTexture &texture, const int &width, const int &height, const puint pixels);

	//Очистить экран и начать рисование
	void BeginDraw();

	//Завершить рисование и вывести все нарисованное на экран
	void EndDraw();

	//Включить буфер трафарета
	void BeginStencil();

	//Выключить буфер трафарета
	void EndStencil();

	//Указать область рисования
	void ViewPortScaled(const int &x, const int &y, const int &width, const int &height);

	//Указать область рисования
	void ViewPort(const int &x, const int &y, const int &width, const int &height);

	//Восстановить стандартную область рисования
	void RestorePort();

	//Указать область рисования (ножницами, сохраняет мартицу)
	void PushScissor(const int &x, const int &y, const int &width, const int &height);
	void PushScissor(const WISP_GEOMETRY::CPoint2Di &position, const int &width, const int &height);
	void PushScissor(const int &x, const int &y, const WISP_GEOMETRY::CSize &size);
	void PushScissor(const WISP_GEOMETRY::CPoint2Di &position, const WISP_GEOMETRY::CSize &size);
	void PushScissor(const WISP_GEOMETRY::CRect &rect);

	void PopScissor();

	void ClearScissorList();

	inline void BindTexture(const GLuint &texture);



	//Нарисовать линию
	void DrawLine(const int &x, const int &y, const int &targetX, const int &targetY);

	//Нарисовать прямоугольник
	void DrawPolygone(const int &x, const int &y, const int &width, const int &height);

	//Нарисовать круг
	void DrawCircle(const float &x, const float &y, const float &radius, const int &gradientMode = 0);



	//Нарисовать текстуру ландшафта
	void DrawLandTexture(const GLuint &texture, const int &x, const int &y, const RECT &rc, CVector *normals);

	//Нарисовать текстуру
	void Draw(const CGLTexture &texture, const int &x, const int &y);

	//Нарисовать повернутую текстуру
	void DrawRotated(const CGLTexture &texture, const int &x, const int &y, const float &angle);

	//Нарисовать текстуру с возможностью зеркального отражения
	void Draw(const CGLTexture &texture, const int &x, const int &y, const bool &mirror);

	//Нарисовать текстуру сидячего персонажа
	void DrawSitting(const CGLTexture &texture, const int &x, const int &y, const bool &mirror, const float &h3mod, const float &h6mod, const float &h9mod);

	//Нарисовать тень
	void DrawShadow(const CGLTexture &texture, const int &x, const int &y, const bool &mirror);

	//Замостить текстуру на заданные габариты
	void Draw(const CGLTexture &texture, const int &x, const int &y, const int &drawWidth, const int &drawHeight);

	//Нарисовать фон
	void DrawResizepic(CGLTexture **th, const int &x, const int &y, const int &width, const int &height);
};
//---------------------------------------------------------------------------
extern CGLEngine g_GL;
//---------------------------------------------------------------------------
#endif