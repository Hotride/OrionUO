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
//Картинки курсора мышки
extern WORD g_CursorData[2][16];
//--------------------------------------------------------------------------
//Менеджер мыши
class TMouseManager
{
private:
	//Смещение картинки относительно курсора
	int m_CursorOffset[2][16];
public:
	TMouseManager() {}
	~TMouseManager() {}

	//Загрузка текстур курсора
	bool LoadCursorTextures();

	//Обновить координаты мыши
	void UpdateMouse();

	int Sgn(int val);
	int GetFacing(int x1, int  y1, int  to_x, int to_y, int current_facing);

	//Получить индекс картинки игрового курсора
	WORD GetGameCursor();

	//Отрисовать курсор
	void Draw(WORD id);
};
//--------------------------------------------------------------------------
extern TMouseManager MouseManager;
//--------------------------------------------------------------------------
#endif