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
enum CUSTOM_HOUSE_FLOOR_VISION_STATE
{
	CHGVS_NORMAL = 0,
	CHGVS_TRANSPARENT_CONTENT,
	CHGVS_HIDE_CONTENT,
	CHGVS_TRANSPARENT_FLOOR,
	CHGVS_HIDE_FLOOR,
	CHGVS_TRANSLUCENT_FLOOR,
	CHGVS_HIDE_ALL
};
//----------------------------------------------------------------------------------
enum CUSTOM_HOUSE_BUILD_TYPE
{
	CHBT_NORMAL = 0,
	CHBT_ROOF,
	CHBT_FLOOR,
	CHBT_STAIR
};
//----------------------------------------------------------------------------------
enum CUSTOM_HOUSE_MULTI_OBJECT_FLAGS
{
	CHMOF_GENERIC_INTERNAL = 0x01,
	CHMOF_FLOOR = 0x02,
	CHMOF_STAIR = 0x04,
	CHMOF_ROOF = 0x08,
	CHMOF_TRANSPARENT = 0x10,
	CHMOF_IGNORE_IN_RENDER = 0x20
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
	SETGET(int, Components, 0);
	SETGET(int, Fixtures, 0);
	SETGET(int, MaxComponents, 0);
	SETGET(int, MaxFixtures, 0);
	SETGET(bool, Erasing, false);
	SETGET(bool, SeekTile, false);
	SETGET(bool, ShowWindow, false);
	SETGET(bool, CombinedStair, false);
	SETGET(WISP_GEOMETRY::CPoint2Di, StartPos, WISP_GEOMETRY::CPoint2Di());
	SETGET(WISP_GEOMETRY::CPoint2Di, EndPos, WISP_GEOMETRY::CPoint2Di());

private:
	vector<CCustomHouseObjectWallCategory> m_Walls;
	vector<CCustomHouseObjectFloor> m_Floors;
	vector<CCustomHouseObjectDoor> m_Doors;
	vector<CCustomHouseObjectMiscCategory> m_Miscs;
	vector<CCustomHouseObjectStair> m_Stairs;
	vector<CCustomHouseObjectTeleport> m_Teleports;
	vector<CCustomHouseObjectRoofCategory> m_Roofs;
	vector<CCustomHouseObjectPlaceInfo> m_ObjectsInfo;
	
	int m_FloorVisionState[4];

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

		ID_GCH_AREA_OBJECTS_INFO,
		ID_GCH_AREA_COST_INFO,
		ID_GCH_AREA_ROOF_Z_INFO,

		ID_GCH_ITEM_IN_LIST
	};

	CGUIDataBox *m_DataBox{ NULL };
	CGUIDataBox *m_DataBoxGUI{ NULL };

	CGUIGumppic *m_Gumppic{ NULL };

	CGUIText *m_TextComponents{ NULL };
	CGUIText *m_TextFixtures{ NULL };
	CGUIText *m_TextCost{ NULL };

	void DrawWallSection();
	void DrawDoorSection();
	void DrawFloorSection();
	void DrawStairSection();
	void DrawRoofSection();
	void DrawMiscSection();
	void DrawMenuSection();

	void UpdateMaxPage();

	pair<int, int> ExistsInList(CUSTOM_HOUSE_GUMP_STATE &state, const ushort &graphic);

protected:
	virtual void CalculateGumpState();

public:
	CGumpCustomHouse(const uint &serial, const int &x, const int &y);
	virtual ~CGumpCustomHouse();

	virtual void UpdateContent();

	virtual void InitToolTip();

	void GenerateFloorPlace();

	void SeekGraphic(const ushort &graphic);

	bool CanBuildHere(vector<CBuildObject> &list, CRenderWorldObject *place, CUSTOM_HOUSE_BUILD_TYPE &type);

	bool ValidateItemPlace(const RECT rect, const ushort &graphic, const int &x, const int &y, const CUSTOM_HOUSE_BUILD_TYPE &type);

	bool CanEraseHere(CRenderWorldObject *place, CUSTOM_HOUSE_BUILD_TYPE &type);

	void OnTargetWorld(CRenderWorldObject *place);

	GUMP_BUTTON_EVENT_H;
	GUMP_TEXT_ENTRY_EVENT_H;

	virtual void OnLeftMouseButtonUp();
};
//----------------------------------------------------------------------------------
extern CGumpCustomHouse *g_CustomHouseGump;
//----------------------------------------------------------------------------------
#endif //GUMPCUSTOMHOUSE_H
//----------------------------------------------------------------------------------
