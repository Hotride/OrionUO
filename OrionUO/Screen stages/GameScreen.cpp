// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
/***********************************************************************************
**
** GameScreen.cpp
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#include "stdafx.h"
//----------------------------------------------------------------------------------
CGameScreen g_GameScreen;
RENDER_VARIABLES_FOR_GAME_WINDOW g_RenderBounds;
//----------------------------------------------------------------------------------
CGameScreen::CGameScreen()
: CBaseScreen(m_GameScreenGump)
{
	WISPFUN_DEBUG("c164_f1");
	m_RenderList.resize(1000);

	memset(&g_RenderBounds, 0, sizeof(g_RenderBounds));

	memset(&m_ObjectHandlesList[0], 0, sizeof(m_ObjectHandlesList));
	memset(&m_Light[0], 0, sizeof(m_Light));
}
//----------------------------------------------------------------------------------
CGameScreen::~CGameScreen()
{
	WISPFUN_DEBUG("c164_f2");
}
//----------------------------------------------------------------------------------
/*!
Инициализация
@return 
*/
void CGameScreen::Init()
{
	WISPFUN_DEBUG("c164_f3");
	g_OrionWindow.NoResize = false;

	g_ScreenEffectManager.UseSunrise();
	m_SmoothScreenAction = 0;
}
//----------------------------------------------------------------------------------
/*!
Обработка события после плавного затемнения экрана
@param [__in_opt] action Идентификатор действия
@return 
*/
void CGameScreen::ProcessSmoothAction(uchar action)
{
	WISPFUN_DEBUG("c164_f4");
	if (action == 0xFF)
		action = m_SmoothScreenAction;

	if (action == ID_SMOOTH_GS_LOGOUT)
		g_LogoutAfterClick = true;
}
//----------------------------------------------------------------------------------
/*!
Инициализация всплывающих подсказок
@return 
*/
void CGameScreen::InitToolTip()
{
	WISPFUN_DEBUG("c164_f5");

	if (!(g_ConfigManager.UseToolTips || g_TooltipsEnabled))
		return;

	if (g_SelectedObject.Gump)
	{
		if (g_SelectedObject.Gump == &m_GameScreenGump)
			m_GameScreenGump.InitToolTip();
		else
			g_GumpManager.InitToolTip();
	}
	else if (g_SelectedObject.Object != NULL && g_SelectedObject.Object->IsGameObject())
		g_ObjectPropertiesManager.Display(g_SelectedObject.Serial);
}
//----------------------------------------------------------------------------------
/*!
Вычисление значений Z координаты для рендера
@param [__out] noDrawRoof Не рисовать крыши и верхние этажи строений
@param [__out] maxGroundZ Максимальная Z координата ландшафта
@return Максимальная Z координата отрисовки
*/

void CGameScreen::UpdateMaxDrawZ()
{
	WISPFUN_DEBUG("c164_f6");
	int playerX = g_Player->X;
	int playerY = g_Player->Y;
	int playerZ = g_Player->Z;

	if (playerX == g_Player->OldX && playerY == g_Player->OldY && playerZ == g_Player->OldZ)
		return;

	g_Player->OldX = g_Player->X;
	g_Player->OldY = g_Player->Y;
	g_Player->OldZ = g_Player->Z;

	int maxZ1 = g_MaxGroundZ;
	int maxZ2 = m_MaxDrawZ;

	g_NoDrawRoof = g_ConfigManager.NoDrawRoofs;
	char maxGroundZ = 127;
	g_MaxGroundZ = 127;
	m_MaxDrawZ = 127;

	int bx = playerX / 8;
	int by = playerY / 8;

	int blockIndex = (bx * g_MapBlockSize[g_CurrentMap].Height) + by;
	CMapBlock *mb = g_MapManager.GetBlock(blockIndex);

	if (mb != NULL)
	{
		int x = playerX % 8;
		int y = playerY % 8;

		int pz14 = playerZ + 14;
		int pz16 = playerZ + 16;

		for (CRenderWorldObject *ro = mb->GetRender(x, y); ro != NULL; ro = ro->m_NextXY)
		{
			char tileZ = ro->Z;

			if (!ro->IsGameObject())
			{
				if (ro->IsLandObject())
				{
					if (pz16 <= tileZ)
					{
						maxGroundZ = pz16;
						g_MaxGroundZ = pz16;
						m_MaxDrawZ = g_MaxGroundZ;

						break;
					}

					continue;
				}
				else if (!ro->IsStaticObject() && !ro->IsMultiObject())
					continue;
			}
			else if (((CGameObject*)ro)->NPC)
				continue;

			if (tileZ > pz14 && m_MaxDrawZ > tileZ && !(((CRenderStaticObject*)ro)->GetStaticData()->Flags & 0x20004) && (!ro->IsRoof() || ro->IsSurface()))
			{
				m_MaxDrawZ = tileZ;

				g_NoDrawRoof = true;
			}
		}

		int tempZ = m_MaxDrawZ;
		g_MaxGroundZ = m_MaxDrawZ;

		playerX++;
		playerY++;

		bx = playerX / 8;
		by = playerY / 8;

		blockIndex = (bx * g_MapBlockSize[g_CurrentMap].Height) + by;
		CMapBlock *mb11 = g_MapManager.GetBlock(blockIndex);

		if (mb11 != NULL)
		{
			x = playerX % 8;
			y = playerY % 8;

			for (CRenderWorldObject *ro = mb11->GetRender(x, y); ro != NULL; ro = ro->m_NextXY)
			{
				if (!ro->IsGameObject())
				{
					if (!ro->IsStaticObject() && !ro->IsMultiObject())
						continue;
				}
				else if (((CGameObject*)ro)->NPC)
					continue;

				char tileZ = ro->Z;

				if (tileZ > pz14 && m_MaxDrawZ > tileZ && !(((CRenderStaticObject*)ro)->GetStaticData()->Flags & 0x204) && ro->IsRoof())
				{
					m_MaxDrawZ = tileZ;
					g_MapManager.ClearBlockAccess();
					g_MaxGroundZ = g_MapManager.CalculateNearZ(tileZ, playerX, playerY, tileZ);

					g_NoDrawRoof = true;
				}
			}

			tempZ = g_MaxGroundZ;
		}

		m_MaxDrawZ = g_MaxGroundZ;

		if (tempZ < pz16)
		{
			m_MaxDrawZ = pz16;
			g_MaxGroundZ = pz16;
		}

		g_MaxGroundZ = maxGroundZ;
	}
}
//----------------------------------------------------------------------------------
/*!
Применение прозрачности крон деревьев в указанных координатах
@param [__in] graphic ндекс картинки дерева
@param [__in] x Координата X дерева
@param [__in] y Координата Y дерева
@param [__in] z Координата Z дерева
@return 
*/
void CGameScreen::ApplyTransparentFoliageToUnion(const ushort &graphic, const int &x, const int &y, const int &z)
{
	WISPFUN_DEBUG("c164_f7");
	int bx = x / 8;
	int by = y / 8;

	int blockIndex = (bx * g_MapBlockSize[g_CurrentMap].Height) + by;
	CMapBlock *mb = g_MapManager.GetBlock(blockIndex);

	if (mb != NULL)
	{
		int tx = x % 8;
		int ty = y % 8;

		for (CRenderWorldObject *obj = mb->GetRender(tx, ty); obj != NULL; obj = obj->m_NextXY)
		{
			ushort testGraphic = obj->Graphic;

			if (obj->IsGameObject() && !((CGameObject*)obj)->NPC && ((CGameItem*)obj)->MultiBody)
				testGraphic = ((CGameItem*)obj)->MultiTileGraphic;

			if (testGraphic == graphic && obj->Z == z)
				obj->StaticGroupObjectPtr()->FoliageTransparentIndex = g_FoliageIndex;
		}
	}
}
//----------------------------------------------------------------------------------
/*!
Проверка принадлежности кроны к группе крон (с последующим применением прозрачности всей группе)
@param [__in] graphic Индекс картинки дерева
@param [__in] x Координата X дерева
@param [__in] y Координата Y дерева
@param [__in] z Координата Z дерева
@return 
*/
void CGameScreen::CheckFoliageUnion(ushort graphic, int x, int y, int z)
{
	WISPFUN_DEBUG("c164_f8");

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
				ApplyTransparentFoliageToUnion(graphic, x, y, z);

			break;
		}
	}
}
//----------------------------------------------------------------------------------
/*!
Расчет списка объектов рендера, вычисление прозрачности крон деревьев (в т.ч. составных)
@return 
*/
void CGameScreen::CalculateRenderList()
{
	WISPFUN_DEBUG("c164_f10");
	m_RenderListCount = 0;

	if (g_Player == NULL)
		return;

	if (g_Target.IsTargeting() && g_Target.MultiGraphic && g_SelectedObject.Object != NULL && g_SelectedObject.Object->IsWorldObject())
	{
		int grZ = 0;
		int stZ = 0;
		CRenderObject *sel = g_SelectedObject.Object;
		g_MapManager.GetMapZ(sel->X, sel->Y, grZ, stZ);

		if (((CRenderWorldObject*)sel)->IsStaticObject() && ((CRenderWorldObject*)sel)->IsWet())
			grZ = ((CRenderWorldObject*)sel)->Z;

		g_Target.LoadMulti(sel->X - g_Target.MultiX, sel->Y - g_Target.MultiY, grZ);
	}

	m_CanProcessAlpha = (m_ProcessAlphaTimer < g_Ticks);

	if (m_CanProcessAlpha)
		m_ProcessAlphaTimer = g_Ticks + 50;

	g_FoliageIndex++;

	if (g_FoliageIndex >= 100)
		g_FoliageIndex = 1;

	switch (g_ConfigManager.DrawAuraState)
	{
		case DAS_IN_WARMODE:
		{
			g_DrawAura = g_Player->Warmode && (!g_ConfigManager.DrawAuraWithCtrlPressed || g_CtrlPressed);
			break;
		}
		case DAS_ALWAYS:
		{
			g_DrawAura = (!g_ConfigManager.DrawAuraWithCtrlPressed || g_CtrlPressed);
			break;
		}
		default:
		{
			g_DrawAura = false;
			break;
		}
	}

	m_ObjectHandlesCount = 0;
	bool useObjectHandles = (!g_GrayedPixels && g_ConfigManager.ObjectHandles && g_ShiftPressed && g_CtrlPressed);

	QFOR(go, g_World->m_Items, CGameObject*)
	{
		if (go->NPC || go->IsCorpse())
		{
			go->m_FrameInfo = g_AnimationManager.CollectFrameInformation(go);

			if (go->IsPlayer())
			{
				int playerZOffset = (g_Player->Z * 4) - g_Player->OffsetZ;

				DRAW_FRAME_INFORMATION &dfInfo = go->m_FrameInfo;

				g_PlayerRect.X = g_RenderBounds.GameWindowCenterX - dfInfo.OffsetX + g_Player->OffsetX;
				g_PlayerRect.Y = g_RenderBounds.GameWindowCenterY + g_Player->OffsetY - playerZOffset - dfInfo.OffsetY;
				g_PlayerRect.Width = dfInfo.Width;
				g_PlayerRect.Height = dfInfo.Height;
			}
		}
	}

#if UO_CHECKERBOARD_SEQUENCE_RENDER_LIST == 1
	int minX = g_RenderBounds.RealMinRangeX;
	int minY = g_RenderBounds.RealMinRangeY;

	int maxX = g_RenderBounds.RealMaxRangeX;
	int maxY = g_RenderBounds.RealMaxRangeY;

	int mapBlockHeight = g_MapBlockSize[g_MapManager.GetActualMap()].Height;
	uint maxBlockIndex = g_MapManager.MaxBlockIndex;

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
			int y = (int)lead;

			if (i)
			{
				x = (int)lead;
				y = maxY;
			}

			while (true)
			{
				if (x < minX || x > maxX || y < minY || y > maxY)
					break;
				else
				{
					int blockX = x / 8;
					int blockY = y / 8;

					uint blockIndex = (blockX * mapBlockHeight) + blockY;

					if (blockIndex < maxBlockIndex)
					{
						CMapBlock *block = g_MapManager.GetBlock(blockIndex);

						if (block == NULL)
						{
							block = g_MapManager.AddBlock(blockIndex);
							block->X = blockX;
							block->Y = blockY;
							g_MapManager.LoadBlock(block);
						}

						AddTileToRenderList(block->GetRender(x % 8, y % 8), x, y, useObjectHandles);
					}
					//else
					//	LOG("Expected: %i %i\n", blockIndex, g_MapManager->MaxBlockIndex);
				}

				x++;
				y--;
			}
		}
	}
