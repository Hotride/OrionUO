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






//---------------------------------------------------------------------------
TEffect::TEffect()
: TRenderObject(0, 0, 0, 0, 0), m_EffectType(EF_MOVING),
m_DestSerial(0), m_DestX(0), m_DestY(0), m_DestZ(0), m_Speed(0), m_Duration(0),
m_FixedDirection(false), m_Explode(false), m_RenderMode(0), m_AnimIndex(0),
m_Increment(0), m_LastChangeFrameTime(0)
{
}
//---------------------------------------------------------------------------
TEffect::~TEffect()
{
}
//---------------------------------------------------------------------------
int TEffect::Draw(bool &mode, int &drawX, int &drawY, DWORD &ticks)
{
	if (mode)
	{
		WORD objGraphic = 0;

		if (LastChangeFrameTime < ticks)
		{
			LastChangeFrameTime = ticks + m_Speed;

			objGraphic = CalculateCurrentGraphic();
		}
		else
			objGraphic = GetCurrentGraphic();

		if (objGraphic)
		{
			int deX = drawX;
			int deY = drawY;
			int deZ = 0;

			if (m_EffectType == EF_MOVING)
			{
				TEffectMoving *moving = (TEffectMoving*)this;

				deX += moving->OffsetX;
				deY += moving->OffsetY;
				deZ += moving->OffsetZ;
			}

			ApplyRenderMode();

			UO->DrawStaticArt(objGraphic, m_Color, deX, deY, m_Z + deZ);

			glDisable(GL_BLEND);
		}
	}

	return 0;
}
//---------------------------------------------------------------------------
WORD TEffect::CalculateCurrentGraphic()
{
	DWORD addressAnimData = (DWORD)FileManager.AnimdataMul.Address;

	if (addressAnimData)
	{
		DWORD addr = (m_Graphic * 68) + 4 * ((m_Graphic / 8) + 1);
		PANIM_DATA pad = (PANIM_DATA)(addressAnimData + addr);

		if (m_AnimIndex < (int)pad->FrameCount - 1)
		{
			m_Increment = pad->FrameData[m_AnimIndex];
			m_AnimIndex++;
		}
		else
		{
			m_AnimIndex = 0;
			m_Increment = pad->FrameData[m_AnimIndex];
		}
	}

	//trace_printf("Generate effectID for 0x%04X (add %i)\n", m_Graphic, m_Increment);

	return m_Graphic + m_Increment;
}
//---------------------------------------------------------------------------
WORD TEffect::GetCurrentGraphic()
{
	return m_Graphic + m_Increment;
}
//---------------------------------------------------------------------------
void TEffect::ApplyRenderMode()
{
	switch (m_RenderMode % 7)
	{
	case 0:
	{
			  break;
	}
	case 1:
	{
			  glEnable(GL_BLEND);

			  //glBlendFunc(GL_SRC_ALPHA, GL_SRC_ALPHA);
			  break;
	}
	case 2:
	{
			  glEnable(GL_BLEND);
			  glBlendFunc(GL_ONE, GL_ONE);
			  break;
	}
	case 3:
	{
			  glEnable(GL_BLEND);
			  glBlendFunc(GL_ONE, GL_ONE);
			  break;
	}
	case 4:
	{
			  glEnable(GL_BLEND);
			  glBlendFunc(GL_ONE, GL_ONE);
			  break;
	}
	case 5:
	{
			  glEnable(GL_BLEND);
			  glBlendFunc(GL_SRC_COLOR, GL_SRC_ALPHA_SATURATE);
			  break;
	}
	case 6:
	{
			  glEnable(GL_BLEND);
			  glBlendFunc(GL_ONE, GL_ONE);
			  //glBlendEquation(GL_FUNC_REVERSE_SUBTRACT);
			  break;
	}
	default:
		break;
	}
}
//---------------------------------------------------------------------------
TEffectMoving::TEffectMoving()
: TEffect(), m_DiffX(0), m_DiffY(0), m_DiffZ(0), m_CosA(0.0), m_SinA(0.0),
m_OffsetX(0), m_OffsetY(0), m_OffsetZ(0), m_Step(0), m_Distance(0)
{
}
//---------------------------------------------------------------------------
TEffectMoving::~TEffectMoving()
{
}
//---------------------------------------------------------------------------
void TEffectMoving::Init()
{
	m_DiffX = m_DestX - X;
	m_DiffY = m_DestY - Y;
	m_DiffZ = m_DestZ - Z;

	int posX = (m_DiffX - m_DiffY) * 44;
	int posY = (m_DiffX + m_DiffY) * 44 + m_DiffZ * 4;

	double alpha = 0.0;
	if (posX == 0)
		alpha = M_PI / 2.0;
	else
		alpha = atan(posY / posX);

	if (alpha < 0.0)
		alpha += M_PI;

	if ((posY > 0) || ((posX > 0) && (posY == 0)))
		alpha += M_PI;

	alpha -= (M_PI / 2.0);
	if (alpha < 0)
		alpha += (2.0 * M_PI);

	m_CosA = cos(alpha);
	m_SinA = sin(alpha);

	m_DiffZ = DestZ - Z;
	m_Distance = (int)floor(sqrt(m_DiffX * m_DiffX + m_DiffY * m_DiffY)) * 2 + 1;

	m_Step = 0;
	m_OffsetX = 0;
	m_OffsetY = 0;
	m_OffsetZ = 0;
}
//---------------------------------------------------------------------------
void TEffectMoving::Update()
{
	if (m_Step == m_Distance + 1)
	{
	}
	else
	{
		int oldX = m_X;
		int oldY = m_Y;
		int oldZ = m_Z;

		int newX = oldX + (int)(m_DiffX * m_Step / m_Distance);
		int newY = oldY + (int)(m_DiffY * m_Step / m_Distance);
		int newZ = oldZ + (int)(m_DiffZ * m_Step / m_Distance);

		m_OffsetX = (int)floor((m_DiffX - m_DiffY) * 22 * m_Step / m_Distance);
		m_OffsetY = (int)floor((m_DiffX + m_DiffY) * 22 * m_Step / m_Distance);
		m_OffsetZ = (int)floor(m_DiffZ * 4 * m_Step / m_Distance);

		m_Step++;

		if (oldX != newX || oldY != newY || oldZ != newZ)
		{
			m_X = newX;
			m_Y = newY;
			m_Z = newZ;

			Init();
		}
	}
}



