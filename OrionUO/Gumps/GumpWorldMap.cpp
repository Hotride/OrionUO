/***********************************************************************************
**
** GumpWorldMap.cpp
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#include "stdafx.h"
//----------------------------------------------------------------------------------
const int m_Scales[7] = { 1, 1, 1, 2, 4, 6, 10 };
//----------------------------------------------------------------------------------
CGumpWorldMap::CGumpWorldMap(uint serial, short x, short y)
: CGump(GT_WORLD_MAP, serial, x, y)
{
	WISPFUN_DEBUG("c132_f1");
	m_Page = 2;

	Add(new CGUIPage(1));
	Add(new CGUIGumppic(0x15E8, 0, 0)); //Earth button

	Add(new CGUIPage(2));

	m_Minimizer = (CGUIButton*)Add(new CGUIButton(ID_GWM_MINIMIZE, 0x082D, 0x082D, 0x082D, (m_Width / 2) - 10, 0));
	m_Background = (CGUIResizepic*)Add(new CGUIResizepic(0, 0x0A3C, 0, 23, m_Width, m_Height));
	m_Resizer = (CGUIResizeButton*)Add(new CGUIResizeButton(ID_GWM_RESIZE, 0x0837, 0x0838, 0x0838, m_Width - 8, m_Height + 13));

	//Map settings
	static const string mapNames[7] =
	{
		"Current map",
		"Britannia",
		"Trammel",
		"Illshenar",
		"Malas",
		"Tokuno",
		"TerMur"
	};

	//Scale settings
	static const string scaleNames[7] =
	{
		"4:1",
		"2:1",
		"1:1",
		"1:2",
		"1:4",
		"1:6",
		"1:10"
	};

	//Link with player checkbox settings
	m_Text = (CGUIText*)Add(new CGUIText(0x03B2, 0, 0));
	m_Text->CreateTextureA(3, "Link with player");
	m_Text->X = m_Width - m_Text->m_Texture.Width;

	m_Checkbox = (CGUICheckbox*)Add(new CGUICheckbox(ID_GWM_LINK_WITH_PLAYER, 0x00D2, 0x00D3, 0x00D2, m_Text->X - 26, 2));
	m_Checkbox->Checked = m_LinkWithPlayer;
	
	m_Scissor = (CGUIScissor*)Add(new CGUIScissor(true, 0, 0, 8, 32, m_Width - 16, m_Height - 16));

	m_MapData = (CGUIWorldMapTexture*)Add(new CGUIWorldMapTexture(8, 31));
	m_MapData->Serial = ID_GWM_MAP;
	int map = GetCurrentMap();
	m_MapData->Index = map;

	m_MapData->MoveOnDrag = (m_LinkWithPlayer || g_CurrentMap == map);

	LoadMap(map);

	int width = 0;
	int height = 0;
	int playerX = g_Player->X;
	int playerY = g_Player->Y;

	GetScaledDimensions(width, height, playerX, playerY);

	m_MapData->Width = width;
	m_MapData->Height = height;

	Add(new CGUIScissor(false));

	m_ComboboxScale = (CGUIComboBox*)Add(new CGUIComboBox(ID_GWM_SCALE_LIST, 0x098D, true, 0x09B5, 110, 0, 46, 7, false));
	m_ComboboxScale->TextOffsetY = -5;
	m_ComboboxScale->SelectedIndex = m_Scale;

	IFOR(i, 0, 7)
		m_ComboboxScale->Add(new CGUIComboboxText(0, 6, scaleNames[i], 36, TS_CENTER, UOFONT_FIXED));

	m_ComboboxMap = (CGUIComboBox*)Add(new CGUIComboBox(ID_GWM_MAP_LIST, 0x098D, true, 0x09B5, 0, 0, 0, 7, false));
	m_ComboboxMap->TextOffsetY = -5;
	m_ComboboxMap->SelectedIndex = m_Map;

	IFOR(i, 0, 7)
		m_ComboboxMap->Add(new CGUIComboboxText(0, 6, mapNames[i], 98, TS_CENTER, UOFONT_FIXED));
}
//----------------------------------------------------------------------------------
CGumpWorldMap::~CGumpWorldMap()
{
}
//----------------------------------------------------------------------------------
int CGumpWorldMap::GetCurrentMap()
{
	WISPFUN_DEBUG("c132_f2");
	int map = m_Map;

	if (!map)
		map = g_CurrentMap;
	else
		map--;

	return map;
}
//----------------------------------------------------------------------------------
void CGumpWorldMap::OnChangeLinkWithPlayer(const bool &val)
{
	WISPFUN_DEBUG("c132_f3");
	m_Checkbox->Checked = val;
	m_MapData->MoveOnDrag = (m_LinkWithPlayer || g_CurrentMap == GetCurrentMap());
	m_WantRedraw = true;
}
//----------------------------------------------------------------------------------
void CGumpWorldMap::OnChangeScale(const int &val)
{
	WISPFUN_DEBUG("c132_f4");
	m_ComboboxScale->SelectedIndex = val;
	m_WantRedraw = true;
}
//----------------------------------------------------------------------------------
void CGumpWorldMap::OnChangeMap(const int &val)
{
	WISPFUN_DEBUG("c132_f5");
	m_ComboboxMap->SelectedIndex = val;
	m_WantRedraw = true;
}
//----------------------------------------------------------------------------------
void CGumpWorldMap::CalculateGumpState()
{
	WISPFUN_DEBUG("c132_f6");
	CGump::CalculateGumpState();

	if (g_GumpPressed)
	{
		if (g_PressedObject.LeftObject != NULL && ((CBaseGUI*)g_PressedObject.LeftObject)->Type == GOT_COMBOBOX)
		{
			g_GumpMovingOffset.Reset();

			if (m_Minimized)
			{
				g_GumpTranslate.X = (float)m_MinimizedX;
				g_GumpTranslate.Y = (float)m_MinimizedY;
			}
			else
			{
				g_GumpTranslate.X = (float)m_X;
				g_GumpTranslate.Y = (float)m_Y;
			}
		}
		else
			m_WantRedraw = true;
	}
}
//----------------------------------------------------------------------------------
void CGumpWorldMap::GetCurrentCenter(int &x, int &y, int &mouseX, int &mouseY)
{
	WISPFUN_DEBUG("c132_f7");
	x = -m_OffsetX + mouseX;
	y = -m_OffsetY + mouseY;
	
	int scale = m_Scale;

	if (!scale)
	{
		x /= 4;
		y /= 4;
	}
	else if (scale == 1)
	{
		x /= 2;
		y /= 2;
	}
	else if (scale > 2)
	{
		scale = m_Scales[scale];
		
		x *= scale;
		y *= scale;
	}
}
//----------------------------------------------------------------------------------
void CGumpWorldMap::ScaleOffsets(int newScale, int mouseX, int mouseY)
{
	WISPFUN_DEBUG("c132_f8");
	int offsetX = 0;
	int offsetY = 0;

	GetCurrentCenter(offsetX, offsetY, mouseX, mouseY);

	int width = 0;
	int height = 0;
	
	m_Scale = newScale;

	GetScaledDimensions(width, height, offsetX, offsetY);

	offsetX = (m_Width / 2) - offsetX;
	if (offsetX > 0)
		offsetX = 0;

	offsetY = ((m_Height - 30) / 2) - offsetY;
	if (offsetY > 0)
		offsetY = 0;

	m_OffsetX = offsetX;
	m_OffsetY = offsetY;

	FixOffsets(m_OffsetX, m_OffsetY, m_Width, m_Height);
}
//----------------------------------------------------------------------------------
void CGumpWorldMap::GetScaledDimensions(int &width, int &height, int &playerX, int &playerY)
{
	WISPFUN_DEBUG("c132_f9");
	int map = GetCurrentMap();
	
	width = g_MapSize[map].Width;
	height = g_MapSize[map].Height;

	int scale = m_Scale;
	
	if (!scale)
	{
		width *= 4;
		height *= 4;
		playerX *= 4;
		playerY *= 4;
	}
	else if (scale == 1)
	{
		width *= 2;
		height *= 2;
		playerX *= 2;
		playerY *= 2;
	}
	else if (scale > 2)
	{
		scale = m_Scales[scale];

		width /= scale;
		height /= scale;
		playerX /= scale;
		playerY /= scale;
	}
}
//----------------------------------------------------------------------------------
void CGumpWorldMap::FixOffsets(int &offsetX, int &offsetY, int &width, int &height)
{
	WISPFUN_DEBUG("c132_f10");
	int mapWidth = 0;
	int mapHeight = 0;
	int playerX = 0;
	int playerY = 0;

	GetScaledDimensions(mapWidth, mapHeight, playerX, playerY);
	
	if (offsetX + mapWidth < width)
		offsetX = width - mapWidth;
	
	if (offsetY + mapHeight < height)
		offsetY = height - mapHeight;
	
	if (offsetX > 0)
		offsetX = 0;

	if (offsetY > 0)
		offsetY = 0;
}
//----------------------------------------------------------------------------------
void CGumpWorldMap::LoadMap(int map)
{
	WISPFUN_DEBUG("c132_f11");

	if ((g_FileManager.m_MapUOP[map].Start == NULL && g_FileManager.m_MapMul[map].Start == NULL) || g_FileManager.m_StaticIdx[map].Start == NULL || g_FileManager.m_StaticMul[map].Start == NULL)
		return;

	if (g_MapTexture[map].Texture == 0)
	{
		uint crc32 = 0;

		if (g_FileManager.m_MapUOP[map].Start == NULL)
			crc32 = g_Orion.GetFileHashCode((uint)g_FileManager.m_MapMul[map].Start, g_FileManager.m_MapMul[map].Size);
		else
			crc32 = g_Orion.GetFileHashCode((uint)g_FileManager.m_MapUOP[map].Start, g_FileManager.m_MapUOP[map].Size);

		crc32 ^= g_Orion.GetFileHashCode((uint)g_FileManager.m_StaticIdx[map].Start, g_FileManager.m_StaticIdx[map].Size);
		crc32 ^= g_Orion.GetFileHashCode((uint)g_FileManager.m_StaticMul[map].Start, g_FileManager.m_StaticMul[map].Size);

		IFOR(i, 0, g_MapManager.PatchesCount)
		{
			if (g_MapManager.m_MapPatchCount[i])
			{
				crc32 ^= g_Orion.GetFileHashCode((uint)g_FileManager.m_MapDifl[i].Start, g_FileManager.m_MapDifl[i].Size);
				crc32 ^= g_Orion.GetFileHashCode((uint)g_FileManager.m_MapDif[i].Start, g_FileManager.m_MapDif[i].Size);
			}

			if (g_MapManager.m_StaticPatchCount[i])
			{
				crc32 ^= g_Orion.GetFileHashCode((uint)g_FileManager.m_StaDifl[i].Start, g_FileManager.m_StaDifl[i].Size);
				crc32 ^= g_Orion.GetFileHashCode((uint)g_FileManager.m_StaDifi[i].Start, g_FileManager.m_StaDifi[i].Size);
			}
		}

		string path = g_App.FilePath("OrionData/WorldMap%08X.cuo", crc32);
		bool fromFile = false;

		USHORT_LIST buf;

		if (PathFileExistsA(path.c_str()))
		{
			fromFile = true;
			FILE *mapFile = NULL;
			fopen_s(&mapFile, path.c_str(), "rb");

			if (mapFile != NULL)
			{
				fseek(mapFile, 0, SEEK_END);
				long size = ftell(mapFile) / sizeof(short);
				fseek(mapFile, 0, SEEK_SET);

				buf.resize(size, 0);
				int readed = fread(&buf[0], sizeof(short), size, mapFile);
				fclose(mapFile);

				if (readed != size)
				{
					LOG("Error reading world map file, want=%i, readed=%i\n", size, readed);
					fromFile = false;
				}
				else
					LOG("World map readed from file!\n");
			}
			else
				LOG("Error open world map file: %s\n", path.c_str());
		}
		
		if (!fromFile)
		{
			buf.resize(g_MapSize[map].Width * g_MapSize[map].Height, 0);
			int maxBlock = (g_MapSize[map].Width * g_MapSize[map].Height) - 1;

			IFOR(bx, 0, g_MapBlockSize[map].Width)
			{
				int mapX = bx * 8;

				IFOR(by, 0, g_MapBlockSize[map].Height)
				{
					CIndexMap *indexMap = g_MapManager.GetIndex(map, bx, by);

					if (indexMap == NULL || indexMap->MapAddress == 0)
						continue;

					int mapY = by * 8;
					MAP_BLOCK info = { 0 };

					PMAP_BLOCK mapBlock = (PMAP_BLOCK)indexMap->MapAddress;

					int pos = 0;

					IFOR(y, 0, 8)
					{
						IFOR(x, 0, 8)
						{
							MAP_CELLS &cell = mapBlock->Cells[pos];
							MAP_CELLS &infoCell = info.Cells[pos];
							infoCell.TileID = cell.TileID;
							infoCell.Z = cell.Z;
							pos++;
						}
					}

					PSTATICS_BLOCK sb = (PSTATICS_BLOCK)indexMap->StaticAddress;

					if (sb != NULL)
					{
						int count = indexMap->StaticCount;

						IFOR(c, 0, count)
						{
							STATICS_BLOCK &staticBlock = sb[c];

							if (staticBlock.Color && staticBlock.Color != 0xFFFF && !CRenderStaticObject::IsNoDrawTile(staticBlock.Color))
							{
								pos = (staticBlock.Y * 8) + staticBlock.X;
								//if (pos > 64) continue;

								MAP_CELLS &infoCell = info.Cells[pos];

								if (infoCell.Z <= staticBlock.Z)
								{
									infoCell.TileID = staticBlock.Color + 0x4000;
									infoCell.Z = staticBlock.Z;
								}
							}
						}
					}

					pos = 0;

					IFOR(y, 0, 8)
					{
						int block = ((mapY + y) * g_MapSize[map].Width) + mapX;

						IFOR(x, 0, 8)
						{
							ushort color = 0x8000 | g_ColorManager.GetRadarColorData(info.Cells[pos].TileID);

							buf[block] = color;

							if (y < 7 && x < 7 && block < maxBlock)
								buf[block + 1] = color;

							block++;
							pos++;
						}
					}
				}
			}

			CreateDirectoryA(g_App.FilePath("OrionData").c_str(), NULL);

			FILE *mapFile = NULL;
			fopen_s(&mapFile, path.c_str(), "wb");

			if (mapFile != NULL)
			{
				int written = fwrite(&buf[0], sizeof(short), buf.size(), mapFile);
				fclose(mapFile);
				
				LOG("Write world map file, want=%i, written=%i\n", buf.size(), written);
			}
		}

		if (buf.size() == g_MapSize[map].Width * g_MapSize[map].Height)
			g_GL_BindTexture16(g_MapTexture[map], g_MapSize[map].Width, g_MapSize[map].Height, &buf[0]);
		else
			LOG("World map build error: buffer=%i, want=%i\n", buf.size(), g_MapSize[map].Width * g_MapSize[map].Height);
	}
}
//----------------------------------------------------------------------------------
void CGumpWorldMap::GenerateFrame(bool stop)
{
	WISPFUN_DEBUG("c132_f12");
	CGump::GenerateFrame(false);

	//Player drawing
	if (!m_Minimized && g_CurrentMap == GetCurrentMap())
	{
		int width = 0;
		int height = 0;
		int playerX = g_Player->X;
		int playerY = g_Player->Y;

		GetScaledDimensions(width, height, playerX, playerY);

		m_Scissor->Draw(false);

		glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

		g_GL.DrawPolygone(m_MapData->OffsetX + playerX + 0, m_MapData->OffsetY + playerY + 30, 16, 2);
		g_GL.DrawPolygone(m_MapData->OffsetX + playerX + 7, m_MapData->OffsetY + playerY + 23, 2, 16);
		g_GL.DrawCircle(m_MapData->OffsetX + playerX + 8.0f, m_MapData->OffsetY + playerY + 31.0f, 3.0f);

		g_GL.PopScissor();
	}

	glEndList();
}
//g_PluginManager.WorldMapDraw();
//----------------------------------------------------------------------------------
void CGumpWorldMap::PrepareContent()
{
	WISPFUN_DEBUG("c132_f13");
	m_CurrentOffsetX = m_OffsetX;
	m_CurrentOffsetY = m_OffsetY;

	int map = GetCurrentMap();

	LoadMap(map);

	int mapWidth = 0;
	int mapHeight = 0;
	int playerX = g_Player->X;
	int playerY = g_Player->Y;

	GetScaledDimensions(mapWidth, mapHeight, playerX, playerY);

	m_MapData->Width = mapWidth;
	m_MapData->Height = mapHeight;

	int oldX = m_CurrentOffsetX;
	int oldY = m_CurrentOffsetY;

	if (m_LinkWithPlayer && g_CurrentMap == map && g_Player != NULL)
	{
		m_CurrentOffsetX = (m_Width / 2) - playerX;

		if (m_CurrentOffsetX > 0)
			m_CurrentOffsetX = 0;

		m_CurrentOffsetY = ((m_Height - 30) / 2) - playerY;

		if (m_CurrentOffsetY > 0)
			m_CurrentOffsetY = 0;

		FixOffsets(m_CurrentOffsetY, m_CurrentOffsetY, m_Width, m_Height);

		if (m_OffsetX != m_CurrentOffsetX || m_OffsetY != m_CurrentOffsetY)
		{
			m_OffsetX = m_CurrentOffsetX;
			m_OffsetY = m_CurrentOffsetY;

			m_WantRedraw = true;
		}
	}
	else if (m_MapMoving) //Если активировано изменение положения карты
	{
		WISP_GEOMETRY::CPoint2Di offset = g_MouseManager.LeftDroppedOffset();

		m_CurrentOffsetX += offset.X;
		m_CurrentOffsetY += offset.Y;

		if (m_CurrentOffsetX > 0)
			m_CurrentOffsetX = 0;

		if (m_CurrentOffsetY > 0)
			m_CurrentOffsetY = 0;

		FixOffsets(m_CurrentOffsetX, m_CurrentOffsetY, m_Width, m_Height);
	}

	m_MapData->OffsetX = m_CurrentOffsetX;
	m_MapData->OffsetY = m_CurrentOffsetY;

	if (oldX != m_CurrentOffsetX || oldY != m_CurrentOffsetY)
		m_WantRedraw = true;
}
//----------------------------------------------------------------------------------
void CGumpWorldMap::OnLeftMouseButtonDown()
{
	WISPFUN_DEBUG("c132_f14");
	CGump::OnLeftMouseButtonDown();

	if (g_PressedObject.LeftObject == m_MapData) //Карта
	{
		if (!m_LinkWithPlayer || g_CurrentMap != GetCurrentMap())
		{
			m_MapData->MoveOnDrag = false;
			m_MapMoving = true;
		}
	}
}
//----------------------------------------------------------------------------------
void CGumpWorldMap::OnLeftMouseButtonUp()
{
	WISPFUN_DEBUG("c132_f15");
	CGump::OnLeftMouseButtonUp();

	if (g_PressedObject.LeftObject == m_MapData) //Карта
	{
		if (m_MapMoving)
		{
			WISP_GEOMETRY::CPoint2Di offset = g_MouseManager.LeftDroppedOffset();
			m_OffsetX += offset.X;
			m_OffsetY += offset.Y;
			FixOffsets(m_OffsetX, m_OffsetY, m_Width, m_Height);
		}

		m_MapMoving = false;
	}
}
//----------------------------------------------------------------------------------
void CGumpWorldMap::GUMP_BUTTON_EVENT_C
{
	WISPFUN_DEBUG("c132_f16");
	if (serial == ID_GWM_MINIMIZE) //Сворачивание
	{
		m_Minimized = true;
		m_Page = 1;
	}
}
//----------------------------------------------------------------------------------
void CGumpWorldMap::GUMP_CHECKBOX_EVENT_C
{
	WISPFUN_DEBUG("c132_f17");
	if (serial == ID_GWM_LINK_WITH_PLAYER) //Привязка к координатам игрока
	{
		m_LinkWithPlayer = state;
		m_MapData->MoveOnDrag = (m_LinkWithPlayer || g_CurrentMap == GetCurrentMap());
	}
}
//----------------------------------------------------------------------------------
void CGumpWorldMap::GUMP_COMBOBOX_SELECTION_EVENT_C
{
	WISPFUN_DEBUG("c132_f18");
	if (serial >= ID_GWM_SCALE_LIST)
	{
		int index = serial - ID_GWM_SCALE_LIST;

		ScaleOffsets(index, (m_Width / 2), ((m_Height - 30) / 2));
	}
	else if (serial >= ID_GWM_MAP_LIST)
	{
		int index = serial - ID_GWM_MAP_LIST;

		int mapTest = index;

		if (!mapTest)
			mapTest = g_CurrentMap;
		else
			mapTest--;

		DebugMsg("g_MapTexture[mapTest].Texture = %i\n", g_MapTexture[mapTest].Texture);
		if (g_MapTexture[mapTest].Texture == 0)
			LoadMap(mapTest);

		if (g_MapTexture[mapTest].Texture != 0)
		{
			if (mapTest != m_Map)
			{
				m_OffsetX = 0;
				m_OffsetY = 0;
			}

			m_Map = index;
			m_MapData->Index = GetCurrentMap();
		}
		else
			m_ComboboxMap->SelectedIndex = m_Map;

		m_MapData->MoveOnDrag = (m_LinkWithPlayer || g_CurrentMap == GetCurrentMap());
	}

	int width = 0;
	int height = 0;
	int playerX = g_Player->X;
	int playerY = g_Player->Y;

	GetScaledDimensions(width, height, playerX, playerY);

	m_MapData->Width = width;
	m_MapData->Height = height;
}
//----------------------------------------------------------------------------------
bool CGumpWorldMap::OnLeftMouseButtonDoubleClick()
{
	WISPFUN_DEBUG("c132_f19");
	bool result = false;

	if (m_Page == 1) //При даблклике по мини-гампу - раскрываем его
	{
		m_Minimized = false;
		m_Page = 2;
		m_WantRedraw = true;

		result = true;
	}

	return result;
}
//----------------------------------------------------------------------------------
void CGumpWorldMap::OnMidMouseButtonScroll(const bool &up)
{
	WISPFUN_DEBUG("c132_f20");
	//Если доступно для изменения масштаба
	if (!m_Minimized && !g_MouseManager.LeftButtonPressed && !g_MouseManager.RightButtonPressed && g_Orion.PolygonePixelsInXY(m_X + 8, m_Y + 31, m_Width - 16, m_Height - 16))
	{
		int ofs = 0;

		if (!up && m_Scale > 0) //Увеличение
			ofs = -1;
		else if (up && m_Scale < 6) //Уменьшение
			ofs = 1;

		if (ofs)
		{
			m_ComboboxScale->SelectedIndex += ofs;
			int mouseX = (m_Width / 2); //g_MouseX - X + 8;
			int mouseY = ((m_Height - 30) / 2); //g_MouseY - Y + 31;

			ScaleOffsets(m_Scale + ofs, mouseX, mouseY);
			m_WantRedraw = true;

			int width = 0;
			int height = 0;
			int playerX = g_Player->X;
			int playerY = g_Player->Y;

			GetScaledDimensions(width, height, playerX, playerY);

			m_MapData->Width = width;
			m_MapData->Height = height;
		}
	}
}
//----------------------------------------------------------------------------------
void CGumpWorldMap::UpdateSize()
{
	WISPFUN_DEBUG("c132_f21");
	//Подкорректируем временное значение высоты
	if (m_Height < MIN_WORLD_MAP_HEIGHT)
		m_Height = MIN_WORLD_MAP_HEIGHT;

	int bh = (GetSystemMetrics(SM_CYSCREEN) - 50);
	if (m_Height >= bh)
		m_Height = bh;

	//Подкорректируем временное значение ширины
	if (m_Width < MIN_WORLD_MAP_WIDTH)
		m_Width = MIN_WORLD_MAP_WIDTH;

	int bw = (GetSystemMetrics(SM_CXSCREEN) - 50);

	if (m_Width >= bw)
		m_Width = bw;

	m_Minimizer->X = (m_Width / 2) - 10;
	m_Background->Width = m_Width;
	m_Background->Height = m_Height;
	m_Resizer->X = m_Width - 8;
	m_Resizer->Y = m_Height + 13;
	m_Text->X = m_Width - m_Text->m_Texture.Width;
	m_Checkbox->X = m_Text->X - 26;
	m_Scissor->Width = m_Width - 16;
	m_Scissor->Height = m_Height - 16;
	m_WantRedraw = true;
}
//----------------------------------------------------------------------------------
void CGumpWorldMap::GUMP_RESIZE_START_EVENT_C
{
	WISPFUN_DEBUG("c132_f22");
	m_StartResizeWidth = m_Width;
	m_StartResizeHeight = m_Height;
}
//----------------------------------------------------------------------------------
void CGumpWorldMap::GUMP_RESIZE_EVENT_C
{
	WISPFUN_DEBUG("c132_f23");
	if (m_StartResizeWidth && m_StartResizeHeight)
	{
		//Событие изменения габаритов гампа с вложенной корректировкой
		WISP_GEOMETRY::CPoint2Di offset = g_MouseManager.LeftDroppedOffset();

		m_Width = m_StartResizeWidth + offset.X;
		m_Height = m_StartResizeHeight + offset.Y;

		UpdateSize();
	}
}
//----------------------------------------------------------------------------------
void CGumpWorldMap::GUMP_RESIZE_END_EVENT_C
{
	m_StartResizeWidth = 0;
	m_StartResizeHeight = 0;
}
//----------------------------------------------------------------------------------
