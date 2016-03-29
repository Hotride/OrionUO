/****************************************************************************
**
** ColorManager.h
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
#ifndef ColorManagerH
#define ColorManagerH
//---------------------------------------------------------------------------
//Структура цветов для отправки в шейдер
struct INT_HUES
{
	GLint Palette[32];
};
//---------------------------------------------------------------------------
//Менеджер цветов
class TColorManager : public IColorManager
{
private:
	//Указатель на палитру
	PHUES_GROUP m_HuesRange;

	//Указатель на список цветов для шейдера
	INT_HUES *m_HuesInt;

	//Количество цветов
	int m_HuesCount;

	//Данные для радара
	WORD m_Radarcol[0x10000];

	//Данные из palette.mul
	PALETTE_BLOCK m_Palette[256];

public:
	TColorManager();
	~TColorManager();

	PHUES_GROUP GetHuesRangePointer() { return m_HuesRange; }
	int GetHuesCount() const {return m_HuesCount;}

	//Патч блока цветов из вердаты
	void SetHuesBlock(int index, PVERDATA_HUES_GROUP group);

	//Создание палитры цветов для шейдера
	void CreateHuesPalette();

	//Отправка цветов в шейдер
	void SendColorsToShader(WORD &color);

	//Конвертирование цветов
	WORD Color32To16(DWORD &c);
	DWORD Color16To32(WORD &c);
	WORD ConvertToGray(WORD &c);

	//Получить цвет
	WORD GetColor16(WORD &c, WORD color);
	WORD GetRadarColorData(WORD &c);
	DWORD GetRadarColor(WORD &c);
	DWORD GetPolygoneColor(WORD c, WORD color);
	DWORD GetUnicodeFontColor(WORD &c, WORD color);
	DWORD GetColor(WORD &c, WORD color);
	DWORD GetPartialHueColor(WORD &c, WORD color);
};
//---------------------------------------------------------------------------
extern TColorManager *ColorManager;
//---------------------------------------------------------------------------
#endif