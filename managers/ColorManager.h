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
//!Структура цветов для отправки в шейдер
struct INT_HUES
{
	//!Палитра цветов, 32 штуки
	GLint Palette[32];
};
//---------------------------------------------------------------------------
//!Кдасс менеджера цветов
class TColorManager
{
private:
	//!Указатель на палитру
	PHUES_GROUP m_HuesRange;

	//!Указатель на список цветов для шейдера
	INT_HUES *m_HuesInt;

	//!Количество цветов
	int m_HuesCount;

	//!Данные для радара
	WORD m_Radarcol[0x10000];

	//!Данные из palette.mul
	PALETTE_BLOCK m_Palette[256];

public:
	TColorManager();
	~TColorManager();

	/*!
	Получить указатель на начало цветов
	@return Ссылка на цвета
	*/
	PHUES_GROUP GetHuesRangePointer() { return m_HuesRange; }

	/*!
	Получить количество цветов
	@return Количество цветов
	*/
	int GetHuesCount() const { return m_HuesCount; }

	/*!
	Патч блока цветов из вердаты
	@param [__in] index Индекс в списке
	@param [__in] group Указатель на цвета патча
	@return 
	*/
	void SetHuesBlock(__in int index, __in PVERDATA_HUES_GROUP group);

	/*!
	Создание палитры цветов для шейдера
	@return 
	*/
	void CreateHuesPalette();

	/*!
	Отправка цветов в шейдер
	@param [__in] color Индекс цвета
	@return 
	*/
	void SendColorsToShader(__in WORD color);



	/*!
	Конвертирование цвета из 32 бит в 16 бит
	@param [__in] c 32-битный цвет
	@return 16-битный цвет
	*/
	WORD Color32To16(__in DWORD &c);

	/*!
	Конвертирование цвета из 16 бит в 32 бит
	@param [__in] c 16-битный цвет
	@return 32-битный цвет
	*/
	DWORD Color16To32(__in WORD &c);

	/*!
	Перевод в серый
	@param [__in] c 16-битный цвет
	@return 16-битный цвет
	*/
	WORD ConvertToGray(__in WORD &c);



	/*!
	Получить 16-битный цвет
	@param [__in] c Исходный цвет
	@param [__in] color Индекс цвета в палитре
	@return 16-битный цвет
	*/
	WORD GetColor16(__in WORD &c, __in WORD color);

	/*!
	Получить 16-битный цвет для радара
	@param [__in] c Исходный 16-битный цвет
	@return 16-битный цвет
	*/
	WORD GetRadarColorData(__in WORD &c);

	/*!
	Получить 32-битный цвет для радара
	@param [__in] c Исходный 16-битный цвет
	@return 32-битный цвет
	*/
	DWORD GetRadarColor(__in WORD &c);

	/*!
	Получить 32-битный цвет без конвертирования входящего цвета
	@param [__in] c Исходный 16-битный цвет
	@param [__in] color Индекс цвета в палитре
	@return 32-битный цвет
	*/
	DWORD GetPolygoneColor(__in WORD c, __in WORD color);

	/*!
	Получить 32-битный цвет для Unicode-шрифтов
	@param [__in] c Исходный 16-битный цвет
	@param [__in] color Индекс цвета в палитре
	@return 32-битный цвет
	*/
	DWORD GetUnicodeFontColor(__in WORD &c, __in WORD color);

	/*!
	Получить 32-битный цвет
	@param [__in] c Исходный 16-битный цвет
	@param [__in] color Индекс цвета в палитре
	@return 32-битный цвет
	*/
	DWORD GetColor(__in WORD &c, __in WORD color);

	/*!
	Получить 32-битный цвет с учетом оттенков серого
	@param [__in] c Исходный 16-битный цвет
	@param [__in] color Индекс цвета в палитре
	@return 32-битный цвет
	*/
	DWORD GetPartialHueColor(__in WORD &c, __in WORD color);
 };
//---------------------------------------------------------------------------
//!Ссылка на менеджер цветов
extern TColorManager *ColorManager;
//---------------------------------------------------------------------------
#endif