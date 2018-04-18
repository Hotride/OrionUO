/***********************************************************************************
**
** BaseQueue.h
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#ifndef BASEQUEUE_H
#define BASEQUEUE_H
//----------------------------------------------------------------------------------
//!TBaseQueue for [extended]
#define QFOR(var, start, type) for (type var = (type)start; var != NULL; var = (type)var->m_Next)
//!TBaseQueue for
#define BQFOR(var, start) QFOR(var, start, CBaseQueueItem*)
//----------------------------------------------------------------------------------
class CBaseQueueItem;
//----------------------------------------------------------------------------------
//!Контейнер для элементов
class CBaseQueue
{
public:
	CBaseQueue();
	virtual ~CBaseQueue();

	//!Первый элемент из списка
	CBaseQueueItem *m_Items;

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
	virtual CBaseQueueItem *Add(CBaseQueueItem *item);

	/*!
	Удаление указанного элемента из списка
	@param [__in] item Ссылка на элемент
	@return 
	*/
	void Delete(CBaseQueueItem *item);

	/*!
	Удаление элемента с указанным индексом
	@param [__in] index Индекс элемента
	@return 
	*/
	void Delete(int index);

	/*!
	Получить элемент с указанным индексом
	@param [__in] index Индекс элемента
	@return Ссылка на элемент или NULL
	*/
	CBaseQueueItem *Get(int index);

	/*!
	Получить индекс указанного элемента
	@param [__in] item Ссылка на элемент
	@return Индекс элемента в очереди
	*/
	int GetItemIndex(CBaseQueueItem *item);

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
	void Unlink(CBaseQueueItem *item);

	/*!
	Осуществляет вставку элемента в очередь
	@param [__in] first Ссылка на предшествующий элемент
	@param [__out] item Ссылка на элемент
	@return 
	*/
	void Insert(CBaseQueueItem *first, CBaseQueueItem *item);

	/*!
	Поместить элемент в начало очереди
	@param [__in] item Ссылка на элемент
	@return 
	*/
	virtual void MoveToFront(CBaseQueueItem *item);

	/*!
	Поместить элемент в конец очереди
	@param [__in] item Ссылка на элемент
	@return 
	*/
	void MoveToBack(CBaseQueueItem *item);

	/*!
	Переместить элемент вверх/вниз по очереди
	@param [__in] item Ссылка на элемент
	@param [__in] up Вверх или вниз по очереди
	@return true в случае успешного перемещения
	*/
	bool Move(CBaseQueueItem *item, bool up);

	/*!
	Пустой список или нет
	@return true в случае успешного перемещения
	*/
	bool Empty() const {return (m_Items == NULL);}

	/*!
	Получить указатель на последний элемент
	@return Ссылка на элемент
	*/
	CBaseQueueItem *Last();
};
//----------------------------------------------------------------------------------
//!Элемент очереди
class CBaseQueueItem : public CBaseQueue
{
public:
	CBaseQueueItem();
	virtual ~CBaseQueueItem();

	//!Следующий элемент
	CBaseQueueItem *m_Next;

	//!Предыдущий элемент
	CBaseQueueItem *m_Prev;
};
//----------------------------------------------------------------------------------
#endif
