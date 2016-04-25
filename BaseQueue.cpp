/****************************************************************************
**
** BaseQueue.cpp
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
#include "stdafx.h"
//---------------------------------------------------------------------------
//--------------------------------TBaseQueueItem-----------------------------
//---------------------------------------------------------------------------
TBaseQueueItem::TBaseQueueItem()
: TBaseQueue(), m_Next(NULL), m_Prev(NULL)
{
}
//---------------------------------------------------------------------------
TBaseQueueItem::~TBaseQueueItem()
{
	//Принудительная очистка при удалении
	Clear();

	//Если есть следующий элемент - улдалим его (контейнер очищается/удаляется)
	if (m_Next != NULL)
	{
		delete m_Next;
		m_Next = NULL;
	}
}
//---------------------------------------------------------------------------
//----------------------------------TBaseQueue-------------------------------
//---------------------------------------------------------------------------
TBaseQueue::TBaseQueue()
: m_Items(NULL)
{
}
//---------------------------------------------------------------------------
TBaseQueue::~TBaseQueue()
{
	//Принудительная очистка при удалении
	Clear();
}
//---------------------------------------------------------------------------
/*!
Очистка списка
@return 
*/
void TBaseQueue::Clear()
{
	//Если в контейнере есть элементы - достаточно просто удалить первый, остальные удалятся вместе с ним
	if (m_Items != NULL)
	{
		delete m_Items;
		m_Items = NULL;
	}
}
//---------------------------------------------------------------------------
/*!
Добавление элемента в список
@param [__in] item Ссылка на новый элемент
@return Ссылка на новый элемент
*/
TBaseQueueItem *TBaseQueue::Add(__in TBaseQueueItem *item)
{
	//Если вставляемый элемент не равен нулю
	if (item != NULL)
	{
		//Если очередь пуста -  вставим элемент в самое начало очереди
		if (m_Items == NULL)
			m_Items = item;
		else //Или, найдем последний элемент и запихаем его в зад
		{
			TBaseQueueItem *current = m_Items;

			while (current->m_Next != NULL)
				current = current->m_Next;

			current->m_Next = item;
			item->m_Prev = current;
		}
	}

	//Вернем вставляемый элемент (для однострочных конструкций типа: item = Container->Add(new TItem());)
	return item;
}
//---------------------------------------------------------------------------
/*!
Удаление указанного элемента из списка
@param [__in] item Ссылка на элемент
@return 
*/
void TBaseQueue::Delete( __in TBaseQueueItem *item)
{
	//Если элемент не равен нулю
	if (item != NULL)
	{
		//Разлинкуем элемент
		Unlink(item);

		//Можно спокойно удалять его
		item->m_Next = NULL;
		item->m_Prev = NULL;
		delete item;
	}
}
//---------------------------------------------------------------------------
/*!
Удаление элемента с указанным индексом
@param [__in] index Индекс элемента
@return
*/
void TBaseQueue::Delete(__in const int &index)
{
	//Получим элемент с указанным индексом и удалим его (если есть)
	Delete(Get(index));
}
//---------------------------------------------------------------------------
/*!
Получить индекс указанного элемента
@param [__in] item Ссылка на элемент
@return Индекс элемента в очереди
*/
int TBaseQueue::GetItemIndex( __in TBaseQueueItem *item)
{
	int index = 0;

	//Пройдемся по очереди
	BQFOR(current, m_Items)
	{
		//Если элемент нашелся - возвращаем вычисленный индекс
		if (current == item)
			return index;

		index++;
	}

	//В случае не удачного поиска - вернем -1 (не найдено)
	return -1;
}
//---------------------------------------------------------------------------
/*!
Получить общее количество элементов в списке
@return Количество объектов в очереди
*/
int TBaseQueue::GetItemsCount()
{
	int count = 0;

	//Пройдемся по всем элементам очереди и запомним общее количество
	//Для экономии памяти не выводит это значение в отдельную переменную
	BQFOR(current, m_Items)
		count++;

	return count;
}
//---------------------------------------------------------------------------
/*!
Получить элемент с указанным индексом
@param [__in] index Индекс элемента
@return Ссылка на элемент или NULL
*/
TBaseQueueItem *TBaseQueue::Get(__in int index)
{
	TBaseQueueItem *item = m_Items;

	//Пройдемся по всем элементам очереди до нахождения нужного или окончания списка
	for (; item != NULL && index; item = item->m_Next, index--);

	return item;
}
//---------------------------------------------------------------------------
/*!
Осуществляет вставку элемента в очередь
@param [__in] first Ссылка на предшествующий элемент
@param [__out] item Ссылка на элемент
@return 
*/
void TBaseQueue::Insert( __in TBaseQueueItem *first, __out TBaseQueueItem *item)
{
	if (first == NULL)
	{
		item->m_Next = m_Items;
		item->m_Prev = NULL;
		
		if (m_Items != NULL)
			m_Items->m_Prev = item;

		m_Items = item;
	}
	else
	{
		TBaseQueueItem *next = first->m_Next;
		item->m_Next = next;
		item->m_Prev = first;
		
		first->m_Next = item;

		if (next != NULL)
			next->m_Prev = item;
	}
}
//---------------------------------------------------------------------------
/*!
Осуществляет изъятие указанного элемента из очереди
@param [__in] item Ссылка на элемент
@return 
*/
void TBaseQueue::Unlink( __in TBaseQueueItem *item)
{
	//Если элемент не равен нулю
	if (item != NULL)
	{
		//Если элемент - начало списка
		if (item == m_Items)
		{
			//Скорректируем его
			m_Items = m_Items->m_Next;

			if (m_Items != NULL)
				m_Items->m_Prev = NULL;
		}
		else
		{
			//Или подменим указатели предыдущего и следующего (при его наличии) элементов друг на друга
			item->m_Prev->m_Next = item->m_Next;

			if (item->m_Next != NULL)
				item->m_Next->m_Prev = item->m_Prev;
		}
	}
}
//---------------------------------------------------------------------------
/*!
Поместить элемент в начало очереди
@param [__in] item Ссылка на элемент
@return 
*/
void TBaseQueue::MoveToFront( __in TBaseQueueItem *item)
{
	//Если элемент не равен нулю и не равен началу очереди
	if (item != NULL && item != m_Items)
	{
		//Разлинкуем
		Unlink(item);

		//Перелинкуем с началом очереди
		if (m_Items != NULL)
			m_Items->m_Prev = item;

		item->m_Next = m_Items;
		item->m_Prev = NULL;

		//Вставим в начало очереди
		m_Items = item;
	}
}
//---------------------------------------------------------------------------
/*!
Поместить элемент в конец очереди
@param [__in] item Ссылка на элемент
@return 
*/
void TBaseQueue::MoveToBack( __in TBaseQueueItem *item)
{
	//Если элемент не равен нулю
	if (item != NULL)
	{
		//Разлинкуем
		Unlink(item);

		//Получим указатель на конец очереди
		TBaseQueueItem *last = Last();

		//Перелинкуем элемент с последним элементом (или с началом очереди при пустой очереди)
		if (last == NULL)
			m_Items = item;
		else
			last->m_Next = item;

		item->m_Prev = last;
		item->m_Next = NULL;
	}
}
//---------------------------------------------------------------------------
/*!
Переместить элемент вверх/вниз по очереди
@param [__in] item Ссылка на элемент
@param [__in] up Вверх или вниз по очереди
@return true в случае успешного перемещения
*/
bool TBaseQueue::Move(__in TBaseQueueItem *item, __in const bool &up)
{
	//Немедленно запишем результат (и исходные данные для первой проверки) в переменную
	bool result = (item != NULL);

	//Если элемент не равен нулю
	if (result)
	{
		//Перемещение "вверх"
		if (up)
		{
			TBaseQueueItem *prev = item->m_Prev;

			//Если предыдущий элемент не равен нулю (есть куда двигаться)
			result = (prev != NULL);

			if (result)
			{
				//Предыдущий элемент - начало очереди
				if (prev == m_Items)
				{
					prev->m_Prev = item;
					prev->m_Next = item->m_Next;
					m_Items = item;
					item->m_Prev = NULL;
					item->m_Next = prev;
				}
				else //Где-то в теле очереди
				{
					TBaseQueueItem *prevprev = prev->m_Prev;
					prev->m_Prev = item;
					prev->m_Next = item->m_Next;
					prevprev->m_Next = item;
					item->m_Prev = prevprev;
					item->m_Next = prev;
				}
			}
		}
		else
		{
			TBaseQueueItem *next = item->m_Next;

			//Если следующий элемент не равен нулю (есть куда двигаться)
			result = (next != NULL);

			if (result)
			{
				//Текущий элемент - начало очереди
				if (item == m_Items)
				{
					item->m_Next = next->m_Next;
					item->m_Prev = next;
					m_Items = item;
					m_Items->m_Prev = NULL;
					m_Items->m_Next = item;
				}
				else //Где-то в теле очереди
				{
					TBaseQueueItem *prev = item->m_Prev;
					prev->m_Next = next;
					item->m_Next = next->m_Next;
					item->m_Prev = next;
					next->m_Prev = prev;
					next->m_Next = item;
				}
			}
		}
	}

	//Если все проверки прошли успешно - элемент перемещен
	return result;
}
//---------------------------------------------------------------------------
/*!
Получить указатель на последний элемент
@return Ссылка на элемент
*/
TBaseQueueItem *TBaseQueue::Last()
{
	//Начинаем поиск с начала очереди
	TBaseQueueItem *last = m_Items;

	//Пройдемся по всем элементам очереди до конца (если очередь не пуста)
	while (last != NULL && last->m_Next != NULL)
		last = last->m_Next;

	//Вернем что получилось в результате поиска
	return last;
}
//---------------------------------------------------------------------------