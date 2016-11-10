/***********************************************************************************
**
** ServerScreen.cpp
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#include "ServerScreen.h"
#include "MainScreen.h"
#include "../OrionWindow.h"
#include "../Managers/ClilocManager.h"
#include "../Managers/ConfigManager.h"
#include "../ServerList.h"
#include "../OrionUO.h"
//----------------------------------------------------------------------------------
CServerScreen g_ServerScreen;
//----------------------------------------------------------------------------------
CServerScreen::CServerScreen()
: CBaseScreen(m_ServerGump), m_SelectionServerTempValue(0)
{
}
//----------------------------------------------------------------------------------
CServerScreen::~CServerScreen()
{
}
//----------------------------------------------------------------------------------
/*!
Инициализация
@return 
*/
void CServerScreen::Init()
{
	g_OrionWindow.SetTitle(string("Ultima Online - ") + g_MainScreen.m_Account->c_str());

	g_ScreenEffectManager.UseSunrise();
	m_SmoothScreenAction = 0;

	g_ConfigManager.UpdateRange = MAX_VIEW_RANGE;

	m_Gump.PrepareTextures();
	m_Gump.WantUpdateContent = true;
}
//----------------------------------------------------------------------------------
/*!
Обработка нажатия клавиши
@param [__in] wparam не подписанный параметр
@param [__in] lparam не подписанный параметр
@return 
*/
void CServerScreen::OnKeyDown(const WPARAM &wParam, const LPARAM &lParam)
{
	m_Gump.OnKeyDown(wParam, lParam);

	if (wParam == VK_RETURN)
	{
		m_SelectionServerTempValue = 0;
		CreateSmoothAction(ID_SMOOTH_SS_SELECT_SERVER);
	}
}
//----------------------------------------------------------------------------------
/*!
Обработка события после плавного затемнения экрана
@param [__in_opt] action Идентификатор действия
@return 
*/
void CServerScreen::ProcessSmoothAction(uchar action)
{
	if (action == 0xFF)
		action = m_SmoothScreenAction;

	if (action == ID_SMOOTH_SS_SELECT_SERVER)
		g_Orion.ServerSelection(m_SelectionServerTempValue);
	else if (action == ID_SMOOTH_SS_QUIT)
		g_OrionWindow.Destroy();
	else if (action == ID_SMOOTH_SS_GO_SCREEN_MAIN)
	{
		g_Orion.Disconnect();
		g_Orion.InitScreen(GS_MAIN);
	}
}
//----------------------------------------------------------------------------------
