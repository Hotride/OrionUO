/***********************************************************************************
**
** GumpCustomHouse.cpp
**
** Copyright (C) September 2017 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#include "stdafx.h"
//----------------------------------------------------------------------------------
template<class T>
void ParseCustomHouseObjectFile(vector<T> &list, const string &path)
{
	LOG("parse CH file: %s\n", path.c_str());

	FILE *file = NULL;
	fopen_s(&file, path.c_str(), "r");

	if (file != NULL)
	{
		int line = 0;

		while (!feof(file))
		{
			char buf[256] = { 0 };
			fgets(&buf[0], 256, file);

			if (!strlen(buf))
				continue;

			line++;

			if (line <= 2)
				continue;

			T item;

			if (item.Parse(buf))
				list.push_back(item);
		}

		fclose(file);
	}
}
//----------------------------------------------------------------------------------
CGumpCustomHouse::CGumpCustomHouse(const uint &serial, const int &x, const int &y)
: CGump(GT_CUSTOM_HOUSE, serial, x, y)
{
	WISPFUN_DEBUG("");

	Add(new CGUIGumppic(0x55F0, 0, 17));

	Add(new CGUIGumppic(0x55F9, 486, 17));
	// or, if 4 floors : Add(new CGUIGumppic(0x55F2, 486, 17));

	Add(new CGUIGumppicTiled(0x55F1, 153, 17, 333, 154));

	ParseCustomHouseObjectFile<CCustomHouseObjectWall>(m_Walls, g_App.FilePath("walls.txt"));
	ParseCustomHouseObjectFile<CCustomHouseObjectFloor>(m_Floors, g_App.FilePath("floors.txt"));
	ParseCustomHouseObjectFile<CCustomHouseObjectDoor>(m_Doors, g_App.FilePath("doors.txt"));
	ParseCustomHouseObjectFile<CCustomHouseObjectMisc>(m_Miscs, g_App.FilePath("misc.txt"));
	ParseCustomHouseObjectFile<CCustomHouseObjectStair>(m_Stairs, g_App.FilePath("stairs.txt"));
	ParseCustomHouseObjectFile<CCustomHouseObjectTeleport>(m_Teleports, g_App.FilePath("teleprts.txt"));
	ParseCustomHouseObjectFile<CCustomHouseObjectRoof>(m_Roofs, g_App.FilePath("roof.txt"));

	//0x56F0
	//0x56EA
	//0x56EC
	//0x
	//0x
	//0x
	//0x
	//0x
	//0x
	//0x
	//0x
	//0x
	//0x
	//0x
	//0x
	//0x
	//0x
	//0x
}
//----------------------------------------------------------------------------------
CGumpCustomHouse::~CGumpCustomHouse()
{
	CPacketExitFromCustomHouseBuilding(0x07).Send();
}
//----------------------------------------------------------------------------------
void CGumpCustomHouse::InitToolTip()
{
	WISPFUN_DEBUG("");
}
//----------------------------------------------------------------------------------
void CGumpCustomHouse::UpdateContent()
{
	WISPFUN_DEBUG("");
}
//----------------------------------------------------------------------------------
void CGumpCustomHouse::GUMP_BUTTON_EVENT_C
{
	WISPFUN_DEBUG("");
}
//----------------------------------------------------------------------------------
