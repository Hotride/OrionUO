// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
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

CGumpCustomHouse *g_CustomHouseGump = NULL;
//----------------------------------------------------------------------------------
template<class T, class A>
void ParseCustomHouseObjectFileWithCategory(vector<A> &list, const string &path)
{
	LOG("parse CH file (CAT): %s\n", path.c_str());

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
			{
				if (item.FeatureMask && !(g_LockedClientFeatures & item.FeatureMask))
					continue;

				bool found = false;
				for (A &cat : list)
				{
					if (cat.Index == item.Category)
					{
						cat.m_Items.push_back(item);
						found = true;
						break;
					}
				}

				if (!found)
				{
					A cat;
					cat.Index = item.Category;
					cat.m_Items.push_back(item);
					list.push_back(cat);
				}
			}
		}

		fclose(file);
	}
}
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
			{
				if (!item.FeatureMask || (g_LockedClientFeatures & item.FeatureMask))
					list.push_back(item);
			}
		}

		fclose(file);
	}
}
//----------------------------------------------------------------------------------
CGumpCustomHouse::CGumpCustomHouse(const uint &serial, const int &x, const int &y)
: CGump(GT_CUSTOM_HOUSE, serial, x, y)
{
	WISPFUN_DEBUG("");

	m_FloorVisionState[0] = m_FloorVisionState[1] = m_FloorVisionState[2] = m_FloorVisionState[3] = CHGVS_NORMAL;

	g_CustomHouseGump = this;

	ParseCustomHouseObjectFileWithCategory<CCustomHouseObjectWall, CCustomHouseObjectWallCategory>(m_Walls, g_App.UOFilesPath("walls.txt"));
	ParseCustomHouseObjectFile<CCustomHouseObjectFloor>(m_Floors, g_App.UOFilesPath("floors.txt"));
	ParseCustomHouseObjectFile<CCustomHouseObjectDoor>(m_Doors, g_App.UOFilesPath("doors.txt"));
	ParseCustomHouseObjectFileWithCategory<CCustomHouseObjectMisc, CCustomHouseObjectMiscCategory>(m_Miscs, g_App.UOFilesPath("misc.txt"));
	ParseCustomHouseObjectFile<CCustomHouseObjectStair>(m_Stairs, g_App.UOFilesPath("stairs.txt"));
	ParseCustomHouseObjectFile<CCustomHouseObjectTeleport>(m_Teleports, g_App.UOFilesPath("teleprts.txt"));
	ParseCustomHouseObjectFileWithCategory<CCustomHouseObjectRoof, CCustomHouseObjectRoofCategory>(m_Roofs, g_App.UOFilesPath("roof.txt"));
	ParseCustomHouseObjectFile<CCustomHouseObjectPlaceInfo>(m_ObjectsInfo, g_App.UOFilesPath("suppinfo.txt"));

	CGameItem *foundationItem = g_World->GetWorldItem(serial);

	if (foundationItem != NULL)
	{
		m_MinHouseZ = foundationItem->Z + 7;

		CMulti* multi = foundationItem->GetMulti();

		if (multi != NULL)
		{
			m_StartPos.X = foundationItem->X + multi->MinX;
			m_StartPos.Y = foundationItem->Y + multi->MinY;
			m_EndPos.X = foundationItem->X + multi->MaxX + 1;
			m_EndPos.Y = foundationItem->Y + multi->MaxY + 1;
		}

		int width = abs(m_EndPos.X - m_StartPos.X);
		int height = abs(m_EndPos.Y - m_StartPos.Y);

		if (width > 13 || height > 13)
			m_FloorCount = 4;
		else
			m_FloorCount = 3;

		int componentsOnFloor = (width - 1) * (height - 1);

		m_MaxComponents = m_FloorCount * (componentsOnFloor + 2 * (width + height) - 4) - (int)((double)(m_FloorCount * componentsOnFloor) * -0.25) + 2 * width + 3 * height - 5;
		m_MaxFixtures = m_MaxComponents / 20;
	}

	LOG("CH multi Bounds: %i %i %i %i\n", m_StartPos.X, m_StartPos.Y, m_EndPos.X, m_EndPos.Y);
	LOG("m_MaxComponents=%i, m_MaxFixtures=%i\n", m_MaxComponents, m_MaxFixtures);



	Add(new CGUIPage(0));
	Add(new CGUIGumppicTiled(0x0E14, 121, 36, 397, 120));
	m_DataBox = (CGUIDataBox*)Add(new CGUIDataBox());

	Add(new CGUIGumppic(0x55F0, 0, 17));

	m_Gumppic = (CGUIGumppic*)Add(new CGUIGumppic((m_FloorCount == 4 ? 0x55F2 : 0x55F9), 486, 17));

	Add(new CGUIGumppicTiled(0x55F1, 153, 17, 333, 154));


	Add(new CGUIButton(ID_GCH_STATE_WALL, 0x5654, 0x5655, 0x5656, 9, 41));
	Add(new CGUIButton(ID_GCH_STATE_DOOR, 0x5657, 0x5658, 0x5659, 39, 40));
	Add(new CGUIButton(ID_GCH_STATE_FLOOR, 0x565A, 0x565B, 0x565C, 70, 40));
	Add(new CGUIButton(ID_GCH_STATE_STAIR, 0x565D, 0x565E, 0x565F, 9, 72));
	Add(new CGUIButton(ID_GCH_STATE_ROOF, 0x5788, 0x5789, 0x578A, 39, 72));
	Add(new CGUIButton(ID_GCH_STATE_MISC, 0x5663, 0x5664, 0x5665, 69, 72));
	Add(new CGUIButton(ID_GCH_STATE_MENU, 0x566C, 0x566D, 0x566E, 69, 100));



	m_TextComponents = (CGUIText*)Add(new CGUIText(0x0481, 82, 142));

	CGUIText *text = (CGUIText*)Add(new CGUIText(0x0481, 84, 142));
	text->CreateTextureA(9, ":");

	m_TextFixtures = (CGUIText*)Add(new CGUIText(0x0481, 94, 142));

	m_TextCost = (CGUIText*)Add(new CGUIText(0x0481, 524, 142));

	CGUIHitBox *box = (CGUIHitBox*)Add(new CGUIHitBox(ID_GCH_AREA_OBJECTS_INFO, 36, 137, 84, 23));
	box->MoveOnDrag = true;

	box = (CGUIHitBox*)Add(new CGUIHitBox(ID_GCH_AREA_COST_INFO, 522, 137, 84, 23));
	box->MoveOnDrag = true;

	m_DataBoxGUI = (CGUIDataBox*)Add(new CGUIDataBox());

	UpdateMaxPage();
	m_WantUpdateContent = true;
}
//----------------------------------------------------------------------------------
CGumpCustomHouse::~CGumpCustomHouse()
{
	g_CustomHouseGump = NULL;

	m_DataBox = NULL;
	m_DataBoxGUI = NULL;

	m_Gumppic = NULL;
	m_TextComponents = NULL;
	m_TextFixtures = NULL;
	m_TextCost = NULL;

	CPacketCustomHouseBuildingExit().Send();
	g_Target.SendCancelTarget();
}
//----------------------------------------------------------------------------------
void CGumpCustomHouse::CalculateGumpState()
{
	WISPFUN_DEBUG("c120_f1");
	CGump::CalculateGumpState();

	if (g_GumpTranslate.X || g_GumpTranslate.Y)
		m_WantRedraw = true;
}
//----------------------------------------------------------------------------------
void CGumpCustomHouse::InitToolTip()
{
	WISPFUN_DEBUG("");
	
	uint id = g_SelectedObject.Serial;

	if (id >= ID_GCH_ITEM_IN_LIST)
	{
		int index = id - ID_GCH_ITEM_IN_LIST;

		if (index < 0)
			return;

		int cliloc = 0;

		if (m_Category == -1 && (m_State == CHGS_WALL || m_State == CHGS_ROOF || m_State == CHGS_MISC))
		{
			if (m_State == CHGS_WALL && index < (int)m_Walls.size() && m_Walls[index].m_Items.size())
				cliloc = m_Walls[index].m_Items[0].TID;
			else if (m_State == CHGS_ROOF && index < (int)m_Roofs.size() && m_Roofs[index].m_Items.size())
				cliloc = m_Roofs[index].m_Items[0].TID;
			else if (m_State == CHGS_MISC && index < (int)m_Miscs.size() && m_Roofs[index].m_Items.size())
				cliloc = m_Miscs[index].m_Items[0].TID;
		}
		else if (m_State == CHGS_ROOF)
			cliloc = 1070640 + index;

		if (cliloc)
			g_ToolTip.Set(cliloc, "");

		return;
	}

	switch (id)
	{
		case ID_GCH_STATE_WALL:
		{
			g_ToolTip.Set(1061016, "Walls");
			break;
		}
		case ID_GCH_STATE_DOOR:
		{
			g_ToolTip.Set(1061017, "Doors");
			break;
		}
		case ID_GCH_STATE_FLOOR:
		{
			g_ToolTip.Set(1061018, "Floors");
			break;
		}
		case ID_GCH_STATE_STAIR:
		{
			g_ToolTip.Set(1061019, "Stairs");
			break;
		}
		case ID_GCH_STATE_ROOF:
		{
			g_ToolTip.Set(1063364, "Roofs");
			break;
		}
		case ID_GCH_STATE_MISC:
		{
			g_ToolTip.Set(1061021, "Miscellaneous");
			break;
		}
		case ID_GCH_STATE_ERASE:
		{
			g_ToolTip.Set(1061022, "Erase");
			break;
		}
		case ID_GCH_STATE_EYEDROPPER:
		{
			g_ToolTip.Set(1061023, "Eyedropper Tool");
			break;
		}
		case ID_GCH_STATE_MENU:
		{
			g_ToolTip.Set(1061024, "System Menu");
			break;
		}
		case ID_GCH_VISIBILITY_STORY_1:
		{
			g_ToolTip.Set(1061029, "Store 1 Visibility");
			break;
		}
		case ID_GCH_VISIBILITY_STORY_2:
		{
			g_ToolTip.Set(1061030, "Store 2 Visibility");
			break;
		}
		case ID_GCH_VISIBILITY_STORY_3:
		{
			g_ToolTip.Set(1061031, "Store 3 Visibility");
			break;
		}
		case ID_GCH_VISIBILITY_STORY_4:
		{
			g_ToolTip.Set(1061032, "Store 4 Visibility");
			break;
		}
		case ID_GCH_GO_FLOOR_1:
		{
			g_ToolTip.Set(1061033, "Go To Story 1");
			break;
		}
		case ID_GCH_GO_FLOOR_2:
		{
			g_ToolTip.Set(1061034, "Go To Story 2");
			break;
		}
		case ID_GCH_GO_FLOOR_3:
		{
			g_ToolTip.Set(1061035, "Go To Story 3");
			break;
		}
		case ID_GCH_GO_FLOOR_4:
		{
			g_ToolTip.Set(1061036, "Go To Story 4");
			break;
		}
		case ID_GCH_LIST_LEFT:
		{
			g_ToolTip.Set(1061028, "Previous Page");
			break;
		}
		case ID_GCH_LIST_RIGHT:
		{
			g_ToolTip.Set(1061027, "Next Page");
			break;
		}
		case ID_GCH_MENU_BACKUP:
		{
			g_ToolTip.Set(1061041, "Store design in progress in a back up buffer, but do not finalize design.");
			break;
		}
		case ID_GCH_MENU_RESTORE:
		{
			g_ToolTip.Set(1061043, "Restore your design in progress to a design you have previously backed up.");
			break;
		}
		case ID_GCH_MENU_SYNCH:
		{
			g_ToolTip.Set(1061044, "Synchronize design state with server.");
			break;
		}
		case ID_GCH_MENU_CLEAR:
		{
			g_ToolTip.Set(1061045, "Clear all changes, returning your design in progress to a blank foundation.");
			break;
		}
		case ID_GCH_MENU_COMMIT:
		{
			g_ToolTip.Set(1061042, "Save existing changes and finalize design.");
			break;
		}
		case ID_GCH_MENU_REVERT:
		{
			g_ToolTip.Set(1061047, "Revert your design in progress to match your currently visible, finalized design.");
			break;
		}
		case ID_GCH_GO_CATEGORY:
		{
			g_ToolTip.Set(1061025, "To Category");
			break;
		}
		case ID_GCH_WALL_SHOW_WINDOW:
		{
			g_ToolTip.Set(1061026, "Window Toggle");
			break;
		}
		case ID_GCH_ROOF_Z_DOWN:
		{
			g_ToolTip.Set(1063392, "Lower Roof Placement Level");
			break;
		}
		case ID_GCH_ROOF_Z_UP:
		{
			g_ToolTip.Set(1063393, "Raise Roof Placement Level");
			break;
		}
		case ID_GCH_AREA_OBJECTS_INFO:
		{
			wstring str = g_ClilocManager.ParseArgumentsToClilocString(1061039, true, std::to_wstring(m_MaxComponents) + L"\t" + std::to_wstring(m_MaxFixtures));
			g_ToolTip.Set(str);
			break;
		}
		case ID_GCH_AREA_COST_INFO:
		{
			g_ToolTip.Set(1061038, "Cost");
			break;
		}
		case ID_GCH_AREA_ROOF_Z_INFO:
		{
			g_ToolTip.Set(1070759, "Current Roof Placement Level");
			break;
		}
		default:
			break;
	}
}
//----------------------------------------------------------------------------------
void CGumpCustomHouse::DrawWallSection()
{
	int x = 0;
	int y = 0;

	if (m_Category == -1)
	{
		int startCategory = m_Page * 16;
		int endCategory = startCategory + 16;

		if (endCategory > (int)m_Walls.size())
			endCategory = (int)m_Walls.size();

		m_DataBox->Add(new CGUIScissor(true, 0, 0, 121, 36, 384, 60));

		IFOR(i, startCategory, endCategory)
		{
			const vector<CCustomHouseObjectWall> &vec = m_Walls[i].m_Items;

			if (!vec.size())
				continue;

			WISP_GEOMETRY::CSize dims = g_Orion.GetArtDimension(vec[0].East1, true);

			int offsetX = x + 121 + (48 - dims.Width) / 2;
			int offsetY = y + 36;

			m_DataBox->Add(new CGUITilepic(vec[0].East1, 0, offsetX, offsetY));

			m_DataBox->Add(new CGUIHitBox(ID_GCH_ITEM_IN_LIST + (int)i, offsetX, offsetY, dims.Width, dims.Height, true));

			x += 48;

			if (x >= 384)
			{
				x = 0;
				y += 60;

				m_DataBox->Add(new CGUIScissor(false));
				m_DataBox->Add(new CGUIScissor(true, 0, 0, 121, 96, 384, 60));
			}
		}

		m_DataBox->Add(new CGUIScissor(false));
	}
	else if (m_Category >= 0 && m_Category < (int)m_Walls.size())
	{
		const vector<CCustomHouseObjectWall> &vec = m_Walls[m_Category].m_Items;

		if (m_Page >= 0 && m_Page < (int)vec.size())
		{
			const CCustomHouseObjectWall &item = vec[m_Page];

			m_DataBox->Add(new CGUIScissor(true, 0, 0, 130, 36, 384, 120));

			IFOR(i, 0, 8)
			{
				const ushort &graphic = (m_ShowWindow ? item.m_WindowGraphics[i] : item.m_Graphics[i]);
				
				if (graphic)
				{
					WISP_GEOMETRY::CSize dims = g_Orion.GetArtDimension(graphic, true);

					int offsetX = x + 130 + (48 - dims.Width) / 2;
					int offsetY = y + 36 + (120 - dims.Height) / 2;

					m_DataBox->Add(new CGUITilepic(graphic, 0, offsetX, offsetY));

					m_DataBox->Add(new CGUIHitBox(ID_GCH_ITEM_IN_LIST + (int)i, offsetX, offsetY, dims.Width, dims.Height, true));
				}

				x += 48;
			}

			m_DataBox->Add(new CGUIScissor(false));
		}

		m_DataBoxGUI->Add(new CGUIGumppic(0x55F3, 152, 0));
		m_DataBoxGUI->Add(new CGUIButton(ID_GCH_GO_CATEGORY, 0x5622, 0x5623, 0x5624, 167, 5));

		m_DataBoxGUI->Add(new CGUIGumppic(0x55F4, 218, 4));

		if (m_ShowWindow)
			m_DataBoxGUI->Add(new CGUIButton(ID_GCH_WALL_SHOW_WINDOW, 0x562E, 0x562F, 0x5630, 228, 9));
		else
			m_DataBoxGUI->Add(new CGUIButton(ID_GCH_WALL_SHOW_WINDOW, 0x562B, 0x562C, 0x562D, 228, 9));
	}
}
//----------------------------------------------------------------------------------
void CGumpCustomHouse::DrawDoorSection()
{
	if (m_Page >= 0 && m_Page < (int)m_Doors.size())
	{
		CCustomHouseObjectDoor &item = m_Doors[m_Page];

		int x = 0;
		int y = 0;

		m_DataBox->Add(new CGUIScissor(true, 0, 0, 138, 36, 384, 120));

		IFOR(i, 0, 8)
		{
			const ushort &graphic = item.m_Graphics[i];

			if (graphic)
			{
				WISP_GEOMETRY::CSize dims = g_Orion.GetArtDimension(graphic, true);

				int offsetX = x + 138 + (48 - dims.Width) / 2;

				if (i > 3)
					offsetX -= 20;

				int offsetY = y + 36 + (120 - dims.Height) / 2;

				m_DataBox->Add(new CGUITilepic(graphic, 0, offsetX, offsetY));

				m_DataBox->Add(new CGUIHitBox(ID_GCH_ITEM_IN_LIST + (int)i, offsetX, offsetY, dims.Width, dims.Height, true));
			}

			x += 48;
		}

		int direction = 0;

		switch (item.Category)
		{
			case 16:
			case 17:
			case 18:
				direction = 1;
				break;
			case 15:
				direction = 2;
				break;
			case 19:
			case 20:
			case 21:
			case 22:
			case 23:
			case 26:
			case 27:
			case 28:
			case 29:
			case 31:
			case 32:
			case 34:
				direction = 3;
				break;
			case 30:
			case 33:
				direction = 4;
				break;
			default:
				break;
		}

		switch (direction)
		{
			case 0:
			{
				m_DataBox->Add(new CGUIGumppic(0x5780, 151, 39));
				m_DataBox->Add(new CGUIGumppic(0x5781, 196, 39));
				m_DataBox->Add(new CGUIGumppic(0x5782, 219, 133));
				m_DataBox->Add(new CGUIGumppic(0x5783, 266, 136));
				m_DataBox->Add(new CGUIGumppic(0x5784, 357, 136));
				m_DataBox->Add(new CGUIGumppic(0x5785, 404, 133));
				m_DataBox->Add(new CGUIGumppic(0x5786, 431, 39));
				m_DataBox->Add(new CGUIGumppic(0x5787, 474, 39));
				break;
			}
			case 1:
			{
				m_DataBox->Add(new CGUIGumppic(0x5785, 245, 39));
				m_DataBox->Add(new CGUIGumppic(0x5787, 290, 39));
				m_DataBox->Add(new CGUIGumppic(0x5780, 337, 39));
				m_DataBox->Add(new CGUIGumppic(0x5782, 380, 39));
				break;
			}
			case 2:
			{
				m_DataBox->Add(new CGUIGumppic(0x5782, 219, 133));
				m_DataBox->Add(new CGUIGumppic(0x5785, 404, 133));
				break;
			}
			case 3:
			{
				m_DataBox->Add(new CGUIGumppic(0x5780, 245, 39));
				m_DataBox->Add(new CGUIGumppic(0x5781, 290, 39));
				m_DataBox->Add(new CGUIGumppic(0x5786, 337, 39));
				m_DataBox->Add(new CGUIGumppic(0x5787, 380, 39));
				break;
			}
			case 4:
			{
				m_DataBox->Add(new CGUIGumppic(0x5780, 151, 39));
				m_DataBox->Add(new CGUIGumppic(0x5781, 196, 39));
				m_DataBox->Add(new CGUIGumppic(0x5780, 245, 39));
				m_DataBox->Add(new CGUIGumppic(0x5781, 290, 39));
				m_DataBox->Add(new CGUIGumppic(0x5786, 337, 39));
				m_DataBox->Add(new CGUIGumppic(0x5787, 380, 39));
				m_DataBox->Add(new CGUIGumppic(0x5786, 431, 39));
				m_DataBox->Add(new CGUIGumppic(0x5787, 474, 39));
				break;
			}
			default:
				break;
		}

		m_DataBox->Add(new CGUIScissor(false));
	}
}
//----------------------------------------------------------------------------------
void CGumpCustomHouse::DrawFloorSection()
{
	if (m_Page >= 0 && m_Page < (int)m_Floors.size())
	{
		CCustomHouseObjectFloor &item = m_Floors[m_Page];

		int x = 0;
		int y = 0;

		m_DataBox->Add(new CGUIScissor(true, 0, 0, 123, 36, 384, 120));

		int index = 0;

		IFOR(j, 0, 2)
		{
			IFOR(i, 0, 8)
			{
				const ushort &graphic = item.m_Graphics[index];

				if (graphic)
				{
					WISP_GEOMETRY::CSize dims = g_Orion.GetArtDimension(graphic, true);

					int offsetX = x + 123 + (48 - dims.Width) / 2;
					int offsetY = y + 36 + (60 - dims.Height) / 2;

					m_DataBox->Add(new CGUITilepic(graphic, 0, offsetX, offsetY));

					m_DataBox->Add(new CGUIHitBox(ID_GCH_ITEM_IN_LIST + index, offsetX, offsetY, dims.Width, dims.Height, true));
				}

				x += 48;
				index++;
			}

			x = 0;
			y += 60;
		}

		m_DataBox->Add(new CGUIScissor(false));
	}
}
//----------------------------------------------------------------------------------
void CGumpCustomHouse::DrawStairSection()
{
	if (m_Page >= 0 && m_Page < (int)m_Stairs.size())
	{
		CCustomHouseObjectStair &item = m_Stairs[m_Page];

		IFOR(j, 0, 2)
		{
			int x = (j ? 96 : 192);
			int y = (j ? 60 : 0);

			m_DataBox->Add(new CGUIScissor(true, 0, 0, 123, 36 + y, 384, 60));

			CGUIText *text = (CGUIText*)m_DataBox->Add(new CGUIText(0xFFFF, 137, (j ? 111 : 51)));
			text->CreateTextureW(0, g_ClilocManager.Cliloc(g_Language)->GetW(1062113 + (int)j), 30, 90);

			int start = (j ? 0 : 5);
			int end = (j ? 6 : 9);
			int combinedStair = (j ? 0 : 10);

			IFOR(i, start, end)
			{
				const ushort &graphic = item.m_Graphics[i];

				if (graphic)
				{
					WISP_GEOMETRY::CSize dims = g_Orion.GetArtDimension(graphic, true);

					int offsetX = x + 123 + (48 - dims.Width) / 2;
					int offsetY = y + 36 + (60 - dims.Height) / 2;

					m_DataBox->Add(new CGUITilepic(graphic, 0, offsetX, offsetY));

					m_DataBox->Add(new CGUIHitBox(ID_GCH_ITEM_IN_LIST + (int)i + combinedStair, offsetX, offsetY, dims.Width, dims.Height, true));
				}

				x += 48;
			}

			m_DataBox->Add(new CGUIScissor(false));
		}

		m_DataBox->Add(new CGUIColoredPolygone(0, 0, 123, 96, 384, 2, 0xFF7F7F7F));
	}
}
//----------------------------------------------------------------------------------
void CGumpCustomHouse::DrawRoofSection()
{
	int x = 0;
	int y = 0;

	if (m_Category == -1)
	{
		int startCategory = m_Page * 16;
		int endCategory = startCategory + 16;

		if (endCategory > (int)m_Roofs.size())
			endCategory = (int)m_Roofs.size();

		m_DataBox->Add(new CGUIScissor(true, 0, 0, 121, 36, 384, 60));

		IFOR(i, startCategory, endCategory)
		{
			const vector<CCustomHouseObjectRoof> &vec = m_Roofs[i].m_Items;

			if (!vec.size())
				continue;

			WISP_GEOMETRY::CSize dims = g_Orion.GetArtDimension(vec[0].NSCrosspiece, true);

			int offsetX = x + 121 + (48 - dims.Width) / 2;
			int offsetY = y + 36;

			m_DataBox->Add(new CGUITilepic(vec[0].NSCrosspiece, 0, offsetX, offsetY));

			m_DataBox->Add(new CGUIHitBox(ID_GCH_ITEM_IN_LIST + (int)i, offsetX, offsetY, dims.Width, dims.Height, true));

			x += 48;

			if (x >= 384)
			{
				x = 0;
				y += 60;

				m_DataBox->Add(new CGUIScissor(false));
				m_DataBox->Add(new CGUIScissor(true, 0, 0, 121, 96, 384, 60));
			}
		}

		m_DataBox->Add(new CGUIScissor(false));
	}
	else if (m_Category >= 0 && m_Category < (int)m_Roofs.size())
	{
		const vector<CCustomHouseObjectRoof> &vec = m_Roofs[m_Category].m_Items;

		if (m_Page >= 0 && m_Page < (int)vec.size())
		{
			const CCustomHouseObjectRoof &item = vec[m_Page];

			m_DataBox->Add(new CGUIScissor(true, 0, 0, 130, 44, 384, 120));

			int index = 0;

			IFOR(j, 0, 2)
			{
				IFOR(i, 0, 8)
				{
					const ushort &graphic = item.m_Graphics[index];

					if (graphic)
					{
						WISP_GEOMETRY::CSize dims = g_Orion.GetArtDimension(graphic, true);

						int offsetX = x + 130 + (48 - dims.Width) / 2;
						int offsetY = y + 44 + (60 - dims.Height) / 2;

						m_DataBox->Add(new CGUITilepic(graphic, 0, offsetX, offsetY));

						m_DataBox->Add(new CGUIHitBox(ID_GCH_ITEM_IN_LIST + index, offsetX, offsetY, dims.Width, dims.Height, true));
					}

					x += 48;
					index++;
				}

				x = 0;
				y += 60;
			}

			m_DataBox->Add(new CGUIScissor(false));
		}

		m_DataBoxGUI->Add(new CGUIGumppic(0x55F3, 152, 0));
		m_DataBoxGUI->Add(new CGUIButton(ID_GCH_GO_CATEGORY, 0x5622, 0x5623, 0x5624, 167, 5));

		m_DataBoxGUI->Add(new CGUIButton(ID_GCH_ROOF_Z_DOWN, 0x578B, 0x578C, 0x578D, 305, 0));
		m_DataBoxGUI->Add(new CGUIButton(ID_GCH_ROOF_Z_UP, 0x578E, 0x578F, 0x5790, 349, 0));

		CGUIGumppic *gumppic = (CGUIGumppic*)m_DataBoxGUI->Add(new CGUIGumppic(0x55F4, 383, 4));
		gumppic->Serial = ID_GCH_AREA_ROOF_Z_INFO;
		gumppic->MoveOnDrag = true;

		CGUIText *text = (CGUIText*)m_DataBoxGUI->Add(new CGUIText(0x04E9, 405, 15));
		text->CreateTextureA(3, std::to_string(m_RoofZ));
		text->Serial = ID_GCH_AREA_ROOF_Z_INFO;
		text->MoveOnDrag = true;
	}
}
//----------------------------------------------------------------------------------
void CGumpCustomHouse::DrawMiscSection()
{
	int x = 0;
	int y = 0;

	if (m_Category == -1)
	{
		int startCategory = m_Page * 16;
		int endCategory = startCategory + 16;

		if (endCategory > (int)m_Miscs.size())
			endCategory = (int)m_Miscs.size();

		m_DataBox->Add(new CGUIScissor(true, 0, 0, 121, 36, 384, 60));

		IFOR(i, startCategory, endCategory)
		{
			const vector<CCustomHouseObjectMisc> &vec = m_Miscs[i].m_Items;

			if (!vec.size())
				continue;

			WISP_GEOMETRY::CSize dims = g_Orion.GetArtDimension(vec[0].Piece5, true);

			int offsetX = x + 121 + (48 - dims.Width) / 2;
			int offsetY = y + 36;

			m_DataBox->Add(new CGUITilepic(vec[0].Piece5, 0, offsetX, offsetY));

			m_DataBox->Add(new CGUIHitBox(ID_GCH_ITEM_IN_LIST + (int)i, offsetX, offsetY, dims.Width, dims.Height, true));

			x += 48;

			if (x >= 384)
			{
				x = 0;
				y += 60;

				m_DataBox->Add(new CGUIScissor(false));
				m_DataBox->Add(new CGUIScissor(true, 0, 0, 121, 96, 384, 60));
			}
		}

		m_DataBox->Add(new CGUIScissor(false));
	}
	else if (m_Category >= 0 && m_Category < (int)m_Miscs.size())
	{
		const vector<CCustomHouseObjectMisc> &vec = m_Miscs[m_Category].m_Items;

		if (m_Page >= 0 && m_Page < (int)vec.size())
		{
			const CCustomHouseObjectMisc &item = vec[m_Page];

			m_DataBox->Add(new CGUIScissor(true, 0, 0, 130, 44, 384, 120));

			IFOR(i, 0, 8)
			{
				const ushort &graphic = item.m_Graphics[i];

				if (graphic)
				{
					WISP_GEOMETRY::CSize dims = g_Orion.GetArtDimension(graphic, true);

					int offsetX = x + 130 + (48 - dims.Width) / 2;
					int offsetY = y + 44 + (120 - dims.Height) / 2;

					m_DataBox->Add(new CGUITilepic(graphic, 0, offsetX, offsetY));

					m_DataBox->Add(new CGUIHitBox(ID_GCH_ITEM_IN_LIST + (int)i, offsetX, offsetY, dims.Width, dims.Height, true));
				}

				x += 48;
			}

			m_DataBox->Add(new CGUIScissor(false));
		}

		m_DataBoxGUI->Add(new CGUIGumppic(0x55F3, 152, 0));
		m_DataBoxGUI->Add(new CGUIButton(ID_GCH_GO_CATEGORY, 0x5622, 0x5623, 0x5624, 167, 5));
	}
}
//----------------------------------------------------------------------------------
void CGumpCustomHouse::DrawMenuSection()
{
	const int textWidth = 108;

	m_DataBox->Add(new CGUIButton(ID_GCH_MENU_BACKUP, 0x098D, 0x098D, 0x098D, 150, 50));
	CGUITextEntry *entry = (CGUITextEntry*)m_DataBox->Add(new CGUITextEntry(ID_GCH_MENU_BACKUP, 0, 0x0036, 0x0036, 150, 50, textWidth, true, 0, TS_CENTER));
	entry->m_Entry.SetText(L"Backup");
	entry->CheckOnSerial = true;
	entry->ReadOnly = true;
	entry->FocusedOffsetY = 2;

	m_DataBox->Add(new CGUIButton(ID_GCH_MENU_RESTORE, 0x098D, 0x098D, 0x098D, 150, 90));
	entry = (CGUITextEntry*)m_DataBox->Add(new CGUITextEntry(ID_GCH_MENU_RESTORE, 0, 0x0036, 0x0036, 150, 90, textWidth, true, 0, TS_CENTER));
	entry->m_Entry.SetText(L"Restore");
	entry->CheckOnSerial = true;
	entry->ReadOnly = true;
	entry->FocusedOffsetY = 2;

	m_DataBox->Add(new CGUIButton(ID_GCH_MENU_SYNCH, 0x098D, 0x098D, 0x098D, 270, 50));
	entry = (CGUITextEntry*)m_DataBox->Add(new CGUITextEntry(ID_GCH_MENU_SYNCH, 0, 0x0036, 0x0036, 270, 50, textWidth, true, 0, TS_CENTER));
	entry->m_Entry.SetText(L"Synch");
	entry->CheckOnSerial = true;
	entry->ReadOnly = true;
	entry->FocusedOffsetY = 2;

	m_DataBox->Add(new CGUIButton(ID_GCH_MENU_CLEAR, 0x098D, 0x098D, 0x098D, 270, 90));
	entry = (CGUITextEntry*)m_DataBox->Add(new CGUITextEntry(ID_GCH_MENU_CLEAR, 0, 0x0036, 0x0036, 270, 90, textWidth, true, 0, TS_CENTER));
	entry->m_Entry.SetText(L"Clear");
	entry->CheckOnSerial = true;
	entry->ReadOnly = true;
	entry->FocusedOffsetY = 2;

	m_DataBox->Add(new CGUIButton(ID_GCH_MENU_COMMIT, 0x098D, 0x098D, 0x098D, 390, 50));
	entry = (CGUITextEntry*)m_DataBox->Add(new CGUITextEntry(ID_GCH_MENU_COMMIT, 0, 0x0036, 0x0036, 390, 50, textWidth, true, 0, TS_CENTER));
	entry->m_Entry.SetText(L"Commit");
	entry->CheckOnSerial = true;
	entry->ReadOnly = true;
	entry->FocusedOffsetY = 2;

	m_DataBox->Add(new CGUIButton(ID_GCH_MENU_REVERT, 0x098D, 0x098D, 0x098D, 390, 90));
	entry = (CGUITextEntry*)m_DataBox->Add(new CGUITextEntry(ID_GCH_MENU_REVERT, 0, 0x0036, 0x0036, 390, 90, textWidth, true, 0, TS_CENTER));
	entry->m_Entry.SetText(L"Revert");
	entry->CheckOnSerial = true;
	entry->ReadOnly = true;
	entry->FocusedOffsetY = 2;
}
//----------------------------------------------------------------------------------
template<class T, class A>
pair<int, int> SeekGraphicInCustomHouseObjectListWithCategory(const vector<A> &list, const ushort &graphic)
{
	IFOR(i, 0, (int)list.size())
	{
		const A &cat = list[i];

		IFOR(j, 0, (int)cat.m_Items.size())
		{
			int contains = cat.m_Items[j].Contains(graphic);

			if (contains != -1)
				return pair<int, int>((int)i, (int)j);
		}
	}

	return pair<int, int>(-1, -1);
}
//----------------------------------------------------------------------------------
template<class T>
pair<int, int> SeekGraphicInCustomHouseObjectList(const vector<T> &list, const ushort &graphic)
{
	IFOR(i, 0, (int)list.size())
	{
		int contains = list[i].Contains(graphic);

		if (contains != -1)
			return pair<int, int>((int)i, graphic);
	}

	return pair<int, int>(-1, -1);
}
//----------------------------------------------------------------------------------
pair<int, int> CGumpCustomHouse::ExistsInList(CUSTOM_HOUSE_GUMP_STATE &state, const ushort &graphic)
{
	pair<int, int> result = SeekGraphicInCustomHouseObjectListWithCategory<CCustomHouseObjectWall, CCustomHouseObjectWallCategory>(m_Walls, graphic);

	if (result.first == -1 || result.second == -1)
	{
		result = SeekGraphicInCustomHouseObjectList<CCustomHouseObjectFloor>(m_Floors, graphic);

		if (result.first == -1 || result.second == -1)
		{
			result = SeekGraphicInCustomHouseObjectList<CCustomHouseObjectDoor>(m_Doors, graphic);

			if (result.first == -1 || result.second == -1)
			{
				result = SeekGraphicInCustomHouseObjectListWithCategory<CCustomHouseObjectMisc, CCustomHouseObjectMiscCategory>(m_Miscs, graphic);

				if (result.first == -1 || result.second == -1)
				{
					result = SeekGraphicInCustomHouseObjectList<CCustomHouseObjectStair>(m_Stairs, graphic);

					if (result.first == -1 || result.second == -1)
					{
						result = SeekGraphicInCustomHouseObjectListWithCategory<CCustomHouseObjectRoof, CCustomHouseObjectRoofCategory>(m_Roofs, graphic);

						if (result.first != -1 && result.second != -1)
							state = CHGS_ROOF;
					}
					else
						state = CHGS_STAIR;
				}
				else
					state = CHGS_MISC;
			}
			else
				state = CHGS_DOOR;
		}
		else
			state = CHGS_FLOOR;
	}
	else
		state = CHGS_WALL;

	return result;
}
//----------------------------------------------------------------------------------
void CGumpCustomHouse::UpdateContent()
{
	WISPFUN_DEBUG("");

	m_DataBox->Clear();
	m_DataBoxGUI->Clear();

	m_Gumppic->Graphic = (m_FloorCount == 4 ? 0x55F2 : 0x55F9);

	m_DataBoxGUI->Add(new CGUIButton(ID_GCH_STATE_ERASE, 0x5666 + (int)m_Erasing, 0x5667, 0x5668, 9, 100));
	Add(new CGUIButton(ID_GCH_STATE_EYEDROPPER, 0x5669 + (int)m_SeekTile, 0x566A, 0x566B, 39, 100));

	ushort floorVisionGraphic1[3] = { 0x572E, 0x5734, 0x5731 };
	ushort floorVisionGraphic2[3] = { 0x5725, 0x5728, 0x572B };
	ushort floorVisionGraphic3[3] = { 0x571C, 0x571F, 0x5722 };
	int associateGraphicTable[7] = { 0, 1, 2, 1, 2, 1, 2 };

	ushort floorVisionGraphic = floorVisionGraphic1[associateGraphicTable[m_FloorVisionState[0]]];
	int graphicOffset = (m_CurrentFloor == 1 ? 3 : 0);
	int graphicOffset2 = (m_CurrentFloor == 1 ? 4 : 0);
	m_DataBoxGUI->Add(new CGUIButton(ID_GCH_VISIBILITY_STORY_1, floorVisionGraphic, floorVisionGraphic + 1, floorVisionGraphic + 2, 533, 108));
	m_DataBoxGUI->Add(new CGUIButton(ID_GCH_GO_FLOOR_1, 0x56CD + graphicOffset2, 0x56CD + graphicOffset2, 0x56D1, 583, 96));
	m_DataBoxGUI->Add(new CGUIButton(ID_GCH_GO_FLOOR_1, 0x56F6 + graphicOffset, 0x56F7 + graphicOffset, 0x56F8 + graphicOffset, 623, 103));

	floorVisionGraphic = floorVisionGraphic2[associateGraphicTable[m_FloorVisionState[1]]];
	graphicOffset = (m_CurrentFloor == 2 ? 3 : 0);
	graphicOffset2 = (m_CurrentFloor == 2 ? 4 : 0);
	m_DataBoxGUI->Add(new CGUIButton(ID_GCH_VISIBILITY_STORY_2, floorVisionGraphic, floorVisionGraphic + 1, floorVisionGraphic + 2, 533, 86));
	m_DataBoxGUI->Add(new CGUIButton(ID_GCH_GO_FLOOR_2, 0x56CE + graphicOffset2, 0x56CE + graphicOffset2, 0x56D2, 583, 73));
	m_DataBoxGUI->Add(new CGUIButton(ID_GCH_GO_FLOOR_2, 0x56F0 + graphicOffset, 0x56F1 + graphicOffset, 0x56F2 + graphicOffset, 623, 86));

	graphicOffset = (m_CurrentFloor == 3 ? 3 : 0);
	graphicOffset2 = (m_CurrentFloor == 3 ? 4 : 0);

	if (m_FloorCount == 4)
	{
		floorVisionGraphic = floorVisionGraphic2[associateGraphicTable[m_FloorVisionState[2]]];
		m_DataBoxGUI->Add(new CGUIButton(ID_GCH_VISIBILITY_STORY_3, floorVisionGraphic, floorVisionGraphic + 1, floorVisionGraphic + 2, 533, 64));
		m_DataBoxGUI->Add(new CGUIButton(ID_GCH_GO_FLOOR_3, 0x56CE + graphicOffset2, 0x56CE + graphicOffset2, 0x56D2, 582, 56));
		m_DataBoxGUI->Add(new CGUIButton(ID_GCH_GO_FLOOR_3, 0x56F0 + graphicOffset, 0x56F1 + graphicOffset, 0x56F2 + graphicOffset, 623, 69));

		floorVisionGraphic = floorVisionGraphic3[associateGraphicTable[m_FloorVisionState[3]]];
		graphicOffset = (m_CurrentFloor == 4 ? 3 : 0);
		graphicOffset2 = (m_CurrentFloor == 4 ? 4 : 0);
		m_DataBoxGUI->Add(new CGUIButton(ID_GCH_VISIBILITY_STORY_4, floorVisionGraphic, floorVisionGraphic + 1, floorVisionGraphic + 2, 533, 42));
		m_DataBoxGUI->Add(new CGUIButton(ID_GCH_GO_FLOOR_4, 0x56D0 + graphicOffset2, 0x56D0 + graphicOffset2, 0x56D4, 583, 42));
		m_DataBoxGUI->Add(new CGUIButton(ID_GCH_GO_FLOOR_4, 0x56EA + graphicOffset, 0x56EB + graphicOffset, 0x56EC + graphicOffset, 623, 50));
	}
	else
	{
		floorVisionGraphic = floorVisionGraphic3[associateGraphicTable[m_FloorVisionState[2]]];
		m_DataBoxGUI->Add(new CGUIButton(ID_GCH_VISIBILITY_STORY_3, floorVisionGraphic, floorVisionGraphic + 1, floorVisionGraphic + 2, 533, 64));
		m_DataBoxGUI->Add(new CGUIButton(ID_GCH_GO_FLOOR_3, 0x56D0 + graphicOffset2, 0x56D0 + graphicOffset2, 0x56D4, 582, 56));
		m_DataBoxGUI->Add(new CGUIButton(ID_GCH_GO_FLOOR_3, 0x56EA + graphicOffset, 0x56EB + graphicOffset, 0x56EC + graphicOffset, 623, 69));
	}

	switch (m_State)
	{
		case CHGS_WALL:
		{
			DrawWallSection();
			break;
		}
		case CHGS_DOOR:
		{
			DrawDoorSection();
			break;
		}
		case CHGS_FLOOR:
		{
			DrawFloorSection();
			break;
		}
		case CHGS_STAIR:
		{
			DrawStairSection();
			break;
		}
		case CHGS_ROOF:
		{
			DrawRoofSection();
			break;
		}
		case CHGS_MISC:
		{
			DrawMiscSection();
			break;
		}
		case CHGS_MENU:
		{
			DrawMenuSection();
			break;
		}
		default:
			break;
	}

	if (m_MaxPage > 1)
	{
		m_DataBoxGUI->Add(new CGUIButton(ID_GCH_LIST_LEFT, 0x5625, 0x5626, 0x5627, 110, 63));
		m_DataBoxGUI->Add(new CGUIButton(ID_GCH_LIST_RIGHT, 0x5628, 0x5629, 0x562A, 510, 63));
	}

	m_Components = 0;
	m_Fixtures = 0;

	CGameItem *foundationItem = g_World->GetWorldItem(m_Serial);

	if (foundationItem != NULL)
	{
		QFOR(multi, foundationItem->m_Items, CMulti*)
		{
			QFOR(item, multi->m_Items, CMultiObject*)
			{
				if (item->IsCustomHouseMulti() && !(item->State & CHMOF_GENERIC_INTERNAL))
				{
					CUSTOM_HOUSE_GUMP_STATE state;
					pair<int, int> result = ExistsInList(state, item->Graphic);

					if (result.first != -1 && result.second != -1)
					{
						if (state == CHGS_DOOR)
							m_Fixtures++;
						else
							m_Components++;
					}
				}
			}
		}
	}

	m_TextComponents->Color = (m_Components >= m_MaxComponents ? 0x0026 : 0x0481);
	m_TextComponents->CreateTextureA(9, std::to_string(m_Components));
	m_TextComponents->X = 82 - m_TextComponents->m_Texture.Width;

	m_TextFixtures->Color = (m_Fixtures >= m_MaxFixtures ? 0x0026 : 0x0481);
	m_TextFixtures->CreateTextureA(9, std::to_string(m_Fixtures));

	m_TextCost->CreateTextureA(9, std::to_string((m_Components + m_Fixtures) * 500));
}
//----------------------------------------------------------------------------------
void CGumpCustomHouse::UpdateMaxPage()
{
	WISPFUN_DEBUG("");
	m_MaxPage = 1;

	switch (m_State)
	{
		case CHGS_WALL:
		{
			if (m_Category == -1)
				m_MaxPage = (int)ceil(m_Walls.size() / 16.0f);
			else
			{
				for (const CCustomHouseObjectWallCategory &cat : m_Walls)
				{
					if (cat.Index == m_Category)
					{
						m_MaxPage = (int)cat.m_Items.size();
						break;
					}
				}
			}

			break;
		}
		case CHGS_DOOR:
		{
			m_MaxPage = (int)m_Doors.size();

			break;
		}
		case CHGS_FLOOR:
		{
			m_MaxPage = (int)m_Floors.size();

			break;
		}
		case CHGS_STAIR:
		{
			m_MaxPage = (int)m_Stairs.size();

			break;
		}
		case CHGS_ROOF:
		{
			if (m_Category == -1)
				m_MaxPage = (int)ceil(m_Roofs.size() / 16.0f);
			else
			{
				for (const CCustomHouseObjectRoofCategory &cat : m_Roofs)
				{
					if (cat.Index == m_Category)
					{
						m_MaxPage = (int)cat.m_Items.size();
						break;
					}
				}
			}

			break;
		}
		case CHGS_MISC:
		{
			if (m_Category == -1)
				m_MaxPage = (int)ceil(m_Miscs.size() / 16.0f);
			else
			{
				for (const CCustomHouseObjectMiscCategory &cat : m_Miscs)
				{
					if (cat.Index == m_Category)
					{
						m_MaxPage = (int)cat.m_Items.size();
						break;
					}
				}
			}

			break;
		}
		default:
			break;
	}
}
//----------------------------------------------------------------------------------
void CGumpCustomHouse::SeekGraphic(const ushort &graphic)
{
	CUSTOM_HOUSE_GUMP_STATE state;
	pair<int, int> result = ExistsInList(state, graphic);

	if (result.first != -1 && result.second != -1)
	{
		m_State = state;

		if (m_State == CHGS_WALL || m_State == CHGS_ROOF || m_State == CHGS_MISC)
		{
			m_Category = result.first;
			m_Page = result.second;
		}
		else
		{
			m_Category = -1;
			m_Page = result.first;
		}

		UpdateMaxPage();
		g_Target.RequestFromCustomHouse();
		m_WantUpdateContent = true;
		m_SelectedGraphic = graphic;
	}
}
//----------------------------------------------------------------------------------
bool CGumpCustomHouse::CanBuildHere(vector<CBuildObject> &list, CRenderWorldObject *place, CUSTOM_HOUSE_BUILD_TYPE &type)
{
	type = CHBT_NORMAL;

	if (!m_SelectedGraphic)
		return false;

	if (m_CombinedStair)
	{
		if (m_Components + 10 > m_MaxComponents)
			return false;

		pair<int, int> result = SeekGraphicInCustomHouseObjectList<CCustomHouseObjectStair>(m_Stairs, m_SelectedGraphic);

		if (result.first == -1 || result.second == -1 || result.first >= (int)m_Stairs.size())
		{
			list.push_back(CBuildObject(m_SelectedGraphic, 0, 0, 0));
			return false;
		}

		const CCustomHouseObjectStair &item = m_Stairs[result.first];

		if (m_SelectedGraphic == item.North)
		{
			list.push_back(CBuildObject(item.Block, 0, -3, 0));
			list.push_back(CBuildObject(item.Block, 0, -2, 0));
			list.push_back(CBuildObject(item.Block, 0, -1, 0));
			list.push_back(CBuildObject(item.North, 0, 0, 0));
			list.push_back(CBuildObject(item.Block, 0, -3, 5));
			list.push_back(CBuildObject(item.Block, 0, -2, 5));
			list.push_back(CBuildObject(item.North, 0, -1, 5));
			list.push_back(CBuildObject(item.Block, 0, -3, 10));
			list.push_back(CBuildObject(item.North, 0, -2, 10));
			list.push_back(CBuildObject(item.North, 0, -3, 15));
		}
		else if (m_SelectedGraphic == item.East)
		{
			list.push_back(CBuildObject(item.East, 0, 0, 0));
			list.push_back(CBuildObject(item.Block, 1, 0, 0));
			list.push_back(CBuildObject(item.Block, 2, 0, 0));
			list.push_back(CBuildObject(item.Block, 3, 0, 0));
			list.push_back(CBuildObject(item.East, 1, 0, 5));
			list.push_back(CBuildObject(item.Block, 2, 0, 5));
			list.push_back(CBuildObject(item.Block, 3, 0, 5));
			list.push_back(CBuildObject(item.East, 2, 0, 10));
			list.push_back(CBuildObject(item.Block, 3, 0, 10));
			list.push_back(CBuildObject(item.East, 3, 0, 15));
		}
		else if (m_SelectedGraphic == item.South)
		{
			list.push_back(CBuildObject(item.South, 0, 0, 0));
			list.push_back(CBuildObject(item.Block, 0, 1, 0));
			list.push_back(CBuildObject(item.Block, 0, 2, 0));
			list.push_back(CBuildObject(item.Block, 0, 3, 0));
			list.push_back(CBuildObject(item.South, 0, 1, 5));
			list.push_back(CBuildObject(item.Block, 0, 2, 5));
			list.push_back(CBuildObject(item.Block, 0, 3, 5));
			list.push_back(CBuildObject(item.South, 0, 2, 10));
			list.push_back(CBuildObject(item.Block, 0, 3, 10));
			list.push_back(CBuildObject(item.South, 0, 3, 15));
		}
		else if (m_SelectedGraphic == item.West)
		{
			list.push_back(CBuildObject(item.Block, -3, 0, 0));
			list.push_back(CBuildObject(item.Block, -2, 0, 0));
			list.push_back(CBuildObject(item.Block, -1, 0, 0));
			list.push_back(CBuildObject(item.West, 0, 0, 0));
			list.push_back(CBuildObject(item.Block, -3, 0, 5));
			list.push_back(CBuildObject(item.Block, -2, 0, 5));
			list.push_back(CBuildObject(item.West, -1, 0, 5));
			list.push_back(CBuildObject(item.Block, -3, 0, 10));
			list.push_back(CBuildObject(item.West, -2, 0, 10));
			list.push_back(CBuildObject(item.West, -3, 0, 15));
		}
		else
			list.push_back(CBuildObject(m_SelectedGraphic, 0, 0, 0));

		type = CHBT_STAIR;
	}
	else
	{
		pair<int, int> fixtureCheck = SeekGraphicInCustomHouseObjectList<CCustomHouseObjectDoor>(m_Doors, m_SelectedGraphic);
		bool isFixture = false;

		if (fixtureCheck.first == -1 || fixtureCheck.second == -1)
		{
			fixtureCheck = SeekGraphicInCustomHouseObjectList<CCustomHouseObjectTeleport>(m_Teleports, m_SelectedGraphic);
			isFixture = (fixtureCheck.first != -1 && fixtureCheck.second != -1);
		}
		else
			isFixture = true;

		if (isFixture)
		{
			if (m_Fixtures + 1 > m_MaxFixtures)
				return false;
		}
		else if (m_Components + 1 > m_MaxComponents)
			return false;

		if (m_State == CHGS_ROOF)
		{
			list.push_back(CBuildObject(m_SelectedGraphic, 0, 0, (m_RoofZ - 2) * 3));
			type = CHBT_ROOF;
		}
		else
		{
			if (m_State == CHGS_STAIR)
			{
				type = CHBT_STAIR;
				list.push_back(CBuildObject(m_SelectedGraphic, 0, 1, 0));
			}
			else
			{
				if (m_State == CHGS_FLOOR)
					type = CHBT_FLOOR;

				list.push_back(CBuildObject(m_SelectedGraphic, 0, 0, 0));
			}
		}
	}

	if (g_SelectedObject.Object != NULL && g_SelectedObject.Object->IsWorldObject())
	{
		CRenderWorldObject *rwo = (CRenderWorldObject*)g_SelectedObject.Object;

		if ((type != CHBT_STAIR || m_CombinedStair) && rwo->Z < m_MinHouseZ && (rwo->X == m_EndPos.X - 1 || rwo->Y == m_EndPos.Y - 1))
			return false;

		CGameItem *foundationItem = g_World->GetWorldItem(m_Serial);

		int minZ = (foundationItem != NULL ? foundationItem->Z : 0) + 7 + (m_CurrentFloor - 1) * 20;
		int maxZ = minZ + 20;

		int boundsOffset = (int)(m_State != CHGS_WALL);
		RECT rect = { m_StartPos.X + boundsOffset, m_StartPos.Y + boundsOffset, m_EndPos.X, m_EndPos.Y };

		for (const CBuildObject &item : list)
		{
			if (type == CHBT_STAIR)
			{
				if (m_CombinedStair)
				{
					if (item.Z)
						continue;
				}
				else
				{
					if (rwo->Y + item.Y < m_EndPos.Y || rwo->X + item.X == m_StartPos.X || rwo->Z >= m_MinHouseZ)
						return false;
					else if (rwo->Y + item.Y != m_EndPos.Y)
						list[0].Y = 0;

					continue;
				}
			}

			if (!ValidateItemPlace(rect, item.Graphic, rwo->X + item.X, rwo->Y + item.Y))
				return false;

			if (type != CHBT_FLOOR && foundationItem != NULL)
			{
				CMulti *multi = foundationItem->GetMultiAtXY(rwo->X + item.X, rwo->Y + item.Y);

				if (multi != NULL)
				{
					QFOR(multiObject, multi->m_Items, CMultiObject*)
					{
						if (multiObject->IsCustomHouseMulti() && !(multiObject->State & CHMOF_GENERIC_INTERNAL) && multiObject->Z >= minZ && multiObject->Z < maxZ)
						{
							if (type == CHBT_STAIR)
							{
								if (!(multiObject->State & CHMOF_FLOOR))
									return false;
							}
							else
							{
								if (multiObject->State & CHMOF_STAIR)
									return false;
							}
						}
					}
				}
			}
		}
	}
	else
		return false;

	return true;
}
//----------------------------------------------------------------------------------
bool CGumpCustomHouse::ValidateItemPlace(const RECT &rect, const ushort &graphic, const int &x, const int &y)
{
	POINT pos = { x, y };

	if (!PtInRect(&rect, pos))
		return false;

	pair<int, int> infoCheck = SeekGraphicInCustomHouseObjectList<CCustomHouseObjectPlaceInfo>(m_ObjectsInfo, graphic);

	if (infoCheck.first != -1 && infoCheck.second != -1)
	{
		const CCustomHouseObjectPlaceInfo &info = m_ObjectsInfo[infoCheck.first];

		if (!info.CanGoW && x == m_StartPos.X)
			return false;
		else if (!info.CanGoN && y == m_StartPos.Y)
			return false;
		if (!info.CanGoNWS && x == m_StartPos.X && y == m_StartPos.Y)
			return false;
	}

	return true;
}
//----------------------------------------------------------------------------------
bool CGumpCustomHouse::ValidatePlaceStructure(CGameItem *foundationItem, CMulti *multi, const int &minZ, const int &maxZ, const int &flags)
{
	if (multi == NULL)
		return false;

	QFOR(item, multi->m_Items, CMultiObject*)
	{
		vector<WISP_GEOMETRY::CPoint2Di> validatedFloors;

		if (item->IsCustomHouseMulti() && !(item->State & (CHMOF_FLOOR | CHMOF_STAIR | CHMOF_ROOF | CHMOF_FIXTURE)) && item->Z >= minZ && item->Z < maxZ)
		{
			pair<int, int> infoCheck = SeekGraphicInCustomHouseObjectList<CCustomHouseObjectPlaceInfo>(m_ObjectsInfo, item->Graphic);

			if (infoCheck.first != -1 && infoCheck.second != -1)
			{
				const CCustomHouseObjectPlaceInfo &info = m_ObjectsInfo[infoCheck.first];

				if (flags & CHVCF_DIRECT_SUPPORT)
				{
					if ((item->State & CHMOF_INCORRECT_PLACE) || !info.DirectSupports)
						continue;

					if (flags & CHVCF_CANGO_W)
					{
						if (info.CanGoW)
							return true;
					}
					else if (flags & CHVCF_CANGO_N)
					{
						if (info.CanGoN)
							return true;
					}
					else
						return true;
				}
				else if (((flags & CHVCF_BOTTOM) && info.Bottom) || ((flags & CHVCF_TOP) && info.Top))
				{
					if (!(item->State & CHMOF_VALIDATED_PLACE))
					{
						if (!ValidateItemPlace(foundationItem, item, minZ, maxZ, validatedFloors))
							item->State = item->State | CHMOF_VALIDATED_PLACE | CHMOF_INCORRECT_PLACE;
						else
							item->State = item->State | CHMOF_VALIDATED_PLACE;
					}

					if (!(item->State & CHMOF_INCORRECT_PLACE))
					{
						if (flags & CHVCF_BOTTOM)
						{
							if ((flags & CHVCF_N) && info.AdjUN)
								return true;
							else if ((flags & CHVCF_E) && info.AdjUE)
								return true;
							else if ((flags & CHVCF_S) && info.AdjUS)
								return true;
							else if ((flags & CHVCF_W) && info.AdjUW)
								return true;
						}
						else
						{
							if ((flags & CHVCF_N) && info.AdjLN)
								return true;
							else if ((flags & CHVCF_E) && info.AdjLE)
								return true;
							else if ((flags & CHVCF_S) && info.AdjLS)
								return true;
							else if ((flags & CHVCF_W) && info.AdjLW)
								return true;
						}
					}
				}
			}
		}
	}

	return false;
}
//----------------------------------------------------------------------------------
bool CGumpCustomHouse::ValidateItemPlace(CGameItem *foundationItem, CMultiObject *item, const int &minZ, const int &maxZ, vector<WISP_GEOMETRY::CPoint2Di> &validatedFloors)
{
	if (item == NULL || !item->IsCustomHouseMulti())
		return true;
	else if (item->State & CHMOF_FLOOR)
	{
		auto existsInList = [](vector<WISP_GEOMETRY::CPoint2Di> &validatedFloors, const WISP_GEOMETRY::CPoint2Di &testPoint)
		{
			for (const WISP_GEOMETRY::CPoint2Di &point : validatedFloors)
			{
				if (testPoint.X == point.X && testPoint.Y == point.Y)
					return true;
			}

			return false;
		};

		//if (existsInList(validatedFloors, WISP_GEOMETRY::CPoint2Di(item->X, item->Y)))
		//	return false;

		if (ValidatePlaceStructure(foundationItem, foundationItem->GetMultiAtXY(item->X, item->Y), minZ - 20, maxZ - 20, CHVCF_DIRECT_SUPPORT) ||
			ValidatePlaceStructure(foundationItem, foundationItem->GetMultiAtXY(item->X - 1, item->Y), minZ - 20, maxZ - 20, CHVCF_DIRECT_SUPPORT | CHVCF_CANGO_W) ||
			ValidatePlaceStructure(foundationItem, foundationItem->GetMultiAtXY(item->X, item->Y - 1), minZ - 20, maxZ - 20, CHVCF_DIRECT_SUPPORT | CHVCF_CANGO_N))
		{
			const WISP_GEOMETRY::CPoint2Di table[4] =
			{
				WISP_GEOMETRY::CPoint2Di(-1, 0),
				WISP_GEOMETRY::CPoint2Di(0, -1),
				WISP_GEOMETRY::CPoint2Di(1, 0),
				WISP_GEOMETRY::CPoint2Di(0, 1)
			};

			IFOR(i, 0, 4)
			{
				WISP_GEOMETRY::CPoint2Di testPoint(item->X + table[i].X, item->Y + table[i].Y);

				if (!existsInList(validatedFloors, testPoint))
					validatedFloors.push_back(testPoint);
			}

			return true;
		}

		return false;
	}
	else if (item->State & (CHMOF_STAIR | CHMOF_ROOF | CHMOF_FIXTURE))
	{
		for (CMultiObject *temp = item; temp != NULL; temp = (CMultiObject*)temp->m_Prev)
		{
			if ((temp->State & CHMOF_FLOOR) && temp->Z >= minZ && temp->Z < maxZ)
			{
				if ((temp->State & CHMOF_VALIDATED_PLACE) && !(temp->State & CHMOF_INCORRECT_PLACE))
					return true;
			}
		}

		for (CMultiObject *temp = item; temp != NULL; temp = (CMultiObject*)temp->m_Next)
		{
			if ((temp->State & CHMOF_FLOOR) && temp->Z >= minZ && temp->Z < maxZ)
			{
				if ((temp->State & CHMOF_VALIDATED_PLACE) && !(temp->State & CHMOF_INCORRECT_PLACE))
					return true;
			}
		}

		return false;
	}

	pair<int, int> infoCheck = SeekGraphicInCustomHouseObjectList<CCustomHouseObjectPlaceInfo>(m_ObjectsInfo, item->Graphic);

	if (infoCheck.first != -1 && infoCheck.second != -1)
	{
		const CCustomHouseObjectPlaceInfo &info = m_ObjectsInfo[infoCheck.first];

		if (!info.CanGoW && item->X == m_StartPos.X)
			return false;
		else if (!info.CanGoN && item->Y == m_StartPos.Y)
			return false;
		else if (!info.CanGoNWS && item->X == m_StartPos.X && item->Y == m_StartPos.Y)
			return false;

		if (!info.Bottom)
		{
			bool found = false;

			if (info.AdjUN)
				found = ValidatePlaceStructure(foundationItem, foundationItem->GetMultiAtXY(item->X, item->Y + 1), minZ, maxZ, CHVCF_BOTTOM | CHVCF_N);

			if (!found && info.AdjUE)
				found = ValidatePlaceStructure(foundationItem, foundationItem->GetMultiAtXY(item->X - 1, item->Y), minZ, maxZ, CHVCF_BOTTOM | CHVCF_E);

			if (!found && info.AdjUS)
				found = ValidatePlaceStructure(foundationItem, foundationItem->GetMultiAtXY(item->X, item->Y - 1), minZ, maxZ, CHVCF_BOTTOM | CHVCF_S);

			if (!found && info.AdjUW)
				found = ValidatePlaceStructure(foundationItem, foundationItem->GetMultiAtXY(item->X + 1, item->Y), minZ, maxZ, CHVCF_BOTTOM | CHVCF_W);

			if (!found)
				return false;
		}

		if (!info.Top)
		{
			bool found = false;

			if (info.AdjLN)
				found = ValidatePlaceStructure(foundationItem, foundationItem->GetMultiAtXY(item->X, item->Y + 1), minZ, maxZ, CHVCF_TOP | CHVCF_N);

			if (!found && info.AdjLE)
				found = ValidatePlaceStructure(foundationItem, foundationItem->GetMultiAtXY(item->X - 1, item->Y), minZ, maxZ, CHVCF_TOP | CHVCF_E);

			if (!found && info.AdjLS)
				found = ValidatePlaceStructure(foundationItem, foundationItem->GetMultiAtXY(item->X, item->Y - 1), minZ, maxZ, CHVCF_TOP | CHVCF_S);

			if (!found && info.AdjLW)
				found = ValidatePlaceStructure(foundationItem, foundationItem->GetMultiAtXY(item->X + 1, item->Y), minZ, maxZ, CHVCF_TOP | CHVCF_W);

			if (!found)
				return false;
		}
	}

	return true;
}
//----------------------------------------------------------------------------------
bool CGumpCustomHouse::CanEraseHere(CRenderWorldObject *place, CUSTOM_HOUSE_BUILD_TYPE &type)
{
	type = CHBT_NORMAL;

	if (place != NULL && place->IsMultiObject())
	{
		CMultiObject *multi = (CMultiObject*)place;

		if (multi->IsCustomHouseMulti() && !(multi->State & CHMOF_GENERIC_INTERNAL))
		{
			if (multi->State & CHMOF_FLOOR)
				type = CHBT_FLOOR;
			else if (multi->State & CHMOF_STAIR)
				type = CHBT_STAIR;
			else if (multi->State & CHMOF_ROOF)
				type = CHBT_ROOF;

			return true;
		}
	}

	return false;
}
//----------------------------------------------------------------------------------
void CGumpCustomHouse::OnTargetWorld(CRenderWorldObject *place)
{
	if (place != NULL && place->IsMultiObject()) // && place->Z >= m_MinHouseZ)
	{
		CMultiObject *multiObject = (CMultiObject*)place;

		int zOffset = 0;

		if (m_CurrentFloor == 1)
			zOffset = -7;

		if (m_SeekTile)
			SeekGraphic(place->Graphic);
		else if (place->Z >= g_Player->Z + zOffset && place->Z < g_Player->Z + 20)
		{
			CGameItem *foundationItem = g_World->GetWorldItem(m_Serial);

			if (foundationItem == NULL)
				return;

			if (m_Erasing)
			{
				CUSTOM_HOUSE_BUILD_TYPE type;

				if (CanEraseHere(place, type))
				{
					CMulti *multi = foundationItem->GetMultiAtXY(place->X, place->Y);

					if (multi == NULL)
						return;

					int z = 7 + (m_CurrentFloor - 1) * 20;

					if (type == CHBT_STAIR || type == CHBT_ROOF)
						z = place->Z - (foundationItem->Z + z) + z;

					if (type == CHBT_ROOF)
						CPacketCustomHouseDeleteRoof(place->Graphic, place->X - foundationItem->X, place->Y - foundationItem->Y, z).Send();
					else
						CPacketCustomHouseDeleteItem(place->Graphic, place->X - foundationItem->X, place->Y - foundationItem->Y, z).Send();

					multi->Delete(place);
				}
			}
			else if (m_SelectedGraphic)
			{
				vector<CBuildObject> list;
				CUSTOM_HOUSE_BUILD_TYPE type;

				if (CanBuildHere(list, place, type) && list.size())
				{
					int placeX = place->X;
					int placeY = place->Y;

					if (type == CHBT_STAIR && m_CombinedStair)
					{
						if (m_Page >= 0 && m_Page < (int)m_Stairs.size())
						{
							const CCustomHouseObjectStair &stair = m_Stairs[m_Page];
							ushort graphic = 0;

							if (m_SelectedGraphic == stair.North)
								graphic = stair.MultiNorth;
							else if (m_SelectedGraphic == stair.East)
								graphic = stair.MultiEast;
							else if (m_SelectedGraphic == stair.South)
								graphic = stair.MultiSouth;
							else if (m_SelectedGraphic == stair.West)
								graphic = stair.MultiWest;

							if (graphic)
								CPacketCustomHouseAddStair(graphic, placeX - foundationItem->X, placeY - foundationItem->Y).Send();
						}
					}
					else
					{
						const CBuildObject &item = list[0];
						int x = placeX - foundationItem->X + item.X;
						int y = placeY - foundationItem->Y + item.Y;

						CMulti *multi = foundationItem->GetMultiAtXY(placeX + item.X, placeY + item.Y);

						if (multi != NULL)
						{
							if (!m_CombinedStair)
							{
								int minZ = foundationItem->Z + 7 + (m_CurrentFloor - 1) * 20;
								int maxZ = minZ + 20;

								if (m_CurrentFloor == 1)
									minZ -= 7;

								CMultiObject *nextMultiObject = NULL;

								for (CMultiObject *multiObject = (CMultiObject*)multi->m_Items; multiObject != NULL; multiObject = nextMultiObject)
								{
									nextMultiObject = (CMultiObject*)multiObject->m_Next;

									int testMinZ = minZ;

									if (multiObject->State & CHMOF_ROOF)
										testMinZ -= 3;

									if (multiObject->Z < testMinZ || multiObject->Z >= maxZ || !multiObject->IsCustomHouseMulti() || (multiObject->State & CHMOF_GENERIC_INTERNAL))
										continue;

									if (type == CHBT_STAIR)
									{
										if (multiObject->State & CHMOF_STAIR)
											multi->Delete(multiObject);
									}
									else if (type == CHBT_ROOF)
									{
										if (multiObject->State & CHMOF_ROOF)
											multi->Delete(multiObject);
									}
									else if (type == CHBT_FLOOR)
									{
										if (multiObject->State & CHMOF_FLOOR)
											multi->Delete(multiObject);
									}
									else
									{
										if (!(multiObject->State & (CHMOF_STAIR | CHMOF_ROOF | CHMOF_FLOOR)))
											multi->Delete(multiObject);
									}
								}

								if (multi->m_Items == NULL)
									foundationItem->Delete(multi);
							}

							if (type == CHBT_ROOF)
								CPacketCustomHouseAddRoof(item.Graphic, x, y, item.Z).Send();
							else
								CPacketCustomHouseAddItem(item.Graphic, x, y).Send();

						}
					}

					int x = placeX - foundationItem->X;
					int y = placeY - foundationItem->Y;
					int z = foundationItem->Z + 7 + (m_CurrentFloor - 1) * 20;

					if (type == CHBT_STAIR && !m_CombinedStair)
						z = foundationItem->Z;

					for (const CBuildObject &item : list)
						foundationItem->AddMulti(item.Graphic, 0, x + item.X, y + item.Y, z + item.Z, true);
				}
			}

			GenerateFloorPlace();
			m_WantUpdateContent = true;
		}
	}
}
//----------------------------------------------------------------------------------
void CGumpCustomHouse::GenerateFloorPlace()
{
	CGameItem *foundationItem = g_World->GetWorldItem(m_Serial);

	if (foundationItem != NULL)
	{
		foundationItem->ClearCustomHouseMultis(CHMOF_GENERIC_INTERNAL);

		QFOR(multi, foundationItem->m_Items, CMulti*)
		{
			QFOR(item, multi->m_Items, CMultiObject*)
			{
				if (!item->IsCustomHouseMulti())
					continue;

				int currentFloor = -1;
				int floorZ = foundationItem->Z + 7;
				int itemZ = item->Z;

				IFOR(i, 0, 4)
				{
					int offset = ((int)i ? 0 : 7);

					if (itemZ >= floorZ - offset && itemZ < floorZ + 20)
					{
						currentFloor = (int)i;
						break;
					}

					floorZ += 20;
				}

				if (currentFloor == -1)
					continue;

				pair<int, int> floorCheck = SeekGraphicInCustomHouseObjectList<CCustomHouseObjectFloor>(m_Floors, item->Graphic);
				int state = item->State;

				if (floorCheck.first != -1 && floorCheck.second != -1)
				{
					state |= CHMOF_FLOOR;

					if (m_FloorVisionState[currentFloor] == CHGVS_HIDE_FLOOR)
						state |= CHMOF_IGNORE_IN_RENDER;
					else if (m_FloorVisionState[currentFloor] == CHGVS_TRANSPARENT_FLOOR || m_FloorVisionState[currentFloor] == CHGVS_TRANSLUCENT_FLOOR)
						state |= CHMOF_TRANSPARENT;
				}
				else
				{
					pair<int, int> stairCheck = SeekGraphicInCustomHouseObjectList<CCustomHouseObjectStair>(m_Stairs, item->Graphic);

					if (stairCheck.first != -1 && stairCheck.second != -1)
						state |= CHMOF_STAIR;
					else
					{
						pair<int, int> roofCheck = SeekGraphicInCustomHouseObjectListWithCategory<CCustomHouseObjectRoof, CCustomHouseObjectRoofCategory>(m_Roofs, item->Graphic);

						if (roofCheck.first != -1 && roofCheck.second != -1)
							state |= CHMOF_ROOF;
						else
						{
							pair<int, int> fixtureCheck = SeekGraphicInCustomHouseObjectList<CCustomHouseObjectDoor>(m_Doors, item->Graphic);

							if (fixtureCheck.first == -1 || fixtureCheck.second == -1)
								fixtureCheck = SeekGraphicInCustomHouseObjectList<CCustomHouseObjectTeleport>(m_Teleports, item->Graphic);

							if (fixtureCheck.first != -1 && fixtureCheck.second != -1)
								state |= CHMOF_FIXTURE;
						}
					}

					if (m_FloorVisionState[currentFloor] == CHGVS_HIDE_CONTENT)
						state |= CHMOF_IGNORE_IN_RENDER;
					else if (m_FloorVisionState[currentFloor] == CHGVS_TRANSPARENT_CONTENT)
						state |= CHMOF_TRANSPARENT;
				}

				if (m_FloorVisionState[currentFloor] == CHGVS_HIDE_ALL)
					state |= CHMOF_IGNORE_IN_RENDER;

				item->State = state;
			}
		}

		int z = foundationItem->Z + 7;

		IFOR(x, m_StartPos.X + 1, m_EndPos.X)
		{
			IFOR(y, m_StartPos.Y + 1, m_EndPos.Y)
			{
				CMulti *multi = foundationItem->GetMultiAtXY((int)x, (int)y);

				if (multi == NULL)
					continue;

				CMultiObject *floorMulti = NULL;
				CMultiObject *floorCustomMulti = NULL;

				QFOR(item, multi->m_Items, CMultiObject*)
				{
					if (item->Z != z || !(item->State & CHMOF_FLOOR))
						continue;

					if (item->IsCustomHouseMulti())
						floorCustomMulti = item;
					else
						floorMulti = item;
				}

				if (floorMulti != NULL && floorCustomMulti == NULL)
				{
					CMultiObject *mo = foundationItem->AddMulti(floorMulti->Graphic, 0, (int)x - foundationItem->X, (int)y - foundationItem->Y, z, true);

					int state = CHMOF_FLOOR;

					if (m_FloorVisionState[0] == CHGVS_HIDE_FLOOR)
						state |= CHMOF_IGNORE_IN_RENDER;
					else if (m_FloorVisionState[0] == CHGVS_TRANSPARENT_FLOOR || m_FloorVisionState[0] == CHGVS_TRANSLUCENT_FLOOR)
						state |= CHMOF_TRANSPARENT;

					mo->State = state;
				}
			}
		}

		IFOR(i, 0, m_FloorCount)
		{
			int minZ = foundationItem->Z + 7 + ((int)i * 20);
			int maxZ = minZ + 20;

			IFOR(j, 0, 2)
			{
				vector<WISP_GEOMETRY::CPoint2Di> validatedFloors;

				IFOR(x, m_StartPos.X, m_EndPos.X + 1)
				{
					IFOR(y, m_StartPos.Y, m_EndPos.Y + 1)
					{
						CMulti *multi = foundationItem->GetMultiAtXY((int)x, (int)y);

						if (multi == NULL)
							continue;

						QFOR(item, multi->m_Items, CMultiObject*)
						{
							if (!item->IsCustomHouseMulti())
								continue;

							if (!j)
							{
								if (!i && item->Z < minZ)
								{
									item->State = item->State | CHMOF_VALIDATED_PLACE;
									continue;
								}

								if (!(item->State & CHMOF_FLOOR))
									continue;

								if (!i && item->Z >= minZ && item->Z < maxZ)
								{
									item->State = item->State | CHMOF_VALIDATED_PLACE;
									continue;
								}
							}
							
							if (!(item->State & (CHMOF_VALIDATED_PLACE | CHMOF_GENERIC_INTERNAL)) && item->Z >= minZ && item->Z < maxZ)
							{
								if (!ValidateItemPlace(foundationItem, item, minZ, maxZ, validatedFloors))
									item->State = item->State | CHMOF_VALIDATED_PLACE | CHMOF_INCORRECT_PLACE;
								else
									item->State = item->State | CHMOF_VALIDATED_PLACE;
							}
						}
					}
				}

				if (i && !j)
				{
					for (const WISP_GEOMETRY::CPoint2Di &point : validatedFloors)
					{
						CMulti *multi = foundationItem->GetMultiAtXY(point.X, point.Y);

						if (multi == NULL)
							continue;

						QFOR(item, multi->m_Items, CMultiObject*)
						{
							if (item->IsCustomHouseMulti() && (item->State & CHMOF_FLOOR) && item->Z >= minZ && item->Z < maxZ)
								item->State = item->State & ~CHMOF_INCORRECT_PLACE;
						}
					}
					IFOR(x, m_StartPos.X, m_EndPos.X + 1)
					{
						int minY = 0;
						int maxY = 0;

						IFOR(y, m_StartPos.Y, m_EndPos.Y + 1)
						{
							CMulti *multi = foundationItem->GetMultiAtXY((int)x, (int)y);

							if (multi == NULL)
								continue;

							QFOR(item, multi->m_Items, CMultiObject*)
							{
								if (item->IsCustomHouseMulti() && (item->State & CHMOF_FLOOR) && (item->State & CHMOF_VALIDATED_PLACE) && !(item->State & CHMOF_INCORRECT_PLACE) && item->Z >= minZ && item->Z < maxZ)
								{
									minY = (int)y;
									break;
								}
							}

							if (minY)
								break;
						}

						DFOR(y, m_EndPos.Y, m_StartPos.Y)
						{
							CMulti *multi = foundationItem->GetMultiAtXY((int)x, (int)y);

							if (multi == NULL)
								continue;

							QFOR(item, multi->m_Items, CMultiObject*)
							{
								if (item->IsCustomHouseMulti() && (item->State & CHMOF_FLOOR) && (item->State & CHMOF_VALIDATED_PLACE) && !(item->State & CHMOF_INCORRECT_PLACE) && item->Z >= minZ && item->Z < maxZ)
								{
									maxY = (int)y;
									break;
								}
							}

							if (maxY)
								break;
						}

						IFOR(y, minY, maxY)
						{
							CMulti *multi = foundationItem->GetMultiAtXY((int)x, (int)y);

							if (multi == NULL)
								continue;

							QFOR(item, multi->m_Items, CMultiObject*)
							{
								if (item->IsCustomHouseMulti() && (item->State & CHMOF_FLOOR) && (item->State & CHMOF_VALIDATED_PLACE) && item->Z >= minZ && item->Z < maxZ)
									item->State = item->State & ~CHMOF_INCORRECT_PLACE;
							}
						}
					}

					IFOR(y, m_StartPos.Y, m_EndPos.Y + 1)
					{
						int minX = 0;
						int maxX = 0;

						IFOR(x, m_StartPos.X, m_EndPos.X + 1)
						{
							CMulti *multi = foundationItem->GetMultiAtXY((int)x, (int)y);

							if (multi == NULL)
								continue;

							QFOR(item, multi->m_Items, CMultiObject*)
							{
								if (item->IsCustomHouseMulti() && (item->State & CHMOF_FLOOR) && (item->State & CHMOF_VALIDATED_PLACE) && !(item->State & CHMOF_INCORRECT_PLACE) && item->Z >= minZ && item->Z < maxZ)
								{
									minX = (int)x;
									break;
								}
							}

							if (minX)
								break;
						}

						DFOR(x, m_EndPos.X, m_StartPos.X)
						{
							CMulti *multi = foundationItem->GetMultiAtXY((int)x, (int)y);

							if (multi == NULL)
								continue;

							QFOR(item, multi->m_Items, CMultiObject*)
							{
								if (item->IsCustomHouseMulti() && (item->State & CHMOF_FLOOR) && (item->State & CHMOF_VALIDATED_PLACE) && !(item->State & CHMOF_INCORRECT_PLACE) && item->Z >= minZ && item->Z < maxZ)
								{
									maxX = (int)x;
									break;
								}
							}

							if (maxX)
								break;
						}

						IFOR(x, minX, maxX)
						{
							CMulti *multi = foundationItem->GetMultiAtXY((int)x, (int)y);

							if (multi == NULL)
								continue;

							QFOR(item, multi->m_Items, CMultiObject*)
							{
								if (item->IsCustomHouseMulti() && (item->State & CHMOF_FLOOR) && (item->State & CHMOF_VALIDATED_PLACE) && item->Z >= minZ && item->Z < maxZ)
									item->State = item->State & ~CHMOF_INCORRECT_PLACE;
							}
						}
					}
				}
			}
		}

		z = foundationItem->Z + 7 + 20;

		ushort color = 0x0051;

		IFOR(i, 1, m_CurrentFloor)
		{
			IFOR(x, m_StartPos.X, m_EndPos.X)
			{
				IFOR(y, m_StartPos.Y, m_EndPos.Y)
				{
					ushort tempColor = color;

					if (x == m_StartPos.X || y == m_StartPos.Y)
						tempColor++;

					CMultiObject *mo = foundationItem->AddMulti(0x0496, tempColor, (int)x - foundationItem->X, (int)y - foundationItem->Y, z, true);
					mo->State = CHMOF_GENERIC_INTERNAL | CHMOF_TRANSPARENT;
					g_MapManager.AddRender(mo);
				}
			}

			color += 5;
			z += 20;
		}
	}
}
//----------------------------------------------------------------------------------
void CGumpCustomHouse::GUMP_TEXT_ENTRY_EVENT_C
{
	WISPFUN_DEBUG("c101_f5");
	QFOR(item, m_DataBox->m_Items, CBaseGUI*)
	{
		if (item->Type == GOT_TEXTENTRY)
		{
			CGUITextEntry *entry = (CGUITextEntry*)item;
			entry->Focused = (entry->Serial == serial);
		}
	}

	OnButton(serial);
}
//----------------------------------------------------------------------------------
void CGumpCustomHouse::OnLeftMouseButtonUp()
{
	WISPFUN_DEBUG("c101_f6");
	CGump::OnLeftMouseButtonUp();

	QFOR(item, m_DataBox->m_Items, CBaseGUI*)
	{
		if (item->Type == GOT_TEXTENTRY)
			((CGUITextEntry*)item)->Focused = false;
	}

	m_WantRedraw = true;
}
//----------------------------------------------------------------------------------
void CGumpCustomHouse::GUMP_BUTTON_EVENT_C
{
	WISPFUN_DEBUG("");

	if (serial >= ID_GCH_ITEM_IN_LIST)
	{
		int index = serial - ID_GCH_ITEM_IN_LIST;

		if (m_Category == -1 && (m_State == CHGS_WALL || m_State == CHGS_ROOF || m_State == CHGS_MISC))
		{
			int newCategory = -1;

			if (m_State == CHGS_WALL && index >= 0 && index < (int)m_Walls.size())
				newCategory = m_Walls[index].Index;
			else if (m_State == CHGS_ROOF && index >= 0 && index < (int)m_Roofs.size())
				newCategory = m_Roofs[index].Index;
			else if (m_State == CHGS_MISC && index >= 0 && index < (int)m_Miscs.size())
				newCategory = m_Miscs[index].Index;

			if (newCategory != -1)
			{
				m_Category = newCategory;
				m_WantUpdateContent = true;
				m_Page = 0;
				m_SelectedGraphic = 0;
				m_Erasing = false;
				m_SeekTile = false;
				m_CombinedStair = false;
				UpdateMaxPage();
			}
		}
		else if (index >= 0 && m_Page >= 0)
		{
			bool combinedStair = false;
			ushort graphic = 0;

			if (m_State == CHGS_WALL || m_State == CHGS_ROOF || m_State == CHGS_MISC)
			{
				if (m_Category >= 0)
				{
					if (m_State == CHGS_WALL && m_Category < (int)m_Walls.size() && index < CCustomHouseObjectWall::GRAPHICS_COUNT)
					{
						const vector<CCustomHouseObjectWall> &list = m_Walls[m_Category].m_Items;

						if (m_Page < (int)list.size())
							graphic = (m_ShowWindow ? list[m_Page].m_WindowGraphics[index] : list[m_Page].m_Graphics[index]);
					}
					else if (m_State == CHGS_ROOF && m_Category < (int)m_Roofs.size() && index < CCustomHouseObjectRoof::GRAPHICS_COUNT)
					{
						const vector<CCustomHouseObjectRoof> &list = m_Roofs[m_Category].m_Items;

						if (m_Page < (int)list.size())
							graphic = list[m_Page].m_Graphics[index];
					}
					else if (m_State == CHGS_MISC && m_Category < (int)m_Miscs.size() && index < CCustomHouseObjectMisc::GRAPHICS_COUNT)
					{
						const vector<CCustomHouseObjectMisc> &list = m_Miscs[m_Category].m_Items;

						if (m_Page < (int)list.size())
							graphic = list[m_Page].m_Graphics[index];
					}
				}
			}
			else
			{
				if (m_State == CHGS_DOOR && m_Page < (int)m_Doors.size() && index < CCustomHouseObjectDoor::GRAPHICS_COUNT)
					graphic = m_Doors[m_Page].m_Graphics[index];
				else if (m_State == CHGS_FLOOR && m_Page < (int)m_Floors.size() && index < CCustomHouseObjectFloor::GRAPHICS_COUNT)
					graphic = m_Floors[m_Page].m_Graphics[index];
				else if (m_State == CHGS_STAIR && m_Page < (int)m_Stairs.size())
				{
					if (index > 10)
					{
						combinedStair = true;
						index -= 10;
					}
					
					if (index < CCustomHouseObjectStair::GRAPHICS_COUNT)
						graphic = m_Stairs[m_Page].m_Graphics[index];
				}
			}

			if (graphic)
			{
				g_Target.RequestFromCustomHouse();
				m_CombinedStair = combinedStair;
				m_WantUpdateContent = true;
				m_SelectedGraphic = graphic;
			}
		}

		return;
	}

	switch (serial)
	{
		case ID_GCH_STATE_WALL:
		{
			m_Category = -1;
			m_State = CHGS_WALL;
			m_WantUpdateContent = true;
			m_Page = 0;
			m_SelectedGraphic = 0;
			m_CombinedStair = false;
			UpdateMaxPage();
			g_Target.SendCancelTarget();
			break;
		}
		case ID_GCH_STATE_DOOR:
		{
			m_Category = -1;
			m_State = CHGS_DOOR;
			m_WantUpdateContent = true;
			m_Page = 0;
			m_SelectedGraphic = 0;
			m_CombinedStair = false;
			UpdateMaxPage();
			g_Target.SendCancelTarget();
			break;
		}
		case ID_GCH_STATE_FLOOR:
		{
			m_Category = -1;
			m_State = CHGS_FLOOR;
			m_WantUpdateContent = true;
			m_Page = 0;
			m_SelectedGraphic = 0;
			m_CombinedStair = false;
			UpdateMaxPage();
			g_Target.SendCancelTarget();
			break;
		}
		case ID_GCH_STATE_STAIR:
		{
			m_Category = -1;
			m_State = CHGS_STAIR;
			m_WantUpdateContent = true;
			m_Page = 0;
			m_SelectedGraphic = 0;
			m_CombinedStair = false;
			UpdateMaxPage();
			g_Target.SendCancelTarget();
			break;
		}
		case ID_GCH_STATE_ROOF:
		{
			m_Category = -1;
			m_State = CHGS_ROOF;
			m_WantUpdateContent = true;
			m_Page = 0;
			m_SelectedGraphic = 0;
			m_CombinedStair = false;
			UpdateMaxPage();
			g_Target.SendCancelTarget();
			break;
		}
		case ID_GCH_STATE_MISC:
		{
			m_Category = -1;
			m_State = CHGS_MISC;
			m_WantUpdateContent = true;
			m_Page = 0;
			m_SelectedGraphic = 0;
			m_CombinedStair = false;
			UpdateMaxPage();
			g_Target.SendCancelTarget();
			break;
		}
		case ID_GCH_STATE_ERASE:
		{
			g_Target.RequestFromCustomHouse();
			m_Erasing = !m_Erasing;
			m_WantUpdateContent = true;
			m_SelectedGraphic = 0;
			m_CombinedStair = false;
			break;
		}
		case ID_GCH_STATE_EYEDROPPER:
		{
			g_Target.RequestFromCustomHouse();
			m_SeekTile = true;
			m_WantUpdateContent = true;
			m_SelectedGraphic = 0;
			m_CombinedStair = false;
			break;
		}
		case ID_GCH_STATE_MENU:
		{
			m_Category = -1;
			m_State = CHGS_MENU;
			m_WantUpdateContent = true;
			m_MaxPage = 1;
			m_Page = 0;
			m_SelectedGraphic = 0;
			m_CombinedStair = false;
			g_Target.SendCancelTarget();
			break;
		}
		case ID_GCH_VISIBILITY_STORY_1:
		case ID_GCH_VISIBILITY_STORY_2:
		case ID_GCH_VISIBILITY_STORY_3:
		case ID_GCH_VISIBILITY_STORY_4:
		{
			int selectedFloor = serial - ID_GCH_VISIBILITY_STORY_1;

			m_FloorVisionState[selectedFloor]++;

			if (m_FloorVisionState[selectedFloor] > CHGVS_HIDE_ALL)
				m_FloorVisionState[selectedFloor] = CHGVS_NORMAL;

			m_WantUpdateContent = true;
			GenerateFloorPlace();

			break;
		}
		case ID_GCH_GO_FLOOR_1:
		{
			m_CurrentFloor = 1;
			CPacketCustomHouseGoToFloor(1).Send();
			m_WantUpdateContent = true;
			m_FloorVisionState[0] = m_FloorVisionState[1] = m_FloorVisionState[2] = m_FloorVisionState[3] = CHGVS_NORMAL;
			break;
		}
		case ID_GCH_GO_FLOOR_2:
		{
			m_CurrentFloor = 2;
			CPacketCustomHouseGoToFloor(2).Send();
			m_WantUpdateContent = true;
			m_FloorVisionState[0] = m_FloorVisionState[1] = m_FloorVisionState[2] = m_FloorVisionState[3] = CHGVS_NORMAL;
			break;
		}
		case ID_GCH_GO_FLOOR_3:
		{
			m_CurrentFloor = 3;
			CPacketCustomHouseGoToFloor(3).Send();
			m_WantUpdateContent = true;
			m_FloorVisionState[0] = m_FloorVisionState[1] = m_FloorVisionState[2] = m_FloorVisionState[3] = CHGVS_NORMAL;
			break;
		}
		case ID_GCH_GO_FLOOR_4:
		{
			m_CurrentFloor = 4;
			CPacketCustomHouseGoToFloor(4).Send();
			m_WantUpdateContent = true;
			m_FloorVisionState[0] = m_FloorVisionState[1] = m_FloorVisionState[2] = m_FloorVisionState[3] = CHGVS_NORMAL;
			break;
		}
		case ID_GCH_LIST_LEFT:
		{
			m_Page--;

			if (m_Page < 0)
			{
				m_Page = m_MaxPage - 1;

				if (m_Page < 0)
					m_Page = 0;
			}

			m_WantUpdateContent = true;

			break;
		}
		case ID_GCH_LIST_RIGHT:
		{
			m_Page++;

			if (m_Page >= m_MaxPage)
				m_Page = 0;

			m_WantUpdateContent = true;

			break;
		}
		case ID_GCH_MENU_BACKUP:
		{
			CPacketCustomHouseBackup().Send();
			break;
		}
		case ID_GCH_MENU_RESTORE:
		{
			CPacketCustomHouseRestore().Send();
			break;
		}
		case ID_GCH_MENU_SYNCH:
		{
			CPacketCustomHouseSync().Send();
			break;
		}
		case ID_GCH_MENU_CLEAR:
		{
			CPacketCustomHouseClear().Send();
			break;
		}
		case ID_GCH_MENU_COMMIT:
		{
			CPacketCustomHouseCommit().Send();
			break;
		}
		case ID_GCH_MENU_REVERT:
		{
			CPacketCustomHouseRevert().Send();
			break;
		}
		case ID_GCH_GO_CATEGORY:
		{
			m_Category = -1;
			m_WantUpdateContent = true;
			m_Page = 0;
			m_SelectedGraphic = 0;
			m_CombinedStair = false;
			UpdateMaxPage();
			g_Target.SendCancelTarget();
			break;
		}
		case ID_GCH_WALL_SHOW_WINDOW:
		{
			m_ShowWindow = !m_ShowWindow;
			m_WantUpdateContent = true;
			break;
		}
		case ID_GCH_ROOF_Z_UP:
		{
			if (m_RoofZ < 6)
			{
				m_RoofZ++;
				m_WantUpdateContent = true;
			}
			break;
		}
		case ID_GCH_ROOF_Z_DOWN:
		{
			if (m_RoofZ > 1)
			{
				m_RoofZ--;
				m_WantUpdateContent = true;
			}

			break;
		}
		default:
			break;
	}
}
//----------------------------------------------------------------------------------
