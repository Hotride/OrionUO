// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
/***********************************************************************************
**
** SkillGroupManager.cpp
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#include "stdafx.h"
//----------------------------------------------------------------------------------
CSkillGroupManager g_SkillGroupManager;
//----------------------------------------------------------------------------------
CSkillGroupManager::CSkillGroupManager()
{
}
//----------------------------------------------------------------------------------
CSkillGroupManager::~CSkillGroupManager()
{
}
//----------------------------------------------------------------------------------
/*!
Выставить группы по-умолчанию
@return 
*/
void CSkillGroupManager::MakeDefault()
{
	WISPFUN_DEBUG("c155_f1");
	Clear();

	MakeDefaultMiscellaneous();
	MakeDefaultCombat();
	MakeDefaultTradeSkills();
	MakeDefaultMagic();
	MakeDefaultWilderness();
	MakeDefaultThieving();
	MakeDefaultBard();

	CSkillGroupObject *group = m_Groups;

	while (group != NULL)
	{
		group->Sort();

		group = group->m_Next;
	}
}
//----------------------------------------------------------------------------------
void CSkillGroupManager::MakeDefaultMiscellaneous()
{
	WISPFUN_DEBUG("c155_f2");
	CSkillGroupObject *group = new CSkillGroupObject();
	group->SetName("Miscellaneous");
	group->Add(4);
	group->Add(6);
	group->Add(10);
	group->Add(12);
	group->Add(19);
	group->Add(3);
	group->Add(36);

	Add(group);
}
//----------------------------------------------------------------------------------
void CSkillGroupManager::MakeDefaultCombat()
{
	WISPFUN_DEBUG("c155_f3");
	int cnt = g_SkillsManager.Count;

	CSkillGroupObject *group = new CSkillGroupObject();
	group->SetName("Combat");
	group->Add(1);
	group->Add(31);
	group->Add(42);
	group->Add(17);
	group->Add(41);
	group->Add(5);
	group->Add(40);
	group->Add(27);

	if (cnt > 57) group->Add(57); //Throving
	group->Add(43);
	
	if (cnt > 50) group->Add(50); //Focus
	if (cnt > 51) group->Add(51); //Chivalry
	if (cnt > 52) group->Add(52); //Bushido
	if (cnt > 53) group->Add(53); //Ninjitsu

	Add(group);
}
//----------------------------------------------------------------------------------
void CSkillGroupManager::MakeDefaultTradeSkills()
{
	WISPFUN_DEBUG("c155_f4");
	CSkillGroupObject *group = new CSkillGroupObject();
	group->SetName("Trade Skills");
	group->Add(0);
	group->Add(7);
	group->Add(8);
	group->Add(11);
	group->Add(13);
	group->Add(23);
	group->Add(44);
	group->Add(45);
	group->Add(34);
	group->Add(37);

	Add(group);
}
//----------------------------------------------------------------------------------
void CSkillGroupManager::MakeDefaultMagic()
{
	WISPFUN_DEBUG("c155_f5");
	int cnt = g_SkillsManager.Count;

	CSkillGroupObject *group = new CSkillGroupObject();
	group->SetName("Magic");
	group->Add(16);
	if (cnt > 56) group->Add(56); //Imbuing
	group->Add(25);
	group->Add(46);
	if (cnt > 55) group->Add(55); //Mysticism
	group->Add(26);
	if (cnt > 54) group->Add(54); //Spellweaving
	group->Add(32);
	if (cnt > 49) group->Add(49); //Necromancy
	
	Add(group);
}
//----------------------------------------------------------------------------------
void CSkillGroupManager::MakeDefaultWilderness()
{
	WISPFUN_DEBUG("c155_f6");
	CSkillGroupObject *group = new CSkillGroupObject();
	group->SetName("Wilderness");
	group->Add(2);
	group->Add(35);
	group->Add(18);
	group->Add(20);
	group->Add(38);
	group->Add(39);
	
	Add(group);
}
//----------------------------------------------------------------------------------
void CSkillGroupManager::MakeDefaultThieving()
{
	WISPFUN_DEBUG("c155_f7");
	CSkillGroupObject *group = new CSkillGroupObject();
	group->SetName("Thieving");
	group->Add(14);
	group->Add(21);
	group->Add(24);
	group->Add(30);
	group->Add(48);
	group->Add(28);
	group->Add(33);
	group->Add(47);
	
	Add(group);
}
//----------------------------------------------------------------------------------
void CSkillGroupManager::MakeDefaultBard()
{
	WISPFUN_DEBUG("c155_f8");
	CSkillGroupObject *group = new CSkillGroupObject();
	group->SetName("Bard");
	group->Add(15);
	group->Add(29);
	group->Add(9);
	group->Add(22);
	
	Add(group);
}
//----------------------------------------------------------------------------------
/*!
Очистить список групп
@return 
*/
void CSkillGroupManager::Clear()
{
	WISPFUN_DEBUG("c155_f9");
	CSkillGroupObject *item = m_Groups;

	while (item != NULL)
	{
		CSkillGroupObject *next = item->m_Next;

		delete item;

		item = next;
	}

	m_Count = 0;
	m_Groups = NULL;
}
//----------------------------------------------------------------------------------
/*!
Добавить группу
@param [__in] group Ссылка на группу
@return
*/
void CSkillGroupManager::Add(CSkillGroupObject *group)
{
	WISPFUN_DEBUG("c155_f10");
	if (m_Groups == NULL)
	{
		m_Groups = group;
		m_Groups->m_Next = NULL;
		m_Groups->m_Prev = NULL;
		m_Count = 1;

		return;
	}

	CSkillGroupObject *item = m_Groups;

	while (item->m_Next != NULL)
		item = item->m_Next;

	item->m_Next = group;
	group->m_Next = NULL;
	group->m_Prev = item;

	m_Count++;
}
//----------------------------------------------------------------------------------
/*!
Удалить группу
@param [__in] group Ссылка на группу
@return 
*/
bool CSkillGroupManager::Remove(CSkillGroupObject *group)
{
	WISPFUN_DEBUG("c155_f11");
	if (group->m_Prev == NULL) //Miscellaneous
	{
		int x = g_ConfigManager.GameWindowX + (g_ConfigManager.GameWindowWidth / 2) - 100;
		int y = g_ConfigManager.GameWindowY + (g_ConfigManager.GameWindowHeight / 2) - 62;

		CGumpNotify *gump = new CGumpNotify(x, y, CGumpNotify::ID_GN_STATE_NOTIFICATION, 200, 125, "Cannot delete this group.");

		g_GumpManager.AddGump(gump);
		
		g_Orion.InitScreen(GS_GAME_BLOCKED);
		g_GameBlockedScreen.Code = 4;

		return false;
	}
	else
	{
		m_Count--;

		if (m_Count < 0)
			m_Count = 0;

		if (group->m_Next != NULL)
			group->m_Next->m_Prev = group->m_Prev;

		group->m_Prev->m_Next = group->m_Next;

		group->TransferTo(m_Groups);
		delete group;
	}

	return true;
}
//----------------------------------------------------------------------------------
/*!
Загрузка групп из файла конфига
@param [__in] path Путь к файлу конфига
@return 
*/
bool CSkillGroupManager::Load(string path)
{
	WISPFUN_DEBUG("c155_f12");
	bool result = false;

	Clear();
	
	WISP_FILE::CMappedFile file;

	if (file.Load(path))
	{
		BYTE version = file.ReadUInt8();

		short count = file.ReadUInt16LE();

		IFOR(i, 0, count)
		{
			PBYTE next = file.Ptr;
			short size = file.ReadUInt16LE();
			next += size;

			CSkillGroupObject *group = new CSkillGroupObject();

			short length = file.ReadUInt16LE();
			string str = file.ReadString(length);
			group->SetName(str);

			short skills = file.ReadUInt16LE();

			IFOR(j, 0, skills)
			{
				BYTE skill = file.ReadUInt8();

				if (skill != 0xFF)
					group->Add(skill);
			}

			group->Sort();

			Add(group);

			file.Ptr = next;
		}

		file.Unload();

		result = true;
	}
	else
		MakeDefault();

	return result;
}
//----------------------------------------------------------------------------------
/*!
Сохранение групп в файл конфиг
@param [__in] path Путьк  файлу конфига
@return 
*/
void CSkillGroupManager::Save(string path)
{
	WISPFUN_DEBUG("c155_f13");
	WISP_FILE::CBinaryFileWritter writter;

	writter.Open(path);
	
	writter.WriteUInt8(0); //version
	
	m_Count = 0;
	CSkillGroupObject *group = m_Groups;
	while (group != NULL)
	{
		m_Count++;
		group = group->m_Next;
	}
	
	writter.WriteUInt16LE(m_Count); //Count

	group = m_Groups;

	IFOR(i, 0, m_Count)
	{
		string str = group->GetName();
		size_t len = str.length() + 1;

		short size = (short)len + 2 + 2 + 2 + group->GetCount();
		writter.WriteUInt16LE(size); //Block size
		
		writter.WriteUInt16LE((short)len); //Name length
		writter.WriteString(str, false); //Name
		
		short count = group->GetCount();

		writter.WriteUInt16LE(count); //Skills count

		IFOR(j, 0, count)
		{
			BYTE skill = group->GetItem(j);
			writter.WriteUInt8(skill); //Skill
		}
		
		writter.WriteBuffer();

		group = group->m_Next;
	}

	writter.Close();
}
//----------------------------------------------------------------------------------
