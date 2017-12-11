// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
/***********************************************************************************
**
** Walker.cpp
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#include "stdafx.h"
//----------------------------------------------------------------------------------
CWalker g_Walker;
//----------------------------------------------------------------------------------
CWalker::CWalker()
{
}
//---------------------------------------------------------------------------
void CWalker::Reset()
{
	m_UnacceptedPacketsCount = 0;
	m_StepsCount = 0;
	m_WalkSequence = 0;
	m_CurrentWalkSequence = 0;
	m_WalkingFailed = false;
	m_ResendPacketSended = false;
	m_LastStepRequestTime = 0;
}
//----------------------------------------------------------------------------------
void CWalker::DenyWalk(const uchar &sequence, const int &x, const int &y, const char &z)
{
	g_Player->m_Steps.clear();

	g_Player->OffsetX = 0;
	g_Player->OffsetY = 0;
	g_Player->OffsetZ = 0;

	Reset();

	/*if (sequence == 0xFF)
	{
	g_Player->GameObject.GameObject.X = g_PlayerX;
	g_Player->GameObject.GameObject.Y = g_PlayerY;
	(*((void(**)(void))g_Player->GameObject.GameObject.VTable + UO_ROFUN_UPDATE_RENDER_AND_DRAW_POS))();
	RenderScene_1();
	(*((void(**)(void))g_Player->GameObject.GameObject.VTable + UO_ROFUN_39))();
	result = UpdateMaxDrawZ();
	}
	else{...}*/
	//UpdatePlayerCoordinates(x, y, z, g_ServerID);

	if (x != -1)
	{
		g_Player->X = x;
		g_Player->Y = y;
		g_Player->Z = z;

		g_RemoveRangeXY.X = x;
		g_RemoveRangeXY.Y = y;
	}
}
//----------------------------------------------------------------------------------
void CWalker::ConfirmWalk(const uchar &sequence)
{
	if (m_UnacceptedPacketsCount)
		m_UnacceptedPacketsCount--;

	int stepIndex = 0;

	IFOR(i, 0, m_StepsCount)
	{
		if (m_Step[i].Sequence == sequence)
			break;

		stepIndex++;
	}

	bool isBadStep = (stepIndex == m_StepsCount);

	if (!isBadStep)
	{
		if (stepIndex >= m_CurrentWalkSequence)
		{
			m_Step[stepIndex].Accepted = true;
			g_RemoveRangeXY.X = m_Step[stepIndex].X;
			g_RemoveRangeXY.Y = m_Step[stepIndex].Y;
		}
		else if (!stepIndex)
		{
			g_RemoveRangeXY.X = m_Step[0].X;
			g_RemoveRangeXY.Y = m_Step[0].Y;

			IFOR(i, 1, m_StepsCount)
			{
				m_Step[i - 1] = m_Step[i];
			}

			m_StepsCount--;
			m_CurrentWalkSequence--;
		}
		else //if (stepIndex)
			isBadStep = true;
	}

	if (isBadStep)
	{
		if (!m_ResendPacketSended)
		{
			CPacketResend().Send();
			m_ResendPacketSended = true;
		}

		m_WalkingFailed = true;
		m_StepsCount = 0;
		m_CurrentWalkSequence = 0;
	}
}
//----------------------------------------------------------------------------------
