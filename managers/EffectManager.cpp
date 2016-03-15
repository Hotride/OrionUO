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
void TEffectManager::AddEffect(TGameEffect *effect)
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
				moving->Init();

				if (moving->X == moving->DestX && moving->Y == moving->DestY)
				{
					if (moving->Explode && obj != NULL)
						CreateExplodeEffect(moving);

					delete effect;

					return;
				}
			}

			Add(effect);
			MapManager->AddRender(effect);

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
					UO->ExecuteGumpPart(0x4E20, 10);

					effect->Duration = GetTickCount() + 400;
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
void TEffectManager::RemoveEffect(TGameEffect *effect)
{
	Unlink(effect);

	effect->m_Next = NULL;
	effect->m_Prev = NULL;
	delete effect;
}
//---------------------------------------------------------------------------
void TEffectManager::CreateExplodeEffect(TGameEffect *effect)
{
	TGameEffect *newEffect = new TGameEffect();

	newEffect->EffectType = EF_STAY_AT_SOURCE;
	newEffect->Serial = effect->DestSerial;
	newEffect->Graphic = 0x36CB;
	newEffect->Speed = 50;
	newEffect->Duration = GetTickCount() + 400;
	//newEffect->FixedDirection = (fixedDirection != 0);
	
	newEffect->Color = effect->Color;
	newEffect->RenderMode = effect->RenderMode;

	AddEffect(newEffect);
}
//---------------------------------------------------------------------------
void TEffectManager::UpdateEffects()
{
	TGameEffect *effect = (TGameEffect*)m_Items;

	while (effect != NULL)
	{
		TGameEffect *next = (TGameEffect*)effect->m_Next;

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

			((TGameEffectMoving*)effect)->Update();
		}

		effect = next;
	}
}
//---------------------------------------------------------------------------
void TEffectManager::RemoveRangedEffects()
{
}
//---------------------------------------------------------------------------