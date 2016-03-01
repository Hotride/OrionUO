/****************************************************************************
**
** GameEffect.cpp
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
TGameEffect::TGameEffect()
: TRenderWorldObject(ROT_EFFECT, 0, 0, 0, 0, 0, 0), m_EffectType(EF_MOVING),
m_DestSerial(0), m_DestX(0), m_DestY(0), m_DestZ(0), m_Speed(0), m_Duration(0),
m_FixedDirection(false), m_Explode(false), m_RenderMode(0), m_AnimIndex(0),
m_Increment(0), m_LastChangeFrameTime(0)
{
	m_RenderQueueIndex = 8;
}
//---------------------------------------------------------------------------
TGameEffect::~TGameEffect()
{
}
//---------------------------------------------------------------------------
int TGameEffect::Draw(bool &mode, int &drawX, int &drawY, DWORD &ticks)
{
	if (mode)
	{
#if UO_DEBUG_INFO!=0
		g_RenderedObjectsCountInGameWindow++;
#endif

		WORD objGraphic = 0;

		if (m_EffectType == EF_STAY_AT_POS && m_Duration < ticks)
			EffectManager->RemoveEffect(this);
		else if (m_EffectType == EF_DRAG)
		{
			if (m_Duration < ticks)
				EffectManager->RemoveEffect(this);
			else
			{
				TGameEffectDrag *ed = (TGameEffectDrag*)this;

				int deX = drawX - ed->OffsetX;
				int deY = drawY - ed->OffsetY;

				UO->DrawStaticArt(m_Graphic, m_Color, deX, deY, m_Z);

				ed->AddOffsetX(10);
				ed->AddOffsetY(10);
			}
		}
		else if (LastChangeFrameTime < ticks)
		{
			LastChangeFrameTime = ticks + m_Speed;

			objGraphic = CalculateCurrentGraphic();
		}
		else
			objGraphic = GetCurrentGraphic();

		if (objGraphic)
		{
			int deX = drawX;
			int deY = drawY;
			int deZ = 0;

			ApplyRenderMode();

			if (m_EffectType == EF_MOVING)
			{
				TGameEffectMoving *moving = (TGameEffectMoving*)this;

				deX += moving->OffsetX;
				deY += moving->OffsetY;
				deZ += moving->OffsetZ;

				UO->DrawStaticArt(objGraphic, m_Color, deX, deY, m_Z + deZ);
			}
			else
				UO->DrawStaticArt(objGraphic, m_Color, deX, deY, m_Z + deZ);

			glDisable(GL_BLEND);
		}
	}

	return 0;
}
//---------------------------------------------------------------------------
WORD TGameEffect::CalculateCurrentGraphic()
{
	DWORD addressAnimData = (DWORD)FileManager.AnimdataMul.Address;

	if (addressAnimData)
	{
		DWORD addr = (m_Graphic * 68) + 4 * ((m_Graphic / 8) + 1);
		PANIM_DATA pad = (PANIM_DATA)(addressAnimData + addr);
			
		if (m_AnimIndex < (int)pad->FrameCount - 1)
		{
			m_Increment = pad->FrameData[m_AnimIndex];
			m_AnimIndex++;
		}
		else
		{
			m_AnimIndex  = 0;
			m_Increment = pad->FrameData[m_AnimIndex];
		}
	}

	//trace_printf("Generate effectID for 0x%04X (add %i)\n", m_Graphic, m_Increment);

	return m_Graphic + m_Increment;
}
//---------------------------------------------------------------------------
WORD TGameEffect::GetCurrentGraphic()
{
	return m_Graphic + m_Increment;
}
//---------------------------------------------------------------------------
void TGameEffect::ApplyRenderMode()
{
	switch (m_RenderMode % 7)
	{
		case 0:
		{
			break;
		}
		case 1:
		{
			glEnable(GL_BLEND);

			//glBlendFunc(GL_SRC_ALPHA, GL_SRC_ALPHA);
			break;
		}
		case 2:
		{
			glEnable(GL_BLEND);
			glBlendFunc(GL_ONE, GL_ONE);
			break;
		}
		case 3:
		{
			glEnable(GL_BLEND);
			glBlendFunc(GL_ONE, GL_ONE);
			break;
		}
		case 4:
		{
			glEnable(GL_BLEND);
			glBlendFunc(GL_ONE, GL_ONE);
			break;
		}
		case 5:
		{
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_COLOR, GL_SRC_ALPHA_SATURATE);
			break;
		}
		case 6:
		{
			glEnable(GL_BLEND);
			glBlendFunc(GL_ONE, GL_ONE);
			//glBlendEquation(GL_FUNC_REVERSE_SUBTRACT);
			break;
		}
		default:
			break;
	}
}
//---------------------------------------------------------------------------
TGameEffectMoving::TGameEffectMoving()
: TGameEffect(), m_DiffX(0), m_DiffY(0), m_DiffZ(0), m_CosA(0.0), m_SinA(0.0),
m_OffsetX(0), m_OffsetY(0), m_OffsetZ(0), m_Step(0), m_Distance(0)
{
}
//---------------------------------------------------------------------------
TGameEffectMoving::~TGameEffectMoving()
{
}
//---------------------------------------------------------------------------
void TGameEffectMoving::Init()
{
	TGameObject *obj = World->FindWorldObject(DestSerial);

	if (obj != NULL)
	{
		obj = obj->GetTopObject();
				
		if (obj != NULL)
		{
			m_DestX = obj->X;
			m_DestY = obj->Y;
			m_DestZ = obj->Z;
		}
	}

	m_DiffX = m_DestX - X;
	m_DiffY = m_DestY - Y;
	m_DiffZ = m_DestZ - Z;

	int posX = (m_DiffX - m_DiffY) * 44;
	int posY = (m_DiffX + m_DiffY) * 44 + m_DiffZ * 4;
	
	double alpha = 0.0;
	if (posX == 0)
		alpha = M_PI / 2.0;
	else
		alpha = atan(posY / posX);
	
	if (alpha < 0.0)
		alpha += M_PI;

	if ((posY > 0) || ((posX > 0) && (posY == 0)))
		alpha += M_PI;

	alpha -= (M_PI / 2.0);
	if (alpha < 0)
		alpha += (2.0 * M_PI);
	
	m_CosA = cos(alpha);
	m_SinA = sin(alpha);

	m_DiffZ = DestZ - Z;
	m_Distance = (int)floor(sqrt(m_DiffX * m_DiffX + m_DiffY * m_DiffY)) * 2 + 1;                

	m_Step = 0;
	m_OffsetX = 0;
	m_OffsetY = 0;
	m_OffsetZ = 0;
}
//---------------------------------------------------------------------------
void TGameEffectMoving::Update()
{
	if (m_Step == m_Distance + 1)
	{
		if (Explode)
		{
			TGameObject *obj = World->FindWorldObject(m_Serial);

			if (obj != NULL && obj->GetTopObject() != NULL)
				EffectManager->CreateExplodeEffect(this);
		}

		EffectManager->RemoveEffect(this);
	}
	else
	{
		int oldX = m_X;
		int oldY = m_Y;
		int oldZ = m_Z;

		int newX = oldX + (int)(m_DiffX * m_Step / m_Distance);
		int newY = oldY + (int)(m_DiffY * m_Step / m_Distance);
		int newZ = oldZ + (int)(m_DiffZ * m_Step / m_Distance);

		m_OffsetX = (int)floor((m_DiffX - m_DiffY) * 22 * m_Step / m_Distance);
		m_OffsetY = (int)floor((m_DiffX + m_DiffY) * 22 * m_Step / m_Distance);
		m_OffsetZ = (int)floor(m_DiffZ * 4 * m_Step / m_Distance);

		m_Step++;

		if (oldX != newX || oldY != newY || oldZ != newZ)
		{
			m_X = newX;
			m_Y = newY;
			m_Z = newZ;

			Init();

			MapManager->AddRender(this);
		}
	}
}
//---------------------------------------------------------------------------
TGameEffectDrag::TGameEffectDrag()
: TGameEffect(), m_OffsetX(0), m_OffsetY(0)
{
}
//---------------------------------------------------------------------------
TGameEffectDrag::~TGameEffectDrag()
{
}
//---------------------------------------------------------------------------