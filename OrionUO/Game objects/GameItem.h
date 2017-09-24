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
//!Класс игрового предмета (или трупа)
class CGameItem : public CGameObject
{
	//!Слой, в котором расположен объект
	SETGET(uchar, Layer, 0);
	//!Индекс картинки анимации
	SETGET(ushort, AnimID, 0);
	//!Используемый предметом слой (из tiledata.mul/verdata)
	SETGET(uchar, UsedLayer, 0);
	//!Открыт ли контейнер/спеллбук
	SETGET(bool, Opened, false);
	//!Выведен ли гамп разделения количества?
	SETGET(bool, Dragged, false);
	//!Это прокси-объект мульти
	SETGET(bool, MultiBody, false);
	//!Необходимо обновить мульти-объект
	SETGET(bool, WantUpdateMulti, true);
	//!Цвет тайла для замены филдов (если 0 - не филд)
	SETGET(ushort, FieldColor, 0);
	//!Бонус дистанции для мульти-объекта
	SETGET(short, MultiDistanceBonus, 0);
	//!Цена для магазина
	SETGET(uint, Price, 0);
	//!Имя из клилока (для гампа магазина)
	SETGET(bool, NameFromCliloc, false);

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

	CMulti* GetMulti();

	void ClearMultiItems();

	ushort GetFirstMultiGraphic();

	virtual void Draw(const int &x, const int &y);

	virtual void Select(const int &x, const int &y);

	void AddMulti(ushort &graphic, char &x, char &y, char &z);

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
	void LoadMulti(const bool &dropAlpha);

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
