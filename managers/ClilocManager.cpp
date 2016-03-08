/****************************************************************************
**
** ClilocManager.cpp
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
//---------------------------------------------------------------------------
#include "stdafx.h"

TClilocManager *ClilocManager = NULL;
//---------------------------------------------------------------------------
//------------------------------------TCliloc--------------------------------
//---------------------------------------------------------------------------
TCliloc::TCliloc(string lang)
: TBaseQueueItem()
{
	m_Loaded = false;

	memset(&m_File, 0, sizeof(m_File));
	m_Language = lang;

	if (m_Language.length())
	{
		string path = FilePath(string("cliloc.") + lang);

		if (FileManager.LoadFileToMemory(m_File, path.c_str()))
			m_Loaded = true;
	}
}
//---------------------------------------------------------------------------
TCliloc::~TCliloc()
{
	FileManager.UnloadFileFromMemory(m_File);

	m_ClilocSystem.clear();
	m_ClilocRegular.clear();
	m_ClilocSupport.clear();
}
//---------------------------------------------------------------------------
string TCliloc::Load(DWORD &id)
{
	string result = "";

	if (m_Loaded)
	{
		m_File.Ptr = (PBYTE)m_File.Address;
		PBYTE eof = m_File.Ptr + m_File.Size;

		while (m_File.Ptr < eof)
		{
			DWORD currentID = m_File.ReadDWord();

			m_File.Move(1);

			short len = m_File.ReadShort();

			if (currentID == id)
			{
				result = m_File.ReadString(len);

				if (id >= 3000000)
					m_ClilocSupport[currentID] = result;
				else if (id >= 1000000)
					m_ClilocRegular[currentID] = result;
				else
					m_ClilocSystem[currentID] = result;

				break;
			}
			else
				m_File.Move(len);
		}
	}

	id = 0;

	return result;
}
//---------------------------------------------------------------------------
string TCliloc::GetA(DWORD id, string result)
{
	if (id >= 3000000)
	{
		CLILOC_MAP::iterator i = m_ClilocSupport.find(id);
		if (i != m_ClilocSupport.end() && (*i).second.length())
			return (*i).second;
	}
	else if (id >= 1000000)
	{
		CLILOC_MAP::iterator i = m_ClilocRegular.find(id);
		if (i != m_ClilocRegular.end() && (*i).second.length())
			return (*i).second;
	}
	else
	{
		CLILOC_MAP::iterator i = m_ClilocSystem.find(id);
		if (i != m_ClilocSystem.end() && (*i).second.length())
			return (*i).second;
	}

	DWORD tmpID = id;
	string loadStr = Load(tmpID);
	if (!tmpID && loadStr.length())
		return loadStr;
	else if (!result.length())
	{
		char str[50] = {0};
		sprintf(str, "Unknown Cliloc #%i", id);

		result = str;
	}

	return result;
}
//---------------------------------------------------------------------------
wstring TCliloc::GetW(DWORD id, string result)
{
	return ToWString(GetA(id, result));
}
//---------------------------------------------------------------------------
//--------------------------------TClilocManager-----------------------------
//---------------------------------------------------------------------------
TClilocManager::TClilocManager()
: TBaseQueue(), m_LastCliloc(NULL)
{
}
//---------------------------------------------------------------------------
TClilocManager::~TClilocManager()
{
}
//---------------------------------------------------------------------------
TCliloc *TClilocManager::Cliloc(string lang)
{
	string language = ToLowerA(lang);

	if (m_LastCliloc != NULL && m_LastCliloc->GetLanguage() == language)
		return m_LastCliloc;

	TCliloc *obj = (TCliloc*)m_Items;

	while (obj != NULL)
	{
		if (obj->GetLanguage() == language)
		{
			m_LastCliloc = obj;
			return obj;
		}

		obj = (TCliloc*)obj->m_Next;
	}

	obj = new TCliloc(language);

	if (!obj->Loaded)
	{
		const string enu("enu");
		delete obj;

		obj = (TCliloc*)m_Items;

		while (obj != NULL)
		{
			if (obj->GetLanguage() == enu)
			{
				m_LastCliloc = obj;
				return obj;
			}

			obj = (TCliloc*)obj->m_Next;
		}

		obj = new TCliloc(enu);
	}

	Add(obj);

	m_LastCliloc = obj;
	return obj;
}
//---------------------------------------------------------------------------