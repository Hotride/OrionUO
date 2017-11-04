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
typedef deque<WISP_GEOMETRY::CRect> SCISSOR_LIST;
//----------------------------------------------------------------------------------
class CGLEngine;
//----------------------------------------------------------------------------------
typedef void (CGLEngine::*BIND_TEXTURE_16_FUNCTION)(CGLTexture&, const int&, const int&, pushort);
typedef void (CGLEngine::*BIND_TEXTURE_32_FUNCTION)(CGLTexture&, const int&, const int&, puint);

typedef void (CGLEngine::*DRAW_LAND_TEXTURE_FUNCTION)(const CGLTexture&, const int&, const int&, class CLandObject*);
typedef void (CGLEngine::*DRAW_TEXTURE_FUNCTION)(const CGLTexture&, const int&, const int&);
typedef void (CGLEngine::*DRAW_TEXTURE_ROTATED_FUNCTION)(const CGLTexture&, const int&, const int&, const float&);
typedef void (CGLEngine::*DRAW_TEXTURE_MIRRORED_FUNCTION)(const CGLTexture&, const int&, const int&, const bool&);
typedef void (CGLEngine::*DRAW_TEXTURE_SITTING_FUNCTION)(const CGLTexture&, const int&, const int&, const bool&, const float&, const float&, const float&);
typedef void (CGLEngine::*DRAW_TEXTURE_SHADOW_FUNCTION)(const CGLTexture&, const int&, const int&, const bool&);
typedef void (CGLEngine::*DRAW_TEXTURE_STRETCHED_FUNCTION)(const CGLTexture&, const int&, const int&, const int&, const int&);
typedef void (CGLEngine::*DRAW_TEXTURE_RESIZEPIC_FUNCTION)(CGLTexture**, const int&, const int&, const int&, const int&);
//----------------------------------------------------------------------------------
//Указатели на функции рисования, в соответствии с текущим контекстом устройства
//Загрузка текстур 16 и 32 бит
extern BIND_TEXTURE_16_FUNCTION g_GL_BindTexture16_Ptr;
extern BIND_TEXTURE_32_FUNCTION g_GL_BindTexture32_Ptr;

//Нарисовать текстуру ландшафта
extern DRAW_LAND_TEXTURE_FUNCTION g_GL_DrawLandTexture_Ptr;

//Нарисовать текстуру
extern DRAW_TEXTURE_FUNCTION g_GL_Draw_Ptr;

//Нарисовать повернутую текстуру
extern DRAW_TEXTURE_ROTATED_FUNCTION g_GL_DrawRotated_Ptr;

//Нарисовать текстуру с возможностью зеркального отражения
extern DRAW_TEXTURE_MIRRORED_FUNCTION g_GL_DrawMirrored_Ptr;

//Нарисовать текстуру сидячего персонажа
extern DRAW_TEXTURE_SITTING_FUNCTION g_GL_DrawSitting_Ptr;

//Нарисовать тень
extern DRAW_TEXTURE_SHADOW_FUNCTION g_GL_DrawShadow_Ptr;

//Замостить текстуру на заданные габариты
extern DRAW_TEXTURE_STRETCHED_FUNCTION g_GL_DrawStretched_Ptr;

//Нарисовать фон
extern DRAW_TEXTURE_RESIZEPIC_FUNCTION g_GL_DrawResizepic_Ptr;
//----------------------------------------------------------------------------------
#define g_GL_BindTexture16 (g_GL.*(g_GL_BindTexture16_Ptr))
#define g_GL_BindTexture32 (g_GL.*(g_GL_BindTexture32_Ptr))

