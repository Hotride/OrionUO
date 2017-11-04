// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
/***********************************************************************************
**
** SkillGroup.cpp
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#include "stdafx.h"
//----------------------------------------------------------------------------------
CSkillGroupObject::CSkillGroupObject()
{
	WISPFUN_DEBUG("c207_f1");
	memset(m_Items, 0xFF, sizeof(m_Items));
}
//----------------------------------------------------------------------------------
CSkillGroupObject::~CSkillGroupObject()
{
}
//----------------------------------------------------------------------------------
uchar CSkillGroupObject::GetItem(int index)
{
	WISPFUN_DEBUG("c207_f2");
	if (index < 0 || index >= m_Count)
		return 0xFF;

	return m_Items[index];
}
//----------------------------------------------------------------------------------
void CSkillGroupObject::Add(uchar index)
{
	WISPFUN_DEBUG("c207_f3");
	if (Contains(index))
		return;

	m_Items[m_Count] = index;
	m_Count++;
}
//----------------------------------------------------------------------------------
void CSkillGroupObject::AddSorted(uchar index)
{
	WISPFUN_DEBUG("c207_f4");
	if (Contains(index))
		return;

	m_Items[m_Count] = index;
	m_Count++;

	Sort();
}
//----------------------------------------------------------------------------------
void CSkillGroupObject::Remove(uchar index)
{
	WISPFUN_DEBUG("c207_f5");
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
//----------------------------------------------------------------------------------
bool CSkillGroupObject::Contains(uchar index)
{
	WISPFUN_DEBUG("c207_f6");
	IFOR(i, 0, m_Count)
	{
		if (m_Items[i] == index)
			return true;
	}

	return false;
}
//----------------------------------------------------------------------------------
void CSkillGroupObject::Sort()
{
	WISPFUN_DEBUG("c207_f7");
	BYTE table[60] = {0};
	int Ptr = 0;

	IFOR(i, 0, g_SkillsManager.Count)
	{
		IFOR(j, 0, m_Count)
		{
			if (g_SkillsManager.GetSortedIndex(i) == m_Items[j])
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
//----------------------------------------------------------------------------------
void CSkillGroupObject::TransferTo(CSkillGroupObject *group)
{
	WISPFUN_DEBUG("c207_f8");
	IFOR(i, 0, m_Count)
		group->Add(m_Items[i]);

	group->Sort();
}
//----------------------------------------------------------------------------------
