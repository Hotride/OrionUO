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
class CGLTextureCircleOfTransparency : public CGLTexture
{
public:
	int Radius = 0;
	int X = 0;
	int Y = 0;

	CGLTextureCircleOfTransparency();
	virtual ~CGLTextureCircleOfTransparency();

	//Создать текстуру
	static void CreatePixels(const int &radius, short &width, short &height, UINT_LIST &pixels);

	//Создать текстуру
	bool Create(int radius);

	//Отрисовать текстуру
	virtual void Draw(const int &x, const int &y, const bool &checktrans = false);

	//Отрисовать текстуру
	void Redraw();
};
//----------------------------------------------------------------------------------
extern CGLTextureCircleOfTransparency g_CircleOfTransparency;
//----------------------------------------------------------------------------------
#endif
