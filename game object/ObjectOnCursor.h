/****************************************************************************
**
** ObjectOnCursor.h
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
#ifndef ObjectOnCursorH
#define ObjectOnCursorH
//--------------------------------------------------------------------------
//!Класс объекта на курсоре
class TObjectOnCursor : public TGameItem
{
private:
	//!Раньше был в стеке
	bool m_Separated;

	//!Предмет удален
	bool m_Deleted;

	//!Бросили предмет (на землю/в контейнер/на персонажа)
	bool m_Dropped;

	//!Это игровая фигура
	bool m_IsGameFigure;

	//!Количество для перемещения
	DWORD m_DragCount;

	//!Не отображать при рендере
	bool m_NoDraw;
public:
	TObjectOnCursor();
	TObjectOnCursor(TGameItem *obj);
	virtual ~TObjectOnCursor() {}

	SETGET(bool, Separated);
	SETGET(bool, Deleted);
	SETGET(bool, Dropped);
	SETGET(bool, IsGameFigure);
	SETGET(DWORD, DragCount);
	SETGET(bool, NoDraw);
};
//--------------------------------------------------------------------------
//!Указатель на объект в руке (на курсоре)
extern TObjectOnCursor *ObjectInHand;
//--------------------------------------------------------------------------
#endif