// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
/***********************************************************************************
**
** SkillsManager.cpp
**
** Copyright (C) Obtober 2017 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#include "stdafx.h"
//----------------------------------------------------------------------------------
CSkillsManager g_SkillsManager;
//----------------------------------------------------------------------------------
CSkill::CSkill(const bool &haveButton, const string &name)
: Button(haveButton)
{
	if (name.length())
		Name = name;
	else
		Name = "NoNameSkill";

	//LOG("Skill loaded (button:%i): %s\n", m_Button, m_Name.c_str());
}
//----------------------------------------------------------------------------------
bool CSkillsManager::Load()
{
	WISPFUN_DEBUG("c194_f56");
	if (!g_FileManager.m_SkillsIdx.Size || !g_FileManager.m_SkillsMul.Size || Count)
		return false;

	WISP_FILE::CMappedFile &idx = g_FileManager.m_SkillsIdx;
	WISP_FILE::CMappedFile &mul = g_FileManager.m_SkillsMul;

	while (!idx.IsEOF())
	{
		PSKILLS_IDX_BLOCK idxBlock = (PSKILLS_IDX_BLOCK)idx.Ptr;
		idx.Move(sizeof(SKILLS_IDX_BLOCK));

		if (idxBlock->Size && idxBlock->Position != 0xFFFFFFFF && idxBlock->Size != 0xFFFFFFFF)
		{
			mul.Ptr = mul.Start + idxBlock->Position;

			bool haveButton = (mul.ReadUInt8() != 0);

			Add(CSkill(haveButton, mul.ReadString(idxBlock->Size - 1)));
		}
	}

	LOG("Skills count: %i\n", Count);

	if (Count < 2 || Count > 100)
	{
		Count = 0;

		return false;
	}

	return true;
}
//----------------------------------------------------------------------------------
void CSkillsManager::Add(const CSkill &skill)
{
	m_Skills.push_back(skill);
	Count++;
}
//----------------------------------------------------------------------------------
void CSkillsManager::Clear()
{
	Count = 0;
	SkillsTotal = 0.0f;
	m_Skills.clear();
	m_SortedTable.clear();
}
//----------------------------------------------------------------------------------
CSkill *CSkillsManager::Get(const uint &index)
{
	if (index < (uint)Count)
		return &m_Skills[index];

	return NULL;
}
//----------------------------------------------------------------------------------
bool CSkillsManager::CompareName(const string &str1, const string &str2)
{
	//Вычисляем минимальную длину строки для сравнения
	size_t len = min(str1.length(), str2.length());

	bool result = false;

	//Пройдемся по всем символам этой строки, сравнивая их друг с другом
	IFOR(i, 0, len)
	{
		char c1 = str1.at(i);
		char c2 = str2.at(i);

		if (c1 < c2)
			return true;
		else if (c1 > c2)
			return false;
	}

	//Вернем что получилось
	return result;
}
//----------------------------------------------------------------------------------
void CSkillsManager::Sort()
{
	m_SortedTable.resize(Count, 0xFF);
	UCHAR_LIST bufTable(Count, 0xFF);

	//Установим первый элемент нулем и количество обработанных навыков - 1
	int parsed = 1;
	bufTable[0] = 0;

	//Пройдемся по всем нвыкам (кроме первого)
	IFOR(i, 1, Count)
	{
		//Пройдемся по обработанным
		IFOR(j, 0, parsed)
		{
			//Если можно вставить в текущую позицию - 
			if (CompareName(m_Skills[bufTable[j]].Name, m_Skills[i].Name))
			{
				//Запомним индекс навыка
				uchar buf = bufTable[j];
				//Перезапишем
				bufTable[j] = (uchar)i;

				//К следующему навыку
				j++;

				//Посмотрим остальные обработанные и перезапишем индекс при необходимости
				for (; j < parsed; j++)
				{
					uchar ptr = bufTable[j];
					bufTable[j] = buf;
					buf = ptr;
				}

				//Запишем индекс в текущий обработанный
				bufTable[parsed] = buf;

				//Увеличиваем счетчик
				parsed++;

				break;
			}
		}
	}

	for (int i = 0, j = parsed - 1; i < parsed; i++, j--)
		m_SortedTable[i] = bufTable[j];
}
//----------------------------------------------------------------------------------
int CSkillsManager::GetSortedIndex(const uint &index)
{
	if (index < (uint)Count)
		return m_SortedTable[index];

	return -1;
}
//----------------------------------------------------------------------------------
void CSkillsManager::UpdateSkillsSum()
{
	SkillsTotal = 0.0f;

	for (const CSkill &skill : m_Skills)
		SkillsTotal += skill.Value;
}
//----------------------------------------------------------------------------------
