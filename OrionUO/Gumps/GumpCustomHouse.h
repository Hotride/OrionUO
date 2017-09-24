/***********************************************************************************
**
** GumpCustomHouse.h
**
** Copyright (C) September 2017 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#ifndef GUMPCUSTOMHOUSE_H
#define GUMPCUSTOMHOUSE_H
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
class CGumpCustomHouse : public CGump
{
private:
	vector<CCustomHouseObjectWall> m_Walls;
	vector<CCustomHouseObjectFloor> m_Floors;
	vector<CCustomHouseObjectDoor> m_Doors;
	vector<CCustomHouseObjectMisc> m_Miscs;
	vector<CCustomHouseObjectStair> m_Stairs;
	vector<CCustomHouseObjectTeleport> m_Teleports;
	vector<CCustomHouseObjectRoof> m_Roofs;

public:
	CGumpCustomHouse(const uint &serial, const int &x, const int &y);
	virtual ~CGumpCustomHouse();

	virtual void UpdateContent();

	virtual void InitToolTip();

	GUMP_BUTTON_EVENT_H;
};
//----------------------------------------------------------------------------------
#endif //GUMPCUSTOMHOUSE_H
//----------------------------------------------------------------------------------