#else
	for (int bx = g_RenderBounds.MinBlockX; bx <= g_RenderBounds.MaxBlockX; bx++)
	{
		for (int by = g_RenderBounds.MinBlockY; by <= g_RenderBounds.MaxBlockY; by++)
		{
			int blockIndex = (bx * g_MapBlockSize[g_CurrentMap].Height) + by;

			CMapBlock *mb = g_MapManager->GetBlock(blockIndex);

			if (mb == NULL)
			{
				mb = g_MapManager->AddBlock(blockIndex);
				mb->X = bx;
				mb->Y = by;
				g_MapManager->LoadBlock(mb);
			}

			IFOR(x, 0, 8)
			{
				int currentX = bx * 8 + x;

				if (currentX < g_RenderBounds.RealMinRangeX || currentX > g_RenderBounds.RealMaxRangeX)
					continue;

				IFOR(y, 0, 8)
				{
					int currentY = by * 8 + y;

					if (currentY < g_RenderBounds.RealMinRangeY || currentY > g_RenderBounds.RealMaxRangeY)
						continue;

					AddTileToRenderList(mb->GetRender(x, y), currentX, currentY, renderIndex, useObjectHandles);
				}
			}
		}
	}
#endif

	if (m_CanProcessAlpha)
	{
		IFOR(i, 0, m_RenderListCount)
		{
			CRenderWorldObject *obj = m_RenderList[i].Object;

			if (obj != NULL && obj->IsStaticGroupObject())
			{
				CRenderStaticObject *rst = obj->StaticGroupObjectPtr();

				if (rst->IsFoliage())
				{
					if (rst->FoliageTransparentIndex == g_FoliageIndex)
						rst->ProcessAlpha(FOLIAGE_ALPHA);
					else
						rst->ProcessAlpha(0xFF);
				}
			}
		}
	}

	if (m_ObjectHandlesCount > MAX_OBJECT_HANDLES)
		m_ObjectHandlesCount = MAX_OBJECT_HANDLES;

	QFOR(go, g_World->m_Items, CGameObject*)
	{
		if (go->UseInRender != m_RenderIndex && (go->NPC || go->IsCorpse()) && go->m_TextControl->m_Items != NULL)
		{
			go->UpdateDrawCoordinates();
			go->UseInRender = m_RenderIndex;
		}
	}

#if UO_RENDER_LIST_SORT == 1
	m_RenderIndex++;

	if (m_RenderIndex >= 100)
		m_RenderIndex = 1;
