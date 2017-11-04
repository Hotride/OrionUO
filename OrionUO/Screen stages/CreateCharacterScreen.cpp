// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
/***********************************************************************************
**
** CreateCharacterScreen.cpp
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#include "stdafx.h"
//----------------------------------------------------------------------------------
CCreateCharacterScreen g_CreateCharacterScreen;
//----------------------------------------------------------------------------------
CCreateCharacterScreen::CCreateCharacterScreen()
: CBaseScreen(m_CreateCharacterGump)
{
}
//----------------------------------------------------------------------------------
CCreateCharacterScreen::~CCreateCharacterScreen()
{
}
//----------------------------------------------------------------------------------
void CCreateCharacterScreen::OnChangeStyleSelection(const int &val)
{
	m_Gump.WantUpdateContent = true;
}
//----------------------------------------------------------------------------------
void CCreateCharacterScreen::OnChangeColorSelection(const int &val)
{
	m_Gump.WantUpdateContent = true;
}
//----------------------------------------------------------------------------------
/*!
Инициализация
@return 
*/
void CCreateCharacterScreen::Init()
{
	WISPFUN_DEBUG("c162_f1");
	g_CreateCharacterManager.Clear();

	m_Name = "";
	m_StyleSelection = 0;
	m_ColorSelection = 0;

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
void CCreateCharacterScreen::ProcessSmoothAction(uchar action)
{
	WISPFUN_DEBUG("c162_f2");
	if (action == 0xFF)
		action = m_SmoothScreenAction;

	if (action == ID_SMOOTH_CCS_QUIT)
		g_OrionWindow.Destroy();
	else if (action == ID_SMOOTH_CCS_GO_SCREEN_CHARACTER)
		g_Orion.InitScreen(GS_CHARACTER);
	else if (action == ID_SMOOTH_CCS_GO_SCREEN_CONNECT)
	{
		g_Orion.InitScreen(GS_GAME_CONNECT);
		g_ConnectionScreen.Type = CST_GAME;
		g_ConnectionScreen.ConnectionFailed = true;
		g_ConnectionScreen.ErrorCode = 1;
	}
	else if (action == ID_SMOOTH_CCS_GO_SCREEN_SELECT_TOWN)
		g_Orion.InitScreen(GS_SELECT_TOWN);
}
//----------------------------------------------------------------------------------
/*!
Нажатие левой кнопки мыши
@return 
*/
void CCreateCharacterScreen::OnLeftMouseButtonDown()
{
	WISPFUN_DEBUG("c162_f3");
	CBaseScreen::OnLeftMouseButtonDown();

	if (g_SelectedObject.Serial == 0)
	{
		if (m_StyleSelection != 0)
		{
			m_StyleSelection = 0;
			m_Gump.WantUpdateContent = true;
		}
	}
}
//----------------------------------------------------------------------------------
/*!
Обработка нажатия клавиши
@param [__in] wparam не подписанный параметр
@param [__in] lparam не подписанный параметр
@return 
*/
void CCreateCharacterScreen::OnCharPress(const WPARAM &wParam, const LPARAM &lParam)
{
	WISPFUN_DEBUG("c162_f4");
	if (wParam >= 0x0100 || !g_FontManager.IsPrintASCII(wParam))
		return;
	else if (g_EntryPointer == NULL)
		return;

	if (g_EntryPointer->Length() < 20) //add char to text field
		g_EntryPointer->Insert(wParam);

	m_Name = g_EntryPointer->c_str();
	m_Gump.WantRedraw = true;
}
//----------------------------------------------------------------------------------
/*!
Обработка нажатия клавиши
@param [__in] wparam не подписанный параметр
@param [__in] lparam не подписанный параметр
@return 
*/
void CCreateCharacterScreen::OnKeyDown(const WPARAM &wParam, const LPARAM &lParam)
{
	WISPFUN_DEBUG("c162_f5");
	if (g_EntryPointer != NULL)
	{
		g_EntryPointer->OnKey(&m_Gump, wParam);

		m_Name = g_EntryPointer->c_str();
		m_Gump.WantRedraw = true;
	}
}
//----------------------------------------------------------------------------------
