/****************************************************************************
**
** MouseManager.h
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
//--------------------------------------------------------------------------
#ifndef MouseManagerH
#define MouseManagerH
//--------------------------------------------------------------------------
//!Картинки курсора мышки
extern WORD g_CursorData[2][16];
//--------------------------------------------------------------------------
//!Класс менеджера мыши
class TMouseManager
{
private:
	//!Смещение картинки относительно курсора
	int m_CursorOffset[2][16];

public:
	TMouseManager() {}
	~TMouseManager() {}

	/*!
	Загрузка текстур курсора
	@return true при успешной загрузке
	*/
	bool LoadCursorTextures();

	/*!
	Обновить координаты мыши
	@return 
	*/
	void UpdateMouse();

	/*!
	Конвертирование значения направления
	@param [__in] val Значение направления
	@return Смещение
	*/
	int Sgn(__in int val);

	/*!
	Получить направление курсора относительно центра игрового окна
	@param [__in] x1 Координата X центра
	@param [__in] y1 Координата Y центра
	@param [__in] to_x Координата X мыши
	@param [__in] to_y Координата Y мыши
	@param [__in] current_facing Текущее направление
	@return Направление курсора
	*/
	int GetFacing(__in int x1, __in int  y1, __in int  to_x, __in int to_y, __in int current_facing);

	/*!
	Получить индекс картинки игрового курсора
	@return Индекс картинки
	*/
	WORD GetGameCursor();

	/*!
	Отрисовать курсор
	@param [__in] id Индекс картинки курсора
	@return 
	*/
	void Draw(__in WORD id);
};
//--------------------------------------------------------------------------
//!Менеджер мыши
extern TMouseManager MouseManager;
//--------------------------------------------------------------------------
#endif