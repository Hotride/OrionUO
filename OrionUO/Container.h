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
//!Структура данных с информацией о доступных координатах дропа предметов в гампы контейнеров
class CContainerOffsetRect
{
	SETGET(int, MinX, 0);
	SETGET(int, MinY, 0);
	SETGET(int, MaxX, 0);
	SETGET(int, MaxY, 0);

public:
	CContainerOffsetRect() {}
	CContainerOffsetRect(const int &minX, const int &minY, const int &maxX, const int &maxY)
		: m_MinX(minX), m_MinY(minY), m_MaxX(maxX), m_MaxY(maxY) {}
	~CContainerOffsetRect() {}
};
//----------------------------------------------------------------------------------
//!Информация о гампе и звуках контейнеров
class CContainerOffset
{
	//!Индекс картинки гампа
	SETGET(ushort, Gump, 0);

	//!Индекс звука открытия (0 - нет звука)
	SETGET(ushort, OpenSound, 0);

	//!Индекс звука закрытия (0 - нет звука)
	SETGET(ushort, CloseSound, 0);

	//!Координаты в гампе для дропа
	SETGET(CContainerOffsetRect, Rect, CContainerOffsetRect());

public:
	CContainerOffset() {}
	CContainerOffset(const ushort &gump, const ushort &openSound, const ushort &closeSound, const CContainerOffsetRect &rect)
		: m_Gump(gump), m_OpenSound(openSound), m_CloseSound(closeSound), m_Rect(rect) {}
	~CContainerOffset() {}
};
//----------------------------------------------------------------------------------
//!Класс для вычисления смещения открытия гампов контейнеров
class CContainerRect
{
	SETGET(short, DefaultX, 40);
	SETGET(short, DefaultY, 40);

	SETGET(short, X, 40);
	SETGET(short, Y, 40);

public:
	CContainerRect() {}
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
	void MakeDefault() { m_X = m_DefaultX; m_Y = m_DefaultY; }
};
//----------------------------------------------------------------------------------
//!Данные о контейнерах
extern vector<CContainerOffset> g_ContainerOffset;

//!Глобальное смещение открытия контейнеров
extern CContainerRect g_ContainerRect;
//----------------------------------------------------------------------------------
#endif
//----------------------------------------------------------------------------------
