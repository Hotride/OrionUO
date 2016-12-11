/***********************************************************************************
**
** GameItem.h
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#ifndef GAMEITEM_H
#define GAMEITEM_H
//----------------------------------------------------------------------------------
#include "GameObject.h"
#include "MultiObject.h"
#include "../Multi.h"
//----------------------------------------------------------------------------------
//!Класс игрового предмета (или трупа)
class CGameItem : public CGameObject
{
	//!Слой, в котором расположен объект
	SETGET(uchar, Layer);
	//!Индекс картинки анимации
	SETGET(ushort, AnimID);
	//!Индекс картинки гампа для папердолла
	SETGET(ushort, ImageID);
	//!Используемый предметом слой (из tiledata.mul/verdata)
	SETGET(uchar, UsedLayer);
	//!Открыт ли контейнер/спеллбук
	SETGET(bool, Opened);
	//!Выведен ли гамп разделения количества?
	SETGET(bool, Dragged);
	//!Это прокси-объект мульти
	SETGET(bool, MultiBody);
	//!Это прокси-объект мульти
	SETGET(bool, WantUpdateMulti);
	//!Цвет тайла для замены филдов (если 0 - не филд)
	SETGET(ushort, FieldColor);
	//!Бонус дистанции для мульти-объекта
	SETGET(short, MultiDistanceBonus);

protected:
	/*!
	Добавить объекта-мульти
	@param [__in] obj Ссылка на мульти-объект
	@return 
	*/
	void AddMultiObject(CMultiObject *obj);

public:
	CGameItem(const uint &serial = 0);
	virtual ~CGameItem();

	void CalculateFieldColor();

	/*!
	Вставка данных из объекта в руке (на курсоре)
	@param [__in] obj Ссылка на объект на курсоре
	@return 
	*/
	void Paste(class CObjectOnCursor *obj);

	void ClearMultiItems();

	ushort GetFirstMultiGraphic();

	virtual void Draw(const int &x, const int &y);

	virtual void Select(const int &x, const int &y);

	/*!
	Событие изменения картинки объекта
	@param [__in_opt] direction Направление предмета (для трупов)
	@return
	*/
	void OnGraphicChange(int direction = 0);

	/*!
	Получить индекс картинки (для анимации)
	@return Индекс картинки
	*/
	ushort GetMountAnimation();

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
	void LoadMulti();

	/*!
	Получение объекта мульти в заданных координатах
	@param [__in] x Координата X
	@param [__in] y Координата Y
	@return Ссылка на мульти или NULL
	*/
	CMulti *GetMultiAtXY(const short &x, const short &y);

	/*!
	Найти объект внутри (рекурсивно) по типу с учетом (и без) цвета
	@param [__in] graphic Индекс картинки
	@param [__in_opt] color Цвет предмета
	@return Ссылка на найденный объект или NULL
	*/
	CGameItem *FindItem(const ushort &graphic, const ushort &color = 0xFFFF);
};
//----------------------------------------------------------------------------------
#endif
//----------------------------------------------------------------------------------
