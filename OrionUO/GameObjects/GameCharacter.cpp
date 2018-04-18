﻿// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
/***********************************************************************************
**
** GameCharacter.cpp
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#include "stdafx.h"
//----------------------------------------------------------------------------------
CGameCharacter::CGameCharacter(int serial)
: CGameObject(serial), Hits(0), MaxHits(0), LastStepSoundTime(GetTickCount()),
TimeToRandomFidget(GetTickCount() + RANDOM_FIDGET_ANIMATION_DELAY)
{
	NPC = true;
	NoDrawTile = false;
	WISPFUN_DEBUG("c15_f1");

	bool wantStatusRequest = (g_GumpManager.UpdateContent(serial, 0, GT_STATUSBAR) != NULL) || (g_GumpManager.UpdateContent(serial, 0, GT_TARGET_SYSTEM) != NULL) || g_ConfigManager.GetDrawStatusState() || (serial == g_LastTargetObject) || (serial == g_LastAttackObject);

	if (!g_ConfigManager.DisableNewTargetSystem && g_NewTargetSystem.Serial == serial && g_GumpManager.UpdateContent(serial, 0, GT_TARGET_SYSTEM) == NULL)
	{
		wantStatusRequest = true;
		g_GumpManager.AddGump(new CGumpTargetSystem(Serial, g_NewTargetSystem.GumpX, g_NewTargetSystem.GumpY));
	}

	if (wantStatusRequest)
		CPacketStatusRequest(Serial).Send();
}
//----------------------------------------------------------------------------------
CGameCharacter::~CGameCharacter()
{
	WISPFUN_DEBUG("c15_f2");
	//!Чистим память
	m_Steps.clear();

	m_HitsTexture.Clear();

	uint serial = Serial & 0x3FFFFFFF;

	if (g_ConfigManager.RemoveStatusbarsWithoutObjects)
		g_GumpManager.CloseGump(serial, 0, GT_STATUSBAR);
	else
	{
		//!Если стянут статусбар - обновим его
		g_GumpManager.UpdateContent(serial, 0, GT_STATUSBAR);
	}
	
	//!Если стянут статусбар таргет системы - обновим его
	g_GumpManager.UpdateContent(serial, 0, GT_TARGET_SYSTEM);

	if (!IsPlayer())
		g_GumpManager.CloseGump(serial, 0, GT_PAPERDOLL);
	//Чистим если находился в пати.
	if (g_Party.Contains(serial))
	{
		IFOR(i, 0, 10)
		{
			CPartyObject &member = g_Party.Member[i];

			if (member.Serial != serial)
				continue;

			member.Serial = 0;
			member.Character = NULL;
		}
	}
}
//----------------------------------------------------------------------------------
void CGameCharacter::UpdateTextCoordinates()
{
	CTextData *text = (CTextData*)m_TextControl->Last();

	if (text == NULL)
		return;

	ANIMATION_DIMENSIONS dims = g_AnimationManager.GetAnimationDimensions(this, 0);
	int offset = 0;

	int x = DrawX + OffsetX;
	int y = DrawY + (OffsetY - OffsetZ) - ((dims.Height + dims.CenterY) + 8);

	if (g_ConfigManager.GetDrawStatusState() == DCSS_ABOVE)
		y -= 14;

	for (; text != NULL; text = (CTextData*)text->m_Prev)
	{
		offset += text->m_Texture.Height;

		text->RealDrawX = x - (text->m_Texture.Width / 2);
		text->RealDrawY = y - offset;
	}
}
//----------------------------------------------------------------------------------
void CGameCharacter::UpdateHitsTexture(uchar hits)
{
	WISPFUN_DEBUG("c15_f3");
	if (HitsPercent != hits || m_HitsTexture.Empty())
	{
		HitsPercent = hits;

		char hitsText[10] = { 0 };
		sprintf_s(hitsText, "[%i%%]", hits);

		ushort color = 0x0044;

		if (hits < 30)
			color = 0x0021;
		else if (hits < 50)
			color = 0x0030;
		else if (hits < 80)
			color = 0x0058;

		g_FontManager.GenerateA(3, m_HitsTexture, hitsText, color);
	}
}
//----------------------------------------------------------------------------------
/*!
Сидит ли персонаж
@return Индекс объекта из таблицы, на котором он восседает
*/
int CGameCharacter::IsSitting()
{
	WISPFUN_DEBUG("c15_f4");
	int result = 0;
	ushort testGraphic = Graphic;
	bool human = (((testGraphic >= 0x0190) && (testGraphic <= 0x0193)) || (testGraphic == 0x03DB) || (testGraphic == 0x03DF) || (testGraphic == 0x03E2));

	if (human && FindLayer(OL_MOUNT) == NULL && !TestStepNoChangeDirection(GetAnimationGroup()))
	{
		CRenderWorldObject *obj = this;

		while (obj != NULL && obj->m_PrevXY != NULL)
			obj = obj->m_PrevXY;

		while (obj != NULL && !result)
		{
			if (obj->IsStaticGroupObject() && abs(m_Z - obj->GetZ()) <= 1) //m_Z == obj->GetZ()
			{
				ushort graphic = obj->Graphic;

				if (obj->IsGameObject())
				{
					if (((CGameObject*)obj)->NPC || ((CGameItem*)obj)->MultiBody)
						graphic = 0;
				}

				switch (graphic)
				{
					case 0x0459:
					case 0x045A:
					case 0x045B:
					case 0x045C:
					case 0x0A2A:
					case 0x0A2B:
					case 0x0B2C:
					case 0x0B2D:
					case 0x0B2E:
					case 0x0B2F:				
					case 0x0B30:
					case 0x0B31:
					case 0x0B32:
					case 0x0B33:
					case 0x0B4E:
					case 0x0B4F:
					case 0x0B50:
					case 0x0B51:
					case 0x0B52:
					case 0x0B53:
					case 0x0B54:
					case 0x0B55:
					case 0x0B56:
					case 0x0B57:
					case 0x0B58:
					case 0x0B59:
					case 0x0B5A:
					case 0x0B5B:
					case 0x0B5C:
					case 0x0B5D:
					case 0x0B5E:
					case 0x0B5F:
					case 0x0B60:
					case 0x0B61:
					case 0x0B62:
					case 0x0B63:
					case 0x0B64:
					case 0x0B65:
					case 0x0B66:
					case 0x0B67:
					case 0x0B68:
					case 0x0B69:
					case 0x0B6A:
					case 0x0B91:
					case 0x0B92:
					case 0x0B93:
					case 0x0B94:
					case 0x0CF3:
					case 0x0CF4:
					case 0x0CF6:
					case 0x0CF7:
					case 0x11FC:
					case 0x1218:
					case 0x1219:
					case 0x121A:
					case 0x121B:
					case 0x1527:
					case 0x1771:
					case 0x1776:
					case 0x1779:
					case 0x1DC7:
					case 0x1DC8:
					case 0x1DC9:
					case 0x1DCA:
					case 0x1DCB:
					case 0x1DCC:
					case 0x1DCD:
					case 0x1DCE:
					case 0x1DCF:
					case 0x1DD0:
					case 0x1DD1:
					case 0x1DD2:
					case 0x2A58:
					case 0x2A59:
					case 0x2A5A:
					case 0x2A5B:
					case 0x2A7F:
					case 0x2A80:
					case 0x2DDF:
					case 0x2DE0:
					case 0x2DE3:
					case 0x2DE4:
					case 0x2DE5:
					case 0x2DE6:
					case 0x2DEB:
					case 0x2DEC:
					case 0x2DED:
					case 0x2DEE:
					case 0x2DF5:
					case 0x2DF6:
					case 0x3088:
					case 0x3089:
					case 0x308A:
					case 0x308B:
					case 0x35ED:
					case 0x35EE:
					case 0x3DFF:
					case 0x3E00:
					{
						IFOR(i, 0, SITTING_ITEMS_COUNT)
						{
							if (SITTING_INFO[i].Graphic == graphic)
							{
								result = (int)i + 1;
								break;
							}
						}
						break;
					}
					default:
						break;
				}
			}

			obj = obj->m_NextXY;
		}
	}

	return result;
}
//----------------------------------------------------------------------------------
/*!
Отрисовать персонажа
@param [__in] mode Режим рисования. true - рисование, false - выбор объектов
@param [__in] drawX Экранная координата X объекта
@param [__in] drawY Экранная координата Y объекта
@param [__in] ticks Таймер рендера
@return При выборе объектов возвращает выбранный элемент
*/
void CGameCharacter::Draw(int x, int y)
{
	WISPFUN_DEBUG("c15_f5");
	if (TimeToRandomFidget < g_Ticks)
		SetRandomFidgetAnimation();

#if UO_DEBUG_INFO!=0
	g_RenderedObjectsCountInGameWindow++;
#endif

	uint lastSBsel = g_StatusbarUnderMouse;

	if (!IsPlayer() && g_Player->Warmode && g_SelectedObject.Object == this)
		g_StatusbarUnderMouse = Serial;

	g_AnimationManager.DrawCharacter(this, x, y); //Draw character

	g_StatusbarUnderMouse = lastSBsel;

	DrawEffects(x, y);
}
//----------------------------------------------------------------------------------
void CGameCharacter::Select(int x, int y)
{
	WISPFUN_DEBUG("c15_f6");
	if (g_AnimationManager.CharacterPixelsInXY(this, x, y))
		g_SelectedObject.Init(this);
}
//----------------------------------------------------------------------------------
/*!
Обновить информацию о поле персонажа, обновление гампов
@param [__in_opt] direction Направление персонажа
@return 
*/
void CGameCharacter::OnGraphicChange(int direction)
{
	WISPFUN_DEBUG("c15_f7");
	//!Обновления пола и расы в зависимости от индекса картинки персонажа
	switch (Graphic)
	{
		case 0x0190:
		case 0x0192:
		{
			Female = false;
			Race = RT_HUMAN;
			break;
		}
		case 0x0191:
		case 0x0193:
		{
			Female = true;
			Race = RT_HUMAN;
			break;
		}
		case 0x025D:
		{
			Female = false;
			Race = RT_ELF;
			break;
		}
		case 0x025E:
		{
			Female = true;
			Race = RT_ELF;
			break;
		}
		case 0x029A:
		{
			Female = false;
			Race = RT_GARGOYLE;
		    break;
		}
		case 0x029B:
		{
			Female = true;
			Race = RT_GARGOYLE;
			break;
		}
		default:
			break;
	}

	if (direction == 1000)
	{
		g_GumpManager.UpdateContent(Serial, 0, GT_PAPERDOLL);

		if ((g_GumpManager.UpdateContent(Serial, 0, GT_STATUSBAR) != NULL || g_GumpManager.UpdateContent(Serial, 0, GT_TARGET_SYSTEM) != NULL) && MaxHits == 0)
			CPacketStatusRequest(Serial).Send();
	}
}
//----------------------------------------------------------------------------------
/*!
Установка анимации от сервера
@param [__in] id Группа анимаци
@param [__in_opt] interval Задержка между кадрами
@param [__in_opt] frameCount Количество кадлов анимации
@param [__in_opt] repeatCount Количество повторов анимации
@param [__in_opt] repeat Зациклено или нет
@param [__out_opt] frameDirection Направление прокрутки кадров (вперед/назад)
@return 
*/
void CGameCharacter::SetAnimation(uchar id, uchar interval, uchar frameCount, uchar repeatCount, bool repeat, bool frameDirection)
{
	WISPFUN_DEBUG("c15_f10");
	AnimationGroup = id;
	AnimIndex = 0;
	AnimationInterval = interval;
	AnimationFrameCount = frameCount;
	AnimationRepeatMode = repeatCount;
	AnimationRepeat = repeat;
	AnimationDirection = frameDirection;
	AnimationFromServer = false;

	LastAnimationChangeTime = g_Ticks;
	TimeToRandomFidget = g_Ticks + RANDOM_FIDGET_ANIMATION_DELAY;
}
//----------------------------------------------------------------------------------
/*!
Установка группы анимации
@param [__in] val Новое значение группы анимации
@return
*/
void CGameCharacter::ResetAnimationGroup(uchar val)
{
	WISPFUN_DEBUG("c15_f11");
	AnimationFrameCount = 0;
	AnimationInterval = 0;
	AnimationRepeat = false;
	AnimationRepeatMode = 0;
	AnimationDirection = false;
	AnimationFromServer = false;

	AnimationGroup = val;
}
//----------------------------------------------------------------------------------
/*!
Установка случайной анимации (при длительном простое)
@return 
*/
void CGameCharacter::SetRandomFidgetAnimation()
{
	WISPFUN_DEBUG("c15_f12");
	TimeToRandomFidget = g_Ticks + RANDOM_FIDGET_ANIMATION_DELAY;

	if (FindLayer(OL_MOUNT) == NULL)
	{
		AnimIndex = 0;
		AnimationFrameCount = 0;
		AnimationInterval = 1;
		AnimationRepeatMode = 1;
		AnimationDirection = true;
		AnimationRepeat = false;
		AnimationFromServer = true;

		ANIMATION_GROUPS groupIndex = g_AnimationManager.GetGroupIndex(GetMountAnimation());

		const BYTE fidgetAnimTable[3][3] =
		{
			{ LAG_FIDGET_1, LAG_FIDGET_2, LAG_FIDGET_1 },
			{ HAG_FIDGET_1, HAG_FIDGET_2, HAG_FIDGET_1 },
			{ PAG_FIDGET_1, PAG_FIDGET_2, PAG_FIDGET_3 }
		};

		AnimationGroup = fidgetAnimTable[groupIndex - 1][RandomInt(3)];
	}
}
//----------------------------------------------------------------------------------
/*!
Скорректировать отношение анимаций
@param [__in] group Группа анимации
@param [__inout] animation Индекс группы анимации
@return
*/
void CGameCharacter::GetAnimationGroup(ANIMATION_GROUPS group, uchar &animation)
{
	WISPFUN_DEBUG("c15_f13");
	const uchar animAssociateTable[PAG_ANIMATION_COUNT][3] =
	{
		{ LAG_WALK,			HAG_WALK,			PAG_WALK_UNARMED },
		{ LAG_WALK,			HAG_WALK,			PAG_WALK_ARMED },
		{ LAG_RUN,			HAG_FLY,			PAG_RUN_UNARMED },
		{ LAG_RUN,			HAG_FLY,			PAG_RUN_ARMED },
		{ LAG_STAND,		HAG_STAND,			PAG_STAND },
		{ LAG_FIDGET_1,		HAG_FIDGET_1,		PAG_FIDGET_1 },
		{ LAG_FIDGET_2,		HAG_FIDGET_2,		PAG_FIDGET_2 },
		{ LAG_STAND,		HAG_STAND,			PAG_STAND_ONEHANDED_ATTACK },
		{ LAG_STAND,		HAG_STAND,			PAG_STAND_TWOHANDED_ATTACK },
		{ LAG_EAT,			HAG_ATTACK_3,		PAG_ATTACK_ONEHANDED },
		{ LAG_EAT,			HAG_ATTACK_1,		PAG_ATTACK_UNARMED_1 },
		{ LAG_EAT,			HAG_ATTACK_2,		PAG_ATTACK_UNARMED_2 },
		{ LAG_EAT,			HAG_ATTACK_3,		PAG_ATTACK_TWOHANDED_DOWN },
		{ LAG_EAT,			HAG_ATTACK_1,		PAG_ATTACK_TWOHANDED_WIDE },
		{ LAG_EAT,			HAG_ATTACK_2,		PAG_ATTACK_TWOHANDED_JAB },
		{ LAG_WALK,			HAG_WALK,			PAG_WALK_WARMODE },
		{ LAG_EAT,			HAG_ATTACK_2,		PAG_CAST_DIRECTED },
		{ LAG_EAT,			HAG_ATTACK_3,		PAG_CAST_AREA },
		{ LAG_EAT,			HAG_ATTACK_1,		PAG_ATTACK_BOW },
		{ LAG_EAT,			HAG_ATTACK_2,		PAG_ATTACK_CROSSBOW },
		{ LAG_EAT,			HAG_GET_HIT_1,		PAG_GET_HIT },
		{ LAG_DIE_1,		HAG_DIE_1,			PAG_DIE_1 },
		{ LAG_DIE_2,		HAG_DIE_2,			PAG_DIE_2 },
		{ LAG_WALK,			HAG_WALK,			PAG_ONMOUNT_RIDE_SLOW },
		{ LAG_RUN,			HAG_FLY,			PAG_ONMOUNT_RIDE_FAST },
		{ LAG_STAND,		HAG_STAND,			PAG_ONMOUNT_STAND },
		{ LAG_EAT,			HAG_ATTACK_1,		PAG_ONMOUNT_ATTACK },
		{ LAG_EAT,			HAG_ATTACK_2,		PAG_ONMOUNT_ATTACK_BOW },
		{ LAG_EAT,			HAG_ATTACK_1,		PAG_ONMOUNT_ATTACK_CROSSBOW },
		{ LAG_EAT,			HAG_ATTACK_2,		PAG_ONMOUNT_SLAP_HORSE },
		{ LAG_EAT,			HAG_STAND,			PAG_TURN },
		{ LAG_WALK,			HAG_WALK,			PAG_ATTACK_UNARMED_AND_WALK },
		{ LAG_EAT,			HAG_STAND,			PAG_EMOTE_BOW },
		{ LAG_EAT,			HAG_STAND,			PAG_EMOTE_SALUTE },
		{ LAG_FIDGET_1,		HAG_FIDGET_1,		PAG_FIDGET_3 }
	};

	if (group && animation < PAG_ANIMATION_COUNT)
		animation = animAssociateTable[animation][group - 1];
}
//----------------------------------------------------------------------------------
/*!
Скорректировать отношение индексов групп анимаций
@param [__in] graphic Индекс картинки
@param [__in] group Группа анимаций
@param [__inout] animation Индекс анимации в группе
@return 
*/
void CGameCharacter::CorrectAnimationGroup(ushort graphic, ANIMATION_GROUPS group, uchar &animation)
{
	WISPFUN_DEBUG("c15_f14");
	if (group == AG_LOW)
	{
		switch (animation)
		{
			case LAG_DIE_2:
				animation = LAG_DIE_1;
				break;
			case LAG_FIDGET_2:
				animation = LAG_FIDGET_1;
				break;
			case LAG_ATTACK_3:
			case LAG_ATTACK_2:
				animation = LAG_ATTACK_1;
				break;
			default:
				break;
		}

		if (!g_AnimationManager.AnimationExists(graphic, animation))
			animation = LAG_STAND;
	}
	else if (group == AG_HIGHT)
	{
		switch (animation)
		{
			case HAG_DIE_2:
				animation = HAG_DIE_1;
				break;
			case HAG_FIDGET_2:
				animation = HAG_FIDGET_1;
				break;
			case HAG_ATTACK_3:
			case HAG_ATTACK_2:
				animation = HAG_ATTACK_1;
				break;
			case HAG_GET_HIT_3:
			case HAG_GET_HIT_2:
				animation = HAG_GET_HIT_1;
				break;
			case HAG_MISC_4:
			case HAG_MISC_3:
			case HAG_MISC_2:
				animation = HAG_MISC_1;
				break;
			case HAG_FLY:
				animation = HAG_WALK;
				break;
			default:
				break;
		}

		if (!g_AnimationManager.AnimationExists(graphic, animation))
			animation = HAG_STAND;
	}
}
//----------------------------------------------------------------------------------
/*!
Проверка на возможность изменения направления персонажа при движении в сидячем положении
@param [__in] group Индекс группы анимации
@return Можно изменять направление или нет
*/
bool CGameCharacter::TestStepNoChangeDirection(uchar group)
{
	WISPFUN_DEBUG("c15_f15");
	bool result = false;

	switch (group)
	{
		case PAG_ONMOUNT_RIDE_FAST:
		case PAG_RUN_ARMED:
		case PAG_RUN_UNARMED:
		case PAG_ONMOUNT_RIDE_SLOW:
		case PAG_WALK_WARMODE:
		case PAG_WALK_ARMED:
		case PAG_WALK_UNARMED:
		{
			if (!m_Steps.empty())
			{
				if (m_Steps.front().X != m_X || m_Steps.front().Y != m_Y)
					result = true;
			}

			break;
		}
		default:
			break;
	}
	
	return result;
}
//----------------------------------------------------------------------------------
/*!
Получить текущую группу анимации
@param [__in_opt] graphic Индекс картинки персонажа
@return Индекс группы анимации
*/
uchar CGameCharacter::GetAnimationGroup(ushort checkGraphic)
{
	WISPFUN_DEBUG("c15_f16");
	ushort graphic = checkGraphic;

	if (!graphic)
		graphic = GetMountAnimation();

	ANIMATION_GROUPS groupIndex = g_AnimationManager.GetGroupIndex(graphic);
	uchar result = AnimationGroup;

	if (result != 0xFF && !(Serial & 0x80000000) && (!AnimationFromServer || checkGraphic))
	{
		GetAnimationGroup(groupIndex, result);

		if (!g_AnimationManager.AnimationExists(graphic, result))
			CorrectAnimationGroup(graphic, groupIndex, result);
	}

	bool isWalking = Walking();
	bool isRun = (Direction & 0x80);

	if (!m_Steps.empty())
	{
		isWalking = true;
		isRun = m_Steps.front().Run();
	}

	if (groupIndex == AG_LOW)
	{
		if (isWalking)
		{
			if (isRun)
				result = (uchar)LAG_RUN;
			else
				result = (uchar)LAG_WALK;
		}
		else if (AnimationGroup == 0xFF)
		{
			result = (uchar)LAG_STAND;
			AnimIndex = 0;
		}
	}
	else if (groupIndex == AG_HIGHT)
	{
		if (isWalking)
		{
			result = (uchar)HAG_WALK;

			if (isRun)
			{
				if (g_AnimationManager.AnimationExists(graphic, HAG_FLY))
					result = (uchar)HAG_FLY;
			}
		}
		else if (AnimationGroup == 0xFF)
		{
			result = (uchar)HAG_STAND;
			AnimIndex = 0;
		}

		//!Глюченный дельфин на всех клиентах
		if (graphic == 151)
			result++;
	}
	else if (groupIndex == AG_PEOPLE)
	{
		bool InWar = InWarMode();

		if (IsPlayer())
			InWar = g_Player->Warmode;

		if (isWalking)
		{
			if (isRun)
			{
				if (FindLayer(OL_MOUNT) != NULL)
					result = (uchar)PAG_ONMOUNT_RIDE_FAST;
				else if (FindLayer(OL_1_HAND) != NULL || FindLayer(OL_2_HAND) != NULL)
					result = (uchar)PAG_RUN_ARMED;
				else
					result = (uchar)PAG_RUN_UNARMED;

				if (!IsHuman() && !g_AnimationManager.AnimationExists(graphic, result))
					goto test_walk;
			}
			else
			{
			test_walk:
				if (FindLayer(OL_MOUNT) != NULL)
					result = (uchar)PAG_ONMOUNT_RIDE_SLOW;
				else if ((FindLayer(OL_1_HAND) != NULL || FindLayer(OL_2_HAND) != NULL) && !Dead())
				{
					if (InWar )
						result = (uchar)PAG_WALK_WARMODE;
					else
						result = (uchar)PAG_WALK_ARMED;
				}
				else if (InWar && !Dead())
					result = (uchar)PAG_WALK_WARMODE;
				else
					result = (uchar)PAG_WALK_UNARMED;
			}
		}
		else if (AnimationGroup == 0xFF)
		{
			if (FindLayer(OL_MOUNT) != NULL)
				result = (uchar)PAG_ONMOUNT_STAND;
			else if (InWar && !Dead())
			{
				if (FindLayer(OL_1_HAND) != NULL)
					result = (uchar)PAG_STAND_ONEHANDED_ATTACK;
				else if (FindLayer(OL_2_HAND) != NULL)
					result = (uchar)PAG_STAND_TWOHANDED_ATTACK;
				else
					result = (uchar)PAG_STAND_ONEHANDED_ATTACK;
			}
			else
				result = (uchar)PAG_STAND;

			AnimIndex = 0;
		}
		//62 gg flying walk
		//63 gg flying run
		//64 gg flying idle
		//65 gg flying warmode on
		//66 gg flying taking1 dmg?
		//67 same as 60/61
		//68 gg flying taking dmg
		
		if (Race == RT_GARGOYLE)
		{
			if (Flying())
			{
				if (result == 0 || result == 1)
					result = 62;
				else if (result == 2 || result == 3)
					result = 63;
				else if (result == 4)
					result = 64;
				else if (result == 6)
					result = 66;
				else if (result == 7 || result == 8)
					result = 65;
				else if (result >= 9 && result <= 11)
				{
					result = 71;
				}
				else if (result >= 12 && result <= 14)
				{
					result = 72;
				}
				else if (result == 15)
				{
					result = 62;
				}
				else if (result == 20)
				{
					result = 77;
				}
				else if (result == 31)
				{
					result = 71;
				}
				else if (result == 34)
				{
					result = 78;
				}
				else if (result >= 200 && result <= 259)
				{
					result = 75;
				}
				else if (result >= 260 && result <= 270)
				{
					result = 75;
				}
			}
		}

	}
	return result;
}
//----------------------------------------------------------------------------------
void CGameCharacter::ProcessGargoyleAnims(int &animGroup)
{
	if (animGroup == 64 || animGroup == 65)
	{
		animGroup = InWarMode() == true ? 65 : 64;
		AnimationGroup = animGroup;
	}
}
//----------------------------------------------------------------------------------
/*!
Получить индекс картинки для вычисления картинки анимации
@return Индекс картинки персонажа
*/
ushort CGameCharacter::GetMountAnimation()
{
	WISPFUN_DEBUG("c15_f17");
	ushort graphic = Graphic;

	switch (graphic)
	{
		case 0x0192: //male ghost
		case 0x0193: //female ghost
		{
			graphic -= 2;

			break;
		}
		default:
			break;
	}
	
	return graphic;
}
//----------------------------------------------------------------------------------
/*!
не подписанная функция
@param [__inout] dir не подписанный параметр
@param [__in] canChange Можно ли изменять состояние стека хотьбы или нет
@return 
*/
void CGameCharacter::UpdateAnimationInfo(BYTE &dir, bool canChange)
{
	WISPFUN_DEBUG("c15_f18");
	dir = Direction & 7;

	if (!m_Steps.empty())
	{
		CWalkData &wd = m_Steps.front();
		
		dir = wd.Direction;
		int run = 0;

		if (dir & 0x80)
		{
			dir &= 7;
			run = 1;
		}

		if (canChange)
		{
			if (AnimationFromServer)
				SetAnimation(0xFF);

			int maxDelay = g_PathFinder.GetWalkSpeed(run, FindLayer(OL_MOUNT) != NULL) - 15;

			int delay = (int)g_Ticks - (int)LastStepTime;
			bool removeStep = (delay >= maxDelay);
			bool directionChange = false;

			if (m_X != wd.X || m_Y != wd.Y)
			{
				bool badStep = false;

				if (!OffsetX && !OffsetY)
				{
					int absX = abs(m_X - wd.X);
					int absY = abs(m_Y - wd.Y);

					badStep = (absX > 1 || absY > 1 || !(absX + absY));

					if (!badStep)
					{
						absX = m_X;
						absY = m_Y;

						g_PathFinder.GetNewXY(wd.Direction & 7, absX, absY);

						badStep = (absX != wd.X || absY != wd.Y);
					}
				}

				if (badStep)
					removeStep = true;
				else
				{
					float steps = maxDelay / g_AnimCharactersDelayValue;

					float x = delay / g_AnimCharactersDelayValue;
					float y = x;
					OffsetZ = (char)(((wd.Z - m_Z) * x) * (4.0f / steps));

					wd.GetOffset(x, y, steps);

					OffsetX = (char)x;
					OffsetY = (char)y;
				}
			}
			else
			{
				directionChange = true;

				removeStep = true; //direction change
			}

			if (removeStep)
			{
				if (IsPlayer())
				{
					if (m_X != wd.X || m_Y != wd.Y || m_Z != wd.Z)
					{
						UOI_PLAYER_XYZ_DATA xyzData = { wd.X, wd.Y, wd.Z };
						g_PluginManager.WindowProc(g_OrionWindow.Handle, UOMSG_UPDATE_PLAYER_XYZ, (WPARAM)&xyzData, 0);
					}

					if (Direction != wd.Direction)
						g_PluginManager.WindowProc(g_OrionWindow.Handle, UOMSG_UPDATE_PLAYER_DIR, (WPARAM)wd.Direction, 0);

					if (m_Z - wd.Z >= 22)
					{
						g_Orion.CreateTextMessage(TT_OBJECT, g_PlayerSerial, 3, 0, "Ouch!");
						//play sound (5) ?
					}

					if (g_Walker.m_Step[g_Walker.CurrentWalkSequence].Accepted)
					{
						int sequencePtr = g_Walker.CurrentWalkSequence + 1;

						if (sequencePtr < g_Walker.StepsCount)
						{
							int count = g_Walker.StepsCount - sequencePtr;

							IFOR(i, 0, count)
							{
								g_Walker.m_Step[sequencePtr - 1] = g_Walker.m_Step[sequencePtr];
								sequencePtr++;
							}
						}

						g_Walker.StepsCount--;
					}
					else
						g_Walker.CurrentWalkSequence++;
				}

				m_X = wd.X;
				m_Y = wd.Y;
				m_Z = wd.Z;

				if (IsPlayer())
					g_GumpManager.RemoveRangedGumps();

				UpdateRealDrawCoordinates();

				Direction = wd.Direction;

				OffsetX = 0;
				OffsetY = 0;
				OffsetZ = 0;

				m_Steps.pop_front();

				TimeToRandomFidget = g_Ticks + RANDOM_FIDGET_ANIMATION_DELAY;

				if (directionChange)
				{
					UpdateAnimationInfo(dir, canChange);
					return;
				}

				if (!RemovedFromRender())
				{
					g_MapManager.AddRender(this);
				}

				LastStepTime = g_Ticks;
			}
		}
	}
	else
	{
		OffsetX = 0;
		OffsetY = 0;
		OffsetZ = 0;
	}
}
//----------------------------------------------------------------------------------
CGameItem *CGameCharacter::FindSecureTradeBox()
{
	WISPFUN_DEBUG("c20_f25");
	QFOR(obj, m_Items, CGameItem*)
	{
		if (obj->Graphic == 0x1E5E && !obj->Layer)
			return obj;
	}

	return NULL;
}
//----------------------------------------------------------------------------------
void CGameCharacter::SetDead(bool &dead)
{
	m_Dead = dead;
}
//----------------------------------------------------------------------------------