#endif

	m_UpdateDrawPos = false;
}
//----------------------------------------------------------------------------------
void CGameScreen::AddTileToRenderList(CRenderWorldObject *obj, const int &worldX, const int &worldY, const bool &useObjectHandles, const int &maxZ)
{
	WISPFUN_DEBUG("c164_f11");
	ushort grayColor = 0;

	if (g_ConfigManager.GrayOutOfRangeObjects)
	{
		if (GetDistance(g_Player, WISP_GEOMETRY::CPoint2Di(worldX, worldY)) > g_ConfigManager.UpdateRange)
			grayColor = 0x038E;
	}

	if (g_CustomHouseGump != NULL)
	{
		RECT rect = { g_CustomHouseGump->StartPos.X, g_CustomHouseGump->StartPos.Y, g_CustomHouseGump->EndPos.X, g_CustomHouseGump->EndPos.Y + 1 };
		POINT pos = { worldX, worldY };

		if (!PtInRect(&rect, pos))
			grayColor = 0x038E;
	}

	for (; obj != NULL; obj = obj->m_NextXY)
	{
#if UO_RENDER_LIST_SORT == 1
		if (obj->CurrentRenderIndex == m_RenderIndex || obj->NoDrawTile)
			continue;
#endif

		if ((m_UpdateDrawPos && obj->CurrentRenderIndex != m_RenderIndex) || obj->Changed)
			obj->UpdateDrawCoordinates();

		obj->UseInRender = 0xFF;
		int drawX = obj->DrawX;
		int drawY = obj->DrawY;

		if (drawX < g_RenderBounds.MinPixelsX || drawX > g_RenderBounds.MaxPixelsX)
			break;

		bool aphaChanged = false;

#if UO_RENDER_LIST_SORT == 1
		int z = obj->Z;

		int maxObjectZ = obj->PriorityZ;

		CRenderStaticObject *rso = obj->StaticGroupObjectPtr();

		if (rso != NULL)
		{
			if (rso->IsGameObject())
			{
				if (((CGameObject*)rso)->NPC)
					maxObjectZ += DEFAULT_CHARACTER_HEIGHT;
				else
					maxObjectZ += rso->GetStaticHeight();
			}
			else
			{
				if (!g_DrawFoliage && rso->IsFoliage())
					continue;
				else if (g_NoDrawRoof && rso->IsRoof())
				{
					if (m_CanProcessAlpha)
						aphaChanged = obj->ProcessAlpha(0);
					else
						aphaChanged = (obj->m_DrawTextureColor[3] != 0);

					if (!aphaChanged)
						continue;
				}
				else if (g_ConfigManager.NoVegetation && rso->Vegetation)
					continue;

				maxObjectZ += rso->GetStaticHeight();
			}
		}

		if (maxObjectZ > maxZ)
			break;

		obj->CurrentRenderIndex = m_RenderIndex;
#endif

		if (obj->IsInternal())
			continue;
		else if (!obj->IsLandObject() && z >= m_MaxDrawZ)
		{
			if (!aphaChanged)
			{
				if (m_CanProcessAlpha)
					aphaChanged = obj->ProcessAlpha(0);
				else
					aphaChanged = (obj->m_DrawTextureColor[3] != 0);

				if (!aphaChanged)
					continue;
			}
		}

		int testMinZ = drawY + (z * 4);
		int testMaxZ = drawY;

		CLandObject *land = obj->LandObjectPtr();

		if (land != NULL && land->IsStretched)
			testMinZ -= (land->MinZ * 4);
		else
			testMinZ = testMaxZ;

		if (testMinZ < g_RenderBounds.MinPixelsY || testMaxZ > g_RenderBounds.MaxPixelsY)
			continue;

		if (obj->IsGameObject())
		{
			CGameObject *go = (CGameObject*)obj;

			if (go->NPC)
			{
				CGameCharacter *character = go->GameCharacterPtr();

				CTextContainer &textContainer = character->m_DamageTextControl;

				if (!textContainer.Empty())
				{
					ANIMATION_DIMENSIONS dims = g_AnimationManager.GetAnimationDimensions(go);

					int textDrawX = drawX + character->OffsetX;
					int textDrawY = drawY + character->OffsetY - (character->OffsetZ + dims.Height + dims.CenterY);

					for (CTextData *text = (CTextData*)textContainer.m_Items; text != NULL;)
					{
						CTextData *next = (CTextData*)text->m_Next;

						if (text->m_Texture.Empty())
						{
							text = next;
							continue;
						}

						if (text->Timer < g_Ticks)
						{
							if (text->Transparent)
							{
								textContainer.Delete(text);
								text = next;
								continue;
							}
							else
							{
								text->Timer = g_Ticks + DAMAGE_TEXT_TRANSPARENT_DELAY;
								text->Transparent = true;
								text->Color = 0x00FF;
							}
						}

						text->RealDrawX = textDrawX - text->X;
						text->RealDrawY = textDrawY + text->Y;

						if (text->Transparent)
						{
							if ((uchar)text->Color >= DAMAGE_TEXT_ALPHA_STEP)
								text->Color -= DAMAGE_TEXT_ALPHA_STEP;
							else
								text->Color = 0;
						}

						if (text->MoveTimer < g_Ticks)
						{
							text->Y -= DAMAGE_TEXT_STEP;
							text->MoveTimer = g_Ticks + DAMAGE_TEXT_MOVE_DELAY;
						}

						text = next;
					}
				}
			}

			if ((go->NPC || (!go->Locked() && !((CGameItem*)go)->MultiBody)) && useObjectHandles && !go->ClosedObjectHandle) // && m_ObjectHandlesCount < MAX_OBJECT_HANDLES)
			{
				int index = m_ObjectHandlesCount % MAX_OBJECT_HANDLES;

				m_ObjectHandlesList[index] = go;

				m_ObjectHandlesCount++;
			}

#if UO_RENDER_LIST_SORT == 1
			if (go->NPC || go->IsCorpse())
				AddOffsetCharacterTileToRenderList(go, useObjectHandles);
#endif
		}
		else if (obj->IsFoliage() && obj->StaticGroupObjectPtr()->FoliageTransparentIndex != g_FoliageIndex)
		{
			char index = 0;

			bool foliageCanBeChecked = (g_RenderBounds.PlayerX <= worldX && g_RenderBounds.PlayerY <= worldY);

			if (!foliageCanBeChecked)
			{
				foliageCanBeChecked = (g_RenderBounds.PlayerY <= worldY && g_RenderBounds.PlayerX <= worldX + 1);

				if (!foliageCanBeChecked)
					foliageCanBeChecked = (g_RenderBounds.PlayerX <= worldX && g_RenderBounds.PlayerY <= worldY + 1);
			}

			if (foliageCanBeChecked)
			{
				WISP_GEOMETRY::CSize fp = g_Orion.GetArtDimension(obj->Graphic, true);

				CImageBounds fib(drawX - fp.Width / 2, drawY - fp.Height, fp.Width, fp.Height);

				if (fib.InRect(g_PlayerRect))
				{
					WISP_GEOMETRY::CRect realRect = g_Orion.GetStaticArtRealPixelDimension(obj->Graphic);

					fib.X += realRect.Position.X;
					fib.Y += realRect.Position.Y;
					fib.Width = realRect.Size.Width;
					fib.Height = realRect.Size.Height;

					if (fib.InRect(g_PlayerRect))
					{
						index = g_FoliageIndex;

						CheckFoliageUnion(obj->Graphic, obj->X, obj->Y, z);
					}
				}
			}

			obj->StaticGroupObjectPtr()->FoliageTransparentIndex = index;
		}

		if (m_CanProcessAlpha && !aphaChanged)
		{
			if (obj->IsTranslucent())
				obj->ProcessAlpha(TRANSLUCENT_ALPHA);
			else if (!obj->IsFoliage() && obj->m_DrawTextureColor[3] != 0xFF)
				obj->ProcessAlpha(0xFF);
		}

		if (m_RenderListCount >= (int)m_RenderList.size())
		{
			size_t newSize = m_RenderList.size() + 1000;

			m_RenderList.resize(newSize);

			if (m_RenderList.size() != newSize)
			{
				LOG("Allocation pixels memory for Render List failed (want size: %i)\n", newSize);

				m_RenderList.resize(newSize - 1000);

				if (m_RenderList.size() != newSize - 1000)
				{
					LOG("Allocation pixels memory for Render List failed SECOND STEP!!! (want size: %i)\n", newSize - 1000);
					m_RenderListCount = 0;
					return;
				}
			}
		}

		//LOG("Item[0x%04X]: x=%i y=%i (dx=%i, dy=%i)\n", obj->Graphic, drawX, drawY, obj->DrawX, obj->DrawY);

		m_RenderList[m_RenderListCount].Object = obj;
		m_RenderList[m_RenderListCount].GrayColor = grayColor;
		obj->UseInRender = m_RenderIndex;

		if (!grayColor && g_CustomHouseGump != NULL && g_Target.IsTargeting() && obj == g_SelectedObject.Object)
		{
			int zOffset = 0;

			if (g_CustomHouseGump->CurrentFloor == 1)
				zOffset = -7;

			if (obj->Z >= g_Player->Z + zOffset && obj->Z < g_Player->Z + 20)
			{
				if (g_CustomHouseGump->Erasing)
				{
					CUSTOM_HOUSE_BUILD_TYPE type;

					if (g_CustomHouseGump->CanEraseHere(obj, type))
						m_RenderList[m_RenderListCount].GrayColor = 0x0021;
				}
				else
					m_RenderList[m_RenderListCount].GrayColor = 0x0035;
			}
		}

		m_RenderListCount++;
	}
}
//----------------------------------------------------------------------------------
void CGameScreen::AddOffsetCharacterTileToRenderList(CGameObject *obj, const bool &useObjectHandles)
{
	WISPFUN_DEBUG("c164_f12");
	int characterX = obj->X;
	int characterY = obj->Y;

	ANIMATION_DIMENSIONS dims = g_AnimationManager.GetAnimationDimensions(obj);
	CGameCharacter *character = obj->GameCharacterPtr();

	DRAW_FRAME_INFORMATION &dfInfo = obj->m_FrameInfo;
	int offsetY = dfInfo.Height - dfInfo.OffsetY;
	bool fullDrawLastItem = false;
	int dropMaxZIndex = -1;

	if (character != NULL)
	{
		//g_GL.DrawPolygone(drawX - dfInfo.OffsetX, drawY, dfInfo.Width, dfInfo.Height - dfInfo.OffsetY);

		if (!character->m_Steps.empty() && (character->m_Steps.back().Direction & 7) == 2)
		{
			fullDrawLastItem = true;
			dropMaxZIndex = 0; //X + 1, Y - 1 : wall
		}
	}

	vector<pair<int, int>> coordinates;

	coordinates.push_back(pair<int, int>(characterX + 1, characterY - 1));
	coordinates.push_back(pair<int, int>(characterX + 1, characterY - 2));
	coordinates.push_back(pair<int, int>(characterX + 2, characterY - 2));
	coordinates.push_back(pair<int, int>(characterX - 1, characterY + 2));
	coordinates.push_back(pair<int, int>(characterX, characterY + 1));
	coordinates.push_back(pair<int, int>(characterX + 1, characterY));
	coordinates.push_back(pair<int, int>(characterX + 2, characterY - 1));
	coordinates.push_back(pair<int, int>(characterX + 1, characterY + 1));

	size_t size = coordinates.size();

	int maxZ = obj->PriorityZ;

	int mapBlockHeight = g_MapBlockSize[g_CurrentMap].Height;
	uint maxBlockIndex = g_MapManager.MaxBlockIndex;

	IFOR(i, 0, size)
	{
		int x = coordinates[i].first;

		if (x < g_RenderBounds.RealMinRangeX || x > g_RenderBounds.RealMaxRangeX)
			continue;

		int y = coordinates[i].second;

		if (y < g_RenderBounds.RealMinRangeY || y > g_RenderBounds.RealMaxRangeY)
			continue;

		int blockX = x / 8;
		int blockY = y / 8;

		uint blockIndex = (blockX * mapBlockHeight) + blockY;

		if (blockIndex < maxBlockIndex)
		{
			CMapBlock *block = g_MapManager.GetBlock(blockIndex);

			if (block == NULL)
			{
				block = g_MapManager.AddBlock(blockIndex);
				block->X = blockX;
				block->Y = blockY;
				g_MapManager.LoadBlock(block);
			}

			int currentMaxZ = maxZ;

			if (i == dropMaxZIndex)
				currentMaxZ += 20;

			AddTileToRenderList(block->GetRender(x % 8, y % 8), x, y, useObjectHandles, currentMaxZ);
		}
	}
}
//----------------------------------------------------------------------------------
/*!
Вычисление параметров игрового окна
@return 
*/
void CGameScreen::CalculateGameWindowBounds()
{
	WISPFUN_DEBUG("c164_f13");
	g_DrawFoliage = (g_Season < ST_WINTER) && !g_ConfigManager.DrawStumps;

	g_GrayedPixels = g_Player->Dead();

	if (g_GrayedPixels && g_Season != ST_DESOLATION)
		g_Orion.ChangeSeason(ST_DESOLATION, DEATH_MUSIC_INDEX);

	g_RenderBounds.PlayerX = g_Player->X;
	g_RenderBounds.PlayerY = g_Player->Y;
	g_RenderBounds.PlayerZ = g_Player->Z;

	int oldDrawOffsetX = g_RenderBounds.WindowDrawOffsetX;
	int oldDrawOffsetY = g_RenderBounds.WindowDrawOffsetY;

	g_RenderBounds.GameWindowPosX = g_ConfigManager.GameWindowX;
	g_RenderBounds.GameWindowPosY = g_ConfigManager.GameWindowY;

	g_RenderBounds.GameWindowWidth = g_ConfigManager.GameWindowWidth;
	g_RenderBounds.GameWindowHeight = g_ConfigManager.GameWindowHeight;

	m_GameScreenGump.UpdateContent();

	//int playerZOffset = (g_Player->Z * 4) - g_Player->OffsetZ;

	g_RenderBounds.GameWindowCenterX = g_RenderBounds.GameWindowPosX + (g_RenderBounds.GameWindowWidth / 2);
	g_RenderBounds.GameWindowCenterY = (g_RenderBounds.GameWindowPosY + g_RenderBounds.GameWindowHeight / 2) + (g_Player->Z * 4);

	/*int earthquakeMagnitude = RandomInt(11);

	if (earthquakeMagnitude)
	{
		g_RenderBounds.GameWindowCenterX += RandomInt(earthquakeMagnitude * 3);
		g_RenderBounds.GameWindowCenterY += RandomInt(earthquakeMagnitude * 3);
	}*/

	g_RenderBounds.GameWindowCenterX -= g_Player->OffsetX;
	g_RenderBounds.GameWindowCenterY -= (g_Player->OffsetY - g_Player->OffsetZ);

	g_RenderBounds.WindowDrawOffsetX = ((g_Player->X - g_Player->Y) * 22) - g_RenderBounds.GameWindowCenterX;
	g_RenderBounds.WindowDrawOffsetY = ((g_Player->X + g_Player->Y) * 22) - g_RenderBounds.GameWindowCenterY;

	if (g_ConfigManager.UseScaling)
	{
		GLdouble left = (GLdouble)g_RenderBounds.GameWindowPosX;
		GLdouble right = (GLdouble)(g_RenderBounds.GameWindowWidth + left);
		GLdouble top = (GLdouble)g_RenderBounds.GameWindowPosY;
		GLdouble bottom = (GLdouble)(g_RenderBounds.GameWindowHeight + top);

		GLdouble newRight = right * g_GlobalScale;
		GLdouble newBottom = bottom * g_GlobalScale;

		g_RenderBounds.GameWindowScaledOffsetX = (int)((left * g_GlobalScale) - (newRight - right));
		g_RenderBounds.GameWindowScaledOffsetY = (int)((top * g_GlobalScale) - (newBottom - bottom));

		g_RenderBounds.GameWindowScaledWidth = (int)(newRight - g_RenderBounds.GameWindowScaledOffsetX);
		g_RenderBounds.GameWindowScaledHeight = (int)(newBottom - g_RenderBounds.GameWindowScaledOffsetY);
	}
	else
	{
		g_GlobalScale = 1.0;

		g_RenderBounds.GameWindowScaledOffsetX = 0;
		g_RenderBounds.GameWindowScaledOffsetY = 0;

		g_RenderBounds.GameWindowScaledWidth = 0;
		g_RenderBounds.GameWindowScaledHeight = 0;
	}

	int rangeX = (int)(((g_RenderBounds.GameWindowWidth / 44) + 1) * g_GlobalScale);
	int rangeY = (int)(((g_RenderBounds.GameWindowHeight / 44) + 1) * g_GlobalScale);

	if (rangeX < rangeY)
		rangeX = rangeY;
	else
		rangeY = rangeX;

	g_RenderBounds.RealMinRangeX = g_Player->X - rangeX;

	if (g_RenderBounds.RealMinRangeX < 0)
		g_RenderBounds.RealMinRangeX = 0;

	g_RenderBounds.RealMaxRangeX = g_Player->X + rangeX;

	if (g_RenderBounds.RealMaxRangeX >= g_MapSize[g_CurrentMap].Width)
		g_RenderBounds.RealMaxRangeX = g_MapSize[g_CurrentMap].Width;

	g_RenderBounds.RealMinRangeY = g_Player->Y - rangeY;

	if (g_RenderBounds.RealMinRangeY < 0)
		g_RenderBounds.RealMinRangeY = 0;

	g_RenderBounds.RealMaxRangeY = g_Player->Y + rangeY;

	if (g_RenderBounds.RealMaxRangeY >= g_MapSize[g_CurrentMap].Height)
		g_RenderBounds.RealMaxRangeY = g_MapSize[g_CurrentMap].Height;

	g_RenderBounds.MinBlockX = (g_RenderBounds.RealMinRangeX / 8) - 1;
	g_RenderBounds.MinBlockY = (g_RenderBounds.RealMinRangeY / 8) - 1;
	g_RenderBounds.MaxBlockX = (g_RenderBounds.RealMaxRangeX / 8) + 1;
	g_RenderBounds.MaxBlockY = (g_RenderBounds.RealMaxRangeY / 8) + 1;

	if (g_RenderBounds.MinBlockX < 0)
		g_RenderBounds.MinBlockX = 0;

	if (g_RenderBounds.MinBlockY < 0)
		g_RenderBounds.MinBlockY = 0;

	if (g_RenderBounds.MaxBlockX >= g_MapBlockSize[g_CurrentMap].Width)
		g_RenderBounds.MaxBlockX = g_MapBlockSize[g_CurrentMap].Width - 1;

	if (g_RenderBounds.MaxBlockY >= g_MapBlockSize[g_CurrentMap].Height)
		g_RenderBounds.MaxBlockY = g_MapBlockSize[g_CurrentMap].Height - 1;

	int drawOffset = (int)(g_GlobalScale * 40.0);

	GLdouble maxX = g_RenderBounds.GameWindowPosX + g_RenderBounds.GameWindowWidth + drawOffset;
	GLdouble maxY = g_RenderBounds.GameWindowPosY + g_RenderBounds.GameWindowHeight + drawOffset;
	GLdouble newMaxX = maxX * g_GlobalScale;
	GLdouble newMaxY = maxY * g_GlobalScale;

	g_RenderBounds.MinPixelsX = (int)(((g_RenderBounds.GameWindowPosX - drawOffset) * g_GlobalScale) - (newMaxX - maxX));
	g_RenderBounds.MaxPixelsX = (int)newMaxX;

	g_RenderBounds.MinPixelsY = (int)(((g_RenderBounds.GameWindowPosY - drawOffset) * g_GlobalScale) - (newMaxY - maxY)); // -playerZOffset;
	g_RenderBounds.MaxPixelsY = (int)newMaxY; // + playerZOffset;

	if (m_UpdateDrawPos || oldDrawOffsetX != g_RenderBounds.WindowDrawOffsetX || oldDrawOffsetX != g_RenderBounds.WindowDrawOffsetY)
	{
		m_UpdateDrawPos = true;
		m_RenderListInitalized = false;
	}

	UpdateMaxDrawZ();

	m_UseLight = (g_PersonalLightLevel < g_LightLevel);

	if (m_UseLight && g_GL.CanUseFrameBuffer)
	{
		int testWidth = g_RenderBounds.GameWindowWidth;
		int testHeight = g_RenderBounds.GameWindowHeight;

		if (g_ConfigManager.UseScaling)
		{
			testWidth = g_RenderBounds.GameWindowScaledWidth;
			testHeight = g_RenderBounds.GameWindowScaledHeight;
		}

		if (!g_LightBuffer.Ready(testWidth, testHeight))
			g_LightBuffer.Init(testWidth, testHeight);
	}
}
//----------------------------------------------------------------------------------
/*!
Функция добавления источника света
@param [__in] rwo Верхний объект, источающий свет
@param [__in] lightObject Объект, источающий свет
@param [__in] x Экранная координата X
@param [__in] y Экранная координата Y
@return 
*/
void CGameScreen::AddLight(CRenderWorldObject *rwo, CRenderWorldObject *lightObject, const int &x, const int &y)
{
	WISPFUN_DEBUG("c164_f14");

	if (m_LightCount < MAX_LIGHT_SOURCES)
	{
		bool canBeAdded = true;
		
		int testX = rwo->X + 1;
		int testY = rwo->Y + 1;
		
		int bx = testX / 8;
		int by = testY / 8;

		int blockIndex = (bx * g_MapBlockSize[g_CurrentMap].Height) + by;
		CMapBlock *mb = g_MapManager.GetBlock(blockIndex);

		if (mb != NULL)
		{
			bx = testX % 8;
			by = testY % 8;
			
			char z5 = rwo->Z + 5;
		
			for (CRenderWorldObject *obj = mb->GetRender(bx, by); obj != NULL; obj = obj->m_NextXY)
			{
				if (!obj->IsStaticGroupObject() || (obj->IsGameObject() && ((CGameObject*)obj)->NPC) || obj->NoDrawTile || obj->IsTransparent())
					continue;

				if (obj->Z < m_MaxDrawZ && obj->Z >= z5)
				{
					canBeAdded = false;
					break;
				}
			}
		}

		if (canBeAdded)
		{
			LIGHT_DATA &light = m_Light[m_LightCount];

			ushort graphic = lightObject->Graphic;

			if ((graphic >= 0x3E02 && graphic <= 0x3E0B) || (graphic >= 0x3914 && graphic <= 0x3929))
				light.ID = 2;
			else if (rwo == lightObject && rwo->IsGameObject())
				light.ID = ((CGameItem*)lightObject)->LightID;
			else
				light.ID = (uchar)lightObject->GetLightID();

			if (light.ID >= MAX_LIGHTS_DATA_INDEX_COUNT)
				return;

			if (g_ConfigManager.ColoredLighting)
				light.Color = g_Orion.GetLightColor(graphic);
			else
				light.Color = 0;

			light.DrawX = x;
			light.DrawY = y;

			m_LightCount++;
		}
	}
}
//----------------------------------------------------------------------------------
/*!
Рисование игрового окна
@param [__in] mode true - отрисовка, false - выбор
@return 
*/
void CGameScreen::DrawGameWindow(const bool &mode)
{
	WISPFUN_DEBUG("c164_f15");
	int playerZPlus5 = g_RenderBounds.PlayerZ + 5;

	if (mode)
	{
		glColor4f(g_DrawColor, g_DrawColor, g_DrawColor, 1.0f);

		if (g_ConfigManager.UseCircleTrans)
		{
			if (g_CircleOfTransparency.Create(g_ConfigManager.CircleTransRadius))
			{
				int drawX = g_RenderBounds.GameWindowCenterX + g_Player->OffsetX;
				int drawY = g_RenderBounds.GameWindowCenterY + g_Player->OffsetY - (g_RenderBounds.PlayerZ * 4 + g_Player->OffsetZ);

				g_CircleOfTransparency.Draw(drawX, drawY);
			}
		}

		m_HitsStack.clear();

		IFOR(i, 0, m_RenderListCount)
		{
			RENDER_OBJECT_DATA &rod = m_RenderList[i];
			CRenderWorldObject *obj = rod.Object;

			if (obj != NULL)
			{
				g_OutOfRangeColor = rod.GrayColor;

				g_UseCircleTrans = (g_ConfigManager.UseCircleTrans && obj->TranparentTest(playerZPlus5));

				int x = obj->DrawX;
				int y = obj->DrawY;

				obj->Draw(x, y);

				if (g_ConfigManager.DrawStatusState && obj->IsGameObject() && ((CGameObject*)obj)->NPC && !((CGameCharacter*)obj)->Dead())
				{
					CGameCharacter *gc = (CGameCharacter*)obj;

					ushort color = g_ConfigManager.GetColorByNotoriety(gc->Notoriety);

					//usual color
					ushort healthColor = 90;

					int width = gc->MaxHits;

					if (width > 0)
					{
						width = (gc->Hits * 100) / width;

						if (width > 100)
							width = 100;

						if (width < 1)
							width = 0;
					}
					else
						continue;

					if (g_ConfigManager.DrawStatusConditionState == DCSCS_ALWAYS ||
						(g_ConfigManager.DrawStatusConditionState == DCSCS_NOT_MAX && gc->Hits != gc->MaxHits) ||
						(g_ConfigManager.DrawStatusConditionState == DCSCS_LOWER && width < g_ConfigManager.DrawStatusConditionValue))
					{
						x += gc->OffsetX;
						y += gc->OffsetY - gc->OffsetZ;

						if (g_ConfigManager.DrawStatusState == DCSS_ABOVE)
						{
							ANIMATION_DIMENSIONS dims = g_AnimationManager.GetAnimationDimensions(gc, 0);
							y -= (dims.Height + dims.CenterY) + 24;

							gc->UpdateHitsTexture(width);
							width = (int)&gc->m_HitsTexture;

							x -= (gc->m_HitsTexture.Width / 2) - 3;
						}
						else
						{
							x -= 20;

							if (g_ConfigManager.DrawStatusState == DCSS_UNDER)
							{
								if (g_TargetGump.TargetedCharacter == obj) continue;
								if (g_AttackTargetGump.TargetedCharacter == obj) continue;
								if (g_NewTargetSystem.TargetedCharacter == obj) continue;
								width = (34 * width) / 100;
								if (gc->Poisoned())
									healthColor = 63; //Character status line (green)
								else if (gc->YellowHits())
									healthColor = 53; //Character status line (green)
							}
								
						}

						OBJECT_HITS_INFO hitsInfo = { x, y, color, width, healthColor};
						m_HitsStack.push_back(hitsInfo);
					}
				}
			}
		}

		glDisable(GL_DEPTH_TEST);

		UnuseShader();

		IFOR(i, 0, m_ObjectHandlesCount)
			m_ObjectHandlesList[i]->DrawObjectHandlesTexture();

		g_PluginManager.WorldDraw(); 
	}
	else
	{
		bool useCircleTrans = (g_ConfigManager.UseCircleTrans && g_CircleOfTransparency.Select(g_CircleOfTransparency.X, g_CircleOfTransparency.Y));

		IFOR(i, 0, m_RenderListCount)
		{
			CRenderWorldObject *obj = m_RenderList[i].Object;

			if (obj != NULL)
			{
				g_UseCircleTrans = (useCircleTrans && obj->TranparentTest(playerZPlus5));

				obj->Select(obj->DrawX, obj->DrawY);
			}
		}

		IFOR(i, 0, m_ObjectHandlesCount)
			m_ObjectHandlesList[i]->SelectObjectHandlesTexture();
	}
}
//----------------------------------------------------------------------------------
/*!
Отображение источников света
@return 
*/
void CGameScreen::DrawGameWindowLight()
{
	WISPFUN_DEBUG("c164_f16");
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

	if (!m_UseLight)
		return;

	g_LightColorizerShader.Use();

	if (g_GL.CanUseFrameBuffer)
	{
		if (/*g_LightBuffer.Ready() &&*/ g_LightBuffer.Use())
		{
			float newLightColor = ((32 - g_LightLevel + g_PersonalLightLevel) / 32.0f);

			if (!g_ConfigManager.DarkNights)
				newLightColor += 0.2f;

			glClearColor(newLightColor, newLightColor, newLightColor, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT);
			glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

			glEnable(GL_BLEND);
			glBlendFunc(GL_ONE, GL_ONE);

			int offsetX = 0;
			int offsetY = 0;

			if (g_ConfigManager.UseScaling)
			{
				offsetX = g_RenderBounds.GameWindowPosX - g_RenderBounds.GameWindowScaledOffsetX;
				offsetY = g_RenderBounds.GameWindowPosY - g_RenderBounds.GameWindowScaledOffsetY;
			}

			GLfloat translateOffsetX = (GLfloat)offsetX;
			GLfloat translateOffsetY = (GLfloat)offsetY;

			glTranslatef(translateOffsetX, translateOffsetY, 0.0f);

			IFOR(i, 0, m_LightCount)
				g_Orion.DrawLight(m_Light[i]);

			glTranslatef(-translateOffsetX, -translateOffsetY, 0.0f);

			UnuseShader();

			g_LightBuffer.Release();

			g_GL.RestorePort();

			g_GL.ViewPortScaled(g_RenderBounds.GameWindowPosX, g_RenderBounds.GameWindowPosY, g_RenderBounds.GameWindowWidth, g_RenderBounds.GameWindowHeight);

			glBlendFunc(GL_ZERO, GL_SRC_COLOR);

			if (g_ConfigManager.UseScaling)
				g_LightBuffer.Draw(g_RenderBounds.GameWindowScaledOffsetX, g_RenderBounds.GameWindowScaledOffsetY);
			else
				g_LightBuffer.Draw(g_RenderBounds.GameWindowPosX, g_RenderBounds.GameWindowPosY);

			glDisable(GL_BLEND);
		}
	}
	else
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_ONE, GL_ONE);

		IFOR(i, 0, m_LightCount)
			g_Orion.DrawLight(m_Light[i]);

		glDisable(GL_BLEND);
	}

	UnuseShader();
}
//----------------------------------------------------------------------------------
/*!
Отображение текста над объектами мира
@param [__in] mode true - отрисовка, false - выбор
@return 
*/
void CGameScreen::DrawGameWindowText(const bool &mode)
{
	WISPFUN_DEBUG("c164_f17");
	if (mode)
	{
		g_FontColorizerShader.Use();

		g_WorldTextRenderer.WorldDraw();

		UnuseShader();

		if (g_ConfigManager.DrawStatusState && m_HitsStack.size())
		{
			if (g_ConfigManager.DrawStatusState == DCSS_ABOVE)
			{
				for (vector<OBJECT_HITS_INFO>::iterator it = m_HitsStack.begin(); it != m_HitsStack.end(); ++it)
				{
					CGLTextTexture *texture = (CGLTextTexture*)it->Width;
					texture->Draw(it->X, it->Y);
				}
			}
			else
			{
				g_ColorizerShader.Use();

				IFOR(i, 0, 2)
				{
					for (vector<OBJECT_HITS_INFO>::iterator it = m_HitsStack.begin(); it != m_HitsStack.end(); ++it)
					{
						if (!i)
							g_Orion.DrawGump(0x1068, it->Color, it->X, it->Y);
						else if (it->Width)
							g_Orion.DrawGump(0x1069, it->HealthColor, it->X, it->Y, it->Width, 0);
					}
				}

				UnuseShader();
			}
		}

		QFOR(obj, g_World->m_Items, CGameObject*)
		{
			if (obj->NPC)
			{
				CTextContainer &textContainer = obj->GameCharacterPtr()->m_DamageTextControl;

				if (textContainer.Empty())
					continue;

				QFOR(text, textContainer.m_Items, CTextData*)
				{
					if (!text->m_Texture.Empty())
					{
						if (text->Transparent)
						{
							glEnable(GL_BLEND);
							glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

							glColor4ub(0xFF, 0xFF, 0xFF, (uchar)text->Color);

							text->m_Texture.Draw(text->RealDrawX, text->RealDrawY);

							glDisable(GL_BLEND);
						}
						else
							text->m_Texture.Draw(text->RealDrawX, text->RealDrawY);
					}
				}
			}
		}

		glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	}
	else
		g_WorldTextRenderer.Select(NULL);
}
//----------------------------------------------------------------------------------
void CGameScreen::PrepareContent()
{
	WISPFUN_DEBUG("c164_f18");
	g_WorldTextRenderer.CalculateWorldPositions(false);

	m_GameScreenGump.PrepareContent();

	g_GumpManager.PrepareContent();

	if (g_SelectedObject.Gump != NULL && (g_SelectedObject.Gump->GumpType == GT_STATUSBAR || g_SelectedObject.Gump->GumpType == GT_POPUP_MENU) && g_SelectedObject.Gump->Serial != g_PlayerSerial)
		g_StatusbarUnderMouse = g_SelectedObject.Gump->Serial;
	else
		g_StatusbarUnderMouse = 0;

	//if (g_SelectedObject.Object() != NULL && g_SelectedObject.Object()->IsGameObject() && g_PressedObject.LeftObject == g_SelectedObject.Object())
	if (g_PressedObject.LeftObject != NULL && g_PressedObject.LeftObject->IsGameObject() && g_MouseManager.LastLeftButtonClickTimer < g_Ticks)
	{
		WISP_GEOMETRY::CPoint2Di offset = g_MouseManager.LeftDroppedOffset();

		if (CanBeDraggedByOffset(offset) || (g_MouseManager.LastLeftButtonClickTimer + g_MouseManager.DoubleClickDelay < g_Ticks))
		{
			CGameItem *selobj = g_World->FindWorldItem(g_PressedObject.LeftSerial);

			if (selobj != NULL && !g_ObjectInHand.Enabled && !selobj->Locked() && GetDistance(g_Player, selobj) <= DRAG_ITEMS_DISTANCE)
			{
				if (selobj->Serial >= 0x40000000 && !g_GrayedPixels) //Item selection
				{
					if (selobj->IsStackable() && selobj->Count > 1 && !g_ShiftPressed)
					{
						CGumpDrag *newgump = new CGumpDrag(g_PressedObject.LeftSerial, g_MouseManager.Position.X - 80, g_MouseManager.Position.Y - 34);

						g_GumpManager.AddGump(newgump);
						g_OrionWindow.EmulateOnLeftMouseButtonDown();
						selobj->Dragged = true;
					}
					else if (!g_Target.IsTargeting())
					{
						g_Orion.PickupItem(selobj);
						g_PressedObject.ClearLeft(); //g_LastObjectLeftMouseDown = 0;
					}
				}
			}
			else if (!g_ObjectInHand.Enabled)
			{
				CGameCharacter *selchar = g_World->FindWorldCharacter(g_PressedObject.LeftSerial);

				if (selchar != NULL) //Character selection
				{
					g_Orion.OpenStatus(selchar->Serial);
					g_GeneratedMouseDown = true;
					g_OrionWindow.EmulateOnLeftMouseButtonDown();
					g_PluginManager.WindowProc(g_OrionWindow.Handle, UOMSG_STATUS_REQUEST, (WPARAM)selchar->Serial, 0);
				}
			}
		}
	}
}
//----------------------------------------------------------------------------------
/*!
Отрисовка/выбор объектов
@param [__in] mode true - отрисовка, false - выбор
@return При выборе объектов - идентификатор выбранного объекта
*/
void CGameScreen::Render(const bool &mode)
{
	WISPFUN_DEBUG("c164_f19");
	if (!m_RenderListInitalized)
		CalculateRenderList();

	if (g_DeathScreenTimer < g_Ticks)
	{
		if (g_DeathScreenTimer && g_ScreenEffectManager.UseSunrise())
			m_SmoothScreenAction = 0;

		g_DeathScreenTimer = 0;
	}
	else if (!g_Player->Dead())
		g_DeathScreenTimer = 0;

	if (mode)
	{
		static uint lastRender = 0;
		static int currentFPS = 0;
		static int FPScount = 0;

		if (lastRender < g_Ticks)
		{
			LOG("FPS=%i\n", FPScount);
			FPScount = currentFPS;
			currentFPS = 0;
			lastRender = g_Ticks + 1000;
		}
		else
			currentFPS++;

		g_GL.BeginDraw();

		if (DrawSmoothMonitor())
			return;

		uint deathScreenTimer = g_DeathScreenTimer;

		if (deathScreenTimer && g_ScreenEffectManager.Mode != SEM_NONE)
			deathScreenTimer = 0;

		CSelectedObject tempSelected;
		CPressedObject tempPressed;

		if (g_GameState == GS_GAME_BLOCKED)
		{
			tempSelected.Init(g_SelectedObject);
			g_SelectedObject.Clear();

			tempPressed.Init(g_PressedObject);
			g_PressedObject.ClearAll();
		}

		g_RenderedObjectsCountInGameWindow = 0;
		
		g_TargetGump.Color = 0;
		g_TargetGump.TargetedCharacter = NULL;
		g_AttackTargetGump.Color = 0;
		g_AttackTargetGump.TargetedCharacter = NULL;
		g_NewTargetSystem.ColorGump = 0;
		g_NewTargetSystem.TargetedCharacter = NULL;

		m_LightCount = 0;

		g_GL.ViewPortScaled(g_RenderBounds.GameWindowPosX, g_RenderBounds.GameWindowPosY, g_RenderBounds.GameWindowWidth, g_RenderBounds.GameWindowHeight);
		
		g_DrawColor = 1.0f;

		if (!g_GL.CanUseFrameBuffer && g_PersonalLightLevel < g_LightLevel)
		{
			g_DrawColor = (32 - g_LightLevel + g_PersonalLightLevel) / 32.0f;

			if (!g_ConfigManager.DarkNights)
				g_DrawColor += 0.2f;
		}

		if (g_GrayedPixels)
			g_DeathShader.Use();
		else
			g_ColorizerShader.Use();

		DrawGameWindow(mode);

		UnuseShader();

		if (!deathScreenTimer)
		{
			if (!g_GrayedPixels)
			{
				DrawGameWindowLight();

				g_ColorizerShader.Use();

				g_NewTargetSystem.Draw();
				g_TargetGump.Draw();
				g_AttackTargetGump.Draw();

				UnuseShader();

				g_Weather.Draw(g_RenderBounds.GameWindowPosX, g_RenderBounds.GameWindowPosY);
			}

			//Отрисовка текста
			DrawGameWindowText(mode);

			DrawSmoothMonitorEffect();
		}
		else
		{
			glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
			g_GL.DrawPolygone(g_RenderBounds.GameWindowPosX, g_RenderBounds.GameWindowPosY, g_RenderBounds.GameWindowWidth, g_RenderBounds.GameWindowHeight);
			glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

			g_FontManager.DrawA(3, "You are dead.", 0, g_RenderBounds.GameWindowPosX + (g_RenderBounds.GameWindowWidth / 2) - 50, g_RenderBounds.GameWindowPosY + (g_RenderBounds.GameWindowHeight / 2) - 20);
		}
		
		g_OutOfRangeColor = 0;

		g_GrayedPixels = false;

		UnuseShader();

		if (!deathScreenTimer)
		{
			g_SystemChat.DrawSystemChat(g_RenderBounds.GameWindowPosX, g_RenderBounds.GameWindowPosY, g_RenderBounds.GameWindowHeight);

			g_QuestArrow.Draw();
		}

		//Восстанавливаем размеры рисуемой области
		g_GL.RestorePort();

		m_GameScreenGump.Draw();

#if UO_DEBUG_INFO!=0
		if (g_DeveloperMode == DM_SHOW_FPS_ONLY)
		{
			char dbf[50] = { 0 };

			sprintf_s(dbf, "FPS=%i (%ims) ping=%i scale=%.1f", FPScount, g_FrameDelay[1], g_Ping != 0 ? g_Ping : g_PingByPacket, g_GlobalScale);

			g_FontManager.DrawA(3, dbf, 0x35, g_RenderBounds.GameWindowPosX + g_RenderBounds.GameWindowWidth + 10, g_RenderBounds.GameWindowPosY);
		}
		else if (g_DeveloperMode == DM_DEBUGGING)
		{
			char dbf[150] = { 0 };

			sprintf_s(dbf, "FPS=%i (%ims) ping=%i Dir=%i Z=%i (MDZ=%i) scale=%.1f", FPScount, g_FrameDelay[1], g_Ping != 0? g_Ping : g_PingByPacket, g_Player->Direction, g_RenderBounds.PlayerZ, m_MaxDrawZ, g_GlobalScale);

			g_FontManager.DrawA(3, dbf, 0x35, 20, 30);

			sprintf_s(dbf, "Rendered %i object counts:\nLand=%i Statics=%i Game=%i Multi=%i Lights=%i",
				g_RenderedObjectsCountInGameWindow, g_LandObjectsCount, g_StaticsObjectsCount, g_GameObjectsCount, g_MultiObjectsCount, m_LightCount);

			g_FontManager.DrawA(3, dbf, 0x35, 20, 54);

			if (g_SelectedObject.Object != NULL && g_SelectedObject.Object->IsWorldObject())
			{
				CRenderWorldObject *selRwo = (CRenderWorldObject*)g_SelectedObject.Object;
				CLandObject *land = selRwo->LandObjectPtr();
				char soName[20] = "UnknownObject";

				switch (selRwo->RenderType)
				{
					case ROT_LAND_OBJECT:
					{
						if (!land->IsStretched)
							sprintf_s(soName, "Land");
						else
							sprintf_s(soName, "LandTex (mz=%i)", land->MinZ);

						break;
					}
					case ROT_STATIC_OBJECT:
					{
						sprintf_s(soName, "Static");
						break;
					}
					case ROT_GAME_OBJECT:
					{
						sprintf_s(soName, "GameObject");
						break;
					}
					case ROT_MULTI_OBJECT:
					{
						sprintf_s(soName, "Multi");

						if (((CMultiObject*)selRwo)->IsCustomHouseMulti())
							sprintf_s(soName, "Multi CH %04X", ((CMultiObject*)selRwo)->State);
						else
							sprintf_s(soName, "Multi");
						break;
					}
					default:
						break;
				}

				int tz = selRwo->Z;

				//Если это тайл текстуры
				if (land != NULL && land->IsStretched)
					tz = (char)land->Serial;

				uint tiledataFlags = (uint)(selRwo->IsStaticGroupObject() ? ((CRenderStaticObject*)selRwo)->GetStaticData()->Flags : 0);

				sprintf_s(dbf, "Selected:\n%s: G=0x%04X C:0x%04X TF=0x%08X X=%i Y=%i Z=%i (%i) PriZ=%i", soName, selRwo->Graphic, selRwo->Color, tiledataFlags, selRwo->X, selRwo->Y, selRwo->Z, tz, selRwo->PriorityZ);

				const string flagNames[] =
				{
					"Background",
					"Weapon",
					"Transparent",
					"Translucent",
					"Wall",
					"Damaging",
					"Impassable",
					"Wet",
					"Unknown",
					"Surface",
					"Bridge",
					"Stackable",
					"Window",
					"NoShoot",
					"PrefixA",
					"PrefixAn",
					"Internal",
					"Foliage",
					"PartialHue",
					"Unknown1",
					"Map",
					"Container",
					"Wearable",
					"LightSource",
					"Animated",
					"NoDiagonal",
					"Unknown2",
					"Armor",
					"Roof",
					"Door",
					"StairBack",
					"StairRight"
				};
				
				string flagsData = "";

				IFOR(f, 0, 32)
				{
					if (tiledataFlags & (1 << f))
						flagsData += string("\n") + flagNames[f];
				}

				flagsData = string(dbf) + flagsData;

				g_FontManager.DrawA(3, flagsData, 0x0035, 20, 102);
			}
		}
#endif //UO_DEBUG_INFO!=0

		g_GumpManager.Draw(false);
		
		// отрисовка ввода игрока
		g_GameConsole.DrawW((uchar)g_ConfigManager.SpeechFont, g_ConfigManager.SpeechColor, g_RenderBounds.GameWindowPosX, g_RenderBounds.GameWindowPosY + g_RenderBounds.GameWindowHeight - 18, TS_LEFT, UOFONT_BLACK_BORDER | UOFONT_FIXED);
		
		g_PluginManager.SceneDraw();

		if (g_GameState == GS_GAME_BLOCKED)
		{
			g_SelectedObject.Init(tempSelected);
			g_PressedObject.Init(tempPressed);

			g_GameBlockedScreen.Render(false);
			g_GameBlockedScreen.Render(true);
		}
		else
		{
			InitToolTip();

			g_MouseManager.Draw(g_MouseManager.GetGameCursor()); //Game Gump mouse cursor
		}

		g_GL.EndDraw();
	}
	else //Выбор объектов
	{
		GLdouble oldScale = g_GlobalScale;
		g_GlobalScale = 1.0;

		g_SelectedObject.Clear();
		g_StatusbarUnderMouse = 0;
		
		g_GumpManager.Select(false);

		if (g_SelectedObject.Gump != NULL)
		{
			if (g_SelectedObject.Object->IsText())
				((CRenderTextObject*)g_SelectedObject.Object)->ToTop();
		}
		else if (m_GameScreenGump.Select() == NULL)
		{
			if (!g_DeathScreenTimer)
			{
				//Проверка текста
				DrawGameWindowText(mode);

				if (g_SelectedObject.Object != NULL && g_SelectedObject.Object->IsText())
					g_WorldTextRenderer.ToTop((CRenderTextObject*)g_SelectedObject.Object);
			}
		
			if (g_SelectedObject.Object == NULL) //Если ничего не выбралось - пройдемся по объектам
			{
				//Если курсор мыши в игровом окне - просканируем его
				if (g_MouseManager.Position.X < g_RenderBounds.GameWindowPosX || g_MouseManager.Position.Y < g_RenderBounds.GameWindowPosY ||
					g_MouseManager.Position.X > (g_RenderBounds.GameWindowPosX + g_RenderBounds.GameWindowWidth) ||
					g_MouseManager.Position.Y > (g_RenderBounds.GameWindowPosY + g_RenderBounds.GameWindowHeight))
				{}
				else
				{
					g_GlobalScale = oldScale;
					WISP_GEOMETRY::CPoint2Di oldMouse = g_MouseManager.Position;

					//g_MouseManager.Position = WISP_GEOMETRY::CPoint2Di((int)((oldMouse.X - (g_RenderBounds.GameWindowScaledOffsetX / g_GlobalScale)) * g_GlobalScale) + g_RenderBounds.GameWindowScaledOffsetX, (int)((oldMouse.Y - (g_RenderBounds.GameWindowScaledOffsetY / g_GlobalScale)) * g_GlobalScale) + g_RenderBounds.GameWindowScaledOffsetY);

					//g_MouseManager.Position = WISP_GEOMETRY::CPoint2Di((int)((oldMouse.X * g_GlobalScale) + g_RenderBounds.GameWindowScaledOffsetX / g_GlobalScale), (int)((oldMouse.Y * g_GlobalScale) + g_RenderBounds.GameWindowScaledOffsetY / g_GlobalScale));

					int mouseX = (int)((oldMouse.X * g_GlobalScale) + g_RenderBounds.GameWindowScaledOffsetX / g_GlobalScale);
					int mouseY = (int)((oldMouse.Y * g_GlobalScale) + g_RenderBounds.GameWindowScaledOffsetY / g_GlobalScale);

					/*g_MouseManager.Position = WISP_GEOMETRY::CPoint2Di
					(
						//(int)((oldMouse.X * g_GlobalScale) + g_RenderBounds.GameWindowScaledOffsetX)
						mouseX
						,
						//(int)((oldMouse.Y * g_GlobalScale) + g_RenderBounds.GameWindowScaledOffsetY)
						mouseY
					);*/

					/*GLdouble left = (GLdouble)g_RenderBounds.GameWindowPosX;
					GLdouble right = (GLdouble)(g_RenderBounds.GameWindowWidth + left);
					GLdouble top = (GLdouble)g_RenderBounds.GameWindowPosY;
					GLdouble bottom = (GLdouble)(g_RenderBounds.GameWindowHeight + top);

					GLdouble newRight = right * g_GlobalScale;
					GLdouble newBottom = bottom * g_GlobalScale;

					g_RenderBounds.GameWindowScaledOffsetX = (int)((left * g_GlobalScale) - (newRight - right));
					g_RenderBounds.GameWindowScaledOffsetY = (int)((top * g_GlobalScale) - (newBottom - bottom));

					g_RenderBounds.GameWindowScaledWidth = (int)(newRight - g_RenderBounds.GameWindowScaledOffsetX);
					g_RenderBounds.GameWindowScaledHeight = (int)(newBottom - g_RenderBounds.GameWindowScaledOffsetY);*/

					DrawGameWindow(mode);

					g_MouseManager.Position = oldMouse;
				}
			}
		}

		g_GlobalScale = oldScale;

		if (g_SelectedObject.Object != g_LastSelectedObject.Object)
		{
			if (g_LastSelectedObject.Object != NULL)
				g_LastSelectedObject.Object->OnMouseExit();

			if (g_SelectedObject.Object != NULL)
				g_SelectedObject.Object->OnMouseEnter();
		}

		g_LastSelectedObject.Init(g_SelectedObject);
	}
}
//----------------------------------------------------------------------------------
/*!
Нажатие левой кнопки мыши
@return
*/
void CGameScreen::OnLeftMouseButtonDown()
{
	WISPFUN_DEBUG("c164_f20");
	CGumpSkills *skillGump = (CGumpSkills*)g_GumpManager.GetGump(0, 0, GT_SKILLS);

	if (skillGump != NULL)
		skillGump->UpdateGroupText();

	if (g_SelectedObject.Gump == &m_GameScreenGump)
		m_GameScreenGump.OnLeftMouseButtonDown();
	else if (g_SelectedObject.Gump != NULL)
	{
		//g_SelectGumpObjects = true;

		g_GumpManager.OnLeftMouseButtonDown(false);

		//g_SelectGumpObjects = false;
	}

	if (g_PopupMenu != NULL && g_SelectedObject.Gump != g_PopupMenu)
		g_GumpManager.RemoveGump(g_PopupMenu);
}
//----------------------------------------------------------------------------------
/*!
Отпускание левой кнопки мыши
@return 
*/
void CGameScreen::OnLeftMouseButtonUp()
{
	WISPFUN_DEBUG("c164_f21");
	if (g_PressedObject.LeftGump == &m_GameScreenGump)
	{
		m_GameScreenGump.OnLeftMouseButtonUp();
		return;
	}
	else if (g_MouseManager.LeftButtonPressed && (g_PressedObject.LeftGump != NULL || g_ObjectInHand.Enabled))
	{
		if (g_GumpManager.OnLeftMouseButtonUp(false))
			return;
	}
	
	int gameWindowPosX = g_ConfigManager.GameWindowX - 4;
	int gameWindowPosY = g_ConfigManager.GameWindowY - 4;

	if (g_MouseManager.Position.X < gameWindowPosX || g_MouseManager.Position.Y < gameWindowPosY || g_MouseManager.Position.X > (gameWindowPosX + g_ConfigManager.GameWindowWidth) ||
		g_MouseManager.Position.Y > (gameWindowPosY + g_ConfigManager.GameWindowHeight))
		return;

	if (g_SelectedObject.Object != NULL)
	{
		CRenderWorldObject *rwo = NULL;

		if (g_SelectedObject.Object->IsWorldObject())
			rwo = (CRenderWorldObject*)g_SelectedObject.Object;

		if (g_CustomHouseGump != NULL && g_Target.IsTargeting())
		{
			g_CustomHouseGump->OnTargetWorld(rwo);

			g_MouseManager.LastLeftButtonClickTimer = 0;

			return;
		}

		if (g_Target.IsTargeting() && !g_ObjectInHand.Enabled)
		{
			if (g_SelectedObject.Object->IsText())
			{
				CTextData *td = (CTextData*)g_SelectedObject.Object;

				if (td->Type == TT_OBJECT)
				{
					CGameObject *obj = g_World->FindWorldObject(td->Serial);

					if (obj != NULL && (obj->NPC || obj->IsCorpse()))
						g_Target.SendTargetObject(td->Serial);
				}

				g_MouseManager.LastLeftButtonClickTimer = 0;

				return;
			}
			else if (rwo != NULL)
			{
				if (rwo->IsGameObject())
					g_Target.SendTargetObject(rwo->Serial);
				else if (rwo->IsLandObject())
					g_Target.SendTargetTile(0/*g_SelectedObject->Index*/, rwo->X, rwo->Y, rwo->Z);
				else if (rwo->IsStaticObject() || rwo->IsMultiObject())
				{
					STATIC_TILES *st = NULL;
					
					if (g_PacketManager.ClientVersion >= CV_7090 && rwo->IsSurface())
						st = ((CRenderStaticObject*)rwo)->GetStaticData();

					short targetZ = rwo->Z;

					if (st != NULL)
						targetZ += st->Height;

					g_Target.SendTargetTile(rwo->Graphic, rwo->X, rwo->Y, (char)targetZ);
				}

				g_MouseManager.LastLeftButtonClickTimer = 0;

				return;
			}
		}

		if (rwo != NULL)
		{
			uint drop_container = 0xFFFFFFFF;
			bool can_drop = false;
			ushort dropX = 0;
			ushort dropY = 0;
			char dropZ = 0;

			if (rwo->IsGameObject() && g_ObjectInHand.Enabled)
			{
				CGameObject *target = (CGameObject*)rwo;

				can_drop = (GetDistance(g_Player, target) <= DRAG_ITEMS_DISTANCE);

				if (can_drop && target != NULL)
				{
					if (target->IsContainer() || target->NPC)
					{
						dropX = 0xFFFF;
						dropY = 0xFFFF;
						dropZ = 0;

						drop_container = target->Serial;
					}
					else if (target->IsSurface() || (target->IsStackable() && target->Graphic == g_ObjectInHand.Graphic))
					{
						if (!target->IsSurface())
							drop_container = target->Serial;

						dropX = target->X;
						dropY = target->Y;
						dropZ = target->Z;
					}
				}
				else
					g_Orion.PlaySoundEffect(0x0051);
			}
			else if ((rwo->IsLandObject() || rwo->IsStaticObject() || rwo->IsMultiObject()) && g_ObjectInHand.Enabled)
			{
				can_drop = (GetDistance(g_Player, WISP_GEOMETRY::CPoint2Di(rwo->X, rwo->Y)) <= DRAG_ITEMS_DISTANCE);

				if (can_drop)
				{
					dropX = rwo->X;
					dropY = rwo->Y;
					dropZ = rwo->Z;
				}
				else
					g_Orion.PlaySoundEffect(0x0051);
			}
	
			if (can_drop /*&& ObjectInHand != NULL*/)
			{
				if (drop_container == 0xFFFFFFFF && !dropX && !dropY)
					can_drop = false;

				if (can_drop)
					g_Orion.DropItem(drop_container, dropX, dropY, dropZ);
			}
			else if (!g_ObjectInHand.Enabled)
			{
				if (rwo->IsGameObject())
				{
					if (!g_ClickObject.Enabled)
					{
						g_ClickObject.Init(rwo);
						g_ClickObject.Timer = g_Ticks + g_MouseManager.DoubleClickDelay;
					}
				}
				else
				{
					if (rwo->IsStaticObject() || rwo->IsMultiObject())
					{
						CTextData *td = (CTextData*)rwo->StaticGroupObjectPtr()->m_TextControl->m_Items;

						if (td == NULL || td->Timer < g_Ticks)
						{
							ushort id = rwo->Graphic;

							string str = g_ClilocManager.Cliloc(g_Language)->GetA(1020000 + id, true, g_Orion.m_StaticData[id].Name);

							if (str.length())
							{
								if (g_PacketManager.ClientVersion >= CV_6000)
									g_Orion.CreateUnicodeTextMessage(TT_CLIENT, (uint)rwo, 3, 946, ToWString(str));
								else
									g_Orion.CreateTextMessage(TT_CLIENT, (uint)rwo, 3, 0x3B2, str);
							}
								
			
						}
					}
				}
			}
		}
	}

	if (g_EntryPointer != &g_GameConsole && g_EntryPointer != NULL)
	{
		CGump *gumpEntry = g_GumpManager.GetTextEntryOwner();

		if (g_ConfigManager.ConsoleNeedEnter)
			g_EntryPointer = NULL;
		else
			g_EntryPointer = &g_GameConsole;

		if (gumpEntry != NULL)
			gumpEntry->FrameCreated = false;
	}
}
//----------------------------------------------------------------------------------
/*!
Двойной клик левой кнопкой мыши
@return true при успешной обработке
*/
bool CGameScreen::OnLeftMouseButtonDoubleClick()
{
	WISPFUN_DEBUG("c164_f22");
	bool result = false;
	uint charUnderMouse = 0;

	if (g_SelectedObject.Gump && g_GumpManager.OnLeftMouseButtonDoubleClick(false))
		result = true;
	else if (g_SelectedObject.Object != NULL)
	{
		if (g_SelectedObject.Object->IsGameObject())
		{
			CGameObject *obj = (CGameObject*)g_SelectedObject.Object;

			if (!obj->NPC)
				g_Orion.DoubleClick(g_SelectedObject.Serial);
			else
				charUnderMouse = g_SelectedObject.Serial;

			result = true;
		}
		else if (g_SelectedObject.Object->IsText())
		{
			CTextData *td = (CTextData*)g_SelectedObject.Object;

			if (td->Type == TT_OBJECT)
			{
				uint serial = td->Serial;

				CGameObject *obj = g_World->FindWorldObject(serial);

				if (obj != NULL && (obj->NPC || obj->IsCorpse()))
				{
					if (obj->NPC)
						charUnderMouse = serial;
					else
						g_Orion.DoubleClick(serial);

					result = true;
				}
			}
		}
	}

	if (charUnderMouse != 0)
	{
		if (!g_ConfigManager.DisableNewTargetSystem && (charUnderMouse != g_PlayerSerial || g_Player->FindLayer(OL_MOUNT) == NULL))
		{
			g_GumpManager.CloseGump(g_NewTargetSystem.Serial, 0, GT_TARGET_SYSTEM);
			g_NewTargetSystem.Serial = charUnderMouse;

			if (g_GumpManager.UpdateContent(charUnderMouse, 0, GT_TARGET_SYSTEM) == NULL)
			{
				CPacketStatusRequest(charUnderMouse).Send();

				g_GumpManager.AddGump(new CGumpTargetSystem(charUnderMouse, g_NewTargetSystem.GumpX, g_NewTargetSystem.GumpY));
			}
		}

		if (g_Player->Warmode && charUnderMouse != g_PlayerSerial)
			g_Orion.Attack(charUnderMouse);
		else
			g_Orion.DoubleClick(charUnderMouse);
	}

	return result;
}
//----------------------------------------------------------------------------------
/*!
Нажатие правой кнопки мыши
@return
*/
void CGameScreen::OnRightMouseButtonDown()
{
	WISPFUN_DEBUG("c164_f23");
	if (g_PressedObject.RightGump != NULL)
		g_GumpManager.OnRightMouseButtonDown(false);

	if (g_PopupMenu != NULL && g_SelectedObject.Gump != g_PopupMenu)
		g_GumpManager.RemoveGump(g_PopupMenu);
}
//----------------------------------------------------------------------------------
/*!
Отпускание правой кнопки мыши
@return 
*/
void CGameScreen::OnRightMouseButtonUp()
{
	WISPFUN_DEBUG("c164_f24");
	if (g_PressedObject.RightGump != NULL)
		g_GumpManager.OnRightMouseButtonUp(false);
	else if (g_PressedObject.RightObject != NULL && g_PressedObject.RightObject->IsGameObject() && g_SelectedGameObjectHandle == g_PressedObject.RightSerial)
		((CGameObject*)g_PressedObject.RightObject)->ClosedObjectHandle = true;

	if ((g_ShiftPressed && !g_CtrlPressed && !g_AltPressed) && g_ConfigManager.HoldShiftForEnablePathfind && g_ConfigManager.EnablePathfind && g_SelectedObject.Object != NULL && g_SelectedObject.Object->IsWorldObject() && !g_PathFinder.AutoWalking)
	{
		CRenderWorldObject *rwo = (CRenderWorldObject*)g_SelectedObject.Object;

		if (rwo->IsLandObject() || rwo->IsSurface())
		{
			if (g_PathFinder.WalkTo(rwo->X, rwo->Y, rwo->Z, 0))
				g_Orion.CreateTextMessage(TT_OBJECT, g_PlayerSerial, 3, 0, "Pathfinding!");
		}
	}
}
//----------------------------------------------------------------------------------
/*!
Двойной клик правой кнопкой мыши
@return true при успешной обработке
*/
bool CGameScreen::OnRightMouseButtonDoubleClick()
{
	WISPFUN_DEBUG("c164_f25");
	if (g_ConfigManager.EnablePathfind && g_SelectedObject.Object != NULL && g_SelectedObject.Object->IsWorldObject() && !g_PathFinder.AutoWalking)
	{
		CRenderWorldObject *rwo = (CRenderWorldObject*)g_SelectedObject.Object;

		if (rwo->IsLandObject() || rwo->IsSurface())
		{
			if (g_PathFinder.WalkTo(rwo->X, rwo->Y, rwo->Z, 0))
			{
				g_Orion.CreateTextMessage(TT_OBJECT, g_PlayerSerial, 3, 0, "Pathfinding!");
				return true;
			}
		}
	}

	return false;
}
//----------------------------------------------------------------------------------
/*!
Обработка средней кнопки (колесика) мыши
@param [__in] state Состояние колесика
@return 
*/
void CGameScreen::OnMidMouseButtonScroll(const bool &up)
{
	WISPFUN_DEBUG("c164_f26");
	if (g_SelectedObject.Gump != NULL)
		g_GumpManager.OnMidMouseButtonScroll(up, false);
	else if (g_ConfigManager.UseScaling)
	{
		int gameWindowPosX = g_ConfigManager.GameWindowX - 4;
		int gameWindowPosY = g_ConfigManager.GameWindowY - 4;

		if (g_MouseManager.Position.X < gameWindowPosX || g_MouseManager.Position.Y < gameWindowPosY || g_MouseManager.Position.X >(gameWindowPosX + g_ConfigManager.GameWindowWidth) ||
			g_MouseManager.Position.Y >(gameWindowPosY + g_ConfigManager.GameWindowHeight))
			return;

		if (up)
			g_GlobalScale += 0.1;
		else
			g_GlobalScale -= 0.1;

		if (g_GlobalScale < 0.7)
			g_GlobalScale = 0.7;
		else if (g_GlobalScale > 2.3)
			g_GlobalScale = 2.3;
	}
}
//----------------------------------------------------------------------------------
void CGameScreen::OnDragging()
{
	WISPFUN_DEBUG("c164_f27");
	if (g_PressedObject.LeftGump != NULL)
		g_GumpManager.OnDragging(false);
}
//----------------------------------------------------------------------------------
/*!
Обработка нажатия клавиши
@param [__in] wparam не подписанный параметр
@param [__in] lparam не подписанный параметр
@return 
*/
void CGameScreen::OnCharPress(const WPARAM &wParam, const LPARAM &lParam)
{
	WISPFUN_DEBUG("c164_f28");
	if (g_EntryPointer == NULL)
		return; //Ignore no print keys

	if (g_EntryPointer != &g_GameConsole && wParam != 0x11 && wParam != 0x17)
	{
		if (g_GumpManager.OnCharPress(wParam, lParam, false))
			return;
	}

	bool altGR = (GetAsyncKeyState(VK_RMENU) & 0x80000000);
	bool altPressed = (GetAsyncKeyState(VK_MENU) & 0x80000000);
	bool ctrlPressed = (GetAsyncKeyState(VK_CONTROL) & 0x80000000);
	//bool shiftPressed = GetAsyncKeyState(VK_SHIFT) & 0x80000000;

	if (g_EntryPointer == &g_GameConsole && (wParam == 0x00000011 || wParam == 0x00000017) && ctrlPressed)
		g_GameConsole.ChangeConsoleMessage(wParam == 0x00000017);
	else if ((altGR || (!altPressed && !ctrlPressed)) && (int)g_EntryPointer->Length() < max(g_EntryPointer->MaxLength, 60))
		g_EntryPointer->Insert((wchar_t)wParam);
}
//----------------------------------------------------------------------------------
void CGameScreen::OnKeyDown(const WPARAM &wParam, const LPARAM &lParam)
{
	WISPFUN_DEBUG("c164_f29");
	if (wParam == VK_TAB && (lParam & 0x40000000))
		return;

	if (g_GumpManager.OnKeyDown(wParam, lParam, false))
		return;

	if (g_EntryPointer == &g_GameConsole)
		g_EntryPointer->OnKey(NULL, wParam);

	switch (wParam)
	{
		case VK_RETURN:
		{
			if (g_EntryPointer != NULL)
			{
				if (g_EntryPointer == &g_GameConsole)
				{
					if (g_ConsolePrompt)
						g_Orion.ConsolePromptSend();
					else if (g_EntryPointer->Length())
					{
						g_GameConsole.SaveConsoleMessage();

						g_GameConsole.Send();
					}

					g_GameConsole.Clear();
				}

				if (g_ConfigManager.GetConsoleNeedEnter())
					g_EntryPointer = NULL;
				else
					g_EntryPointer = &g_GameConsole;
			}
			else
				g_EntryPointer = &g_GameConsole;

			if (g_GumpConsoleType != NULL)
				g_GumpConsoleType->SetConsolePrefix();

			break;
		}
		case VK_PRIOR: //PgUp
		{
			//walk N (0)
			if (!g_PathFinder.AutoWalking)
				g_PathFinder.Walk(g_ConfigManager.AlwaysRun, 0);

			break;
		}
		case VK_NEXT: //PgDown
		{
			//walk E (2)
			if (!g_PathFinder.AutoWalking)
				g_PathFinder.Walk(g_ConfigManager.AlwaysRun, 2);

			break;
		}
		case VK_HOME:
		{
			//walk W (6)
			if (!g_PathFinder.AutoWalking)
				g_PathFinder.Walk(g_ConfigManager.AlwaysRun, 6);

			break;
		}
		case VK_END:
		{
			//walk S (4)
			if (!g_PathFinder.AutoWalking)
				g_PathFinder.Walk(g_ConfigManager.AlwaysRun, 4);

			break;
		}
		case VK_UP:
		{
			//Walk NW (7)
			if (!g_PathFinder.AutoWalking)
				g_PathFinder.Walk(g_ConfigManager.AlwaysRun, 7);

			break;
		}
		case VK_LEFT:
		{
			//Walk SW (5)
			if (!g_PathFinder.AutoWalking)
				g_PathFinder.Walk(g_ConfigManager.AlwaysRun, 5);

			break;
		}
		case VK_DOWN:
		{
			//Walk SE (3)
			if (!g_PathFinder.AutoWalking)
				g_PathFinder.Walk(g_ConfigManager.AlwaysRun, 3);

			break;
		}
		case VK_RIGHT:
		{
			//Walk NE (1)
			if (!g_PathFinder.AutoWalking)
				g_PathFinder.Walk(g_ConfigManager.AlwaysRun, 1);

			break;
		}
		case VK_ESCAPE:
		{
			if (g_Target.IsTargeting())
				g_Target.SendCancelTarget();
			else if (g_NewTargetSystem.Serial)
				g_NewTargetSystem.Serial = 0;
			else if (g_PathFinder.AutoWalking && g_PathFinder.PathFindidngCanBeCancelled)
				g_PathFinder.StopAutoWalk();

			if (g_ConsolePrompt)
				g_Orion.ConsolePromptCancel();

			break;
		}
		default:
			break;
	}

	if (wParam == VK_TAB)
	{
		if (g_ConfigManager.HoldTabForCombat)
			g_Orion.ChangeWarmode(1);
		else
			g_Orion.ChangeWarmode();
	}

	//if (g_MacroPointer == NULL)
	{
		bool altPressed = GetAsyncKeyState(VK_MENU) & 0x80000000;
		bool ctrlPressed = GetAsyncKeyState(VK_CONTROL) & 0x80000000;
		bool shiftPressed = GetAsyncKeyState(VK_SHIFT) & 0x80000000;

		CMacro *macro = g_MacroManager.FindMacro((ushort)wParam, altPressed, ctrlPressed, shiftPressed);

		if (macro != NULL)
		{
			g_MacroManager.ChangePointer((CMacroObject*)macro->m_Items);

			g_MacroManager.WaitingBandageTarget = false;
			g_MacroManager.WaitForTargetTimer = 0;
			g_MacroManager.Execute();
		}
	}
}
//----------------------------------------------------------------------------------
void CGameScreen::OnKeyUp(const WPARAM &wParam, const LPARAM &lParam)
{
	WISPFUN_DEBUG("c164_f30");
	if (wParam == VK_TAB && g_GameState == GS_GAME)
	{
		if (g_ConfigManager.HoldTabForCombat)
			g_Orion.ChangeWarmode(0);
	}
}
//----------------------------------------------------------------------------------
