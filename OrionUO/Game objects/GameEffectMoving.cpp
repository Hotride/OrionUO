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
	if (m_LastMoveTime > g_Ticks)
		return;

	m_LastMoveTime = g_Ticks + m_MoveDelay;

	CGameEffect::Update(parent);

	CGameObject *obj = g_World->FindWorldObject(m_DestSerial);

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

	int screenCenterX = g_RenderBounds.GameWindowCenterX;
	int screenCenterY = g_RenderBounds.GameWindowCenterY;

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

	if (newX == m_DestX && newY == m_DestY && m_Z == m_DestZ)
	{
		if (m_Explode)
		{
			EFFECT_TYPE type = EF_STAY_AT_POS;

			if (g_World->FindWorldObject(m_Serial) != NULL)
				type = EF_STAY_AT_SOURCE;

			m_Z = m_DestZ;
			g_EffectManager.CreateExplodeEffect(this, type);
		}

		g_EffectManager.RemoveEffect(this);
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
				int countZ = 1; // m_OffsetZ / 4;

				if (incZ)
					m_Z += countZ;
				else
					m_Z -= countZ;

				if (m_Z == m_DestZ)
					m_OffsetZ = 0;
				else
					m_OffsetZ %= 8;
					//m_OffsetZ -= countZ * 4;

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
		{
			UpdateDrawCoordinates();

			g_MapManager->AddRender(this);
		}
	}
}
//----------------------------------------------------------------------------------
