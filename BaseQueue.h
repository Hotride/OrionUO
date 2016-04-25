/****************************************************************************
**
** BaseQueue.h
**
** Copyright (C) May 2015 Hotride
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
#ifndef BaseQueueH
#define BaseQueueH

class TBaseQueueItem;
//---------------------------------------------------------------------------
//!Контейнер для элементов
class TBaseQueue
{
public:
	TBaseQueue();
	virtual ~TBaseQueue();

	//!Первый элемент из списка
	TBaseQueueItem *m_Items;

	/*!
	Очистка списка
	@return 
	*/
	virtual void Clear();

	/*!
	Добавление элемента в список
	@param [__in] item Ссылка на новый элемент
	@return Ссылка на новый элемент
	*/
	virtual TBaseQueueItem *Add(__in TBaseQueueItem *item);

	/*!
	Удаление указанного элемента из списка
	@param [__in] item Ссылка на элемент
	@return 
	*/
	void Delete(__in TBaseQueueItem *item);

	/*!
	Удаление элемента с указанным индексом
	@param [__in] index Индекс элемента
	@return 
	*/
	void Delete(__in const int &index);

	/*!
	Получить элемент с указанным индексом
	@param [__in] index Индекс элемента
	@return Ссылка на элемент или NULL
	*/
	TBaseQueueItem *Get(__in int index);

	/*!
	Получить индекс указанного элемента
	@param [__in] item Ссылка на элемент
	@return Индекс элемента в очереди
	*/
	int GetItemIndex(__in TBaseQueueItem *item);

	/*!
	Получить общее количество элементов в списке
	@return Количество объектов в очереди
	*/
	int GetItemsCount();

	/*!
	Осуществляет изъятие указанного элемента из очереди
	@param [__in] item Ссылка на элемент
	@return 
	*/
	void Unlink(__in TBaseQueueItem *item);

	/*!
	Осуществляет вставку элемента в очередь
	@param [__in] first Ссылка на предшествующий элемент
	@param [__out] item Ссылка на элемент
	@return 
	*/
	void Insert(__in TBaseQueueItem *first, __out TBaseQueueItem *item);

	/*!
	Поместить элемент в начало очереди
	@param [__in] item Ссылка на элемент
	@return 
	*/
	void MoveToFront(__in TBaseQueueItem *item);

	/*!
	Поместить элемент в конец очереди
	@param [__in] item Ссылка на элемент
	@return 
	*/
	void MoveToBack(__in TBaseQueueItem *item);

	/*!
	Переместить элемент вверх/вниз по очереди
	@param [__in] item Ссылка на элемент
	@param [__in] up Вверх или вниз по очереди
	@return true в случае успешного перемещения
	*/
	bool Move(__in TBaseQueueItem *item, __in const bool &up);

	/*!
	Пустой список или нет
	@return true в случае успешного перемещения
	*/
	bool Empty() const {return (m_Items == NULL);}

	/*!
	Получить указатель на последний элемент
	@return Ссылка на элемент
	*/
	TBaseQueueItem *Last();
};
//---------------------------------------------------------------------------
//!Элемент очереди
class TBaseQueueItem : public TBaseQueue
{
public:
	TBaseQueueItem();
	virtual ~TBaseQueueItem();

	//!Следующий элемент
	TBaseQueueItem *m_Next;

	//!Предыдущий элемент
	TBaseQueueItem *m_Prev;
};
//---------------------------------------------------------------------------
#endif