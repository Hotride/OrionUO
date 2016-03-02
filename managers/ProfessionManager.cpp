/****************************************************************************
**
** ProfessionManager.cpp
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
#include "stdafx.h"

TProfessionManager *ProfessionManager = NULL;
//---------------------------------------------------------------------------
const string TProfessionManager::m_Keys[m_KeyCount] =
{
	"begin",
	"name",
	"truename",
	"desc",
	"toplevel",
	"gump",
	"type",
	"children",
	"skill",
	"stat",
	"str",
	"int",
	"dex",
	"end",
	"true",
	"category"
};
//---------------------------------------------------------------------------
TProfessionManager::TProfessionManager()
: TBaseQueue(), Selected(NULL)
{
}
//---------------------------------------------------------------------------
TProfessionManager::~TProfessionManager()
{
}
//---------------------------------------------------------------------------
int TProfessionManager::GetKeyCode(const string &key)
{
	string str = ToLowerA(key);
	int result = 0;

	IFOR(i, 0, m_KeyCount && !result)
	{
		if (str == m_Keys[i])
			result = i + 1;
	}

	return result;
}
//---------------------------------------------------------------------------
bool TProfessionManager::ParseFilePart(TTextFileParser &file)
{
	PROFESSION_TYPE type = PT_NO_PROF;
	std::vector<string> childrens;
	string name = "";
	string trueName = "";
	int descriptionIndex = 0;
	WORD gump = 0;
	bool topLevel = false;
	int skillCount = 0;
	int skillIndex[4] = { 0xFF, 0xFF, 0xFF, 0xFF };
	int skillValue[4] = { 0 };
	int stats[3] = { 0 };

	bool exit = false;

	while (!file.IsEOF() && !exit)
	{
		vector<string> strings = file.ReadTokens();

		if (!strings.size())
			continue;

		int code = GetKeyCode(strings[0]);

		switch (code)
		{
			case PM_CODE_BEGIN:
			case PM_CODE_END:
			{
				exit = true;
				break;
			}
			case PM_CODE_NAME:
			{
				name = strings[1];
				break;
			}
			case PM_CODE_TRUENAME:
			{
				trueName = strings[1];
				break;
			}
			case PM_CODE_DESC:
			{
				descriptionIndex = atoi(strings[1].c_str());
				break;
			}
			case PM_CODE_TOPLEVEL:
			{
				topLevel = (GetKeyCode(strings[1]) == PM_CODE_TRUE);
				break;
			}
			case PM_CODE_GUMP:
			{
				gump = atoi(strings[1].c_str());

				UO->ExecuteGump(gump, 0);
				UO->ExecuteGump(gump + 1, 0);
				break;
			}
			case PM_CODE_TYPE:
			{
				if (GetKeyCode(strings[1]) == PM_CODE_CATEGORY)
					type = PT_CATEGORY;
				else
					type = PT_PROFESSION;

				break;
			}
			case PM_CODE_CHILDREN:
			{
				IFOR(j, 1, (int)strings.size())
					childrens.push_back(strings[j]);

				break;
			}
			case PM_CODE_SKILL:
			{
				if (strings.size() > 2 && skillCount < 4)
				{
					IFOR(j, 0, 54)
					{
						if (strings[1] == g_SkillName[j])
						{
							skillIndex[skillCount] = j;
							skillValue[skillCount] = atoi(strings[2].c_str());
							skillCount++;

							break;
						}
					}
				}

				break;
			}
			case PM_CODE_STAT:
			{
				if (strings.size() > 2)
				{
					code = GetKeyCode(strings[1]);
					int val = atoi(strings[2].c_str());

					if (code == PM_CODE_STR)
						stats[0] = val;
					else if (code == PM_CODE_INT)
						stats[1] = val;
					else if (code == PM_CODE_DEX)
						stats[2] = val;
				}

				break;
			}
			default:
				break;
		}
	}

	TBaseProfession *obj = NULL;

	if (type == PT_CATEGORY)
	{
		TProfessionCategory *temp = new TProfessionCategory();

		IFOR(i, 0, (int)childrens.size())
			temp->AddChildren(childrens[i]);

		obj = temp;
	}
	else if (type == PT_PROFESSION)
	{
		TProfession *temp = new TProfession();

		temp->Str = stats[0];
		temp->Int = stats[1];
		temp->Dex = stats[2];

		IFOR(i, 0, 4)
		{
			temp->SetSkillIndex(i, (BYTE)skillIndex[i]);
			temp->SetSkillValue(i, (BYTE)skillValue[i]);
		}

		obj = temp;
	}

	bool result = (type != PT_NONE);

	if (obj != NULL)
	{
		obj->SetName(name);
		obj->TrueName = trueName;
		obj->DescriptionIndex = descriptionIndex;
		obj->TopLevel = topLevel;
		obj->Gump = gump;
		obj->Type = type;

		if (topLevel)
			m_Items->Add(obj);
		else
		{
			TBaseProfession *parent = (TBaseProfession*)m_Items;

			while (parent != NULL)
			{
				result = AddChild(parent, obj);

				if (result)
					break;

				parent = (TBaseProfession*)parent->m_Next;
			}

			if (!result)
				delete obj;
		}
	}

	return result;
}
//---------------------------------------------------------------------------
bool TProfessionManager::AddChild(TBaseProfession *parent, TBaseProfession *child)
{
	bool result = false;

	if (parent->Type == PT_CATEGORY)
	{
		TProfessionCategory *cat = (TProfessionCategory*)parent;

		string check = string("|") + child->GetName() + "|";

		if (cat->Childrens.find(check) != string::npos)
		{
			cat->Add(child);
			result = true;
		}
		else
		{
			TBaseProfession *item = (TBaseProfession*)cat->m_Items;

			while (item != NULL)
			{
				result = AddChild(item, child);

				if (result)
					break;

				item = (TBaseProfession*)item->m_Next;
			}
		}
	}

	return result;
}
//---------------------------------------------------------------------------
bool TProfessionManager::Load()
{
	bool result = false;

	TProfessionCategory *head = new TProfessionCategory();
	head->SetTrueName("parent");
	head->SetName("Parent");
	head->DescriptionIndex = -2;
	head->Type = PT_CATEGORY;
	head->Gump = 0x15A9;
	head->TopLevel = true;
	Add(head);

	TTextFileParser file(FilePath("prof.txt").c_str(), " \t,", "#;", "\"\"");

	if (!file.IsEOF())
	{
		while (!file.IsEOF())
		{
			std::vector<std::string> strings = file.ReadTokens();

			if (strings.size() > 0)
			{
				if (ToLowerA(strings[0]) == string("begin"))
				{
					result = ParseFilePart(file);

					if (!result)
						break;
				}

			}
		}

		UO->ExecuteGump(0x15A9, 0);
		UO->ExecuteGump(0x15AA, 0);

		TProfession *apc = new TProfession();
		apc->SetTrueName("advanced");
		apc->SetName("Advanced");
		apc->Type = PT_PROFESSION;
		apc->Gump = 0x15A9;
		apc->DescriptionIndex = -1;
		apc->Str = 44;
		apc->Int = 10;
		apc->Dex = 11;
		apc->SetSkillIndex(0, 0xFF);
		apc->SetSkillIndex(1, 0xFF);
		apc->SetSkillIndex(2, 0xFF);
		apc->SetSkillIndex(3, 0xFF);
		apc->SetSkillValue(0, 50);
		apc->SetSkillValue(1, 50);
		apc->SetSkillValue(2, 0);
		apc->SetSkillValue(3, 0);

		head->Add(apc);

		LoadProfessionDescription();
	}
	else
	{
		TPRINT("Failed to load prof.txt\n");
		MessageBoxA(g_hWnd, "Failed to load prof.txt", "Failed to load", MB_OK);
	}

	return result;
}
//--------------------------------------------------------------------------
void TProfessionManager::LoadProfessionDescription()
{
	TMappedHeader file;
	memset(&file, 0, sizeof(file));

	if (FileManager.LoadFileToMemory(file, FilePath("professn.enu").c_str()))
	{
		char *ptr = (char*)file.Address;
		char *end = (char*)((DWORD)file.Address + file.Size);

		vector<string> list;

		while (ptr < end)
		{
			if (!memcmp(ptr, "TEXT", 4))
			{
				ptr += 8;

				while (ptr < end)
				{
					if (!(*(ptr - 1)) && !(*ptr)) //end of names section
					{
						ptr++;

						break;
					}

					list.push_back(ptr);
					ptr += strlen(ptr) + 1;
				}

				((TBaseProfession*)m_Items)->AddDescription(-2, "parent", ptr);
				ptr += strlen(ptr) + 1;

				IFOR(i, 0, (int)list.size())
				{
					if (!((TBaseProfession*)m_Items)->AddDescription(i - 1, list[i], ptr))
						TPRINT("Failed to add description! (%s)\n", list[i].c_str());

					ptr += strlen(ptr) + 1;
				}

				break;
			}

			ptr++;
		}

		list.clear();

		FileManager.UnloadFileFromMemory(file);
	}
	else
	{
		TPRINT("Failed to load professn.enu\n");
		MessageBoxA(g_hWnd, "Failed to load professn.enu", "Failed to load", MB_OK);
	}
}
//--------------------------------------------------------------------------
TBaseProfession *TProfessionManager::GetParent(TBaseProfession *obj, TBaseProfession *check)
{
	if (check == NULL)
		check = (TBaseProfession*)m_Items;

	if (obj == m_Items)
		return obj;

	TBaseProfession *item = (TBaseProfession*)check->m_Items;
	TBaseProfession *result = NULL;

	while (item != NULL && result == NULL)
	{
		if (obj == item)
			result = check;
		else
			result = GetParent(obj, item);

		item = (TBaseProfession*)item->m_Next;
	}

	return result;
}
//---------------------------------------------------------------------------