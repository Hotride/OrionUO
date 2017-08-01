/***********************************************************************************
**
** Walker.cpp
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#include "Walker.h"
#include "../Game objects/GamePlayer.h"
#include "../Network/Packets.h"
//----------------------------------------------------------------------------------
CWalker g_Walker;
//----------------------------------------------------------------------------------
CWalker::CWalker()
{
	memset(m_Direction, 0, sizeof(m_Direction));
}
//---------------------------------------------------------------------------
void CWalker::IncSequence()
{
	if (m_Sequence == 255)
		m_Sequence = 1;
	else
		m_Sequence++;
}
//---------------------------------------------------------------------------
void CWalker::SetSequence(const uchar &seq, const uchar &dir)
{
	m_Sequence = seq;
	m_Direction[seq] = dir;
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
}
//----------------------------------------------------------------------------------
void CWalker::DenyWalk(const uchar &sequence, const int &x, const int &y, const char &z)
{
	g_WalkRequestCount = 0;
	g_PendingDelayTime = 0;

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

	if (stepIndex == m_StepsCount)
	{
		if (!m_ResendPacketSended)
		{
			CPacketResend().Send();
			m_ResendPacketSended = true;
		}

		m_WalkingFailed = true;
		m_StepsCount = 0;
		m_CurrentWalkSequence = 0;

		return;
	}

	if (m_Step[stepIndex].Direction2 == m_Step[stepIndex].Direction)
	{
		const int offsetX[8] = { 0, 1, 1, 1, 0, -1, -1, -1 };
		const int offsetY[8] = { -1, -1, 0, 1, 1, 1, 0, -1 };

		int dir = m_Step[stepIndex].Direction2;
		g_Player->UpdatePlayerCoordinates(offsetX[dir], offsetY[dir], m_Step[stepIndex].Z);
	}

	if (stepIndex >= m_CurrentWalkSequence)
		m_Step[stepIndex].Accepted = true;
	else
	{
		if (stepIndex)
		{
			if (!m_ResendPacketSended)
			{
				CPacketResend().Send();
				m_ResendPacketSended = true;
			}

			m_WalkingFailed = true;
			m_StepsCount = 0;
			m_CurrentWalkSequence = 0;

			return;
		}

		IFOR(i, 1, m_StepsCount)
		{
			m_Step[i - 1] = m_Step[i];
		}

		m_StepsCount--;
		m_CurrentWalkSequence--;
	}
}
//----------------------------------------------------------------------------------
