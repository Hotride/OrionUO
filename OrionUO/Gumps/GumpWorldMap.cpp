// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
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
CGumpWorldMap::CGumpWorldMap(short x, short y)
: CGump(GT_WORLD_MAP, 0, x, y)
{
	WISPFUN_DEBUG("c132_f1");
	Page = 2;

	Add(new CGUIPage(1));
	Add(new CGUIGumppic(0x15E8, 0, 0)); //Earth button

	Add(new CGUIPage(2));

	m_Minimizer = (CGUIButton*)Add(new CGUIButton(ID_GWM_MINIMIZE, 0x082D, 0x082D, 0x082D, (Width / 2) - 10, 0));
	m_Background = (CGUIResizepic*)Add(new CGUIResizepic(0, 0x0A3C, 0, 23, Width, Height));
	m_Resizer = (CGUIResizeButton*)Add(new CGUIResizeButton(ID_GWM_RESIZE, 0x0837, 0x0838, 0x0838, Width - 8, Height + 13));

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
	Text = (CGUIText*)Add(new CGUIText(0x03B2, 0, 0));
	Text->CreateTextureA(3, "Link with player");
	Text->X = Width - Text->m_Texture.Width;

	m_Checkbox = (CGUICheckbox*)Add(new CGUICheckbox(ID_GWM_LINK_WITH_PLAYER, 0x00D2, 0x00D3, 0x00D2, Text->X - 26, 2));
	m_Checkbox->Checked = m_LinkWithPlayer;
	
	m_Scissor = (CGUIScissor*)Add(new CGUIScissor(true, 0, 0, 8, 32, Width - 16, Height - 16));

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
void CGumpWorldMap::SetLinkWithPlayer(const bool &val)
{
	WISPFUN_DEBUG("c132_f3");
	m_LinkWithPlayer = val;
	m_Checkbox->Checked = val;
	m_MapData->MoveOnDrag = (m_LinkWithPlayer || g_CurrentMap == GetCurrentMap());
	WantRedraw = true;
}
//----------------------------------------------------------------------------------
void CGumpWorldMap::SetScale(const int &val)
{
	WISPFUN_DEBUG("c132_f4");
	m_Scale = val;
	m_ComboboxScale->SelectedIndex = val;
	WantRedraw = true;
}
//----------------------------------------------------------------------------------
void CGumpWorldMap::SetMap(const int &val)
{
	WISPFUN_DEBUG("c132_f5");
	m_Map = val;
	m_ComboboxMap->SelectedIndex = val;
	WantRedraw = true;
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

			if (Minimized)
			{
				g_GumpTranslate.X = (float)MinimizedX;
				g_GumpTranslate.Y = (float)MinimizedY;
			}
			else
			{
				g_GumpTranslate.X = (float)m_X;
				g_GumpTranslate.Y = (float)m_Y;
			}
		}
		else
			WantRedraw = true;
	}
}
//----------------------------------------------------------------------------------
void CGumpWorldMap::GetCurrentCenter(int &x, int &y, int &mouseX, int &mouseY)
{
	WISPFUN_DEBUG("c132_f7");
	x = -OffsetX + mouseX;
	y = -OffsetY + mouseY;
	
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

	offsetX = (Width / 2) - offsetX;
	if (offsetX > 0)
		offsetX = 0;

	offsetY = ((Height - 30) / 2) - offsetY;
	if (offsetY > 0)
		offsetY = 0;

	OffsetX = offsetX;
	OffsetY = offsetY;

	FixOffsets(OffsetX, OffsetY, Width, Height);
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
void CGumpWorldMap::LoadMap(const int &map)
{
	WISPFUN_DEBUG("c132_f11");

	if (!Called || (g_FileManager.m_MapUOP[map].Start == NULL && g_FileManager.m_MapMul[map].Start == NULL) || g_FileManager.m_StaticIdx[map].Start == NULL || g_FileManager.m_StaticMul[map].Start == NULL)
		return;

	if (g_MapTexture[map].Texture == 0)
	{
		uint crc32 = 0;

		if (g_FileManager.m_MapUOP[map].Start == NULL)
			crc32 = g_Orion.GetFileHashCode(g_FileManager.m_MapMul[map].Start, g_FileManager.m_MapMul[map].Size);
		else
			crc32 = g_Orion.GetFileHashCode(g_FileManager.m_MapUOP[map].Start, g_FileManager.m_MapUOP[map].Size);

		crc32 ^= g_Orion.GetFileHashCode(g_FileManager.m_StaticIdx[map].Start, g_FileManager.m_StaticIdx[map].Size);
		crc32 ^= g_Orion.GetFileHashCode(g_FileManager.m_StaticMul[map].Start, g_FileManager.m_StaticMul[map].Size);

		if (g_FileManager.m_MapMul[map].Start != NULL)
		{
			IFOR(i, 0, g_MapManager.PatchesCount)
			{
				if (g_MapManager.m_MapPatchCount[i])
				{
					crc32 ^= g_Orion.GetFileHashCode(g_FileManager.m_MapDifl[i].Start, g_FileManager.m_MapDifl[i].Size);
					crc32 ^= g_Orion.GetFileHashCode(g_FileManager.m_MapDif[i].Start, g_FileManager.m_MapDif[i].Size);
				}

				if (g_MapManager.m_StaticPatchCount[i])
				{
					crc32 ^= g_Orion.GetFileHashCode(g_FileManager.m_StaDifl[i].Start, g_FileManager.m_StaDifl[i].Size);
					crc32 ^= g_Orion.GetFileHashCode(g_FileManager.m_StaDifi[i].Start, g_FileManager.m_StaDifi[i].Size);
				}
			}
		}

		string path = g_App.ExeFilePath("OrionData/WorldMap%08X.cuo", crc32);
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

				if (buf.size() != size)
				{
					LOG("Allocation pixels memory for World Map failed (want size: %i)\n", size);
					fclose(mapFile);
					return;
				}

				size_t readed = fread(&buf[0], sizeof(short), size, mapFile);
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
		
		int wantSize = g_MapSize[map].Width * g_MapSize[map].Height;

		if (!fromFile)
		{
			buf.resize(wantSize, 0);

			if (buf.size() != wantSize)
			{
				LOG("Allocation pixels memory for World Map failed (want size: %i)\n", wantSize);
				return;
			}

			int maxBlock = wantSize - 1;

			IFOR(bx, 0, g_MapBlockSize[map].Width)
			{
				int mapX = (int)bx * 8;

				IFOR(by, 0, g_MapBlockSize[map].Height)
				{
					CIndexMap *indexMap = g_MapManager.GetIndex(map, (int)bx, (int)by);

					if (indexMap == NULL || indexMap->MapAddress == 0)
						continue;

					int mapY = (int)by * 8;
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
						int block = ((mapY + (int)y) * g_MapSize[map].Width) + mapX;

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

			CreateDirectoryA(g_App.ExeFilePath("OrionData").c_str(), NULL);

			FILE *mapFile = NULL;
			fopen_s(&mapFile, path.c_str(), "wb");

			if (mapFile != NULL)
			{
				size_t written = fwrite(&buf[0], sizeof(short), buf.size(), mapFile);
				fclose(mapFile);
				
				LOG("Write world map file, want=%i, written=%i\n", buf.size(), written);
			}
		}

		if (buf.size() == wantSize)
		{
			g_GL.IgnoreHitMap = true;
			g_GL_BindTexture16(g_MapTexture[map], g_MapSize[map].Width, g_MapSize[map].Height, &buf[0]);
			g_GL.IgnoreHitMap = false;
		}
		else
			LOG("World map build error: buffer=%i, want=%i\n", buf.size(), g_MapSize[map].Width * g_MapSize[map].Height);
	}
}
//----------------------------------------------------------------------------------
void CGumpWorldMap::GenerateFrame(const bool &stop)
{
	WISPFUN_DEBUG("c132_f12");
	CGump::GenerateFrame(false);

	//Player drawing
	if (!Minimized && g_CurrentMap == GetCurrentMap())
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

	if (g_ConfigManager.UseGLListsForInterface)
		glEndList();
}
//g_PluginManager.WorldMapDraw();
//----------------------------------------------------------------------------------
void CGumpWorldMap::PrepareContent()
{
	WISPFUN_DEBUG("c132_f13");
	CurrentOffsetX = OffsetX;
	CurrentOffsetY = OffsetY;

	int map = GetCurrentMap();

	LoadMap(map);

	int mapWidth = 0;
	int mapHeight = 0;
	int playerX = 0;
	int playerY = 0;

	if (g_Player != NULL)
	{
		playerX = g_Player->X;
		playerY = g_Player->Y;
	}

	GetScaledDimensions(mapWidth, mapHeight, playerX, playerY);

	m_MapData->Width = mapWidth;
	m_MapData->Height = mapHeight;

	int oldX = CurrentOffsetX;
	int oldY = CurrentOffsetY;

	if (m_LinkWithPlayer && g_CurrentMap == map && g_Player != NULL)
	{
		CurrentOffsetX = (Width / 2) - playerX;

		if (CurrentOffsetX > 0)
			CurrentOffsetX = 0;

		CurrentOffsetY = ((Height - 30) / 2) - playerY;

		if (CurrentOffsetY > 0)
			CurrentOffsetY = 0;

		FixOffsets(CurrentOffsetY, CurrentOffsetY, Width, Height);

		if (OffsetX != CurrentOffsetX || OffsetY != CurrentOffsetY)
		{
			OffsetX = CurrentOffsetX;
			OffsetY = CurrentOffsetY;

			WantRedraw = true;
		}
	}
	else if (m_MapMoving) //Если активировано изменение положения карты
	{
		WISP_GEOMETRY::CPoint2Di offset = g_MouseManager.LeftDroppedOffset();

		CurrentOffsetX += offset.X;
		CurrentOffsetY += offset.Y;

		if (CurrentOffsetX > 0)
			CurrentOffsetX = 0;

		if (CurrentOffsetY > 0)
			CurrentOffsetY = 0;

		FixOffsets(CurrentOffsetX, CurrentOffsetY, Width, Height);
	}

	m_MapData->OffsetX = CurrentOffsetX;
	m_MapData->OffsetY = CurrentOffsetY;

	if (oldX != CurrentOffsetX || oldY != CurrentOffsetY)
		WantRedraw = true;
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
			OffsetX += offset.X;
			OffsetY += offset.Y;
			FixOffsets(OffsetX, OffsetY, Width, Height);
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
		Minimized = true;
		Page = 1;
		WantUpdateContent = true;
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

		ScaleOffsets(index, (Width / 2), ((Height - 30) / 2));
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
				OffsetX = 0;
				OffsetY = 0;
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

	if (Page == 1) //При даблклике по мини-гампу - раскрываем его
	{
		Minimized = false;
		Page = 2;
		WantRedraw = true;

		result = true;
	}

	return result;
}
//----------------------------------------------------------------------------------
void CGumpWorldMap::OnMidMouseButtonScroll(const bool &up)
{
	WISPFUN_DEBUG("c132_f20");
	//Если доступно для изменения масштаба
	if (!Minimized && !g_MouseManager.LeftButtonPressed && !g_MouseManager.RightButtonPressed && g_Orion.PolygonePixelsInXY(m_X + 8, m_Y + 31, Width - 16, Height - 16))
	{
		int ofs = 0;

		if (!up && m_Scale > 0) //Увеличение
			ofs = -1;
		else if (up && m_Scale < 6) //Уменьшение
			ofs = 1;

		if (ofs)
		{
			m_ComboboxScale->SelectedIndex += ofs;
			int mouseX = (Width / 2); //g_MouseX - X + 8;
			int mouseY = ((Height - 30) / 2); //g_MouseY - Y + 31;

			ScaleOffsets(m_Scale + ofs, mouseX, mouseY);
			WantRedraw = true;

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
	if (Height < MIN_WORLD_MAP_HEIGHT)
		Height = MIN_WORLD_MAP_HEIGHT;

	int bh = (GetSystemMetrics(SM_CYSCREEN) - 50);
	if (Height >= bh)
		Height = bh;

	//Подкорректируем временное значение ширины
	if (Width < MIN_WORLD_MAP_WIDTH)
		Width = MIN_WORLD_MAP_WIDTH;

	int bw = (GetSystemMetrics(SM_CXSCREEN) - 50);

	if (Width >= bw)
		Width = bw;

	m_Minimizer->X = (Width / 2) - 10;
	m_Background->Width = Width;
	m_Background->Height = Height;
	m_Resizer->X = Width - 8;
	m_Resizer->Y = Height + 13;
	Text->X = Width - Text->m_Texture.Width;
	m_Checkbox->X = Text->X - 26;
	m_Scissor->Width = Width - 16;
	m_Scissor->Height = Height - 16;
	WantRedraw = true;
	WantUpdateContent = true;
}
//----------------------------------------------------------------------------------
void CGumpWorldMap::GUMP_RESIZE_START_EVENT_C
{
	WISPFUN_DEBUG("c132_f22");
	m_StartResizeWidth = Width;
	m_StartResizeHeight = Height;
}
//----------------------------------------------------------------------------------
void CGumpWorldMap::GUMP_RESIZE_EVENT_C
{
	WISPFUN_DEBUG("c132_f23");
	if (m_StartResizeWidth && m_StartResizeHeight)
	{
		//Событие изменения габаритов гампа с вложенной корректировкой
		WISP_GEOMETRY::CPoint2Di offset = g_MouseManager.LeftDroppedOffset();

		Width = m_StartResizeWidth + offset.X;
		Height = m_StartResizeHeight + offset.Y;

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
