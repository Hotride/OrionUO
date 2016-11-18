/***********************************************************************************
**
** OrionWindow.cpp
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#include "OrionWindow.h"
#include "OrionUO.h"
#include "Wisp/WispApplication.h"
#include "Wisp/WispLogger.h"
#include "GLEngine/GLEngine.h"
#include "Globals.h"
#include <tchar.h>
#include "Resource.h"
#include "Screen stages/BaseScreen.h"
#include "SelectedObject.h"
#include "PressedObject.h"
#include "Constants.h"
#include "Managers/ConfigManager.h"
#include "Game objects/ObjectOnCursor.h"
#include "ClickObject.h"
#include <iostream>
#include "Managers/PluginManager.h"
#include "Managers/PacketManager.h"
#include "Managers/ConnectionManager.h"
//----------------------------------------------------------------------------------
COrionWindow g_OrionWindow;
//----------------------------------------------------------------------------------
int APIENTRY _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
{
	INITLOGGER("uolog.txt");

	if (!g_OrionWindow.Create(hInstance, L"Orion UO Client", L"Ultima Online", true, 640, 480, LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ORIONUO)), LoadCursor(hInstance, MAKEINTRESOURCE(IDC_CURSOR1))))
			return 0;

	g_OrionWindow.ShowWindow(true);
	g_OrionWindow.NoResize = true;

	g_Orion.LoadPluginConfig();

	return g_App.Run(hInstance);
}
//----------------------------------------------------------------------------------
COrionWindow::COrionWindow()
: WISP_WINDOW::CWindow()
{
}
//----------------------------------------------------------------------------------
COrionWindow::~COrionWindow()
{
}
//----------------------------------------------------------------------------------
void COrionWindow::SetRenderTimerDelay(const int &delay)
{
	WISP_THREADED_TIMER::CThreadedTimer *timer = GetThreadedTimer(RENDER_TIMER_ID);

	if (timer != NULL)
		timer->ChangeDelay(delay);
}
//----------------------------------------------------------------------------------
bool COrionWindow::OnCreate()
{
	if (!g_GL.Install())
	{
		LOG("Error install OpenGL\n");
		MessageBoxA(m_Handle, "Error install OpenGL", "Error install OpenGL!", MB_OK);
		return false;
	}

	if (!g_Orion.Install())
		return false;

	g_GL.UpdateRect();

	CreateThreadedTimer(RENDER_TIMER_ID, FRAME_DELAY_ACTIVE_WINDOW, false, true, true);
	//CreateThreadedTimer(UPDATE_TIMER_ID, 10);
	CreateTimer(UPDATE_TIMER_ID, 10);

	return true;
}
//----------------------------------------------------------------------------------
void COrionWindow::OnDestroy()
{
	g_PluginManager.WindowProc(m_Handle, WM_CLOSE, 0, 0);

	g_Orion.Uninstall();
}
//----------------------------------------------------------------------------------
void COrionWindow::OnResize(WISP_GEOMETRY::CSize &newSize)
{
	g_GL.UpdateRect();
}
//----------------------------------------------------------------------------------
void COrionWindow::EmulateOnLeftMouseButtonDown()
{
	if (g_CurrentScreen != NULL && g_ScreenEffectManager.Mode == SEM_NONE)
	{
		g_CurrentScreen->Render(false);

		g_PressedObject.InitLeft(g_SelectedObject);

		if (g_SelectedObject.Object() != NULL || g_GameState == GS_GAME)
			g_CurrentScreen->OnLeftMouseButtonDown();
	}
}
//----------------------------------------------------------------------------------
void COrionWindow::OnLeftMouseButtonDown()
{
	if (g_CurrentScreen != NULL && g_ScreenEffectManager.Mode == SEM_NONE)
	{
		g_GeneratedMouseDown = false;

		g_CurrentScreen->Render(false);

		g_PressedObject.InitLeft(g_SelectedObject);

		if (g_SelectedObject.Object() != NULL || g_GameState == GS_GAME)
			g_CurrentScreen->OnLeftMouseButtonDown();
	}
}
//----------------------------------------------------------------------------------
void COrionWindow::OnLeftMouseButtonUp()
{
	if (g_CurrentScreen != NULL && g_ScreenEffectManager.Mode == SEM_NONE)
	{
		g_CurrentScreen->Render(false);

		//if ((g_SelectedObject.Object() != NULL && g_SelectedObject.Object() == g_PressedObject.LeftObject() && g_SelectedObject.Serial) || g_GameState >= GS_GAME)
		if ((g_SelectedObject.Object() != NULL && g_SelectedObject.Serial) || g_GameState >= GS_GAME)
		{
			g_CurrentScreen->OnLeftMouseButtonUp();

			if (g_MovingFromMouse && g_PressedObject.LeftGump() == NULL)
				g_AutoMoving = true;
		}

		if (g_PressedObject.LeftObject() != NULL && g_PressedObject.LeftObject()->IsGUI() && ((CBaseGUI*)g_PressedObject.LeftObject())->Type == GOT_COMBOBOX && g_PressedObject.LeftGump() != NULL)
			g_PressedObject.LeftGump()->WantRedraw = true;

		g_PressedObject.ClearLeft();
	}
}
//----------------------------------------------------------------------------------
bool COrionWindow::OnLeftMouseButtonDoubleClick()
{
	bool result = false;

	if (g_CurrentScreen != NULL && g_ScreenEffectManager.Mode == SEM_NONE)
	{
		g_CurrentScreen->Render(false);

		g_PressedObject.InitLeft(g_SelectedObject);

		result = (g_SelectedObject.Object() != NULL && g_SelectedObject.Object() == g_PressedObject.LeftObject()) && g_CurrentScreen->OnLeftMouseButtonDoubleClick();

		if (result)
		{
			g_PressedObject.ClearLeft();
			g_ClickObject.Clear();
		}
	}

	return result;
}
//----------------------------------------------------------------------------------
void COrionWindow::OnRightMouseButtonDown()
{
	if (g_CurrentScreen != NULL && g_ScreenEffectManager.Mode == SEM_NONE)
	{
		g_CurrentScreen->Render(false);

		g_PressedObject.InitRight(g_SelectedObject);

		g_CurrentScreen->OnRightMouseButtonDown();

		if (g_SelectedObject.Gump() == NULL &&
			!(g_MouseManager.Position.X < g_ConfigManager.GameWindowX || g_MouseManager.Position.Y < g_ConfigManager.GameWindowY ||
			g_MouseManager.Position.X >(g_ConfigManager.GameWindowX + g_ConfigManager.GameWindowWidth) ||
			g_MouseManager.Position.Y >(g_ConfigManager.GameWindowY + g_ConfigManager.GameWindowHeight)))
		{
			g_MovingFromMouse = true;
			g_AutoMoving = false;
		}
	}
}
//----------------------------------------------------------------------------------
void COrionWindow::OnRightMouseButtonUp()
{
	if (g_CurrentScreen != NULL && g_ScreenEffectManager.Mode == SEM_NONE)
	{
		g_CurrentScreen->Render(false);

		if ((g_SelectedObject.Object() != NULL && g_SelectedObject.Object() == g_PressedObject.RightObject() && g_SelectedObject.Serial) || g_GameState >= GS_GAME)
			g_CurrentScreen->OnRightMouseButtonUp();

		g_MovingFromMouse = false;
		g_PressedObject.ClearRight();
	}
}
//----------------------------------------------------------------------------------
bool COrionWindow::OnRightMouseButtonDoubleClick()
{
	bool result = false;

	if (g_CurrentScreen != NULL && g_ScreenEffectManager.Mode == SEM_NONE)
	{
		g_CurrentScreen->Render(false);

		g_PressedObject.InitRight(g_SelectedObject);

		result = (g_SelectedObject.Object() != NULL && g_SelectedObject.Object() == g_PressedObject.RightObject()) && g_CurrentScreen->OnRightMouseButtonDoubleClick();

		if (result)
			g_PressedObject.ClearRight();
	}

	return result;
}
//----------------------------------------------------------------------------------
void COrionWindow::OnMidMouseButtonDown()
{
	if (g_PluginManager.WindowProc(m_Handle, WM_MBUTTONDOWN, 0, 0))
		return;

	if (g_CurrentScreen != NULL && g_ScreenEffectManager.Mode == SEM_NONE)
		g_CurrentScreen->OnMidMouseButtonDown();
}
//----------------------------------------------------------------------------------
void COrionWindow::OnMidMouseButtonUp()
{
	if (g_PluginManager.WindowProc(m_Handle, WM_MBUTTONUP, 0, 0))
		return;

	if (g_CurrentScreen != NULL && g_ScreenEffectManager.Mode == SEM_NONE)
		g_CurrentScreen->OnMidMouseButtonUp();
}
//----------------------------------------------------------------------------------
bool COrionWindow::OnMidMouseButtonDoubleClick()
{
	if (g_CurrentScreen != NULL && g_ScreenEffectManager.Mode == SEM_NONE)
		return g_CurrentScreen->OnMidMouseButtonDoubleClick();

	return false;
}
//----------------------------------------------------------------------------------
void COrionWindow::OnMidMouseButtonScroll(const bool &up)
{
	if (g_PluginManager.WindowProc(m_Handle, WM_MOUSEWHEEL, (up ? 0 : 0x11110000), 0))
		return;

	if (g_CurrentScreen != NULL && g_ScreenEffectManager.Mode == SEM_NONE)
	{
		g_CurrentScreen->Render(false);

		g_CurrentScreen->OnMidMouseButtonScroll(up);
	}
}
//----------------------------------------------------------------------------------
void COrionWindow::OnDragging()
{
	if (g_CurrentScreen != NULL && g_ScreenEffectManager.Mode == SEM_NONE)
		g_CurrentScreen->OnDragging();
}
//----------------------------------------------------------------------------------
void COrionWindow::OnActivate()
{
	SetRenderTimerDelay(g_FrameDelay[1]);

	if (!g_PluginManager.Empty())
		g_PluginManager.WindowProc(m_Handle, WM_NCACTIVATE, 1, 0);
}
//----------------------------------------------------------------------------------
void COrionWindow::OnDeactivate()
{
	if (g_ConfigManager.ReduceFPSUnactiveWindow)
		SetRenderTimerDelay(g_FrameDelay[0]);

	if (!g_PluginManager.Empty())
		g_PluginManager.WindowProc(m_Handle, WM_NCACTIVATE, 0, 0);
}
//----------------------------------------------------------------------------------
void COrionWindow::OnCharPress(const WPARAM &wParam, const LPARAM &lParam)
{
	if (g_PluginManager.WindowProc(m_Handle, WM_CHAR, wParam, lParam))
		return;

	if ((iswprint(wParam) || (g_GameState >= GS_GAME && (wParam == 0x11 || wParam == 0x17))) && g_CurrentScreen != NULL && g_ScreenEffectManager.Mode == SEM_NONE)
		g_CurrentScreen->OnCharPress(wParam, lParam);
}
//----------------------------------------------------------------------------------
void COrionWindow::OnKeyDown(const WPARAM &wParam, const LPARAM &lParam)
{
	if (g_PluginManager.WindowProc(m_Handle, WM_KEYDOWN, wParam, lParam))
		return;

	if (/*!iswprint(wParam) &&*/ g_CurrentScreen != NULL && g_ScreenEffectManager.Mode == SEM_NONE)
		g_CurrentScreen->OnKeyDown(wParam, lParam);
}
//----------------------------------------------------------------------------------
void COrionWindow::OnKeyUp(const WPARAM &wParam, const LPARAM &lParam)
{
	if (g_PluginManager.WindowProc(m_Handle, WM_KEYUP, wParam, lParam))
		return;

	if (/*!iswprint(wParam) &&*/ g_CurrentScreen != NULL && g_ScreenEffectManager.Mode == SEM_NONE)
		g_CurrentScreen->OnKeyUp(wParam, lParam);
}
//----------------------------------------------------------------------------------
HRESULT COrionWindow::OnRepaint(const WPARAM &wParam, const LPARAM &lParam)
{
	if (!g_PluginManager.Empty())
		return g_PluginManager.WindowProc(m_Handle, WM_NCPAINT, wParam, lParam);

	return DefWindowProc(m_Handle, WM_NCPAINT, wParam, lParam);
}
//----------------------------------------------------------------------------------
void COrionWindow::OnTimer(uint id)
{
	if (id == UPDATE_TIMER_ID)
	{
		g_Ticks = timeGetTime();
		g_Orion.Process(false);
	}
}
//----------------------------------------------------------------------------------
void COrionWindow::OnThreadedTimer(uint nowTime, WISP_THREADED_TIMER::CThreadedTimer *timer)
{
	g_Ticks = nowTime;

	if (timer->TimerID == RENDER_TIMER_ID)
	{
		g_Orion.Process(true);
	}
	else if (timer->TimerID == UPDATE_TIMER_ID)
	{
		g_Orion.Process(false);
	}
}
//----------------------------------------------------------------------------------
LRESULT COrionWindow::OnUserMessages(const UINT &message, const WPARAM &wParam, const LPARAM &lParam)
{
	switch (message)
	{
		case UOMSG_RECV:
			g_PacketManager.SavePluginReceivePacket((PBYTE)wParam, lParam);
			//g_PacketManager.PluginReceiveHandler((PBYTE)wParam, lParam);
			return S_OK;
		case UOMSG_SEND:
			g_ConnectionManager.Send((PBYTE)wParam, lParam);
			return S_OK;
		default:
			break;
	}

	g_PluginManager.WindowProc(m_Handle, message, wParam, lParam);

	return S_OK;
}
//----------------------------------------------------------------------------------
