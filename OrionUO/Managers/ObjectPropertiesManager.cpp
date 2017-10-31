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
CObjectProperty::CObjectProperty(const uint &revision, const wstring &data)
: m_Revision(revision), m_Data(data)
{
}
//----------------------------------------------------------------------------------
//------------------------------CObjectPropertiesManager----------------------------
//----------------------------------------------------------------------------------
CObjectPropertiesManager::~CObjectPropertiesManager()
{
	m_Map.clear();
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
wstring CObjectPropertiesManager::GetData(const uint &serial)
{
	OBJECT_PROPERTIES_MAP::iterator it = m_Map.find(serial);

	if (it == m_Map.end())
		return L"";

	return it->second.Data;
}
//----------------------------------------------------------------------------------
void CObjectPropertiesManager::Add(const uint &serial, const CObjectProperty &objectProperty)
{
	m_Map[serial] = objectProperty;
}
//----------------------------------------------------------------------------------
