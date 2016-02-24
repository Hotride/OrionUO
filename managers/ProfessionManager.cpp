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
//------------------------------TBaseProfession------------------------------
//---------------------------------------------------------------------------
TBaseProfession_y::TBaseProfession_y()
: m_Name(""), m_TrueName(""), m_DescriptionIndex(0), m_TopLevel(false), m_Gump(0),
m_Type(PT_NO_PROF)
{
}
//---------------------------------------------------------------------------
TBaseProfession_y::~TBaseProfession_y()
{
	m_TextureName.Clear();
	m_TextureDescription.Clear();
}
//---------------------------------------------------------------------------
void TBaseProfession_y::SetName(string val)
{
	m_Name = val;

	FontManager->GenerateW(1, m_TextureName, ToWString(val).c_str(), 0, 30, 185);
}
//---------------------------------------------------------------------------
//-----------------------------TProfessionCategory---------------------------
//---------------------------------------------------------------------------
TProfessionCategory_y::TProfessionCategory_y()
: TBaseProfession_y(), m_Childrens("|")
{
}
//---------------------------------------------------------------------------
TProfessionCategory_y::~TProfessionCategory_y()
{
}
//---------------------------------------------------------------------------
void TProfessionCategory_y::AddChildren(string child)
{
	m_Childrens += child + "|";
}
//---------------------------------------------------------------------------
//---------------------------------TProfession-------------------------------
//---------------------------------------------------------------------------
TProfession_y::TProfession_y()
: TBaseProfession_y(), m_Str(0), m_Int(0), m_Dex(0)
{
	memset(&m_SkillIndex[0], 0, sizeof(m_SkillIndex));
	memset(&m_SkillValue[0], 0, sizeof(m_SkillValue));
}
//---------------------------------------------------------------------------
TProfession_y::~TProfession_y()
{
}
//---------------------------------------------------------------------------
//-----------------------------TProfessionManager----------------------------
//---------------------------------------------------------------------------
TProfessionManager::TProfessionManager()
: TBaseQueue(), m_SelectedProfession(NULL)
{
}
//---------------------------------------------------------------------------
TProfessionManager::~TProfessionManager()
{
}
//---------------------------------------------------------------------------
std::vector<string> TProfessionManager::ParseToTokens(const char *str)
{
	std::vector<string> vec;
	string buf = "";

	int len = strlen(str);
	char *ptr = (char*)str;

	IFOR(i, 0, len)
	{
		if (*ptr == '"')
		{
			for (; i < len; i++)
			{
				ptr++;

				if (*ptr && *ptr == '"')
					break;

				buf.push_back(*ptr);
			}

			if (buf.length())
			{
				vec.push_back(buf);
				buf.clear();
			}

			if (*ptr)
				ptr++;
		}
		else if (*ptr)
		{
			if (*ptr == ' ' || *ptr == '\t' || *ptr == ',')
			{
				if (buf.length())
				{
					vec.push_back(buf);
					buf.clear();
				}

				while (*ptr && (*ptr == ' ' || *ptr == '\t' || *ptr == ','))
					ptr++;
			}
			else
			{
				buf.push_back(*ptr);
				ptr++;
			}
		}
	}

	if (buf.length())
		vec.push_back(buf);

	return vec;
}
//---------------------------------------------------------------------------
int TProfessionManager::GetKeyCode(const char *str)
{
	int index = 0;

	char buf[50] = { 0 };

	int len = strlen(str);
	if (len)
	{
		if (len >= 50)
			len = 49;

		memcpy(&buf[0], &str[0], len);
	}

	char *text = _strlwr(buf);

	if (!memcmp(text, "begin", 5))
		index = PM_CODE_BEGIN;
	else if (!memcmp(text, "name", 4))
		index = PM_CODE_NAME;
	else if (!memcmp(text, "truename", 8))
		index = PM_CODE_TRUENAME;
	else if (!memcmp(text, "desc", 4))
		index = PM_CODE_DESC;
	else if (!memcmp(text, "toplevel", 8))
		index = PM_CODE_TOPLEVEL;
	else if (!memcmp(text, "gump", 4))
		index = PM_CODE_GUMP;
	else if (!memcmp(text, "type", 4))
		index = PM_CODE_TYPE;
	else if (!memcmp(text, "children", 8))
		index = PM_CODE_CHILDREN;
	else if (!memcmp(text, "skill", 5))
		index = PM_CODE_SKILL;
	else if (!memcmp(text, "stat", 4))
		index = PM_CODE_STAT;
	else if (!memcmp(text, "str", 3))
		index = PM_CODE_STR;
	else if (!memcmp(text, "int", 3))
		index = PM_CODE_INT;
	else if (!memcmp(text, "dex", 3))
		index = PM_CODE_DEX;
	else if (!memcmp(text, "end", 3))
		index = PM_CODE_END;
	else if (!memcmp(text, "true", 4))
		index = PM_CODE_TRUE;
	else if (!memcmp(text, "category", 8))
		index = PM_CODE_CATEGORY;

	return index;
}
//---------------------------------------------------------------------------
bool TProfessionManager::ParseFilePart(FILE *file)
{
	PROFESSION_TYPE type = PT_NO_PROF;
	string name = "";
	string trueName = "";
	std::vector<string> childrens;
	int descriptionIndex = 0;
	WORD gump = 0;
	bool topLevel = false;
	int skillCount = 0;
	int skillIndex[4] = { 0xFF, 0xFF, 0xFF, 0xFF };
	int skillValue[4] = { 0 };
	int stats[3] = {0};

	char buf[512] = { 0 };

	while (fgets(buf, 512, file))
	{
		if (!strlen(buf))
			continue;

		buf[strlen(buf) - 1] = 0;

		std::vector<string> tokens = ParseToTokens(buf);
		int code = 0;

		if (tokens.size())
			code = GetKeyCode(tokens[0].c_str());

		if (tokens.size() > 1)
		{
			bool exit = false;

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
					name = tokens[1];
					break;
				}
				case PM_CODE_TRUENAME:
				{
					trueName = tokens[1];
					break;
				}
				case PM_CODE_DESC:
				{
					descriptionIndex = atoi(tokens[1].c_str());
					break;
				}
				case PM_CODE_TOPLEVEL:
				{
					topLevel = (GetKeyCode(tokens[1].c_str()) == PM_CODE_TRUE);
					break;
				}
				case PM_CODE_GUMP:
				{
					gump = atoi(tokens[1].c_str());
					
					UO->ExecuteGump(gump, 0);
					UO->ExecuteGump(gump + 1, 0);
					break;
				}
				case PM_CODE_TYPE:
				{
					if (GetKeyCode(tokens[1].c_str()) == PM_CODE_CATEGORY)
						type = PT_CATEGORY;
					else
						type = PT_PROFESSION;

					break;
				}
				case PM_CODE_CHILDREN:
				{
					IFOR(j, 1, (int)tokens.size())
						childrens.push_back(tokens[j]);

					break;
				}
				case PM_CODE_SKILL:
				{
					if (tokens.size() > 2)
					{
						IFOR(j, 0, 54)
						{
							if (tokens[1] == g_SkillName[j])
							{
								skillIndex[skillCount] = j;
								skillValue[skillCount] = atoi(tokens[2].c_str());
								skillCount++;

								break;
							}
						}
					}

					break;
				}
				case PM_CODE_STAT:
				{
					if (tokens.size() > 2)
					{
						code = GetKeyCode(tokens[1].c_str());
						int val = atoi(tokens[2].c_str());

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

			if (exit)
				break;
		}
		else if (code == PM_CODE_BEGIN || code == PM_CODE_END)
			break;
	}

	TBaseProfession_y *obj = NULL;

	if (type == PT_CATEGORY)
	{
		TProfessionCategory_y *temp = new TProfessionCategory_y();

		IFOR(i, 0, (int)childrens.size())
			temp->AddChildren(childrens[i]);

		obj = temp;
	}
	else if (type == PT_PROFESSION)
	{
		TProfession_y *temp = new TProfession_y();

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
			Add(obj);
		else
		{
			TBaseProfession_y *parent = (TBaseProfession_y*)m_Items;

			while (parent != NULL)
			{
				result = AddChild(parent, obj);

				if (result)
					break;

				parent = (TBaseProfession_y*)parent->m_Next;
			}

			if (!result)
				delete obj;
		}
	}

	return result;
}
//---------------------------------------------------------------------------
bool TProfessionManager::AddChild(TBaseProfession_y *parent, TBaseProfession_y *child)
{
	bool result = false;

	if (parent->Type == PT_CATEGORY)
	{
		TProfessionCategory_y *cat = (TProfessionCategory_y*)parent;

		string check = string("|") + child->GetName() + "|";

		if (cat->Childrens.find(check) != string::npos)
		{
			cat->Add(child);
			result = true;
		}
		else
		{
			TBaseProfession_y *item = (TBaseProfession_y*)cat->m_Items;

			while (item != NULL)
			{
				result = AddChild(item, child);

				if (result)
					break;

				item = (TBaseProfession_y*)item->m_Next;
			}
		}
	}

	return result;
}
//---------------------------------------------------------------------------
bool TProfessionManager::Load()
{
	bool result = false;

	Profession = new TProfessionCategory();

	FILE *file = fopen(FilePath("prof.txt").c_str(), "rt");

	if (file != NULL)
	{
		while (!feof(file))
		{
			char buf[512] = { 0 };
			fgets(buf, 512, file);

			if (!strlen(buf))
				continue;

			buf[strlen(buf) - 1] = 0;
			char *text = _strlwr(buf);

			while (*text && (*text == ' ' || *text == '\t'))
				text++;

			if (!memcmp(text, "begin", 5))
			{
				result = ParseFilePart(file);

				if (!result)
					break;
			}
		}

		fclose(file);

		UO->ExecuteGump(0x15A9, 0);
		UO->ExecuteGump(0x15AA, 0);

		TProfession_y *apc = new TProfession_y();
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

		Add(apc);

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

				//Profession->AddDescription(-2, "parent", ptr);
				ptr += strlen(ptr) + 1;

				IFOR(i, 0, (int)list.size())
				{
					if (!Profession->AddDescription(i - 1, list[i], ptr))
						trace_printf("Failed to add description! (%s)\n", list[i].c_str());

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
//---------------------------------------------------------------------------