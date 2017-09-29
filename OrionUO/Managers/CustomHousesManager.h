/***********************************************************************************
**
** CustomHousesManager.h
**
** Copyright (C) September 2017 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#ifndef CUSTOMHOUSESMANAGER_H
#define CUSTOMHOUSESMANAGER_H
//----------------------------------------------------------------------------------
class CBuildObject
{
	SETGET(ushort, Graphic, 0);
	SETGET(char, X, 0);
	SETGET(char, Y, 0);
	SETGET(char, Z, 0);

public:
	CBuildObject() {}
	CBuildObject(const ushort &graphic, const char &x, const char &y, const char &z)
		: m_Graphic(graphic), m_X(x), m_Y(y), m_Z(z) {}
	~CBuildObject() {}
};
//----------------------------------------------------------------------------------
class CCustomHouse
{
	SETGET(uint, Serial, 0);
	SETGET(uint, Revision, 0);

public:
	CCustomHouse() {}
	CCustomHouse(const uint &serial, const uint &revision)
		: m_Serial(serial), m_Revision(revision) {}
	~CCustomHouse() {}

	vector<CBuildObject> m_Items;

	void Paste(CGameItem *foundation);
};
//----------------------------------------------------------------------------------
class CustomHousesManager : public WISP_DATASTREAM::CDataReader
{
	unordered_map<uint, CCustomHouse*> m_Items;

public:
	CustomHousesManager() {}
	~CustomHousesManager();

	void Clear();

	CCustomHouse *Get(const uint &serial);

	void Add(CCustomHouse* house);

	void Load(const string &path);
	void Save(const string &path);
};
//----------------------------------------------------------------------------------
extern CustomHousesManager g_CustomHousesManager;
//----------------------------------------------------------------------------------
#endif
//----------------------------------------------------------------------------------

