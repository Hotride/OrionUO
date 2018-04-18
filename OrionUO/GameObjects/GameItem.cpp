﻿// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
/***********************************************************************************
**
** GameItem.cpp
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#include "stdafx.h"
//----------------------------------------------------------------------------------
CGameItem::CGameItem(int serial)
: CGameObject(serial)
{
	NPC = false;
}
//----------------------------------------------------------------------------------
CGameItem::~CGameItem()
{
	WISPFUN_DEBUG("c19_f1");
	ClearMultiItems();
	
	if (Opened)
	{
		g_GumpManager.CloseGump(Serial, 0, GT_CONTAINER);
		g_GumpManager.CloseGump(Serial, 0, GT_SPELLBOOK);
		g_GumpManager.CloseGump(Serial, 0, GT_MAP);

		CGump *gump = g_GumpManager.GetGump(Serial, 0, GT_BULLETIN_BOARD);
		if (gump != NULL)
			g_GumpManager.RemoveGump(gump);

		Opened = false;
	}

	if (Dragged)
	{
		g_GumpManager.CloseGump(Serial, 0, GT_DRAG);

		Dragged = false;
	}
}
//----------------------------------------------------------------------------------
void CGameItem::ClearMultiItems()
{
	WISPFUN_DEBUG("c19_f2");
	if (MultiBody && m_Items != NULL)
	{
		CMulti *multi = (CMulti*)m_Items;
		m_Items = NULL;
		delete multi;
	}

	WantUpdateMulti = true;
}
//----------------------------------------------------------------------------------
/*!
Событие изменения картинки объекта
@param [__in_opt] direction Направление предмета (для трупов)
@return 
*/
void CGameItem::OnGraphicChange(int direction)
{
	WISPFUN_DEBUG("c19_f4");
	if (!MultiBody)
	{
		if (Graphic >= g_Orion.m_StaticData.size())
			return;

		if (IsCorpse())
		{
			AnimIndex = 99;

			if (direction & 0x80)
			{
				UsedLayer = 1;
				direction &= 0x7F;
			}
			else
				UsedLayer = 0;

			Layer = direction;

			NoDrawTile = false;
		}
		else
		{
			m_TiledataPtr = &g_Orion.m_StaticData[Graphic];
			STATIC_TILES &tile = *m_TiledataPtr;

			NoDrawTile = IsNoDrawTile(Graphic);

			if (IsWearable() || Graphic == 0x0A28)
			{
				AnimID = tile.AnimID;
				
				g_Orion.ExecuteGump(tile.AnimID + MALE_GUMP_OFFSET);
				g_Orion.ExecuteGump(tile.AnimID + FEMALE_GUMP_OFFSET);

				UsedLayer = tile.Layer;
			}
			else if (Layer == OL_MOUNT)
			{
				AnimID = tile.AnimID;
				UsedLayer = tile.Layer;
			}

			CalculateFieldColor();

			g_Orion.ExecuteStaticArt(Graphic);
		}
	}
	else if (m_Items == NULL || WantUpdateMulti)
	{
		if (!MultiDistanceBonus || CheckMultiDistance(g_RemoveRangeXY, this, g_ConfigManager.UpdateRange))
			LoadMulti(m_Items == NULL);
	}
}
//----------------------------------------------------------------------------------
void CGameItem::CalculateFieldColor()
{
	WISPFUN_DEBUG("c19_f5");
	FieldColor = 0;

	if (!g_ConfigManager.GetChangeFieldsGraphic())
		return;

	//fire field
	if (IN_RANGE(Graphic, 0x398C, 0x399F))
		FieldColor = 0x0020;
	//paralyze field
	else if (IN_RANGE(Graphic, 0x3967, 0x397A))
		FieldColor = 0x0058;
	//energy field
	else if (IN_RANGE(Graphic, 0x3946, 0x3964))
		FieldColor = 0x0070;
	//poison field
	else if (IN_RANGE(Graphic, 0x3914, 0x3929))
		FieldColor = 0x0044;
	//wall of stone
	else if (Graphic == 0x0080)
		FieldColor = 0x038A;
}
//----------------------------------------------------------------------------------
void CGameItem::Draw(int x, int y)
{
	WISPFUN_DEBUG("c19_f7");
	if (Container == 0xFFFFFFFF)
	{
		if (MultiBody)
		{
			RenderGraphic = MultiTileGraphic;

			if (RenderGraphic)
			{
				if (!Locked() && g_SelectedObject.Object == this)
					RenderColor = 0x0035;
				else
					RenderColor = Color;

				CRenderStaticObject::Draw(x, y);
			}

			return;
		}

#if UO_DEBUG_INFO!=0
		g_RenderedObjectsCountInGameWindow++;
#endif

		bool useAlpha = (m_DrawTextureColor[3] != 0xFF);

		if (useAlpha)
		{
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			glColor4ub(m_DrawTextureColor[0], m_DrawTextureColor[1], m_DrawTextureColor[2], m_DrawTextureColor[3]);
		}

		if (IsCorpse()) //Трупик
			g_AnimationManager.DrawCorpse(this, x, y - 3);
		else
		{
			bool doubleDraw = false;
			bool selMode = false;
			ushort objGraphic = GetDrawGraphic(doubleDraw);
			ushort objColor = Color & 0x3FFF;

			if (Hidden())
			{
				selMode = true;
				objColor = 0x038E;
			}

			if (!Locked() && g_SelectedObject.Object == this)
			{
				objColor = 0x0035;
				selMode = true;
			}

			if (Container == 0xFFFFFFFF)
				selMode = false;

			if (doubleDraw)
			{
				g_Orion.DrawStaticArt(objGraphic, objColor, x - 2, y - 5, selMode || Hidden());
				g_Orion.DrawStaticArt(objGraphic, objColor, x + 3, y, selMode || Hidden());
			}
			else
			{
				if (FieldColor)
					g_Orion.DrawStaticArt(FIELD_REPLACE_GRAPHIC, FieldColor, x, y, selMode || Hidden());
				else
					g_Orion.DrawStaticArtAnimated(objGraphic, objColor, x, y, selMode || Hidden());
			}

			if (IsLightSource() && g_GameScreen.UseLight)
				g_GameScreen.AddLight(this, this, x, y);
		}

		if (useAlpha)
		{
			glColor4ub(0xFF, 0xFF, 0xFF, 0xFF);
			glDisable(GL_BLEND);
		}

		if (!g_ConfigManager.DisableNewTargetSystem && g_NewTargetSystem.Serial == Serial && !Locked())
		{
			WISP_GEOMETRY::CSize size = g_Orion.GetStaticArtDimension(Graphic);

			if (size.Width >= 80)
			{
				g_NewTargetSystem.GumpTop = 0x756D;
				g_NewTargetSystem.GumpBottom = 0x756A;
			}
			else if (size.Width >= 40)
			{
				g_NewTargetSystem.GumpTop = 0x756E;
				g_NewTargetSystem.GumpBottom = 0x756B;
			}
			else
			{
				g_NewTargetSystem.GumpTop = 0x756F;
				g_NewTargetSystem.GumpBottom = 0x756C;
			}

			g_NewTargetSystem.ColorGump = 0x7570;

			g_NewTargetSystem.Hits = 0;
			g_NewTargetSystem.X = x;
			g_NewTargetSystem.TopY = y - size.Height - 8;
			g_NewTargetSystem.BottomY = y + 7;
		}

		DrawEffects(x, y);
	}
}
//----------------------------------------------------------------------------------
/*!
Отрисовать предмет
@param [__in] mode Режим рисования. true - рисование, false - выбор объектов
@param [__in] drawX Экранная координата X объекта
@param [__in] drawY Экранная координата Y объекта
@param [__in] ticks Таймер рендера
@return При выборе объектов возвращает выбранный элемент
*/
void CGameItem::Select(int x, int y)
{
	WISPFUN_DEBUG("c19_f8");
	if (Container == 0xFFFFFFFF)
	{
		if (MultiBody)
		{
			RenderGraphic = MultiTileGraphic;

			if (RenderGraphic)
				CRenderStaticObject::Select(x, y);

			return;
		}

		if (IsCorpse()) //Трупик
		{
			if (g_AnimationManager.CorpsePixelsInXY(this, x, y - 3))
				g_SelectedObject.Init(this);
		}
		else
		{
			bool doubleDraw = false;
			ushort objGraphic = GetDrawGraphic(doubleDraw);

			if (doubleDraw)
			{
				if (g_Orion.StaticPixelsInXY(objGraphic, x - 2, y - 5))
					g_SelectedObject.Init(this);
				else if (g_Orion.StaticPixelsInXY(objGraphic, x + 3, y))
					g_SelectedObject.Init(this);
			}
			else if (FieldColor)
			{
				if (g_Orion.StaticPixelsInXY(FIELD_REPLACE_GRAPHIC, x, y))
					g_SelectedObject.Init(this);
			}
			else if (g_Orion.StaticPixelsInXYAnimated(objGraphic, x, y))
				g_SelectedObject.Init(this);
		}
	}
}
//----------------------------------------------------------------------------------
/*!
Получить индекс картинки (для анимации)
@return Индекс картинки
*/
ushort CGameItem::GetMountAnimation()
{
	WISPFUN_DEBUG("c19_f9");
	ushort graphic = Graphic;

	if (Layer == OL_MOUNT)
	{
		switch (graphic)
		{
			case 0x3E90: // 16016 Reptalon
			{
				graphic = 0x0114;
				break;
			}
			case 0x3E91: // 16017
			{
				graphic = 0x0115;
				break;
			}
			case 0x3E92: // 16018
			{
				graphic = 0x011C;
				break;
			}
			case 0x3E94: // 16020
			{
				graphic = 0x00F3;
				break;
			}
			case 0x3E95: // 16021
			{
				graphic = 0x00A9;
				break;
			}
			case 0x3E97: // 16023 Ethereal Giant Beetle
			{
				graphic = 0x00C3;
				break;
			}
			case 0x3E98: // 16024 Ethereal Swamp Dragon
			{
				graphic = 0x00C2;
				break;
			}
			case 0x3E9A: // 16026 Ethereal Ridgeback
			{
				graphic = 0x00C1;
				break;
			}
			case 0x3E9B: // 16027
			case 0x3E9D: // 16029 Ethereal Unicorn
			{
				graphic = 0x00C0;
				break;
			}
			case 0x3E9C: // 16028 Ethereal Kirin
			{
				graphic = 0x00BF;
				break;
			}
			case 0x3E9E: // 16030
			{
				graphic = 0x00BE;
				break;
			}
			case 0x3EA0: // 16032 light grey/horse3
			{
				graphic = 0x00E2;
				break;
			}
			case 0x3EA1: // 16033 greybrown/horse4
			{
				graphic = 0x00E4;
				break;
			}
			case 0x3EA2: // 16034 dark brown/horse
			{
				graphic = 0x00CC;
				break;
			}
			case 0x3EA3: // 16035 desert ostard
			{
				graphic = 0x00D2;
				break;
			}
			case 0x3EA4: // 16036 frenzied ostard (=zostrich)
			{
				graphic = 0x00DA;
				break;
			}
			case 0x3EA5: // 16037 forest ostard
			{
				graphic = 0x00DB;
				break;
			}
			case 0x3EA6: // 16038 Llama
			{
				graphic = 0x00DC;
				break;
			}
			case 0x3EA7: // 16039 Nightmare / Vortex
			{
				graphic = 0x0074;
				break;
			}
			case 0x3EA8: // 16040 Silver Steed
			{
				graphic = 0x0075;
				break;
			}
			case 0x3EA9: // 16041 Nightmare
			{
				graphic = 0x0072;
				break;
			}
			case 0x3EAA: // 16042 Ethereal Horse
			{
				graphic = 0x0073;
				break;
			}
			case 0x3EAB: // 16043 Ethereal Llama
			{
				graphic = 0x00AA;
				break;
			}
			case 0x3EAC: // 16044 Ethereal Ostard
			{
				graphic = 0x00AB;
				break;
			}
			case 0x3EAD: // 16045 Kirin
			{
				graphic = 0x0084;
				break;
			}
			case 0x3EAF: // 16047 War Horse (Blood Red)
			{
				graphic = 0x0078;
				break;
			}
			case 0x3EB0: // 16048 War Horse (Light Green)
			{
				graphic = 0x0079;
				break;
			}
			case 0x3EB1: // 16049 War Horse (Light Blue)
			{
				graphic = 0x0077;
				break;
			}
			case 0x3EB2: // 16050 War Horse (Purple)
			{
				graphic = 0x0076;
				break;
			}
			case 0x3EB3: // 16051 Sea Horse (Medium Blue)
			{
				graphic = 0x0090;
				break;
			}
			case 0x3EB4: // 16052 Unicorn
			{
				graphic = 0x007A;
				break;
			}
			case 0x3EB5: // 16053 Nightmare
			{
				graphic = 0x00B1;
				break;
			}
			case 0x3EB6: // 16054 Nightmare 4
			{
				graphic = 0x00B2;
				break;
			}
			case 0x3EB7: // 16055 Dark Steed
			{
				graphic = 0x00B3;
				break;
			}
			case 0x3EB8: // 16056 Ridgeback
			{
				graphic = 0x00BC;
				break;
			}
			case 0x3EBA: // 16058 Ridgeback, Savage
			{
				graphic = 0x00BB;
				break;
			}
			case 0x3EBB: // 16059 Skeletal Mount
			{
				graphic = 0x0319;
				break;
			}
			case 0x3EBC: // 16060 Beetle
			{
				graphic = 0x0317;
				break;
			}
			case 0x3EBD: // 16061 SwampDragon
			{
				graphic = 0x031A;
				break;
			}
			case 0x3EBE: // 16062 Armored Swamp Dragon
			{
				graphic = 0x031F;
				break;
			}
			case 0x3EC3: //16067 Beetle
			{
				graphic = 0x02D4;
				break;
			}
			case 0x3EC5: // 16069
			case 0x3F3A: // 16186 snow bear ???
			{
				graphic = 0x00D5;
				break;
			}
			case 0x3EC6: // 16070 Boura
			{
				graphic = 0x01B0;
				break;
			}
			case 0x3EC7: // 16071 Tiger
			{
				graphic = 0x04E6;
				break;
			}
			case 0x3EC8: // 16072 Tiger
			{
				graphic = 0x04E7;
				break;
			}
			case 0x3EC9: // 16073
			{
				graphic = 0x042D;
				break;
			}
			default: //lightbrown/horse2
			{
				graphic = 0x00C8;

				break;
			}
		}

		if (m_TiledataPtr->AnimID != 0)
			graphic = m_TiledataPtr->AnimID;
	}
	else if (IsCorpse())
		graphic = (ushort)Count;
		
	return graphic;
}
//----------------------------------------------------------------------------------
void CGameItem::ClearCustomHouseMultis(int state)
{
	CMulti *nextMulti = NULL;

	int checkZ = m_Z + 7;

	for (CMulti *multi = (CMulti*)m_Items; multi != NULL; multi = nextMulti)
	{
		nextMulti = (CMulti*)multi->m_Next;

		CMultiObject *nextItem = NULL;

		for (CMultiObject *item = (CMultiObject*)multi->m_Items; item != NULL; item = nextItem)
		{
			nextItem = (CMultiObject*)item->m_Next;

			item->State = item->State & ~(CHMOF_TRANSPARENT | CHMOF_IGNORE_IN_RENDER | CHMOF_VALIDATED_PLACE | CHMOF_INCORRECT_PLACE);

			if (item->IsCustomHouseMulti())
			{
				if (!state || item->State & state)
					multi->Delete(item);
			}
			else if (item->GetZ() == checkZ)
				item->State = item->State | CHMOF_FLOOR | CHMOF_IGNORE_IN_RENDER;
		}

		if (multi->m_Items == NULL)
			Delete(multi);
	}
}
//----------------------------------------------------------------------------------
/*!
Добавить мульти в текущий объект
@return
*/
CMultiObject *CGameItem::AddMulti(ushort graphic, ushort color, char x, char y, char z, bool isCustomHouseMulti)
{
	CMultiObject *mo = NULL;

	if (isCustomHouseMulti)
		mo = new CCustomHouseMultiObject(graphic, color, GetX() + x, GetY() + y, z, 1);
	else
		mo = new CMultiObject(graphic, GetX() + x, GetY() + y, z, 1);

	g_MapManager.AddRender(mo);
	AddMultiObject(mo);

	return mo;
}
//----------------------------------------------------------------------------------
/*!
Загрузка мульти в текущий объект
@return 
*/
void CGameItem::LoadMulti(bool dropAlpha)
{
	WISPFUN_DEBUG("c19_f10");
	ClearMultiItems();

	if (Graphic >= MAX_MULTI_DATA_INDEX_COUNT)
		return;

	WantUpdateMulti = false;

	CIndexMulti &index = g_Orion.m_MultiDataIndex[Graphic];

	size_t address = index.Address;
	int count = index.Count;

	int minX = 0;
	int minY = 0;
	int maxX = 0;
	int maxY = 0;

	uchar alpha = 0;

	if (!dropAlpha)
		alpha = 0xFF;

	if (index.UopBlock != NULL)
	{
		UCHAR_LIST data = g_FileManager.m_MultiCollection.GetData(*index.UopBlock);

		if (data.empty())
			return;

		WISP_DATASTREAM::CDataReader reader(&data[0], data.size());
		reader.Move(8); //ID + Count

		IFOR(i, 0, count)
		{
			ushort graphic = reader.ReadUInt16LE();
			short x = reader.ReadInt16LE();
			short y = reader.ReadInt16LE();
			short z = reader.ReadInt16LE();
			ushort flags = reader.ReadUInt16LE();
			uint clilocsCount = reader.ReadUInt32LE();

			if (clilocsCount)
				reader.Move(clilocsCount * 4);

			if (!flags)
			{
				CMultiObject *mo = new CMultiObject(graphic, m_X + x, m_Y + y, m_Z + (char)z, 1);

				mo->m_DrawTextureColor[3] = alpha;

				g_MapManager.AddRender(mo);
				AddMultiObject(mo);
			}
			else if (!i)
				MultiTileGraphic = graphic;

			if (x < minX)
				minX = x;
			if (x > maxX)
				maxX = x;

			if (y < minY)
				minY = y;
			if (y > maxY)
				maxY = y;
		}
	}
	else if (address != 0)
	{
		int itemOffset = sizeof(MULTI_BLOCK);

		if (g_PacketManager.GetClientVersion() >= CV_7090)
			itemOffset = sizeof(MULTI_BLOCK_NEW);

		IFOR(j, 0, count)
		{
			PMULTI_BLOCK pmb = (PMULTI_BLOCK)(address + (j * itemOffset));

			if (pmb->Flags)
			{
				CMultiObject *mo = new CMultiObject(pmb->ID, m_X + pmb->X, m_Y + pmb->Y, m_Z + (char)pmb->Z, pmb->Flags);

				mo->m_DrawTextureColor[3] = alpha;

				g_MapManager.AddRender(mo);
				AddMultiObject(mo);
			}
			else if (!j)
				MultiTileGraphic = pmb->ID;

			if (pmb->X < minX)
				minX = pmb->X;
			if (pmb->X > maxX)
				maxX = pmb->X;
			
			if (pmb->Y < minY)
				minY = pmb->Y;
			if (pmb->Y > maxY)
				maxY = pmb->Y;
		}
	}

	CMulti *multi = (CMulti*)m_Items;

	if (multi != NULL)
	{
		multi->MinX = minX;
		multi->MinY = minY;

		multi->MaxX = maxX;
		multi->MaxY = maxY;

		MultiDistanceBonus = max(max(abs(minX), maxX), max(abs(minY), maxY));
	}

	CGumpMinimap *minimap = (CGumpMinimap*)g_GumpManager.GetGump(0, 0, GT_MINIMAP);

	if (minimap != NULL)
		minimap->LastX = 0;
}
//----------------------------------------------------------------------------------
/*!
Добавить объекта-мульти
@param [__in] obj Ссылка на мульти-объект
@return 
*/
void CGameItem::AddMultiObject(CMultiObject *obj)
{
	WISPFUN_DEBUG("c19_f11");
	if (m_Items == NULL)
	{
		m_Items = new CMulti(obj->GetX(), obj->GetY());
		m_Items->m_Next = NULL;
		m_Items->m_Items = obj;
		obj->m_Next = NULL;
		obj->m_Prev = NULL;
	}
	else
	{
		CMulti *multi = GetMultiAtXY(obj->GetX(), obj->GetY());

		if (multi != NULL)
		{
			QFOR(multiobj, multi->m_Items, CMultiObject*)
			{
				if (obj->GetZ() < multiobj->GetZ())
				{
					if (multiobj->m_Prev == NULL)
						multi->Insert(multiobj->m_Prev, obj);
					else
						multi->Insert(multiobj, obj);

					return;
				}

				if (multiobj->m_Next == NULL)
				{
					multiobj->m_Next = obj;
					obj->m_Prev = multiobj;
					obj->m_Next = NULL;

					return;
				}
			}

			//Если пришли сюда - что-то пошло не так
		}
		else
		{
			CMulti *newmulti = new CMulti(obj->GetX(), obj->GetY());
			newmulti->m_Next = NULL;
			newmulti->m_Items = obj;
			obj->m_Next = NULL;
			obj->m_Prev = NULL;

			QFOR(multi, m_Items, CMulti*)
			{
				if (multi->m_Next == NULL)
				{
					multi->m_Next = newmulti;
					newmulti->m_Prev = multi;
					break;
				}
			}
		}
	}
}
//----------------------------------------------------------------------------------
/*!
Получение объекта мульти в заданных координатах
@param [__in] x Координата X
@param [__in] y Координата Y
@return Ссылка на мульти или NULL
*/
CMulti *CGameItem::GetMultiAtXY(short x, short y)
{
	WISPFUN_DEBUG("c19_f12");
	QFOR(multi, m_Items, CMulti*)
	{
		if (multi->X == x && multi->Y == y)
			return multi;
	}

	return NULL;
}
//----------------------------------------------------------------------------------
/*!
Найти объект внутри (рекурсивно) по типу с учетом (и без) цвета
@param [__in] graphic Индекс картинки
@param [__in_opt] color Цвет предмета
@return Ссылка на найденный объект или NULL
*/
CGameItem *CGameItem::FindItem(ushort graphic, ushort color)
{
	WISPFUN_DEBUG("c19_f13");
	CGameItem *item = NULL;

	if (color == 0xFFFF) //Поиск по минимальному цвету
	{
		WORD minColor = 0xFFFF;

		QFOR(obj, m_Items, CGameItem*)
		{
			if (obj->Graphic == graphic)
			{
				if (obj->Color < minColor)
				{
					item = obj;
					minColor = obj->Color;
				}
			}

			if (!obj->Empty())
			{
				CGameItem *found = obj->FindItem(graphic, color);
				
				if (found != NULL && found->Color < minColor)
				{
					item = found;
					minColor = found->Color;
				}
			}
		}
	}
	else //стандартный поиск
	{
		QFOR(obj, m_Items, CGameItem*)
		{
			if (obj->Graphic == graphic && obj->Color == color)
				item = obj;

			if (!obj->Empty())
			{
				CGameItem *found = obj->FindItem(graphic, color);
				
				if (found != NULL)
					item = found;
			}
		}
	}

	return item;
}
//----------------------------------------------------------------------------------
CMulti* CGameItem::GetMulti()
{
	CMulti *multi = (CMulti*)m_Items;
	return multi;
}
//----------------------------------------------------------------------------------
