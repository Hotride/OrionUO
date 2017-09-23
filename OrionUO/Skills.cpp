/***********************************************************************************
**
** Skills.cpp
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#include "stdafx.h"
//----------------------------------------------------------------------------------
CSkillSort g_SkillSort;
vector<CSkillData> g_Skills;
int g_SkillsCount = 0;
bool g_SkillsRequested = false;
//----------------------------------------------------------------------------------
CSkill::CSkill()
{
}
//----------------------------------------------------------------------------------
bool CSkillSort::SkillCmp(const char *str1, int len1, const char *str2, int len2)
{
	WISPFUN_DEBUG("c208_f1");
	//Вычисляем минимальную длину строки для сравнения
	int len = len1;
	if (len > len2)
		len = len2;

	bool result = false;

	//Пройдемся по всем символам этой строки, сравнивая их друг с другом
	IFOR(i, 0, len)
	{
		if (str1[i] < str2[i])
		{
			result = true;
			break;
		}
		else if (str1[i] > str2[i])
		{
			result = false;
			break;
		}
	}

	//Вернем что получилось
	return result;
}
//----------------------------------------------------------------------------------
void CSkillSort::Init()
{
	WISPFUN_DEBUG("c208_f2");
	//Заполняем все инвалидными значениями
	memset(m_Skills, 0xFF, sizeof(m_Skills));

	//И буфер
	uchar skillBuf[60];
	memset(skillBuf, 0xFF, sizeof(skillBuf));

	//Установим первый элемент нулем и количество обработанных навыков - 1
	int parsed = 1;
	skillBuf[0] = 0;

	//Пройдемся по всем нвыкам (кроме первого)
	IFOR(i, 1, g_SkillsCount)
	{
		//TPRINT("Skill[%d] = %s\n", i, skills[i].SkillName);

		//Пройдемся по обработанным
		IFOR(j, 0, parsed)
		{
			//Если можно вставить в текущую позицию - 
			if (SkillCmp(g_Skills[skillBuf[j]].Name.c_str(), g_Skills[skillBuf[j]].Name.length(), g_Skills[i].Name.c_str(), g_Skills[i].Name.length()))
			{
				//Запомним индекс навыка
				uchar buf = skillBuf[j];
				//Перезапишем
				skillBuf[j] = (uchar)i;

				//К следующему навыку
				j++;

				//Посмотрим остальные обработанные и перезапишем индекс при необходимости
				for(; j < parsed; j++)
				{
					uchar ptr = skillBuf[j];
					skillBuf[j] = buf;
					buf = ptr;
				}

				//Запишем индекс в текущий обработанный
				skillBuf[parsed] = buf;

				//Увеличиваем счетчик
				parsed++;

				break;
			}
		}
	}

	//Перезаписываем индексы навыков
	for (int i = 0, j = parsed - 1; i < parsed; i++, j--)
		m_Skills[i] = skillBuf[j];
}
//----------------------------------------------------------------------------------
string g_SkillName[54] =
{
	"Alchemy",
	"Anatomy",
	"AnimalLore",
	"Item ID",
	"Arms Lore",
	"Parrying",
	"Begging",
	"Blacksmith",
	"Bowcraft",
	"Peacemaking",
	"Camping",
	"Carpentry",
	"Cartography",
	"Cooking",
	"Detect Hidden",
	"Enticement",
	"EvaluateIntelligence",
	"Healing",
	"Fishing",
	"Forensic Evaluation",
	"Herding",
	"Hiding",
	"Provocation",
	"Inscription",
	"Lockpicking",
	"Magery",
	"ResistingSpells",
	"Tactics",
	"Snooping",
	"Musicianship",
	"Poisoning",
	"Archery",
	"Spirit Speak",
	"Stealing",
	"Tailoring",
	"AnimalTaming",
	"Taste Identification",
	"Tinkering",
	"Tracking",
	"Veterinary",
	"Swordsmanship",
	"MaceFighting",
	"Fencing",
	"Wrestling",
	"Lumberjacking",
	"Mining",
	"Meditation",
	"Stealth",
	"Remove Trap",
	"Necromancy",
	"Focus",
	"Chivalry",
	"Bushido",
	"Ninjitsu"
};
//----------------------------------------------------------------------------------
