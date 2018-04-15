/***********************************************************************************
**
** ObjectOnCursor.h
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#ifndef OBJECTONCURSOR_H
#define OBJECTONCURSOR_H
//----------------------------------------------------------------------------------
//!Класс объекта на курсоре
class CObjectOnCursor
{
public:
	//!Активен
	bool Enabled = false;
	//!Сброшен
	bool Dropped = false;
	//!Серийник объекта
	uint Serial = 0;
	//!Индекс картинки
	ushort Graphic = 0;
	//!Цвет
	ushort Color = 0;
	//!Количество для перемещения
	ushort Count = 0;
	//!Количество ранее в стеке
	ushort TotalCount = 0;
	//!Слой
	uchar Layer = 0;
	//!Флаги
	uchar Flags = 0;
	//!Контейнер
	uint Container = 0;
	//!Координата X
	ushort X = 0;
	//!Координата Y
	ushort Y = 0;
	//!Координата Z
	char Z = 0;
	//!Это игровая фигура
	bool IsGameFigure = false;
	//!Указатель на тайлдату для объекта
	STATIC_TILES* TiledataPtr = NULL;	//!Обновлен ли предмет в мире
	bool UpdatedInWorld = false;

	CObjectOnCursor() {}
	virtual ~CObjectOnCursor() {}

	void Clear();

	ushort GetDrawGraphic(bool &doubleDraw);
};
//----------------------------------------------------------------------------------
//!Указатель на объект в руке (на курсоре)
extern CObjectOnCursor g_ObjectInHand;
//----------------------------------------------------------------------------------
#endif
