/****************************************************************************
**
** GameObject.cpp
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
//---------------------------------------------------------------------------
TGameCharacter::TGameCharacter(DWORD serial)
: TGameObject(serial), m_Hits(0), m_MaxHits(0), m_Sex(false), m_Direction(0),
m_Notoriety(0), m_CanChangeName(false), m_AnimationGroup(0xFF),
m_AnimationInterval(0), m_AnimationFrameCount(0), m_AnimationRepeat(false),
m_AnimationRepeatMode(1), m_AnimationDirection(false), m_AnimationFromServer(false),
m_MaxMana(0), m_MaxStam(0), m_Mana(0), m_Stam(0), m_OffsetX(0), m_OffsetY(0),
m_OffsetZ(0), m_LastStepTime(0), m_LastStepSoundTime(GetTickCount()), m_Race(0),
m_TimeToRandomFidget(GetTickCount() + RANDOM_FIDGET_ANIMATION_DELAY),
m_AfterStepDelay(0), m_StepSoundOffset(0), m_CorpseLink(0)
{
	m_RenderQueueIndex = 7; //Высокий приоритет прорисовки (будет выше остального на тайле с одинаковой Z коориднатой)
	m_WalkStack.Init(); //Инициализация счетчика шагов

	if (!ConfigManager.DisableNewTargetSystem && NewTargetSystem.Serial == serial && GumpManager->GetGump(serial, 0, GT_TARGET_SYSTEM) == NULL)
		GumpManager->AddGump(new TGumpTargetSystem(m_Serial, NewTargetSystem.GumpX, NewTargetSystem.GumpY));
}
//---------------------------------------------------------------------------
TGameCharacter::~TGameCharacter()
{
	//Чистим память
	m_WalkStack.Clear();
	m_PaperdollTextTexture.Clear();

	//Если стянут статусбар - обновим его
	GumpManager->UpdateGump(m_Serial, 0, GT_STATUSBAR);
	
	//Если стянут статусбар таргет системы - обновим его
	GumpManager->UpdateGump(m_Serial, 0, GT_TARGET_SYSTEM);
}
//---------------------------------------------------------------------------
void TGameCharacter::SetPaperdollText(string val)
{
	m_PaperdollTextTexture.Clear(); //Очищаем текстуру
	m_PaperdollText = val;

	if (val.length()) //Если текст есть - сгенерируем новую текстуру
		FontManager->GenerateA(1, m_PaperdollTextTexture, val.c_str(), 0x0386, 185);
}
//---------------------------------------------------------------------------
int TGameCharacter::IsSitting()
{
	int result = 0;
	WORD testGraphic = m_Graphic;
	AnimationManager->GetBodyGraphic(testGraphic);
	bool human = (((testGraphic >= 0x0190) && (testGraphic <= 0x0193)) || (testGraphic == 0x03DB) || (testGraphic == 0x03DF) || (testGraphic == 0x03E2));

	if (human && FindLayer(OL_MOUNT) == NULL && !TestStepNoChangeDirection(GetAnimationGroup()))
	{
		TRenderWorldObject *obj = this;

		while (obj != NULL && obj->m_PrevXY != NULL)
			obj = obj->m_PrevXY;

		while (obj != NULL && !result)
		{
			if (obj->IsStaticGroupObject() && abs(m_Z - obj->Z) <= 1) //m_Z == obj->Z
			{
				WORD graphic = obj->Graphic;

				if (obj->IsGameObject())
				{
					if (graphic >= 0x4000 || ((TGameObject*)obj)->NPC)
						graphic = 0;
				}
				else
					graphic -= 0x4000;

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
								result = i + 1;
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
//---------------------------------------------------------------------------
int TGameCharacter::Draw(bool &mode, int &drawX, int &drawY, DWORD &ticks)
{
	if (mode)
	{
		if (m_TimeToRandomFidget < ticks)
			SetRandomFidgetAnimation();

#if UO_DEBUG_INFO!=0
		g_RenderedObjectsCountInGameWindow++;
#endif

		DWORD lastSBsel = g_StatusbarUnderMouse;

		if (!IsPlayer() && g_Player->Warmode && g_LastSelectedObject == m_Serial)
			g_StatusbarUnderMouse = m_Serial;

		AnimationManager->DrawCharacter(this, drawX, drawY, m_Z); //Draw character

		g_StatusbarUnderMouse = lastSBsel;

		DrawEffects(drawX, drawY, ticks);
	}
	else
	{
		if (AnimationManager->CharacterPixelsInXY(this, drawX, drawY, m_Z))
		{
			g_LastObjectType = SOT_GAME_OBJECT;
			g_LastSelectedObject = m_Serial;
			g_SelectedObject = this;
		}
	}

	return 0;
}
//---------------------------------------------------------------------------
void TGameCharacter::OnGraphicChange(int direction)
{
	//Обновления пола в зависимости от индекса картинки персонажа
	switch (m_Graphic)
	{
		case 0x0190:
		case 0x0192:
		{
			m_Sex = false;
			break;
		}
		case 0x0191:
		case 0x0193:
		{
			m_Sex = true;
			break;
		}
		default:
			break;
	}

	if (direction == 1000)
	{
		if (!m_Clicked && !IsPlayer())
		{
			UO->StatusReq(m_Serial);

			if (ConfigManager.ShowIncomingNames)
				UO->NameReq(m_Serial);
		}
	
		GumpManager->UpdateGump(m_Serial, 0, GT_PAPERDOLL);
		GumpManager->UpdateGump(m_Serial, 0, GT_STATUSBAR);
	}
}
//---------------------------------------------------------------------------
bool TGameCharacter::IsCorrectStep(WORD &cx, WORD &cy, WORD &x, WORD &y, BYTE &dir)
{
	switch (dir & 7)
	{
		case 0:
		{
			cy--;
			break;
		}
		case 1:
		{
			cx++;
			cy--;
			break;
		}
		case 2:
		{
			cx++;
			break;
		}
		case 3:
		{
			cx++;
			cy++;
			break;
		}
		case 4:
		{
			cy++;
			break;
		}
		case 5:
		{
			cx--;
			cy++;
			break;
		}
		case 6:
		{
			cx--;
			break;
		}
		case 7:
		{
			cx--;
			cy--;
			break;
		}
	}

	return (cx == x && cy == y);
}
//---------------------------------------------------------------------------
bool TGameCharacter::IsTeleportAction(WORD &x, WORD &y, BYTE &dir)
{
	bool result = false;

	TWalkData *wd = m_WalkStack.m_Items;
	
	WORD cx = m_X;
	WORD cy = m_Y;
	BYTE cdir = m_Direction;

	if (wd != NULL)
	{
		while (wd->m_Next != NULL)
			wd = wd->m_Next;
		
		cx = wd->X;
		cy = wd->Y;
		cdir = wd->Direction;
	}

	if ((cdir & 7) == (dir & 7))
		result = !IsCorrectStep(cx, cy, x, y, dir);

	return result;
}
//---------------------------------------------------------------------------
void TGameCharacter::SetAnimation(BYTE id, BYTE interval, BYTE frameCount, BYTE repeatCount, bool repeat, bool frameDirection)
{
	m_AnimationGroup = id;
	m_AnimIndex = 0;
	m_AnimationInterval = interval;
	m_AnimationFrameCount = frameCount;
	m_AnimationRepeatMode = repeatCount;
	m_AnimationRepeat = repeat;
	m_AnimationDirection = frameDirection;
	m_AnimationFromServer = false;

	m_LastAnimationChangeTime = GetTickCount();
	m_TimeToRandomFidget = GetTickCount() + RANDOM_FIDGET_ANIMATION_DELAY;
}
//---------------------------------------------------------------------------
void TGameCharacter::SetRandomFidgetAnimation()
{
	m_AnimIndex = 0;
	m_AnimationFrameCount = 0;
	m_AnimationInterval = 1;
	m_AnimationRepeatMode = 1;
	m_AnimationDirection = true;
	m_AnimationRepeat = false;
	m_AnimationFromServer = true;

	m_TimeToRandomFidget = GetTickCount() + RANDOM_FIDGET_ANIMATION_DELAY;

	ANIMATION_GROUPS groupIndex = AnimationManager->GetGroupIndex(GetMountAnimation());

	const BYTE fidgetAnimTable[3][3] =
	{
		{LAG_FIDGET_1, LAG_FIDGET_2, LAG_FIDGET_1},
		{HAG_FIDGET_1, HAG_FIDGET_2, HAG_FIDGET_1},
		{PAG_FIDGET_1, PAG_FIDGET_2, PAG_FIDGET_3}
	};

	m_AnimationGroup = fidgetAnimTable[groupIndex - 1][RandomInt(3)];
}
//---------------------------------------------------------------------------
void TGameCharacter::SetAnimationGroup(BYTE val)
{
	m_AnimationFrameCount = 0;
	m_AnimationInterval = 0;
	m_AnimationRepeat = false;
	m_AnimationRepeatMode = 0;
	m_AnimationDirection = false;
	m_AnimationFromServer = false;

	m_AnimationGroup = val;
}
//---------------------------------------------------------------------------
void TGameCharacter::GetAnimationGroup(ANIMATION_GROUPS group, BYTE &animation)
{
	const BYTE animAssociateTable[35][3] =
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

	if (group && animation < 35)
		animation = animAssociateTable[animation][group - 1];
}
//---------------------------------------------------------------------------
void TGameCharacter::CorrectAnimationGroup(WORD &graphic, ANIMATION_GROUPS &group, BYTE &animation)
{
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

		if (!AnimationManager->AnimationExists(graphic, animation))
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

		if (!AnimationManager->AnimationExists(graphic, animation))
			animation = HAG_STAND;
	}
}
//---------------------------------------------------------------------------
bool TGameCharacter::TestStepNoChangeDirection(BYTE group)
{
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
			if (m_WalkStack.m_Items != NULL)
			{
				TWalkData *wd = m_WalkStack.m_Items;

				if (wd->X != m_X || wd->Y != m_Y)
					result = true;
			}

			break;
		}
		default:
			break;
	}
	
	return result;
}
//---------------------------------------------------------------------------
BYTE TGameCharacter::GetAnimationGroup(WORD graphic)
{
	if (!graphic)
		graphic = GetMountAnimation();

	AnimationManager->GetBodyGraphic(graphic);

	ANIMATION_GROUPS groupIndex = AnimationManager->GetGroupIndex(graphic);
	BYTE result = m_AnimationGroup;

	if (result != 0xFF)
	{
		GetAnimationGroup(groupIndex, result);

		if (!AnimationManager->AnimationExists(graphic, result))
			CorrectAnimationGroup(graphic, groupIndex, result);
	}

	bool isWalking = Walking();
	bool isRun = (m_Direction & 0x80);

	if (!m_WalkStack.Empty())
	{
		isWalking = true;
		isRun = m_WalkStack.m_Items->Run();
	}

	if (groupIndex == AG_LOW)
	{
		if (isWalking)
		{
			if (isRun)
				result = (BYTE)LAG_RUN;
			else
				result = (BYTE)LAG_WALK;
		}
		else if (m_AnimationGroup == 0xFF)
			result = (BYTE)LAG_STAND;
	}
	else if (groupIndex == AG_HIGHT)
	{
		if (isWalking)
		{
			result = (BYTE)HAG_WALK;

			if (isRun)
			{
				if (AnimationManager->AnimationExists(graphic, HAG_FLY))
					result = (BYTE)HAG_FLY;
			}
		}
		else if (m_AnimationGroup == 0xFF)
			result = (BYTE)HAG_STAND;

		//Глюченный дельфин на всех клиентах
		if (graphic == 151)
			result++;
	}
	else if (groupIndex == AG_PEOPLE)
	{
		bool InWar = InWarMode();

		if (Serial == g_PlayerSerial)
			InWar = g_Player->Warmode;

		if (isWalking)
		{
			if (isRun)
			{
				if (FindLayer(OL_MOUNT) != NULL)
					result = (BYTE)PAG_ONMOUNT_RIDE_FAST;
				else if (FindLayer(OL_1_HAND) != NULL || FindLayer(OL_2_HAND) != NULL)
					result = (BYTE)PAG_RUN_ARMED;
				else
					result = (BYTE)PAG_RUN_UNARMED;

				if (!AnimationManager->AnimationExists(graphic, result))
					goto test_walk;
			}
			else
			{
				test_walk:
				if (FindLayer(OL_MOUNT) != NULL)
					result = (BYTE)PAG_ONMOUNT_RIDE_SLOW;
				else if (FindLayer(OL_1_HAND) != NULL || FindLayer(OL_2_HAND) != NULL)
				{
					if (InWar)
						result = (BYTE)PAG_WALK_WARMODE;
					else
						result = (BYTE)PAG_WALK_ARMED;
				}
				else if (InWar)
					result = (BYTE)PAG_WALK_WARMODE;
				else
					result = (BYTE)PAG_WALK_UNARMED;
			}
		}
		else if (m_AnimationGroup == 0xFF)
		{
			if (FindLayer(OL_MOUNT) != NULL)
				result = (BYTE)PAG_ONMOUNT_STAND;
			else if (InWar)
			{
				if (FindLayer(OL_1_HAND) != NULL)
					result = (BYTE)PAG_STAND_ONEHANDED_ATTACK;
				else if (FindLayer(OL_2_HAND) != NULL)
					result = (BYTE)PAG_STAND_TWOHANDED_ATTACK;
				else
					result = (BYTE)PAG_STAND_ONEHANDED_ATTACK;
			}
			else
				result = (BYTE)PAG_STAND;
		}
	}

	return result;
}
//---------------------------------------------------------------------------
WORD TGameCharacter::GetMountAnimation()
{
	WORD graphic = m_Graphic;

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
//---------------------------------------------------------------------------
void TGameCharacter::UpdateAnimationInfo(BYTE &dir, bool canChange)
{
	dir = m_Direction;
	if (dir & 0x80)
		dir ^= 0x80;

	TWalkData *wd = m_WalkStack.m_Items;

	if (wd != NULL)
	{
		DWORD ticks = GetTickCount();

		m_TimeToRandomFidget = ticks + RANDOM_FIDGET_ANIMATION_DELAY;
		
		dir = wd->Direction;
		int run = 0;

		if (dir & 0x80)
		{
			dir ^= 0x80;
			run = 1;
		}

		if (canChange)
		{
			int onMount = (int)(FindLayer(OL_MOUNT) != NULL);

			int maxDelay = CHARACTER_ANIMATION_DELAY_TABLE[onMount][run];

			int delay = (int)ticks - (int)m_LastStepTime;
			bool removeStep = (delay >= maxDelay);

			if (m_X != wd->X || m_Y != wd->Y)
			{
				float steps = maxDelay / g_AnimCharactersDelayValue;
				
				float x = (delay / g_AnimCharactersDelayValue);
				float y = x;
				m_OffsetZ = (char)((wd->Z - m_Z) * x);

				wd->GetOffset(x, y, steps);

				m_OffsetX = (char)x;
				m_OffsetY = (char)y;
			}
			else
			{
				m_OffsetX = 0;
				m_OffsetY = 0;
				m_OffsetZ = 0;

				removeStep = (delay >= 100); //direction change
			}

			if (removeStep)
			{
				m_X = wd->X;
				m_Y = wd->Y;
				m_Z = wd->Z;
				m_Direction = wd->Direction;

				m_AfterStepDelay = maxDelay / 3;

				m_OffsetX = 0;
				m_OffsetY = 0;
				m_OffsetZ = 0;

				m_WalkStack.Pop();

				MapManager->AddRender(this);
				//World->MoveToTop(this);

				m_LastStepTime = ticks;
			}
		}
	}
	else
	{
		m_OffsetX = 0;
		m_OffsetY = 0;
		m_OffsetZ = 0;
	}
}
//---------------------------------------------------------------------------
TGameItem *TGameCharacter::FindLayer(int layer)
{
	TGameItem *item = NULL;

	QFOR(obj, m_Items, TGameObject*)
	{
		if (((TGameItem*)obj)->Layer == layer)
		{
			item = (TGameItem*)obj;
			break;
		}
	}

	return item;
}
//---------------------------------------------------------------------------