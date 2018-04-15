// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
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
: Serial(serial), Revision(revision), Name(name), Data(data)
{
}
//----------------------------------------------------------------------------------
bool CObjectProperty::Empty()
{
	return (!Name.length() && !Data.length());
}
//----------------------------------------------------------------------------------
wstring CObjectProperty::CreateTextData(const bool &extended)
{
	CGameObject *obj = g_World->FindWorldObject(Serial);
	bool coloredStartFont = false;
	wstring result = L"";

	if (Name.length())
	{
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

		result += Name;

		if (coloredStartFont)
			result += L"<basefont color=\"#FFFFFFFF\">";
	}

	if (Data.length())
		result += L"\n" + Data;
	else if (extended)
	{
		if (result.length())
			result += L"\nNo significant properties...";
		else
			result = L"\nNo Data";
	}

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
	if (!g_ConfigManager.GetItemPropertiesIcon() || !g_TooltipsEnabled || g_ConfigManager.GetItemPropertiesMode() != OPM_SINGLE_CLICK)
		return;

	OBJECT_PROPERTIES_MAP::iterator it = m_Map.find(serial);

	if (it == m_Map.end() || it->second.Empty())
		return;

	g_GumpManager.CloseGump(0, 0, GT_PROPERTY);
	g_GumpManager.AddGump(new CGumpProperty(it->second.CreateTextData(true)));
}
//----------------------------------------------------------------------------------
void CObjectPropertiesManager::Display(const uint &serial)
{
	OBJECT_PROPERTIES_MAP::iterator it = m_Map.find(serial);

	if (it == m_Map.end() || it->second.Empty())
	{
		if (m_Object != NULL)
		{
			if (g_ConfigManager.GetItemPropertiesMode() == OPM_AT_ICON)
			{
				CGumpPropertyIcon *gump = (CGumpPropertyIcon*)g_GumpManager.UpdateContent(0, 0, GT_PROPERTY_ICON);

				if (gump != NULL)
					gump->Object = NULL;
			}

			Reset();
		}

		return;
	}

	if (!g_ConfigManager.GetItemPropertiesIcon() || g_ConfigManager.GetItemPropertiesMode() == OPM_FOLLOW_MOUSE)
	{
		g_ToolTip.Set(it->second.CreateTextData(false));
		return;
	}
	else if (g_ConfigManager.GetItemPropertiesMode() == OPM_SINGLE_CLICK)
		return;

	CGumpPropertyIcon *gump = (CGumpPropertyIcon*)g_GumpManager.GetGump(0, 0, GT_PROPERTY_ICON);

	if (gump == NULL)
		return;

	bool condition = (g_ConfigManager.GetItemPropertiesMode() == OPM_ALWAYS_UP);

	CRenderObject *object = g_SelectedObject.Object;

	if (object != m_Object)
	{
		m_Object = object;
		Timer = g_Ticks + g_ConfigManager.ToolTipsDelay;
	}

	if (!condition)
		condition = !(Timer > g_Ticks);

	if (condition && gump->Object != m_Object)
	{
		gump->SetText(it->second.CreateTextData(true));
		gump->Object = m_Object;
	}
}
//----------------------------------------------------------------------------------
void CObjectPropertiesManager::Add(const uint &serial, const CObjectProperty &objectProperty)
{
	m_Map[serial] = objectProperty;
}
//----------------------------------------------------------------------------------
