/****************************************************************************
**
** Profession.cpp
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

//---------------------------------------------------------------------------
//------------------------------TBaseProfession------------------------------
//---------------------------------------------------------------------------
TBaseProfession::TBaseProfession()
: m_Name(""), m_TrueName(""), m_DescriptionIndex(0), m_TopLevel(false), m_Gump(0),
m_Type(PT_NO_PROF), m_NameClilocID(0), m_DescriptionClilocID(0)
{
}
//---------------------------------------------------------------------------
TBaseProfession::~TBaseProfession()
{
	m_TextureName.Clear();
	m_TextureDescription.Clear();
}
//---------------------------------------------------------------------------
void TBaseProfession::SetName(string val)
{
	m_Name = ClilocManager->Cliloc("ENU")->GetA(m_NameClilocID, val);
	
	FontManager->GenerateW(2, m_TextureName, ToWString(m_Name).c_str(), 0, 30, 185, TS_LEFT, UOFONT_SOLID);
}
//---------------------------------------------------------------------------
bool TBaseProfession::AddDescription(int desc, string name, const char *val)
{
	bool result = m_DescriptionIndex == desc;

	if (result)
	{
		SetName(name);

		FontManager->SetUseHTML(true);

		if (desc == -2)
			FontManager->GenerateW(1, m_TextureDescription, ToWString(val).c_str(), 0, 30, 195);
		else
			FontManager->GenerateW(1, m_TextureDescription, ToWString(name + "\n" + val).c_str(), 0, 30, 195);

		FontManager->SetUseHTML(false);
	}
	else
	{
		for (TBaseProfession *obj = (TBaseProfession*)m_Items; obj != NULL && !result; obj = (TBaseProfession*)obj->m_Next)
			result = obj->AddDescription(desc, name, val);
	}

	return result;
}
//---------------------------------------------------------------------------
//-----------------------------TProfessionCategory---------------------------
//---------------------------------------------------------------------------
TProfessionCategory::TProfessionCategory()
: TBaseProfession(), m_Childrens("|")
{
}
//---------------------------------------------------------------------------
TProfessionCategory::~TProfessionCategory()
{
}
//---------------------------------------------------------------------------
void TProfessionCategory::AddChildren(string child)
{
	m_Childrens += child + "|";
}
//---------------------------------------------------------------------------
//---------------------------------TProfession-------------------------------
//---------------------------------------------------------------------------
TProfession::TProfession()
: TBaseProfession(), m_Str(0), m_Int(0), m_Dex(0)
{
	memset(&m_SkillIndex[0], 0, sizeof(m_SkillIndex));
	memset(&m_SkillValue[0], 0, sizeof(m_SkillValue));
}
//---------------------------------------------------------------------------
TProfession::~TProfession()
{
}
//---------------------------------------------------------------------------