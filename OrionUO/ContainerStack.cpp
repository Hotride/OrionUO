/***********************************************************************************
**
** ContainerStack.cpp
**
** Copyright (C) September 2017 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#include "stdafx.h"
//----------------------------------------------------------------------------------
deque<CContainerStackItem> g_ContainerStack;
uint g_CheckContainerStackTimer = 0;
//----------------------------------------------------------------------------------
CContainerStackItem::CContainerStackItem(const uint &serial, const short &x, const short &y, const short &minimizedX, const short &minimizedY, const bool &minimized, const bool &lockMoving)
: m_Serial(serial), m_X(x), m_Y(y), m_MinimizedX(minimizedX), m_MinimizedY(minimizedY),
m_Minimized(minimized), m_LockMoving(lockMoving)
{
}
//----------------------------------------------------------------------------------
