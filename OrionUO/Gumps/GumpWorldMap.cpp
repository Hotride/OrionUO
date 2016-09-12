/***********************************************************************************
**
** GumpWorldMap.cpp
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#include "GumpWorldMap.h"
#include "../Managers/MouseManager.h"
#include "../Managers/FileManager.h"
#include "../Managers/MapManager.h"
#include "../Managers/ColorManager.h"
#include "../OrionUO.h"
#include "../PressedObject.h"
#include "../Wisp/WispMappedFile.h"
#include "../Game objects/GamePlayer.h"
//----------------------------------------------------------------------------------
const int m_Scales[7] = { 1, 1, 1, 2, 4, 6, 10 };
//----------------------------------------------------------------------------------
CGumpWorldMap::CGumpWorldMap(uint serial, short x, short y)
: CGump(GT_WORLD_MAP, serial, x, y), m_Width(400), m_Height(300), m_Scale(2),
m_Map(0), m_OffsetX(0), m_OffsetY(0), m_MapMoving(false), m_LinkWithPlayer(true),
m_Called(false), m_CurrentOffsetX(0), m_CurrentOffsetY(0)
{
	m_Page = 2;

	Add(new CGUIPage(1));
	Add(new CGUIGumppic(0x15E8, 0, 0)); //Earth button

	Add(new CGUIPage(2));

	Add(new CGUIButton(ID_GWM_MINIMIZE, 0x082D, 0x082D, 0x082D, (m_Width / 2) - 10, 0)); //Minimize
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

	CGUIComboBox *combo = (CGUIComboBox*)Add(new CGUIComboBox(ID_GWM_MAP_LIST, 0x098D, true, 0x09B5, 0, 0, 200, 7, false));
	combo->TextOffsetY = -4;

	IFOR(i, 0, 7)
		combo->Add(new CGUIComboboxText(0, 6, mapNames[i], 98, TS_CENTER, UOFONT_FIXED));

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

	combo = (CGUIComboBox*)Add(new CGUIComboBox(ID_GWM_SCALE_LIST, 0x098D, true, 0x09B5, 110, 0, 200, 7, false));
	combo->TextOffsetY = -4;

	IFOR(i, 0, 7)
		combo->Add(new CGUIComboboxText(0, 6, scaleNames[i], 36, TS_CENTER, UOFONT_FIXED));

	//Link with player checkbox settings
	m_Text = (CGUIText*)Add(new CGUIText(0x03B2, 0, 0));
	m_Text->CreateTextureA(3, "Link with player");
	m_Text->X = m_Width - m_Text->m_Texture.Width;

	m_Checkbox = (CGUICheckbox*)Add(new CGUICheckbox(ID_GWM_LINK_WITH_PLAYER, 0x00D2, 0x00D3, 0x00D2, m_Text->X - 26, 2));
	
	m_Scissor = (CGUIScissor*)Add(new CGUIScissor(true, 0, 0, 8, 32, m_Width - 16, m_Height - 16));

	m_MapData = (CGUIWorldMapTexture*)Add(new CGUIWorldMapTexture(0, 0));
	m_MapData->Index = m_Map;

	int width = 0;
	int height = 0;
	int playerX = g_Player->X;
	int playerY = g_Player->Y;

	GetScaledDimensions(width, height, playerX, playerY);

	m_MapData->Width = width;
	m_MapData->Height = height;

	Add(new CGUIScissor(false));

	/*//Player drawing
	if (g_CurrentMap == map)
	{
		glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
		g_GL.DrawPolygone(posX + offsetX + playerX + 0, posY + offsetY + playerY + 30, 16, 2);
		g_GL.DrawPolygone(posX + offsetX + playerX + 7, posY + offsetY + playerY + 23, 2, 16);
		g_GL.DrawCircle(posX + offsetX + 8.0f + playerX, posY + offsetY + 31.0f + playerY, 3.0f);
	}*/
}
//----------------------------------------------------------------------------------
CGumpWorldMap::~CGumpWorldMap()
{
}
//---------------------------------------------------------------------------
void CGumpWorldMap::CalculateGumpState()
{
	CGump::CalculateGumpState();

	if (g_GumpPressed)
	{
		if (g_PressedObject.LeftObject() != NULL && ((CBaseGUI*)g_PressedObject.LeftObject())->Type == GOT_COMBOBOX)
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
	}
}
//----------------------------------------------------------------------------------
void CGumpWorldMap::GetCurrentCenter(int &x, int &y, int &mouseX, int &mouseY)
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
//----------------------------------------------------------------------------------
void CGumpWorldMap::ScaleOffsets(int newScale, int mouseX, int mouseY)
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
//----------------------------------------------------------------------------------
void CGumpWorldMap::GetScaledDimensions(int &width, int &height, int &playerX, int &playerY)
{
	int map = m_Map;

	if (!map)
		map = g_CurrentMap;
	else
		map--;
	
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
	if (g_MapTexture[map].Texture == 0)
	{
		WISP_FILE::CMappedFile file;

		bool foundInTable = false;

		const int mapsInfoFileSize = sizeof(uint)* 2 * 6;
		uchar mapsInfoData[mapsInfoFileSize] = { 0 };

		uint dataSize = g_MapSize[map].Width * g_MapSize[map].Height;
		pushort data = NULL;

		uint mapHash = 0;
		uint staticsHash = 0;
		uint mulMapHash = 0;
		uint mulStaticsHash = 0;

		char pathBuf[50] = { 0 };
		sprintf(pathBuf, "mapImage%i.cuo", map);

		if (file.Load(g_App.FilePath("MapsInfo.cuo")) && file.Size)
		{
			uint hashSizeContent = (g_MapSize[map].Width << 16) | g_MapSize[map].Height;

			mulMapHash = g_Orion.GetFileHashCode((uint)g_FileManager.m_MapMul[map].Start, g_FileManager.m_MapMul[map].Size);
			mulMapHash ^= hashSizeContent;
			mulStaticsHash = g_Orion.GetFileHashCode((uint)g_FileManager.m_StaticIdx[map].Start, g_FileManager.m_StaticIdx[map].Size);
			mulStaticsHash ^= g_Orion.GetFileHashCode((uint)g_FileManager.m_StaticMul[map].Start, g_FileManager.m_StaticMul[map].Size);
			mulStaticsHash ^= hashSizeContent;

			if (file.Size != 0 && file.Size != (dataSize * 2))
			{
				memcpy(&mapsInfoData[0], file.Start, mapsInfoFileSize);
				mapHash = file.ReadUInt32LE();
				staticsHash = file.ReadUInt32LE();

				if ((mapHash && mapHash == mulMapHash) && (staticsHash && staticsHash == mulStaticsHash))
				{
					WISP_FILE::CMappedFile mapFile;

					if (mapFile.Load(g_App.FilePath(pathBuf)) && mapFile.Size)
					{
						data = (pushort)mapFile.Start;
						g_GL.BindTexture16(g_MapTexture[map].Texture, g_MapSize[map].Width, g_MapSize[map].Height, data);

						foundInTable = true;
					}

					mapFile.Unload();
				}
			}
		}

		file.Unload();

		if (!foundInTable && g_FileManager.m_MapMul[map].Size != 0 && g_FileManager.m_StaticIdx[map].Size != 0 && g_FileManager.m_StaticMul[map].Size != 0)
		{
			puint dataPtr = (puint)(mapsInfoData + ((sizeof(uint) * 2) * (map + 1)));

			*dataPtr++ = mulMapHash;
			*dataPtr = mulStaticsHash;

			data = new ushort[dataSize];

			IFOR(bx, 0, g_MapBlockSize[map].Width)
			{
				IFOR(by, 0, g_MapBlockSize[map].Height)
				{
					MAP_BLOCK mb = {0};
					g_MapManager->GetWorldMapBlock(map, bx, by, mb);
				
					int mapX = bx * 8;
					int mapY = by * 8;

					IFOR(x, 0, 8)
					{
						IFOR(y, 0, 8)
						{
							int px = mapX + x;
							int py = mapY + y;
							
							ushort color = mb.Cells[(y * 8) + x].TileID;
							ushort pcl = 0x8000 | g_ColorManager.GetRadarColorData(color);
							int block = py * g_MapSize[map].Width + px;
							data[block] = pcl;

							if (x < 7 && y < 7)
								data[block + 1] = pcl;
						}
					}
				}
			}

			g_GL.BindTexture16(g_MapTexture[map].Texture, g_MapSize[map].Width, g_MapSize[map].Height, data);

			FILE *mapDataFile = fopen(g_App.FilePath(pathBuf).c_str(), "wb");

			if (mapDataFile != NULL)
			{
				fwrite(&data[0], 2, dataSize, mapDataFile);
				fclose(mapDataFile);
			}

			FILE *mapsInfoFile = fopen(g_App.FilePath("MapsInfo.cuo").c_str(), "wb");

			if (mapsInfoFile != NULL)
			{
				fwrite(&mapsInfoData[0], mapsInfoFileSize, 1, mapsInfoFile);
				fclose(mapsInfoFile);
			}

			delete data;
		}
	}
}
//----------------------------------------------------------------------------------
void CGumpWorldMap::PrepareContent()
{
	m_CurrentOffsetX = m_OffsetX;
	m_CurrentOffsetY = m_OffsetY;

	int map = m_Map;

	if (!map)
		map = g_CurrentMap;
	else
		map--;

	LoadMap(map);

	if (m_LinkWithPlayer && g_CurrentMap == map && g_Player != NULL)
	{
		m_CurrentOffsetX = (m_Width / 2) - g_Player->X;

		if (m_CurrentOffsetX > 0)
			m_CurrentOffsetX = 0;

		m_CurrentOffsetY = ((m_Height - 30) / 2) - g_Player->Y;

		if (m_CurrentOffsetY > 0)
			m_CurrentOffsetY = 0;

		FixOffsets(m_CurrentOffsetY, m_CurrentOffsetY, m_Width, m_Height);

		if (m_OffsetX != m_CurrentOffsetX || m_OffsetY != m_CurrentOffsetY)
		{
			m_OffsetX = m_CurrentOffsetX;
			m_OffsetY = m_CurrentOffsetY;

			m_WantRedraw = true;

			m_MapData->OffsetX = m_CurrentOffsetX;
			m_MapData->OffsetY = m_CurrentOffsetY;
		}
	}
	else if (m_MapMoving && (!m_LinkWithPlayer || g_CurrentMap != map)) //Если активировано изменение положения карты
	{
		int oldX = m_CurrentOffsetX;
		int oldY = m_CurrentOffsetY;

		WISP_GEOMETRY::CPoint2Di offset = g_MouseManager.LeftDroppedOffset();

		m_CurrentOffsetX += offset.X;
		m_CurrentOffsetY += offset.Y;

		if (m_CurrentOffsetX > 0)
			m_CurrentOffsetX = 0;

		if (m_CurrentOffsetY > 0)
			m_CurrentOffsetY = 0;

		FixOffsets(m_CurrentOffsetX, m_CurrentOffsetY, m_Width, m_Height);

		if (oldX != m_CurrentOffsetX || oldY != m_CurrentOffsetY)
		{
			m_WantRedraw = true;

			m_MapData->OffsetX = m_CurrentOffsetX;
			m_MapData->OffsetY = m_CurrentOffsetY;
		}
	}
}
//----------------------------------------------------------------------------------
/*int CGumpWorldMap::Draw(bool &mode)
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
			Orion->DrawGump(0x15E8, 0, posX, posY); //Earth button

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

		Orion->DrawGump(0x082D, 0, posX + (width / 2) - 10, posY); //Minimize

		Orion->DrawResizepicGump(0x0A3C, posX, posY + 23, width, height);

		//Ресайзер
		WORD resizeGumpID = 0x0837 + (int)(CanPressedButton == 2);

		Orion->DrawGump(resizeGumpID, 0, posX + width - 8, posY + 23 + height - 10);
		
		//Map settings
		Orion->DrawGump(0x098D, 0, posX, posY);
		m_TextMap[m_Map].Draw(posX + 4, posY);
		Orion->DrawGump(0x0985, 0, posX + 94, posY + 7);

		//Scale settings
		TTextureObject *g = Orion->ExecuteGump(0x098B);
		if (g != NULL)
		{
			int tmpX = posX + 110;
			g_GL.Draw(g->Texture, tmpX, posY, 46, g->Height);
		}
		else
			Orion->DrawGump(0x098B, 0, posX + 110, posY);

		m_TextScale[m_Scale].Draw(posX + 114, posY);
		Orion->DrawGump(0x0985, 0, posX + 142, posY + 7);

		//Link with player checkbox settings
		int drawX = posX + width - m_Text.Width;
		m_Text.Draw(drawX, posY);

		drawX -= 26;
		Orion->DrawGump(0x00D2 + (int)m_LinkWithPlayer, 0, drawX, posY + 2);

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

		/*if (m_OpenedList)
		{
			posY += 12;
			//Top
			Orion->DrawGump(0x09B5, 0, posX - 5, posY - 11);

			int ofs = 0;

			IFOR(i, 0, 7)
			{
				WORD gumpID = 0x09B6 + ofs;
				ofs = (ofs + 1) % 3;

				Orion->DrawGump(gumpID, 0, posX, posY);
				
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
			Orion->DrawGump(0x09B9, 0, posX - 5, posY);
		}
	}
	else //Выбор объектов
	{
		if (m_Minimized)
		{
			if (Orion->GumpPixelsInXY(0x15E8, posX, posY)) //Earth button
			{
				g_LastSelectedObject = 0;
				g_LastSelectedGump = index;
			}

			return 0;
		}

		int LSG = 0;

		//Если выбран основной гамп - меняем глобальный указатель на выбранный гамп на него
		if (Orion->ResizepicPixelsInXY(0x0A3C, posX, posY + 23, width, height))
		{
			g_LastSelectedObject = 0;
			g_LastSelectedGump = index;
		}

		if (Orion->GumpPixelsInXY(0x082D, posX + (width / 2) - 10, posY)) //Minimize
			LSG = ID_GWM_MINIMIZE;
		else if (Orion->GumpPixelsInXY(0x0837, posX + width - 8, posY + 23 + height - 10)) //Ресайзер
			LSG = ID_GWM_RESIZE;
		else if ((!m_LinkWithPlayer || g_CurrentMap != map) && Orion->PolygonePixelsInXY(posX + 8, posY + 31, mapViewWidth, mapViewHeight)) //Карта
			LSG = ID_GWM_MAP;
		else if (Orion->PolygonePixelsInXY(posX, posY, 110, 22)) //Настройки карты
			LSG = ID_GWM_MAP_LIST;
		else if (Orion->PolygonePixelsInXY(posX + 110, posY, 46, 22)) //Настройки масштаба
			LSG = ID_GWM_SCALE_LIST;
		else if (Orion->PolygonePixelsInXY(posX + width - (m_Text.Width + 26), posY, m_Text.Width + 26, 22)) //Настройки привязки к координатам игрока
			LSG = ID_GWM_LINK_WITH_PLAYER;
		
		if (m_OpenedList)
		{
			posY += 12;
			//Top
			Orion->DrawGump(0x09B5, 0, posX - 5, posY - 11);

			int ofs = 0;

			IFOR(i, 0, 7)
			{
				if (Orion->GumpPixelsInXY(0x09B6 + ofs, posX, posY))
					LSG = ID_GWM_LIST + i + 1;
				
				ofs = (ofs + 1) % 3;
				posY += 15;
			}

			//Bottom
			Orion->DrawGump(0x09B9, 0, posX - 5, posY);
		}

		return LSG;
	}

	return 0;
}*/
//----------------------------------------------------------------------------------
void CGumpWorldMap::OnLeftMouseButtonDown()
{
	CGump::OnLeftMouseButtonDown();

	if (g_PressedObject.LeftSerial == ID_GWM_MAP) //Карта
	{
		int map = m_Map;

		if (!map)
			map = g_CurrentMap;
		else
			map--;

		if (!m_LinkWithPlayer || g_CurrentMap != map)
			m_MapMoving = true;
	}
}
//----------------------------------------------------------------------------------
void CGumpWorldMap::GUMP_BUTTON_EVENT_C
{
	if (serial == ID_GWM_MAP && m_MapMoving) //Карта
	{
		int map = m_Map;

		if (!map)
			map = g_CurrentMap;
		else
			map--;

		if (!m_LinkWithPlayer || g_CurrentMap != map)
		{
			WISP_GEOMETRY::CPoint2Di offset = g_MouseManager.LeftDroppedOffset();
			m_OffsetX += offset.X;
			m_OffsetY += offset.Y;
			FixOffsets(m_OffsetX, m_OffsetY, m_Width, m_Height);
		}

		m_MapMoving = false;
	}
	else if (serial == ID_GWM_MINIMIZE) //Сворачивание
	{
		m_Minimized = true;
		m_Page = 1;
	}
}
//----------------------------------------------------------------------------------
void CGumpWorldMap::GUMP_CHECKBOX_EVENT_C
{
	if (serial == ID_GWM_LINK_WITH_PLAYER) //Привязка к координатам игрока
		m_LinkWithPlayer = state;
}
//----------------------------------------------------------------------------------
void CGumpWorldMap::GUMP_COMBOBOX_SELECTION_EVENT_C
{
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
		}
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
	bool result = false;

	if (m_Minimized) //При даблклике по мини-гампу - раскрываем его
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
	//Если доступно для изменения масштаба
	if (!m_Minimized && !g_MouseManager.LeftButtonPressed && !g_MouseManager.RightButtonPressed && g_Orion.PolygonePixelsInXY(m_X + 8, m_Y + 31, m_Width - 16, m_Height - 16))
	{
		int ofs = 0;

		if (up && m_Scale > 0) //Увеличение
			ofs = -1;
		else if (!up && m_Scale < 6) //Уменьшение
			ofs = 1;

		if (ofs)
		{
			int mouseX = (m_Width / 2); //g_MouseX - X + 8;
			int mouseY = ((m_Height - 30) / 2); //g_MouseY - Y + 31;

			ScaleOffsets(m_Scale + ofs, mouseX, mouseY);
			m_WantRedraw = true;
		}
	}
}
//----------------------------------------------------------------------------------
void CGumpWorldMap::UpdateSize()
{
	//Событие изменения габаритов гампа с вложенной корректировкой
	WISP_GEOMETRY::CPoint2Di offset = g_MouseManager.LeftDroppedOffset();

	m_Width = m_StartResizeWidth + offset.X;
	m_Height = m_StartResizeHeight + offset.Y;

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

	m_Background->Width = m_Width;
	m_Background->Height = m_Height;
	m_Resizer->X = m_Width - 8;
	m_Resizer->Y = m_Height + 13;
	m_Text->X = m_Width - m_Text->m_Texture.Width;
	m_Checkbox->X = m_Text->X - 26;
	m_Scissor->Width = m_Width - 16;
	m_Scissor->Height = m_Height - 16;
}
//----------------------------------------------------------------------------------
void CGumpWorldMap::GUMP_RESIZE_START_EVENT_C
{
	m_StartResizeWidth = m_Width;
	m_StartResizeHeight = m_Height;
}
//----------------------------------------------------------------------------------
void CGumpWorldMap::GUMP_RESIZE_EVENT_C
{
	if (m_StartResizeWidth && m_StartResizeHeight)
		UpdateSize();
}
//----------------------------------------------------------------------------------
void CGumpWorldMap::GUMP_RESIZE_END_EVENT_C
{
	m_StartResizeWidth = 0;
	m_StartResizeHeight = 0;
}
//----------------------------------------------------------------------------------
