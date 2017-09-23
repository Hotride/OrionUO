/***********************************************************************************
**
** Skills.h
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#ifndef SKILLS_H
#define SKILLS_H
//----------------------------------------------------------------------------------
//Класс навыка
class CSkill
{
	SETGET(float, BaseValue, 0.0f);
	SETGET(float, Value, 0.0f);
	SETGET(float, Cap, 0.0f);
	SETGET(uchar, Status, 0);

public:
	CSkill();
	~CSkill() {}
};
//----------------------------------------------------------------------------------
//Класс скилл сортера
class CSkillSort
{
private:
	//Сравнение названия навыка
	bool SkillCmp(const char *str1, int len1, const char *str2, int len2);

public:
	CSkillSort() {}
	~CSkillSort() {}

	//Инициализация
	void Init();

	//Буфер сортировки
	uchar m_Skills[60];
};
//----------------------------------------------------------------------------------
class CSkillData
{
	SETGET(uchar, Button, 0);
	SETGET(string, Name, "");

public:
	CSkillData() :m_Button(0), m_Name("") {}
	~CSkillData() {};
};
//----------------------------------------------------------------------------------
extern CSkillSort g_SkillSort; //Указатель на скилл сортер
extern vector<CSkillData> g_Skills; //Указатель на навыки
extern int g_SkillsCount; //Количество доступных навыков
extern string g_SkillName[54]; //Стандартные названия навыков
extern bool g_SkillsRequested;
//----------------------------------------------------------------------------------
#endif
//----------------------------------------------------------------------------------
