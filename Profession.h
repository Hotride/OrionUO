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
class TBaseProfession : public TBaseQueueItem
{
private:
	string m_Name;
	string m_TrueName;
	DWORD m_NameClilocID;
	DWORD m_DescriptionClilocID;
	int m_DescriptionIndex;
	bool m_TopLevel;
	WORD m_Gump;
	PROFESSION_TYPE m_Type;
public:
	TBaseProfession();
	virtual ~TBaseProfession();

	SETGET(string, TrueName);
	SETGET(DWORD, NameClilocID);
	SETGET(DWORD, DescriptionClilocID);
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
class TProfessionCategory : public TBaseProfession
{
private:
	string m_Childrens;
public:
	TProfessionCategory();
	virtual ~TProfessionCategory();

	SETGET(string, Childrens);

	void AddChildren(string child);
};
//---------------------------------------------------------------------------
class TProfession : public TBaseProfession
{
private:
	BYTE m_SkillIndex[4];
	BYTE m_SkillValue[4];

	BYTE m_Str;
	BYTE m_Int;
	BYTE m_Dex;
public:
	TProfession();
	virtual ~TProfession();

	SETGET(BYTE, Str);
	SETGET(BYTE, Int);
	SETGET(BYTE, Dex);

	void SetSkillIndex(int index, BYTE val) { if (index >= 0 && index < 4) m_SkillIndex[index] = val; }
	void SetSkillValue(int index, BYTE val) { if (index >= 0 && index < 4) m_SkillValue[index] = val; }

	BYTE GetSkillIndex(int index) const { if (index >= 0 && index < 4) return m_SkillIndex[index]; else return 0xFF; }
	BYTE GetSkillValue(int index) const { if (index >= 0 && index < 4) return m_SkillValue[index]; else return 0xFF; }
};
//---------------------------------------------------------------------------
#endif