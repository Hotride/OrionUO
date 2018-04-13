// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
/***********************************************************************************
**
** GameEffectMoving.h
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#include "stdafx.h"
//----------------------------------------------------------------------------------
CGameEffectMoving::CGameEffectMoving()
: CGameEffectDrag()
{
}
//----------------------------------------------------------------------------------
CGameEffectMoving::~CGameEffectMoving()
{
}
//----------------------------------------------------------------------------------
/*!
Обновить эффект
@return
*/
void CGameEffectMoving::Update(CGameObject *parent)
{
	WISPFUN_DEBUG("c18_f1");
	if (LastMoveTime > g_Ticks)
		return;

	LastMoveTime = g_Ticks + MoveDelay;

	CGameEffect::Update(parent);

	CGameObject *obj = g_World->FindWorldObject(DestSerial);

	if (obj != NULL)
	{
		obj = obj->GetTopObject();

		if (obj != NULL)
		{
			DestX = obj->X;
			DestY = obj->Y;
			DestZ = obj->Z;
		}
	}

	int screenCenterX = g_RenderBounds.GameWindowCenterX;
	int screenCenterY = g_RenderBounds.GameWindowCenterY;

	int playerX = g_Player->X;
	int playerY = g_Player->Y;

	int offsetX = m_X - playerX;
	int offsetY = m_Y - playerY;

	int drawX = screenCenterX + (offsetX - offsetY) * 22;
	int drawY = screenCenterY + (offsetX + offsetY) * 22;

	int realDrawX = drawX + OffsetX;
	int realDrawY = drawY + OffsetY;

	int offsetDestX = DestX - playerX;
	int offsetDestY = DestY - playerY;

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
	static const int effectSpeed = 5;
	int tempXY[2] = { effectSpeed/*m_Speed*/, 0 };

	for (int j = 0; j < effectSpeed/*m_Speed*/; j++)
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

	if (newX == DestX && newY == DestY && m_Z == DestZ)
	{
		if (Explode)
		{
			EFFECT_TYPE type = EF_STAY_AT_POS;

			if (g_World->FindWorldObject(Serial) != NULL)
				type = EF_STAY_AT_SOURCE;

			m_Z = DestZ;
			g_EffectManager.CreateExplodeEffect(this, type);
		}

		g_EffectManager.RemoveEffect(this);
	}
	else
	{
		int newDrawX = screenCenterX + (newCoordX - newCoordY) * 22;
		int newDrawY = screenCenterY + (newCoordX + newCoordY) * 22;

		Changed = true;
		OffsetX = realDrawX - newDrawX;
		OffsetY = realDrawY - newDrawY;

		bool wantUpdateInRenderList = false;

		int countX = drawDestX - (newDrawX + OffsetX);
		int countY = drawDestY - (newDrawY + OffsetY);
		//int countY = drawDestY - (newDrawY + m_OffsetY + m_OffsetZ) - (m_DestZ - m_Z) * 4;

		if (m_Z != DestZ)
		{
			int stepsCountX = countX / (tempXY[x] + 1);
			int stepsCountY = countY / (tempXY[(x + 1) % 2] + 1);

			if (stepsCountX < stepsCountY)
				stepsCountX = stepsCountY;

			if (stepsCountX <= 0)
				stepsCountX = 1;

			int totalOffsetZ = 0;

			bool incZ = m_Z < DestZ;

			if (incZ)
				totalOffsetZ = (DestZ - m_Z) * 4;
			else
				totalOffsetZ = (m_Z - DestZ) * 4;

			totalOffsetZ /= stepsCountX;

			if (!totalOffsetZ)
				totalOffsetZ = 1;

			OffsetZ += totalOffsetZ;

			if (OffsetZ >= 4)
			{
				int countZ = 1; // m_OffsetZ / 4;

				if (incZ)
					m_Z += countZ;
				else
					m_Z -= countZ;

				if (m_Z == DestZ)
					OffsetZ = 0;
				else
					OffsetZ %= 8;
					//m_OffsetZ -= countZ * 4;

				wantUpdateInRenderList = true;
			}
		}

		countY -= OffsetZ + (DestZ - m_Z) * 4;

		Angle = 180.0f + (float)(atan2(countY, countX) * 57.295780); //180.0f / M_PI = 57.295780f

		if (m_X != newX || m_Y != newY)
		{
			m_X = newX;
			m_Y = newY;

			wantUpdateInRenderList = true;
		}

		if (wantUpdateInRenderList)
		{
			UpdateRealDrawCoordinates();

			g_MapManager.AddRender(this);
		}
	}
}
//----------------------------------------------------------------------------------
