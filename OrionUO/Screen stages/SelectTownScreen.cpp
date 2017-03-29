/***********************************************************************************
**
** SelectTownScreen.cpp
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#include "SelectTownScreen.h"
#include "CreateCharacterScreen.h"
#include "../OrionUO.h"
#include "../OrionWindow.h"
#include "../Network/Packets.h"
#include "../Managers/PacketManager.h"
//----------------------------------------------------------------------------------
CSelectTownScreen g_SelectTownScreen;
//----------------------------------------------------------------------------------
CSelectTownScreen::CSelectTownScreen()
: CBaseScreen(m_SelectTownGump), m_City(NULL)
{
}
//----------------------------------------------------------------------------------
CSelectTownScreen::~CSelectTownScreen()
{
}
//----------------------------------------------------------------------------------
/*!
Инициализация
@return 
*/
void CSelectTownScreen::Init()
{
	WISPFUN_DEBUG("c167_f1");
	if (g_PacketManager.ClientVersion >= CV_70130)
		m_City = g_CityList.GetCity(0);
	else
		m_City = g_CityList.GetCity(3);

	g_ScreenEffectManager.UseSunrise();
	m_SmoothScreenAction = 0;
	
	m_Gump.PrepareTextures();
	m_Gump.WantUpdateContent = true;
}
//----------------------------------------------------------------------------------
/*!
Обработка события после плавного затемнения экрана
@param [__in_opt] action Идентификатор действия
@return 
*/
void CSelectTownScreen::ProcessSmoothAction(uchar action)
{
	WISPFUN_DEBUG("c167_f2");
	if (action == 0xFF)
		action = m_SmoothScreenAction;

	if (action == ID_SMOOTH_STS_QUIT)
		g_OrionWindow.Destroy();
	else if (action == ID_SMOOTH_STS_GO_SCREEN_CHARACTER)
		g_Orion.InitScreen(GS_CHARACTER);
	else if (action == ID_SMOOTH_STS_GO_SCREEN_GAME_CONNECT)
	{
		CPacketCreateCharacter(g_CreateCharacterScreen.Name).Send();
		g_Orion.InitScreen(GS_GAME_CONNECT);
	}
}
//----------------------------------------------------------------------------------
