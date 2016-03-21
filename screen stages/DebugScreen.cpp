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

			if (m_EffectType == EF_MOVING)
			{
				TEffectMoving *moving = (TEffectMoving*)this;

				deX += moving->OffsetX;
				deY += moving->OffsetY + moving->OffsetZ;
			}

			ApplyRenderMode();

			if (m_FixedDirection)
				UO->DrawStaticArt(objGraphic, m_Color, deX, deY, m_Z);
			else
			{
				TTextureObject *th = UO->ExecuteStaticArt(objGraphic);

				if (th != NULL)
				{
					glBindTexture(GL_TEXTURE_2D, th->Texture);

					deY -= (m_Z * 4);

					int width = th->Width;
					int heightDiv2 = th->Height / 2;

					float angle = ((TEffectMoving*)this)->Angle;

					glLoadIdentity();
					glTranslatef((GLfloat)deX, (GLfloat)(deY - heightDiv2), 0.0f);

					glRotatef(angle, 0.0f, 0.0f, 1.0f);

					glBegin(GL_TRIANGLE_STRIP);
						glTexCoord2i(0, 1); glVertex2i(0, -heightDiv2);
						glTexCoord2i(1, 1); glVertex2i(width, -heightDiv2);
						glTexCoord2i(0, 0); glVertex2i(0, heightDiv2);
						glTexCoord2i(1, 0); glVertex2i(width, heightDiv2);
					glEnd();
				}
			}

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

	//TPRINT("Generate effectID for 0x%04X (add %i)\n", m_Graphic, m_Increment);

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
		case 1: //ok
		{
			glEnable(GL_BLEND);
			glBlendFunc(GL_ZERO, GL_SRC_COLOR);
			break;
		}
		case 2: //ok
		case 3: //ok
		{
			glEnable(GL_BLEND);
			glBlendFunc(GL_ONE, GL_ONE);
			break;
		}
		case 4:
		{
			glEnable(GL_BLEND);
			glBlendFunc(GL_DST_COLOR, GL_ONE_MINUS_SRC_ALPHA);
			glColor4f(g_DrawColor, g_DrawColor, g_DrawColor, 0.1f);
			break;
		}
		case 5:
		{
			glEnable(GL_BLEND);
			glBlendFunc(GL_DST_COLOR, GL_SRC_COLOR);
			break;
		}
		case 6: //ok
		{
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_COLOR, GL_ONE_MINUS_SRC_COLOR);
			glBlendEquation(GL_FUNC_REVERSE_SUBTRACT);
			break;
		}
		default:
			break;
	}
}
//---------------------------------------------------------------------------
TEffectMoving::TEffectMoving()
: TEffect(), m_Angle(0.0f), m_OffsetX(0), m_OffsetY(0), m_OffsetZ(0)
{
}
//---------------------------------------------------------------------------
TEffectMoving::~TEffectMoving()
{
}
//---------------------------------------------------------------------------
void TEffectMoving::Update()
{
	int cx = 320;
	int cy = 240;

	int ctx = 10;
	int cty = 10;



	int offsetX = m_X - ctx;
	int offsetY = m_Y - cty;

	int drawX = cx + (offsetX - offsetY) * 22;
	int drawY = cy + (offsetX + offsetY) * 22;

	int realDrawX = drawX + m_OffsetX;
	int realDrawY = drawY + m_OffsetY;

	int offsetDestX = m_DestX - ctx;
	int offsetDestY = m_DestY - cty;

	int drawDestX = cx + (offsetDestX - offsetDestY) * 22;
	int drawDestY = cy + (offsetDestX + offsetDestY) * 22;

	int x = 0;

	int deltaXY[2] = { abs(drawDestX - realDrawX), abs(drawDestY - realDrawY) };

	if (deltaXY[0] < deltaXY[1])
	{
		x = 1;

		int temp = deltaXY[0];

		deltaXY[0] = deltaXY[1];
		deltaXY[1] = temp;
	}

	int delta = deltaXY[0] / 2;
	int stepXY = 0;

	int step = m_Speed;
	int tempXY[2] = { step, 0 };

	for (int j = 0; j < step; j++)
	{
		stepXY += deltaXY[1];

		if (stepXY >= delta)
		{
			tempXY[1]++;

			stepXY -= deltaXY[0];
		}
	}

	if (realDrawX < drawDestX)
	{
		realDrawX += tempXY[x];

		if (realDrawX > drawDestX)
			realDrawX = drawDestX;
	}
	else
	{
		realDrawX -= tempXY[x];

		if (realDrawX < drawDestX)
			realDrawX = drawDestX;
	}

	if (realDrawY < drawDestY)
	{
		realDrawY += tempXY[(x + 1) % 2];

		if (realDrawY > drawDestY)
			realDrawY = drawDestY;
	}
	else
	{
		realDrawY -= tempXY[(x + 1) % 2];

		if (realDrawY < drawDestY)
			realDrawY = drawDestY;
	}

	int ox = (realDrawX - cx) / 22;
	int oy = (realDrawY - cy) / 22;

	int dx = 0;
	int dy = 0;

	TileOffsetOnMonitorToXY(ox, oy, dx, dy);

	int newX = ctx + dx;
	int newY = cty + dy;

	if (newX == m_DestX && newY == m_DestY)
	{
		m_Z = m_DestZ;
	}
	else
	{
		int newDrawX = cx + (dx - dy) * 22;
		int newDrawY = cy + (dx + dy) * 22;

		m_OffsetX = realDrawX - newDrawX;
		m_OffsetY = realDrawY - newDrawY;

		int countX = drawDestX - (newDrawX + m_OffsetX);
		int countY = drawDestY - (newDrawY + m_OffsetY);
		//int countY = drawDestY - (newDrawY + m_OffsetY + m_OffsetZ) - (m_DestZ - m_Z) * 4;

		if (m_Z != m_DestZ)
		{
			int stepsCountX = countX / (tempXY[x] + 1);
			int stepsCountY = countY / (tempXY[(x + 1) % 2] + 1);

			if (stepsCountX < stepsCountY)
				stepsCountX = stepsCountY;

			if (stepsCountX <= 0)
				stepsCountX = 1;

			int totalOffsetZ = 0;

			if (m_Z < m_DestZ)
				totalOffsetZ = (m_DestZ - m_Z) * 4;
			else
				totalOffsetZ = (m_Z - m_DestZ) * 4;

			totalOffsetZ /= stepsCountX;

			if (!totalOffsetZ)
				totalOffsetZ = 1;

			m_OffsetZ += totalOffsetZ;

			if (m_OffsetZ >= 4)
			{
				int countZ = m_OffsetZ / 4;
				m_OffsetZ -= countZ * 4;

				if (m_Z < m_DestZ)
					m_Z += countZ;
				else
					m_Z -= countZ;
			}
		}

		countY -= m_OffsetZ + (m_DestZ - m_Z) * 4;

		m_Angle = 180.0f + ((float)atan2(countY, countX) * 57.295780f); //180.0f / M_PI = 57.295780f

		if (m_X != newX || m_Y != newY)
		{
			m_X = newX;
			m_Y = newY;
		}
	}
}



