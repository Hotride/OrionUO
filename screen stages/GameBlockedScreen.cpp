/****************************************************************************
**
** GameBlockedGump.cpp
**
** Copyright (C) September 2015 Hotride
**
** This program is free software; you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation; either version 2 of the License, or
** (at your option) any later version.
**
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**
*****************************************************************************
*/
//---------------------------------------------------------------------------
#include "stdafx.h"

TGameBlockedScreen *GameBlockedScreen = NULL;
//---------------------------------------------------------------------------
TGameBlockedScreen::TGameBlockedScreen()
: TBaseScreen(), m_Code(0)
{
}
//---------------------------------------------------------------------------
TGameBlockedScreen::~TGameBlockedScreen()
{
}
//---------------------------------------------------------------------------
/*!
Инициализация
@return 
*/
void TGameBlockedScreen::Init()
{
	m_Code = 0;
}
//---------------------------------------------------------------------------
/*!
Отрисовка/выбор объектов
@param [__in] mode true - отрисовка, false - выбор
@return При выборе объектов - идентификатор выбранного объекта
*/
int TGameBlockedScreen::Render( __in bool mode)
{
	if (mode)
	{
		GumpManager->Draw(mode, true);
		
		InitPopupHelp();

		MouseManager.Draw(0x2073); //Main Gump mouse cursor
	}
	else
	{
		g_LastSelectedObject = 0;
		g_LastObjectType = SOT_NO_OBJECT;
		g_LastSelectedGump = 0;
		
		GumpManager->Draw(mode, true);

		return g_LastSelectedObject;
	}

	return 0;
}
//---------------------------------------------------------------------------
/*!
Нажатие левой кнопки мыши
@return 
*/
void TGameBlockedScreen::OnLeftMouseDown()
{
	g_LastSelectedGump = 0;

	Render(false);

	if (g_LastSelectedGump)
		GumpManager->OnLeftMouseDown(true);
}
//---------------------------------------------------------------------------
/*!
Отпускание левой кнопки мыши
@return 
*/
void TGameBlockedScreen::OnLeftMouseUp()
{
	Render(false);

	if (g_LeftMouseDown && g_LastGumpLeftMouseDown)
		GumpManager->OnLeftMouseUp(true);
}
//---------------------------------------------------------------------------
/*!
Обработка нажатия клавиши
@param [__in] wparam не подписанный параметр
@param [__in] lparam не подписанный параметр
@return 
*/
void TGameBlockedScreen::OnCharPress( __in WPARAM wparam, __in LPARAM lparam)
{
	if (EntryPointer == NULL || EntryPointer == GameConsole)
		return;

	GumpManager->OnCharPress(wparam, lparam, true);
}
//---------------------------------------------------------------------------
/*!
Обработка нажатия клавиши
@param [__in] wparam не подписанный параметр
@param [__in] lparam не подписанный параметр
@return 
*/
void TGameBlockedScreen::OnKeyPress( __in WPARAM wparam, __in LPARAM lparam)
{
	TGumpNotify *notify = (TGumpNotify*)GumpManager->GetGump(0,0, GT_NOTIFY);

	if (EntryPointer == NULL || EntryPointer == GameConsole)
	{
		if (wparam == VK_RETURN && notify != NULL)
			notify->OnKeyPress(wparam, lparam);
	}
	else
	{
		TGump *gump = GumpManager->GetTextEntryOwner();

		if (gump != NULL && gump->GumpType == GT_TEXT_ENTRY_DIALOG)
			gump->OnKeyPress(wparam, lparam);
		else if (notify != NULL)
			notify->OnKeyPress(wparam, lparam);
	}
}
//---------------------------------------------------------------------------