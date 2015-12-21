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

TProfessionCategory *Profession = NULL;
TProfessionCategory *UsedProfessionCategory = NULL;
TProfession *UsedProfession = NULL;
//---------------------------------------------------------------------------
TBaseProfession::TBaseProfession()
: m_Parent(NULL), m_DefName(""), m_Name(""), m_Type(false), m_Gump(0), m_Index(0)
{
	m_TextureName.Clear();
	m_TextureDescription.Clear();
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
	m_Name = val;

	FontManager->GenerateW(1, m_TextureName, ToWString(val).c_str(), 0, 30, 185);
}
//---------------------------------------------------------------------------
bool TBaseProfession::AddDescription(int desc, string name, const char *val)
{
	if (m_Index == desc)
	{
		SetName(name);

		FontManager->SetUseHTML(true);

		if (desc == -2)
			FontManager->GenerateW(1, m_TextureDescription, ToWString(val).c_str(), 0, 30, 195);
		else
			FontManager->GenerateW(1, m_TextureDescription, ToWString(name + "\n" + val).c_str(), 0, 30, 195);

		FontManager->SetUseHTML(false);

		return true;
	}

	return false;
}
//---------------------------------------------------------------------------
//-------------------------------TProfession--------------------------------
//---------------------------------------------------------------------------
TProfession::TProfession()
: TBaseProfession(), m_Str(0), m_Dex(0), m_Int(0)
{
	memset(m_SkillID, 0, sizeof(m_SkillID));
	memset(m_SkillValue, 0, sizeof(m_SkillValue));
}
//---------------------------------------------------------------------------
TProfession::~TProfession()
{
	m_TextureName.Clear();
	m_TextureDescription.Clear();
}
//---------------------------------------------------------------------------
//-------------------------TProfessionCategory------------------------------
//---------------------------------------------------------------------------
TProfessionCategory::TProfessionCategory()
: TBaseProfession(), m_CatChildCount(0), m_ProfChildCount(0)
{
	IFOR(i, 0, 4)
	{
		m_CatChild[i] = NULL;
		m_ProfChild[i] = NULL;
		Childs[i] = "";
	}
	
	m_TextureName.Clear();
	m_TextureDescription.Clear();
}
//---------------------------------------------------------------------------
TProfessionCategory::~TProfessionCategory()
{
	IFOR(i, 0, m_CatChildCount)
	{
		delete m_CatChild[i];
		m_CatChild[i] = NULL;
	}

	IFOR(i, 0, m_ProfChildCount)
	{
		delete m_ProfChild[i];
		m_ProfChild[i] = NULL;
	}
}
//---------------------------------------------------------------------------
bool TProfessionCategory::AddCategory(TProfessionCategory *pc, bool topObj)
{
	if (topObj && m_CatChildCount <= 3)
	{
		pc->m_Parent = this;
		m_CatChild[m_CatChildCount] = pc;
		m_CatChildCount++;

		return true;
	}

	if (m_CatChildCount < 4)
	{
		IFOR(i, 0, 4)
		{
			if (Childs[i] == pc->GetDefName())
			{
				pc->m_Parent = this;
				m_CatChild[m_CatChildCount] = pc;
				m_CatChildCount++;

				return true;
			}
		}
	}

	if (m_CatChildCount > 0)
	{
		IFOR(i, 0, m_CatChildCount)
		{
			pc->m_Parent = this;

			if (m_CatChild[i]->AddCategory(pc))
				return true;
		}
	}

	return false;
}
//---------------------------------------------------------------------------
bool TProfessionCategory::Add(TProfession *p)
{
	if (m_CatChildCount > 0)
	{
		IFOR(i, 0, m_CatChildCount)
		{
			if (m_CatChild[i]->Add(p))
				return true;
		}
	}

	if (m_ProfChildCount < 4)
	{
		IFOR(i, 0, 4)
		{
			if (Childs[i] == p->GetDefName())
			{
				p->m_Parent = this;
				m_ProfChild[m_ProfChildCount] = p;
				m_ProfChildCount++;

				return true;
			}
		}
	}

	return false;
}
//---------------------------------------------------------------------------
void TProfessionCategory::AddChildren(string children)
{
	IFOR(i, 0, 4)
	{
		if (!Childs[i].length())
		{
			Childs[i] = children;

			break;
		}
	}
}
//---------------------------------------------------------------------------
bool TProfessionCategory::AddDescription(int desc, string name, const char *val)
{
	if (Index == desc)
	{
		SetName(name);

		FontManager->SetUseHTML(true);

		if (desc == -2)
			FontManager->GenerateW(1, m_TextureDescription, ToWString(val).c_str(), 0, 30, 195);
		else
			FontManager->GenerateW(1, m_TextureDescription, ToWString(name + "\n" + val).c_str(), 0, 30, 195);
		FontManager->SetUseHTML(false);

		return true;
	}

	if (m_CatChildCount > 0)
	{
		IFOR(i, 0, m_CatChildCount)
		{
			if (m_CatChild[i]->AddDescription(desc, name, val))
				return true;
		}
	}

	if (m_ProfChildCount > 0 )
	{
		IFOR(i, 0, m_ProfChildCount)
		{
			if (m_ProfChild[i]->AddDescription(desc, name, val))
				return true;
		}
	}

	return false;
}
//---------------------------------------------------------------------------