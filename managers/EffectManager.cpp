/****************************************************************************
**
** EffectManager.cpp
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

TEffectManager *EffectManager = NULL;
//---------------------------------------------------------------------------
/*!
Добавить эффект
@param [__in] effect Ссылка на эффект
@return 
*/
void TEffectManager::AddEffect( __in TGameEffect *effect)
{
	switch (effect->EffectType)
	{
		case EF_MOVING:
		case EF_STAY_AT_POS:
		case EF_DRAG:
		{
			if (effect->EffectType == EF_MOVING)
			{
				TGameObject *obj = World->FindWorldObject(effect->DestSerial);
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

				TGameEffectMoving *moving = (TGameEffectMoving*)effect;

				if (moving->X == moving->DestX && moving->Y == moving->DestY)
				{
					if (moving->Explode && obj != NULL)
						CreateExplodeEffect(moving);

					delete effect;

					return;
				}
			}

			if (effect->EffectType != EF_STAY_AT_POS)
			{
				TGameEffectDrag *effectDrag = (TGameEffectDrag*)effect;
				effectDrag->OffsetX = 0;
				effectDrag->OffsetY = 0;
			}

			Add(effect);

			MapManager->AddRender(effect);

			effect->Update();

			break;
		}
		case EF_LIGHTING:
		case EF_STAY_AT_SOURCE:
		{
			TGameObject *obj = World->FindWorldObject(effect->Serial);
			if (obj != NULL)
			{
				if (effect->EffectType == EF_LIGHTING)
				{
					Orion->ExecuteGumpPart(0x4E20, 10);

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
//---------------------------------------------------------------------------
/*!
Удалить эффект
@param [__in] effect Ссылка на эффект
@return 
*/
void TEffectManager::RemoveEffect( __in TGameEffect *effect)
{
	Unlink(effect);

	effect->m_Next = NULL;
	effect->m_Prev = NULL;
	delete effect;
}
//---------------------------------------------------------------------------
/*!
Создать эффект взрыва
@param [__in] effect Ссылка на эффект
@return 
*/
void TEffectManager::CreateExplodeEffect( __in TGameEffect *effect)
{
	TGameEffect *newEffect = new TGameEffect();

	newEffect->EffectType = EF_STAY_AT_SOURCE;
	newEffect->Serial = effect->DestSerial;
	newEffect->Graphic = 0x36CB;
	newEffect->Speed = 50;
	newEffect->Duration = g_Ticks + 400;
	//newEffect->FixedDirection = (fixedDirection != 0);
	
	newEffect->Color = effect->Color;
	newEffect->RenderMode = effect->RenderMode;

	AddEffect(newEffect);
}
//---------------------------------------------------------------------------
/*!
Обновление эффектов
@return 
*/
void TEffectManager::UpdateEffects()
{
	for (TGameEffect *effect = (TGameEffect*)m_Items; effect != NULL;)
	{
		TGameEffect *next = (TGameEffect*)effect->m_Next;

		effect->Update();

		effect = next;
	}
}
//---------------------------------------------------------------------------
/*!
Удаление эффектов, вышедших за пределы экрана
@return 
*/
void TEffectManager::RemoveRangedEffects()
{
}
//---------------------------------------------------------------------------