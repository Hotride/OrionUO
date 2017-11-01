/***********************************************************************************
**
** ObjectPropertiesManager.cpp
**
** Copyright (C) October 2017 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#include "stdafx.h"
//----------------------------------------------------------------------------------
CObjectPropertiesManager g_ObjectPropertiesManager;
//----------------------------------------------------------------------------------
//----------------------------------CObjectProperty---------------------------------
//----------------------------------------------------------------------------------
CObjectProperty::CObjectProperty(const uint &serial, const uint &revision, const wstring &name, const wstring &data)
: m_Serial(serial), m_Revision(revision), m_Name(name), m_Data(data)
{
}
//----------------------------------------------------------------------------------
bool CObjectProperty::Empty()
{
	return (!m_Name.length() && !m_Data.length());
}
//----------------------------------------------------------------------------------
wstring CObjectProperty::CreateTextData()
{
	CGameObject *obj = g_World->FindWorldObject(m_Serial);
	bool coloredStartFont = false;
	wstring result = L"";

	if (obj != NULL)
	{
		if (!obj->NPC)
		{
			result = L"<basefont color=\"yellow\">";
			coloredStartFont = true;
		}
		else
		{
			CGameCharacter *gc = obj->GameCharacterPtr();
			coloredStartFont = true;

			switch (gc->Notoriety)
			{
				case NT_INNOCENT:
				{
					result = L"<basefont color=\"cyan\">";
					break;
				}
				case NT_SOMEONE_GRAY:
				case NT_CRIMINAL:
				{
					result = L"<basefont color=\"gray\">";
					break;
				}
				case NT_MURDERER:
				{
					result = L"<basefont color=\"red\">";
					break;
				}
				case NT_INVULNERABLE:
				{
					result = L"<basefont color=\"yellow\">";
					break;
				}
				default:
					break;
			}
		}
	}

	result += m_Name;

	if (coloredStartFont)
		result += L"<basefont color=\"#FFFFFFFF\">";

	if (m_Data.length())
		result += L"\n" + m_Data;

	return result;
}
//----------------------------------------------------------------------------------
//------------------------------CObjectPropertiesManager----------------------------
//----------------------------------------------------------------------------------
CObjectPropertiesManager::~CObjectPropertiesManager()
{
	m_Map.clear();
}
//----------------------------------------------------------------------------------
void CObjectPropertiesManager::Reset()
{
	m_Object = NULL;
	g_ToolTip.Reset();
}
//----------------------------------------------------------------------------------
bool CObjectPropertiesManager::RevisionCheck(const uint &serial, const uint &revision)
{
	OBJECT_PROPERTIES_MAP::iterator it = m_Map.find(serial);

	if (it == m_Map.end())
		return false;

	return (it->second.Revision == revision);
}
//----------------------------------------------------------------------------------
void CObjectPropertiesManager::OnItemClicked(const uint &serial)
{
	if (!g_TooltipsEnabled || g_ConfigManager.ItemPropertiesMode != OPM_SINGLE_CLICK)
		return;

	OBJECT_PROPERTIES_MAP::iterator it = m_Map.find(serial);

	if (it == m_Map.end() || it->second.Empty())
		return;

	g_GumpManager.CloseGump(0, 0, GT_PROPERTY);
	g_GumpManager.AddGump(new CGumpProperty(it->second.CreateTextData()));
}
//----------------------------------------------------------------------------------
void CObjectPropertiesManager::Display(const uint &serial)
{
	if (g_ConfigManager.ItemPropertiesMode == OPM_SINGLE_CLICK)
		return;

	OBJECT_PROPERTIES_MAP::iterator it = m_Map.find(serial);

	if (it == m_Map.end() || it->second.Empty())
		return;

	if (g_ConfigManager.ItemPropertiesMode == OPM_FOLLOW_MOUSE)
	{
		g_ToolTip.Set(it->second.CreateTextData());
		return;
	}
	
	/*bool condition = (g_ConfigManager.ItemPropertiesMode == OPM_ALWAYS_UP);

	if (!condition)
	{
		CRenderObject *object = g_SelectedObject.Object;

		if (object != NULL && object != m_Object)
		{
			m_Timer = g_Ticks + g_ConfigManager.ToolTipsDelay;
			m_Object = object;
		}

		condition = !(m_Timer > g_Ticks);
	}*/

	//Temp!!!
	g_ToolTip.Set(it->second.CreateTextData());

	/*if (condition)
	{
	}*/
}
//----------------------------------------------------------------------------------
void CObjectPropertiesManager::Add(const uint &serial, const CObjectProperty &objectProperty)
{
	m_Map[serial] = objectProperty;
}
//----------------------------------------------------------------------------------
