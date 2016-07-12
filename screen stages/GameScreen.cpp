/****************************************************************************
**
** GameScreen.cpp
**
** Copyright (C) September 2015 Hotride
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
//---------------------------------------------------------------------------
#include "stdafx.h"

TGameScreen *GameScreen = NULL;
RENDER_VARIABLES_FOR_GAME_WINDOW *g_RenderBounds = NULL;
//---------------------------------------------------------------------------
TGameScreen::TGameScreen()
: TBaseScreen(), m_GameWindowMoving(false), m_GameWindowResizing(false),
m_UseLight(false), m_MaxDrawZ(0), m_RenderListSize(1000),
m_RenderListInitalized(false), m_RenderListCount(0),
m_ObjectHandlesCount(0)
{
	m_RenderList = new RENDER_OBJECT_DATA[1000];

	g_RenderBounds = &m_RenderBounds;

	memset(&m_ObjectHandlesList[0], 0, sizeof(OBJECT_HANDLES_DATA) * MAX_OBJECT_HANDLES);
}
//---------------------------------------------------------------------------
TGameScreen::~TGameScreen()
{
	if (m_RenderList != NULL)
	{
		delete m_RenderList;
		m_RenderList = NULL;
	}
}
//---------------------------------------------------------------------------
/*!
Инициализация
@return 
*/
void TGameScreen::Init()
{
	m_GameWindowMoving = false;
	m_GameWindowResizing = false;

	SmoothMonitor.UseSunrise();
	m_SmoothScreenAction = 0;

	ToolTip.SeqIndex = 0;

	//Prepare textures on Game Screen:
	UO->ExecuteGump(0x0588); //Main Screen background
	UO->ExecuteGump(0x157C); //Main Screen
	UO->ExecuteGump(0x15A0); //Main Screen Notes
	UO->ExecuteResizepic(0x13BE); //ActPwd Container
	UO->ExecuteGump(0x058A); //Main Screen Castle?
	UO->ExecuteGumpPart(0x1589, 3); //X gump
	UO->ExecuteGumpPart(0x15A4, 3); //> gump
	UO->ExecuteResizepic(0x0BB8); //Account/Password text field
	UO->ExecuteGumpPart(0x00D2, 2); //Checkbox on / off
}
//---------------------------------------------------------------------------
/*!
Обработка события после плавного затемнения экрана
@param [__in_opt] action Идентификатор действия
@return 
*/
void TGameScreen::ProcessSmoothAction( __in_opt BYTE action)
{
	if (action == 0xFF)
		action = m_SmoothScreenAction;

	if (action == ID_SMOOTH_GS_LOGOUT)
		UO->LogOut();
}
//---------------------------------------------------------------------------
/*!
Инициализация всплывающих подсказок
@return 
*/
void TGameScreen::InitToolTip()
{
	if (!ConfigManager.UseToolTips)
		return;

	FontManager->SetUseHTML(true);
	FontManager->RecalculateWidthByInfo = true;

	if (g_LastSelectedGump)
		GumpManager->OnToolTip();
	else if (g_LastObjectType == SOT_GAME_OBJECT && g_LastSelectedObject)
	{
		TGameObject *obj = World->FindWorldObject(g_LastSelectedObject);

		if (obj != NULL && (obj->NPC || !obj->Locked()) && obj->ClilocMessage.length())
			ToolTip.Set(obj->ClilocMessage, SOT_GAME_OBJECT, g_LastSelectedObject);
	}

	FontManager->RecalculateWidthByInfo = false;
	FontManager->SetUseHTML(false);
}
//---------------------------------------------------------------------------
/*!
Вычисление значений Z координаты для рендера
@param [__out] noDrawRoof Не рисовать крыши и верхние этажи строений
@param [__out] maxGroundZ Максимальная Z координата ландшафта
@return Максимальная Z координата отрисовки
*/
int TGameScreen::GetMaxDrawZ( __out bool &noDrawRoof, __out char &maxGroundZ)
{
	int playerX = g_Player->X;
	int playerY = g_Player->Y;
	int playerZ = g_Player->Z;

	int maxDrawZ = 125; //playerZ + 15;

	int bx = playerX / 8;
	int by = playerY / 8;

	int blockIndex = (bx * g_MapBlockY[g_CurrentMap]) + by;
	TMapBlock *mb = MapManager->GetBlock(blockIndex);
	TMapBlock *mb11 = NULL;

	if (mb != NULL)
	{
		int x = playerX % 8;
		int y = playerY % 8;

		maxDrawZ = 125;
		int pz15 = playerZ + 15;
		int pz5 = playerZ + 5;

		for (TRenderWorldObject *ro = mb->GetRender(x, y); ro != NULL; ro = ro->m_NextXY)
		{
			TLandObject *land = ro->LandObjectPtr();

			if (land != NULL)
			{
				int testZ = land->Z; //MinZ;

				if (pz15 <= testZ)
				{
					maxGroundZ = pz15 + 1;
					maxDrawZ = maxGroundZ;

					break;
				}
			}

			if (!ro->IsStaticObject() && !ro->IsGameObject() && !ro->IsMultiObject())
				continue;

			if (ro->IsGameObject() && ((TGameObject*)ro)->NPC)
				continue;
	
			if (ro->Z >= pz15 && maxDrawZ > ro->Z && !ro->IsRoof() && (ro->IsSurface() || ro->IsImpassable()))
			{
				maxDrawZ = pz15 + 1; // ro->Z;

				noDrawRoof = true;
			}
			else  if (ro->Z > pz5 && (ro->IsRoof() || (ro->IsBackground() && ro->IsSurface())))
			{
				bool canNoRoof = !ro->IsRoof();

				if (!canNoRoof)
				{
					if (mb11 == NULL)
					{
						playerX++;
						playerY++;

						bx = playerX / 8;
						by = playerY / 8;

						blockIndex = (bx * g_MapBlockY[g_CurrentMap]) + by;
						mb11 = MapManager->GetBlock(blockIndex);

						x = playerX % 8;
						y = playerY % 8;
					}
					
					for (TRenderWorldObject *ro11 = mb11->GetRender(x, y); ro11 != NULL; ro11 = ro11->m_NextXY)
					{
						if (!ro11->IsStaticObject() && !ro11->IsGameObject() && !ro11->IsMultiObject())
							continue;

						if (ro11->IsGameObject() && ((TGameObject*)ro11)->NPC)
							continue;
	
						if (ro11->Z > pz5 && ro11->IsRoof())
						{
							canNoRoof = true;
							break;
						}
					}
				}

				if (canNoRoof)
				{
					if (maxDrawZ > pz15)
						maxDrawZ = pz15 + 1;

					noDrawRoof = true;
				}
			}
		}
	}

	return maxDrawZ;
}
//---------------------------------------------------------------------------
/*!
Применение прозрачности крон деревьев в указанных координатах
@param [__in] graphic ндекс картинки дерева
@param [__in] x Координата X дерева
@param [__in] y Координата Y дерева
@param [__in] z Координата Z дерева
@return 
*/
void TGameScreen::ApplyTransparentFoliageToUnion(__in const WORD &graphic, __in const int &x, __in const int &y, __in const int &z)
{
	int bx = x / 8;
	int by = y / 8;

	int blockIndex = (bx * g_MapBlockY[g_CurrentMap]) + by;
	TMapBlock *mb = MapManager->GetBlock(blockIndex);

	if (mb != NULL)
	{
		int tx = x % 8;
		int ty = y % 8;

		for (TRenderWorldObject *obj = mb->GetRender(tx, ty); obj != NULL; obj = obj->m_NextXY)
		{
			if (obj->Graphic == graphic && obj->Z == z)
				obj->StaticGroupObjectPtr()->FoliageTransparentIndex = g_FoliageIndex;
		}
	}
}
//---------------------------------------------------------------------------
/*!
Проверка принадлежности кроны к группе крон (с последующим применением прозрачности всей группе)
@param [__in] graphic Индекс картинки дерева
@param [__in] x Координата X дерева
@param [__in] y Координата Y дерева
@param [__in] z Координата Z дерева
@return 
*/
void TGameScreen::CheckFoliageUnion( __in WORD graphic, __in int x, __in int y, __in int z)
{
	graphic -= 0x4000;

	IFOR(i, 0, TREE_COUNT)
	{
		const TREE_UNIONS &info = TREE_INFO[i];

		if (info.GraphicStart <= graphic && graphic <= info.GraphicEnd)
		{
			while (graphic > info.GraphicStart)
			{
				graphic--;
				x--;
				y++;
			}

			for (graphic = info.GraphicStart; graphic <= info.GraphicEnd; graphic++, x++, y--)
				ApplyTransparentFoliageToUnion(graphic + 0x4000, x, y, z);

			break;
		}
	}
}
//---------------------------------------------------------------------------
/*!
Функция увеличения размера списка рендера
@return 
*/
void TGameScreen::IncreaseRenderList()
{
	RENDER_OBJECT_DATA *list = new RENDER_OBJECT_DATA[m_RenderListSize + 1000];

	if (list != NULL)
	{
		memcpy(&list[0], &m_RenderList[0], sizeof(RENDER_OBJECT_DATA) * m_RenderListSize);

		delete m_RenderList;
		m_RenderList = list;

		m_RenderListSize += 1000;
	}
}
//---------------------------------------------------------------------------
/*!
Расчет списка объектов рендера, вычисление прозрачности крон деревьев (в т.ч. составных)
@return 
*/
void TGameScreen::CalculateRenderList()
{
	g_FoliageIndex++;

	if (g_FoliageIndex >= 100)
		g_FoliageIndex = 1;

	m_ObjectHandlesCount = 0;
	m_RenderListCount = 0;
	int objectHandlesOffsetX = g_ObjectHandlesWidth / 2;
	bool useObjectHandles = (!g_GrayedPixels && ConfigManager.ObjectHandles && g_ShiftPressed && g_CtrlPressed);

	static BYTE renderIndex = 1;

#if UO_CHECKERBOARD_SEQUENCE_RENDER_LIST == 1
	int minX = m_RenderBounds.RealMinRangeX;
	int minY = m_RenderBounds.RealMinRangeY;

	int maxX = m_RenderBounds.RealMaxRangeX;
	int maxY = m_RenderBounds.RealMaxRangeY;

	IFOR(i, 0, 2)
	{
		int minValue = minY;
		int maxValue = maxY;

		if (i)
		{
			minValue = minX;
			maxValue = maxX;
		}

		IFOR(lead, minValue, maxValue)
		{
			int x = minX;
			int y = lead;

			if (i)
			{
				x = lead;
				y = maxY;
			}

			while (true)
			{
				if (x < minX || x > maxX || y < minY || y > maxY)
					break;
				else
				{
					int offsetX = x - m_RenderBounds.PlayerX;
					int offsetY = y - m_RenderBounds.PlayerY;

					int drawX = m_RenderBounds.GameWindowCenterX + (offsetX - offsetY) * 22;
					int drawY = m_RenderBounds.GameWindowCenterY + (offsetX + offsetY) * 22;

					if (drawX >= m_RenderBounds.MinPixelsX && drawX <= m_RenderBounds.MaxPixelsX)
					{
						int blockX = x / 8;
						int blockY = y / 8;

						int blockIndex = (blockX * g_MapBlockY[g_CurrentMap]) + blockY;

						TMapBlock *block = MapManager->GetBlock(blockIndex);

						if (block == NULL)
						{
							block = MapManager->AddBlock(blockIndex);
							block->X = blockX;
							block->Y = blockY;
							MapManager->LoadBlock(block);
						}

						AddTileToRenderList(block->GetRender(x % 8, y % 8), drawX, drawY, x, y, renderIndex, useObjectHandles, objectHandlesOffsetX);
					}
				}

				x++;
				y--;
			}
		}
	}
#else
	for (int bx = m_RenderBounds.MinBlockX; bx <= m_RenderBounds.MaxBlockX; bx++)
	{
		for (int by = m_RenderBounds.MinBlockY; by <= m_RenderBounds.MaxBlockY; by++)
		{
			int blockIndex = (bx * g_MapBlockY[g_CurrentMap]) + by;

			TMapBlock *mb = MapManager->GetBlock(blockIndex);

			if (mb == NULL)
			{
				mb = MapManager->AddBlock(blockIndex);
				mb->X = bx;
				mb->Y = by;
				MapManager->LoadBlock(mb);
			}

			IFOR(x, 0, 8)
			{
				int currentX = bx * 8 + x;

				if (currentX < m_RenderBounds.RealMinRangeX || currentX > m_RenderBounds.RealMaxRangeX)
					continue;

				int offsetX = currentX - m_RenderBounds.PlayerX;

				IFOR(y, 0, 8)
				{
					int currentY = by * 8 + y;

					if (currentY < m_RenderBounds.RealMinRangeY || currentY > m_RenderBounds.RealMaxRangeY)
						continue;

					int offsetY = currentY - m_RenderBounds.PlayerY;

					int drawX = m_RenderBounds.GameWindowCenterX + (offsetX - offsetY) * 22;
					int drawY = m_RenderBounds.GameWindowCenterY + (offsetX + offsetY) * 22;

					if (drawX < m_RenderBounds.MinPixelsX || drawX > m_RenderBounds.MaxPixelsX)
						continue;

					AddTileToRenderList(mb->GetRender(x, y), drawX, drawY, currentX, currentY, renderIndex, useObjectHandles, objectHandlesOffsetX);
				}
			}
		}
	}
#endif

	if (m_ObjectHandlesCount > MAX_OBJECT_HANDLES)
		m_ObjectHandlesCount = MAX_OBJECT_HANDLES;

#if UO_RENDER_LIST_SORT == 1
	renderIndex++;

	if (renderIndex >= 100)
		renderIndex = 1;
#endif
}
//---------------------------------------------------------------------------
void TGameScreen::AddTileToRenderList(TRenderWorldObject *obj, const int &drawX, const int &drawY, const int &worldX, const int &worldY, const BYTE &renderIndex, const bool &useObjectHandles, const int &objectHandlesOffsetX, const int &maxZ)
{
	WORD grayColor = 0;

	if (ConfigManager.GrayOutOfRangeObjects)
	{
		POINT testPos = { worldX, worldY };

		if (GetDistance(g_Player, testPos) > g_UpdateRange)
			grayColor = 0x0386;
	}

	//if (maxZ != 150)
	//	grayColor = 0x0021;

	for (; obj != NULL; obj = obj->m_NextXY)
	{
#if UO_RENDER_LIST_SORT == 1
		if (obj->CurrentRenderIndex == renderIndex)
			continue;

		int maxObjectZ = obj->Z + obj->RenderQueueIndex;

		TRenderStaticObject *rso = obj->StaticGroupObjectPtr();

		if (rso != NULL)
		{
			if (rso->IsGameObject() && ((TGameObject*)rso)->NPC)
				maxObjectZ += DEFAULT_CHARACTER_HEIGHT;
			else
				maxObjectZ += rso->GetStaticHeight();
		}

		if (maxObjectZ >= maxZ)
			break;

		obj->CurrentRenderIndex = renderIndex;
#endif

		int z = obj->Z;

		if (obj->IsInternal() || (!obj->IsLandObject() && z >= m_MaxDrawZ))
			continue;

		int testMinZ = drawY;
		int testMaxZ = drawY - (z * 4);

		TLandObject *land = obj->LandObjectPtr();

		if (land != NULL && land->IsStretched)
			testMinZ -= (land->MinZ * 4);
		else
			testMinZ = testMaxZ;

		if (testMinZ < m_RenderBounds.MinPixelsY || testMaxZ > m_RenderBounds.MaxPixelsY)
			continue;

		if (obj->IsGameObject())
		{
			TGameObject *go = (TGameObject*)obj;

			if (((!go->Locked() && go->Graphic < 0x4000) || go->NPC) && useObjectHandles) // && m_ObjectHandlesCount < MAX_OBJECT_HANDLES)
			{
				int index = m_ObjectHandlesCount % MAX_OBJECT_HANDLES;

				m_ObjectHandlesList[index].Obj = go;
				m_ObjectHandlesList[index].X = drawX - objectHandlesOffsetX;
				m_ObjectHandlesList[index].Y = drawY - (go->Z * 4);

				if (go->NPC)
				{
					TGameCharacter *character = go->GameCharacterPtr();

					ANIMATION_DIMENSIONS dims = AnimationManager->GetAnimationDimensions(go);

					m_ObjectHandlesList[index].X += character->OffsetX;
					m_ObjectHandlesList[index].Y += character->OffsetY - (character->OffsetZ + dims.Height + dims.CenterY);
				}
				else
				{
					POINT p = { 0 };
					UO->GetArtDimension(go->Graphic + 0x4000, p);
					m_ObjectHandlesList[index].Y -= p.y;
				}

				m_ObjectHandlesCount++;
			}

#if UO_RENDER_LIST_SORT == 1
			if (go->NPC || go->IsCorpse())
				AddOffsetCharacterTileToRenderList(go, drawX, drawY, renderIndex, useObjectHandles, objectHandlesOffsetX);
#endif
		}
		else if (obj->IsFoliage() && obj->StaticGroupObjectPtr()->FoliageTransparentIndex != g_FoliageIndex)
		{
			char index = 0;

			bool foliageCanBeChecked = (m_RenderBounds.PlayerX <= worldX && m_RenderBounds.PlayerY <= worldY);

			if (!foliageCanBeChecked)
			{
				foliageCanBeChecked = (m_RenderBounds.PlayerY <= worldY && m_RenderBounds.PlayerX <= worldX + 1);

				if (!foliageCanBeChecked)
					foliageCanBeChecked = (m_RenderBounds.PlayerX <= worldX && m_RenderBounds.PlayerY <= worldY + 1);
			}

			if (foliageCanBeChecked)
			{
				POINT fp = { 0, 0 };
				UO->GetArtDimension(obj->Graphic, fp);

				TImageBounds fib(drawX - fp.x / 2, drawY - fp.y - (z * 4), fp.x, fp.y);

				if (fib.InRect(g_PlayerRect))
				{
					RECT realRect = { 0 };
					UO->GetStaticArtRealPixelDimension(obj->Graphic - 0x4000, realRect);

					fib.X += realRect.left;
					fib.Y += realRect.top;
					fib.Width = realRect.right;
					fib.Height = realRect.bottom;

					if (fib.InRect(g_PlayerRect))
					{
						index = g_FoliageIndex;

						CheckFoliageUnion(obj->Graphic, obj->X, obj->Y, z);
					}
				}
			}

			obj->StaticGroupObjectPtr()->FoliageTransparentIndex = index;
		}

		if (m_RenderListCount >= m_RenderListSize)
			IncreaseRenderList();

		m_RenderList[m_RenderListCount].Obj = obj;
		m_RenderList[m_RenderListCount].X = drawX;
		m_RenderList[m_RenderListCount].Y = drawY;
		m_RenderList[m_RenderListCount].GrayColor = grayColor;

		m_RenderListCount++;
	}
}
//---------------------------------------------------------------------------
void TGameScreen::AddOffsetCharacterTileToRenderList(TGameObject *obj, int drawX, int drawY, const BYTE &renderIndex, const bool &useObjectHandles, const int &objectHandlesOffsetX)
{
	int characterX = obj->X;
	int characterY = obj->Y;

	ANIMATION_DIMENSIONS dims = AnimationManager->GetAnimationDimensions(obj);
	TGameCharacter *character = obj->GameCharacterPtr();

	if (character != NULL)
	{
		drawX += character->OffsetX;
		drawY += character->OffsetY - character->OffsetZ; // (character->OffsetZ + dims.Height + dims.CenterY);
	}

	vector<pair<int, int>> coordinates;

	coordinates.push_back(pair<int, int>(characterX, characterY + 1));
	coordinates.push_back(pair<int, int>(characterX, characterY + 2));

	coordinates.push_back(pair<int, int>(characterX + 1, characterY - 2));
	coordinates.push_back(pair<int, int>(characterX + 1, characterY - 1));
	coordinates.push_back(pair<int, int>(characterX + 1, characterY));
	coordinates.push_back(pair<int, int>(characterX + 1, characterY + 1));
	coordinates.push_back(pair<int, int>(characterX + 1, characterY + 2));

	coordinates.push_back(pair<int, int>(characterX + 2, characterY - 2));
	coordinates.push_back(pair<int, int>(characterX + 2, characterY - 1));
	coordinates.push_back(pair<int, int>(characterX + 2, characterY));
	coordinates.push_back(pair<int, int>(characterX + 2, characterY + 1));
	coordinates.push_back(pair<int, int>(characterX + 2, characterY + 2));

	int size = coordinates.size();

	int maxZ = obj->Z + obj->RenderQueueIndex;

	IFOR(i, 0, size)
	{
		int x = coordinates[i].first;

		if (x < m_RenderBounds.RealMinRangeX || x > m_RenderBounds.RealMaxRangeX)
			continue;

		int y = coordinates[i].second;

		if (y < m_RenderBounds.RealMinRangeY || y > m_RenderBounds.RealMaxRangeY)
			continue;

		int offsetX = x - m_RenderBounds.PlayerX;
		int offsetY = y - m_RenderBounds.PlayerY;

		int drawX = m_RenderBounds.GameWindowCenterX + (offsetX - offsetY) * 22;
		int drawY = m_RenderBounds.GameWindowCenterY + (offsetX + offsetY) * 22;

		if (drawX < m_RenderBounds.MinPixelsX || drawX > m_RenderBounds.MaxPixelsX)
			continue;

		int blockX = x / 8;
		int blockY = y / 8;

		int blockIndex = (blockX * g_MapBlockY[g_CurrentMap]) + blockY;

		TMapBlock *block = MapManager->GetBlock(blockIndex);

		if (block == NULL)
		{
			block = MapManager->AddBlock(blockIndex);
			block->X = blockX;
			block->Y = blockY;
			MapManager->LoadBlock(block);
		}

		AddTileToRenderList(block->GetRender(x % 8, y % 8), drawX, drawY, x, y, renderIndex, useObjectHandles, objectHandlesOffsetX, maxZ);
	}
}
//---------------------------------------------------------------------------
/*!
Вычисление параметров игрового окна
@return 
*/
void TGameScreen::CalculateGameWindowBounds()
{
	m_RenderBounds.PlayerX = g_Player->X;
	m_RenderBounds.PlayerY = g_Player->Y;
	m_RenderBounds.PlayerZ = g_Player->Z;

	m_RenderBounds.GameWindowPosX = g_GameWindowPosX;
	m_RenderBounds.GameWindowPosY = g_GameWindowPosY;

	m_RenderBounds.GameWindowSizeX = g_GameWindowWidth;
	m_RenderBounds.GameWindowSizeY = g_GameWindowHeight;

	if (g_LastObjectType == SOT_GAME_GUMP_SCOPE && g_LeftMouseDown && !g_LastGumpLeftMouseDown && !g_LastGumpRightMouseDown)
	{
		if (g_LastObjectLeftMouseDown == 2)
		{
			m_RenderBounds.GameWindowSizeX += (g_MouseX - g_DroppedLeftMouseX);
			m_RenderBounds.GameWindowSizeY += (g_MouseY - g_DroppedLeftMouseY);

			if (m_RenderBounds.GameWindowSizeX < 640)
				m_RenderBounds.GameWindowSizeX = 640;

			if (m_RenderBounds.GameWindowSizeY < 480)
				m_RenderBounds.GameWindowSizeY = 480;

			TGumpOptions *opt = (TGumpOptions*)GumpManager->UpdateGump(g_PlayerSerial, 0, GT_OPTIONS);

			if (opt != NULL)
			{
				opt->TextEntryGameSizeX->SetText(std::to_string(m_RenderBounds.GameWindowSizeX));
				opt->TextEntryGameSizeY->SetText(std::to_string(m_RenderBounds.GameWindowSizeY));
			}
		}

		if (g_LastObjectLeftMouseDown == 1)
		{
			m_RenderBounds.GameWindowPosX += (g_MouseX - g_DroppedLeftMouseX);
			m_RenderBounds.GameWindowPosY += (g_MouseY - g_DroppedLeftMouseY);

			if (m_RenderBounds.GameWindowPosX < 0)
				m_RenderBounds.GameWindowPosX = 0;

			if (m_RenderBounds.GameWindowPosY < 0)
				m_RenderBounds.GameWindowPosY = 0;

			if (m_RenderBounds.GameWindowPosX + m_RenderBounds.GameWindowSizeX > g_ClientWidth)
				m_RenderBounds.GameWindowPosX = g_ClientWidth - m_RenderBounds.GameWindowSizeX;

			if (m_RenderBounds.GameWindowPosY + m_RenderBounds.GameWindowSizeY > g_ClientHeight)
				m_RenderBounds.GameWindowPosY = g_ClientHeight - m_RenderBounds.GameWindowSizeY;
		}
	}

	int playerZOffset = (g_Player->Z * 4) - g_Player->OffsetZ;

	m_RenderBounds.GameWindowCenterX = m_RenderBounds.GameWindowPosX + m_RenderBounds.GameWindowSizeX / 2;
	m_RenderBounds.GameWindowCenterY = (m_RenderBounds.GameWindowPosY + m_RenderBounds.GameWindowSizeY / 2) + (g_Player->Z * 4);

	/*int earthquakeMagnitude = RandomInt(11);

	if (earthquakeMagnitude)
	{
		m_RenderBounds.GameWindowCenterX += RandomInt(earthquakeMagnitude * 3);
		m_RenderBounds.GameWindowCenterY += RandomInt(earthquakeMagnitude * 3);
	}*/

	m_RenderBounds.GameWindowCenterX -= (int)g_Player->OffsetX;
	m_RenderBounds.GameWindowCenterY -= (int)(g_Player->OffsetY - g_Player->OffsetZ);

	ANIMATION_DIMENSIONS dims = AnimationManager->GetAnimationDimensions(g_Player);

	g_PlayerRect.X = m_RenderBounds.GameWindowCenterX - (dims.Width / 2);
	g_PlayerRect.Y = m_RenderBounds.GameWindowCenterY - dims.Height - playerZOffset - dims.CenterY;
	g_PlayerRect.Width = dims.Width;
	g_PlayerRect.Height = dims.Height;

	int rangeX = (m_RenderBounds.GameWindowSizeX / 44) + 1;
	int rangeY = (m_RenderBounds.GameWindowSizeY / 44) + 1;

	if (rangeX < rangeY)
		rangeX = rangeY;
	else
		rangeY = rangeX;

	m_RenderBounds.RealMinRangeX = g_Player->X - rangeX;

	if (m_RenderBounds.RealMinRangeX < 0)
		m_RenderBounds.RealMinRangeX = 0;

	m_RenderBounds.RealMaxRangeX = g_Player->X + rangeX;

	if (m_RenderBounds.RealMaxRangeX >= g_MapSizeX[g_CurrentMap])
		m_RenderBounds.RealMaxRangeX = g_MapSizeX[g_CurrentMap];

	m_RenderBounds.RealMinRangeY = g_Player->Y - rangeY;

	if (m_RenderBounds.RealMinRangeY < 0)
		m_RenderBounds.RealMinRangeY = 0;

	m_RenderBounds.RealMaxRangeY = g_Player->Y + rangeY;

	if (m_RenderBounds.RealMaxRangeY >= g_MapSizeY[g_CurrentMap])
		m_RenderBounds.RealMaxRangeY = g_MapSizeY[g_CurrentMap];

	m_RenderBounds.MinBlockX = (m_RenderBounds.RealMinRangeX / 8) - 1;
	m_RenderBounds.MinBlockY = (m_RenderBounds.RealMinRangeY / 8) - 1;
	m_RenderBounds.MaxBlockX = (m_RenderBounds.RealMaxRangeX / 8) + 1;
	m_RenderBounds.MaxBlockY = (m_RenderBounds.RealMaxRangeY / 8) + 1;

	if (m_RenderBounds.MinBlockX < 0)
		m_RenderBounds.MinBlockX = 0;

	if (m_RenderBounds.MinBlockY < 0)
		m_RenderBounds.MinBlockY = 0;

	if (m_RenderBounds.MaxBlockX >= g_MapBlockX[g_CurrentMap])
		m_RenderBounds.MaxBlockX = g_MapBlockX[g_CurrentMap] - 1;

	if (m_RenderBounds.MaxBlockY >= g_MapBlockY[g_CurrentMap])
		m_RenderBounds.MaxBlockY = g_MapBlockY[g_CurrentMap] - 1;

	int drawOffset = 40;

	m_RenderBounds.MinPixelsX = m_RenderBounds.GameWindowPosX - drawOffset;
	m_RenderBounds.MaxPixelsX = m_RenderBounds.GameWindowPosX + m_RenderBounds.GameWindowSizeX + drawOffset;

	m_RenderBounds.MinPixelsY = m_RenderBounds.GameWindowPosY - drawOffset; // -playerZOffset;
	m_RenderBounds.MaxPixelsY = m_RenderBounds.GameWindowPosY + m_RenderBounds.GameWindowSizeY + drawOffset; // + playerZOffset;

	g_NoDrawRoof = false;
	g_MaxGroundZ = 125;
	m_MaxDrawZ = GetMaxDrawZ(g_NoDrawRoof, g_MaxGroundZ);
}
//---------------------------------------------------------------------------
/*!
Обработка сообщений мыши (стягивание статуса. драг-гампа и т.п.)
@return 
*/
void TGameScreen::CheckMouseEvents()
{
	g_GrayedPixels = g_Player->Dead();

	if (g_StatusbarUnderMouse != g_LastSelectedGump)
		g_StatusbarUnderMouse = 0;
	else
	{
		TGump *gsb = GumpManager->GumpExists(g_StatusbarUnderMouse);

		if (gsb != NULL)
			g_StatusbarUnderMouse = gsb->Serial;
		else
			g_StatusbarUnderMouse = 0;
	}

	if (g_LastObjectType == SOT_GAME_OBJECT && g_LastObjectLeftMouseDown && ((g_MouseX != g_DroppedLeftMouseX || g_MouseY != g_DroppedLeftMouseY) || (g_LastMouseDownTime + DCLICK_DELAY < GetTickCount())))
	{
		TGameItem *selobj = World->FindWorldItem(g_LastObjectLeftMouseDown);

		if (selobj != NULL && ObjectInHand == NULL && !selobj->Locked() && GetDistance(g_Player, selobj) < 3)
		{
			if (selobj->Serial >= 0x40000000 && !g_GrayedPixels) //Item selection
			{
				if (selobj->IsStackable() && selobj->Count > 1)
				{
					TGumpDrag *newgump = new TGumpDrag(g_LastObjectLeftMouseDown, g_MouseX - 80, g_MouseY - 34);
						
					char text_amount[20] = {0};
					sprintf(text_amount, "%d", selobj->Count);

					TGump *gumpEntry = GumpManager->GetTextEntryOwner();
						
					EntryPointer = newgump->TextEntry;
					EntryPointer->SetText(text_amount);

					if (gumpEntry != NULL)
						gumpEntry->FrameCreated = false;
	
					GumpManager->AddGump(newgump);
					CurrentScreen->OnLeftMouseDown();
					selobj->Dragged = true;
				}
				else if (!Target.IsTargeting())
				{
					UO->PickupItem(selobj);
					//g_LastGumpLeftMouseDown = 0;
					g_LastObjectLeftMouseDown = 0;
				}
			}
		}
		else if (ObjectInHand == NULL)
		{
			TGameCharacter *selchar = World->FindWorldCharacter(g_LastObjectLeftMouseDown);
			if (selchar!= NULL) //Character selection
			{
				UO->OpenStatus(selchar->Serial);
				CurrentScreen->OnLeftMouseDown();
				g_GeneratedMouseDown = true;
			}
		}
	}
}
//---------------------------------------------------------------------------
/*!
Функция добавления источника света
@param [__in] rwo Верхний объект, источающий свет
@param [__in] lightObject Объект, источающий свет
@param [__in] x Экранная координата X
@param [__in] y Экранная координата Y
@return 
*/
void TGameScreen::AddLight( __in TRenderWorldObject *rwo, __in TRenderWorldObject *lightObject, __in int x, __in int y)
{
	if (lightObject->IsStaticGroupObject())
	{
		STATIC_TILES *st = lightObject->StaticGroupObjectPtr()->GetStaticData();

		if (st->Quality == 0xFF && lightObject->IsPrefixAn())
			return;
	}

	if (m_LightCount < MAX_LIGHT_SOURCES)
	{
		bool canBeAdded = true;
		
		int testX = rwo->X + 1;
		int testY = rwo->Y + 1;
		
		int bx = testX / 8;
		int by = testY / 8;

		int blockIndex = (bx * g_MapBlockY[g_CurrentMap]) + by;
		TMapBlock *mb = MapManager->GetBlock(blockIndex);

		if (mb != NULL)
		{
			bx = testX % 8;
			by = testY % 8;
			
			char z5 = rwo->Z + 5;
		
			for (TRenderWorldObject *obj = mb->GetRender(bx, by); obj != NULL; obj = obj->m_NextXY)
			{
				if (!obj->IsStaticGroupObject() || (obj->IsGameObject() && ((TGameObject*)obj)->NPC))
					continue;

				int z = obj->Z;
	
				if (z >= m_MaxDrawZ)
					continue;
				
				//if (z >= z5 && (obj->IsRoof() || (obj->IsBackground() && obj->IsSurface())))
				if (z >= z5)
				{
					canBeAdded = false;
					break;
				}
			}
		}

		if (canBeAdded)
		{
			m_Light[m_LightCount].ID = lightObject->GetLightID();

			if (ConfigManager.ColoredLighting)
			{
				WORD graphic = lightObject->Graphic;

				if (!lightObject->IsGameObject())
					graphic -= 0x4000;

				m_Light[m_LightCount].Color = UO->GetLightColor(graphic);
			}
			else
				m_Light[m_LightCount].Color = 0;

			m_Light[m_LightCount].DrawX = x;
			m_Light[m_LightCount].DrawY = y;

			m_LightCount++;
		}
	}
}
//---------------------------------------------------------------------------
/*!
Вычисление параметров отображаемого текста
@param [__in] mode true - отрисовка, false - выбор
@return 
*/
void TGameScreen::CalculateGameWindowText( __in bool &mode)
{
	if (mode)
		WorldTextRenderer->ClearRect();

	TRenderTextObject *rto = WorldTextRenderer->m_Items;

	//Вычисление положения и прозрачности текста
	for (; rto != NULL; rto = rto->m_NextDraw)
	{
		if (!rto->IsText())
		{
			if (rto->m_NextDraw == NULL)
				break;

			continue;
		}

		TTextData *td = (TTextData*)rto;
		TTextTexture &tth = td->m_Texture;

		TRenderWorldObject *rwo = NULL;
		int drawX = 0;
		int drawY = 0;

		switch (td->Type)
		{
			case TT_OBJECT:
			{
				TGameObject *go = World->FindWorldObject(td->Serial);

				if (go != NULL && go->Graphic < 0x4000)
				{
					rwo = go;

					int gox = go->X - m_RenderBounds.PlayerX;
					int goy = go->Y - m_RenderBounds.PlayerY;

					drawX = m_RenderBounds.GameWindowCenterX + ((gox - goy) * 22);
					drawY = ((m_RenderBounds.GameWindowCenterY + ((gox + goy) * 22)) - (go->Z * 4));

					if (go->NPC)
					{
						TGameCharacter *gc = go->GameCharacterPtr();

						drawX += gc->OffsetX;
						drawY += gc->OffsetY - gc->OffsetZ;

						ANIMATION_DIMENSIONS dims = AnimationManager->GetAnimationDimensions(go, 0);
						drawY -= (dims.Height + dims.CenterY);

						/*drawY -= 55;

						if (gc->FindLayer(OL_MOUNT) != NULL)
							drawY -= 25;*/
					}
					else
						drawY -= (go->GetStaticData()->Height + 20);

					drawX -= go->GetTextOffsetX(td);
					drawY -= go->GetTextOffsetY(td);
				}

				break;
			}
			case TT_CLIENT:
			{
				rwo = (TRenderWorldObject*)td->Serial;

				if (rwo != NULL)
				{
					int gox = rwo->X - m_RenderBounds.PlayerX;
					int goy = rwo->Y - m_RenderBounds.PlayerY;

					drawX = m_RenderBounds.GameWindowCenterX + ((gox - goy) * 22);
					drawY = ((m_RenderBounds.GameWindowCenterY + ((gox + goy) * 22)) - (rwo->Z * 4));

					WORD gID = rwo->Graphic - 0x4000;

					int height = UO->m_StaticData[gID / 32].Tiles[gID % 32].Height;

					drawY -= (height + 20);

					drawX -= rwo->GetTextOffsetX(td);
					drawY -= rwo->GetTextOffsetY(td);
				}

				break;
			}
			default:
				break;
		}

		if (rwo != NULL)
		{
			td->DrawX = drawX;
			td->DrawY = drawY;

			if (mode)
			{
				TTextImageBounds ib(drawX, drawY, tth.Width, tth.Height, td);

				td->Transparent = WorldTextRenderer->InRect(ib, rwo);

				WorldTextRenderer->AddRect(ib);
			}
		}

		if (rto->m_NextDraw == NULL)
			break;
	}

	m_GameWindowText = rto;
}
//---------------------------------------------------------------------------
/*!
Рисование игрового окна
@param [__in] mode true - отрисовка, false - выбор
@return 
*/
void TGameScreen::DrawGameWindow( __in bool &mode)
{
	DWORD ticks = GetTickCount();
	int playerZPlus5 = m_RenderBounds.PlayerZ + 5;

	if (mode)
	{
		m_UseLight = (g_PersonalLightLevel < g_LightLevel);

		glColor3f(g_DrawColor, g_DrawColor, g_DrawColor);

		AnimationManager->ShadowCount = 0;
		//bool useGrayObjects = ConfigManager.GrayOutOfRangeObjects;

		if (ConfigManager.UseCircleTrans)
		{
			if (g_CircleOfTransparency.Create(ConfigManager.CircleTransRadius))
			{
				int drawX = m_RenderBounds.GameWindowCenterX + g_Player->OffsetX;
				int drawY = m_RenderBounds.GameWindowCenterY + g_Player->OffsetY - (m_RenderBounds.PlayerZ * 4 + g_Player->OffsetZ);

				g_CircleOfTransparency.Draw(drawX, drawY);
			}
		}

		IFOR(i, 0, m_RenderListCount)
		{
			RENDER_OBJECT_DATA &rod = m_RenderList[i];
			TRenderWorldObject *obj = rod.Obj;

			if (obj != NULL)
			{
				g_OutOfRangeColor = rod.GrayColor;

				g_UseCircleTrans = (ConfigManager.UseCircleTrans && obj->TranparentTest(playerZPlus5));

#if UO_DEPTH_TEST == 1
				g_ZBuffer = obj->Z + obj->RenderQueueIndex;
#endif
				obj->Draw(mode, rod.X, rod.Y, ticks);
			}
		}

		//AnimationManager->DrawShadows();

		g_ZBuffer = 0;
		glDisable(GL_DEPTH_TEST);
	}
	else
	{
		bool useCircleTrans = (ConfigManager.UseCircleTrans && UO->CircleTransPixelsInXY());

		IFOR(i, 0, m_RenderListCount)
		{
			RENDER_OBJECT_DATA &rod = m_RenderList[i];
			TRenderWorldObject *obj = rod.Obj;

			if (obj != NULL)
			{
				g_UseCircleTrans = (useCircleTrans && obj->TranparentTest(playerZPlus5));

				obj->Draw(mode, rod.X, rod.Y, ticks);
			}
		}
	}

	if (mode)
	{
		UnuseShader();

		IFOR(i, 0, m_ObjectHandlesCount)
		{
			OBJECT_HANDLES_DATA &ohd = m_ObjectHandlesList[i];
			ohd.Obj->DrawObjectHandlesTexture(mode, ohd.X, ohd.Y);
		}

		if (PluginManager != NULL)
			PluginManager->WorldDraw();
	}
	else
	{
		IFOR(i, 0, m_ObjectHandlesCount)
		{
			OBJECT_HANDLES_DATA &ohd = m_ObjectHandlesList[i];
			ohd.Obj->DrawObjectHandlesTexture(mode, ohd.X, ohd.Y);
		}
	}
}
//---------------------------------------------------------------------------
/*!
Отображение источников света
@return 
*/
void TGameScreen::DrawGameWindowLight()
{
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

	if (!m_UseLight)
		return;

	LightColorizerShader->Use();

	if (g_UseFrameBuffer)
	{
		if (g_LightBuffer.Ready() && g_LightBuffer.Use())
		{
			float newLightColor = ((32 - g_LightLevel + g_PersonalLightLevel) / 32.0f);

			if (!ConfigManager.DarkNights)
				newLightColor += 0.2f;

			glClearColor(newLightColor, newLightColor, newLightColor, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT);
			glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

			glEnable(GL_BLEND);
			glBlendFunc(GL_ONE, GL_ONE);

			IFOR(i, 0, m_LightCount)
			{
				LIGHT_DATA &light = m_Light[i];

				UO->DrawLight(light.ID, light.Color, light.DrawX - m_RenderBounds.GameWindowPosX, light.DrawY - m_RenderBounds.GameWindowPosY);
			}

			UnuseShader();

			g_LightBuffer.Release();

			g_GL.RestorePort();

			g_GL.ViewPort(m_RenderBounds.GameWindowPosX, m_RenderBounds.GameWindowPosY, m_RenderBounds.GameWindowSizeX, m_RenderBounds.GameWindowSizeY);

			glBlendFunc(GL_ZERO, GL_SRC_COLOR);

			g_LightBuffer.Draw(m_RenderBounds.GameWindowPosX, m_RenderBounds.GameWindowPosY);

			glDisable(GL_BLEND);
		}
	}
	else
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_ONE, GL_ONE);

		IFOR(i, 0, m_LightCount)
		{
			LIGHT_DATA &light = m_Light[i];

			UO->DrawLight(light.ID, light.Color, light.DrawX - m_RenderBounds.GameWindowPosX, light.DrawY - m_RenderBounds.GameWindowPosY);
		}

		glDisable(GL_BLEND);
	}

	UnuseShader();
}
//---------------------------------------------------------------------------
/*!
Отображение текста над объектами мира
@param [__in] mode true - отрисовка, false - выбор
@return 
*/
void TGameScreen::DrawGameWindowText( __in bool &mode)
{
	DWORD ticks = GetTickCount();

	if (mode)
	{
		/*for (TRenderTextObject *bgwt = m_GameWindowText; bgwt != NULL; bgwt = bgwt->m_PrevDraw)
		{
			if (!bgwt->IsText())
				continue;

			TTextData *td = (TTextData*)bgwt;

			if (td->Type != TT_SYSTEM)
			{
				TTextTexture &tth = td->m_Texture;

				if (td->Timer >= ticks)
					DPOLY(td->DrawX, td->DrawY, tth.Width, tth.Height);
			}
		}*/

		FontColorizerShader->Use();

		for (; m_GameWindowText != NULL; m_GameWindowText = m_GameWindowText->m_PrevDraw)
		{
			if (!m_GameWindowText->IsText())
				continue;

			TTextData *td = (TTextData*)m_GameWindowText;

			if (td->Type != TT_SYSTEM)
			{
				TTextTexture &tth = td->m_Texture;

				if (td->Timer >= ticks)
				{
					WORD textColor = td->Color;

					bool isYellowed = (g_LastObjectType == SOT_TEXT_OBJECT && g_SelectedTextObject == m_GameWindowText);

					if (isYellowed)
					{
						TGameObject *textOwner = World->FindWorldObject(td->Serial);

						if (textOwner != NULL && (textOwner->NPC || textOwner->IsCorpse()))
							textColor = 0x0035;
					}

					int drawMode = 0;

					if (textColor)
					{
						ColorManager->SendColorsToShader(textColor);

						if (td->Unicode)
							drawMode = 3;
						else if (td->Font != 5 && td->Font != 8)
							drawMode = 2;
						else
							drawMode = 1;
					}

					glUniform1iARB(ShaderDrawMode, drawMode);

					if (td->Transparent)
					{
						glEnable(GL_BLEND);
						glBlendFunc(GL_SRC_COLOR, GL_SRC_ALPHA);
						glColor4f(1.0f, 1.0f, 1.0f, 0.45f);

						tth.Draw(td->DrawX, td->DrawY);

						glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
						glDisable(GL_BLEND);
					}
					else
						tth.Draw(td->DrawX, td->DrawY);
				}
			}
		}

		UnuseShader();

		QFOR(obj, World->m_Items, TGameObject*)
		{
			if (obj->NPC)
			{
				TGameCharacter *character = obj->GameCharacterPtr();

				TTextContainer *textContainer = character->m_DamageTextControl;

				if (textContainer->m_Top == NULL)
					continue;

				int gox = obj->X - m_RenderBounds.PlayerX;
				int goy = obj->Y - m_RenderBounds.PlayerY;

				int drawX = m_RenderBounds.GameWindowCenterX + ((gox - goy) * 22);
				int drawY = ((m_RenderBounds.GameWindowCenterY + ((gox + goy) * 22)) - (obj->Z * 4));

				drawX += character->OffsetX;
				drawY += character->OffsetY - character->OffsetZ;

				ANIMATION_DIMENSIONS dims = AnimationManager->GetAnimationDimensions(character, 0);
				drawY -= (dims.Height + dims.CenterY);

				/*drawY -= 55;

				if (character->FindLayer(OL_MOUNT) != NULL)
					drawY -= 25;*/

				for (TTextData *text = textContainer->m_Top; text != NULL; )
				{
					TTextData *next = text->m_Next;

					if (text->m_Texture.Empty())
					{
						text = next;
						continue;
					}

					if (text->Timer < ticks)
					{
						if (text->Transparent)
						{
							textContainer->Delete(text);
							text = next;
							continue;
						}
						else
						{
							text->Timer = ticks + DAMAGE_TEXT_TRANSPARENT_DELAY;
							text->Transparent = true;
							text->Color = 0x00FF;
						}
					}

					if (text->Transparent)
					{
						glEnable(GL_BLEND);
						glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

						BYTE alpha = (BYTE)text->Color;
						if (alpha >= DAMAGE_TEXT_ALPHA_STEP)
							text->Color -= DAMAGE_TEXT_ALPHA_STEP;
						else
							text->Color = 0;

						glColor4ub(0xFF, 0xFF, 0xFF, alpha);

						text->m_Texture.Draw(drawX - text->DrawX, drawY + text->DrawY);

						glEnable(GL_BLEND);
						glColor4ub(0xFF, 0xFF, 0xFF, 0xFF);
					}
					else
						text->m_Texture.Draw(drawX - text->DrawX, drawY + text->DrawY);

					text->DrawY -= DAMAGE_TEXT_STEP;

					text = next;
				}
			}
		}
	}
	else
	{
		for (; m_GameWindowText != NULL; m_GameWindowText = m_GameWindowText->m_PrevDraw)
		{
			if (!m_GameWindowText->IsText())
				continue;

			TTextData *td = (TTextData*)m_GameWindowText;

			if (td->Type != TT_SYSTEM)
			{
				if (td->Timer >= ticks)
				{
					TTextTexture &tth = td->m_Texture;

					if (tth.UnderMouse(td->DrawX, td->DrawY))
					{
						g_LastObjectType = SOT_TEXT_OBJECT;
						g_LastSelectedObject = 4;
						g_SelectedTextObject = m_GameWindowText;
					}
				}
			}
		}
	}
}
//---------------------------------------------------------------------------
/*!
Отрисовка/выбор объектов
@param [__in] mode true - отрисовка, false - выбор
@return При выборе объектов - идентификатор выбранного объекта
*/
int TGameScreen::Render(__in bool mode)
{
	DWORD ticks = GetTickCount();

	if (g_LastRenderTime > ticks)
	{
		if (mode || !g_SelectGumpObjects)
			return 0;
	}

	if (!m_RenderListInitalized)
		CalculateRenderList();

	if (g_DeathScreenTimer < ticks)
	{
		if (g_DeathScreenTimer && SmoothMonitor.UseSunrise())
			m_SmoothScreenAction = 0;

		g_DeathScreenTimer = 0;
	}

	//Вычисление положения и прозрачности текста
	CalculateGameWindowText(mode);

	if (mode)
	{
		static DWORD lastRender = GetTickCount() + 1000;
		static int currentFPS = 0;
		static int FPScount = 0;

		if (lastRender < ticks)
		{
			FPScount = currentFPS;
			currentFPS = 0;
			lastRender = ticks + 1000;
		}
		else
			currentFPS++;

		g_LastRenderTime = ticks + g_FrameDelay[(int)(GetForegroundWindow() == g_hWnd)];

		bool RMD = false;
		bool LMD = false;
		int LGLMD = 0;
		int LOLMD = 0;

		if (g_GameState == GS_GAME_BLOCKED)
		{
			RMD = g_RightMouseDown;
			g_RightMouseDown = false;
			LMD = g_LeftMouseDown;
			g_LeftMouseDown = false;
			g_LastObjectType = SOT_NO_OBJECT;
			g_LastSelectedObject = 0;

			LGLMD = g_LastGumpLeftMouseDown;
			LOLMD = g_LastObjectLeftMouseDown;

			g_LastObjectLeftMouseDown = 0;
			g_LastObjectRightMouseDown = 0;
			g_LastSelectedGump = 0;
			g_LastGumpLeftMouseDown = 0;
			g_LastGumpRightMouseDown = 0;
		}

		bool multiOnTarget = false;

		if (Target.IsTargeting() && Target.MultiGraphic && g_SelectedObject != NULL)
		{
			multiOnTarget = true;
			int grZ = 0;
			int stZ = 0;
			MapManager->GetMapZ(g_SelectedObject->X, g_SelectedObject->Y, grZ, stZ);
			Target.LoadMulti(g_SelectedObject->X, g_SelectedObject->Y, grZ);
		}

		g_RenderedObjectsCountInGameWindow = 0;
		
		CheckMouseEvents();
		
		TargetGump.Color = 0;
		AttackTargetGump.Color = 0;
		NewTargetSystem.ColorGump = 0;

		m_LightCount = 0;

		g_GL.BeginDraw();

		if (DrawSmoothMonitor())
			return 0;

		g_GL.ViewPort(m_RenderBounds.GameWindowPosX, m_RenderBounds.GameWindowPosY, m_RenderBounds.GameWindowSizeX, m_RenderBounds.GameWindowSizeY);
		
		g_DrawColor = 1.0f;

		if (!g_UseFrameBuffer && g_PersonalLightLevel < g_LightLevel)
		{
			g_DrawColor = ((32 - g_LightLevel + g_PersonalLightLevel) / 32.0f);

			if (!ConfigManager.DarkNights)
				g_DrawColor += 0.2f;
		}

		if (g_GrayedPixels)
			CurrentShader = DeathShader;
		else
			CurrentShader = ColorizerShader;

		CurrentShader->Use();

		DrawGameWindow(mode);

		UnuseShader();

		if (!g_DeathScreenTimer)
		{
			if (!g_GrayedPixels)
			{
				DrawGameWindowLight();

				ColorizerShader->Use();

				NewTargetSystem.Draw();
				TargetGump.Draw();
				AttackTargetGump.Draw();

				UnuseShader();

				Weather.Draw(m_RenderBounds.GameWindowPosX, m_RenderBounds.GameWindowPosY);
			}

			//Отрисовка текста
			DrawGameWindowText(mode);

			SystemChat->DrawSystemChat(m_RenderBounds.GameWindowPosX, m_RenderBounds.GameWindowPosY, m_RenderBounds.GameWindowSizeY);

			QuestArrow.Draw(m_RenderBounds);

			DrawSmoothMonitorEffect();
		}
		else
		{
			glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
			g_GL.DrawPolygone(m_RenderBounds.GameWindowPosX, m_RenderBounds.GameWindowPosY, m_RenderBounds.GameWindowSizeX, m_RenderBounds.GameWindowSizeY);
			glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

			FontManager->DrawA(3, "You are dead.", 0, m_RenderBounds.GameWindowCenterX - 50, m_RenderBounds.GameWindowCenterY - 20);
		}
		
		g_OutOfRangeColor = 0;

		g_GrayedPixels = false;

		UnuseShader();

		//Восстанавливаем размеры рисуемой области
		g_GL.RestorePort();

#pragma region GameWindowScope
		//Рамка игрового окна
		UO->DrawGump(0x0A8D, 0, m_RenderBounds.GameWindowPosX - 4, m_RenderBounds.GameWindowPosY - 4, 0, m_RenderBounds.GameWindowSizeY + 8);
		UO->DrawGump(0x0A8D, 0, m_RenderBounds.GameWindowPosX + m_RenderBounds.GameWindowSizeX, m_RenderBounds.GameWindowPosY - 4, 0, m_RenderBounds.GameWindowSizeY + 8);

		UO->DrawGump(0x0A8C, 0, m_RenderBounds.GameWindowPosX - 4, m_RenderBounds.GameWindowPosY - 4, m_RenderBounds.GameWindowSizeX + 4, 0);
		UO->DrawGump(0x0A8C, 0, m_RenderBounds.GameWindowPosX - 4, m_RenderBounds.GameWindowPosY + m_RenderBounds.GameWindowSizeY, m_RenderBounds.GameWindowSizeX + 8, 0);

		//Ресайзер / замок
		WORD resizeGumpID = 0x0837; //button
		if (ConfigManager.LockResizingGameWindow)
			 resizeGumpID = 0x082C; //lock
		else if (g_LastObjectType == SOT_GAME_GUMP_SCOPE && g_LastObjectLeftMouseDown == 2)
			resizeGumpID++; //lighted button

		UO->DrawGump(resizeGumpID, 0, m_RenderBounds.GameWindowPosX + m_RenderBounds.GameWindowSizeX - 3, m_RenderBounds.GameWindowPosY + m_RenderBounds.GameWindowSizeY - 3);
#pragma endregion

#if UO_DEBUG_INFO!=0
		char dbf[150] = {0};

		sprintf(dbf, "FPS=%i (%ims) Dir=%i Z=%i (MDZ=%i)", FPScount, g_FrameDelay[1], g_Player->Direction, m_RenderBounds.PlayerZ, m_MaxDrawZ);

		FontManager->DrawA(3, dbf, 0x35, 20, 50);

		sprintf(dbf, "Rendered %i object counts:\nLand=%i Statics=%i Game=%i Multi=%i Lights=%i",
			g_RenderedObjectsCountInGameWindow, g_LandObjectsCount, g_StaticsObjectsCount, g_GameObjectsCount, g_MultiObjectsCount, m_LightCount);

		FontManager->DrawA(3, dbf, 0x35, 20, 74);

		if (g_SelectedObject != NULL)
		{
			char soName[20] = "UnknownObject";

			switch (g_SelectedObject->RenderType)
			{
				case ROT_LAND_OBJECT:
				{
					if (!g_SelectedObject->LandObjectPtr()->IsStretched)
						sprintf(soName, "Land");
					else
						sprintf(soName, "LandTex (mz=%i)", g_SelectedObject->LandObjectPtr()->MinZ);

					break;
				}
				case ROT_STATIC_OBJECT:
				{
					sprintf(soName, "Static");
					break;
				}
				case ROT_GAME_OBJECT:
				{
					sprintf(soName, "GameObject");
					break;
				}
				case ROT_MULTI_OBJECT:
				{
					sprintf(soName, "Multi");
					break;
				}
				default:
					break;
			}

			int tz = g_SelectedObject->Z;

			TLandObject *land = g_SelectedObject->LandObjectPtr();

			//Если это тайл текстуры
			if (land != NULL && land->IsStretched)
				tz = (char)land->Serial;

			sprintf(dbf, "Selected:\n%s: G=0x%04X X=%i Y=%i Z=%i (%i) RQI=%i (SUM=%i)", soName, g_SelectedObject->Graphic, g_SelectedObject->X, g_SelectedObject->Y, g_SelectedObject->Z, tz, g_SelectedObject->RenderQueueIndex, g_SelectedObject->Z + g_SelectedObject->RenderQueueIndex);

			FontManager->DrawA(3, dbf, 0x35, 20, 122);
		}
#endif //UO_DEBUG_INFO!=0

		GumpManager->Draw(mode, false);
		
		// отрисовка ввода игрока
		GameConsole->DrawW((BYTE)ConfigManager.SpeechFont, ConfigManager.SpeechColor, m_RenderBounds.GameWindowPosX, m_RenderBounds.GameWindowPosY + m_RenderBounds.GameWindowSizeY - 18, TS_LEFT, UOFONT_BLACK_BORDER | UOFONT_FIXED);
		
		if (multiOnTarget)
			Target.UnloadMulti();

		if (PluginManager != NULL)
			PluginManager->SceneDraw();

		if (g_GameState == GS_GAME_BLOCKED)
		{
			g_RightMouseDown = RMD;
			g_LeftMouseDown = LMD;
			g_LastObjectLeftMouseDown = LOLMD;
			g_LastGumpLeftMouseDown = LGLMD;

			GameBlockedScreen->Render(false);
			GameBlockedScreen->Render(true);
		}
		else
		{
			if (ObjectInHand != NULL && !ObjectInHand->NoDraw)
			{
				bool doubleDraw = false;
				WORD ohGraphic = ObjectInHand->GetDrawGraphic(doubleDraw);

				WORD ohColor = ObjectInHand->Color;
				DWORD ohCount = ObjectInHand->DragCount;

				if (ohColor != 0)
					ColorizerShader->Use();

				if (ObjectInHand->IsGameFigure)
				{
					ohGraphic -= GAME_FIGURE_GUMP_OFFSET;

					TTextureObject *to = UO->ExecuteGump(ohGraphic);

					if (to != NULL)
						UO->DrawGump(ohGraphic, ohColor, g_MouseX - (to->Width / 2), g_MouseY - (to->Height / 2));
				}
				else
				{
					UO->DrawStaticArtInContainer(ohGraphic, ohColor, g_MouseX, g_MouseY, false, true);

					if (doubleDraw)
						UO->DrawStaticArtInContainer(ohGraphic, ohColor, g_MouseX + 5, g_MouseY + 5, false, true);
				}

				if (ohColor != 0)
					UnuseShader();
			}

			InitToolTip();

			MouseManager.Draw(MouseManager.GetGameCursor()); //Game Gump mouse cursor
		}

		g_GL.EndDraw();
	}
	else //Выбор объектов
	{
		g_LastSelectedObject = 0;
		g_LastObjectType = SOT_NO_OBJECT;
		g_LastSelectedGump = 0;
		g_StatusbarUnderMouse = 0;
		
		GumpManager->Draw(mode, false);

		if (g_LastSelectedGump != 0)
		{
			if (g_LastObjectType == SOT_TEXT_OBJECT)
				g_SelectedTextObject->ToTop();

			return g_LastSelectedObject;
		}
		
		//Если ничего не выбралось - пройдемся по рамке
		if (UO->GumpPixelsInXY(0x0A8D, m_RenderBounds.GameWindowPosX - 4, m_RenderBounds.GameWindowPosY - 4, 0, m_RenderBounds.GameWindowSizeY + 8))
		{
			g_LastObjectType = SOT_GAME_GUMP_SCOPE;
			g_LastSelectedObject = 1;
		}
		else if (UO->GumpPixelsInXY(0x0A8D, m_RenderBounds.GameWindowPosX + m_RenderBounds.GameWindowSizeX, m_RenderBounds.GameWindowPosY - 4, 0, m_RenderBounds.GameWindowSizeY + 8))
		{
			g_LastObjectType = SOT_GAME_GUMP_SCOPE;
			g_LastSelectedObject = 1;
		}
		else if (UO->GumpPixelsInXY(0x0A8C, m_RenderBounds.GameWindowPosX - 4, m_RenderBounds.GameWindowPosY - 4, m_RenderBounds.GameWindowSizeX + 8, 0))
		{
			g_LastObjectType = SOT_GAME_GUMP_SCOPE;
			g_LastSelectedObject = 1;
		}
		else if (UO->GumpPixelsInXY(0x0A8C, m_RenderBounds.GameWindowPosX - 4, m_RenderBounds.GameWindowPosY + m_RenderBounds.GameWindowSizeY, m_RenderBounds.GameWindowSizeX + 8, 0))
		{
			g_LastObjectType = SOT_GAME_GUMP_SCOPE;
			g_LastSelectedObject = 1;
		}
		
		if (!ConfigManager.LockResizingGameWindow)
		{
			if (UO->GumpPixelsInXY(0x0837, m_RenderBounds.GameWindowPosX + m_RenderBounds.GameWindowSizeX - 3, m_RenderBounds.GameWindowPosY + m_RenderBounds.GameWindowSizeY - 3))
			{
				g_LastObjectType = SOT_GAME_GUMP_SCOPE;
				g_LastSelectedObject = 2; //Button
			}
		}

		if (!g_DeathScreenTimer && g_LastObjectType == SOT_NO_OBJECT)
		{
			//Проверка текста
			DrawGameWindowText(mode);

			if (g_LastObjectType == SOT_TEXT_OBJECT)
				WorldTextRenderer->ToTop(g_SelectedTextObject);
		}
		
		if (g_LastObjectType == SOT_NO_OBJECT) //Если ничего не выбралось - пройдемся по объектам
		{
			//Если курсор мыши в игровом окне - просканируем его
			if (g_MouseX < m_RenderBounds.GameWindowPosX || g_MouseY < m_RenderBounds.GameWindowPosY || g_MouseX >(m_RenderBounds.GameWindowPosX + m_RenderBounds.GameWindowSizeX) || g_MouseY >(m_RenderBounds.GameWindowPosY + m_RenderBounds.GameWindowSizeY))
				return g_LastSelectedObject;

			DrawGameWindow(mode);
		}

		return g_LastSelectedObject;
	}

	return 0;
}
//---------------------------------------------------------------------------
/*!
Нажатие левой кнопки мыши
@return
*/
void TGameScreen::OnLeftMouseDown()
{
	g_SelectGumpObjects = true;
	g_LastSelectedGump = 0;

	Render(false);
	
	if (g_LastSelectedGump)
		GumpManager->OnLeftMouseDown(false);
	
	m_GameWindowMoving = false;
	m_GameWindowResizing = false;

	if (g_LastObjectType == SOT_GAME_GUMP_SCOPE && !g_LastSelectedGump)
	{
		if (g_LastSelectedObject == 1)
			m_GameWindowMoving = true;
		else if (g_LastSelectedObject == 2)
			m_GameWindowResizing = true;

		if (g_GumpConsoleType != NULL)
			GumpManager->MoveToBack(g_GumpConsoleType);
	}
	
	g_SelectGumpObjects = false;

	g_LeftMouseDown = true;
	g_DroppedLeftMouseX = g_MouseX;
	g_DroppedLeftMouseY = g_MouseY;
	g_LastObjectLeftMouseDown = g_LastSelectedObject;
	g_LastGumpLeftMouseDown = g_LastSelectedGump;
}
//---------------------------------------------------------------------------
/*!
Отпускание левой кнопки мыши
@return 
*/
void TGameScreen::OnLeftMouseUp()
{
	Render(false);

	if (g_ResizedGump != NULL)
	{
		g_ResizedGump->FrameCreated = false;
		g_ResizedGump->ChangeHeight();

		return;
	}
	else if (g_LeftMouseDown && !g_LastGumpLeftMouseDown && !g_LastGumpRightMouseDown)
	{
		if (m_GameWindowMoving && g_LastObjectLeftMouseDown == 1)
		{
			g_GameWindowPosX = g_GameWindowPosX + (g_MouseX - g_DroppedLeftMouseX);
			g_GameWindowPosY = g_GameWindowPosY + (g_MouseY - g_DroppedLeftMouseY);

			if (g_GameWindowPosX < 1)
				g_GameWindowPosX = 0;

			if (g_GameWindowPosY < 1)
				g_GameWindowPosY = 0;

			if (g_GameWindowPosX + g_GameWindowWidth > g_ClientWidth)
				g_GameWindowPosX = g_ClientWidth - g_GameWindowWidth;

			if (g_GameWindowPosY + g_GameWindowHeight > g_ClientHeight)
				g_GameWindowPosY = g_ClientHeight - g_GameWindowHeight;

			m_GameWindowMoving = false;

			return;
		}
		else if (m_GameWindowResizing && g_LastObjectLeftMouseDown == 2)
		{
			g_GameWindowWidth = g_GameWindowWidth + (g_MouseX - g_DroppedLeftMouseX);
			g_GameWindowHeight = g_GameWindowHeight + (g_MouseY - g_DroppedLeftMouseY);
			
			if (g_GameWindowWidth < 640)
				g_GameWindowWidth = 640;
			
			if (g_GameWindowHeight < 480)
				g_GameWindowHeight = 480;

			if (PacketManager.GetClientVersion() >= CV_200)
			{
				TPacketGameWindowSize packet;
				packet.Send();
			}

			m_GameWindowResizing = false;

			return;
		}
	}

	if (g_LeftMouseDown && (g_LastGumpLeftMouseDown || ObjectInHand != NULL))
	{
		if (GumpManager->OnLeftMouseUp(false))
			return;
	}
	
	int GameWindowPosX = g_GameWindowPosX - 4;
	int GameWindowPosY = g_GameWindowPosY - 4;

	if (g_MouseX < GameWindowPosX || g_MouseY < GameWindowPosY || g_MouseX > (GameWindowPosX + g_GameWindowWidth) || g_MouseY > (GameWindowPosY + g_GameWindowHeight))
		return;

	if (Target.IsTargeting())
	{
		if (g_LastObjectType == SOT_TEXT_OBJECT && g_SelectedTextObject != NULL)
		{
			TTextData *td = (TTextData*)g_SelectedTextObject;

			if (td->Type == TT_OBJECT)
			{
				TGameObject *obj = World->FindWorldObject(td->Serial);

				if (obj != NULL && (obj->NPC || obj->IsCorpse()))
					Target.SendTargetObject(td->Serial);
			}

			g_LastLClickTime = 0;

			return;
		}
		else if (g_SelectedObject != NULL)
		{
			if (g_LastObjectType == SOT_GAME_OBJECT && g_LastSelectedObject)
				Target.SendTargetObject(g_LastSelectedObject);
			else if (g_LastObjectType == SOT_LAND_OBJECT)
				Target.SendTargetTile(0/*g_SelectedObject->Graphic*/, g_SelectedObject->X, g_SelectedObject->Y, g_SelectedObject->Z);
			else if (g_LastObjectType == SOT_STATIC_OBJECT)
				Target.SendTargetTile(g_SelectedObject->Graphic - 0x4000, g_SelectedObject->X, g_SelectedObject->Y, g_SelectedObject->Z);

			g_LastLClickTime = 0;

			return;
		}
	}
	
	DWORD drop_container = 0xFFFFFFFF;
	bool can_drop = false;
	WORD dropX = 0;
	WORD dropY = 0;
	char dropZ = 0;

	if (g_LastObjectType == SOT_GAME_OBJECT && g_LastSelectedObject && ObjectInHand != NULL)
	{
		TGameObject *target = World->FindWorldObject(g_LastSelectedObject);

		can_drop = (GetDistance(g_Player, target) < 3);

		if (can_drop && target != NULL)
		{
			if (target->IsContainer())
				drop_container = target->Serial;
			else if (target->IsStackable() && target->Graphic == ObjectInHand->Graphic)
				drop_container = target->Serial;
			else if (target->NPC)
				drop_container = target->Serial;

			//if (drop_container != 0xFFFFFFFF)
			{
				dropX = target->X;
				dropY = target->Y;
				dropZ = target->Z;
			}
		}
		else
			UO->PlaySoundEffect(0x0051);
	}
	else if ((g_LastObjectType == SOT_LAND_OBJECT || g_LastObjectType == SOT_STATIC_OBJECT) && ObjectInHand != NULL && g_SelectedObject != NULL)
	{
		POINT pt = {g_SelectedObject->X, g_SelectedObject->Y};

		can_drop = (GetDistance(g_Player, pt) < 3);

		if (can_drop)
		{
			dropX = g_SelectedObject->X;
			dropY = g_SelectedObject->Y;
			dropZ = g_SelectedObject->Z;
		}
		else
			UO->PlaySoundEffect(0x0051);
	}
	
	if (can_drop /*&& ObjectInHand != NULL*/)
	{
		if (drop_container == 0xFFFFFFFF && !dropX && !dropY)
			can_drop = false;

		if (can_drop)
			UO->DropItem(drop_container, dropX, dropY, dropZ);
	}
	else if (ObjectInHand == NULL)
	{
		if (g_LastObjectType == SOT_GAME_OBJECT)
		{
			if (!g_ClickObjectReq)
			{
				TGameObject *clickTarget = World->FindWorldObject(g_LastObjectLeftMouseDown);
				if (clickTarget == NULL)
					return;

				g_ClickObjectReq = true;
				g_ClickObject.Init(COT_GAME_OBJECT);
				g_ClickObject.Serial = g_LastObjectLeftMouseDown;
				g_ClickObject.Timer = GetTickCount() + DCLICK_DELAY;
			}
		}
		else if (g_SelectedObject)
		{
			if (g_LastObjectType == SOT_STATIC_OBJECT)
			{
				TTextData *td = g_SelectedObject->StaticGroupObjectPtr()->m_TextControl->m_Head;

				if (td == NULL || td->Timer < GetTickCount())
				{
					WORD id = g_SelectedObject->Graphic - 0x4000;

					string str = ClilocManager->Cliloc(g_Language)->GetA(102000 + id, UO->m_StaticData[id / 32].Tiles[id % 32].Name);

					if (str.length())
						UO->CreateTextMessage(TT_CLIENT, (DWORD)g_SelectedObject, 3, 0x03B5, str);
				}
			}
			/*else if (g_LastObjectType == SOT_LAND_OBJECT)
			{
				WORD ID = g_SelectedObject->Graphic;
				string str(UO->m_LandData[ID / 32].Tiles[ID % 32].Name);
				if (str.length())
					UO->CreateTextMessage(TT_CLIENT, 0, 3, 0x03B5, str);
			}*/
		}
	}

	if (EntryPointer != GameConsole && EntryPointer != NULL)
	{
		TGump *gumpEntry = GumpManager->GetTextEntryOwner();

		if (ConfigManager.GetConsoleNeedEnter())
			EntryPointer = NULL;
		else
			EntryPointer = GameConsole;

		if (gumpEntry != NULL)
			gumpEntry->FrameCreated = false;
	}
}
//---------------------------------------------------------------------------
/*!
Нажатие правой кнопки мыши
@return 
*/
void TGameScreen::OnRightMouseDown()
{
	Render(false);
	
	g_LastGumpRightMouseDown = g_LastSelectedGump;

	if (g_LastSelectedGump)
		GumpManager->OnRightMouseDown(false);
}
//---------------------------------------------------------------------------
/*!
Отпускание правой кнопки мыши
@return 
*/
void TGameScreen::OnRightMouseUp()
{
	Render(false);
	
	if (g_RightMouseDown && g_LastGumpRightMouseDown)
		GumpManager->OnRightMouseUp(false);

	if ((g_ShiftPressed && !g_CtrlPressed && !g_AltPressed) && ConfigManager.HoldShiftForEnablePathfind && ConfigManager.EnablePathfind && g_SelectedObject != NULL && !PathFinder->AutoWalking)
	{
		if (g_SelectedObject->IsLandObject() || g_SelectedObject->IsSurface())
		{
			if (PathFinder->WalkTo(g_SelectedObject->X, g_SelectedObject->Y, g_SelectedObject->Z, 0))
				UO->CreateTextMessage(TT_OBJECT, g_PlayerSerial, 3, 0, "Pathfinding!");
		}
	}
}
//---------------------------------------------------------------------------
/*!
Двойной клик левой кнопкой мыши
@return true при успешной обработке
*/
bool TGameScreen::OnLeftMouseDoubleClick()
{
	Render(false);

	bool result = false;
	DWORD charUnderMouse = 0;

	if (g_LastSelectedGump && GumpManager->OnLeftMouseDoubleClick(false))
		result = true;
	else if (g_LastObjectType == SOT_GAME_OBJECT)
	{
		TGameObject *obj = World->FindWorldObject(g_LastSelectedObject);

		if (obj != NULL)
		{
			if (!obj->NPC)
				UO->DoubleClick(g_LastSelectedObject);
			else
				charUnderMouse = g_LastSelectedObject;
		}
		result = true;
	}
	else if (g_LastObjectType == SOT_TEXT_OBJECT && g_SelectedTextObject != NULL)
	{
		TTextData *td = (TTextData*)g_SelectedTextObject;

		if (td->Type == TT_OBJECT)
		{
			DWORD serial = td->Serial;

			TGameObject *obj = World->FindWorldObject(serial);

			if (obj != NULL && (obj->NPC || obj->IsCorpse()))
			{
				if (obj->NPC)
					charUnderMouse = serial;
				else
					UO->DoubleClick(serial);

				result = true;
			}
		}
	}

	if (charUnderMouse != 0)
	{
		if (!ConfigManager.DisableNewTargetSystem && charUnderMouse < 0x40000000)
		{
			GumpManager->CloseGump(NewTargetSystem.Serial, 0, GT_TARGET_SYSTEM);
			NewTargetSystem.Serial = charUnderMouse;

			if (GumpManager->GetGump(charUnderMouse, 0, GT_TARGET_SYSTEM) == NULL)
			{
				TPacketStatusRequest packet(charUnderMouse);
				packet.Send();

				GumpManager->AddGump(new TGumpTargetSystem(charUnderMouse, NewTargetSystem.GumpX, NewTargetSystem.GumpY));
			}
		}

		if (g_Player->Warmode && charUnderMouse != g_PlayerSerial)
			UO->Attack(charUnderMouse);
		else
			UO->DoubleClick(charUnderMouse);
			//UO->PaperdollReq(charUnderMouse);
	}

	return result;
}
//---------------------------------------------------------------------------
/*!
Двойной клик правой кнопкой мыши
@return true при успешной обработке
*/
bool TGameScreen::OnRightMouseDoubleClick()
{
	if (ConfigManager.EnablePathfind && g_SelectedObject != NULL && !PathFinder->AutoWalking)
	{
		if (g_SelectedObject->IsLandObject() || g_SelectedObject->IsSurface())
		{
			if (PathFinder->WalkTo(g_SelectedObject->X, g_SelectedObject->Y, g_SelectedObject->Z, 0))
			{
				UO->CreateTextMessage(TT_OBJECT, g_PlayerSerial, 3, 0, "Pathfinding!");
				return true;
			}
		}
	}

	return false;
}
//---------------------------------------------------------------------------
/*!
Обработка средней кнопки (колесика) мыши
@param [__in] state Состояние колесика
@return 
*/
void TGameScreen::OnMouseWheel( __in MOUSE_WHEEL_STATE state)
{
	Render(false);

	if (g_LastSelectedGump)
		GumpManager->OnMouseWheel(state, false);
	else if (g_SelectedObject)
	{
		bool ctrlPressed = GetAsyncKeyState(VK_CONTROL) & 0x80000000;

		if (ctrlPressed)
		{
			if (state == MWS_UP)
				g_SelectedObject->IncZ();
			else if (state == MWS_DOWN)
				g_SelectedObject->DecZ();
		}
		else //if (g_SelectedObject->IsStaticObject() || g_SelectedObject->IsLandObject())
		{
			if (state == MWS_UP)
			{
				if (g_SelectedObject->m_NextXY != NULL)
				{
					TRenderWorldObject *next = g_SelectedObject->m_NextXY;
					g_SelectedObject->RemoveRender();
					g_SelectedObject->m_NextXY = next->m_NextXY;
					next->m_NextXY = g_SelectedObject;
					g_SelectedObject->m_PrevXY = next;

					if (g_SelectedObject->m_NextXY)
						g_SelectedObject->m_NextXY->m_PrevXY = g_SelectedObject;
				}
			}
			else if (state == MWS_DOWN)
			{
				if (g_SelectedObject->m_PrevXY != NULL)
				{
					TRenderWorldObject *prev = g_SelectedObject->m_PrevXY;
					g_SelectedObject->RemoveRender();
					g_SelectedObject->m_NextXY = prev;
					g_SelectedObject->m_PrevXY = prev->m_PrevXY;
					prev->m_PrevXY = g_SelectedObject;

					if (g_SelectedObject->m_PrevXY)
						g_SelectedObject->m_PrevXY->m_NextXY = g_SelectedObject;
				}
			}
		}
	}
}
//---------------------------------------------------------------------------
/*!
Обработка нажатия клавиши
@param [__in] wparam не подписанный параметр
@param [__in] lparam не подписанный параметр
@return 
*/
void TGameScreen::OnCharPress( __in WPARAM wparam, __in LPARAM lparam)
{
	if (wparam == VK_RETURN || wparam == VK_BACK || wparam == VK_ESCAPE || EntryPointer == NULL)
		return; //Ignore no print keys

	wchar_t wstr[2] = {0};
	wstr[0] = wparam;
	wstring str(wstr);

	if (!str.length())
		return;
	
	if (EntryPointer != GameConsole)
	{
		if (GumpManager->OnCharPress(wparam, lparam, false))
			return;
	}

	bool altPressed = GetAsyncKeyState(VK_MENU) & 0x80000000;
	bool ctrlPressed = GetAsyncKeyState(VK_CONTROL) & 0x80000000;
	//bool shiftPressed = GetAsyncKeyState(VK_SHIFT) & 0x80000000;

	if (EntryPointer == GameConsole && /*(!EntryPointer->Length() || (g_GumpConsoleType != NULL && g_GumpConsoleType->ConsoleIsEmpty())) &&*/ lparam == 0x100001 && ctrlPressed)
	{
		if (g_ConsoleStackCount)
		{
			EntryPointer->SetText(g_ConsoleStack[g_ConsoleSelectedIndex]);
			EntryPointer->SetPos(g_ConsoleStack[g_ConsoleSelectedIndex].length());

			g_ConsoleSelectedIndex--;

			if (g_ConsoleSelectedIndex < 0)
				g_ConsoleSelectedIndex = (g_ConsoleStackCount - 1) % MAX_CONSOLE_STACK_SIZE;
		}
	}
	else if (iswprint(wparam) && !altPressed && !ctrlPressed && EntryPointer->Length() < 60)
		EntryPointer->Insert(wparam);
}
//---------------------------------------------------------------------------
/*!
Обработка нажатия клавиши
@param [__in] wparam не подписанный параметр
@param [__in] lparam не подписанный параметр
@return 
*/
void TGameScreen::OnKeyPress( __in WPARAM wparam, __in LPARAM lparam)
{
	if (!GumpManager->OnKeyPress(wparam, lparam, false))
	{
		if (EntryPointer == GameConsole)
			EntryPointer->OnKey(NULL, wparam);

		switch (wparam)
		{
			case VK_RETURN:
			{
				if (EntryPointer != NULL)
				{
					if (EntryPointer == GameConsole)
					{
						if (g_ConsolePrompt)
							UO->ConsolePromptSend();
						else if (EntryPointer->Length())
						{
							g_ConsoleStack[g_ConsoleStackCount % MAX_CONSOLE_STACK_SIZE] = EntryPointer->Data();
							g_ConsoleStackCount++;

							if (g_ConsoleStackCount > 1100)
								g_ConsoleStackCount -= 1000;

							g_ConsoleSelectedIndex = (g_ConsoleStackCount - 1) % MAX_CONSOLE_STACK_SIZE;

							GameConsole->Send();
						}

						EntryPointer->Clear();
					}

					if (ConfigManager.GetConsoleNeedEnter())
						EntryPointer = NULL;
					else
						EntryPointer = GameConsole;
				}
				else
					EntryPointer = GameConsole;

				if (g_GumpConsoleType != NULL)
					g_GumpConsoleType->SetConsolePrefix();

				break;
			}
			case VK_PRIOR: //PgUp
			{
				//walk N (0)
				if (!PathFinder->AutoWalking)
					PathFinder->Walk(ConfigManager.AlwaysRun, 0);

				break;
			}
			case VK_NEXT: //PgDown
			{
				//walk E (2)
				if (!PathFinder->AutoWalking)
					PathFinder->Walk(ConfigManager.AlwaysRun, 2);

				break;
			}
			case VK_HOME:
			{
				//walk W (6)
				if (!PathFinder->AutoWalking)
					PathFinder->Walk(ConfigManager.AlwaysRun, 6);

				break;
			}
			case VK_END:
			{
				//walk S (4)
				if (!PathFinder->AutoWalking)
					PathFinder->Walk(ConfigManager.AlwaysRun, 4);

				break;
			}
			case VK_UP:
			{
				//Walk NW (7)
				if (!PathFinder->AutoWalking)
					PathFinder->Walk(ConfigManager.AlwaysRun, 7);

				break;
			}
			case VK_LEFT:
			{
				//Walk SW (5)
				if (!PathFinder->AutoWalking)
					PathFinder->Walk(ConfigManager.AlwaysRun, 5);

				break;
			}
			case VK_DOWN:
			{
				//Walk SE (3)
				if (!PathFinder->AutoWalking)
					PathFinder->Walk(ConfigManager.AlwaysRun, 3);

				break;
			}
			case VK_RIGHT:
			{
				//Walk NE (1)
				if (!PathFinder->AutoWalking)
					PathFinder->Walk(ConfigManager.AlwaysRun, 1);

				break;
			}
			case VK_ESCAPE:
			{
				if (Target.IsTargeting())
					Target.SendCancelTarget();
				else if (NewTargetSystem.Serial)
					NewTargetSystem.Serial = 0;
				else if (PathFinder->AutoWalking && PathFinder->PathFindidngCanBeCancelled)
					PathFinder->StopAutoWalk();

				if (g_ConsolePrompt)
					UO->ConsolePromptCancel();

				break;
			}
			default:
				break;
		}
	}

	if (wparam == VK_TAB)
	{
		if (ConfigManager.HoldTabForCombat)
			UO->ChangeWarmode(1);
		else
			UO->ChangeWarmode();
	}

	if (MacroPointer == NULL)
	{
		bool altPressed = GetAsyncKeyState(VK_MENU) & 0x80000000;
		bool ctrlPressed = GetAsyncKeyState(VK_CONTROL) & 0x80000000;
		bool shiftPressed = GetAsyncKeyState(VK_SHIFT) & 0x80000000;

		TMacro *macro = MacroManager->FindMacro(wparam, altPressed, ctrlPressed, shiftPressed);

		if (macro != NULL)
			MacroPointer = (TMacroObject*)macro->m_Items;

		MacroManager->WaitingBandageTarget = false;
		MacroManager->WaitForTargetTimer = 0;
		MacroManager->Execute();
	}
}
//---------------------------------------------------------------------------