/****************************************************************************
**
** GameObject.h
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
#ifndef GameItemH
#define GameItemH
//--------------------------------------------------------------------------
//!Класс игрового предмета (или трупа)
class TGameItem : public TGameObject
{
protected:
	//!Слой, в котором расположен объект
	BYTE m_Layer;

	//!Индекс анимации
	WORD m_AnimID;

	//!Индекс гампа для папердолла
	WORD m_ImageID;

	//!Используемый предметом слой (из tiledata.mul/verdata)
	BYTE m_UsedLayer;

	//!Открыт ли контейнер/спеллбук
	bool m_Opened;

	//!Выведен ли гамп разделения количества?
	bool m_Dragged;

	/*!
	Добавить объекта-мульти
	@param [__in] obj Ссылка на мульти-объект
	@return 
	*/
	void AddMultiObject(__in TMultiObject *obj);
public:
	TGameItem(DWORD serial = 0);
	virtual ~TGameItem();

	//!Указатель на элемент листа покупок
	TShopItem *ShopItem;

	SETGET(BYTE, Layer);
	SETGET(WORD, AnimID);
	SETGET(WORD, ImageID);
	SETGET(BYTE, UsedLayer);
	SETGET(bool, Opened);
	SETGET(bool, Dragged);

	/*!
	Вставка данных из объекта в руке (на курсоре)
	@param [__in] obj Ссылка на объект на курсоре
	@return 
	*/
	void Paste(__in class TObjectOnCursor *obj);

	/*!
	Отрисовать предмет
	@param [__in] mode Режим рисования. true - рисование, false - выбор объектов
	@param [__in] drawX Экранная координата X объекта
	@param [__in] drawY Экранная координата Y объекта
	@param [__in] ticks Таймер рендера
	@return При выборе объектов возвращает выбранный элемент
	*/
	virtual int Draw(bool &mode, int &drawX, int &drawY, DWORD &ticks);

	/*!
	Событие изменения картинки объекта
	@param [__in_opt] direction Направление предмета (для трупов)
	@return
	*/
	void OnGraphicChange(__in_opt int direction = 0);

	/*!
	Получить индекс картинки (для анимации)
	@return Индекс картинки
	*/
	WORD GetMountAnimation();

	/*!
	Внутренний ли это объект, исключая трупы
	@return 
	*/
	virtual bool IsInternal() { return ((m_TiledataPtr->Flags & 0x00010000) && !IsCorpse()); }

	/*!
	Человекоподобный труп ли это?
	@return 
	*/
	bool IsHuman() { return (IsCorpse() && (m_Count >= 0x0190) && (m_Count <= 0x0193)); }

	/*!
	Загрузка мульти в текущий объект
	@return 
	*/
	void LoadMulti(__in);

	/*!
	Получение объекта мульти в заданных координатах
	@param [__in] x Координата X
	@param [__in] y Координата Y
	@return Ссылка на мульти или NULL
	*/
	TMulti *GetMultiAtXY(__in short x, __in short y);

	/*!
	Найти объект внутри (рекурсивно) по типу с учетом (и без) цвета
	@param [__in] graphic Индекс картинки
	@param [__in_opt] color Цвет предмета
	@return Ссылка на найденный объект или NULL
	*/
	TGameItem *FindItem(__in WORD graphic, __in_opt WORD color = 0xFFFF);

	/*!
	Найти объект на указанном слое (для трупов)
	@param [__in] layer Номер слоя
	@return Ссылка на найденный объект или NULL
	*/
	TGameItem *FindLayer(__in int layer);

	virtual TGameItem *GameItemPtr() { return this; }
};
//---------------------------------------------------------------------------
#endif