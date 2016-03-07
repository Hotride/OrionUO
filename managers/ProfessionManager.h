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
//Менеджер профессий
class TProfessionManager : public TBaseQueue
{
private:
	static const int m_KeyCount = 18;

	static const string m_Keys[m_KeyCount];

	enum
	{
		PM_CODE_BEGIN = 1,
		PM_CODE_NAME,
		PM_CODE_TRUENAME,
		PM_CODE_DESC,
		PM_CODE_TOPLEVEL,
		PM_CODE_GUMP,
		PM_CODE_TYPE,
		PM_CODE_CHILDREN,
		PM_CODE_SKILL,
		PM_CODE_STAT,
		PM_CODE_STR,
		PM_CODE_INT,
		PM_CODE_DEX,
		PM_CODE_END,
		PM_CODE_TRUE,
		PM_CODE_CATEGORY,
		PM_CODE_NAME_CLILOC_ID,
		PM_CODE_DESCRIPTION_CLILOC_ID
	};

	int GetKeyCode(const string &key);
	bool ParseFilePart(TTextFileParser &file);
	bool AddChild(TBaseProfession *parent, TBaseProfession *child);
	void LoadProfessionDescription();
public:
	TProfessionManager();
	virtual ~TProfessionManager();

	//Получить указатель на профессию
	TBaseProfession *Selected;
	
	TBaseProfession *GetParent(TBaseProfession *obj, TBaseProfession *check = NULL);

	bool Load();
};
//---------------------------------------------------------------------------
extern TProfessionManager *ProfessionManager;
//---------------------------------------------------------------------------
#endif