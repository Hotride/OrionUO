// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
/***********************************************************************************
**
** GumpMinimap.cpp
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#include "stdafx.h"
//----------------------------------------------------------------------------------
CGumpMinimap::CGumpMinimap(short x, short y, bool minimized)
: CGump(GT_MINIMAP, 0, x, y)
{
	WISPFUN_DEBUG("c102_f1");
	m_Minimized = minimized;
	m_Locker.Serial = ID_GMM_LOCK_MOVING;
	GenerateMap();
}
//----------------------------------------------------------------------------------
CGumpMinimap::~CGumpMinimap()
{
	WISPFUN_DEBUG("c102_f2");
	m_Texture.Clear();
}
//----------------------------------------------------------------------------------
void CGumpMinimap::CalculateGumpState()
{
	WISPFUN_DEBUG("c102_f3");
	bool minimized = m_Minimized;
	m_Minimized = false;

	CGump::CalculateGumpState();

	m_Minimized = minimized;
}
//----------------------------------------------------------------------------------
void CGumpMinimap::GenerateMap()
{
	WISPFUN_DEBUG("c102_f4");

	/*const WISP_GEOMETRY::CPoint2Di foliageOffsetTable[17 * 3] =
	{
		WISP_GEOMETRY::CPoint2Di(0, 0),
		WISP_GEOMETRY::CPoint2Di(-2, 1),
		WISP_GEOMETRY::CPoint2Di(-2, -2),
		WISP_GEOMETRY::CPoint2Di(-1, -1),
		WISP_GEOMETRY::CPoint2Di(-1, 0),
		WISP_GEOMETRY::CPoint2Di(-1, 1),
		WISP_GEOMETRY::CPoint2Di(-1, 2),
		WISP_GEOMETRY::CPoint2Di(-1, -1),
		WISP_GEOMETRY::CPoint2Di(0, 1),
		WISP_GEOMETRY::CPoint2Di(0, 2),
		WISP_GEOMETRY::CPoint2Di(0, -2),
		WISP_GEOMETRY::CPoint2Di(1, -1),
		WISP_GEOMETRY::CPoint2Di(1, 0),
		WISP_GEOMETRY::CPoint2Di(1, 1),
		WISP_GEOMETRY::CPoint2Di(1, -1),
		WISP_GEOMETRY::CPoint2Di(2, 0),
		WISP_GEOMETRY::CPoint2Di(2, 0),

		WISP_GEOMETRY::CPoint2Di(0, -1),
		WISP_GEOMETRY::CPoint2Di(-2, 0),
		WISP_GEOMETRY::CPoint2Di(-2, -1),
		WISP_GEOMETRY::CPoint2Di(-1, 0),
		WISP_GEOMETRY::CPoint2Di(-1, 1),
		WISP_GEOMETRY::CPoint2Di(-1, 2),
		WISP_GEOMETRY::CPoint2Di(-1, -2),
		WISP_GEOMETRY::CPoint2Di(0, -1),
		WISP_GEOMETRY::CPoint2Di(0, 1),
		WISP_GEOMETRY::CPoint2Di(0, 2),
		WISP_GEOMETRY::CPoint2Di(0, -2),
		WISP_GEOMETRY::CPoint2Di(1, -1),
		WISP_GEOMETRY::CPoint2Di(1, 0),
		WISP_GEOMETRY::CPoint2Di(1, 1),
		WISP_GEOMETRY::CPoint2Di(1, 0),
		WISP_GEOMETRY::CPoint2Di(2, 1),
		WISP_GEOMETRY::CPoint2Di(2, 0),

		WISP_GEOMETRY::CPoint2Di(0, -1),
		WISP_GEOMETRY::CPoint2Di(-2, 1),
		WISP_GEOMETRY::CPoint2Di(-2, -2),
		WISP_GEOMETRY::CPoint2Di(-1, -1),
		WISP_GEOMETRY::CPoint2Di(-1, 0),
		WISP_GEOMETRY::CPoint2Di(-1, 1),
		WISP_GEOMETRY::CPoint2Di(-1, 2),
		WISP_GEOMETRY::CPoint2Di(-1, -1),
		WISP_GEOMETRY::CPoint2Di(0, 1),
		WISP_GEOMETRY::CPoint2Di(0, -2),
		WISP_GEOMETRY::CPoint2Di(1, -1),
		WISP_GEOMETRY::CPoint2Di(1, 0),
		WISP_GEOMETRY::CPoint2Di(1, 1),
		WISP_GEOMETRY::CPoint2Di(1, 2),
		WISP_GEOMETRY::CPoint2Di(1, -1),
		WISP_GEOMETRY::CPoint2Di(2, 1),
		WISP_GEOMETRY::CPoint2Di(2, 0)
	};*/

	const WISP_GEOMETRY::CPoint2Di originalOffsetTable[2] =
	{
		WISP_GEOMETRY::CPoint2Di(0, 0),
		WISP_GEOMETRY::CPoint2Di(0, 1)
	};

	if (g_Player != NULL)
	{
		m_LastX = g_Player->X;
		m_LastY = g_Player->Y;
	}

	m_Texture.Clear();
	
	ushort gumpID = 0x1393 - (int)m_Minimized;
	CIndexObject &io = g_Orion.m_GumpDataIndex[gumpID];

	int gumpWidth = io.Width;
	int gumpHeight = io.Height;

	USHORT_LIST data = g_UOFileReader.GetGumpPixels(io);
	
	if (!data.size())
		return;

	int blockOffsetX = gumpWidth / 4;
	int blockOffsetY = gumpHeight / 4;

	int gumpCenterX = gumpWidth / 2;
	int gumpCenterY = gumpHeight / 2;

	//0xFF080808 - pixel32
	//0x8421 - pixel16

	int minBlockX = (m_LastX - blockOffsetX) / 8 - 1;
	int minBlockY = (m_LastY - blockOffsetY) / 8 - 1;
	int maxBlockX = ((m_LastX + blockOffsetX) / 8) + 1;
	int maxBlockY = ((m_LastY + blockOffsetY) / 8) + 1;

	if (minBlockX < 0)
		minBlockX = 0;

	if (minBlockY < 0)
		minBlockY = 0;

	int map = g_MapManager.GetActualMap();
	uint maxBlockIndex = g_MapManager.MaxBlockIndex;
	int mapBlockHeight = g_MapBlockSize[map].Height;

	for (int i = minBlockX; i <= maxBlockX; i++)
	{
		uint blockIndexOffset = i * mapBlockHeight;

		for (int j = minBlockY; j <= maxBlockY; j++)
		{
			uint blockIndex = blockIndexOffset + j;

			if (blockIndex >= maxBlockIndex)
				break;

			RADAR_MAP_BLOCK mb = { 0 };
			g_MapManager.GetRadarMapBlock(i, j, mb);

			CMapBlock *mapBlock = g_MapManager.GetBlock(blockIndex);

			int realBlockX = (i * 8);
			int realBlockY = (j * 8);

			IFOR(x, 0, 8)
			{
				int px = ((realBlockX + (int)x) - m_LastX) + gumpCenterX;

				IFOR(y, 0, 8)
				{
					int py = (realBlockY + (int)y) - m_LastY;

					int gx = px - py;
					int gy = px + py;

					uint color = mb.Cells[x][y].Graphic;
					char &isLand = mb.Cells[x][y].IsLand;

					if (mapBlock != NULL)
					{
						ushort multiColor = mapBlock->GetRadarColor((int)x, (int)y);

						if (multiColor)
						{
							color = multiColor;
							isLand = false;
						}
					}

					if (!isLand)
						color = g_Orion.GetSeasonGraphic(color) + 0x4000;
					else
						color = g_Orion.GetLandSeasonGraphic(color);

					int tableSize = 2;
					const WISP_GEOMETRY::CPoint2Di *table = &originalOffsetTable[0];

					/*if (color > 0x4000 && ::IsFoliage(g_Orion.GetStaticFlags(color - 0x4000)))
					{
						tableSize = 17;
						table = &foliageOffsetTable[((color - 0x4000) % 3) * tableSize];
					}*/

					color = 0x8000 | g_ColorManager.GetRadarColorData(color);

					CreatePixels(data, color, gx, gy, gumpWidth, gumpHeight, table, tableSize);
				}
			}
		}
	}

	g_GL_BindTexture16(m_Texture, gumpWidth, gumpHeight, &data[0]);

	m_WantUpdateContent = true;
}
//----------------------------------------------------------------------------------
void CGumpMinimap::CreatePixels(USHORT_LIST &data, const uint &color, const int &x, const int &y, const int &width, const int &height, const WISP_GEOMETRY::CPoint2Di *table, const int &count)
{
	int px = x;
	int py = y;

	IFOR(i, 0, count)
	{
		px += table[i].X;
		py += table[i].Y;
		int gx = px;

		if (gx < 0 || gx >= width)
			continue;

		int gy = py;

		if (gy < 0 || gy >= height)
			break;

		int block = (gy * width) + gx;

		if (data[block] == 0x8421)
			data[block] = color;
	}
}
//----------------------------------------------------------------------------------
void CGumpMinimap::PrepareContent()
{
	WISPFUN_DEBUG("c102_f5");
	if (g_Player->X != m_LastX || g_Player->Y != m_LastY || m_Texture.Texture == 0)
		GenerateMap();
	else if (!m_Count || m_Count == 6 || m_WantRedraw)
		m_WantUpdateContent = true;

	static uint ticks = 0;

	if (ticks < g_Ticks)
	{
		m_Count += 7;
		ticks = g_Ticks + 300;
	}

	if (m_Count > 12)
		m_Count = 0;
}
//----------------------------------------------------------------------------------
void CGumpMinimap::UpdateContent()
{
	WISPFUN_DEBUG("c102_f6");
	ushort graphic = 0x1393 - (int)m_Minimized;

	CGLTexture *th = g_Orion.ExecuteGump(graphic);

	if (th == NULL)
		return;

	if (m_Items == NULL)
	{
		m_DataBox = (CGUIDataBox*)Add(new CGUIDataBox());
		m_Body = (CGUIGumppic*)Add(new CGUIGumppic(graphic, 0, 0));
		m_Body->SelectOnly = true;
	}
	else
		m_DataBox->Clear();

	m_Body->Graphic = graphic;

	int playerX = g_Player->X;
	int playerY = g_Player->Y;

	int gumpWidth = th->Width;
	int gumpHeight = th->Height;

	int gumpCenterX = (gumpWidth / 2) - 1;
	int gumpCenterY = (gumpHeight / 2) - 1;

	m_DataBox->Add(new CGUIExternalTexture(&m_Texture, false, 0, 0));

	if (m_Count < 6)
	{
		QFOR(go, g_World->m_Items, CGameObject*)
		{
			if (go->Container != 0xFFFFFFFF || ((CGameItem*)go)->MultiBody)
				continue; //multi

			if (go->NPC && !go->IsPlayer())
			{
				ushort color = g_ConfigManager.GetColorByNotoriety(go->GameCharacterPtr()->Notoriety);

				if (color)
				{
					uint pcl = g_ColorManager.GetPolygoneColor(16, color);

					int x = go->X - playerX;
					int y = go->Y - playerY;

					int gx = x - y;
					int gy = x + y;

					m_DataBox->Add(new CGUIColoredPolygone(0, 0, gumpCenterX + gx, gumpCenterY + gy, 2, 2, pcl));
				}
			}
		}

		m_DataBox->Add(new CGUIColoredPolygone(0, 0, gumpCenterX, gumpCenterY, 2, 2, 0xFFFFFFFF));
	}
}
//----------------------------------------------------------------------------------
void CGumpMinimap::GUMP_BUTTON_EVENT_C
{
	WISPFUN_DEBUG("c102_f7");
	if (serial == ID_GMM_LOCK_MOVING)
		m_LockMoving = !m_LockMoving;
}
//----------------------------------------------------------------------------------
bool CGumpMinimap::OnLeftMouseButtonDoubleClick()
{
	WISPFUN_DEBUG("c102_f8");

	g_Orion.OpenMinimap();

	return true;
}
//----------------------------------------------------------------------------------
