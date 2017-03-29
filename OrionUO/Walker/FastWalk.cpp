/***********************************************************************************
**
** FastWalk.cpp
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#include "FastWalk.h"
//----------------------------------------------------------------------------------
void CFastWalkStack::Reset()
{
	WISPFUN_DEBUG("c176_f1");
	m_Current = 0;
	m_Deep = 0;
	memset(m_Keys, 0, sizeof(m_Keys));
}
//---------------------------------------------------------------------------
void CFastWalkStack::Push(const uint &key)
{
	WISPFUN_DEBUG("c176_f2");
	if (m_Deep < MAX_FAST_WALK_STACK_SIZE)
		m_Deep++;

	m_Current++;
	if (m_Current > MAX_FAST_WALK_STACK_SIZE)
		m_Current = 0;

	m_Keys[m_Current] = key;
}
//---------------------------------------------------------------------------
uint CFastWalkStack::Pop()
{
	WISPFUN_DEBUG("c176_f3");
	if (m_Deep <= 0)
	{
		m_Deep = 0;
		return 0;
	}

	m_Deep--;
	uint result = m_Keys[m_Current];
	m_Current--;

	if (m_Current < 0)
		m_Current = 255;

	return result;
}
//----------------------------------------------------------------------------------
