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
	UnacceptedPacketsCount = 0;
	StepsCount = 0;
	WalkSequence = 0;
	CurrentWalkSequence = 0;
	WalkingFailed = false;
	ResendPacketSended = false;
	LastStepRequestTime = 0;
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
		g_Player->SetX(x);
		g_Player->SetY(y);
		g_Player->SetZ(z);

		g_RemoveRangeXY.X = x;
		g_RemoveRangeXY.Y = y;

		UOI_PLAYER_XYZ_DATA xyzData = { g_RemoveRangeXY.X, g_RemoveRangeXY.Y, 0 };
		g_PluginManager.WindowProc(g_OrionWindow.Handle, UOMSG_UPDATE_REMOVE_POS, (WPARAM)&xyzData, 0);
	}
}
//----------------------------------------------------------------------------------
void CWalker::ConfirmWalk(const uchar &sequence)
{
	if (UnacceptedPacketsCount)
		UnacceptedPacketsCount--;

	int stepIndex = 0;

	IFOR(i, 0, StepsCount)
	{
		if (m_Step[i].Sequence == sequence)
			break;

		stepIndex++;
	}

	bool isBadStep = (stepIndex == StepsCount);

	if (!isBadStep)
	{
		if (stepIndex >= CurrentWalkSequence)
		{
			m_Step[stepIndex].Accepted = true;
			g_RemoveRangeXY.X = m_Step[stepIndex].X;
			g_RemoveRangeXY.Y = m_Step[stepIndex].Y;
		}
		else if (!stepIndex)
		{
			g_RemoveRangeXY.X = m_Step[0].X;
			g_RemoveRangeXY.Y = m_Step[0].Y;

			IFOR(i, 1, StepsCount)
			{
				m_Step[i - 1] = m_Step[i];
			}

			StepsCount--;
			CurrentWalkSequence--;
		}
		else //if (stepIndex)
			isBadStep = true;
	}

	if (isBadStep)
	{
		if (!ResendPacketSended)
		{
			CPacketResend().Send();
			ResendPacketSended = true;
		}

		WalkingFailed = true;
		StepsCount = 0;
		CurrentWalkSequence = 0;
	}
	else
	{
		UOI_PLAYER_XYZ_DATA xyzData = { g_RemoveRangeXY.X, g_RemoveRangeXY.Y, 0 };
		g_PluginManager.WindowProc(g_OrionWindow.Handle, UOMSG_UPDATE_REMOVE_POS, (WPARAM)&xyzData, 0);
	}
}
//----------------------------------------------------------------------------------
