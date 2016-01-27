/****************************************************************************
**
** SkillGroupManager.h
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
#ifndef SkillGroupH
#define SkillGroupH
//---------------------------------------------------------------------------
class TSkillGroupObject
{
private:
	BYTE m_Items[60];
	int m_Count;
	bool m_Maximized;
	BYTE m_Selected;

	string m_Name;

public:
	TSkillGroupObject *m_Next;
	TSkillGroupObject *m_Prev;

	TSkillGroupObject();
	~TSkillGroupObject();

	SETGET(int, Count);
	SETGET(bool, Maximized);
	SETGET(BYTE, Selected);

	void SetName(string val);
	string GetName() const {return m_Name;}

	TTextTexture m_Texture;

	BYTE GetItem(int index);
	
	void Add(BYTE index);
	void AddSorted(BYTE index);
	void Remove(BYTE index);
	bool Contains(BYTE index);
	void Sort();
	void TransferTo(TSkillGroupObject *group);
};
//---------------------------------------------------------------------------
#endif