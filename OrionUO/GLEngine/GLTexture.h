/***********************************************************************************
**
** GLTexture.h
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#ifndef GLTEXTURE_H
#define GLTEXTURE_H
//----------------------------------------------------------------------------------
#define UO_ENABLE_TEXTURE_DATA_SAVING 0
//----------------------------------------------------------------------------------
#include "../Wisp/WispGlobal.h"
#include "../Wisp/WispGeometry.h"
#include "glew.h"
//----------------------------------------------------------------------------------
class CGLTexture
{
	//!Габариты текстуры
	SETGET(int, Width);
	SETGET(int, Height);

	//!Буфер вершин
	SETGET(GLuint, VertexBuffer);

	//!Буфер вершин для зеркального отображения анимации
	SETGET(GLuint, MirroredVertexBuffer);

public:
	CGLTexture();
	virtual ~CGLTexture();

	GLuint Texture;

#if UO_ENABLE_TEXTURE_DATA_SAVING == 1
	vector<ushort> PixelsData;
#endif

	virtual void Draw(const int &x, const int &y, const bool &checktrans = false);
	virtual void Draw(const int &x, const int &y, int width, int height, const bool &checktrans = false);

	virtual void DrawRotated(const int &x, const int &y, const float &angle);

	virtual void DrawTransparent(const int &x, const int &y, const bool &stencil = true);

	virtual void Clear();
};
//----------------------------------------------------------------------------------
#endif