/***********************************************************************************
**
** WalkStack.cpp
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#include "WalkStack.h"
//----------------------------------------------------------------------------------
void CWalkStack::Clear()
{
	CWalkData *top = m_Items;
	while (top != NULL)
	{
		CWalkData *next = top->m_Next;
		delete top;
		top = next;
	}

	m_Items = NULL;
}
//----------------------------------------------------------------------------------
void CWalkStack::Push(CWalkData *obj)
{
	if (Empty())
	{
		m_Items = obj;
		m_Items->m_Prev = NULL;
		m_Items->m_Next = NULL;
	}
	else
	{
		CWalkData *top = m_Items;
		while (top->m_Next != NULL)
			top = top->m_Next;

		top->m_Next = obj;
		obj->m_Next = NULL;
		obj->m_Prev = top;
	}
}
//----------------------------------------------------------------------------------
void CWalkStack::Pop()
{
	if (!Empty())
	{
		if (m_Items->m_Next != NULL)
		{
			CWalkData *next = m_Items->m_Next;
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
//----------------------------------------------------------------------------------
CWalkData *CWalkStack::Top()
{
	CWalkData *obj = m_Items;

	if (obj != NULL)
	{
		while (obj->m_Next != NULL)
			obj = obj->m_Next;
	}

	return obj;
}
//----------------------------------------------------------------------------------
