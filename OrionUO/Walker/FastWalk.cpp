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
void CFastWalkStack::SetValue(const int &index, const uint &value)
{
	WISPFUN_DEBUG("c176_f1");
	if (index >= 0 && index < 5)
		m_Keys[index] = value;
}
//----------------------------------------------------------------------------------
void CFastWalkStack::AddValue(const uint &value)
{
	WISPFUN_DEBUG("c176_f2");

	IFOR(i, 0, 5)
	{
		if (!m_Keys[i])
		{
			m_Keys[i] = value;
			break;
		}
	}
}
//---------------------------------------------------------------------------
uint CFastWalkStack::GetValue()
{
	WISPFUN_DEBUG("c176_f3");
	int index = 0;

	IFOR(i, 0, 5)
	{
		uint key = m_Keys[i];

		if (key)
		{
			m_Keys[i] = 0;
			return key;
		}
	}

	return 0;
}
//----------------------------------------------------------------------------------
