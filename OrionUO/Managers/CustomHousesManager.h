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
public:
	ushort Graphic = 0;
	char X = 0;
	char Y = 0;
	char Z = 0;

	CBuildObject() {}
	CBuildObject(ushort graphic, char x, char y, char z)
		: Graphic(graphic), X(x), Y(y), Z(z) {}
	~CBuildObject() {}
};
//----------------------------------------------------------------------------------
class CCustomHouse
{
public:
	uint Serial = 0;
	uint Revision = 0;

	CCustomHouse() {}
	CCustomHouse(int serial, int revision)
		: Serial(serial), Revision(revision) {}
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

	CCustomHouse *Get(int serial);

	void Add(CCustomHouse* house);

	void Load(const string &path);
	void Save(const string &path);
};
//----------------------------------------------------------------------------------
extern CustomHousesManager g_CustomHousesManager;
//----------------------------------------------------------------------------------
#endif
//----------------------------------------------------------------------------------