TDebugScreen *DebugScreen = NULL;
//---------------------------------------------------------------------------
TDebugScreen::TDebugScreen()
: TBaseScreen()
{
	effect = new TEffectMoving();
	effect->Graphic = 0x36BD;
	effect->Duration = GetTickCount() + 150;
	effect->Speed = 50;
	effect->X = 0;
	effect->Y = 0;
	effect->DestX = 10;
	effect->DestY = 6;

	effect->Init();
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

	if (g_UseSmoothMonitor)
	{
		g_SmoothMonitorMode = SMOOTH_MONITOR_SUNRISE;
		g_SmoothMonitorColor = 0.0f;
		g_SmoothMonitorStep = (GLfloat)g_SmoothMonitorScale * 0.01f;
		m_SmoothScreenAction = 0;
	}
	else
		g_SmoothMonitorMode = SMOOTH_MONITOR_NONE;

	Tooltip.SeqIndex = 0;

	//Prepare textures on Main Screen:
	UO->ExecuteGump(0x0588); //Main Screen background
	UO->ExecuteGump(0x157C); //Main Screen
	UO->ExecuteGump(0x15A0); //Main Screen Notes
	UO->ExecuteResizepic(0x13BE); //ActPwd Container
	UO->ExecuteGump(0x058A); //Main Screen Castle?
	UO->ExecuteGumpPart(0x1589, 3); //X gump
	UO->ExecuteGumpPart(0x15A4, 3); //> gump
	UO->ExecuteResizepic(0x0BB8); //Account/Password text field
	UO->ExecuteGumpPart(0x00D2, 2); //Checkbox on / off
}
//---------------------------------------------------------------------------
void TDebugScreen::ProcessSmoothAction(BYTE action)
{
	if (action == 0xFF)
		action = m_SmoothScreenAction;

	if (action == ID_SMOOTH_DS_GO_SCREEN_MAIN)
		UO->InitScreen(GS_MAIN);
	else if (action == ID_SMOOTH_DS_QUIT)
		PostMessage(g_hWnd, WM_CLOSE, 0, 0);
}
//---------------------------------------------------------------------------
void TDebugScreen::InitTooltip()
{
}
//---------------------------------------------------------------------------
int TDebugScreen::Render(bool mode)
{
	DWORD ticks = GetTickCount();

	if (g_LastRenderTime > ticks)
	{
		if (mode || !g_SelectGumpObjects)
			return 0;
	}

	if (mode)
	{
		int CanSelectedButton = g_LastSelectedObject;

		int CanPressedButton = 0;
		if (g_LeftMouseDown && g_LastObjectLeftMouseDown == g_LastSelectedObject)
			CanPressedButton = g_LastObjectLeftMouseDown;

		g_LastRenderTime = ticks + g_FrameDelay;

		g_GL.BeginDraw();

		if (DrawSmoothMonitor())
			return 0;

		glColor3f(1.0f, 1.0f, 1.0f);

		ColorizerShader->Use();

		WORD GumpID = 0x1589 + (int)(CanSelectedButton == ID_DS_QUIT); //X gump /lighted
		if (CanPressedButton == ID_DS_QUIT)
			GumpID = 0x158B; //X gump (pressed)
		UO->DrawGump(GumpID, 0x21, 555, 4);

		UnuseShader();

		GumpID = 0x1589 + (int)(CanSelectedButton == ID_DS_QUIT); //X gump /lighted
		if (CanPressedButton == ID_DS_QUIT)
			GumpID = 0x158B; //X gump (pressed)
		UO->DrawGump(GumpID, 0, 555, 64);

		DeathShader->Use();

		GumpID = 0x1589 + (int)(CanSelectedButton == ID_DS_QUIT); //X gump /lighted
		if (CanPressedButton == ID_DS_QUIT)
			GumpID = 0x158B; //X gump (pressed)
		UO->DrawGump(GumpID, 0, 555, 124);

		GumpID = 0x15A4 + (int)(CanSelectedButton == ID_DS_GO_SCREEN_MAIN); //> gump / lighted
		if (CanPressedButton == ID_DS_GO_SCREEN_MAIN)
			GumpID = 0x15A6; //> gump pressed
		UO->DrawGump(GumpID, 0, 610, 445);

		ColorizerShader->Use();

		effect->Update();
		int gx = effect->X;
		int gy = effect->Y;
		bool mde = true;
		ticks = GetTickCount();

		int drawX = 320 + (gx - gy) * 22;
		int drawY = 240 + (gx + gy) * 22;

		effect->Draw(mde, drawX, drawY, ticks);

		UnuseShader();

		DrawSmoothMonitorEffect();

		MouseManager.Draw(0x2073); //Main Gump mouse cursor

		g_GL.EndDraw();
	}
	else
	{
		g_LastSelectedObject = 0;

		if (UO->GumpPixelsInXY(0x1589, 555, 4))
			g_LastSelectedObject = ID_DS_QUIT; //X gump
		else if (UO->GumpPixelsInXY(0x15A4, 610, 445))
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

	if (g_LastObjectLeftMouseDown == ID_DS_QUIT) //x button
		CreateSmoothAction(ID_SMOOTH_DS_QUIT);
	else if (g_LastObjectLeftMouseDown == ID_DS_GO_SCREEN_MAIN) //> button
		CreateSmoothAction(ID_SMOOTH_DS_GO_SCREEN_MAIN);

	g_LastObjectLeftMouseDown = 0;
}
//---------------------------------------------------------------------------