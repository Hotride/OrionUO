/***********************************************************************************
**
** GLTextTexture.h
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#ifndef GLTEXTTEXTURE_H
#define GLTEXTTEXTURE_H
//----------------------------------------------------------------------------------
#include "GLTexture.h"
//----------------------------------------------------------------------------------
//Структура данных для хранения информации о веб-ссылке
struct WEB_LINK_RECT
{
	//Индекс ссылки
	ushort LinkID;

	//Начало ссылки относительно начальных координат текста
	int StartX;
	int StartY;

	//Конец ссылки относительно начальных координат текста
	int EndX;
	int EndY;
};
//----------------------------------------------------------------------------------
class CGLTextTexture : public CGLTexture
{
	SETGET(int, LinesCount);

public:
	CGLTextTexture();
	virtual ~CGLTextTexture();

	//Чтоб не было конфликта с классом-родителем
#if UO_ENABLE_TEXTURE_DATA_SAVING == 0
	vector<ushort> PixelsData;
#endif

	bool Empty() { return (Texture == 0); }

	virtual void Clear();

	//Отрисовать текстуру
	virtual void Draw(const int &x, const int &y, const bool &checktrans = false);

	//Проверка на наличие текстуры под мышкой
	bool UnderMouse(int x, int y);

	//Очистка веб-ссылки
	virtual void ClearWebLink() {}

	//Добавиление веб-ссылки
	virtual void AddWebLink(WEB_LINK_RECT &wl) {}

	//Проверка веб-ссылки под мышкой
	virtual ushort WebLinkUnderMouse(int x, int y) { return 0; }
};
//----------------------------------------------------------------------------------
#endif