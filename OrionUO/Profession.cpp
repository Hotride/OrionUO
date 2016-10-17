/***********************************************************************************
**
** Profession.cpp
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#include "Profession.h"
#include "Managers/FontsManager.h"
#include "Managers/ClilocManager.h"
//----------------------------------------------------------------------------------
//---------------------------------CBaseProfession----------------------------------
//----------------------------------------------------------------------------------
CBaseProfession::CBaseProfession()
: CBaseQueueItem(), m_Name(""), m_TrueName(""), m_DescriptionIndex(0), m_TopLevel(false),
m_Gump(0), m_Type(PT_NO_PROF), m_NameClilocID(0), m_DescriptionClilocID(0),
m_Description(L"")
{
}
//----------------------------------------------------------------------------------
CBaseProfession::~CBaseProfession()
{
}
//----------------------------------------------------------------------------------
void CBaseProfession::OnChangeName(const string &name)
{
	m_Name = g_ClilocManager.Cliloc(g_Language)->GetA(m_NameClilocID, name);
}
//----------------------------------------------------------------------------------
bool CBaseProfession::AddDescription(int desc, string name, const char *val)
{
	bool result = m_DescriptionIndex == desc;

	if (result)
	{
		SetName(name);

		g_FontManager.SetUseHTML(true);

		if (desc == -2)
			m_Description = ToWString(val);
		else
			m_Description = ToWString(name + "\n" + val);

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
: CBaseProfession(), m_Childrens("|")
{
}
//----------------------------------------------------------------------------------
CProfessionCategory::~CProfessionCategory()
{
}
//----------------------------------------------------------------------------------
void CProfessionCategory::AddChildren(string child)
{
	m_Childrens += child + "|";
}
//----------------------------------------------------------------------------------
//-------------------------------------CProfession----------------------------------
//----------------------------------------------------------------------------------
CProfession::CProfession()
: CBaseProfession(), m_Str(0), m_Int(0), m_Dex(0)
{
	memset(&m_SkillIndex[0], 0, sizeof(m_SkillIndex));
	memset(&m_SkillValue[0], 0, sizeof(m_SkillValue));
}
//---------------------------------------------------------------------------
CProfession::~CProfession()
{
}
//---------------------------------------------------------------------------