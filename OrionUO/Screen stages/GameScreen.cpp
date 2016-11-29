/***********************************************************************************
**
** GameScreen.cpp
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#include "GameScreen.h"
#include "GameBlockedScreen.h"
#include "../Gumps/Gump.h"
#include "../OrionUO.h"
#include "../OrionWindow.h"
#include "../Managers/ClilocManager.h"
#include "../Managers/ConfigManager.h"
#include "../Managers/FontsManager.h"
#include "../Managers/MapManager.h"
#include "../Managers/ColorManager.h"
#include "../Managers/AnimationManager.h"
#include "../Managers/GumpManager.h"
#include "../Managers/MacroManager.h"
#include "../Managers/PacketManager.h"
#include "../SelectedObject.h"
#include "../PressedObject.h"
#include "../Game objects/GameWorld.h"
#include "../Game objects/GamePlayer.h"
#include "../ImageBounds.h"
#include "../TextEngine/TextRenderer.h"
#include "../GLEngine/GLTextureCircleOfTransparency.h"
#include "../Game objects/ObjectOnCursor.h"
#include "../Target.h"
#include "../TargetGump.h"
#include "../Weather.h"
#include "../QuestArrow.h"
#include "../TextEngine/TextContainer.h"
#include "../TextEngine/GameConsole.h"
#include "../Walker/PathFinder.h"
#include "../Network/Packets.h"
#include "../Gumps/GumpConsoleType.h"
#include "../ScreenshotBuilder.h"
#include "../Gumps/GumpDrag.h"
#include "../ClickObject.h"
#include "../Macro.h"
#include "../Gumps/GumpSkills.h"
#include "../Gumps/GumpTargetSystem.h"
#include "../Gumps/GumpPopupMenu.h"
#include "../Managers/PluginManager.h"
//----------------------------------------------------------------------------------
CGameScreen g_GameScreen;
RENDER_VARIABLES_FOR_GAME_WINDOW g_RenderBounds;
//----------------------------------------------------------------------------------
CGameScreen::CGameScreen()
: CBaseScreen(m_GameScreenGump), m_UseLight(false), m_MaxDrawZ(0), m_RenderListSize(1000),
m_RenderListInitalized(false), m_RenderListCount(0), m_ObjectHandlesCount(0)
{
	m_RenderList = new RENDER_OBJECT_DATA[1000];

	memset(&g_RenderBounds, 0, sizeof(RENDER_VARIABLES_FOR_GAME_WINDOW));

	memset(&m_ObjectHandlesList[0], 0, sizeof(OBJECT_HANDLES_DATA) * MAX_OBJECT_HANDLES);
}
//----------------------------------------------------------------------------------
CGameScreen::~CGameScreen()
{
	if (m_RenderList != NULL)
	{
		delete m_RenderList;
		m_RenderList = NULL;
	}
}
//----------------------------------------------------------------------------------
/*!
Инициализация
@return 
*/
void CGameScreen::Init()
{
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
	g_FontManager.SetUseHTML(true);
	g_FontManager.RecalculateWidthByInfo = true;

	if (g_SelectedObject.Gump())
	{
		if (g_SelectedObject.Gump() == &m_GameScreenGump)
		{
			if (g_ConfigManager.UseToolTips)
				m_GameScreenGump.InitToolTip();
		}
		else if (g_ConfigManager.UseToolTips || g_PacketManager.ClientVersion >= CV_308Z)
			g_GumpManager.InitToolTip();
	}
	else if (g_SelectedObject.Object() != NULL && g_SelectedObject.Object()->IsGameObject() && (g_ConfigManager.UseToolTips || g_PacketManager.ClientVersion >= CV_308Z))
	{
		CGameObject *obj = g_World->FindWorldObject(g_SelectedObject.Serial);

		if (obj != NULL && (obj->NPC || !obj->Locked()) && obj->ClilocMessage.length())
			g_ToolTip.Set(obj->ClilocMessage, g_SelectedObject.Object());
	}

	g_FontManager.RecalculateWidthByInfo = false;
	g_FontManager.SetUseHTML(false);
}
//----------------------------------------------------------------------------------
/*!
Вычисление значений Z координаты для рендера
@param [__out] noDrawRoof Не рисовать крыши и верхние этажи строений
@param [__out] maxGroundZ Максимальная Z координата ландшафта
@return Максимальная Z координата отрисовки
*/
int CGameScreen::GetMaxDrawZ(bool &noDrawRoof, char &maxGroundZ)
{
	int playerX = g_Player->X;
	int playerY = g_Player->Y;
	int playerZ = g_Player->Z;

	int maxDrawZ = 125; //playerZ + 15;

	int bx = playerX / 8;
	int by = playerY / 8;

	int blockIndex = (bx * g_MapBlockSize[g_CurrentMap].Height) + by;
	CMapBlock *mb = g_MapManager->GetBlock(blockIndex);

	bx = (playerX + 1) / 8;
	by = (playerY + 1) / 8;

	blockIndex = (bx * g_MapBlockSize[g_CurrentMap].Height) + by;
	CMapBlock *mb11 = g_MapManager->GetBlock(blockIndex);

	if (mb != NULL)
	{
		int x = playerX % 8;
		int y = playerY % 8;

		maxDrawZ = 125;
		int pz15 = playerZ + 15;
		int pz5 = playerZ + 5;

		for (CRenderWorldObject *ro = mb->GetRender(x, y); ro != NULL; ro = ro->m_NextXY)
		{
			CLandObject *land = ro->LandObjectPtr();

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

			if (!ro->IsGameObject())
			{
				if (!ro->IsStaticObject() && !ro->IsMultiObject())
					continue;
			}
			else if (((CGameObject*)ro)->NPC)
				continue;
	
			if (ro->Z >= pz15 && maxDrawZ > ro->Z && !ro->IsRoof() && (ro->IsSurface() || ro->IsImpassable()))
			{
				maxDrawZ = pz15 + 1; // ro->Z;

				noDrawRoof = true;
			}
			else  if (ro->Z > pz5 && (ro->IsRoof() || (ro->IsBackground() && ro->IsSurface())))
			{
				bool canNoRoof = !ro->IsRoof();

				if (!canNoRoof && mb11 != NULL)
				{
					x = (playerX + 1) % 8;
					y = (playerY + 1) % 8;
					
					for (CRenderWorldObject *ro11 = mb11->GetRender(x, y); ro11 != NULL; ro11 = ro11->m_NextXY)
					{
						if (!ro11->IsGameObject())
						{
							if (!ro11->IsStaticObject() && !ro11->IsMultiObject())
								continue;
						}
						else if (((CGameObject*)ro11)->NPC)
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
	int bx = x / 8;
	int by = y / 8;

	int blockIndex = (bx * g_MapBlockSize[g_CurrentMap].Height) + by;
	CMapBlock *mb = g_MapManager->GetBlock(blockIndex);

	if (mb != NULL)
	{
		int tx = x % 8;
		int ty = y % 8;

		for (CRenderWorldObject *obj = mb->GetRender(tx, ty); obj != NULL; obj = obj->m_NextXY)
		{
			if (obj->Graphic == graphic && obj->Z == z)
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
//----------------------------------------------------------------------------------
/*!
Функция увеличения размера списка рендера
@return 
*/
void CGameScreen::IncreaseRenderList()
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
//----------------------------------------------------------------------------------
/*!
Расчет списка объектов рендера, вычисление прозрачности крон деревьев (в т.ч. составных)
@return 
*/
void CGameScreen::CalculateRenderList()
{
	if (g_Target.IsTargeting() && g_Target.MultiGraphic && g_SelectedObject.Object() != NULL && g_SelectedObject.Object()->IsWorldObject())
	{
		int grZ = 0;
		int stZ = 0;
		CRenderObject *sel = g_SelectedObject.Object();
		g_MapManager->GetMapZ(sel->X, sel->Y, grZ, stZ);
		g_Target.LoadMulti(sel->X, sel->Y, grZ);
	}

	g_FoliageIndex++;

	if (g_FoliageIndex >= 100)
		g_FoliageIndex = 1;

	m_ObjectHandlesCount = 0;
	m_RenderListCount = 0;
	int objectHandlesOffsetX = g_ObjectHandlesWidth / 2;
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

	static uchar renderIndex = 1;

#if UO_CHECKERBOARD_SEQUENCE_RENDER_LIST == 1
	int minX = g_RenderBounds.RealMinRangeX;
	int minY = g_RenderBounds.RealMinRangeY;

	int maxX = g_RenderBounds.RealMaxRangeX;
	int maxY = g_RenderBounds.RealMaxRangeY;

	int mapBlockHeight = g_MapBlockSize[g_MapManager->GetActualMap()].Height;
	uint maxBlockIndex = g_MapManager->MaxBlockIndex;

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
					int offsetX = x - g_RenderBounds.PlayerX;
					int offsetY = y - g_RenderBounds.PlayerY;

					int drawX = g_RenderBounds.GameWindowCenterX + (offsetX - offsetY) * 22;
					int drawY = g_RenderBounds.GameWindowCenterY + (offsetX + offsetY) * 22;

					if (drawX >= g_RenderBounds.MinPixelsX && drawX <= g_RenderBounds.MaxPixelsX)
					{
						int blockX = x / 8;
						int blockY = y / 8;

						uint blockIndex = (blockX * mapBlockHeight) + blockY;

						if (blockIndex < maxBlockIndex)
						{
							CMapBlock *block = g_MapManager->GetBlock(blockIndex);

							if (block == NULL)
							{
								block = g_MapManager->AddBlock(blockIndex);
								block->X = blockX;
								block->Y = blockY;
								g_MapManager->LoadBlock(block);
							}

							AddTileToRenderList(block->GetRender(x % 8, y % 8), drawX, drawY, x, y, renderIndex, useObjectHandles, objectHandlesOffsetX);
						}
						else
							LOG("Expected: %i %i\n", blockIndex, g_MapManager->MaxBlockIndex);
					}
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

				int offsetX = currentX - g_RenderBounds.PlayerX;

				IFOR(y, 0, 8)
				{
					int currentY = by * 8 + y;

					if (currentY < g_RenderBounds.RealMinRangeY || currentY > g_RenderBounds.RealMaxRangeY)
						continue;

					int offsetY = currentY - g_RenderBounds.PlayerY;

					int drawX = g_RenderBounds.GameWindowCenterX + (offsetX - offsetY) * 22;
					int drawY = g_RenderBounds.GameWindowCenterY + (offsetX + offsetY) * 22;

					if (drawX < g_RenderBounds.MinPixelsX || drawX > g_RenderBounds.MaxPixelsX)
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
//----------------------------------------------------------------------------------
void CGameScreen::AddTileToRenderList(CRenderWorldObject *obj, const int &drawX, const int &drawY, const int &worldX, const int &worldY, const uchar &renderIndex, const bool &useObjectHandles, const int &objectHandlesOffsetX, const int &maxZ)
{
	ushort grayColor = 0;

	if (g_ConfigManager.GrayOutOfRangeObjects)
	{
		if (GetDistance(g_Player, WISP_GEOMETRY::CPoint2Di(worldX, worldY)) > g_ConfigManager.UpdateRange)
			grayColor = 0x038E;
	}

	for (; obj != NULL; obj = obj->m_NextXY)
	{
#if UO_RENDER_LIST_SORT == 1
		if (obj->CurrentRenderIndex == renderIndex)
			continue;

		int maxObjectZ = obj->Z + obj->RenderQueueIndex;

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
					continue;
				else if (g_ConfigManager.NoVegetation && rso->Vegetation)
					continue;

				maxObjectZ += rso->GetStaticHeight();
			}
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
			int characterOffsetY = 0;

			if (go->NPC)
			{
				CGameCharacter *character = go->GameCharacterPtr();

				ANIMATION_DIMENSIONS dims = g_AnimationManager.GetAnimationDimensions(go);

				characterOffsetY = character->OffsetY - (character->OffsetZ + dims.Height + dims.CenterY);

				CTextContainer &textContainer = character->m_DamageTextControl;

				if (!textContainer.Empty())
				{
					int textDrawX = drawX + character->OffsetX;
					int textDrawY = drawY + characterOffsetY;

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

						text->DrawX = textDrawX - text->X;
						text->DrawY = textDrawY + text->Y;

						if (text->Transparent)
						{
							if ((uchar)text->Color >= DAMAGE_TEXT_ALPHA_STEP)
								text->Color -= DAMAGE_TEXT_ALPHA_STEP;
							else
								text->Color = 0;
						}

						text->Y -= DAMAGE_TEXT_STEP;

						text = next;
					}
				}
			}

			if ((go->NPC || (!go->Locked() && !((CGameItem*)go)->MultiBody)) && useObjectHandles) // && m_ObjectHandlesCount < MAX_OBJECT_HANDLES)
			{
				int index = m_ObjectHandlesCount % MAX_OBJECT_HANDLES;

				m_ObjectHandlesList[index].Obj = go;
				m_ObjectHandlesList[index].X = drawX - objectHandlesOffsetX;
				m_ObjectHandlesList[index].Y = drawY - (go->Z * 4);

				if (go->NPC)
				{
					m_ObjectHandlesList[index].X += go->GameCharacterPtr()->OffsetX;
					m_ObjectHandlesList[index].Y += characterOffsetY;
				}
				else
					m_ObjectHandlesList[index].Y -= g_Orion.GetArtDimension(go->Graphic + 0x4000).Height;

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

			bool foliageCanBeChecked = (g_RenderBounds.PlayerX <= worldX && g_RenderBounds.PlayerY <= worldY);

			if (!foliageCanBeChecked)
			{
				foliageCanBeChecked = (g_RenderBounds.PlayerY <= worldY && g_RenderBounds.PlayerX <= worldX + 1);

				if (!foliageCanBeChecked)
					foliageCanBeChecked = (g_RenderBounds.PlayerX <= worldX && g_RenderBounds.PlayerY <= worldY + 1);
			}

			if (foliageCanBeChecked)
			{
				WISP_GEOMETRY::CSize fp = g_Orion.GetArtDimension(obj->Graphic);

				CImageBounds fib(drawX - fp.Width / 2, drawY - fp.Height - (z * 4), fp.Width, fp.Height);

				if (fib.InRect(g_PlayerRect))
				{
					WISP_GEOMETRY::CRect realRect = g_Orion.GetStaticArtRealPixelDimension(obj->Graphic - 0x4000);

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

		if (m_RenderListCount >= m_RenderListSize)
			IncreaseRenderList();

		m_RenderList[m_RenderListCount].Object = obj;
		m_RenderList[m_RenderListCount].X = drawX;
		m_RenderList[m_RenderListCount].Y = drawY;
		m_RenderList[m_RenderListCount].GrayColor = grayColor;

		m_RenderListCount++;
	}
}
//----------------------------------------------------------------------------------
void CGameScreen::AddOffsetCharacterTileToRenderList(CGameObject *obj, int drawX, int drawY, const uchar &renderIndex, const bool &useObjectHandles, const int &objectHandlesOffsetX)
{
	int characterX = obj->X;
	int characterY = obj->Y;

	drawY -= (obj->Z * 4);

	ANIMATION_DIMENSIONS dims = g_AnimationManager.GetAnimationDimensions(obj);
	CGameCharacter *character = obj->GameCharacterPtr();

	DRAW_FRAME_INFORMATION &dfInfo = obj->m_FrameInfo;
	int offsetY = dfInfo.Height - dfInfo.OffsetY;

	if (character != NULL)
	{
		drawX += character->OffsetX;
		drawY += character->OffsetY - character->OffsetZ;

		//g_GL.DrawPolygone(drawX - dfInfo.OffsetX, drawY, dfInfo.Width, dfInfo.Height - dfInfo.OffsetY);
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

	int size = coordinates.size();

	int maxZ = obj->Z + obj->RenderQueueIndex;

	int mapBlockHeight = g_MapBlockSize[g_CurrentMap].Height;
	uint maxBlockIndex = g_MapManager->MaxBlockIndex;

	IFOR(i, 0, size)
	{
		int x = coordinates[i].first;

		if (x < g_RenderBounds.RealMinRangeX || x > g_RenderBounds.RealMaxRangeX)
			continue;

		int y = coordinates[i].second;

		if (y < g_RenderBounds.RealMinRangeY || y > g_RenderBounds.RealMaxRangeY)
			continue;

		int offsetX = x - g_RenderBounds.PlayerX;
		int offsetY = y - g_RenderBounds.PlayerY;

		drawX = g_RenderBounds.GameWindowCenterX + (offsetX - offsetY) * 22;
		drawY = g_RenderBounds.GameWindowCenterY + (offsetX + offsetY) * 22;

		if (drawX < g_RenderBounds.MinPixelsX || drawX > g_RenderBounds.MaxPixelsX)
			continue;

		int blockX = x / 8;
		int blockY = y / 8;

		uint blockIndex = (blockX * mapBlockHeight) + blockY;

		if (blockIndex < maxBlockIndex)
		{
			CMapBlock *block = g_MapManager->GetBlock(blockIndex);

			if (block == NULL)
			{
				block = g_MapManager->AddBlock(blockIndex);
				block->X = blockX;
				block->Y = blockY;
				g_MapManager->LoadBlock(block);
			}

			AddTileToRenderList(block->GetRender(x % 8, y % 8), drawX, drawY, x, y, renderIndex, useObjectHandles, objectHandlesOffsetX, maxZ);
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
	g_DrawFoliage = (g_Season < ST_WINTER) && !g_ConfigManager.DrawStumps;

	g_GrayedPixels = g_Player->Dead();

	if (g_GrayedPixels && g_Season != ST_DESOLATION)
		g_Orion.ChangeSeason(ST_DESOLATION, DEATH_MUSIC_INDEX);

	g_RenderBounds.PlayerX = g_Player->X;
	g_RenderBounds.PlayerY = g_Player->Y;
	g_RenderBounds.PlayerZ = g_Player->Z;

	g_RenderBounds.GameWindowPosX = g_ConfigManager.GameWindowX;
	g_RenderBounds.GameWindowPosY = g_ConfigManager.GameWindowY;

	g_RenderBounds.GameWindowWidth = g_ConfigManager.GameWindowWidth;
	g_RenderBounds.GameWindowHeight = g_ConfigManager.GameWindowHeight;

	m_GameScreenGump.UpdateContent();

	//int playerZOffset = (g_Player->Z * 4) - g_Player->OffsetZ;

	g_RenderBounds.GameWindowCenterX = g_RenderBounds.GameWindowPosX + g_RenderBounds.GameWindowWidth / 2;
	g_RenderBounds.GameWindowCenterY = (g_RenderBounds.GameWindowPosY + g_RenderBounds.GameWindowHeight / 2) + (g_Player->Z * 4);

	/*int earthquakeMagnitude = RandomInt(11);

	if (earthquakeMagnitude)
	{
		g_RenderBounds.GameWindowCenterX += RandomInt(earthquakeMagnitude * 3);
		g_RenderBounds.GameWindowCenterY += RandomInt(earthquakeMagnitude * 3);
	}*/

	g_RenderBounds.GameWindowCenterX -= (int)g_Player->OffsetX;
	g_RenderBounds.GameWindowCenterY -= (int)(g_Player->OffsetY - g_Player->OffsetZ);

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

	g_NoDrawRoof = false;
	g_MaxGroundZ = 125;
	m_MaxDrawZ = GetMaxDrawZ(g_NoDrawRoof, g_MaxGroundZ);

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

		int blockIndex = (bx * g_MapBlockSize[g_CurrentMap].Height) + by;
		CMapBlock *mb = g_MapManager->GetBlock(blockIndex);

		if (mb != NULL)
		{
			bx = testX % 8;
			by = testY % 8;
			
			char z5 = rwo->Z + 5;
		
			for (CRenderWorldObject *obj = mb->GetRender(bx, by); obj != NULL; obj = obj->m_NextXY)
			{
				if (!obj->IsStaticGroupObject() || (obj->IsGameObject() && ((CGameObject*)obj)->NPC))
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

			if (g_ConfigManager.ColoredLighting)
			{
				ushort graphic = lightObject->Graphic;

				if (!lightObject->IsGameObject())
					graphic -= 0x4000;

				m_Light[m_LightCount].Color = g_Orion.GetLightColor(graphic);
			}
			else
				m_Light[m_LightCount].Color = 0;

			m_Light[m_LightCount].DrawX = x;
			m_Light[m_LightCount].DrawY = y;

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

		uint ignoreSerial = 0;

		if (g_ObjectInHand != NULL)
			ignoreSerial = g_ObjectInHand->Serial;

		m_HitsStack.clear();

		IFOR(i, 0, m_RenderListCount)
		{
			RENDER_OBJECT_DATA &rod = m_RenderList[i];
			CRenderWorldObject *obj = rod.Object;

			if (obj != NULL)
			{
				if (obj->IsGameObject() && obj->Serial == ignoreSerial)
					continue;

				g_OutOfRangeColor = rod.GrayColor;

				g_UseCircleTrans = (g_ConfigManager.UseCircleTrans && obj->TranparentTest(playerZPlus5));

				obj->Draw(rod.X, rod.Y);

				if (g_ConfigManager.DrawStatusState && obj->IsGameObject() && ((CGameObject*)obj)->NPC)
				{
					CGameCharacter *gc = (CGameCharacter*)obj;

					ushort color = g_ConfigManager.GetColorByNotoriety(gc->Notoriety);

					int width = gc->MaxHits;

					if (width)
					{
						width = (gc->Hits * 100) / width;

						if (width > 100)
							width = 100;

						if (width < 1)
							width = 0;
					}

					if (g_ConfigManager.DrawStatusConditionState == DCSCS_ALWAYS ||
						(g_ConfigManager.DrawStatusConditionState == DCSCS_NOT_MAX && gc->Hits != gc->MaxHits) ||
						(g_ConfigManager.DrawStatusConditionState == DCSCS_LOWER && width < g_ConfigManager.DrawStatusConditionValue))
					{
						int x = rod.X + gc->OffsetX;
						int y = rod.Y + gc->OffsetY - (gc->Z * 4) - gc->OffsetZ;

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
								width = (34 * width) / 100;
						}

						OBJECT_HITS_INFO hitsInfo = { x, y, color, width };
						m_HitsStack.push_back(hitsInfo);
					}
				}
			}
		}

		glDisable(GL_DEPTH_TEST);

		UnuseShader();

		IFOR(i, 0, m_ObjectHandlesCount)
		{
			OBJECT_HANDLES_DATA &ohd = m_ObjectHandlesList[i];
			ohd.Obj->DrawObjectHandlesTexture(ohd.X, ohd.Y);
		}

		g_PluginManager.WorldDraw(); 
	}
	else
	{
		bool useCircleTrans = (g_ConfigManager.UseCircleTrans && g_Orion.CircleTransPixelsInXY());

		IFOR(i, 0, m_RenderListCount)
		{
			RENDER_OBJECT_DATA &rod = m_RenderList[i];
			CRenderWorldObject *obj = rod.Object;

			if (obj != NULL)
			{
				g_UseCircleTrans = (useCircleTrans && obj->TranparentTest(playerZPlus5));

				obj->Select(rod.X, rod.Y);
			}
		}

		IFOR(i, 0, m_ObjectHandlesCount)
		{
			OBJECT_HANDLES_DATA &ohd = m_ObjectHandlesList[i];
			ohd.Obj->SelectObjectHandlesTexture(ohd.X, ohd.Y);
		}
	}
}
//----------------------------------------------------------------------------------
/*!
Отображение источников света
@return 
*/
void CGameScreen::DrawGameWindowLight()
{
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

	if (!m_UseLight)
		return;

	g_LightColorizerShader->Use();

	if (g_GL.CanUseFrameBuffer)
	{
		if (g_LightBuffer.Ready() && g_LightBuffer.Use())
		{
			float newLightColor = ((32 - g_LightLevel + g_PersonalLightLevel) / 32.0f);

			if (!g_ConfigManager.DarkNights)
				newLightColor += 0.2f;

			glClearColor(newLightColor, newLightColor, newLightColor, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT);
			glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

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
	if (mode)
	{
		g_FontColorizerShader->Use();

		g_WorldTextRenderer.WorldDraw();

		UnuseShader();

		if (g_ConfigManager.DrawStatusState && m_HitsStack.size())
		{
			if (g_ConfigManager.DrawStatusState == DCSS_ABOVE)
			{
				for (vector<OBJECT_HITS_INFO>::iterator it = m_HitsStack.begin(); it != m_HitsStack.end(); it++)
				{
					CGLTextTexture *texture = (CGLTextTexture*)it->Width;
					texture->Draw(it->X, it->Y);
				}
			}
			else
			{
				g_ColorizerShader->Use();

				IFOR(i, 0, 2)
				{
					for (vector<OBJECT_HITS_INFO>::iterator it = m_HitsStack.begin(); it != m_HitsStack.end(); it++)
					{
						if (!i)
							g_Orion.DrawGump(0x1068, it->Color, it->X, it->Y);
						else if (it->Width)
							g_Orion.DrawGump(0x1069, 0x0044, it->X, it->Y, it->Width, 0);
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

							text->m_Texture.Draw(text->DrawX, text->DrawY);

							glDisable(GL_BLEND);
						}
						else
							text->m_Texture.Draw(text->DrawX, text->DrawY);
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
	g_WorldTextRenderer.CalculateWorldPositions(false);

	m_GameScreenGump.PrepareContent();

	g_GumpManager.PrepareContent();

	if (g_SelectedObject.Gump() != NULL && g_SelectedObject.Gump()->GumpType == GT_STATUSBAR && g_SelectedObject.Gump()->Serial != g_PlayerSerial)
		g_StatusbarUnderMouse = g_SelectedObject.Gump()->Serial;
	else
		g_StatusbarUnderMouse = 0;

	//if (g_SelectedObject.Object() != NULL && g_SelectedObject.Object()->IsGameObject() && g_PressedObject.LeftObject() == g_SelectedObject.Object())
	if (g_PressedObject.LeftObject() != NULL && g_PressedObject.LeftObject()->IsGameObject())
	{
		WISP_GEOMETRY::CPoint2Di offset = g_MouseManager.LeftDroppedOffset();

		if ((abs(offset.X) >= DRAG_PIXEL_RANGE || abs(offset.Y) >= DRAG_PIXEL_RANGE) || (g_MouseManager.LastLeftButtonClickTimer + g_MouseManager.DoubleClickDelay < g_Ticks))
		{
			CGameItem *selobj = g_World->FindWorldItem(g_PressedObject.LeftSerial);

			if (selobj != NULL && g_ObjectInHand == NULL && !selobj->Locked() && GetDistance(g_Player, selobj) < 3)
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
			else if (g_ObjectInHand == NULL)
			{
				CGameCharacter *selchar = g_World->FindWorldCharacter(g_PressedObject.LeftSerial);

				if (selchar != NULL) //Character selection
				{
					g_Orion.OpenStatus(selchar->Serial);
					g_GeneratedMouseDown = true;
					g_OrionWindow.EmulateOnLeftMouseButtonDown();
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
	if (!m_RenderListInitalized)
		CalculateRenderList();

	if (g_DeathScreenTimer < g_Ticks)
	{
		if (g_DeathScreenTimer && g_ScreenEffectManager.UseSunrise())
			m_SmoothScreenAction = 0;

		g_DeathScreenTimer = 0;
	}

	if (mode)
	{
		static uint lastRender = 0;
		static int currentFPS = 0;
		static int FPScount = 0;

		if (lastRender < g_Ticks)
		{
			FPScount = currentFPS;
			currentFPS = 0;
			lastRender = g_Ticks + 1000;
		}
		else
			currentFPS++;

		g_GL.BeginDraw();

		if (DrawSmoothMonitor())
			return;

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
		g_AttackTargetGump.Color = 0;
		g_NewTargetSystem.ColorGump = 0;

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
			g_CurrentShader = g_DeathShader;
		else
			g_CurrentShader = g_ColorizerShader;

		g_CurrentShader->Use();

		DrawGameWindow(mode);

		UnuseShader();

		if (!g_DeathScreenTimer)
		{
			if (!g_GrayedPixels)
			{
				DrawGameWindowLight();

				g_ColorizerShader->Use();

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

		//Восстанавливаем размеры рисуемой области
		g_GL.RestorePort();

		if (!g_DeathScreenTimer)
		{
			g_SystemChat.DrawSystemChat(g_RenderBounds.GameWindowPosX, g_RenderBounds.GameWindowPosY, g_RenderBounds.GameWindowHeight);

			g_QuestArrow.Draw();
		}

		m_GameScreenGump.Draw();

#if UO_DEBUG_INFO!=0
		if (g_DeveloperMode == DM_SHOW_FPS_ONLY)
		{
			char dbf[50] = { 0 };

			sprintf_s(dbf, "FPS=%i (%ims) ping=%i scale=%.1f", FPScount, g_FrameDelay[1], g_Ping, g_GlobalScale);

			g_FontManager.DrawA(3, dbf, 0x35, g_RenderBounds.GameWindowPosX + g_RenderBounds.GameWindowWidth + 10, g_RenderBounds.GameWindowPosY);
		}
		else if (g_DeveloperMode == DM_DEBUGGING)
		{
			char dbf[150] = { 0 };

			sprintf_s(dbf, "FPS=%i (%ims) ping=%i Dir=%i Z=%i (MDZ=%i) scale=%.1f", FPScount, g_FrameDelay[1], g_Ping, g_Player->Direction, g_RenderBounds.PlayerZ, m_MaxDrawZ, g_GlobalScale);

			g_FontManager.DrawA(3, dbf, 0x35, 20, 30);

			sprintf_s(dbf, "Rendered %i object counts:\nLand=%i Statics=%i Game=%i Multi=%i Lights=%i",
				g_RenderedObjectsCountInGameWindow, g_LandObjectsCount, g_StaticsObjectsCount, g_GameObjectsCount, g_MultiObjectsCount, m_LightCount);

			g_FontManager.DrawA(3, dbf, 0x35, 20, 54);

			if (g_SelectedObject.Object() != NULL && g_SelectedObject.Object()->IsWorldObject())
			{
				CRenderWorldObject *selRwo = (CRenderWorldObject*)g_SelectedObject.Object();
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
						break;
					}
					default:
						break;
				}

				int tz = selRwo->Z;

				//Если это тайл текстуры
				if (land != NULL && land->IsStretched)
					tz = (char)land->Serial;

				sprintf_s(dbf, "Selected:\n%s: G=0x%04X X=%i Y=%i Z=%i (%i) RQI=%i (SUM=%i)", soName, selRwo->Graphic, selRwo->X, selRwo->Y, selRwo->Z, tz, selRwo->RenderQueueIndex, selRwo->Z + selRwo->RenderQueueIndex);

				g_FontManager.DrawA(3, dbf, 0x35, 20, 102);
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

		if (g_SelectedObject.Gump() != NULL)
		{
			if (g_SelectedObject.Object()->IsText())
				((CRenderTextObject*)g_SelectedObject.Object())->ToTop();
		}
		else if (m_GameScreenGump.Select() == NULL)
		{
			if (!g_DeathScreenTimer)
			{
				//Проверка текста
				DrawGameWindowText(mode);

				if (g_SelectedObject.Object() != NULL && g_SelectedObject.Object()->IsText())
					g_WorldTextRenderer.ToTop((CRenderTextObject*)g_SelectedObject.Object());
			}
		
			if (g_SelectedObject.Object() == NULL) //Если ничего не выбралось - пройдемся по объектам
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

		if (g_SelectedObject.Object() != g_LastSelectedObject.Object())
		{
			if (g_LastSelectedObject.Object() != NULL)
				g_LastSelectedObject.Object()->OnMouseExit();

			if (g_SelectedObject.Object() != NULL)
				g_SelectedObject.Object()->OnMouseEnter();
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
	CGumpSkills *skillGump = (CGumpSkills*)g_GumpManager.GetGump(g_PlayerSerial, 0, GT_SKILLS);

	if (skillGump != NULL)
		skillGump->UpdateGroupText();

	if (g_SelectedObject.Gump() == &m_GameScreenGump)
		m_GameScreenGump.OnLeftMouseButtonDown();
	else if (g_SelectedObject.Gump() != NULL)
	{
		//g_SelectGumpObjects = true;

		g_GumpManager.OnLeftMouseButtonDown(false);

		//g_SelectGumpObjects = false;
	}

	if (g_PopupMenu != NULL && g_SelectedObject.Gump() != g_PopupMenu)
		g_GumpManager.RemoveGump(g_PopupMenu);
}
//----------------------------------------------------------------------------------
/*!
Отпускание левой кнопки мыши
@return 
*/
void CGameScreen::OnLeftMouseButtonUp()
{
	if (g_PressedObject.LeftGump() == &m_GameScreenGump)
	{
		m_GameScreenGump.OnLeftMouseButtonUp();
		return;
	}
	else if (g_MouseManager.LeftButtonPressed && (g_PressedObject.LeftGump() != NULL || g_ObjectInHand != NULL))
	{
		if (g_GumpManager.OnLeftMouseButtonUp(false))
			return;
	}
	
	int gameWindowPosX = g_ConfigManager.GameWindowX - 4;
	int gameWindowPosY = g_ConfigManager.GameWindowY - 4;

	if (g_MouseManager.Position.X < gameWindowPosX || g_MouseManager.Position.Y < gameWindowPosY || g_MouseManager.Position.X > (gameWindowPosX + g_ConfigManager.GameWindowWidth) ||
		g_MouseManager.Position.Y > (gameWindowPosY + g_ConfigManager.GameWindowHeight))
		return;

	if (g_SelectedObject.Object() != NULL)
	{
		CRenderWorldObject *rwo = NULL;

		if (g_SelectedObject.Object()->IsWorldObject())
			rwo = (CRenderWorldObject*)g_SelectedObject.Object();

		if (g_Target.IsTargeting())
		{
			if (g_SelectedObject.Object()->IsText())
			{
				CTextData *td = (CTextData*)g_SelectedObject.Object();

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
					g_Target.SendTargetTile(0/*g_SelectedObject->Graphic*/, rwo->X, rwo->Y, rwo->Z);
				else if (rwo->IsStaticObject() || rwo->IsMultiObject())
					g_Target.SendTargetTile(rwo->Graphic - 0x4000, rwo->X, rwo->Y, rwo->Z);

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

			if (rwo->IsGameObject() && g_ObjectInHand != NULL)
			{
				CGameObject *target = (CGameObject*)rwo;

				can_drop = (GetDistance(g_Player, target) < 3);

				if (can_drop && target != NULL)
				{
					if (target->IsContainer())
						drop_container = target->Serial;
					else if (target->IsStackable() && target->Graphic == g_ObjectInHand->Graphic)
						drop_container = target->Serial;
					else if (target->NPC)
						drop_container = target->Serial;

					//if (drop_container != 0xFFFFFFFF)
					{
						dropX = 0xFFFF; //target->X;
						dropY = 0xFFFF; //target->Y;
						dropZ = 0; //target->Z;
					}
				}
				else
					g_Orion.PlaySoundEffect(0x0051);
			}
			else if ((rwo->IsLandObject() || rwo->IsStaticObject() || rwo->IsMultiObject()) && g_ObjectInHand != NULL)
			{
				can_drop = (GetDistance(g_Player, WISP_GEOMETRY::CPoint2Di(rwo->X, rwo->Y)) < 3);

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
			else if (g_ObjectInHand == NULL)
			{
				if (rwo->IsGameObject())
				{
					if (!g_ClickObject.Enabled) // && g_PacketManager.ClientVersion < CV_308Z)
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
							ushort id = rwo->Graphic - 0x4000;

							string str = g_ClilocManager.Cliloc(g_Language)->GetA(102000 + id, g_Orion.m_StaticData[id / 32].Tiles[id % 32].Name);

							if (str.length())
								g_Orion.CreateTextMessage(TT_CLIENT, (uint)rwo, 3, 0x038F, str);
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
	bool result = false;
	uint charUnderMouse = 0;

	if (g_SelectedObject.Gump() && g_GumpManager.OnLeftMouseButtonDoubleClick(false))
		result = true;
	else if (g_SelectedObject.Object() != NULL)
	{
		if (g_SelectedObject.Object()->IsGameObject())
		{
			CGameObject *obj = (CGameObject*)g_SelectedObject.Object();

			if (!obj->NPC)
				g_Orion.DoubleClick(g_SelectedObject.Serial);
			else
				charUnderMouse = g_SelectedObject.Serial;

			result = true;
		}
		else if (g_SelectedObject.Object()->IsText())
		{
			CTextData *td = (CTextData*)g_SelectedObject.Object();

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
		//UO->PaperdollReq(charUnderMouse);
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
	if (g_PressedObject.RightGump() != NULL)
		g_GumpManager.OnRightMouseButtonDown(false);

	if (g_PopupMenu != NULL && g_SelectedObject.Gump() != g_PopupMenu)
		g_GumpManager.RemoveGump(g_PopupMenu);
}
//----------------------------------------------------------------------------------
/*!
Отпускание правой кнопки мыши
@return 
*/
void CGameScreen::OnRightMouseButtonUp()
{
	if (g_PressedObject.RightGump() != NULL)
		g_GumpManager.OnRightMouseButtonUp(false);

	if ((g_ShiftPressed && !g_CtrlPressed && !g_AltPressed) && g_ConfigManager.HoldShiftForEnablePathfind && g_ConfigManager.EnablePathfind && g_SelectedObject.Object() != NULL && g_SelectedObject.Object()->IsWorldObject() && !g_PathFinder.AutoWalking)
	{
		CRenderWorldObject *rwo = (CRenderWorldObject*)g_SelectedObject.Object();

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
	if (g_ConfigManager.EnablePathfind && g_SelectedObject.Object() != NULL && g_SelectedObject.Object()->IsWorldObject() && !g_PathFinder.AutoWalking)
	{
		CRenderWorldObject *rwo = (CRenderWorldObject*)g_SelectedObject.Object();

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
	if (g_SelectedObject.Gump() != NULL)
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
	if (g_PressedObject.LeftGump() != NULL)
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
	if (g_EntryPointer == NULL)
		return; //Ignore no print keys

	if (g_EntryPointer != &g_GameConsole && wParam != 0x11 && wParam != 0x17)
	{
		if (g_GumpManager.OnCharPress(wParam, lParam, false))
			return;
	}

	bool altPressed = GetAsyncKeyState(VK_MENU) & 0x80000000;
	bool ctrlPressed = GetAsyncKeyState(VK_CONTROL) & 0x80000000;
	//bool shiftPressed = GetAsyncKeyState(VK_SHIFT) & 0x80000000;

	if (g_EntryPointer == &g_GameConsole && (wParam == 0x00000011 || wParam == 0x00000017) && ctrlPressed)
		g_GameConsole.ChangeConsoleMessage(wParam == 0x00000017);
	else if (!altPressed && !ctrlPressed && g_EntryPointer->Length() < 60)
		g_EntryPointer->Insert(wParam);
}
//----------------------------------------------------------------------------------
void CGameScreen::OnKeyDown(const WPARAM &wParam, const LPARAM &lParam)
{
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

	if (g_MacroPointer == NULL)
	{
		bool altPressed = GetAsyncKeyState(VK_MENU) & 0x80000000;
		bool ctrlPressed = GetAsyncKeyState(VK_CONTROL) & 0x80000000;
		bool shiftPressed = GetAsyncKeyState(VK_SHIFT) & 0x80000000;

		CMacro *macro = g_MacroManager.FindMacro(wParam, altPressed, ctrlPressed, shiftPressed);

		if (macro != NULL)
			g_MacroPointer = (CMacroObject*)macro->m_Items;

		g_MacroManager.WaitingBandageTarget = false;
		g_MacroManager.WaitForTargetTimer = 0;
		g_MacroManager.Execute();
	}
}
//----------------------------------------------------------------------------------
void CGameScreen::OnKeyUp(const WPARAM &wParam, const LPARAM &lParam)
{
	if (wParam == VK_TAB && g_GameState == GS_GAME)
	{
		if (g_ConfigManager.HoldTabForCombat)
			g_Orion.ChangeWarmode(0);
	}
	else if (wParam == 0x2C) //Print Screen
		g_ScreenshotBuilder.SaveScreen();
}
//----------------------------------------------------------------------------------
