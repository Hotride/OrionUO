// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
/***********************************************************************************
**
** Profession.cpp
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#include "stdafx.h"
//----------------------------------------------------------------------------------
//---------------------------------CBaseProfession----------------------------------
//----------------------------------------------------------------------------------
CBaseProfession::CBaseProfession()
: CBaseQueueItem()
{
}
//----------------------------------------------------------------------------------
CBaseProfession::~CBaseProfession()
{
}
//----------------------------------------------------------------------------------
bool CBaseProfession::AddDescription(int desc, string name, const char *val)
{
	WISPFUN_DEBUG("c199_f1");
	bool result = (DescriptionIndex == desc);

	if (result)
	{
		if (!NameClilocID)
			Name = name;

		g_FontManager.SetUseHTML(true);

		if (desc == -2)
			Description = ToWString(val);
		else
			Description = ToWString(name + "\n" + val);

		g_FontManager.SetUseHTML(false);
	}
	else
	{
		for (CBaseProfession *obj = (CBaseProfession*)m_Items; obj != NULL && !result; obj = (CBaseProfession*)obj->m_Next)
			result = obj->AddDescription(desc, name, val);
	}

	return result;
}
//----------------------------------------------------------------------------------
//---------------------------------CProfessionCategory------------------------------
//----------------------------------------------------------------------------------
CProfessionCategory::CProfessionCategory()
: CBaseProfession()
{
}
//----------------------------------------------------------------------------------
CProfessionCategory::~CProfessionCategory()
{
}
//----------------------------------------------------------------------------------
void CProfessionCategory::AddChildren(string child)
{
	Childrens += child + "|";
}
//----------------------------------------------------------------------------------
//-------------------------------------CProfession----------------------------------
//----------------------------------------------------------------------------------
CProfession::CProfession()
: CBaseProfession()
{
	memset(&m_SkillIndex[0], 0, sizeof(m_SkillIndex));
	memset(&m_SkillValue[0], 0, sizeof(m_SkillValue));
}
//---------------------------------------------------------------------------
CProfession::~CProfession()
{
}
//---------------------------------------------------------------------------