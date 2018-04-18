// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
/***********************************************************************************
**
** ProfessionManager.cpp
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#include "stdafx.h"
//----------------------------------------------------------------------------------
CProfessionManager g_ProfessionManager;
//----------------------------------------------------------------------------------
//!Ключи конфига
const string CProfessionManager::m_Keys[m_KeyCount] =
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
	"category",
	"nameid",
	"descid"
};
//----------------------------------------------------------------------------------
CProfessionManager::CProfessionManager()
: CBaseQueue(), Selected(NULL)
{
}
//----------------------------------------------------------------------------------
CProfessionManager::~CProfessionManager()
{
}
//----------------------------------------------------------------------------------
/*!
Получить код кофига по строке
@param [__in] key Строка из файла
@return Код конфига
*/
int CProfessionManager::GetKeyCode(const string &key)
{
	WISPFUN_DEBUG("c153_f1");
	string str = ToLowerA(key);
	int result = 0;

	IFOR(i, 0, m_KeyCount && !result)
	{
		if (str == m_Keys[i])
			result = (int)i + 1;
	}

	return result;
}
//----------------------------------------------------------------------------------
/*!
Обработать часть файла (загрузка профессии)
@param [__in] file Указатель на файл
@return true при успешной обработке
*/
bool CProfessionManager::ParseFilePart(WISP_FILE::CTextFileParser &file)
{
	WISPFUN_DEBUG("c153_f2");
	PROFESSION_TYPE type = PT_NO_PROF;
	std::vector<string> childrens;
	string name = "";
	string trueName = "";
	uint nameClilocID = 0;
	uint descriptionClilocID = 0;
	int descriptionIndex = 0;
	ushort gump = 0;
	bool topLevel = false;
	int skillCount = 0;
	int skillIndex[4] = { 0xFF, 0xFF, 0xFF, 0xFF };
	int skillValue[4] = { 0, 0, 0, 0 };
	int stats[3] = { 0 };

	bool exit = false;

	while (!file.IsEOF() && !exit)
	{
		STRING_LIST strings = file.ReadTokens();

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

				g_Orion.ExecuteGump(gump);
				g_Orion.ExecuteGump(gump + 1);
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
						CSkill *skillPtr = g_SkillsManager.Get((uint)j);

						if (skillPtr != NULL && strings[1] == skillPtr->Name)
						{
							skillIndex[skillCount] = (int)j;
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
			case PM_CODE_NAME_CLILOC_ID:
			{
				nameClilocID = atoi(strings[1].c_str());
				name = ToUpperA(g_ClilocManager.Cliloc(g_Language)->GetA(nameClilocID, true, name));
				break;
			}
			case PM_CODE_DESCRIPTION_CLILOC_ID:
			{
				descriptionClilocID = atoi(strings[1].c_str());
				break;
			}
			default:
				break;
		}
	}

	CBaseProfession *obj = NULL;

	if (type == PT_CATEGORY)
	{
		CProfessionCategory *temp = new CProfessionCategory();

		IFOR(i, 0, (int)childrens.size())
			temp->AddChildren(childrens[i]);

		obj = temp;
	}
	else if (type == PT_PROFESSION)
	{
		CProfession *temp = new CProfession();

		temp->Str = stats[0];
		temp->Int = stats[1];
		temp->Dex = stats[2];

		IFOR(i, 0, 4)
		{
			temp->SetSkillIndex((int)i, (uchar)skillIndex[i]);
			temp->SetSkillValue((int)i, (uchar)skillValue[i]);
		}

		obj = temp;
	}

	bool result = (type != PROFESSION_TYPE::PT_NO_PROF);

	if (obj != NULL)
	{
		obj->NameClilocID = nameClilocID;
		obj->Name = name;
		obj->TrueName = trueName;
		obj->DescriptionClilocID = descriptionClilocID;
		obj->DescriptionIndex = descriptionIndex;
		obj->TopLevel = topLevel;
		obj->Gump = gump;
		obj->Type = type;

		if (topLevel)
			m_Items->Add(obj);
		else
		{
			CBaseProfession *parent = (CBaseProfession*)m_Items;

			while (parent != NULL)
			{
				result = AddChild(parent, obj);

				//if (result)
				//	break;

				parent = (CBaseProfession*)parent->m_Next;
			}

			if (!result)
				delete obj;
		}
	}

	return result;
}
//----------------------------------------------------------------------------------
/*!
Добавить ребенка в профессию
@param [__in] parent Ссылка на родителя
@param [__in] child Ссылка на детишку
@return true в случае успешного добавления
*/
bool CProfessionManager::AddChild(CBaseProfession *parent, CBaseProfession *child)
{
	WISPFUN_DEBUG("c153_f3");
	bool result = false;

	if (parent->Type == PT_CATEGORY)
	{
		CProfessionCategory *cat = (CProfessionCategory*)parent;

		string check = string("|") + child->Name + "|";

		if (cat->Childrens.find(check) != string::npos)
		{
			cat->Add(child);
			result = true;
		}
		else
		{
			CBaseProfession *item = (CBaseProfession*)cat->m_Items;

			while (item != NULL)
			{
				result = AddChild(item, child);

				if (result)
					break;

				item = (CBaseProfession*)item->m_Next;
			}
		}
	}

	return result;
}
//----------------------------------------------------------------------------------
/*!
Загрузка профессий из файла
@return true в случае успешной загрузки
*/
bool CProfessionManager::Load()
{
	WISPFUN_DEBUG("c153_f4");
	bool result = false;

	CProfessionCategory *head = new CProfessionCategory();
	head->TrueName = "parent";
	head->Name = "Parent";
	head->DescriptionIndex = -2;
	head->Type = PT_CATEGORY;
	head->Gump = 0x15A9;
	head->TopLevel = true;
	Add(head);

	WISP_FILE::CTextFileParser file(g_App.UOFilesPath("prof.txt"), " \t,", "#;", "\"\"");

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

		g_Orion.ExecuteGump(0x15A9);
		g_Orion.ExecuteGump(0x15AA);

		CProfession *apc = new CProfession();
		apc->TrueName = "advanced";
		apc->Name = "Advanced";
		apc->Type = PT_PROFESSION;
		apc->Gump = 0x15A9;
		apc->DescriptionIndex = -1;
		apc->SetSkillIndex(0, 0xFF);
		apc->SetSkillIndex(1, 0xFF);
		apc->SetSkillIndex(2, 0xFF);
		apc->SetSkillIndex(3, 0xFF);

		if (g_PacketManager.GetClientVersion() >= CV_70160)
		{
			apc->Str = 45;
			apc->Int = 35;
			apc->Dex = 10;

			apc->SetSkillValue(0, 30);
			apc->SetSkillValue(1, 30);
			apc->SetSkillValue(2, 30);
			apc->SetSkillValue(3, 30);
		}
		else
		{
			apc->Str = 44;
			apc->Int = 10;
			apc->Dex = 11;

			apc->SetSkillValue(0, 50);
			apc->SetSkillValue(1, 50);
			apc->SetSkillValue(2, 0);
			apc->SetSkillValue(3, 0);
		}

		head->Add(apc);

		LoadProfessionDescription();
	}
	else
		LOG("Could not find prof.txt in your UO directory. Character creation professions loading failed.\n");

	return result;
}
//----------------------------------------------------------------------------------
/*!
Загрузка описания профессий
@return 
*/
void CProfessionManager::LoadProfessionDescription()
{
	WISPFUN_DEBUG("c153_f5");
	WISP_FILE::CMappedFile file;

	if (file.Load(g_App.UOFilesPath("professn.enu")))
	{
		char *ptr = (char*)file.Start;
		char *end = (char*)((uint)file.Start + file.Size);

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

				((CBaseProfession*)m_Items)->AddDescription(-2, "parent", ptr);
				ptr += strlen(ptr) + 1;

				IFOR(i, 0, (int)list.size())
				{
					if (!((CBaseProfession*)m_Items)->AddDescription((int)i - 1, list[i], ptr))
					{
						//LOG("Failed to add description! (%s)\n", list[i].c_str());
					}
					ptr += strlen(ptr) + 1;
				}

				break;
			}

			ptr++;
		}

		list.clear();

		file.Unload();
	}
	else
	{
		LOG("Failed to load professn.enu\n");
		g_OrionWindow.ShowMessage("Failed to load professn.enu", "Failed to load");
	}
}
//----------------------------------------------------------------------------------
/*!
Получить родителя профессии
@param [__in] obj Ссылка на объект профессии
@param [__in_opt] check Ссылка на проверочный объект профессии
@return Ссылка на найденного родителя объекта профессии
*/
CBaseProfession *CProfessionManager::GetParent(CBaseProfession *obj, CBaseProfession *check)
{
	WISPFUN_DEBUG("c153_f6");
	if (check == NULL)
		check = (CBaseProfession*)m_Items;

	if (obj == m_Items)
		return obj;

	CBaseProfession *item = (CBaseProfession*)check->m_Items;
	CBaseProfession *result = NULL;

	while (item != NULL && result == NULL)
	{
		if (obj == item)
			result = check;
		else
			result = GetParent(obj, item);

		item = (CBaseProfession*)item->m_Next;
	}

	return result;
}
//----------------------------------------------------------------------------------
