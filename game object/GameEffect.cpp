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
			LastChangeFrameTime = ticks + 50;

			objGraphic = CalculateCurrentGraphic();
		}
		else
			objGraphic = GetCurrentGraphic();

		if (objGraphic)
		{
			int deX = drawX;
			int deY = drawY;

			ApplyRenderMode();

			if (m_EffectType == EF_MOVING)
			{
				TGameEffectMoving *moving = (TGameEffectMoving*)this;

				deX += moving->OffsetX;
				deY += moving->OffsetY;

				if (moving->FixedDirection)
					UO->DrawStaticArt(objGraphic, m_Color, deX, deY, m_Z);
				else
				{
					//
					UO->DrawStaticArt(objGraphic, m_Color, deX, deY, m_Z);
				}
			}
			else
				UO->DrawStaticArt(objGraphic, m_Color, deX, deY, m_Z);

			glColor4f(g_DrawColor, g_DrawColor, g_DrawColor, 1.0f);
			glBlendEquation(GL_FUNC_ADD);
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
		case 1: //ok
		{
			glEnable(GL_BLEND);
			glBlendFunc(GL_ZERO, GL_SRC_COLOR);
			break;
		}
		case 2: //ok
		case 3: //ok
		{
			glEnable(GL_BLEND);
			glBlendFunc(GL_ONE, GL_ONE);
			break;
		}
		case 4:
		{
			glEnable(GL_BLEND);
			glBlendFunc(GL_DST_COLOR, GL_ONE_MINUS_SRC_ALPHA);
			glColor4f(g_DrawColor, g_DrawColor, g_DrawColor, 0.1f);
			break;
		}
		case 5:
		{
			glEnable(GL_BLEND);
			glBlendFunc(GL_DST_COLOR, GL_SRC_COLOR);
			break;
		}
		case 6: //ok
		{
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_COLOR, GL_ONE_MINUS_SRC_COLOR);
			glBlendEquation(GL_FUNC_REVERSE_SUBTRACT);
			break;
		}
		default:
			break;
	}
}
//---------------------------------------------------------------------------
TGameEffectMoving::TGameEffectMoving()
: TGameEffect(), m_CosA(0.0), m_SinA(0.0), m_OffsetX(0), m_OffsetY(0)
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

	int diffX = m_DestX - X;
	int diffY = m_DestY - Y;
	int diffZ = m_DestZ - Z;

	int posX = (diffX - diffY) * 44;
	int posY = (diffX + diffY) * 44 + diffZ * 4;
	
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

	m_OffsetX = 0;
	m_OffsetY = 0;
}
//---------------------------------------------------------------------------
void TGameEffectMoving::Update()
{
	int cx = g_RenderBounds->GameWindowCenterX;
	int cy = g_RenderBounds->GameWindowCenterY;

	int ctx = g_Player->X;
	int cty = g_Player->Y;

	int offsetX = m_X - ctx;
	int offsetY = m_Y - cty;

	int drawX = cx + (offsetX - offsetY) * 22;
	int drawY = cy + (offsetX + offsetY) * 22;

	int realDrawX = drawX + m_OffsetX;
	int realDrawY = drawY + m_OffsetY;

	int offsetDestX = m_DestX - ctx;
	int offsetDestY = m_DestY - cty;

	int drawDestX = cx + (offsetDestX - offsetDestY) * 22;
	int drawDestY = cy + (offsetDestX + offsetDestY) * 22;

	int x = 0;

	int deltaXY[2] = { abs(drawDestX - realDrawX), abs(drawDestY - realDrawY) };

	if (deltaXY[0] < deltaXY[1])
	{
		x = 1;

		int temp = deltaXY[0];

		deltaXY[0] = deltaXY[1];
		deltaXY[1] = temp;
	}

	if (deltaXY[0] == 0)
		deltaXY[0] = 1;

	double delta = deltaXY[1] / (double)deltaXY[0];
	double stepXY = 0.0;

	int step = m_Speed;
	int tempXY[2] = { step, 0 };

	for (int j = 0; j < step; j++)
	{
		stepXY += delta;

		if (stepXY >= 0.5)
		{
			tempXY[1] += 1;

			stepXY -= 1.0;
		}
	}

	bool incX = (realDrawX < drawDestX);
	bool incY = (realDrawY < drawDestY);

	if (incX)
	{
		realDrawX += tempXY[x];

		if (realDrawX > drawDestX)
			realDrawX = drawDestX;
	}
	else
	{
		realDrawX -= tempXY[x];

		if (realDrawX < drawDestX)
			realDrawX = drawDestX;
	}

	if (incY)
	{
		realDrawY += tempXY[(x + 1) % 2];

		if (realDrawY > drawDestY)
			realDrawY = drawDestY;
	}
	else
	{
		realDrawY -= tempXY[(x + 1) % 2];

		if (realDrawY < drawDestY)
			realDrawY = drawDestY;
	}

	int ox = (realDrawX - cx) / 22;
	int oy = (realDrawY - cy) / 22;

	int dx = 0;
	int dy = 0;

	TileOffsetOnMonitorToXY(ox, oy, dx, dy);

	int newX = ctx + dx;
	int newY = cty + dy;

	if (newX == m_DestX && newY == m_DestY)
	{
		if (m_Explode)
		{
			TGameObject *obj = World->FindWorldObject(m_Serial);

			if (obj != NULL && obj->GetTopObject() != NULL)
				EffectManager->CreateExplodeEffect(this);
		}

		EffectManager->RemoveEffect(this);
	}
	else
	{
		int newDrawX = cx + (dx - dy) * 22;
		int newDrawY = cy + (dx + dy) * 22;

		m_OffsetX = realDrawX - newDrawX;
		m_OffsetY = realDrawY - newDrawY;

		if (m_X != newX || m_Y != newY)
		{
			m_X = newX;
			m_Y = newY;

			if (m_Z < m_DestZ)
				m_Z++;
			else if (m_Z > m_DestZ)
				m_Z--;

			MapManager->AddRender(this);
		}
	}

	/*if (m_Step == m_Distance + 1)
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
	}*/
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