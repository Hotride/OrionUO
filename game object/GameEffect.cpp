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
				TGameEffectDrag *dragEffect = (TGameEffectDrag*)this;

				UO->DrawStaticArt(m_Graphic, m_Color, drawX - dragEffect->OffsetX, drawY - dragEffect->OffsetY, m_Z);
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
			int drawEffectX = drawX;
			int drawEffectY = drawY;

			ApplyRenderMode();

			if (m_EffectType == EF_MOVING)
			{
				TGameEffectMoving *moving = (TGameEffectMoving*)this;

				drawEffectX += moving->OffsetX;
				drawEffectY += moving->OffsetY + moving->OffsetZ;

				if (moving->FixedDirection)
					UO->DrawStaticArt(objGraphic, m_Color, drawEffectX, drawEffectY, m_Z);
				else
				{
					TTextureObject *th = UO->ExecuteStaticArt(objGraphic);

					if (th != NULL)
					{
						glBindTexture(GL_TEXTURE_2D, th->Texture);

						drawEffectY -= (m_Z * 4);

						int width = th->Width;
						int heightDiv2 = th->Height / 2;

						glLoadIdentity();
						glTranslatef((GLfloat)drawEffectX, (GLfloat)(drawEffectY - heightDiv2), 0.0f);

						glRotatef(moving->Angle, 0.0f, 0.0f, 1.0f);

						glBegin(GL_TRIANGLE_STRIP);
							glTexCoord2i(0, 1); glVertex2i(0, -heightDiv2);
							glTexCoord2i(1, 1); glVertex2i(width, -heightDiv2);
							glTexCoord2i(0, 0); glVertex2i(0, heightDiv2);
							glTexCoord2i(1, 0); glVertex2i(width, heightDiv2);
						glEnd();
					}
				}
			}
			else
				UO->DrawStaticArt(objGraphic, m_Color, drawEffectX, drawEffectY, m_Z);

			RemoveRenderMode();
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

	//TPRINT("Generate effectID for 0x%04X (add %i)\n", m_Graphic, m_Increment);

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
	switch (m_RenderMode)
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
		case 4: //?
		{
			glEnable(GL_BLEND);
			glBlendFunc(GL_DST_COLOR, GL_ONE_MINUS_SRC_ALPHA);
			break;
		}
		case 5: //?
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
void TGameEffect::RemoveRenderMode()
{
	switch (m_RenderMode)
	{
		case 1: //ok
		case 2: //ok
		case 3: //ok
		{
			glDisable(GL_BLEND);
			break;
		}
		case 4:
		{
			glDisable(GL_BLEND);
			break;
		}
		case 5:
		{
			glDisable(GL_BLEND);
			break;
		}
		case 6: //ok
		{
			glDisable(GL_BLEND);
			glBlendEquation(GL_FUNC_ADD);
			break;
		}
		default:
			break;
	}
}
//---------------------------------------------------------------------------
//------------------------------TGameEffectDrag------------------------------
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
void TGameEffectDrag::Update()
{
	m_OffsetX += 10;
	m_OffsetY += 10;
}
//---------------------------------------------------------------------------
//-----------------------------TGameEffectMoving-----------------------------
//---------------------------------------------------------------------------
TGameEffectMoving::TGameEffectMoving()
: TGameEffectDrag(), m_Angle(0.0f), m_OffsetZ(0)
{
}
//---------------------------------------------------------------------------
TGameEffectMoving::~TGameEffectMoving()
{
}
//---------------------------------------------------------------------------
void TGameEffectMoving::Update()
{
	TGameObject *obj = World->FindWorldObject(m_DestSerial);

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

	int screenCenterX = g_RenderBounds->GameWindowCenterX;
	int screenCenterY = g_RenderBounds->GameWindowCenterY;

	int playerX = g_Player->X;
	int playerY = g_Player->Y;

	int offsetX = m_X - playerX;
	int offsetY = m_Y - playerY;

	int drawX = screenCenterX + (offsetX - offsetY) * 22;
	int drawY = screenCenterY + (offsetX + offsetY) * 22;

	int realDrawX = drawX + m_OffsetX;
	int realDrawY = drawY + m_OffsetY;

	int offsetDestX = m_DestX - playerX;
	int offsetDestY = m_DestY - playerY;

	int drawDestX = screenCenterX + (offsetDestX - offsetDestY) * 22;
	int drawDestY = screenCenterY + (offsetDestX + offsetDestY) * 22;

	int deltaXY[2] = { abs(drawDestX - realDrawX), abs(drawDestY - realDrawY) };

	int x = 0;

	if (deltaXY[0] < deltaXY[1])
	{
		x = 1;

		int temp = deltaXY[0];

		deltaXY[0] = deltaXY[1];
		deltaXY[1] = temp;
	}

	int delta = deltaXY[0];
	int stepXY = 0;
	int tempXY[2] = { m_Speed, 0 };

	for (int j = 0; j < m_Speed; j++)
	{
		stepXY += deltaXY[1];

		if (stepXY >= delta)
		{
			tempXY[1]++;

			stepXY -= deltaXY[0];
		}
	}

	if (realDrawX < drawDestX)
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

	if (realDrawY < drawDestY)
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

	int newOffsetX = (realDrawX - screenCenterX) / 22;
	int newOffsetY = (realDrawY - screenCenterY) / 22;

	int newCoordX = 0;
	int newCoordY = 0;

	TileOffsetOnMonitorToXY(newOffsetX, newOffsetY, newCoordX, newCoordY);

	int newX = playerX + newCoordX;
	int newY = playerY + newCoordY;

	if (newX == m_DestX && newY == m_DestY)
	{
		if (m_Explode)
		{
			TGameObject *obj = World->FindWorldObject(m_Serial);

			if (obj != NULL && obj->GetTopObject() != NULL)
			{
				m_Z = m_DestZ;
				EffectManager->CreateExplodeEffect(this);
			}
		}

		EffectManager->RemoveEffect(this);
	}
	else
	{
		int newDrawX = screenCenterX + (newCoordX - newCoordY) * 22;
		int newDrawY = screenCenterY + (newCoordX + newCoordY) * 22;

		m_OffsetX = realDrawX - newDrawX;
		m_OffsetY = realDrawY - newDrawY;

		bool wantUpdateInRenderList = false;

		int countX = drawDestX - (newDrawX + m_OffsetX);
		int countY = drawDestY - (newDrawY + m_OffsetY);
		//int countY = drawDestY - (newDrawY + m_OffsetY + m_OffsetZ) - (m_DestZ - m_Z) * 4;

		if (m_Z != m_DestZ)
		{
			int stepsCountX = countX / (tempXY[x] + 1);
			int stepsCountY = countY / (tempXY[(x + 1) % 2] + 1);

			if (stepsCountX < stepsCountY)
				stepsCountX = stepsCountY;

			if (stepsCountX <= 0)
				stepsCountX = 1;

			int totalOffsetZ = 0;

			bool incZ = m_Z < m_DestZ;

			if (incZ)
				totalOffsetZ = (m_DestZ - m_Z) * 4;
			else
				totalOffsetZ = (m_Z - m_DestZ) * 4;

			totalOffsetZ /= stepsCountX;

			if (!totalOffsetZ)
				totalOffsetZ = 1;

			m_OffsetZ += totalOffsetZ;

			if (m_OffsetZ >= 4)
			{
				int countZ = m_OffsetZ / 4;

				if (incZ)
					m_Z += countZ;
				else
					m_Z -= countZ;

				if (m_Z == m_DestZ)
					m_OffsetZ = 0;
				else
					m_OffsetZ -= countZ * 4;

				wantUpdateInRenderList = true;
			}
		}

		countY -= m_OffsetZ + (m_DestZ - m_Z) * 4;

		m_Angle = 180.0f + (float)(atan2(countY, countX) * 57.295780); //180.0f / M_PI = 57.295780f

		if (m_X != newX || m_Y != newY)
		{
			m_X = newX;
			m_Y = newY;

			wantUpdateInRenderList = true;
		}

		if (wantUpdateInRenderList)
			MapManager->AddRender(this);
	}
}
//---------------------------------------------------------------------------