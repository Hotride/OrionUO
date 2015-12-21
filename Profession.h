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
class TBaseProfession
{
private:
	string m_DefName;
	string m_Name;
	bool m_Type; //false - Category; true - Profession
	WORD m_Gump;
	int m_Index;

public:
	TBaseProfession();
	virtual ~TBaseProfession();
	
	TTextTexture m_TextureName;
	TTextTexture m_TextureDescription;

	TProfessionCategory *m_Parent;

	SETGET(string, DefName)
	SETGET(bool, Type)
	SETGET(WORD, Gump)
	SETGET(int, Index)
	
	void SetName(string str);
	
	string GetName() const {return m_Name;}
	
	virtual bool AddDescription(int desc, string name, const char *val);
};
//---------------------------------------------------------------------------
class TProfession : public TBaseProfession
{
private:
	BYTE m_SkillID[4];
	BYTE m_SkillValue[4];
	BYTE m_Str;
	BYTE m_Int;
	BYTE m_Dex;

public:
	TProfession();
	virtual ~TProfession();
	
	SETGET(BYTE, Str)
	SETGET(BYTE, Int)
	SETGET(BYTE, Dex)

	void SetSkillID(int index, BYTE val) {if (index >= 0 && index < 4) m_SkillID[index] = val;}
	void SetSkillValue(int index, BYTE val) {if (index >= 0 && index < 4) m_SkillValue[index] = val;}

	BYTE GetSkillID(int index) const {if (index >= 0 && index < 4) return m_SkillID[index]; else return 0xFF;}
	BYTE GetSkillValue(int index) const {if (index >= 0 && index < 4) return m_SkillValue[index]; else return 0xFF;}
};
//---------------------------------------------------------------------------
class TProfessionCategory : public TBaseProfession
{
private:
public:
	TProfessionCategory();
	virtual ~TProfessionCategory();
	
	string Childs[4];

	int m_CatChildCount;
	TProfessionCategory *m_CatChild[4];

	int m_ProfChildCount;
	TProfession *m_ProfChild[4];
	
	bool AddCategory(TProfessionCategory *pc, bool topObj = false);
	bool Add(TProfession *p);
	void AddChildren(string children);
	bool AddDescription(int desc, string name, const char *val);
};
//---------------------------------------------------------------------------
extern TProfessionCategory *Profession;
extern TProfessionCategory *UsedProfessionCategory;
extern TProfession *UsedProfession;
//---------------------------------------------------------------------------
#endif