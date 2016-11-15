/***********************************************************************************
**
** CharacterListScreen.cpp
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#include "CharacterListScreen.h"
#include "MainScreen.h"
#include "ConnectionScreen.h"
#include "../ServerList.h"
#include "../CharacterList.h"
#include "../OrionUO.h"
#include "../OrionWindow.h"
#include "../Managers/ConfigManager.h"
#include "../Managers/ConnectionManager.h"
//----------------------------------------------------------------------------------
CCharacterListScreen g_CharacterListScreen;
//----------------------------------------------------------------------------------
CCharacterListScreen::CCharacterListScreen()
: CBaseScreen(m_CharacterListGump)
{
}
//----------------------------------------------------------------------------------
CCharacterListScreen::~CCharacterListScreen()
{
}
//----------------------------------------------------------------------------------
/*!
Инициализация
@return 
*/
void CCharacterListScreen::Init()
{
	g_OrionWindow.SetTitle(string("Ultima Online - ") + g_MainScreen.m_Account->c_str() + "(" + g_ServerList.GetSelectedServer()->Name + ")");

	g_CharacterList.Selected = 0;

	g_ConfigManager.UpdateRange = MAX_VIEW_RANGE;

	g_ScreenEffectManager.UseSunrise();
	m_SmoothScreenAction = 0;

	m_Gump.PrepareTextures();
	m_Gump.WantUpdateContent = true;
}
//----------------------------------------------------------------------------------
/*!
Обработка события после перехода
@param [__in_opt] action Идентификатор действия
@return 
*/
void CCharacterListScreen::ProcessSmoothAction(uchar action)
{
	if (action == 0xFF)
		action = m_SmoothScreenAction;

	if (action == ID_SMOOTH_CLS_QUIT)
		g_OrionWindow.Destroy();
	else if (action == ID_SMOOTH_CLS_CONNECT)
		g_Orion.Connect();
	else if (action == ID_SMOOTH_CLS_SELECT_CHARACTER)
	{
		g_Orion.CharacterSelection(g_CharacterList.Selected);

		if (!g_CharacterList.GetName(g_CharacterList.Selected).length())
			g_ConnectionScreen.ErrorCode = 2;
	}
	else if (action == ID_SMOOTH_CLS_GO_SCREEN_PROFESSION_SELECT)
		g_Orion.InitScreen(GS_PROFESSION_SELECT);
	else if (action == ID_SMOOTH_CLS_GO_SCREEN_DELETE)
	{
		if (g_CharacterList.GetSelectedName().length())
		{
			g_Orion.InitScreen(GS_DELETE);
			g_ConnectionScreen.Type = CST_CHARACTER_LIST;
		}
	}
}
//----------------------------------------------------------------------------------
