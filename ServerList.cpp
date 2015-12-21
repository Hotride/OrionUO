/****************************************************************************
**
** ServerList.cpp
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

TServerList ServerList;
//---------------------------------------------------------------------------
TServerList::TServerList()
:m_Count(0), m_SelectedServer(0), m_Index(NULL), m_Name(NULL), TextTexture(NULL)
{
}
//---------------------------------------------------------------------------
TServerList::~TServerList()
{
	Clear();
}
//---------------------------------------------------------------------------
void TServerList::Clear()
{
	if (m_Count > 0)
	{
		delete m_Index;
		delete []m_Name;
		delete []TextTexture;
	}

	m_Index = NULL;
	m_Name = NULL;
	TextTexture = NULL;

	m_Count = 0;
}
//---------------------------------------------------------------------------
void TServerList::Init(int count)
{
	if (m_Count)
	{
		delete m_Index;
		delete []m_Name;
		delete []TextTexture;
	}

	m_Count = count;

	if (m_Count < 1)
	{
		m_Count = 0;
		m_Index = NULL;
		m_Name = NULL;

		return;
	}

	m_Index = new WORD[m_Count];
	m_Name = new string[m_Count];
	TextTexture = new TTextTexture[m_Count];

	IFOR(i, 0, m_Count)
	{
		m_Index[i] = 0;
		m_Name[i] = "";
		TextTexture[i].Clear();
	}
}
//---------------------------------------------------------------------------
void TServerList::SetServer(int pos, WORD index, string name)
{
	if (pos < 0 || pos >= m_Count)
		return;

	m_Index[pos] = index;
	m_Name[pos] = name;
	TextTexture[pos].Clear();
	FontManager->GenerateA(5, TextTexture[pos], name.c_str(), 0x034F);
}
//---------------------------------------------------------------------------
string TServerList::GetServerName()
{
	string res = "";

	if (m_SelectedServer >= 0 && m_SelectedServer < m_Count)
		res = m_Name[m_SelectedServer];

	return res;
}
//---------------------------------------------------------------------------
WORD TServerList::GetIndex(int pos) const
{
	if (pos < 0 || pos >= m_Count)
		return 0;

	return m_Index[pos];
}
//---------------------------------------------------------------------------
string TServerList::GetName(int pos) const
{
	if (pos < 0 || pos >= m_Count)
		return string("");

	return m_Name[pos];
}
//--------------------------------------------------------------------------