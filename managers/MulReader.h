/****************************************************************************
**
** MulReader.h
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
#ifndef MulReaderH
#define MulReaderH
//---------------------------------------------------------------------------
//!Класс для чтения данных из файлов и создания текстур
class TMulReader
{
public:
	TMulReader() {}
	virtual ~TMulReader() {}

	/*!
	Получить массив пикселей гампа
	@param [__in] io Ссылка на данные о гампе
	@return Массив пикселей или NULL
	*/
	virtual PWORD GetGumpPixels(__in TIndexObject &io);

	/*!
	Прочитать гамп и сгенерировать текстуру
	@param [__in] io Ссылка на данные о гампе
	@return Ссылка на данные о текстуре
	*/
	virtual TTextureObject *ReadGump(__in TIndexObject &io);

	/*!
	Прочитать арт и сгенерировать текстуру
	@param [__in] ID Индекс арта
	@param [__in] io Ссылка на данные о арте
	@return Ссылка на данные о текстуре
	*/
	virtual TTextureObject *ReadArt(__in const WORD &id, __in TIndexObject &io);

	/*!
	Прочитать арт и вычислить реальные пииксельные границы картинки
	@param [__in] io Ссылка на данные о арте
	@param [__out] r Структура с габаритами на выходе
	@return Ссылка на данные о текстуре
	*/
	virtual void ReadStaticArtPixelDimension(__in TIndexObject &io, __out RECT &r);

	/*!
	Прочитать текстуру ландшафта и сгенерировать тексруту
	@param [__in] io Ссылка на данные о текстуре ландшафта
	@return Ссылка на данные о текстуре
	*/
	virtual TTextureObject *ReadTexture(__in TIndexObject &io);

	/*!
	Прочитать освещение и сгенерировать текстуру
	@param [__in] io Ссылка на данные о освещении
	@return Ссылка на данные о текстуре
	*/
	TTextureObject *ReadLight(__in TIndexObject &io);

	/*!
	Проверить нахождение пикселя гампа в указанных координатах
	@param [__in] io Ссылка на данные о гампе
	@param [__in] checkX Координата X
	@param [__in] checkY Координата Y
	@return true в случае успешного теста
	*/
	virtual bool GumpPixelsInXY(__in TIndexObject &io, __in const int &checkX, __in const int &checkY);

	/*!
	Проверить нахождение пикселя арта в указанных координатах
	@param [__in] land Ландшафт или статика
	@param [__in] io Ссылка на данные о арте
	@param [__in] checkX Координата X
	@param [__in] checkY Координата Y
	@return true в случае успешного теста
	*/
	virtual bool ArtPixelsInXY(__in const bool &land, __in TIndexObject &io, __in const int &checkX, __in const int &checkY);
};
//---------------------------------------------------------------------------
//!Класс для чтения данных из файлов и создания текстур
class TUopReader : public TMulReader
{
public:
	TUopReader() : TMulReader() {}
	virtual ~TUopReader() {}

	/*!
	Получить массив пикселей гампа
	@param [__in] io Ссылка на данные о гампе
	@return Массив пикселей или NULL
	*/
	virtual PWORD GetGumpPixels(__in TIndexObject &io);

	/*!
	Прочитать гамп и сгенерировать текстуру
	@param [__in] io Ссылка на данные о гампе
	@return Ссылка на данные о текстуре
	*/
	virtual TTextureObject *ReadGump(__in TIndexObject &io);

	/*!
	Прочитать арт и сгенерировать текстуру
	@param [__in] ID Индекс арта
	@param [__in] io Ссылка на данные о арте
	@return Ссылка на данные о текстуре
	*/
	virtual TTextureObject *ReadArt(__in const WORD &id, __in TIndexObject &io);

	/*!
	Прочитать арт и вычислить реальные пииксельные границы картинки
	@param [__in] io Ссылка на данные о арте
	@param [__out] r Структура с габаритами на выходе
	@return Ссылка на данные о текстуре
	*/
	virtual void ReadStaticArtPixelDimension(__in TIndexObject &io, __out RECT &r);

	/*!
	Прочитать текстуру ландшафта и сгенерировать тексруту
	@param [__in] io Ссылка на данные о текстуре ландшафта
	@return Ссылка на данные о текстуре
	*/
	virtual TTextureObject *ReadTexture(__in TIndexObject &io);

	/*!
	Проверить нахождение пикселя гампа в указанных координатах
	@param [__in] io Ссылка на данные о гампе
	@param [__in] checkX Координата X
	@param [__in] checkY Координата Y
	@return true в случае успешного теста
	*/
	virtual bool GumpPixelsInXY(__in TIndexObject &io, __in const int &checkX, __in const int &checkY);

	/*!
	Проверить нахождение пикселя арта в указанных координатах
	@param [__in] land Ландшафт или статика
	@param [__in] io Ссылка на данные о арте
	@param [__in] checkX Координата X
	@param [__in] checkY Координата Y
	@return true в случае успешного теста
	*/
	virtual bool ArtPixelsInXY(__in const bool &land, __in TIndexObject &io, __in const int &checkX, __in const int &checkY);
};
//---------------------------------------------------------------------------
//!Класс чтения данных
extern TMulReader *MulReader;
//---------------------------------------------------------------------------
#endif