TDebugScreen *DebugScreen = NULL;
//---------------------------------------------------------------------------
TDebugScreen::TDebugScreen()
: TBaseScreen()
{
	TEffectMoving *ef = new TEffectMoving();
	ef->Graphic = 0x36E4;
	ef->Duration = GetTickCount();
	ef->Speed = 5;
	ef->X = 10;
	ef->Y = 10;
	ef->Z = 20;
	ef->DestX = 10;
	ef->DestY = 0;
	ef->DestZ = 0;
	//ef->RenderMode = 3;
	ef->FixedDirection = false;
	ef->Update();

	effect = ef;
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

	PopupHelp.SeqIndex = 0;

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
void TDebugScreen::InitPopupHelp()
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

		g_LastRenderTime = ticks + (g_FrameDelay[(int)(GetForegroundWindow() == g_hWnd)]);

		g_GL.BeginDraw();

		if (DrawSmoothMonitor())
			return 0;

		ColorizerShader->Use();

		WORD GumpID = 0x15A4 + (int)(CanSelectedButton == ID_DS_GO_SCREEN_MAIN); //> gump / lighted
		if (CanPressedButton == ID_DS_GO_SCREEN_MAIN)
			GumpID = 0x15A6; //> gump pressed
		UO->DrawGump(GumpID, 0, 610, 445);

		effect->Update();
		int gx = effect->X - 10;
		int gy = effect->Y - 10;
		bool mde = true;
		ticks = GetTickCount();

		int drawX = 320 + (gx - gy) * 22;
		int drawY = 240 + (gx + gy) * 22;

		effect->Draw(mde, drawX, drawY, ticks);

		UnuseShader();

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

		MouseManager.Draw(0x2073); //Main Gump mouse cursor

		g_GL.EndDraw();
	}
	else
	{
		g_LastSelectedObject = 0;

		if (UO->GumpPixelsInXY(0x15A4, 610, 445))
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

	/*effect->OffsetX = 0;
	effect->OffsetY = 0;
	effect->OffsetZ = 0;

	effect->X = 10;
	effect->Y = 10;

	if (!(rand() % 2))
		effect->Z = 20;*/

	int cx = 320;
	int cy = 240;

	int px = g_MouseX;
	int py = g_MouseY;

	int ox = (px - cx) / 22;
	int oy = (py - cy) / 22;

	int dx = 0;
	int dy = 0;

	TileOffsetOnMonitorToXY(ox, oy, dx, dy);

	effect->DestX = 10 + dx;
	effect->DestY = 10 + dy;

	effect->Update();

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
void TDebugScreen::OnKeyPress(WPARAM wparam, LPARAM lparam)
{
	if (wparam == VK_RETURN)
		CreateSmoothAction(ID_SMOOTH_DS_GO_SCREEN_MAIN);
}
//---------------------------------------------------------------------------