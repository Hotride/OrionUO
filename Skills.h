/****************************************************************************
**
** Skills.h
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
#ifndef SkillsH
#define SkillsH
//--------------------------------------------------------------------------
class TSkill
{
private:
	float m_BaseValue; //Реальное значение навыка
	float m_Value; //Значение навыка
	float m_Cap; //Максимальное значение навыка
	BYTE m_Status; //Статус прокачки навыка (вверх/вниз/лок)

public:
	TSkill();
	~TSkill() {}
	
	SETGET(float, BaseValue)
	SETGET(float, Value)
	SETGET(float, Cap)
	SETGET(BYTE, Status)
};
//--------------------------------------------------------------------------
class TSkillSort
{
private:
	//Сравнение названия навыка
	bool SkillCmp(const char *str1, int len1, const char *str2, int len2);

public:
	TSkillSort() {}
	~TSkillSort() {}

	//Инициализация
	void Init();

	//Буфер сортировки
	BYTE m_Skills[60];
};
//---------------------------------------------------------------------------
class SKILLS_DATA
{
private:
public:
	//Кнопка (есть/нет)
	BYTE m_Button;
	//Название навыка
	string m_Name;
	//Текстура названия
	TTextTexture m_Texture;

	SKILLS_DATA() :m_Button(0), m_Name("") {memset(&m_Texture, 0, sizeof(m_Texture));}
	~SKILLS_DATA() {m_Texture.Clear();};
};
typedef SKILLS_DATA *PSKILLS_DATA;
//--------------------------------------------------------------------------
extern TSkillSort g_SkillSort; //Указатель на скилл сортер
extern PSKILLS_DATA g_Skills; //Указатель на навыки
extern int g_SkillsCount; //Количество доступных навыков
extern string g_SkillName[54]; //Стандартные названия навыков
//--------------------------------------------------------------------------
#endif