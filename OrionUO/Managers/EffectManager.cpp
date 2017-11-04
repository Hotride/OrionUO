// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
/***********************************************************************************
**
** EffectManager.cpp
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#include "stdafx.h"
//----------------------------------------------------------------------------------
CEffectManager g_EffectManager;
//----------------------------------------------------------------------------------
CEffectManager::CEffectManager()
: CBaseQueue()
{
}
//----------------------------------------------------------------------------------
/*!
Добавить эффект
@param [__in] effect Ссылка на эффект
@return 
*/
void CEffectManager::AddEffect(CGameEffect *effect)
{
	WISPFUN_DEBUG("c141_f1");
	switch (effect->EffectType)
	{
		case EF_MOVING:
		case EF_STAY_AT_POS:
		case EF_DRAG:
		{
			if (effect->EffectType == EF_MOVING)
			{
				CGameObject *obj = g_World->FindWorldObject(effect->DestSerial);
				if (obj != NULL)
				{
					obj = obj->GetTopObject();

					if (obj != NULL)
					{
						effect->DestX = obj->X;
						effect->DestY = obj->Y;
						effect->DestZ = obj->Z;
					}
				}

				CGameEffectMoving *moving = (CGameEffectMoving*)effect;

				if (moving->X == moving->DestX && moving->Y == moving->DestY && moving->Z == moving->DestZ)
				{
					if (moving->Explode)
					{
						EFFECT_TYPE type = EF_STAY_AT_POS;

						if (obj != NULL)
							type = EF_STAY_AT_SOURCE;

						CreateExplodeEffect(moving, type);
					}

					delete effect;

					return;
				}
			}

			if (effect->EffectType != EF_STAY_AT_POS)
			{
				CGameEffectDrag *effectDrag = (CGameEffectDrag*)effect;
				effectDrag->OffsetX = 0;
				effectDrag->OffsetY = 0;
			}

			Add(effect);

			g_MapManager.AddRender(effect);

			effect->Update(NULL);

			break;
		}
		case EF_LIGHTING:
		case EF_STAY_AT_SOURCE:
		{
			CGameObject *obj = g_World->FindWorldObject(effect->Serial);
			if (obj != NULL)
			{
				if (effect->EffectType == EF_LIGHTING)
				{
					g_Orion.ExecuteGumpPart(0x4E20, 10);

					effect->Duration = g_Ticks + 400;
					effect->Speed = 50;
				}

				obj->AddEffect(effect);
			}
			else
				delete effect;

			break;
		}
		default:
			break;
	}
}
//----------------------------------------------------------------------------------
/*!
Удалить эффект
@param [__in] effect Ссылка на эффект
@return 
*/
void CEffectManager::RemoveEffect(CGameEffect *effect)
{
	WISPFUN_DEBUG("c141_f2");
	Unlink(effect);

	effect->m_Next = NULL;
	effect->m_Prev = NULL;
	delete effect;
}
//----------------------------------------------------------------------------------
/*!
Создать эффект взрыва
@param [__in] effect Ссылка на эффект
@return 
*/
void CEffectManager::CreateExplodeEffect(CGameEffect *effect, const EFFECT_TYPE &type)
{
	WISPFUN_DEBUG("c141_f3");
	CGameEffect *newEffect = new CGameEffect();

	newEffect->EffectType = type;
	newEffect->Serial = effect->DestSerial;
	newEffect->X = effect->DestX;
	newEffect->Y = effect->DestY;
	newEffect->Z = effect->DestZ;
	newEffect->Graphic = 0x36CB;
	newEffect->Speed = 50;
	newEffect->Duration = g_Ticks + 400;
	newEffect->FixedDirection = effect->FixedDirection;
	
	newEffect->Color = effect->Color;
	newEffect->RenderMode = effect->RenderMode;

	AddEffect(newEffect);
}
//----------------------------------------------------------------------------------
/*!
Обновление эффектов
@return 
*/
void CEffectManager::UpdateEffects()
{
	WISPFUN_DEBUG("c141_f3");
	for (CGameEffect *effect = (CGameEffect*)m_Items; effect != NULL;)
	{
		CGameEffect *next = (CGameEffect*)effect->m_Next;

		effect->Update(NULL);

		effect = next;
	}
}
//----------------------------------------------------------------------------------
/*!
Удаление эффектов, вышедших за пределы экрана
@return 
*/
void CEffectManager::RemoveRangedEffects()
{
	WISPFUN_DEBUG("c141_f4");
}
//----------------------------------------------------------------------------------
