/***********************************************************************************
**
** GLTextureCircleOfTransparency.h
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#ifndef GLTEXTURECIRCLEOFTRANSPARENCY_H
#define GLTEXTURECIRCLEOFTRANSPARENCY_H
//----------------------------------------------------------------------------------
#include "GLTexture.h"
//----------------------------------------------------------------------------------
class CGLTextureCircleOfTransparency : public CGLTexture
{
	SETGET(int, Radius, 0);
	SETGET(int, X, 0);
	SETGET(int, Y, 0);

public:
	CGLTextureCircleOfTransparency();
	virtual ~CGLTextureCircleOfTransparency();

	//Создать текстуру
	static void CreatePixels(const int &radius, int &width, int &height, UINT_LIST &pixels);

	//Создать текстуру
	bool Create(int radius);

	//Отрисовать текстуру
	virtual void Draw(const int &x, const int &y, const bool &checktrans = false);

	//Отрисовать текстуру
	void Redraw();

#if UO_ENABLE_TEXTURE_DATA_SAVING == 0
	vector<ushort> PixelsData;
#endif
};
//----------------------------------------------------------------------------------
extern CGLTextureCircleOfTransparency g_CircleOfTransparency;
//----------------------------------------------------------------------------------
#endif
