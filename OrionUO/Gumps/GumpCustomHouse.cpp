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

	g_CustomHouseGump = this;

	ParseCustomHouseObjectFileWithCategory<CCustomHouseObjectWall, CCustomHouseObjectWallCategory>(m_Walls, g_App.FilePath("walls.txt"));
	ParseCustomHouseObjectFile<CCustomHouseObjectFloor>(m_Floors, g_App.FilePath("floors.txt"));
	ParseCustomHouseObjectFile<CCustomHouseObjectDoor>(m_Doors, g_App.FilePath("doors.txt"));
	ParseCustomHouseObjectFileWithCategory<CCustomHouseObjectMisc, CCustomHouseObjectMiscCategory>(m_Miscs, g_App.FilePath("misc.txt"));
	ParseCustomHouseObjectFile<CCustomHouseObjectStair>(m_Stairs, g_App.FilePath("stairs.txt"));
	ParseCustomHouseObjectFile<CCustomHouseObjectTeleport>(m_Teleports, g_App.FilePath("teleprts.txt"));
	ParseCustomHouseObjectFileWithCategory<CCustomHouseObjectRoof, CCustomHouseObjectRoofCategory>(m_Roofs, g_App.FilePath("roof.txt"));
	ParseCustomHouseObjectFile<CCustomHouseObjectPlaceInfo>(m_ObjectsInfo, g_App.FilePath("suppinfo.txt"));

	CGameItem *foundationItem = g_World->GetWorldItem(serial);

	if (foundationItem != NULL)
	{
		CMulti* multi = foundationItem->GetMulti();

		if (multi != NULL)
		{
			m_StartPos.X = foundationItem->X + multi->MinX;
			m_StartPos.Y = foundationItem->Y + multi->MinY;
			m_EndPos.X = foundationItem->X + multi->MaxX + 1;
			m_EndPos.Y = foundationItem->Y + multi->MaxY + 1;
		}
	}

	LOG("CH multi Bounds: %i %i %i %i\n", m_StartPos.X, m_StartPos.Y, m_EndPos.X, m_EndPos.Y);



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



	m_TextItems = (CGUIText*)Add(new CGUIText(0x0481, 33, 142));
	m_TextCost = (CGUIText*)Add(new CGUIText(0x0481, 524, 142));

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
	m_TextItems = NULL;
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

			m_DataBox->Add(new CGUIHitBox(ID_GCH_ITEM_IN_LIST + i, offsetX, offsetY, dims.Width, dims.Height, true));

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

			if (m_ShowWindow)
			{
			}

			m_DataBox->Add(new CGUIScissor(true, 0, 0, 130, 36, 384, 120));

			IFOR(i, 0, 8)
			{
				const ushort &graphic = item.m_Graphics[i];

				if (graphic)
				{
					WISP_GEOMETRY::CSize dims = g_Orion.GetArtDimension(graphic, true);

					int offsetX = x + 130 + (48 - dims.Width) / 2;
					int offsetY = y + 36 + (120 - dims.Height) / 2;

					m_DataBox->Add(new CGUITilepic(graphic, 0, offsetX, offsetY));

					m_DataBox->Add(new CGUIHitBox(ID_GCH_ITEM_IN_LIST + i, offsetX, offsetY, dims.Width, dims.Height, true));
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

				m_DataBox->Add(new CGUIHitBox(ID_GCH_ITEM_IN_LIST + i, offsetX, offsetY, dims.Width, dims.Height, true));
			}

			x += 48;
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

			m_DataBox->Add(new CGUIHitBox(ID_GCH_ITEM_IN_LIST + i, offsetX, offsetY, dims.Width, dims.Height, true));

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

		m_DataBoxGUI->Add(new CGUIButton(ID_GCH_ROOF_Z_UP, 0x578B, 0x578C, 0x578D, 305, 0));
		m_DataBoxGUI->Add(new CGUIButton(ID_GCH_ROOF_Z_DOWN, 0x578E, 0x578F, 0x5790, 349, 0));

		m_DataBoxGUI->Add(new CGUIGumppic(0x55F4, 383, 4));
		CGUIText *text = (CGUIText*)m_DataBoxGUI->Add(new CGUIText(0x04E9, 405, 15));
		text->CreateTextureA(3, std::to_string(m_RoofZ));
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

			m_DataBox->Add(new CGUIHitBox(ID_GCH_ITEM_IN_LIST + i, offsetX, offsetY, dims.Width, dims.Height, true));

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

					m_DataBox->Add(new CGUIHitBox(ID_GCH_ITEM_IN_LIST + i, offsetX, offsetY, dims.Width, dims.Height, true));
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
			const T &item = cat.m_Items[j];

			IFOR(g, 0, T::GRAPHICS_COUNT)
			{
				if (item.m_Graphics[g] == graphic)
					return pair<int, int>(i, j);
			}
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
		const T &item = list[i];

		IFOR(j, 0, T::GRAPHICS_COUNT)
		{
			if (item.m_Graphics[j] == graphic)
				return pair<int, int>(i, i);
		}
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

	int graphicOffset = (m_CurrentFloor == 1 ? 3 : 0);
	int graphicOffset2 = (m_CurrentFloor == 1 ? 4 : 0);
	m_DataBoxGUI->Add(new CGUIButton(ID_GCH_VISIBILITY_STORY_1, 0x572E, 0x572F, 0x5730, 533, 108));
	m_DataBoxGUI->Add(new CGUIButton(ID_GCH_GO_FLOOR_1, 0x56CD + graphicOffset2, 0x56CD + graphicOffset2, 0x56D1, 583, 96));
	m_DataBoxGUI->Add(new CGUIButton(ID_GCH_GO_FLOOR_1, 0x56F6 + graphicOffset, 0x56F7 + graphicOffset, 0x56F8 + graphicOffset, 623, 103));

	graphicOffset = (m_CurrentFloor == 2 ? 3 : 0);
	graphicOffset2 = (m_CurrentFloor == 2 ? 4 : 0);
	m_DataBoxGUI->Add(new CGUIButton(ID_GCH_VISIBILITY_STORY_2, 0x5725, 0x5726, 0x5727, 533, 86));
	m_DataBoxGUI->Add(new CGUIButton(ID_GCH_GO_FLOOR_2, 0x56CE + graphicOffset2, 0x56CE + graphicOffset2, 0x56D2, 583, 73));
	m_DataBoxGUI->Add(new CGUIButton(ID_GCH_GO_FLOOR_2, 0x56F0 + graphicOffset, 0x56F1 + graphicOffset, 0x56F2 + graphicOffset, 623, 86));

	graphicOffset = (m_CurrentFloor == 3 ? 3 : 0);
	graphicOffset2 = (m_CurrentFloor == 3 ? 4 : 0);

	if (m_FloorCount == 4)
	{
		m_DataBoxGUI->Add(new CGUIButton(ID_GCH_VISIBILITY_STORY_3, 0x5725, 0x5726, 0x5727, 533, 64));
		m_DataBoxGUI->Add(new CGUIButton(ID_GCH_GO_FLOOR_3, 0x56CE + graphicOffset2, 0x56CE + graphicOffset2, 0x56D2, 582, 56));
		m_DataBoxGUI->Add(new CGUIButton(ID_GCH_GO_FLOOR_3, 0x56F0 + graphicOffset, 0x56F1 + graphicOffset, 0x56F2 + graphicOffset, 623, 69));

		graphicOffset = (m_CurrentFloor == 4 ? 3 : 0);
		graphicOffset2 = (m_CurrentFloor == 4 ? 4 : 0);
		m_DataBoxGUI->Add(new CGUIButton(ID_GCH_VISIBILITY_STORY_4, 0x571C, 0x571D, 0x571E, 533, 42));
		m_DataBoxGUI->Add(new CGUIButton(ID_GCH_GO_FLOOR_4, 0x56D0 + graphicOffset2, 0x56D0 + graphicOffset2, 0x56D4, 583, 42));
		m_DataBoxGUI->Add(new CGUIButton(ID_GCH_GO_FLOOR_4, 0x56EA + graphicOffset, 0x56EB + graphicOffset, 0x56EC + graphicOffset, 623, 50));
	}
	else
	{
		m_DataBoxGUI->Add(new CGUIButton(ID_GCH_VISIBILITY_STORY_3, 0x571C, 0x571D, 0x571E, 533, 64));
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

	int componentsCount = 0;
	int fixturesCount = 0;

	CGameItem *foundationItem = g_World->GetWorldItem(m_Serial);

	if (foundationItem != NULL)
	{
		QFOR(multi, foundationItem->m_Items, CMulti*)
		{
			QFOR(item, multi->m_Items, CMultiObject*)
			{
				if (item->IsCustomHouseMulti())
				{
					if (!(((CCustomHouseMultiObject*)item)->State & CHMOF_GENERIC_INTERNAL))
					{
						CUSTOM_HOUSE_GUMP_STATE state;
						pair<int, int> result = ExistsInList(state, item->Graphic);

						if (result.first != -1 && result.second != -1)
						{
							if (state == CHGS_DOOR)
								fixturesCount++;
							else
								componentsCount++;
						}
					}
				}
			}
		}
	}

	m_TextItems->CreateTextureA(9, std::to_string(componentsCount) + " : " + std::to_string(fixturesCount), 100, TS_CENTER);
	m_TextCost->CreateTextureA(9, std::to_string((componentsCount + fixturesCount) * 500));
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
						m_MaxPage = cat.m_Items.size();
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
						m_MaxPage = cat.m_Items.size();
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
						m_MaxPage = cat.m_Items.size();
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
void CGumpCustomHouse::GenerateFloorPlace()
{
	CGameItem *foundationItem = g_World->GetWorldItem(m_Serial);

	if (foundationItem != NULL)
	{
		foundationItem->ClearCustomHouseMultis(CHMOF_GENERIC_INTERNAL);

		int z = foundationItem->Z + 7 + 20;

		IFOR(i, 1, m_CurrentFloor)
		{
			ushort color = 0;

			if (i == 1)
				color = 0x0051;
			else if (i == 2)
				color = 0x0056;
			else if (i == 3)
				color = 0x005B;

			IFOR(x, m_StartPos.X + 1, m_EndPos.X)
			{
				IFOR(y, m_StartPos.Y + 1, m_EndPos.Y)
				{
					CCustomHouseMultiObject *mo = (CCustomHouseMultiObject*)foundationItem->AddMulti(0x0496, color, x - foundationItem->X, y - foundationItem->Y, z, true);
					mo->State = CHMOF_INTERNAL | CHMOF_GENERIC_INTERNAL | CHMOF_TRANSPARENT;
				}
			}

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
				UpdateMaxPage();
			}
		}
		else if (index >= 0 && m_Page >= 0)
		{
			ushort graphic = 0;

			if (m_State == CHGS_WALL || m_State == CHGS_ROOF || m_State == CHGS_MISC)
			{
				if (m_Category >= 0)
				{
					if (m_State == CHGS_WALL && m_Category < (int)m_Walls.size() && index < CCustomHouseObjectWall::GRAPHICS_COUNT)
					{
						const vector<CCustomHouseObjectWall> &list = m_Walls[m_Category].m_Items;

						if (m_Page < (int)list.size())
							graphic = list[m_Page].m_Graphics[index];
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
			}

			if (graphic)
			{
				g_Target.RequestFromCustomHouse();
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
			break;
		}
		case ID_GCH_STATE_EYEDROPPER:
		{
			g_Target.RequestFromCustomHouse();
			m_SeekTile = true;
			m_WantUpdateContent = true;
			m_SelectedGraphic = 0;
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
			g_Target.SendCancelTarget();
			break;
		}
		case ID_GCH_VISIBILITY_STORY_1:
		{
			break;
		}
		case ID_GCH_VISIBILITY_STORY_2:
		{
			break;
		}
		case ID_GCH_VISIBILITY_STORY_3:
		{
			break;
		}
		case ID_GCH_VISIBILITY_STORY_4:
		{
		}
		case ID_GCH_GO_FLOOR_1:
		{
			m_CurrentFloor = 1;
			CPacketCustomHouseGoToFloor(1).Send();
			m_WantUpdateContent = true;
			break;
		}
		case ID_GCH_GO_FLOOR_2:
		{
			m_CurrentFloor = 2;
			CPacketCustomHouseGoToFloor(2).Send();
			m_WantUpdateContent = true;
			break;
		}
		case ID_GCH_GO_FLOOR_3:
		{
			m_CurrentFloor = 3;
			CPacketCustomHouseGoToFloor(3).Send();
			m_WantUpdateContent = true;
			break;
		}
		case ID_GCH_GO_FLOOR_4:
		{
			m_CurrentFloor = 4;
			CPacketCustomHouseGoToFloor(4).Send();
			m_WantUpdateContent = true;
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
