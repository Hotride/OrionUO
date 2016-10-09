/***********************************************************************************
**
** DebugScreen.cpp
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#include "DebugScreen.h"
#include "../SelectedObject.h"
#include "../PressedObject.h"
#include "../OrionUO.h"
#include "../OrionWindow.h"
#include "../Managers/FontsManager.h"
#include "../ToolTip.h"
//----------------------------------------------------------------------------------
CDebugScreen g_DebugScreen;
//----------------------------------------------------------------------------------
CDebugScreen::CDebugScreen()
: CBaseScreen(CGump(GT_NONE, 0, 0, 0))
{
}
//----------------------------------------------------------------------------------
CDebugScreen::~CDebugScreen()
{
}
//----------------------------------------------------------------------------------
void CDebugScreen::Init()
{
	//Скамейка
	/*m_Chair = new TGameItem(1);
	m_Chair->Graphic = 2909;
	m_Chair->OnGraphicChange();
	m_Chair->Color = 0x0386;

	//Персонаж
	m_Player = new TGameCharacter(2);
	m_Player->Graphic = 0x0190;
	m_Chair->OnGraphicChange();
	m_Player->Color = 0x0386;
	m_Player->Direction = 2;
	m_Player->Hits = 1;
	m_Player->MaxHits = 1;
	m_Player->NPC = true;
	m_Player->Serial = 0;

	//Линковка в псевдомире: стул; персонаж
	m_Chair->m_NextXY = m_Player;
	m_Player->m_PrevXY = m_Chair;

	//Маунт
	m_Mount = new TGameItem(3);
	m_Mount->Graphic = 0x3EA3;
	m_Mount->OnGraphicChange();
	m_Mount->Color = 0x0386;
	m_Mount->Count = 1;
	m_Mount->Layer = OL_MOUNT;
	m_Player->Add(m_Mount);

	//Слои: волосы, роба, клока
	TGameItem *obj = new TGameItem(4);
	obj->Graphic = 0x203C;
	obj->OnGraphicChange();
	obj->Color = 0x0386; //0x0A01;
	obj->Count = 1;
	obj->Layer = OL_HAIR;
	m_Player->Add(obj);

	obj = new TGameItem(5);
	obj->Graphic = 0x1F03;
	obj->OnGraphicChange();
	obj->Color = 0x0386; //0x0BB6;
	obj->Count = 1;
	obj->Layer = OL_ROBE;
	m_Player->Add(obj);

	m_Cloak = new TGameItem(6);
	m_Cloak->Graphic = 0x1515;
	m_Cloak->OnGraphicChange();
	m_Cloak->Color = 0x0386; //0x0A04;
	m_Cloak->Count = 1;
	m_Cloak->Layer = OL_CLOAK;
	m_Player->Add(m_Cloak);

	m_Map1 = new TGumpMap(0, 0, 0);
	m_Map1->StartX = 0;
	m_Map1->StartY = 0;
	m_Map1->EndX = 2595;
	m_Map1->EndY = 3048;
	m_Map1->Width = 200;
	m_Map1->Height = 200;

	//MultiMap->LoadMap(m_Map1);
	MultiMap->LoadFacet(m_Map1, 0);

	m_Map2 = new TGumpMap(0, 260, 0);
	m_Map2->StartX = 0;
	m_Map2->StartY = 200;
	m_Map2->EndX = 1347;
	m_Map2->EndY = 1800;
	m_Map2->Width = 200;
	m_Map2->Height = 200;

	//MultiMap->LoadMap(m_Map2);
	MultiMap->LoadFacet(m_Map2, 0);

	m_Map3 = new TGumpMap(0, 0, 260);
	m_Map3->StartX = 1219;
	m_Map3->StartY = 1477;
	m_Map3->EndX = 1619;
	m_Map3->EndY = 1877;
	m_Map3->Width = 200;
	m_Map3->Height = 200;

	//MultiMap.LoadMap(m_Map3);
	MultiMap.LoadFacet(m_Map3, 0);*/

	g_ConfigLoaded = false;

	g_OrionWindow.Size = WISP_GEOMETRY::CSize(640, 480);
	g_OrionWindow.NoResize = true;
	g_OrionWindow.SetTitle("Ultima Online");

	g_FontManager.SetSavePixels(false);

	g_ScreenEffectManager.UseSunrise();
	m_SmoothScreenAction = 0;
}
//----------------------------------------------------------------------------------
void CDebugScreen::ProcessSmoothAction(BYTE action)
{
	if (action == 0xFF)
		action = m_SmoothScreenAction;

	if (action == ID_SMOOTH_DS_GO_SCREEN_MAIN)
		g_Orion.InitScreen(GS_MAIN);
	else if (action == ID_SMOOTH_DS_QUIT)
		g_OrionWindow.Destroy();
}
//----------------------------------------------------------------------------------
void CDebugScreen::InitToolTip()
{
}
//----------------------------------------------------------------------------------
void CDebugScreen::Render(const bool &mode)
{
	if (mode)
	{
		g_GL.BeginDraw();

		if (DrawSmoothMonitor())
			return;
		/*
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
			if (DISPLAY_MAP_TEST)
			{
				m_Map1->Draw(mode);
				m_Map2->Draw(mode);
				m_Map3->Draw(mode);
			}
			else
			{
				if (DISPLAY_SHADOW_TEST)
				{
					float cl = 0.1f;
					glColor4f(cl + 0.01f, cl, cl, 1.0f);
					g_GL.DrawPolygone(0, 0, 640, 480);
					glColor4f(1.f, 1.0f, 1.0f, 1.0f);

					ColorizerShader->Use();
					const WORD mountTable[5] = { 0x3EA2, 0x3EA6, 0x3EA3, 0x3EA4, 0x2F05 }; //horse, llama, ostard, zostrich, genie(FW_Verdata)

					m_Mount->Graphic = mountTable[4];
					m_Mount->OnGraphicChange();
					m_Mount->Layer = OL_MOUNT;

					IFOR(j, 0, 2)
					{
						IFOR(i, 0, 4)
						{
							m_Player->Direction = i + (j * 4);
							int x = 100 + (i * 100);
							int y = 100 +(j * 100);

							AnimationManager->DrawCharacter(m_Player, x, y, 0);
						}
					}

					UnuseShader();
				}
				else
				{
					ColorizerShader->Use();
					const WORD mountTable[4] = { 0x3EA2, 0x3EA6, 0x3EA3, 0x3EA4 }; //horse, llama, ostard, zostrich

					//Для рисования 1 ряда размаунтим маунта			
					IFOR(j, 0, 3)
					{
						IFOR(i, 0, 4)
						{
							m_Player->Direction = i + 1;
							m_Chair->Graphic = 2894 + i;
							m_Chair->OnGraphicChange();
							int x = 100 + (i * 100);
							int y = 100 + (j * 100);

							//Рисуем стул только в верхнем ряду, в нижнем - перс на маунте
							if (j < 2)
								m_Chair->Draw(mode, x, y, g_Ticks);
							else
								m_Mount->Graphic = mountTable[i];

							if (j == 0 && i == 0)
								m_Mount->Layer = OL_NONE;
							else if (j == 1 && i == 0)
								m_Cloak->Layer = OL_NONE;
							else if (j == 2 && i == 0)
							{
								m_Cloak->Layer = OL_CLOAK;
								m_Mount->Layer = OL_MOUNT;
							}

							AnimationManager->DrawCharacter(m_Player, x, y, 0);
						}
					}
					UnuseShader();
				}
			}
		}

		fb.Free();
		*/

		InitToolTip();

		g_MouseManager.Draw(0x2073); //Main Gump mouse cursor

		g_GL.EndDraw();
	}
	else
	{
	}
}
//----------------------------------------------------------------------------------
void CDebugScreen::OnLeftMouseButtonUp()
{
	Render(false);

	if (g_SelectedObject.Object() != NULL && g_SelectedObject.Object() == g_PressedObject.LeftObject())
	{
		if (g_SelectedObject.Serial == ID_DS_GO_SCREEN_MAIN) //> button
			CreateSmoothAction(ID_SMOOTH_DS_GO_SCREEN_MAIN);
	}

	g_PressedObject.ClearLeft();
}
//----------------------------------------------------------------------------------
void CDebugScreen::OnKeyDown(const WPARAM &wParam, const LPARAM &lParam)
{
	if (wParam == VK_RETURN)
		CreateSmoothAction(ID_SMOOTH_DS_GO_SCREEN_MAIN);
}
//----------------------------------------------------------------------------------
