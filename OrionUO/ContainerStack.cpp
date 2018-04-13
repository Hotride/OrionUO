// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
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
: Serial(serial), X(x), Y(y), MinimizedX(minimizedX), MinimizedY(minimizedY),
Minimized(minimized), LockMoving(lockMoving)
{
}
//----------------------------------------------------------------------------------
