/***********************************************************************************
**
** SkillGroup.cpp
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#include "SkillGroup.h"
#include "Skills.h"
#include "Managers/FontsManager.h"
//----------------------------------------------------------------------------------
CSkillGroupObject::CSkillGroupObject()
: m_Count(0), m_Maximized(false), m_Next(NULL), m_Prev(NULL)
{
	memset(m_Items, 0xFF, sizeof(m_Items));
	m_Name = "No Name";
}
//----------------------------------------------------------------------------------
CSkillGroupObject::~CSkillGroupObject()
{
}
//----------------------------------------------------------------------------------
uchar CSkillGroupObject::GetItem(int index)
{
	if (index < 0 || index >= m_Count)
		return 0xFF;

	return m_Items[index];
}
//----------------------------------------------------------------------------------
void CSkillGroupObject::Add(uchar index)
{
	if (Contains(index))
		return;

	m_Items[m_Count] = index;
	m_Count++;
}
//----------------------------------------------------------------------------------
void CSkillGroupObject::AddSorted(uchar index)
{
	if (Contains(index))
		return;

	m_Items[m_Count] = index;
	m_Count++;

	Sort();
}
//----------------------------------------------------------------------------------
void CSkillGroupObject::Remove(uchar index)
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
//----------------------------------------------------------------------------------
bool CSkillGroupObject::Contains(uchar index)
{
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
//----------------------------------------------------------------------------------
void CSkillGroupObject::TransferTo(CSkillGroupObject *group)
{
	IFOR(i, 0, m_Count)
		group->Add(m_Items[i]);

	group->Sort();
}
//----------------------------------------------------------------------------------
