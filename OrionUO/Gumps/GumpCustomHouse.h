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
enum CUSTOM_HOUSE_GUMP_STATE
{
	CHGS_WALL = 0,
	CHGS_DOOR,
	CHGS_FLOOR,
	CHGS_STAIR,
	CHGS_ROOF,
	CHGS_MISC,
	CHGS_MENU
};
//----------------------------------------------------------------------------------
class CGumpCustomHouse : public CGump
{
	SETGET(CUSTOM_HOUSE_GUMP_STATE, State, CHGS_WALL);
	SETGET(int, Category, -1);
	SETGET(int, MaxPage, 1);
	SETGET(ushort, SelectedGraphic, 0);
	SETGET(int, CurrentFloor, 1);
	SETGET(int, FloorCount, 4);
	SETGET(int, RoofZ, 1);
	SETGET(int, MinHouseZ, -120);
	SETGET(bool, Erasing, false);
	SETGET(bool, SeekTile, false);
	SETGET(bool, ShowWindow, false);
	SETGET(WISP_GEOMETRY::CPoint2Di, StartPos, WISP_GEOMETRY::CPoint2Di());
	SETGET(WISP_GEOMETRY::CPoint2Di, EndPos, WISP_GEOMETRY::CPoint2Di());

private:
	vector<CCustomHouseObjectWallCategory> m_Walls;
	vector<CCustomHouseObjectFloorCategory> m_Floors;
	vector<CCustomHouseObjectDoorCategory> m_Doors;
	vector<CCustomHouseObjectMiscCategory> m_Miscs;
	vector<CCustomHouseObjectStairCategory> m_Stairs;
	vector<CCustomHouseObjectTeleportCategory> m_Teleports;
	vector<CCustomHouseObjectRoofCategory> m_Roofs;
	
	enum ID_GUMP_CUSTOM_HOUSE
	{
		ID_GCH_STATE_WALL = 1,
		ID_GCH_STATE_DOOR,
		ID_GCH_STATE_FLOOR,
		ID_GCH_STATE_STAIR,
		ID_GCH_STATE_ROOF,
		ID_GCH_STATE_MISC,
		ID_GCH_STATE_ERASE,
		ID_GCH_STATE_EYEDROPPER,
		ID_GCH_STATE_MENU,

		ID_GCH_VISIBILITY_STORY_1,
		ID_GCH_VISIBILITY_STORY_2,
		ID_GCH_VISIBILITY_STORY_3,
		ID_GCH_VISIBILITY_STORY_4,

		ID_GCH_GO_FLOOR_1,
		ID_GCH_GO_FLOOR_2,
		ID_GCH_GO_FLOOR_3,
		ID_GCH_GO_FLOOR_4,

		ID_GCH_LIST_LEFT,
		ID_GCH_LIST_RIGHT,

		ID_GCH_MENU_BACKUP,
		ID_GCH_MENU_RESTORE,
		ID_GCH_MENU_SYNCH,
		ID_GCH_MENU_CLEAR,
		ID_GCH_MENU_COMMIT,
		ID_GCH_MENU_REVERT,

		ID_GCH_GO_CATEGORY,
		ID_GCH_WALL_SHOW_WINDOW,
		ID_GCH_ROOF_Z_UP,
		ID_GCH_ROOF_Z_DOWN,

		ID_GCH_ITEM_IN_LIST
	};

	CGUIDataBox *m_DataBox{ NULL };
	CGUIDataBox *m_DataBoxGUI{ NULL };

	CGUIText *m_TextItems{ NULL };
	CGUIText *m_TextCost{ NULL };

	void DrawWallSection();
	void DrawDoorSection();
	void DrawFloorSection();
	void DrawStairSection();
	void DrawRoofSection();
	void DrawMiscSection();
	void DrawMenuSection();

	void UpdateMaxPage();

protected:
	virtual void CalculateGumpState();

public:
	CGumpCustomHouse(const uint &serial, const int &x, const int &y);
	virtual ~CGumpCustomHouse();

	virtual void UpdateContent();

	virtual void InitToolTip();

	void SeekGraphic(const ushort &graphic);

	GUMP_BUTTON_EVENT_H;
	GUMP_TEXT_ENTRY_EVENT_H;

	virtual void OnLeftMouseButtonUp();
};
//----------------------------------------------------------------------------------
extern CGumpCustomHouse *g_CustomHouseGump;
//----------------------------------------------------------------------------------
#endif //GUMPCUSTOMHOUSE_H
//----------------------------------------------------------------------------------
