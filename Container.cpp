/****************************************************************************
**
** Container.cpp
**
** Copyright (C) September 2015 Hotride
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
//----------------------------------------------------------------------------
#include "stdafx.h"

TContainerStack *ContainerStack = NULL;
DWORD g_CheckContainerStackTimer = 0;
TContainerRect ContainerRect;
//----------------------------------------------------------------------------
TContainerStack::TContainerStack(DWORD serial, short x, short y, short minimizedX, short minimizedY, bool minimized, bool lockMoving)
: m_Serial(serial), m_X(x), m_Y(y), m_MinimizedX(minimizedX), m_MinimizedY(minimizedY),
m_Minimized(minimized), m_LockMoving(lockMoving), m_Next(NULL), m_Prev(NULL)
{
}
//----------------------------------------------------------------------------
TContainerStack::~TContainerStack()
{
	if (m_Next != NULL)
		delete m_Next;

	m_Next = NULL;
	m_Prev = NULL;
}
//---------------------------------------------------------------------------
CONTAINER_OFFSET g_ContainerOffset[CONTAINERS_COUNT] =
{
	//Gump   OpenSnd  CloseSnd  X   Y   Width Height
	{ 0x0009, 0x0000, 0x0000, { 20, 85, 124, 196 } }, //corpse
	{ 0x003C, 0x0048, 0x0058, { 44, 65, 186, 159 } },
	{ 0x003D, 0x0048, 0x0058, { 29, 34, 137, 128 } },
	{ 0x003E, 0x002F, 0x002E, { 33, 36, 142, 148 } },
	{ 0x003F, 0x004F, 0x0058, { 19, 47, 182, 123 } },
	{ 0x0041, 0x004F, 0x0058, { 35, 38, 145, 116 } },
	{ 0x0042, 0x002D, 0x002C, { 18, 105, 162, 178 } },
	{ 0x0043, 0x002D, 0x002C, { 16, 51, 181, 124 } },
	{ 0x0044, 0x002D, 0x002C, { 20, 10, 170, 100 } },
	{ 0x0048, 0x002F, 0x002E, { 16, 10, 154, 94 } },
	{ 0x0049, 0x002D, 0x002C, { 18, 105, 162, 178 } },
	{ 0x004A, 0x002D, 0x002C, { 18, 105, 162, 178 } },
	{ 0x004B, 0x002D, 0x002C, { 16, 51, 184, 124 } },
	{ 0x004C, 0x002D, 0x002C, { 46, 74, 196, 184 } },
	{ 0x004D, 0x002F, 0x002E, { 76, 12, 140, 68 } },
	{ 0x004E, 0x002D, 0x002C, { 24, 96, 140, 152 } }, //bugged
	{ 0x004F, 0x002D, 0x002C, { 24, 96, 140, 152 } }, //bugged
	{ 0x0051, 0x002F, 0x002E, { 16, 10, 154, 94 } },
	{ 0x091A, 0x0000, 0x0000, { 1, 13, 260, 199 } }, //game board
	{ 0x092E, 0x0000, 0x0000, { 1, 13, 260, 199 } } //backgammon game
};
//----------------------------------------------------------------------------
TContainerRect::TContainerRect()
: m_X(100), m_Y(100)
{
}
//----------------------------------------------------------------------------
void TContainerRect::Calculate(WORD gumpID)
{
	//”казатель на текстуру
	TTextureObject *tex = UO->ExecuteGump(gumpID);

	//≈сли текстура есть в пам€ти
	if (tex != NULL)
	{
		//≈сли выключено смещение - открываем гамп в правом верхнем углу клиента
		if (!ConfigManager.OffsetInterfaceWindows)
		{
			m_X = g_ClientWidth - tex->Width;
			m_Y = g_ClientHeight - tex->Height;
		}
		else //»ли вычисл€ем смещение и открываем в результируемых координатах
		{
			int passed = 0;

			//÷икл в 4 итерации и пока не будут валидные координаты
			IFOR(i, 0, 4 && !passed)
			{
				//ѕора измен€ть смещение по оси Y и обнул€ть по оси X
				if (m_X + tex->Width + CONTAINERS_RECT_STEP > g_ClientWidth)
				{
					m_X = CONTAINERS_RECT_DEFAULT_POS;

					//≈сли смещение по оси Y достигло максимума - выставим стандартное значение
					if (m_Y + tex->Height + CONTAINERS_RECT_LINESTEP > g_ClientHeight)
						m_Y = CONTAINERS_RECT_DEFAULT_POS;
					else
						m_Y += CONTAINERS_RECT_LINESTEP;
				}
				//ѕора измен€ть смещение по оси X и обнул€ть по оси Y
				else if (m_Y + tex->Height + CONTAINERS_RECT_STEP > g_ClientHeight)
				{
					//≈сли смещение по оси X достигло максимума - выставим стандартное значение
					if (m_X + tex->Width + CONTAINERS_RECT_LINESTEP > g_ClientWidth)
						m_X = CONTAINERS_RECT_DEFAULT_POS;
					else
						m_X += CONTAINERS_RECT_LINESTEP;

					m_Y = CONTAINERS_RECT_DEFAULT_POS;
				}
				else //¬се отлично, пропускаем дальше
					passed = i + 1;
			}

			if (!passed) //—мещение не вычислено. ¬ыставим значение по-умолчанию
				MakeDefault();
			else if (passed == 1) //ѕрошло с 1 раза, можно изменить смещение
			{
				m_X += CONTAINERS_RECT_STEP;
				m_Y += CONTAINERS_RECT_STEP;
			}
		}
	}
}
//----------------------------------------------------------------------------