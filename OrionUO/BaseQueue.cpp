// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
/***********************************************************************************
**
** BaseQueue.cpp
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#include "stdafx.h"
//----------------------------------------------------------------------------------
//-----------------------------------CBaseQueueItem---------------------------------
//----------------------------------------------------------------------------------
CBaseQueueItem::CBaseQueueItem()
: CBaseQueue(), m_Next(NULL), m_Prev(NULL)
{
}
//----------------------------------------------------------------------------------
CBaseQueueItem::~CBaseQueueItem()
{
	WISPFUN_DEBUG("c180_f1");
	//Принудительная очистка при удалении
	Clear();

	CBaseQueueItem *item = m_Next;
	//while (item != NULL && item->m_Next != NULL)
	//	item = item->m_Next;

	while (item != NULL && item != this)
	{
		CBaseQueueItem *next = item->m_Next;
		item->m_Next = NULL;
		delete item;
		item = next;
	}

	//Если есть следующий элемент - улдалим его (контейнер очищается/удаляется)
	/*if (m_Next != NULL)
	{
		delete m_Next;
		m_Next = NULL;
	}*/
}
//----------------------------------------------------------------------------------
//-------------------------------------CBaseQueue-----------------------------------
//----------------------------------------------------------------------------------
CBaseQueue::CBaseQueue()
: m_Items(NULL)
{
}
//----------------------------------------------------------------------------------
CBaseQueue::~CBaseQueue()
{
	WISPFUN_DEBUG("c181_f1");
	//Принудительная очистка при удалении
	Clear();
}
//----------------------------------------------------------------------------------
/*!
Очистка списка
@return 
*/
void CBaseQueue::Clear()
{
	WISPFUN_DEBUG("c181_f2");
	//Если в контейнере есть элементы - достаточно просто удалить первый, остальные удалятся вместе с ним
	if (m_Items != NULL)
	{
		CBaseQueueItem *item = m_Items;
		m_Items = NULL;

		while (item != NULL)
		{
			CBaseQueueItem *next = item->m_Next;
			item->m_Next = NULL;
			delete item;
			item = next;
		}
	}
}
//----------------------------------------------------------------------------------
/*!
Добавление элемента в список
@param [__in] item Ссылка на новый элемент
@return Ссылка на новый элемент
*/
CBaseQueueItem *CBaseQueue::Add(CBaseQueueItem *item)
{
	WISPFUN_DEBUG("c181_f3");
	//Если вставляемый элемент не равен нулю
	if (item != NULL)
	{
		//Если очередь пуста -  вставим элемент в самое начало очереди
		if (m_Items == NULL)
			m_Items = item;
		else //Или, найдем последний элемент и запихаем его в зад
		{
			CBaseQueueItem *current = m_Items;

			while (current->m_Next != NULL)
				current = current->m_Next;

			current->m_Next = item;
			item->m_Prev = current;
		}
	}

	//Вернем вставляемый элемент (для однострочных конструкций типа: item = Container->Add(new TItem());)
	return item;
}
//----------------------------------------------------------------------------------
/*!
Удаление указанного элемента из списка
@param [__in] item Ссылка на элемент
@return 
*/
void CBaseQueue::Delete(CBaseQueueItem *item)
{
	WISPFUN_DEBUG("c181_f4");
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
//----------------------------------------------------------------------------------
/*!
Удаление элемента с указанным индексом
@param [__in] index Индекс элемента
@return
*/
void CBaseQueue::Delete(int index)
{
	WISPFUN_DEBUG("c181_f5");
	//Получим элемент с указанным индексом и удалим его (если есть)
	Delete(Get(index));
}
//----------------------------------------------------------------------------------
/*!
Получить индекс указанного элемента
@param [__in] item Ссылка на элемент
@return Индекс элемента в очереди
*/
int CBaseQueue::GetItemIndex(CBaseQueueItem *item)
{
	WISPFUN_DEBUG("c181_f6");
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
//----------------------------------------------------------------------------------
/*!
Получить общее количество элементов в списке
@return Количество объектов в очереди
*/
int CBaseQueue::GetItemsCount()
{
	WISPFUN_DEBUG("c181_f7");
	int count = 0;

	//Пройдемся по всем элементам очереди и запомним общее количество
	//Для экономии памяти не выводит это значение в отдельную переменную
	BQFOR(current, m_Items)
		count++;

	return count;
}
//----------------------------------------------------------------------------------
/*!
Получить элемент с указанным индексом
@param [__in] index Индекс элемента
@return Ссылка на элемент или NULL
*/
CBaseQueueItem *CBaseQueue::Get(int index)
{
	WISPFUN_DEBUG("c181_f8");
	CBaseQueueItem *item = m_Items;

	//Пройдемся по всем элементам очереди до нахождения нужного или окончания списка
	for (; item != NULL && index; item = item->m_Next, index--);

	return item;
}
//----------------------------------------------------------------------------------
/*!
Осуществляет вставку элемента в очередь
@param [__in] first Ссылка на предшествующий элемент
@param [__out] item Ссылка на элемент
@return 
*/
void CBaseQueue::Insert(CBaseQueueItem *first, CBaseQueueItem *item)
{
	WISPFUN_DEBUG("c181_f9");
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
		CBaseQueueItem *next = first->m_Next;
		item->m_Next = next;
		item->m_Prev = first;
		
		first->m_Next = item;

		if (next != NULL)
			next->m_Prev = item;
	}
}
//----------------------------------------------------------------------------------
/*!
Осуществляет изъятие указанного элемента из очереди
@param [__in] item Ссылка на элемент
@return 
*/
void CBaseQueue::Unlink(CBaseQueueItem *item)
{
	WISPFUN_DEBUG("c181_f10");
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
//----------------------------------------------------------------------------------
/*!
Поместить элемент в начало очереди
@param [__in] item Ссылка на элемент
@return 
*/
void CBaseQueue::MoveToFront(CBaseQueueItem *item)
{
	WISPFUN_DEBUG("c181_f11");
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
//----------------------------------------------------------------------------------
/*!
Поместить элемент в конец очереди
@param [__in] item Ссылка на элемент
@return 
*/
void CBaseQueue::MoveToBack(CBaseQueueItem *item)
{
	WISPFUN_DEBUG("c181_f12");
	//Если элемент не равен нулю
	if (item != NULL)
	{
		//Разлинкуем
		Unlink(item);

		//Получим указатель на конец очереди
		CBaseQueueItem *last = Last();

		//Перелинкуем элемент с последним элементом (или с началом очереди при пустой очереди)
		if (last == NULL)
			m_Items = item;
		else
			last->m_Next = item;

		item->m_Prev = last;
		item->m_Next = NULL;
	}
}
//----------------------------------------------------------------------------------
/*!
Переместить элемент вверх/вниз по очереди
@param [__in] item Ссылка на элемент
@param [__in] up Вверх или вниз по очереди
@return true в случае успешного перемещения
*/
bool CBaseQueue::Move(CBaseQueueItem *item, bool up)
{
	WISPFUN_DEBUG("c181_f13");
	//Немедленно запишем результат (и исходные данные для первой проверки) в переменную
	bool result = (item != NULL);

	//Если элемент не равен нулю
	if (result)
	{
		//Перемещение "вверх"
		if (up)
		{
			CBaseQueueItem *prev = item->m_Prev;

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
					CBaseQueueItem *prevprev = prev->m_Prev;
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
			CBaseQueueItem *next = item->m_Next;

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
					CBaseQueueItem *prev = item->m_Prev;
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
//----------------------------------------------------------------------------------
/*!
Получить указатель на последний элемент
@return Ссылка на элемент
*/
CBaseQueueItem *CBaseQueue::Last()
{
	WISPFUN_DEBUG("c181_f14");
	//Начинаем поиск с начала очереди
	CBaseQueueItem *last = m_Items;

	//Пройдемся по всем элементам очереди до конца (если очередь не пуста)
	while (last != NULL && last->m_Next != NULL)
		last = last->m_Next;

	//Вернем что получилось в результате поиска
	return last;
}
//----------------------------------------------------------------------------------
