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
//Контейнер для элементов
class TBaseQueue
{
private:
public:
	TBaseQueue();
	virtual ~TBaseQueue();

	//Первый элемент из списка
	TBaseQueueItem *m_Items;

	//Очистка списка
	virtual void Clear();

	//Добавление элемента в список
	virtual TBaseQueueItem *Add(TBaseQueueItem *item);

	//Удаление указанного элемента из списка
	void Delete(TBaseQueueItem *item);
	//Удаление элемента с указанным индексом
	void Delete(int index);

	//Получить элемент с указанным индексом
	TBaseQueueItem *Get(int index);
	//Получить индекс указанного элемента
	int GetItemIndex(TBaseQueueItem *item);
	//Получить общее количество элементов в списке
	int GetItemsCount();

	//Осуществляет изъятие указанного элемента из очереди
	void Unlink(TBaseQueueItem *item);
	
	//Осуществляет вставку элемента в очередь
	void Insert(TBaseQueueItem *first, TBaseQueueItem *item);

	//Поместить элемент в начало очереди
	void MoveToFront(TBaseQueueItem *item);
	//Поместить элемент в конец очереди
	void MoveToBack(TBaseQueueItem *item);
	//Переместить элемент вверх/вниз по очереди
	bool Move(TBaseQueueItem *item, bool up);

	//Пустой список или нет
	bool Empty() const {return (m_Items == NULL);}

	//Указатель на последний элемент
	TBaseQueueItem *Last();
};
//---------------------------------------------------------------------------
//Элемент очереди
class TBaseQueueItem : public TBaseQueue
{
private:
public:
	TBaseQueueItem();
	virtual ~TBaseQueueItem();

	//Следующий элемент
	TBaseQueueItem *m_Next;
	//Предыдущий элемент
	TBaseQueueItem *m_Prev;
};
//---------------------------------------------------------------------------
#endif