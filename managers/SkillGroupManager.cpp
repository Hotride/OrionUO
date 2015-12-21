/****************************************************************************
**
** SkillGroupManager.cpp
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
//----------------------------------------------------------------------------
#include "stdafx.h"

TSkillGroupManager SkillGroupManager;
//----------------------------------------------------------------------------
TSkillGroupManager::TSkillGroupManager()
: m_Count(0)
{
}
//----------------------------------------------------------------------------
TSkillGroupManager::~TSkillGroupManager()
{
}
//----------------------------------------------------------------------------
void TSkillGroupManager::MakeDefault()
{
	Clear();

	MakeDefaultMiscellaneous();
	MakeDefaultCombat();
	MakeDefaultTradeSkills();
	MakeDefaultMagic();
	MakeDefaultWilderness();
	MakeDefaultThieving();
	MakeDefaultBard();

	TSkillGroupObject *group = m_Groups;

	while (group != NULL)
	{
		group->Sort();

		group = group->m_Next;
	}
}
//----------------------------------------------------------------------------
void TSkillGroupManager::MakeDefaultMiscellaneous()
{
	TSkillGroupObject *group = new TSkillGroupObject();
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
//----------------------------------------------------------------------------
void TSkillGroupManager::MakeDefaultCombat()
{
	int cnt = g_SkillsCount;

	TSkillGroupObject *group = new TSkillGroupObject();
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
//----------------------------------------------------------------------------
void TSkillGroupManager::MakeDefaultTradeSkills()
{
	TSkillGroupObject *group = new TSkillGroupObject();
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
//----------------------------------------------------------------------------
void TSkillGroupManager::MakeDefaultMagic()
{
	int cnt = g_SkillsCount;

	TSkillGroupObject *group = new TSkillGroupObject();
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
//----------------------------------------------------------------------------
void TSkillGroupManager::MakeDefaultWilderness()
{
	TSkillGroupObject *group = new TSkillGroupObject();
	group->SetName("Wilderness");
	group->Add(2);
	group->Add(35);
	group->Add(18);
	group->Add(20);
	group->Add(38);
	group->Add(39);
	
	Add(group);
}
//----------------------------------------------------------------------------
void TSkillGroupManager::MakeDefaultThieving()
{
	TSkillGroupObject *group = new TSkillGroupObject();
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
//----------------------------------------------------------------------------
void TSkillGroupManager::MakeDefaultBard()
{
	TSkillGroupObject *group = new TSkillGroupObject();
	group->SetName("Bard");
	group->Add(15);
	group->Add(29);
	group->Add(9);
	group->Add(22);
	
	Add(group);
}
//----------------------------------------------------------------------------
void TSkillGroupManager::Clear()
{
	TSkillGroupObject *item = m_Groups;

	while (item != NULL)
	{
		TSkillGroupObject *next = item->m_Next;

		delete item;

		item = next;
	}

	m_Count = 0;
	m_Groups = NULL;
}
//----------------------------------------------------------------------------
void TSkillGroupManager::Add(TSkillGroupObject *group)
{
	if (m_Groups == NULL)
	{
		m_Groups = group;
		m_Groups->m_Next = NULL;
		m_Groups->m_Prev = NULL;
		m_Count = 1;

		return;
	}

	TSkillGroupObject *item = m_Groups;

	while (item->m_Next != NULL) item = item->m_Next;

	item->m_Next = group;
	group->m_Next = NULL;
	group->m_Prev = item;

	m_Count++;
}
//----------------------------------------------------------------------------
void TSkillGroupManager::Remove(TSkillGroupObject *group)
{
	if (group->m_Prev == NULL) //Miscellaneous
	{
		int x = g_GameWindowPosX + (g_GameWindowSizeX / 2) - 100;
		int y = g_GameWindowPosY + (g_GameWindowSizeY / 2) - 62;

		TGumpNotify *gump = new TGumpNotify(0, x, y, 2, 200, 125, "Cannot delete this group.");

		GumpManager->AddGump(gump);
		
		UO->InitScreen(GS_GAME_BLOCKED);
		GameBlockedScreen->Code = 4;
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
}
//----------------------------------------------------------------------------
int TSkillGroupManager::GetVisibleLinesCount()
{
	int count = 0;

	TSkillGroupObject *group = m_Groups;

	while (group != NULL)
	{
		count++;
		if (group->GetMaximized())
			count += group->GetCount();

		group = group->m_Next;
	}

	if (count)
		count--;

	return count;
}
//----------------------------------------------------------------------------
void TSkillGroupManager::Load(string path)
{
	Clear();
	
	TMappedHeader file;
	memset(&file, 0, sizeof(TMappedHeader));

	FileManager.LoadFileToMemory(file, path.c_str());

	if (file.Size)
	{
		BYTE version = file.ReadByte();

		short count = file.ReadShort();

		IFOR(i, 0, count)
		{
			PBYTE next = file.Ptr;
			short size = file.ReadShort();
			next += size;

			TSkillGroupObject *group = new TSkillGroupObject();

			short length = file.ReadShort();
			string str = file.ReadString(length);
			group->SetName(str);

			short skills = file.ReadShort();

			IFOR(j, 0, skills)
			{
				BYTE skill = file.ReadByte();

				if (skill != 0xFF)
					group->Add(skill);
			}

			group->Sort();

			Add(group);

			file.Ptr = next;
		}

		FileManager.UnloadFileFromMemory(file);
	}
	else
		MakeDefault();
}
//----------------------------------------------------------------------------
void TSkillGroupManager::Save(string path)
{
	TFileWriter *writer = new TFileWriter(path, true);
	
	writer->WriteByte(0); //version
	
	m_Count = 0;
	TSkillGroupObject *group = m_Groups;
	while (group != NULL)
	{
		m_Count++;
		group = group->m_Next;
	}
	
	writer->WriteShort(m_Count); //Count

	group = m_Groups;

	IFOR(i, 0, m_Count)
	{
		string str = group->GetName();
		int len = str.length() + 1;

		short size = len + 2 + 2 + 2 + group->GetCount();
		writer->WriteShort(size); //Block size
		
		writer->WriteShort(len); //Name length
		writer->WriteAsciiString(str); //Name
		
		short count = group->GetCount();

		writer->WriteShort(count); //Skills count

		IFOR(j, 0, count)
		{
			BYTE skill = group->GetItem(j);
			writer->WriteByte(skill); //Skill
		}
		
		writer->WriteBuffer();

		group = group->m_Next;
	}

	delete writer;
}
//----------------------------------------------------------------------------