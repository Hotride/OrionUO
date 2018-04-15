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
public:
	string Name = "";
	string TrueName = "";
	wstring Description = L"";
	uint NameClilocID = 0;
	uint DescriptionClilocID = 0;
	PROFESSION_TYPE Type = PT_NO_PROF;
	ushort Gump = 0;
	bool TopLevel = false;
	int DescriptionIndex = 0;

	CBaseProfession();
	virtual ~CBaseProfession();

	//Добавить описание профессии
	bool AddDescription(int desc, string name, const char *val);
};
//----------------------------------------------------------------------------------
class CProfessionCategory : public CBaseProfession
{
public:
	string Childrens = "|";

	CProfessionCategory();
	virtual ~CProfessionCategory();

	void AddChildren(string child);
};
//----------------------------------------------------------------------------------
class CProfession : public CBaseProfession
{
public:
	uchar Str = 0;
	uchar Int = 0;
	uchar Dex = 0;

private:
	uchar m_SkillIndex[4];
	uchar m_SkillValue[4];

public:
	CProfession();
	virtual ~CProfession();

	void SetSkillIndex(int index, uchar val) { if (index >= 0 && index < 4) m_SkillIndex[index] = val; }
	void SetSkillValue(int index, uchar val) { if (index >= 0 && index < 4) m_SkillValue[index] = val; }

	uchar GetSkillIndex(int index) const { if (index >= 0 && index < 4) return m_SkillIndex[index]; else return 0xFF; }
	uchar GetSkillValue(int index) const { if (index >= 0 && index < 4) return m_SkillValue[index]; else return 0xFF; }
};
//----------------------------------------------------------------------------------
#endif
//----------------------------------------------------------------------------------
