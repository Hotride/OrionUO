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
			int deX = 0;// drawX;
			int deY = 0;// drawY;
			int deZ = 0;

			if (m_EffectType == EF_MOVING)
			{
				TEffectMoving *moving = (TEffectMoving*)this;

				deX += moving->OffsetX;
				deY += moving->OffsetY;
				deZ += moving->OffsetZ;
			}

			ApplyRenderMode();

			if (m_FixedDirection)
				UO->DrawStaticArt(objGraphic, m_Color, deX, deY, m_Z + deZ);
			else
			{
				TTextureObject *th = UO->ExecuteStaticArt(objGraphic);

				if (th != NULL)
				{
					glBindTexture(GL_TEXTURE_2D, th->Texture);

					deY -= (m_Z * 4 + deZ);

					double cosA = ((TEffectMoving*)(this))->CosA;
					double sinA = ((TEffectMoving*)(this))->SinA;

					int posMinX = deX - (th->Width / 2);
					int posMinY = deY - (th->Height + 44);
					int posMaxX = posMinX + th->Width;
					int posMaxY = posMinY + th->Height;

					int mx = posMinX + (posMaxX - posMinX) / 2;
					int my = posMinY + (posMaxY - posMinY) / 2;

					int tmpX = posMinX - mx;
					int tmpY = posMinY - my;
					int x1 = mx + (int)floor(tmpX * cosA - tmpY * sinA);
					int y1 = my + (int)floor(tmpX * sinA + tmpY * cosA);

					tmpX = posMaxX - mx;
					tmpY = posMinY - my;
					int x2 = mx + (int)floor(tmpX * cosA - tmpY * sinA);
					int y2 = my + (int)floor(tmpX * sinA + tmpY * cosA);

					tmpX = posMaxX - mx;
					tmpY = posMaxY - my;
					int x3 = mx + (int)floor(tmpX * cosA - tmpY * sinA);
					int y3 = my + (int)floor(tmpX * sinA + tmpY * cosA);

					tmpX = posMinX - mx;
					tmpY = posMaxY - my;
					int x4 = mx + (int)floor(tmpX * cosA - tmpY * sinA);
					int y4 = my + (int)floor(tmpX * sinA + tmpY * cosA);

					glLoadIdentity();
					glTranslatef((GLfloat)(deX - posMinX), (GLfloat)(deY - posMinY), 0.0f);

					glBegin(GL_QUADS);
						glTexCoord2i(0, 1); glVertex2i(x1, y1);
						glTexCoord2i(1, 1); glVertex2i(x2, y2);
						glTexCoord2i(1, 0); glVertex2i(x3, y3);
						glTexCoord2i(0, 0); glVertex2i(x4, y4);
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
			break;
		case 1:
			glEnable(GL_BLEND);
			//glBlendFunc(GL_SRC_ALPHA, GL_SRC_ALPHA);
			break;
		case 2:
			glEnable(GL_BLEND);
			glBlendFunc(GL_ONE, GL_ONE);
			break;
		case 3:
			glEnable(GL_BLEND);
			glBlendFunc(GL_ONE, GL_ONE);
			break;
		case 4:
			glEnable(GL_BLEND);
			glBlendFunc(GL_ONE, GL_ONE);
			break;
		case 5:
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_COLOR, GL_SRC_ALPHA_SATURATE);
			break;
		case 6:
			glEnable(GL_BLEND);
			glBlendFunc(GL_ONE, GL_ONE);
			//glBlendEquation(GL_FUNC_REVERSE_SUBTRACT);
			break;
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
	m_DiffX = m_DestX - m_X;
	m_DiffY = m_DestY - m_Y;
	m_DiffZ = m_DestZ - m_Z;

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

	m_Distance = (int)floor(sqrt(m_DiffX * m_DiffX + m_DiffY * m_DiffY)) * 2 + 1;

	m_Step = 0;
	m_OffsetX = 320;
	m_OffsetY = 240;
	m_OffsetZ = 0;
}
//---------------------------------------------------------------------------
void TEffectMoving::Update()
{
	int cx = 320;
	int cy = 240;

	int ctx = 10;
	int cty = 10;



	int realDrawX = cx + m_OffsetX;
	int realDrawY = cy + m_OffsetY;

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

	if (deltaXY[0] == 0)
		deltaXY[0] = 1;

	double delta = deltaXY[1] / deltaXY[0];
	double stepXY = 0.0;

	int step = m_Speed;// *5;
	int tempXY[2] = { step, 0 };

	for (int j = 0; j < step; j++)
	{
		stepXY += delta;

		if (stepXY >= 0.5)
		{
			tempXY[1] += 1;

			stepXY -= 1.0;
		}
	}

	bool incX = (realDrawX < drawDestX);
	bool incY = (realDrawY < drawDestY);

	if (incX)
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

	if (incY)
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

	//TPRINT("lofs: %i %i\n", m_OffsetX, m_OffsetY);
	m_OffsetX = realDrawX - cx;
	m_OffsetY = realDrawY - cy;
	//TPRINT("nofs: %i %i\n", m_OffsetX, m_OffsetY);


	/*int cx = 320;
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

	if (deltaXY[0] == 0)
		deltaXY[0] = 1;

	double delta = deltaXY[1] / deltaXY[0];
	double stepXY = 0.0;

	int step = m_Speed;// *5;
	int tempXY[2] = { step, 0 };

	for (int j = 0; j < step; j++)
	{
		stepXY += delta;

		if (stepXY >= 0.5)
		{
			tempXY[1] += 1;

			stepXY -= 1.0;
		}
	}

	bool incX = (realDrawX < drawDestX);
	bool incY = (realDrawY < drawDestY);

	if (incX)
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

	if (incY)
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
	}
	else
	{
		int newDrawX = cx + (dx - dy) * 22;
		int newDrawY = cy + (dx + dy) * 22;

		m_OffsetX = realDrawX - newDrawX;
		m_OffsetY = realDrawY - newDrawY;

		m_X = newX;
		m_Y = newY;
	}*/

	/*if (m_Step == m_Distance + 1)
	{
	}
	else
	{
		int oldX = m_X;
		int oldY = m_Y;
		int oldZ = m_Z;

		double offset = m_Step / (double)m_Distance;

		int newX = oldX + (int)(m_DiffX * offset);
		int newY = oldY + (int)(m_DiffY * offset);
		int newZ = oldZ + (int)(m_DiffZ * offset);

		m_OffsetX = (int)floor((m_DiffX - m_DiffY) * 22 * offset);
		m_OffsetY = (int)floor((m_DiffX + m_DiffY) * 22 * offset);
		m_OffsetZ = (int)floor(m_DiffZ * 4 * offset);

		m_Step++;

		if (oldX != newX || oldY != newY || oldZ != newZ)
		{
			m_X = newX;
			m_Y = newY;
			m_Z = newZ;

			Init();
		}
	}*/
}



