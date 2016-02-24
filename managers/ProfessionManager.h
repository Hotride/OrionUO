/****************************************************************************
**
** ProfessionManager.h
**
** Copyright (C) February 2016 Hotride
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
#ifndef ProfessionManagerH
#define ProfessionManagerH
//---------------------------------------------------------------------------
class TBaseProfession_y : public TBaseQueueItem
{
private:
	string m_Name;
	string m_TrueName;
	int m_DescriptionIndex;
	bool m_TopLevel;
	WORD m_Gump;
	PROFESSION_TYPE m_Type;
public:
	TBaseProfession_y();
	virtual ~TBaseProfession_y();

	SETGET(string, TrueName);
	SETGET(PROFESSION_TYPE, Type);
	SETGET(WORD, Gump);
	SETGET(bool, TopLevel);
	SETGET(int, DescriptionIndex);

	void SetName(string str);

	string GetName() const { return m_Name; }

	//Текстура названия профессии
	TTextTexture m_TextureName;

	//Текстура описания профессии
	TTextTexture m_TextureDescription;

	//Добавить описание профессии
	bool AddDescription(int desc, string name, const char *val);
};
//---------------------------------------------------------------------------
class TProfessionCategory_y : public TBaseProfession_y
{
private:
	string m_Childrens;
public:
	TProfessionCategory_y();
	virtual ~TProfessionCategory_y();

	SETGET(string, Childrens);

	void AddChildren(string child);
};
//---------------------------------------------------------------------------
class TProfession_y : public TBaseProfession_y
{
private:
	BYTE m_SkillIndex[4];
	BYTE m_SkillValue[4];

	BYTE m_Str;
	BYTE m_Int;
	BYTE m_Dex;
public:
	TProfession_y();
	virtual ~TProfession_y();

	SETGET(BYTE, Str);
	SETGET(BYTE, Int);
	SETGET(BYTE, Dex);

	void SetSkillIndex(int index, BYTE val) { if (index >= 0 && index < 4) m_SkillIndex[index] = val; }
	void SetSkillValue(int index, BYTE val) { if (index >= 0 && index < 4) m_SkillValue[index] = val; }

	BYTE GetSkillIndex(int index) const { if (index >= 0 && index < 4) return m_SkillIndex[index]; else return 0xFF; }
	BYTE GetSkillValue(int index) const { if (index >= 0 && index < 4) return m_SkillValue[index]; else return 0xFF; }
};
//---------------------------------------------------------------------------
//Менеджер профессий
class TProfessionManager : public TBaseQueue
{
private:
	TBaseProfession_y *m_SelectedProfession;

	static const int PM_CODE_BEGIN = 1;
	static const int PM_CODE_NAME = 2;
	static const int PM_CODE_TRUENAME = 3;
	static const int PM_CODE_DESC = 4;
	static const int PM_CODE_TOPLEVEL = 5;
	static const int PM_CODE_GUMP = 6;
	static const int PM_CODE_TYPE = 7;
	static const int PM_CODE_CHILDREN = 8;
	static const int PM_CODE_SKILL = 9;
	static const int PM_CODE_STAT = 10;
	static const int PM_CODE_STR = 11;
	static const int PM_CODE_INT = 12;
	static const int PM_CODE_DEX = 13;
	static const int PM_CODE_END = 14;
	static const int PM_CODE_TRUE = 15;
	static const int PM_CODE_CATEGORY = 16;

	std::vector<string> TProfessionManager::ParseToTokens(const char *str);
	int GetKeyCode(const char *str);
	bool ParseFilePart(FILE *file);
	bool AddChild(TBaseProfession_y *parent, TBaseProfession_y *child);
	void LoadProfessionDescription();
public:
	TProfessionManager();
	virtual ~TProfessionManager();

	//Текстура описания профессии
	TTextTexture m_TextureDescription;

	//Получить указатель на профессию
	TBaseProfession_y *GetSelected() { return m_SelectedProfession; }

	bool Load();
};
//---------------------------------------------------------------------------
extern TProfessionManager *ProfessionManager;
//---------------------------------------------------------------------------
#endif