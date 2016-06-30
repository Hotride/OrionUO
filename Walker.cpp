/****************************************************************************
**
** Walker.cpp
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
//---------------------------------------------------------------------------
#include "stdafx.h"

TWalker *Walker = NULL;
//---------------------------------------------------------------------------
//-----------------------------TFastWalkStack-------------------------------
//---------------------------------------------------------------------------
void TFastWalkStack::Reset()
{
	m_Current = 0;
	m_Deep = 0;
	memset(m_Keys, 0, sizeof(m_Keys));
}
//---------------------------------------------------------------------------
void TFastWalkStack::Push(DWORD key)
{
	if (m_Deep < MAX_FAST_WALK_STACK_SIZE)
		m_Deep++;

	m_Current++;
	if (m_Current > MAX_FAST_WALK_STACK_SIZE)
		m_Current = 0;

	m_Keys[m_Current] = key;
}
//---------------------------------------------------------------------------
DWORD TFastWalkStack::Pop()
{
	if (m_Deep <= 0)
	{
		m_Deep = 0;
		return 0;
	}

	m_Deep--;
	DWORD res = m_Keys[m_Current];
	m_Current--;

	if (m_Current < 0)
		m_Current = 255;

	return res;
}
//---------------------------------------------------------------------------
//--------------------------------TWalker-----------------------------------
//---------------------------------------------------------------------------
TWalker::TWalker()
: m_Sequence(0)
{
	memset(m_Direction, 0, sizeof(m_Direction));
	m_FastWalkStack.Reset();
}
//---------------------------------------------------------------------------
void TWalker::IncSequence()
{
	if (m_Sequence == 255)
		m_Sequence = 1;
	else
		m_Sequence++;
}
//---------------------------------------------------------------------------
void TWalker::SetSequence(BYTE Seq, BYTE dir)
{
	m_Sequence = Seq;
	m_Direction[m_Sequence] = dir;
}
//---------------------------------------------------------------------------
//--------------------------------TWalkData---------------------------------
//---------------------------------------------------------------------------
void TWalkData::GetOffset(float &x, float &y, float &steps)
{
	BYTE dir = m_Direction + 1;
	
	float step_NESW_D = 44.0f / steps; //NW NE SW SE
	float step_NESW = 22.0f / steps; //N E S W

	switch (dir & 7)
	{
		case 0: //NW
		{
			x = 0.0f;
			y *= -step_NESW_D;
			break;
		}
		case 1: //N
		{
			x *= step_NESW;
			y *= -step_NESW;
			break;
		}
		case 2: //NE
		{
			x *= step_NESW_D;
			y = 0.0f;
			break;
		}
		case 3: //E
		{
			x *= step_NESW;
			y *= step_NESW;
			break;
		}
		case 4: //SE
		{
			x = 0.0f;
			y *= step_NESW_D;
			break;
		}
		case 5: //S
		{
			x *= -step_NESW;
			y *= step_NESW;
			break;
		}
		case 6: //SW
		{
			x *= -step_NESW_D;
			y = 0.0f;
			break;
		}
		case 7: //W
		{
			x *= -step_NESW;
			y *= -step_NESW;
			break;
		}
		default:
			break;
	}
}
//---------------------------------------------------------------------------
//-------------------------------TWalkStack---------------------------------
//---------------------------------------------------------------------------
void TWalkStack::Clear()
{
	TWalkData *top = m_Items;
	while (top != NULL)
	{
		TWalkData *next = top->m_Next;
		delete top;
		top = next;
	}

	m_Items = NULL;
}
//---------------------------------------------------------------------------
void TWalkStack::Push(TWalkData *obj)
{
	if (Empty())
	{
		m_Items = obj;
		m_Items->m_Prev = NULL;
		m_Items->m_Next = NULL;
	}
	else
	{
		TWalkData *top = m_Items;
		while (top->m_Next != NULL)
			top = top->m_Next;

		top->m_Next = obj;
		obj->m_Next = NULL;
		obj->m_Prev = top;
	}
}
//---------------------------------------------------------------------------
void TWalkStack::Pop()
{
	if (!Empty())
	{
		if (m_Items->m_Next != NULL)
		{
			TWalkData *next = m_Items->m_Next;
			delete m_Items;
			m_Items = next;
			m_Items->m_Prev = NULL;
		}
		else
		{
			delete m_Items;
			m_Items = NULL;
		}
	}
}
//---------------------------------------------------------------------------
TWalkData *TWalkStack::Top()
{
	TWalkData *obj = m_Items;

	if (obj != NULL)
	{
		while (obj->m_Next != NULL)
			obj = obj->m_Next;
	}

	return obj;
}
//---------------------------------------------------------------------------