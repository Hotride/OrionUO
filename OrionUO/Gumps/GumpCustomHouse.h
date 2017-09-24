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
