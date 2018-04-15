// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
/***********************************************************************************
**
** ConnectionScreen.cpp
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#include "stdafx.h"
//----------------------------------------------------------------------------------
CConnectionScreen g_ConnectionScreen;
//----------------------------------------------------------------------------------
CConnectionScreen::CConnectionScreen()
: CBaseScreen(m_ConnectionGump)
{
}
//----------------------------------------------------------------------------------
CConnectionScreen::~CConnectionScreen()
{
}
//----------------------------------------------------------------------------------
/*!
Инициализация
@return 
*/
void CConnectionScreen::Init()
{
	WISPFUN_DEBUG("c161_f1");
	m_Text = "";
	m_ConnectionFailed = false;
	m_Connected = false;
	m_Completed= false;
	m_ErrorCode = 0;
	m_Type = CST_LOGIN;

	g_ScreenEffectManager.UseSunrise();
	SmoothScreenAction = 0;

	m_Gump.PrepareTextures();
	m_Gump.WantUpdateContent = true;
}
//----------------------------------------------------------------------------------
void CConnectionScreen::SetConnectionFailed(const bool &val)
{
	m_ConnectionFailed = val;
	m_Gump.WantUpdateContent = true;
}
//----------------------------------------------------------------------------------
void CConnectionScreen::SetConnected(const bool &val)
{
	m_Connected = val;
	m_Gump.WantUpdateContent = true;
}
//----------------------------------------------------------------------------------
void CConnectionScreen::SetCompleted(const bool &val)
{
	m_Completed = val;
	m_Gump.WantUpdateContent = true;
}
//----------------------------------------------------------------------------------
void CConnectionScreen::SetErrorCode(const int &val)
{
	m_ErrorCode = val;
	m_Gump.WantUpdateContent = true;
}
//----------------------------------------------------------------------------------
void CConnectionScreen::SetType(const CONNECTION_SCREEN_TYPE &val)
{
	m_Type = val;
	m_Gump.WantUpdateContent = true;
}
//----------------------------------------------------------------------------------
void CConnectionScreen::SetText(const string &val)
{
	m_Text = val;
	m_Gump.WantUpdateContent = true;
}
//----------------------------------------------------------------------------------
/*!
Обработка события после плавного затемнения экрана
@param [__in_opt] action Идентификатор действия
@return 
*/
void CConnectionScreen::ProcessSmoothAction(uchar action)
{
	WISPFUN_DEBUG("c161_f2");
	if (action == 0xFF)
		action = SmoothScreenAction;

	if (action == ID_SMOOTH_CS_GO_SCREEN_MAIN)
		g_Orion.InitScreen(GS_MAIN);
	else if (action == ID_SMOOTH_CS_GO_SCREEN_CHARACTER)
		g_Orion.InitScreen(GS_CHARACTER);
	else if (action == ID_SMOOTH_CS_GO_SCREEN_PROFESSION)
		g_Orion.InitScreen(GS_PROFESSION_SELECT);
	else if (action == ID_SMOOTH_CS_SEND_DELETE)
		CPacketDeleteCharacter(g_CharacterList.Selected).Send();
}
//----------------------------------------------------------------------------------
/*!
Обработка нажатия клавиши
@param [__in] wparam не подписанный параметр
@param [__in] lparam не подписанный параметр
@return 
*/
void CConnectionScreen::OnKeyDown(const WPARAM &wParam, const LPARAM &lParam)
{
	WISPFUN_DEBUG("c161_f3");
	switch (wParam)
	{
		case VK_RETURN:
		{
			if (m_Type == CST_CHARACTER_LIST)
				CreateSmoothAction(ID_SMOOTH_CS_SEND_DELETE);
			else if (m_Type == CST_SELECT_PROFESSOIN)
				CreateSmoothAction(ID_SMOOTH_CS_GO_SCREEN_PROFESSION);
			else if (m_Type == CST_GAME)
			{
				if (m_ErrorCode > 0)
					CreateSmoothAction(ID_SMOOTH_CS_GO_SCREEN_CHARACTER);
			}
			else if (m_Type == CST_CONLOST || m_ConnectionFailed)
				CreateSmoothAction(ID_SMOOTH_CS_GO_SCREEN_MAIN);

			break;
		}
		case VK_ESCAPE:
		{
			if (m_Type == CST_CHARACTER_LIST)
				CreateSmoothAction(ID_SMOOTH_CS_GO_SCREEN_CHARACTER);
			else if (m_Type == CST_SELECT_PROFESSOIN)
				CreateSmoothAction(ID_SMOOTH_CS_GO_SCREEN_PROFESSION);
			else if (m_Type == CST_GAME)
			{
				if (m_ErrorCode > 0)
					CreateSmoothAction(ID_SMOOTH_CS_GO_SCREEN_CHARACTER);
			}
			else if (m_Type == CST_CONLOST || m_ConnectionFailed)
				CreateSmoothAction(ID_SMOOTH_CS_GO_SCREEN_MAIN);

			break;
		}
		default:
			break;
	}
}
//----------------------------------------------------------------------------------
