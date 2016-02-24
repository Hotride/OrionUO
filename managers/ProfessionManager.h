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