TDebugScreen *DebugScreen = NULL;
//---------------------------------------------------------------------------
TDebugScreen::TDebugScreen()
: TBaseScreen()
{
	m_Text = new TEntryText(0, 100);
	m_Text->SetText("test text");

	TEffectMoving *ef = new TEffectMoving();
	ef->Graphic = 0x36E4;
	ef->Duration = GetTickCount();
	ef->Speed = 5;
	ef->X = 10;
	ef->Y = 10;
	ef->Z = 0;
	ef->DestX = 10;
	ef->DestY = 0;
	ef->FixedDirection = true;

	ef->Init();

	effect = ef;
}
//---------------------------------------------------------------------------
TDebugScreen::~TDebugScreen()
{
	if (m_Text != NULL)
	{
		delete m_Text;
		m_Text = NULL;
	}
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

		/*WORD GumpID = 0x1589 + (int)(CanSelectedButton == ID_DS_QUIT); //X gump /lighted
		if (CanPressedButton == ID_DS_QUIT)
			GumpID = 0x158B; //X gump (pressed)
		UO->DrawGump(GumpID, 0x21, 555, 4);*/

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

		/*TTextureObject *th = UO->ExecuteStaticArt(0x36E4);

		if (th != NULL)
		{
			glBindTexture(GL_TEXTURE_2D, th->Texture);

			glLoadIdentity();
			glTranslatef(320.0f, 240.0f, 0.0f);

			glBegin(GL_QUADS);
				glTexCoord2i(0, 1); glVertex2i(0, th->Height / 2);
				glTexCoord2i(1, 1); glVertex2i(th->Width * 2, th->Height * 2);
				glTexCoord2i(1, 0); glVertex2i(th->Width / 2, 0);
				glTexCoord2i(0, 0); glVertex2i(0, 0);
			glEnd();
		}*/
		
		UnuseShader();

		DPOLY(315, 235, 10, 10);

		g_GL.DrawLine(0x30000030, 320, 240, g_MouseX, g_MouseY);

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

		EntryPointer = m_Text;
		m_Text->DrawA(3, 0x0386, 20, 100, TS_LEFT, UOFONT_FIXED);
		//m_Text->DrawW(1, 0x0021, 20, 100, TS_LEFT, UOFONT_FIXED);

		DrawSmoothMonitorEffect();

		MouseManager.Draw(0x2073); //Main Gump mouse cursor

		g_GL.EndDraw();
	}
	else
	{
		g_LastSelectedObject = 0;

		/*if (UO->GumpPixelsInXY(0x1589, 555, 4))
			g_LastSelectedObject = ID_DS_QUIT; //X gump
		else if (UO->GumpPixelsInXY(0x15A4, 610, 445))
			g_LastSelectedObject = ID_DS_GO_SCREEN_MAIN; //> gump*/

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

	effect->X = 10;
	effect->Y = 10;

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

	effect->Init();

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
void TDebugScreen::OnCharPress(WPARAM wparam, LPARAM lparam)
{
	if (wparam == VK_RETURN || wparam == VK_BACK || wparam == VK_ESCAPE || m_Text == NULL)
		return; //Ignore no print keys

	m_Text->Insert(wparam);
}
//---------------------------------------------------------------------------
void TDebugScreen::OnKeyPress(WPARAM wparam, LPARAM lparam)
{
	if (wparam == VK_RETURN)
		CreateSmoothAction(ID_SMOOTH_DS_GO_SCREEN_MAIN);
	else if (wparam == VK_BACK || wparam == VK_ESCAPE)
		TPRINT("Data text: %s\n", m_Text->c_str());
	else
		m_Text->OnKey(NULL, wparam);
}
//---------------------------------------------------------------------------