#define g_GL_DrawLandTexture (g_GL.*(g_GL_DrawLandTexture_Ptr))
#define g_GL_Draw (g_GL.*(g_GL_Draw_Ptr))
#define g_GL_DrawRotated (g_GL.*(g_GL_DrawRotated_Ptr))
#define g_GL_DrawMirrored (g_GL.*(g_GL_DrawMirrored_Ptr))
#define g_GL_DrawSitting (g_GL.*(g_GL_DrawSitting_Ptr))
#define g_GL_DrawShadow (g_GL.*(g_GL_DrawShadow_Ptr))
#define g_GL_DrawStretched (g_GL.*(g_GL_DrawStretched_Ptr))
#define g_GL_DrawResizepic (g_GL.*(g_GL_DrawResizepic_Ptr))
//----------------------------------------------------------------------------------
class CGLEngine
{
	SETGET(HDC, DC, 0);
	SETGET(HGLRC, RC, 0);
	SETGET(GLuint, OldTexture, 0);
	SETGET(bool, Drawing, false);
	SETGET(bool, CanUseFrameBuffer, false);
	SETGET(bool, CanUseBuffer, false);
	SETGET(float, SittingCharacterOffset, 8.0f);
	SETGET(GLuint, PositionBuffer, 0);
	SETGET(bool, IgnoreHitMap, false);

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



	//Функции OpenGL 1x

	//Загрузка текстур 16 и 32 бит
	void GL1_BindTexture16(CGLTexture &texture, const int &width, const int &height, pushort pixels);
	void GL1_BindTexture32(CGLTexture &texture, const int &width, const int &height, puint pixels);

	//Нарисовать текстуру ландшафта
	void GL1_DrawLandTexture(const CGLTexture &texture, const int &x, const int &y, CLandObject *land);

	//Нарисовать текстуру
	void GL1_Draw(const CGLTexture &texture, const int &x, const int &y);

	//Нарисовать повернутую текстуру
	void GL1_DrawRotated(const CGLTexture &texture, const int &x, const int &y, const float &angle);

	//Нарисовать текстуру с возможностью зеркального отражения
	void GL1_DrawMirrored(const CGLTexture &texture, const int &x, const int &y, const bool &mirror);

	//Нарисовать текстуру сидячего персонажа
	void GL1_DrawSitting(const CGLTexture &texture, const int &x, const int &y, const bool &mirror, const float &h3mod, const float &h6mod, const float &h9mod);

	//Нарисовать тень
	void GL1_DrawShadow(const CGLTexture &texture, const int &x, const int &y, const bool &mirror);

	//Замостить текстуру на заданные габариты
	void GL1_DrawStretched(const CGLTexture &texture, const int &x, const int &y, const int &drawWidth, const int &drawHeight);

	//Нарисовать фон
	void GL1_DrawResizepic(CGLTexture **th, const int &x, const int &y, const int &width, const int &height);



	//Функции OpenGL 2x

	void GL2_CreateArrays(CGLTexture &texture, const int &width, const int &height);

	//Загрузка текстур 16 и 32 бит
	void GL2_BindTexture16(CGLTexture &texture, const int &width, const int &height, pushort pixels);
	void GL2_BindTexture32(CGLTexture &texture, const int &width, const int &height, puint pixels);

	//Нарисовать текстуру ландшафта
	void GL2_DrawLandTexture(const CGLTexture &texture, const int &x, const int &y, CLandObject *land);

	//Нарисовать текстуру
	void GL2_Draw(const CGLTexture &texture, const int &x, const int &y);

	//Нарисовать повернутую текстуру
	void GL2_DrawRotated(const CGLTexture &texture, const int &x, const int &y, const float &angle);

	//Нарисовать текстуру с возможностью зеркального отражения
	void GL2_DrawMirrored(const CGLTexture &texture, const int &x, const int &y, const bool &mirror);

	//Нарисовать текстуру сидячего персонажа
	void GL2_DrawSitting(const CGLTexture &texture, const int &x, const int &y, const bool &mirror, const float &h3mod, const float &h6mod, const float &h9mod);

	//Нарисовать тень
	void GL2_DrawShadow(const CGLTexture &texture, const int &x, const int &y, const bool &mirror);

	//Замостить текстуру на заданные габариты
	void GL2_DrawStretched(const CGLTexture &texture, const int &x, const int &y, const int &drawWidth, const int &drawHeight);

	//Нарисовать фон
	void GL2_DrawResizepic(CGLTexture **th, const int &x, const int &y, const int &width, const int &height);
};
//---------------------------------------------------------------------------
extern CGLEngine g_GL;
//---------------------------------------------------------------------------
#endif