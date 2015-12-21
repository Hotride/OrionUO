/****************************************************************************
**
** SkillGroupManager.cpp
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
//----------------------------------------------------------------------------
TSkillGroupObject::TSkillGroupObject()
: m_Count(0), m_Maximized(false), m_Selected(0), m_Next(NULL), m_Prev(NULL)
{
	memset(m_Items, 0xFF, sizeof(m_Items));
	m_Name = "No Name";
}
//----------------------------------------------------------------------------
TSkillGroupObject::~TSkillGroupObject()
{
	m_Texture.Clear();
}
//----------------------------------------------------------------------------
void TSkillGroupObject::SetName(string val)
{
	m_Name = val;
	m_Texture.Clear();

	if (val.length())
		FontManager->GenerateA(6, m_Texture, m_Name.c_str(), 0);
}
//----------------------------------------------------------------------------
BYTE TSkillGroupObject::GetItem(int index)
{
	if (index < 0 || index >= m_Count)
		return 0xFF;

	return m_Items[index];
}
//----------------------------------------------------------------------------
void TSkillGroupObject::Add(BYTE index)
{
	if (Contains(index))
		return;

	m_Items[m_Count] = index;
	m_Count++;
}
//----------------------------------------------------------------------------
void TSkillGroupObject::AddSorted(BYTE index)
{
	if (Contains(index))
		return;

	m_Items[m_Count] = index;
	m_Count++;

	Sort();
}
//----------------------------------------------------------------------------
void TSkillGroupObject::Remove(BYTE index)
{
	bool removed = false;

	IFOR(i, 0, m_Count)
	{
		if (m_Items[i] == index)
		{
			removed = true;

			for(; i < m_Count - 1; i++)
				m_Items[i] = m_Items[i + 1];

			break;
		}
	}

	if (removed)
	{
		m_Count--;
		if (m_Count < 0)
			m_Count = 0;

		m_Items[m_Count] = 0xFF;
	}
}
//----------------------------------------------------------------------------
bool TSkillGroupObject::Contains(BYTE index)
{
	IFOR(i, 0, m_Count)
	{
		if (m_Items[i] == index)
			return true;
	}

	return false;
}
//----------------------------------------------------------------------------
void TSkillGroupObject::Sort()
{
	BYTE table[60] = {0};
	int Ptr = 0;

	IFOR(i, 0, g_SkillsCount)
	{
		IFOR(j, 0, m_Count)
		{
			if (g_SkillSort.m_Skills[i] == m_Items[j])
			{
				table[Ptr] = m_Items[j];
				Ptr++;
				break;
			}
		}
	}
	
	IFOR(j, 0, m_Count)
		m_Items[j] = table[j];
}
//----------------------------------------------------------------------------
void TSkillGroupObject::TransferTo(TSkillGroupObject *group)
{
	IFOR(i, 0, m_Count)
		group->Add(m_Items[i]);

	group->Sort();
}
//----------------------------------------------------------------------------