/***********************************************************************************
**
** GumpScreenGame.cpp
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#include "GumpScreenGame.h"
#include "../Screen stages/GameScreen.h"
#include "../OrionUO.h"
#include "../Managers/ConfigManager.h"
#include "../OrionWindow.h"
#include "../Managers/PacketManager.h"
#include "../Network/Packets.h"
#include "../Managers/GumpManager.h"
#include "GumpConsoleType.h"
#include "GumpOptions.h"
//----------------------------------------------------------------------------------
CGumpScreenGame::CGumpScreenGame()
: CGump(GT_NONE, 0, 0, 0)
{
	m_NoMove = true;
	m_NoClose = true;

	Add(new CGUIButton(ID_GS_RESIZE, 0x0837, 0x0837, 0x0838, 0, 0));
	Add(new CGUIGumppic(0x0E14, 0, 0));
}
//----------------------------------------------------------------------------------
CGumpScreenGame::~CGumpScreenGame()
{
}
//----------------------------------------------------------------------------------
void CGumpScreenGame::UpdateContent()
{
	if (g_PressedObject.LeftGump() == this)
	{
		WISP_GEOMETRY::CPoint2Di offset = g_MouseManager.LeftDroppedOffset();

		if (g_PressedObject.LeftObject() == m_Items) //resizer
		{
			g_RenderBounds.GameWindowWidth += offset.X;
			g_RenderBounds.GameWindowHeight += offset.Y;

			if (g_RenderBounds.GameWindowWidth < 640)
				g_RenderBounds.GameWindowWidth = 640;

			if (g_RenderBounds.GameWindowWidth >= GetSystemMetrics(SM_CXSCREEN) - 20)
				g_RenderBounds.GameWindowWidth = GetSystemMetrics(SM_CXSCREEN) - 20;

			if (g_RenderBounds.GameWindowHeight < 480)
				g_RenderBounds.GameWindowHeight = 480;

			if (g_RenderBounds.GameWindowHeight >= GetSystemMetrics(SM_CYSCREEN) - 60)
				g_RenderBounds.GameWindowHeight = GetSystemMetrics(SM_CYSCREEN) - 60;


			CGumpOptions *opt = (CGumpOptions*)g_GumpManager.UpdateGump(g_PlayerSerial, 0, GT_OPTIONS);

			if (opt != NULL)
			{
				opt->m_GameWindowWidth->m_Entry.SetText(std::to_wstring(g_RenderBounds.GameWindowWidth));
				opt->m_GameWindowHeight->m_Entry.SetText(std::to_wstring(g_RenderBounds.GameWindowHeight));
			}
		}
		else //scope
		{
			g_RenderBounds.GameWindowPosX += offset.X;
			g_RenderBounds.GameWindowPosY += offset.Y;

			if (g_RenderBounds.GameWindowPosX < 0)
				g_RenderBounds.GameWindowPosX = 0;

			if (g_RenderBounds.GameWindowPosY < 0)
				g_RenderBounds.GameWindowPosY = 0;

			if (g_RenderBounds.GameWindowPosX + g_RenderBounds.GameWindowWidth > g_OrionWindow.Size.Width)
				g_RenderBounds.GameWindowPosX = g_OrionWindow.Size.Width - g_RenderBounds.GameWindowWidth;

			if (g_RenderBounds.GameWindowPosY + g_RenderBounds.GameWindowHeight > g_OrionWindow.Size.Height)
				g_RenderBounds.GameWindowPosY = g_OrionWindow.Size.Height - g_RenderBounds.GameWindowHeight;
		}
	}
}
//----------------------------------------------------------------------------------
void CGumpScreenGame::InitToolTip()
{
	if (!g_ConfigManager.UseToolTips || g_SelectedObject.Object() == NULL)
		return;

	if (g_SelectedObject.Serial == ID_GS_RESIZE)
		g_ToolTip.Set(L"Resize game window", 100);
}
//----------------------------------------------------------------------------------
void CGumpScreenGame::Draw()
{
	//Рамка игрового окна
	g_Orion.DrawGump(0x0A8D, 0, g_RenderBounds.GameWindowPosX - 4, g_RenderBounds.GameWindowPosY - 4, 0, g_RenderBounds.GameWindowHeight + 8);
	g_Orion.DrawGump(0x0A8D, 0, g_RenderBounds.GameWindowPosX + g_RenderBounds.GameWindowWidth, g_RenderBounds.GameWindowPosY - 4, 0, g_RenderBounds.GameWindowHeight + 8);

	g_Orion.DrawGump(0x0A8C, 0, g_RenderBounds.GameWindowPosX - 4, g_RenderBounds.GameWindowPosY - 4, g_RenderBounds.GameWindowWidth + 4, 0);
	g_Orion.DrawGump(0x0A8C, 0, g_RenderBounds.GameWindowPosX - 4, g_RenderBounds.GameWindowPosY + g_RenderBounds.GameWindowHeight, g_RenderBounds.GameWindowWidth + 8, 0);

	ushort resizeGumpID = 0x0837; //button
	if (g_ConfigManager.LockResizingGameWindow)
		resizeGumpID = 0x082C; //lock
	else if (g_SelectedObject.Object() == m_Items)
		resizeGumpID++; //lighted button

	g_Orion.DrawGump(resizeGumpID, 0, g_RenderBounds.GameWindowPosX + g_RenderBounds.GameWindowWidth - 3, g_RenderBounds.GameWindowPosY + g_RenderBounds.GameWindowHeight - 3);
}
//----------------------------------------------------------------------------------
CRenderObject *CGumpScreenGame::Select()
{
	CRenderObject *selected = NULL;

	//Если ничего не выбралось - пройдемся по рамке
	if (g_Orion.GumpPixelsInXY(0x0A8D, g_RenderBounds.GameWindowPosX - 4, g_RenderBounds.GameWindowPosY - 4, 0, g_RenderBounds.GameWindowHeight + 8))
		selected = (CRenderObject*)m_Items->m_Next;
	else if (g_Orion.GumpPixelsInXY(0x0A8D, g_RenderBounds.GameWindowPosX + g_RenderBounds.GameWindowWidth, g_RenderBounds.GameWindowPosY - 4, 0, g_RenderBounds.GameWindowHeight + 8))
		selected = (CRenderObject*)m_Items->m_Next;
	else if (g_Orion.GumpPixelsInXY(0x0A8C, g_RenderBounds.GameWindowPosX - 4, g_RenderBounds.GameWindowPosY - 4, g_RenderBounds.GameWindowWidth + 8, 0))
		selected = (CRenderObject*)m_Items->m_Next;
	else if (g_Orion.GumpPixelsInXY(0x0A8C, g_RenderBounds.GameWindowPosX - 4, g_RenderBounds.GameWindowPosY + g_RenderBounds.GameWindowHeight, g_RenderBounds.GameWindowWidth + 8, 0))
		selected = (CRenderObject*)m_Items->m_Next;

	if (!g_ConfigManager.LockResizingGameWindow)
	{
		if (g_Orion.GumpPixelsInXY(0x0837, g_RenderBounds.GameWindowPosX + g_RenderBounds.GameWindowWidth - 3, g_RenderBounds.GameWindowPosY + g_RenderBounds.GameWindowHeight - 3))
			selected = (CRenderObject*)m_Items;
	}

	if (selected != NULL)
		g_SelectedObject.Init(selected, this);

	return selected;
}
//----------------------------------------------------------------------------------
void CGumpScreenGame::OnLeftMouseButtonDown()
{
	//CGump::OnLeftMouseButtonDown();

	if (g_GumpConsoleType != NULL)
		g_GumpManager.MoveToBack(g_GumpConsoleType);
}
//----------------------------------------------------------------------------------
void CGumpScreenGame::OnLeftMouseButtonUp()
{
	WISP_GEOMETRY::CPoint2Di offset = g_MouseManager.LeftDroppedOffset();

	if (g_PressedObject.LeftObject() == m_Items) //resizer
	{
		g_ConfigManager.GameWindowWidth = g_ConfigManager.GameWindowWidth + offset.X;
		g_ConfigManager.GameWindowHeight = g_ConfigManager.GameWindowHeight + offset.Y;

		if (g_ConfigManager.GameWindowWidth < 640)
			g_ConfigManager.GameWindowWidth = 640;

		if (g_ConfigManager.GameWindowWidth >= GetSystemMetrics(SM_CXSCREEN) - 20)
			g_ConfigManager.GameWindowWidth = GetSystemMetrics(SM_CXSCREEN) - 20;

		if (g_ConfigManager.GameWindowHeight < 480)
			g_ConfigManager.GameWindowHeight = 480;

		if (g_ConfigManager.GameWindowHeight >= GetSystemMetrics(SM_CYSCREEN) - 60)
			g_ConfigManager.GameWindowHeight = GetSystemMetrics(SM_CYSCREEN) - 60;

		if (g_PacketManager.ClientVersion >= CV_200)
			CPacketGameWindowSize().Send();
	}
	else //scope
	{
		g_ConfigManager.GameWindowX = g_ConfigManager.GameWindowX + offset.X;
		g_ConfigManager.GameWindowY = g_ConfigManager.GameWindowY + offset.Y;

		if (g_ConfigManager.GameWindowX < 1)
			g_ConfigManager.GameWindowX = 0;

		if (g_ConfigManager.GameWindowY < 1)
			g_ConfigManager.GameWindowY = 0;

		if (g_ConfigManager.GameWindowX + g_ConfigManager.GameWindowWidth > g_OrionWindow.Size.Width)
			g_ConfigManager.GameWindowX = g_OrionWindow.Size.Width - g_ConfigManager.GameWindowWidth;

		if (g_ConfigManager.GameWindowY + g_ConfigManager.GameWindowHeight > g_OrionWindow.Size.Height)
			g_ConfigManager.GameWindowY = g_OrionWindow.Size.Height - g_ConfigManager.GameWindowHeight;
	}
}
//----------------------------------------------------------------------------------
