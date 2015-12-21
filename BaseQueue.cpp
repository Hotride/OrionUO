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
TBaseQueueItem *TBaseQueue::Add(TBaseQueueItem *item)
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
void TBaseQueue::Delete(TBaseQueueItem *item)
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
void TBaseQueue::Delete(int index)
{
	//Получим элемент с указанным индексом и удалим его (если есть)
	Delete(Get(index));
}
//---------------------------------------------------------------------------
int TBaseQueue::GetItemIndex(TBaseQueueItem *item)
{
	int index = 0;

	//Пройдемся по очереди
	for (TBaseQueueItem *current = m_Items; current != NULL; current = current->m_Next, index++)
	{
		//Если элемент нашелся - возвращаем вычисленный индекс
		if (current == item)
			return index;
	}

	//В случае не удачного поиска - вернем -1 (не найдено)
	return -1;
}
//---------------------------------------------------------------------------
int TBaseQueue::GetItemsCount()
{
	int count = 0;

	//Пройдемся по всем элементам очереди и запомним общее количество
	//Для экономии памяти не выводит это значение в отдельную переменную
	for (TBaseQueueItem *current = m_Items; current != NULL; current = current->m_Next, count++);

	return count;
}
//---------------------------------------------------------------------------
TBaseQueueItem *TBaseQueue::Get(int index)
{
	TBaseQueueItem *item = m_Items;

	//Пройдемся по всем элементам очереди до нахождения нужного или окончания списка
	for (; item != NULL && index; item = item->m_Next, index--);

	return item;
}
//---------------------------------------------------------------------------
void TBaseQueue::Insert(TBaseQueueItem *first, TBaseQueueItem *item)
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
void TBaseQueue::Unlink(TBaseQueueItem *item)
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
void TBaseQueue::MoveToFront(TBaseQueueItem *item)
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
void TBaseQueue::MoveToBack(TBaseQueueItem *item)
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
bool TBaseQueue::Move(TBaseQueueItem *item, bool up)
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