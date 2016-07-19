/****************************************************************************
**
** DebugScreen.cpp
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

TDebugScreen *DebugScreen = NULL;
//---------------------------------------------------------------------------
TDebugScreen::TDebugScreen()
: TBaseScreen()
{
	//Скамейка
	m_Chair = new TGameItem(1);
	m_Chair->Graphic = 2909;
	m_Chair->Color = 0x0386;

	//Персонаж
	m_Player = new TGameCharacter(2);
	m_Player->Graphic = 0x0190;
	m_Player->Color = 0x0386;
	m_Player->Direction = 2;
	m_Player->Hits = 1;
	m_Player->MaxHits = 1;

	//Линковка в псевдомире: стул; персонаж
	m_Chair->m_NextXY = m_Player;
	m_Player->m_PrevXY = m_Chair;

	//Маунт
	m_Mount = new TGameItem(3);
	m_Mount->Graphic = 0x3EA3;
	m_Mount->Color = 0x0386;
	m_Mount->Count = 1;
	m_Mount->Layer = OL_MOUNT;
	m_Player->Add(m_Mount);

	//Слои: волосы, роба, клока
	TGameItem *obj = new TGameItem(4);
	obj->Graphic = 0x203C;
	obj->OnGraphicChange();
	obj->Color = 0x0A01;
	obj->Count = 1;
	obj->Layer = OL_HAIR;
	m_Player->Add(obj);

	obj = new TGameItem(5);
	obj->Graphic = 0x1F03;
	obj->OnGraphicChange();
	obj->Color = 0x0BB6;
	obj->Count = 1;
	obj->Layer = OL_ROBE;
	m_Player->Add(obj);

	m_Cloak = new TGameItem(6);
	m_Cloak->Graphic = 0x1515;
	m_Cloak->OnGraphicChange();
	m_Cloak->Color = 0x0A04;
	m_Cloak->Count = 1;
	m_Cloak->Layer = OL_CLOAK;
	m_Player->Add(m_Cloak);
}
//---------------------------------------------------------------------------
TDebugScreen::~TDebugScreen()
{
}
//---------------------------------------------------------------------------
void TDebugScreen::Init()
{
	g_ConfigLoaded = false;

	SetWindowTextA(g_hWnd, "Ultima Online");

	RECT r = { 0, 0, 0, 0 };
	r.right = 640;
	r.bottom = 480;
	AdjustWindowRectEx(&r, GetWindowLongA(g_hWnd, GWL_STYLE), FALSE, GetWindowLongA(g_hWnd, GWL_EXSTYLE));

	if (r.left < 0)
		r.right += (r.left * (-1));

	if (r.top < 0)
		r.bottom += (r.top * (-1));

	SetWindowPos(g_hWnd, HWND_TOP, 0, 0, r.right, r.bottom, 0);

	QuestArrow.Enabled = false;

	g_TotalSendSize = 0;
	g_TotalRecvSize = 0;

	g_LightLevel = 0;
	g_PersonalLightLevel = 0;

	FontManager->SetSavePixels(false);

	SmoothMonitor.UseSunrise();
	m_SmoothScreenAction = 0;

	ToolTip.SeqIndex = 0;

	//Prepare textures on Main Screen:
	Orion->ExecuteGump(0x0588); //Main Screen background
	Orion->ExecuteGump(0x157C); //Main Screen
	Orion->ExecuteGump(0x15A0); //Main Screen Notes
	Orion->ExecuteResizepic(0x13BE); //ActPwd Container
	Orion->ExecuteGump(0x058A); //Main Screen Castle?
	Orion->ExecuteGumpPart(0x1589, 3); //X gump
	Orion->ExecuteGumpPart(0x15A4, 3); //> gump
	Orion->ExecuteResizepic(0x0BB8); //Account/Password text field
	Orion->ExecuteGumpPart(0x00D2, 2); //Checkbox on / off
}
//---------------------------------------------------------------------------
void TDebugScreen::ProcessSmoothAction(BYTE action)
{
	if (action == 0xFF)
		action = m_SmoothScreenAction;

	if (action == ID_SMOOTH_DS_GO_SCREEN_MAIN)
		Orion->InitScreen(GS_MAIN);
	else if (action == ID_SMOOTH_DS_QUIT)
		PostMessage(g_hWnd, WM_CLOSE, 0, 0);
}
//---------------------------------------------------------------------------
void TDebugScreen::InitToolTip()
{
}
//---------------------------------------------------------------------------
int TDebugScreen::Render(bool mode)
{
	DWORD ticks = g_Ticks;

	/*if (g_LastRenderTime > ticks)
	{
		if (mode || !g_SelectGumpObjects)
			return 0;
	}*/

	if (mode)
	{
		int CanSelectedButton = g_LastSelectedObject;

		int CanPressedButton = 0;
		if (g_LeftMouseDown && g_LastObjectLeftMouseDown == g_LastSelectedObject)
			CanPressedButton = g_LastObjectLeftMouseDown;

		g_LastRenderTime = ticks + (g_FrameDelay[(int)(GetForegroundWindow() == g_hWnd)]);

		g_GL.BeginDraw();

		if (DrawSmoothMonitor())
			return 0;

		TFrameBuffer fb;
		fb.Init(640, 480);

		if (fb.Ready() && fb.Use())
		{
			ColorizerShader->Use();

			WORD GumpID = 0x15A4 + (int)(CanSelectedButton == ID_DS_GO_SCREEN_MAIN); //> gump / lighted
			if (CanPressedButton == ID_DS_GO_SCREEN_MAIN)
				GumpID = 0x15A6; //> gump pressed
			Orion->DrawGump(GumpID, 0, 610, 445);

			UnuseShader();

			glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
			DPOLY(315, 235, 10, 10);

			glColor4f(0.37f, 0.0f, 0.0f, 1.0f);
			g_GL.DrawLine(320, 240, g_MouseX, g_MouseY);
			glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

			int cx = 320;
			int cy = 240;

			int px = g_MouseX;
			int py = g_MouseY;

			int ox = (px - cx) / 22;
			int oy = (py - cy) / 22;

			int dx = 0;
			int dy = 0;

			TileOffsetOnMonitorToXY(ox, oy, dx, dy);

			char str[100] = { 0 };
			sprintf(str, "cur = %i %i", 10 + dx, 10 + dy);

			FontManager->DrawA(3, str, 0x0386, 20, 20);

			DrawSmoothMonitorEffect();

			fb.Release();

			g_GL.RestorePort();

			g_GL.ViewPort(0, 0, 640, 480);

			fb.Draw(0, 0);

			ColorizerShader->Use();
			const WORD mountTable[4] = { 0x3EA2, 0x3EA6, 0x3EA3, 0x3EA4 }; //horse, llama, ostard, zostrich

			//Для рисования 1 ряда размаунтим маунта			
			IFOR(j, 0, 3)
			{
				IFOR(i, 0, 4)
				{
					m_Player->Direction = i + 1;
					m_Chair->Graphic = 2906 + i;
					int x = 100 + (i * 100);
					int y = 100 + (j * 100);

					//Рисуем стул только в верхнем ряду, в нижнем - перс на маунте
					if (j < 2)
						m_Chair->Draw(mode, x, y, ticks);			
					else
						m_Mount->Graphic = mountTable[i];

					if (j == 0 && i == 0)
						m_Mount->Layer = OL_NONE;
					else if (j == 1 && i == 0)
					{
						m_Cloak->Layer = OL_NONE;	

					}					
					else if (j == 2 && i == 0)
					{
						m_Cloak->Layer = OL_CLOAK;
						m_Mount->Layer = OL_MOUNT;
					}
						

					AnimationManager->DrawCharacter(m_Player, x, y, 0);						
				}
			}

			UnuseShader();

			InitToolTip();

			MouseManager.Draw(0x2073); //Main Gump mouse cursor
		}

		fb.Free();

		g_GL.EndDraw();
	}
	else
	{
		g_LastSelectedObject = 0;

		if (Orion->GumpPixelsInXY(0x15A4, 610, 445))
			g_LastSelectedObject = ID_DS_GO_SCREEN_MAIN; //> gump

		return g_LastSelectedObject;
	}

	return 0;
}
//---------------------------------------------------------------------------
void TDebugScreen::OnLeftMouseUp()
{
	Render(false);

	g_LeftMouseDown = false;
	g_DroppedLeftMouseX = 0;
	g_DroppedLeftMouseY = 0;

	if (g_LastSelectedObject == 0 || g_LastSelectedObject != g_LastObjectLeftMouseDown)
	{
		g_LastObjectLeftMouseDown = 0;
		return;
	}

	if (g_LastObjectLeftMouseDown == ID_DS_GO_SCREEN_MAIN) //> button
		CreateSmoothAction(ID_SMOOTH_DS_GO_SCREEN_MAIN);

	g_LastObjectLeftMouseDown = 0;
}
//---------------------------------------------------------------------------
void TDebugScreen::OnCharPress(__in WPARAM wparam, __in LPARAM lparam)
{
	if (wparam == VK_RETURN || wparam == VK_BACK || wparam == VK_ESCAPE)
		return; //Ignore no print keys

	wchar_t wstr[2] = { 0 };
	wstr[0] = wparam;
	wstring str(wstr);

	if (!str.length())
		return;
}
//---------------------------------------------------------------------------
void TDebugScreen::OnKeyPress(WPARAM wparam, LPARAM lparam)
{
	if (wparam == VK_RETURN)
		CreateSmoothAction(ID_SMOOTH_DS_GO_SCREEN_MAIN);
}
//---------------------------------------------------------------------------