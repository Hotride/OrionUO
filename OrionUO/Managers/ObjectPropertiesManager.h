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
	SETGET(uint, Serial, 0);
	SETGET(uint, Revision, 0);
	SETGET(wstring, Name, L"");
	SETGET(wstring, Data, L"");

public:
	CObjectProperty() {}
	CObjectProperty(const uint &serial, const uint &revision, const wstring &name, const wstring &data);

	bool Empty();

	wstring CreateTextData(const bool &extended);
};
//----------------------------------------------------------------------------------
typedef map<uint, CObjectProperty> OBJECT_PROPERTIES_MAP;
//----------------------------------------------------------------------------------
class CObjectPropertiesManager
{
	SETGET(uint, Timer, 0);

private:
	OBJECT_PROPERTIES_MAP m_Map;

	class CRenderObject *m_Object{ NULL };
public:
	CObjectPropertiesManager() {}
	virtual ~CObjectPropertiesManager();

	void Reset();

	bool RevisionCheck(const uint &serial, const uint &revision);

	void OnItemClicked(const uint &serial);

	void Display(const uint &serial);

	void Add(const uint &serial, const CObjectProperty &objectProperty);
};
//----------------------------------------------------------------------------------
extern CObjectPropertiesManager g_ObjectPropertiesManager;
//----------------------------------------------------------------------------------
#endif //OBJECTPROPERTIESMANAGER_H
//----------------------------------------------------------------------------------
