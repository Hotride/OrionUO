/***********************************************************************************
**
** Container.h
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#ifndef CONTAINER_H
#define CONTAINER_H
//----------------------------------------------------------------------------------
#include "Globals.h"
//----------------------------------------------------------------------------------
class CUseItemActions
{
	SETGET(uint, Timer);

private:
	deque<uint> m_List;

public:
	CUseItemActions() : m_Timer(0) {}
	~CUseItemActions() {}

	void Add(const uint &serial);

	void Clear() { m_List.clear(); }

	void Process();
};
//----------------------------------------------------------------------------------
extern CUseItemActions g_UseItemActions;
//----------------------------------------------------------------------------------
//!Класс стека контейнеров, используется для хранения информации при загрузке гапмов контейнеров из конфига при входе в игру
class CContainerStackItem
{
	SETGET(uint, Serial);
	SETGET(short, X);
	SETGET(short, Y);
	SETGET(short, MinimizedX);
	SETGET(short, MinimizedY);
	SETGET(bool, Minimized);
	SETGET(bool, LockMoving);

public:
	CContainerStackItem(uint serial, short x, short y, short minimizedX, short minimizedY, bool minimized, bool lockMoving);
	~CContainerStackItem();
};
//----------------------------------------------------------------------------------
//!Структура данных с информацией о доступных координатах дропа предметов в гампы контейнеров
struct CONTAINER_OFFSET_RECT
{
	int MinX;
	int MinY;
	int MaxX;
	int MaxY;
};
//----------------------------------------------------------------------------------
//!Информация о гампе и звуках контейнеров
struct CONTAINER_OFFSET
{
	//!Индекс картинки гампа
	ushort Gump;

	//!Индекс звука открытия (0 - нет звука)
	ushort OpenSound;

	//!Индекс звука закрытия (0 - нет звука)
	ushort CloseSound;

	//!Координаты в гампе для дропа
	CONTAINER_OFFSET_RECT rect;
};
//----------------------------------------------------------------------------------
//!Буферная информация о предмете в трупе
struct CORPSE_EQUIPMENT_DATA
{
	//!Серийник
	uint Serial;

	//!Слой
	int Layer;
};
//----------------------------------------------------------------------------------
//!Стартовая позиция открытия контейнеров
const int CONTAINERS_RECT_DEFAULT_POS = 40;

//!Смещение при заполнении линии
const int CONTAINERS_RECT_LINESTEP = 800;

//!Шаг смещения
const int CONTAINERS_RECT_STEP = 20;
//----------------------------------------------------------------------------------
//!Класс для вычисления смещения открытия гампов контейнеров
class CContainerRect
{
	SETGET(short, X);
	SETGET(short, Y);

public:
	CContainerRect();
	~CContainerRect() {}

	/*!
	Функция вычисления смещения для текущей картинки
	@param [__in] gumpID Индекс картинки
	@return 
	*/
	void Calculate(ushort gumpID);

	/*!
	Выставим параметры по-умолчанию
	@return 
	*/
	void MakeDefault() { m_X = m_Y = CONTAINERS_RECT_DEFAULT_POS; }
};
//----------------------------------------------------------------------------------
//!Количество контейнеров
const int CONTAINERS_COUNT = 21;

//!Данные о контейнерах
extern CONTAINER_OFFSET g_ContainerOffset[CONTAINERS_COUNT];

//!Ссылка на стек открытия контейнеров
extern deque<CContainerStackItem> g_ContainerStack;

//!Таймер стека открытия контейнеров
extern uint g_CheckContainerStackTimer;

//!Глобальное смещение открытия контейнеров
extern CContainerRect g_ContainerRect;
//----------------------------------------------------------------------------------
#endif
//----------------------------------------------------------------------------------
