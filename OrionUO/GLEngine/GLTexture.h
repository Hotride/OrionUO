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
typedef vector<uchar> HIT_MAP_TYPE;
//----------------------------------------------------------------------------------
class CGLTexture
{
public:
	//!Габариты текстуры
	short Width = 0;
	short Height = 0;

	short ImageOffsetX = 0;
	short ImageOffsetY = 0;

	short ImageWidth = 0;
	short ImageHeight = 0;

	//!Буфер вершин
	GLuint VertexBuffer = 0;

	//!Буфер вершин для зеркального отображения анимации
	GLuint MirroredVertexBuffer = 0;

	CGLTexture();
	virtual ~CGLTexture();

	GLuint Texture{ 0 };

	HIT_MAP_TYPE m_HitMap;

	virtual void Draw(const int &x, const int &y, const bool &checktrans = false);
	virtual void Draw(const int &x, const int &y, int width, int height, const bool &checktrans = false);

	virtual void DrawRotated(const int &x, const int &y, const float &angle);

	virtual void DrawTransparent(const int &x, const int &y, const bool &stencil = true);

	virtual bool Select(int x, int y, const bool &pixelCheck = true);

	virtual void Clear();
};
//----------------------------------------------------------------------------------
#endif
//----------------------------------------------------------------------------------
