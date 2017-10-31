/***********************************************************************************
**
** ObjectPripertiesManager.h
**
** Copyright (C) October 2017 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#ifndef OBJECTPROPERTIESMANAGER_H
#define OBJECTPROPERTIESMANAGER_H
//----------------------------------------------------------------------------------
class CObjectProperty
{
	SETGET(uint, Revision, 0);
	SETGET(wstring, Data, L"");

public:
	CObjectProperty() {}
	CObjectProperty(const uint &revision, const wstring &data);
};
//----------------------------------------------------------------------------------
typedef map<uint, CObjectProperty> OBJECT_PROPERTIES_MAP;
//----------------------------------------------------------------------------------
class CObjectPropertiesManager
{
private:
	OBJECT_PROPERTIES_MAP m_Map;

public:
	CObjectPropertiesManager() {}
	virtual ~CObjectPropertiesManager();

	bool RevisionCheck(const uint &serial, const uint &revision);

	wstring GetData(const uint &serial);

	void Add(const uint &serial, const CObjectProperty &objectProperty);
};
//----------------------------------------------------------------------------------
extern CObjectPropertiesManager g_ObjectPropertiesManager;
//----------------------------------------------------------------------------------
#endif //OBJECTPROPERTIESMANAGER_H
//----------------------------------------------------------------------------------
