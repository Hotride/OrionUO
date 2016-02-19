/****************************************************************************
**
** Profession.h
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
#ifndef ProfessionH
#define ProfessionH
//---------------------------------------------------------------------------
class TProfessionCategory;
//---------------------------------------------------------------------------
//Базовый класс профессии
class TBaseProfession
{
private:
	//Стандартное название профессии
	string m_DefName;

	//Название профессии
	string m_Name;

	//Тип профессии. false - Category; true - Profession
	bool m_Type;

	//Индекс картинки профессии
	WORD m_Gump;

	//Индекс профессии
	int m_Index;

public:
	TBaseProfession();
	virtual ~TBaseProfession();

	//Текстура названия профессии
	TTextTexture m_TextureName;

	//Текстура описания профессии
	TTextTexture m_TextureDescription;

	//Родитель профессии
	TProfessionCategory *m_Parent;

	SETGET(string, DefName);
	SETGET(bool, Type);
	SETGET(WORD, Gump);
	SETGET(int, Index);

	void SetName(string str);

	string GetName() const {return m_Name;}

	//Добавить описание профессии
	virtual bool AddDescription(int desc, string name, const char *val);
};
//---------------------------------------------------------------------------
//Класс профессии
class TProfession : public TBaseProfession
{
private:
	//Навыки профессии
	BYTE m_SkillID[4];

	//Значения навыков профессии
	BYTE m_SkillValue[4];

	//Сила
	BYTE m_Str;

	//Интеллект
	BYTE m_Int;

	//Ловкость
	BYTE m_Dex;

public:
	TProfession();
	virtual ~TProfession();
	
	SETGET(BYTE, Str);
	SETGET(BYTE, Int);
	SETGET(BYTE, Dex);

	void SetSkillID(int index, BYTE val) {if (index >= 0 && index < 4) m_SkillID[index] = val;}
	void SetSkillValue(int index, BYTE val) {if (index >= 0 && index < 4) m_SkillValue[index] = val;}

	BYTE GetSkillID(int index) const {if (index >= 0 && index < 4) return m_SkillID[index]; else return 0xFF;}
	BYTE GetSkillValue(int index) const {if (index >= 0 && index < 4) return m_SkillValue[index]; else return 0xFF;}
};
//---------------------------------------------------------------------------
//Класс категории профессии
class TProfessionCategory : public TBaseProfession
{
private:
public:
	TProfessionCategory();
	virtual ~TProfessionCategory();

	//Имена подкатегорий
	string Childs[4];

	//Количество подкатегорий
	int m_CatChildCount;

	//Ссылки на категории
	TProfessionCategory *m_CatChild[4];

	//Количество профессий
	int m_ProfChildCount;

	//Ссылки на профессии
	TProfession *m_ProfChild[4];

	//Добавить категорию
	bool AddCategory(TProfessionCategory *pc, bool topObj = false);

	//Добавить профессию
	bool Add(TProfession *p);

	//Добавить имя подкатегории
	void AddChildren(string children);

	//Добавить описание
	bool AddDescription(int desc, string name, const char *val);
};
//---------------------------------------------------------------------------
extern TProfessionCategory *Profession;
extern TProfessionCategory *UsedProfessionCategory;
extern TProfession *UsedProfession;
//---------------------------------------------------------------------------
#endif