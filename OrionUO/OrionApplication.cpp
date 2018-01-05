// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
/***********************************************************************************
**
** OrionApplication.cpp
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#include "stdafx.h"
//----------------------------------------------------------------------------------
COrionApplication g_App;
//----------------------------------------------------------------------------------
void COrionApplication::OnMainLoop()
{
	WISPFUN_DEBUG("c193_f1");

	g_Ticks = timeGetTime();

	if (m_NextRenderTime <= g_Ticks)
	{
		//m_NextUpdateTime = g_Ticks + 50;
		m_NextRenderTime = g_Ticks + g_OrionWindow.RenderTimerDelay;

		g_Orion.Process(true);
	}
	/*else if (m_NextUpdateTime <= g_Ticks)
	{
		m_NextUpdateTime = g_Ticks + 50;

		g_Orion.Process(false);
	}*/
	else
		Sleep(1);
}
//----------------------------------------------------------------------------------
