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
	g_Orion.Uninstall();
}
//----------------------------------------------------------------------------------
void COrionWindow::OnResize(WISP_GEOMETRY::CSize &newSize)
{
	g_GL.UpdateRect();
}
//----------------------------------------------------------------------------------
void COrionWindow::OnLeftMouseButtonDown()
{
	if (g_CurrentScreen != NULL && g_SmoothMonitor.Type == SMT_NONE)
	{
		g_GeneratedMouseDown = false;

		g_CurrentScreen->Render(false);

		g_PressedObject.InitLeft(g_SelectedObject);

		if (g_SelectedObject.Object() != NULL)
			g_CurrentScreen->OnLeftMouseButtonDown();
	}
}
//----------------------------------------------------------------------------------
void COrionWindow::OnLeftMouseButtonUp()
{
	if (g_CurrentScreen != NULL && g_SmoothMonitor.Type == SMT_NONE)
	{
		g_CurrentScreen->Render(false);

		//if ((g_SelectedObject.Object() != NULL && g_SelectedObject.Object() == g_PressedObject.LeftObject() && g_SelectedObject.Serial) || g_GameState >= GS_GAME)
		if ((g_SelectedObject.Object() != NULL && g_SelectedObject.Serial) || g_GameState >= GS_GAME)
			g_CurrentScreen->OnLeftMouseButtonUp();

		if (g_PressedObject.LeftObject() != NULL && g_PressedObject.LeftObject()->IsGUI() && ((CBaseGUI*)g_PressedObject.LeftObject())->Type == GOT_COMBOBOX && g_PressedObject.LeftGump() != NULL)
			g_PressedObject.LeftGump()->WantRedraw = true;

		g_PressedObject.ClearLeft();
	}
}
//----------------------------------------------------------------------------------
bool COrionWindow::OnLeftMouseButtonDoubleClick()
{
	bool result = false;

	if (g_CurrentScreen != NULL && g_SmoothMonitor.Type == SMT_NONE)
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
	if (g_CurrentScreen != NULL && g_SmoothMonitor.Type == SMT_NONE)
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
	if (g_CurrentScreen != NULL && g_SmoothMonitor.Type == SMT_NONE)
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

	if (g_CurrentScreen != NULL && g_SmoothMonitor.Type == SMT_NONE)
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
	if (g_CurrentScreen != NULL && g_SmoothMonitor.Type == SMT_NONE)
		g_CurrentScreen->OnMidMouseButtonDown();
}
//----------------------------------------------------------------------------------
void COrionWindow::OnMidMouseButtonUp()
{
	if (g_CurrentScreen != NULL && g_SmoothMonitor.Type == SMT_NONE)
		g_CurrentScreen->OnMidMouseButtonUp();
}
//----------------------------------------------------------------------------------
bool COrionWindow::OnMidMouseButtonDoubleClick()
{
	if (g_CurrentScreen != NULL && g_SmoothMonitor.Type == SMT_NONE)
		return g_CurrentScreen->OnMidMouseButtonDoubleClick();

	return false;
}
//----------------------------------------------------------------------------------
void COrionWindow::OnMidMouseButtonScroll(const bool &up)
{
	if (g_CurrentScreen != NULL && g_SmoothMonitor.Type == SMT_NONE)
	{
		g_CurrentScreen->Render(false);

		g_CurrentScreen->OnMidMouseButtonScroll(up);
	}
}
//----------------------------------------------------------------------------------
void COrionWindow::OnDragging()
{
	if (g_CurrentScreen != NULL && g_SmoothMonitor.Type == SMT_NONE)
		g_CurrentScreen->OnDragging();
}
//----------------------------------------------------------------------------------
void COrionWindow::OnActivate()
{
}
//----------------------------------------------------------------------------------
void COrionWindow::OnDeactivate()
{
}
//----------------------------------------------------------------------------------
void COrionWindow::OnCharPress(const WPARAM &wParam, const LPARAM &lParam)
{
	if ((iswprint(wParam) || (g_GameState >= GS_GAME && wParam == 0x11)) && g_CurrentScreen != NULL && g_SmoothMonitor.Type == SMT_NONE)
		g_CurrentScreen->OnCharPress(wParam, lParam);
}
//----------------------------------------------------------------------------------
void COrionWindow::OnKeyDown(const WPARAM &wParam, const LPARAM &lParam)
{
	if (/*!iswprint(wParam) &&*/ g_CurrentScreen != NULL && g_SmoothMonitor.Type == SMT_NONE)
		g_CurrentScreen->OnKeyDown(wParam, lParam);
}
//----------------------------------------------------------------------------------
void COrionWindow::OnKeyUp(const WPARAM &wParam, const LPARAM &lParam)
{
	if (/*!iswprint(wParam) &&*/ g_CurrentScreen != NULL && g_SmoothMonitor.Type == SMT_NONE)
		g_CurrentScreen->OnKeyUp(wParam, lParam);
}
//----------------------------------------------------------------------------------
void COrionWindow::OnRepaint()
{
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
	return S_OK;
}
//----------------------------------------------------------------------------------
