/***********************************************************************************
**
** UOFileReader.h
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#ifndef MULREADER_H
#define MULREADER_H
//----------------------------------------------------------------------------------
//!Класс для чтения данных из файлов и создания текстур
class UOFileReader
{
public:
	UOFileReader() {}
	virtual ~UOFileReader() {}

	/*!
	Получить массив пикселей гампа
	@param [__in] io Ссылка на данные о гампе
	@return Массив пикселей или NULL
	*/
	virtual USHORT_LIST GetGumpPixels(CIndexObject &io);

	/*!
	Прочитать гамп и сгенерировать текстуру
	@param [__in] io Ссылка на данные о гампе
	@return Ссылка на данные о текстуре
	*/
	virtual CGLTexture *ReadGump(CIndexObject &io);

	/*!
	Прочитать арт и сгенерировать текстуру
	@param [__in] ID Индекс арта
	@param [__in] io Ссылка на данные о арте
	@return Ссылка на данные о текстуре
	*/
	virtual CGLTexture *ReadArt(const ushort &id, CIndexObject &io, const bool &run);

	/*!
	Прочитать арт и вычислить реальные пииксельные границы картинки
	@param [__in] io Ссылка на данные о арте
	@param [__out] r Структура с габаритами на выходе
	@return Ссылка на данные о текстуре
	*/
	virtual WISP_GEOMETRY::CRect ReadStaticArtPixelDimension(CIndexObject &io);

	/*!
	Прочитать текстуру ландшафта и сгенерировать тексруту
	@param [__in] io Ссылка на данные о текстуре ландшафта
	@return Ссылка на данные о текстуре
	*/
	virtual CGLTexture *ReadTexture(CIndexObject &io);

	/*!
	Прочитать освещение и сгенерировать текстуру
	@param [__in] io Ссылка на данные о освещении
	@return Ссылка на данные о текстуре
	*/
	virtual CGLTexture *ReadLight(CIndexObject &io);

	/*!
	Проверить нахождение пикселя гампа в указанных координатах
	@param [__in] io Ссылка на данные о гампе
	@param [__in] checkX Координата X
	@param [__in] checkY Координата Y
	@return true в случае успешного теста
	*/
	virtual bool GumpPixelsInXY(CIndexObject &io, const int &checkX, const int &checkY);

	/*!
	Проверить нахождение пикселя арта в указанных координатах
	@param [__in] land Ландшафт или статика
	@param [__in] io Ссылка на данные о арте
	@param [__in] checkX Координата X
	@param [__in] checkY Координата Y
	@return true в случае успешного теста
	*/
	virtual bool ArtPixelsInXY(const bool &land, CIndexObject &io, const int &checkX, const int &checkY);
};
//----------------------------------------------------------------------------------
//!Класс чтения данных
extern UOFileReader g_UOFileReader;
//----------------------------------------------------------------------------------
#endif
//----------------------------------------------------------------------------------
