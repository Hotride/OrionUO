/***********************************************************************************
**
** ContainerStack.h
**
** Copyright (C) September 2017 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#ifndef CONTAINERSTACK_H
#define CONTAINERSTACK_H
//----------------------------------------------------------------------------------
//!Класс стека контейнеров, используется для хранения информации при загрузке гапмов контейнеров из конфига при входе в игру
class CContainerStackItem
{
	SETGET(uint, Serial, 0);
	SETGET(short, X, 0);
	SETGET(short, Y, 0);
	SETGET(short, MinimizedX, 0);
	SETGET(short, MinimizedY, 0);
	SETGET(bool, Minimized, false);
	SETGET(bool, LockMoving, false);

public:
	CContainerStackItem(const uint &serial, const short &x, const short &y, const short &minimizedX, const short &minimizedY, const bool &minimized, const bool &lockMoving);
	~CContainerStackItem() {}
};
//----------------------------------------------------------------------------------
//!Ссылка на стек открытия контейнеров
extern deque<CContainerStackItem> g_ContainerStack;

//!Таймер стека открытия контейнеров
extern uint g_CheckContainerStackTimer;
//----------------------------------------------------------------------------------
#endif //CONTAINERSTACK_H
//----------------------------------------------------------------------------------
