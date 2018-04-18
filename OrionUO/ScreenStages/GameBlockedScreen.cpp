// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
/***********************************************************************************
**
** GameBlockedScreen.cpp
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#include "stdafx.h"
//----------------------------------------------------------------------------------
CGameBlockedScreen g_GameBlockedScreen;
//----------------------------------------------------------------------------------
CGameBlockedScreen::CGameBlockedScreen()
: CBaseScreen(m_GameBlockedScreenGump)
{
}
//----------------------------------------------------------------------------------
CGameBlockedScreen::~CGameBlockedScreen()
{
}
//----------------------------------------------------------------------------------
/*!
Инициализация
@return 
*/
void CGameBlockedScreen::Init()
{
	Code = 0;
}
//----------------------------------------------------------------------------------
/*!
Отрисовка/выбор объектов
@param [__in] mode true - отрисовка, false - выбор
@return При выборе объектов - идентификатор выбранного объекта
*/
void CGameBlockedScreen::Render(bool mode)
{
	WISPFUN_DEBUG("c163_f1");
	if (mode)
	{
		g_GumpManager.Draw(true);
		
		InitToolTip();

		g_MouseManager.Draw(0x2073); //Main Gump mouse cursor
	}
	else
	{
		g_SelectedObject.Clear();

		g_GumpManager.Select(true);

		if (g_SelectedObject.Object != g_LastSelectedObject.Object)
		{
			if (g_SelectedObject.Object != NULL)
				g_SelectedObject.Object->OnMouseEnter();

			if (g_LastSelectedObject.Object != NULL)
				g_LastSelectedObject.Object->OnMouseExit();
		}

		g_LastSelectedObject.Init(g_SelectedObject);
	}
}
//----------------------------------------------------------------------------------
/*!
Нажатие левой кнопки мыши
@return 
*/
void CGameBlockedScreen::OnLeftMouseButtonDown()
{
	WISPFUN_DEBUG("c163_f2");
	if (g_SelectedObject.Gump != NULL)
		g_GumpManager.OnLeftMouseButtonDown(true);
}
//----------------------------------------------------------------------------------
/*!
Отпускание левой кнопки мыши
@return 
*/
void CGameBlockedScreen::OnLeftMouseButtonUp()
{
	WISPFUN_DEBUG("c163_f3");
	if (g_PressedObject.LeftGump != NULL)
		g_GumpManager.OnLeftMouseButtonUp(true);
}
//----------------------------------------------------------------------------------
/*!
Обработка нажатия клавиши
@param [__in] wparam не подписанный параметр
@param [__in] lparam не подписанный параметр
@return 
*/
void CGameBlockedScreen::OnCharPress(const WPARAM &wParam, const LPARAM &lParam)
{
	WISPFUN_DEBUG("c163_f4");
	if (g_EntryPointer == NULL || g_EntryPointer == &g_GameConsole)
		return;

	g_GumpManager.OnCharPress(wParam, lParam, true);
}
//----------------------------------------------------------------------------------
/*!
Обработка нажатия клавиши
@param [__in] wparam не подписанный параметр
@param [__in] lparam не подписанный параметр
@return 
*/
void CGameBlockedScreen::OnKeyDown(const WPARAM &wParam, const LPARAM &lParam)
{
	WISPFUN_DEBUG("c163_f5");
	CGumpNotify *notify = (CGumpNotify*)g_GumpManager.GetGump(0,0, GT_NOTIFY);

	if (g_EntryPointer == NULL || g_EntryPointer == &g_GameConsole)
	{
		if (wParam == VK_RETURN && notify != NULL)
			notify->OnKeyDown(wParam, lParam);
	}
	else
	{
		CGump *gump = g_GumpManager.GetTextEntryOwner();

		if (gump != NULL && gump->GumpType == GT_TEXT_ENTRY_DIALOG)
			gump->OnKeyDown(wParam, lParam);
		else if (notify != NULL)
			notify->OnKeyDown(wParam, lParam);
	}
}
//----------------------------------------------------------------------------------
