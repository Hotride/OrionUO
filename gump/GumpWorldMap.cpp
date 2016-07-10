/****************************************************************************
**
** GumpWorldMap.cpp
**
** Copyright (C) November 2015 Hotride
**
** This program is free software; you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation; either version 2 of the License, or
** (at your option) any later version.
**
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**
*****************************************************************************
*/
//----------------------------------------------------------------------------
#include "stdafx.h"

TTextTexture TGumpWorldMap::m_Text;
TTextTexture TGumpWorldMap::m_TextMap[7];
TTextTexture TGumpWorldMap::m_TextScale[7];

const int m_Scales[7] = {1, 1, 1, 2, 4, 6, 10};
//---------------------------------------------------------------------------
TGumpWorldMap::TGumpWorldMap(DWORD serial, short x, short y)
: TGump(GT_WORLD_MAP, serial, x, y), m_Width(400), m_Height(300), m_Scale(2),
m_Map(0), m_OffsetX(0), m_OffsetY(0), m_OpenedList(0), m_MapMoving(false),
m_Resizing(false), m_LinkWithPlayer(true), m_Called(false)
{
}
//---------------------------------------------------------------------------
TGumpWorldMap::~TGumpWorldMap()
{
}
//----------------------------------------------------------------------------
void TGumpWorldMap::InitTextTextures()
{
	const int font = 6;
	const WORD color = 0;

	FontManager->GenerateA(3, m_Text, "Link with player", 0x03B2);

	int width = 98;
	FontManager->GenerateA(font, m_TextMap[0], "Current map", color, width, TS_CENTER, UOFONT_FIXED);
	FontManager->GenerateA(font, m_TextMap[1], "Britannia", color, width, TS_CENTER, UOFONT_FIXED);
	FontManager->GenerateA(font, m_TextMap[2], "Trammel", color, width, TS_CENTER, UOFONT_FIXED);
	FontManager->GenerateA(font, m_TextMap[3], "Illshenar", color, width, TS_CENTER, UOFONT_FIXED);
	FontManager->GenerateA(font, m_TextMap[4], "Malas", color, width, TS_CENTER, UOFONT_FIXED);
	FontManager->GenerateA(font, m_TextMap[5], "Tokuno", color, width, TS_CENTER, UOFONT_FIXED);
	FontManager->GenerateA(font, m_TextMap[6], "TerMur", color, width, TS_CENTER, UOFONT_FIXED);
	
	width = 36;
	FontManager->GenerateA(font, m_TextScale[0], "4:1", color, width, TS_CENTER, UOFONT_FIXED);
	FontManager->GenerateA(font, m_TextScale[1], "2:1", color, width, TS_CENTER, UOFONT_FIXED);
	FontManager->GenerateA(font, m_TextScale[2], "1:1", color, width, TS_CENTER, UOFONT_FIXED);
	FontManager->GenerateA(font, m_TextScale[3], "1:2", color, width, TS_CENTER, UOFONT_FIXED);
	FontManager->GenerateA(font, m_TextScale[4], "1:4", color, width, TS_CENTER, UOFONT_FIXED);
	FontManager->GenerateA(font, m_TextScale[5], "1:6", color, width, TS_CENTER, UOFONT_FIXED);
	FontManager->GenerateA(font, m_TextScale[6], "1:10", color, width, TS_CENTER, UOFONT_FIXED);
}
//----------------------------------------------------------------------------
void TGumpWorldMap::ReleaseTextTextures()
{
	m_Text.Clear();

	IFOR(i, 0, 7)
		m_TextMap[i].Clear();

	IFOR(i, 0, 7)
		m_TextScale[i].Clear();

}
//---------------------------------------------------------------------------
void TGumpWorldMap::ChangeHeight()
{
	//Событие изменения габаритов гампа с вложенной корректировкой
	if (m_Resizing)
	{
		m_Width += (g_MouseX - g_DroppedLeftMouseX);
		m_Height += (g_MouseY - g_DroppedLeftMouseY);

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
	}

	m_Resizing = false;
	g_ResizedGump = NULL;
}
//---------------------------------------------------------------------------
void TGumpWorldMap::GetCurrentCenter(int &x, int &y, int &mouseX, int &mouseY)
{
	x = (m_OffsetX * (-1)) + mouseX;
	y = (m_OffsetY * (-1)) + mouseY;
	
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
//---------------------------------------------------------------------------
void TGumpWorldMap::ScaleOffsets(int newScale, int mouseX, int mouseY)
{
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
//---------------------------------------------------------------------------
void TGumpWorldMap::GetScaledDimensions(int &width, int &height, int &playerX, int &playerY)
{
	int map = m_Map;

	if (!map)
		map = g_CurrentMap;
	else
		map--;
	
	width = g_MapSizeX[map];
	height = g_MapSizeY[map];

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
//---------------------------------------------------------------------------
void TGumpWorldMap::FixOffsets(int &offsetX, int &offsetY, int &width, int &height)
{
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
//---------------------------------------------------------------------------
void TGumpWorldMap::LoadMap(int map)
{
	if (!g_MapTexture[map])
	{
		TMappedHeader file;
		memset(&file, 0, sizeof(file));

		FileManager.LoadFileToMemory(file, FilePath("MapsInfo.cuo").c_str());

		const int mapsInfoFileSize = sizeof(DWORD) * 2 * 6;
		BYTE mapsInfoData[mapsInfoFileSize] = {0};

		bool foundInTable = false;

		DWORD mapHash = 0;
		DWORD staticsHash = 0;

		DWORD hashSizeContent = (g_MapSizeX[map] << 16) | g_MapSizeY[map];

		DWORD mulMapHash = UO->GetFileHashCode((DWORD)FileManager.MapMul[map].Address, FileManager.MapMul[map].Size);
		mulMapHash ^= hashSizeContent;
		DWORD mulStaticsHash = UO->GetFileHashCode((DWORD)FileManager.StaticIdx[map].Address, FileManager.StaticIdx[map].Size);
		mulStaticsHash ^= UO->GetFileHashCode((DWORD)FileManager.StaticMul[map].Address, FileManager.StaticMul[map].Size);
		mulStaticsHash ^= hashSizeContent;

		char pathBuf[50] = {0};
		sprintf(pathBuf, "mapImage%i.cuo", map);

		PWORD data = NULL;
		DWORD dataSize = g_MapSizeX[map] * g_MapSizeY[map];

		if (file.Size != 0 && file.Size != (dataSize * 2))
		{
			memcpy(&mapsInfoData[0], (PBYTE)file.Address, mapsInfoFileSize);
			mapHash = file.ReadDWord();
			staticsHash = file.ReadDWord();

			if ((mapHash && mapHash == mulMapHash) && (staticsHash && staticsHash == mulStaticsHash))
			{
				TMappedHeader mapFile;
				memset(&mapFile, 0, sizeof(mapFile));

				FileManager.LoadFileToMemory(mapFile, FilePath(pathBuf).c_str());

				if (mapFile.Size)
				{
					data = (PWORD)mapFile.Address;
					g_GL.BindTexture16(g_MapTexture[map], g_MapSizeX[map], g_MapSizeY[map], data);

					FileManager.UnloadFileFromMemory(mapFile);

					foundInTable = true;
				}
			}
		}

		FileManager.UnloadFileFromMemory(file);
		
		if (!foundInTable && FileManager.MapMul[map].Size != 0 && FileManager.StaticIdx[map].Size != 0 && FileManager.StaticMul[map].Size != 0)
		{
			PDWORD dataPtr = (PDWORD)(mapsInfoData + ((sizeof(DWORD) * 2) * (map + 1)));

			*dataPtr++ = mulMapHash;
			*dataPtr = mulStaticsHash;

			data = new WORD[dataSize];

			IFOR(bx, 0, g_MapBlockX[map])
			{
				IFOR(by, 0, g_MapBlockY[map])
				{
					MAP_BLOCK mb = {0};
					MapManager->GetWorldMapBlock(map, bx, by, mb);
				
					int mapX = bx * 8;
					int mapY = by * 8;

					IFOR(x, 0, 8)
					{
						IFOR(y, 0, 8)
						{
							int px = mapX + x;
							int py = mapY + y;
							
							WORD color = mb.Cells[(y * 8) + x].TileID;
							WORD pcl = 0x8000 | ColorManager->GetRadarColorData(color);
							int block = py * g_MapSizeX[map] + px;
							data[block] = pcl;

							if (x < 7 && y < 7)
								data[block + 1] = pcl;
						}
					}
				}
			}

			g_GL.BindTexture16(g_MapTexture[map], g_MapSizeX[map], g_MapSizeY[map], data);

			FILE *mapDataFile = fopen(FilePath(pathBuf).c_str(), "wb");

			if (mapDataFile != NULL)
			{
				fwrite(&data[0], 2, dataSize, mapDataFile);
				fclose(mapDataFile);
			}

			FILE *mapsInfoFile = fopen(FilePath("MapsInfo.cuo").c_str(), "wb");

			if (mapsInfoFile != NULL)
			{
				fwrite(&mapsInfoData[0], mapsInfoFileSize, 1, mapsInfoFile);
				fclose(mapsInfoFile);
			}

			delete data;
		}
	}
}
//---------------------------------------------------------------------------
void TGumpWorldMap::PrepareTextures()
{
	UO->ExecuteResizepic(0x0A3C);
	UO->ExecuteGumpPart(0x0837, 2);
	UO->ExecuteGump(0x139D);
}
//---------------------------------------------------------------------------
void TGumpWorldMap::GenerateFrame()
{
	if (!g_DrawMode)
	{
		m_FrameRedraw = false;
		m_FrameCreated = false;

		return;
	}
}
//----------------------------------------------------------------------------
int TGumpWorldMap::Draw(bool &mode)
{
	if (!m_Called)
		return 0;

	DWORD index = (DWORD)this;

	//Для быстрого доступа
	int posX = X;
	int posY = Y;

	if (Minimized)
	{
		posX = MinimizedX;
		posY = MinimizedY;
	}

	//Нажата ли кнопка в окне?
	bool IsPressed = (g_LeftMouseDown && g_LastGumpLeftMouseDown == index && g_LastSelectedGump == index);

	//Может ли быть подсвечен элемент?
	int CanSelectedButton = ((g_LastSelectedGump == index) ? g_LastSelectedObject : 0);

	//Может ли быть нажат элемент?
	int CanPressedButton = 0;
	if (IsPressed && g_LastObjectLeftMouseDown == g_LastSelectedObject)
		CanPressedButton = g_LastObjectLeftMouseDown;
	
	int height = m_Height; //Высота для быстрого доступа и безобезненной временной корректировки
	int width = m_Width; //Ширина для быстрого доступа и безобезненной временной корректировки

	if (m_Resizing) //Если активировано изменение габаритов гампа
	{
		width += (g_MouseX - g_DroppedLeftMouseX);
		height += (g_MouseY - g_DroppedLeftMouseY);

		//Подкорректируем временное значение высоты
		if (height < MIN_WORLD_MAP_HEIGHT)
			height = MIN_WORLD_MAP_HEIGHT;

		int bh = (GetSystemMetrics(SM_CYSCREEN) - 50);
		if (height >= bh)
			height = bh;

		//Подкорректируем временное значение ширины
		if (width < MIN_WORLD_MAP_WIDTH)
			width = MIN_WORLD_MAP_WIDTH;

		int bw = (GetSystemMetrics(SM_CXSCREEN) - 50);
		if (width >= bw)
			width = bw;
	}

	int offsetX = m_OffsetX;
	int offsetY = m_OffsetY;
	
	int map = m_Map;

	if (!map)
		map = g_CurrentMap;
	else
		map--;

	LoadMap(map);

	if (m_MapMoving && (!m_LinkWithPlayer || g_CurrentMap != map)) //Если активировано изменение положения карты
	{
		offsetX += (g_MouseX - g_DroppedLeftMouseX);
		offsetY += (g_MouseY - g_DroppedLeftMouseY);
		
		if (offsetX > 0)
			offsetX = 0;
		
		if (offsetY > 0)
			offsetY = 0;

		FixOffsets(offsetX, offsetY, width, height);
	}

	//Если окошко захвачено для перемещения - вычислим оффсеты
	if (mode && g_LeftMouseDown && g_LastGumpLeftMouseDown == index && !g_LastObjectLeftMouseDown)
	{
		posX += (g_MouseX - g_DroppedLeftMouseX);
		posY += (g_MouseY - g_DroppedLeftMouseY);
	}

	int mapViewWidth = width - 16;
	int mapViewHeight = height - 16;

	int mapWidth = 0;
	int mapHeight = 0;

	int playerX = g_Player->X;
	int playerY = g_Player->Y;

	GetScaledDimensions(mapWidth, mapHeight, playerX, playerY);
	
	if (mode) //Отрисовка
	{
		if (Minimized)
		{
			UO->DrawGump(0x15E8, 0, posX, posY); //Earth button

			return 0;
		}

		if (m_LinkWithPlayer && g_CurrentMap == map)
		{
			offsetX = (width / 2) - playerX;
			if (offsetX > 0)
				offsetX = 0;

			offsetY = ((height - 30) / 2) - playerY;
			if (offsetY > 0)
				offsetY = 0;

			FixOffsets(offsetX, offsetY, width, height);
			
			m_OffsetX = offsetX;
			m_OffsetY = offsetY;
		}

		UO->DrawGump(0x082D, 0, posX + (width / 2) - 10, posY); //Minimize

		UO->DrawResizepicGump(0x0A3C, posX, posY + 23, width, height);

		//Ресайзер
		WORD resizeGumpID = 0x0837 + (int)(CanPressedButton == 2);

		UO->DrawGump(resizeGumpID, 0, posX + width - 8, posY + 23 + height - 10);
		
		//Map settings
		UO->DrawGump(0x098D, 0, posX, posY);
		m_TextMap[m_Map].Draw(posX + 4, posY);
		UO->DrawGump(0x0985, 0, posX + 94, posY + 7);

		//Scale settings
		TTextureObject *g = UO->ExecuteGump(0x098B);
		if (g != NULL)
		{
			int tmpX = posX + 110;
			g_GL.Draw(g->Texture, tmpX, posY, 46, g->Height);
		}
		else
			UO->DrawGump(0x098B, 0, posX + 110, posY);

		m_TextScale[m_Scale].Draw(posX + 114, posY);
		UO->DrawGump(0x0985, 0, posX + 142, posY + 7);

		//Link with player checkbox settings
		int drawX = posX + width - m_Text.Width;
		m_Text.Draw(drawX, posY);

		drawX -= 26;
		UO->DrawGump(0x00D2 + (int)m_LinkWithPlayer, 0, drawX, posY + 2);

		//Map drawing
		g_GL.ViewPort(posX + 8, posY + 31, mapViewWidth, mapViewHeight);

		int drawMapX = posX + offsetX + 8;
		int drawMapY = posY + offsetY + 31;
		g_GL.Draw(g_MapTexture[map], drawMapX, drawMapY, mapWidth, mapHeight);
		
		//Player drawing
		if (g_CurrentMap == map)
		{
			glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
			g_GL.DrawPolygone(posX + offsetX + playerX + 0, posY + offsetY + playerY + 30, 16, 2);
			g_GL.DrawPolygone(posX + offsetX + playerX + 7, posY + offsetY + playerY + 23, 2, 16);
			g_GL.DrawCircle(posX + offsetX + 8.0f + playerX, posY + offsetY + 31.0f + playerY, 3.0f);
		}

		g_GL.RestorePort();

		/*int cx = 0, cy = 0, mx = (m_Width / 2), my = ((m_Height - 30) / 2);
		GetCurrentCenter(cx, cy, mx, my);
		char buff[50] = {0};
		sprintf(buff, "X=%i Y=%i", cx, cy);
		FontManager->DrawA(3, buff, 0x44, posX + 10, posY + 30);*/

		if (m_OpenedList)
		{
			posY += 12;
			//Top
			UO->DrawGump(0x09B5, 0, posX - 5, posY - 11);

			int ofs = 0;

			IFOR(i, 0, 7)
			{
				WORD gumpID = 0x09B6 + ofs;
				ofs = (ofs + 1) % 3;

				UO->DrawGump(gumpID, 0, posX, posY);
				
				if (CanSelectedButton >= ID_GWM_LIST)
				{
					if (i + 1 == (CanSelectedButton - ID_GWM_LIST))
						g_GL.DrawPolygone(posX + 4, posY, 150, 14);
				}

				if (m_OpenedList == 1)
					m_TextMap[i].Draw(posX + 4, posY - 5);
				else
					m_TextScale[i].Draw(posX + 4, posY - 5);

				posY += 15;
			}

			//Bottom
			UO->DrawGump(0x09B9, 0, posX - 5, posY);
		}
	}
	else //Выбор объектов
	{
		if (m_Minimized)
		{
			if (UO->GumpPixelsInXY(0x15E8, posX, posY)) //Earth button
			{
				g_LastSelectedObject = 0;
				g_LastSelectedGump = index;
			}

			return 0;
		}

		int LSG = 0;

		//Если выбран основной гамп - меняем глобальный указатель на выбранный гамп на него
		if (UO->ResizepicPixelsInXY(0x0A3C, posX, posY + 23, width, height))
		{
			g_LastSelectedObject = 0;
			g_LastSelectedGump = index;
		}

		if (UO->GumpPixelsInXY(0x082D, posX + (width / 2) - 10, posY)) //Minimize
			LSG = ID_GWM_MINIMIZE;
		else if (UO->GumpPixelsInXY(0x0837, posX + width - 8, posY + 23 + height - 10)) //Ресайзер
			LSG = ID_GWM_RESIZE;
		else if ((!m_LinkWithPlayer || g_CurrentMap != map) && UO->PolygonePixelsInXY(posX + 8, posY + 31, mapViewWidth, mapViewHeight)) //Карта
			LSG = ID_GWM_MAP;
		else if (UO->PolygonePixelsInXY(posX, posY, 110, 22)) //Настройки карты
			LSG = ID_GWM_MAP_LIST;
		else if (UO->PolygonePixelsInXY(posX + 110, posY, 46, 22)) //Настройки масштаба
			LSG = ID_GWM_SCALE_LIST;
		else if (UO->PolygonePixelsInXY(posX + width - (m_Text.Width + 26), posY, m_Text.Width + 26, 22)) //Настройки привязки к координатам игрока
			LSG = ID_GWM_LINK_WITH_PLAYER;
		
		if (m_OpenedList)
		{
			posY += 12;
			//Top
			UO->DrawGump(0x09B5, 0, posX - 5, posY - 11);

			int ofs = 0;

			IFOR(i, 0, 7)
			{
				if (UO->GumpPixelsInXY(0x09B6 + ofs, posX, posY))
					LSG = ID_GWM_LIST + i + 1;
				
				ofs = (ofs + 1) % 3;
				posY += 15;
			}

			//Bottom
			UO->DrawGump(0x09B9, 0, posX - 5, posY);
		}

		if (LSG != 0)
		{
			g_LastSelectedObject = LSG; //Если что-то нашлось - выбираем
			g_LastSelectedGump = index;
		}

		return LSG;
	}

	return 0;
}
//----------------------------------------------------------------------------
void TGumpWorldMap::OnLeftMouseDown()
{
	if (g_LastSelectedObject == ID_GWM_RESIZE) //Ресайзинг
	{
		m_Resizing = true;
		g_ResizedGump = this;
	}
	else if (g_LastSelectedObject == ID_GWM_MAP) //Карта
		m_MapMoving = true;
	else if (g_LastSelectedObject == ID_GWM_MAP_LIST) //Найстройка карты
		m_OpenedList = 1;
	else if (g_LastSelectedObject == ID_GWM_SCALE_LIST) //Найстройка масштаба
		m_OpenedList = 2;
}
//----------------------------------------------------------------------------
void TGumpWorldMap::OnLeftMouseUp()
{
	if (g_LastObjectLeftMouseDown == ID_GWM_MAP && m_MapMoving) //Карта
	{
		int map = m_Map;

		if (!map)
			map = g_CurrentMap;
		else
			map--;

		if (!m_LinkWithPlayer || g_CurrentMap != map)
		{
			m_OffsetX += (g_MouseX - g_DroppedLeftMouseX);
			m_OffsetY += (g_MouseY - g_DroppedLeftMouseY);
			FixOffsets(m_OffsetX, m_OffsetY, m_Width, m_Height);
		}

		m_MapMoving = false;
	}
	else if (g_LastObjectLeftMouseDown != g_LastSelectedObject)
	{
		int index = g_LastSelectedObject - ID_GWM_LIST - 1;

		if (index >= 0 && index < 7)
		{
			switch (m_OpenedList)
			{
				case 1:
				{
					int mapTest = index;

					if (!mapTest)
						mapTest = g_CurrentMap;
					else
						mapTest--;

					if (g_MapTexture[mapTest] == 0)
						LoadMap(mapTest);

					if (g_MapTexture[mapTest] != 0)
					{
						if (mapTest != m_Map)
						{
							m_OffsetX = 0;
							m_OffsetY = 0;
						}

						m_Map = index;
						m_FrameCreated = false;
					}

					break;
				}
				case 2:
				{
						  ScaleOffsets(index, (m_Width / 2), ((m_Height - 30) / 2));
						  m_FrameCreated = false;

					break;
				}
				default:
					break;
			}
		}
	}
	else
	{
		if (g_LastObjectLeftMouseDown == ID_GWM_RESIZE) //Ресайзинг
		{
			if (m_Resizing)
				ChangeHeight();
		}
		else if (g_LastObjectLeftMouseDown == ID_GWM_MINIMIZE) //Сворачивание
		{
			m_Minimized = true;
			m_FrameCreated = false;
		}
		else if (g_LastObjectLeftMouseDown == ID_GWM_LINK_WITH_PLAYER) //Привязка к координатам игрока
		{
			m_LinkWithPlayer = !m_LinkWithPlayer;
			m_FrameCreated = false;
		}
	}

	m_OpenedList = 0;
}
//----------------------------------------------------------------------------
bool TGumpWorldMap::OnLeftMouseDoubleClick()
{
	bool result = false;

	if (m_Minimized) //При даблклике по мини-гампу - раскрываем его
	{
		m_Minimized = false;
		m_FrameCreated = false;

		result = true;
	}

	return result;
}
//----------------------------------------------------------------------------
void TGumpWorldMap::OnMouseWheel(MOUSE_WHEEL_STATE &state)
{
	//Если доступно для изменения масштаба
	if (!m_Minimized && !g_LeftMouseDown && !g_RightMouseDown && UO->PolygonePixelsInXY(m_X + 8, m_Y + 31, m_Width - 16, m_Height - 16) /*g_LastSelectedObject == ID_GWM_MAP*/)
	{
		int ofs = 0;

		if (state == MWS_UP && m_Scale > 0) //Увеличение
			ofs = -1;
		else if (state == MWS_DOWN && m_Scale < 6) //Уменьшение
			ofs = 1;

		if (ofs)
		{
			int mouseX = (m_Width / 2); //g_MouseX - X + 8;
			int mouseY = ((m_Height - 30) / 2); //g_MouseY - Y + 31;

			ScaleOffsets(m_Scale + ofs, mouseX, mouseY);
			m_FrameCreated = false;
		}
	}
}
//----------------------------------------------------------------------------