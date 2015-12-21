/****************************************************************************
**
** ServerList.h
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
#ifndef ServerListH
#define ServerListH
//--------------------------------------------------------------------------
class TServerList
{
private:
	int m_Count;
	int m_SelectedServer;

	PWORD m_Index;
	string *m_Name;
public:
	TServerList();
	~TServerList();

	SETGET(int, Count)
	SETGET(int, SelectedServer)

	void Init(int count);
	void Clear();

	TTextTexture *TextTexture;

	void SetServer(int pos, WORD index, string name);
	string GetServerName();

	WORD GetIndex(int pos) const;
	string GetName(int pos) const;
};

extern TServerList ServerList;
//---------------------------------------------------------------------------
#endif