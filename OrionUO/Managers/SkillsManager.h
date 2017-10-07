/***********************************************************************************
**
** SkillsManager.h
**
** Copyright (C) Obtober 2017 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#ifndef SKILLSMANAGER_H
#define SKILLSMANAGER_H
//----------------------------------------------------------------------------------
class CSkill
{
	SETGET(bool, Button, 0);
	SETGET(string, Name, "");
	SETGET(float, BaseValue, 0.0f);
	SETGET(float, Value, 0.0f);
	SETGET(float, Cap, 0.0f);
	SETGET(uchar, Status, 0);

public:
	CSkill() {}
	CSkill(const bool &haveButton, const string &name);
	~CSkill() {}
};
//----------------------------------------------------------------------------------
class CSkillsManager
{
	SETGET(int, Count, 0);
	SETGET(bool, SkillsRequested, false);
	SETGET(float, SkillsTotal, 0.0f);

private:
	vector<CSkill> m_Skills;
	UCHAR_LIST m_SortedTable;

	bool CompareName(const string &str1, const string &str2);

public:
	CSkillsManager() {}
	~CSkillsManager() {}

	bool Load();

	void Add(const CSkill &skill);

	void Clear();

	CSkill *Get(const uint &index);

	void Sort();

	int GetSortedIndex(const uint &index);

	void UpdateSkillsSum();
};
//----------------------------------------------------------------------------------
extern CSkillsManager g_SkillsManager;
//----------------------------------------------------------------------------------
#endif //SKILLSMANAGER_H
//----------------------------------------------------------------------------------
