/***********************************************************************************
**
** Profession.h
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#ifndef PROFESSION_H
#define PROFESSION_H
//----------------------------------------------------------------------------------
class CBaseProfession : public CBaseQueueItem
{
	SETGET(string, Name, "");
	SETGET(string, TrueName, "");
	SETGET(wstring, Description, L"");
	SETGET(uint, NameClilocID, 0);
	SETGET(uint, DescriptionClilocID, 0);
	SETGET(PROFESSION_TYPE, Type, PT_NO_PROF);
	SETGET(ushort, Gump, 0);
	SETGET(bool, TopLevel, false);
	SETGET(int, DescriptionIndex, 0);

public:
	CBaseProfession();
	virtual ~CBaseProfession();

	//Добавить описание профессии
	bool AddDescription(int desc, string name, const char *val);
};
//----------------------------------------------------------------------------------
class CProfessionCategory : public CBaseProfession
{
	SETGET(string, Childrens, "|");

public:
	CProfessionCategory();
	virtual ~CProfessionCategory();

	void AddChildren(string child);
};
//----------------------------------------------------------------------------------
class CProfession : public CBaseProfession
{
	SETGET(uchar, Str, 0);
	SETGET(uchar, Int, 0);
	SETGET(uchar, Dex, 0);

private:
	uchar m_SkillIndex[4];
	uchar m_SkillValue[4];

public:
	CProfession();
	virtual ~CProfession();

	void SetSkillIndex(const int &index, const uchar &val) { if (index >= 0 && index < 4) m_SkillIndex[index] = val; }
	void SetSkillValue(const int &index, const uchar &val) { if (index >= 0 && index < 4) m_SkillValue[index] = val; }

	uchar GetSkillIndex(const int &index) const { if (index >= 0 && index < 4) return m_SkillIndex[index]; else return 0xFF; }
	uchar GetSkillValue(const int &index) const { if (index >= 0 && index < 4) return m_SkillValue[index]; else return 0xFF; }
};
//----------------------------------------------------------------------------------
#endif
//----------------------------------------------------------------------------------
