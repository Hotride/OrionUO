/****************************************************************************
**
** CreateCharacterGump.cpp
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

TCreateCharacterScreen *CreateCharacterScreen = NULL;
//---------------------------------------------------------------------------
TCreateCharacterScreen::TCreateCharacterScreen()
: TBaseScreen(), m_StyleSelection(0), m_ColorSelection(0)
{
}
//---------------------------------------------------------------------------
TCreateCharacterScreen::~TCreateCharacterScreen()
{
}
//---------------------------------------------------------------------------
/*!
Инициализация
@return 
*/
void TCreateCharacterScreen::Init()
{
	CreateCharacterManager.Clear();
	EntryPointer = CreateCharacterManager.m_Name;

	m_StyleSelection = m_ColorSelection = 0;

	SmoothMonitor.UseSunrise();
	m_SmoothScreenAction = 0;

	ToolTip.SeqIndex = 0;

	//Prepare textures on Main Screen:
	Orion->ExecuteGump(0x0588); //Main Screen background
	Orion->ExecuteGump(0x157C); //Main Screen
	Orion->ExecuteGump(0x15A0); //Main Screen Notes
	Orion->ExecuteGumpPart(0x1589, 3); //X gump
	Orion->ExecuteGumpPart(0x15A1, 3); //< gump
	Orion->ExecuteGumpPart(0x15A4, 3); //> gump
	Orion->ExecuteResizepic(0x0E10); //Create character field
	Orion->ExecuteGump(0x0709); //Character name gump
	Orion->ExecuteGumpPart(0x070A, 3); //Character name field
	Orion->ExecuteGump(0x0708); //Cheate character podium
	Orion->ExecuteResizepic(0xBB8); //text field
	Orion->ExecuteGump(0x00FD); //v gump
	Orion->ExecuteGumpPart(0x070D, 3); //Character female button
	Orion->ExecuteGump(0x0761); //Character create male body gump
	Orion->ExecuteGump(0x0739); //Character create shirt
	Orion->ExecuteGump(0x0738); //Character create pants
	Orion->ExecuteGump(0x0762); //Character create shoes
	Orion->ExecuteGumpPart(0x0710, 3); //Character male button
}
//---------------------------------------------------------------------------
/*!
Обработка события после плавного затемнения экрана
@param [__in_opt] action Идентификатор действия
@return 
*/
void TCreateCharacterScreen::ProcessSmoothAction( __in_opt BYTE action)
{
	if (action == 0xFF)
		action = m_SmoothScreenAction;

	if (action == ID_SMOOTH_CCS_QUIT)
		PostMessage(g_hWnd, WM_CLOSE, 0, 0);
	else if (action == ID_SMOOTH_CCS_GO_SCREEN_CHARACTER)
		Orion->InitScreen(GS_CHARACTER);
	else if (action == ID_SMOOTH_CCS_GO_SCREEN_CONNECT)
	{
		Orion->InitScreen(GS_GAME_CONNECT);
		ConnectionScreen->Type = CST_GAME;
		ConnectionScreen->ConnectionFailed = true;
		ConnectionScreen->ErrorCode = 1;
	}
	else if (action == ID_SMOOTH_CCS_GO_SCREEN_SELECT_TOWN)
		Orion->InitScreen(GS_SELECT_TOWN);
}
//---------------------------------------------------------------------------
/*!
Инициализация всплывающих подсказок
@return 
*/
void TCreateCharacterScreen::InitToolTip()
{
	if (!ConfigManager.UseToolTips)
		return;

	switch (g_LastSelectedObject)
	{
		case ID_CCS_QUIT:
		{
			ToolTip.Set(L"Quit Ultima Online", SOT_NO_OBJECT, g_LastSelectedObject, 80);
			break;
		}
		case ID_CCS_ARROW_NEXT:
		{
			ToolTip.Set(L"Accept this character", SOT_NO_OBJECT, g_LastSelectedObject);
			break;
		}
		case ID_CCS_ARROW_PREV:
		{
			ToolTip.Set(L"Back to character generation screen", SOT_NO_OBJECT, g_LastSelectedObject, 150);
			break;
		}
		default:
			break;
	}
}
//---------------------------------------------------------------------------
/*!
Отрисовка/выбор объектов
@param [__in] mode true - отрисовка, false - выбор
@return При выборе объектов - идентификатор выбранного объекта
*/
int TCreateCharacterScreen::Render( __in bool mode)
{
	DWORD ticks = g_Ticks;

	/*if (g_LastRenderTime > ticks)
	{
		if (mode || !g_SelectGumpObjects)
			return 0;
	}*/

	if (mode)
	{
		WORD cell = 10;

		int CanSelectedButton = g_LastSelectedObject;

		int CanPressedButton = 0;
		if (g_LeftMouseDown && g_LastObjectLeftMouseDown == g_LastSelectedObject)
			CanPressedButton = g_LastObjectLeftMouseDown;
	
		g_LastRenderTime = ticks + (g_FrameDelay[(int)(GetForegroundWindow() == g_hWnd)]);

		g_GL.BeginDraw();

		if (DrawSmoothMonitor())
			return 0;

		glColor3f(1.0f, 1.0f, 1.0f);

		Orion->DrawGump(0x0588, 0, 0, 0, 640, 480); //Main Gump background
		Orion->DrawGump(0x157C, 0, 0, 0); //Main Gump
		Orion->DrawGump(0x15A0, 0, 0, 4); //Main Gump Notes
		
		WORD GumpID = 0x1589 + (int)(CanSelectedButton == ID_CCS_QUIT); //X gump /lighted
		if (CanPressedButton == ID_CCS_QUIT)
			GumpID = 0x158B; //X gump (pressed)
		Orion->DrawGump(GumpID, 0, 555, 4);
		
		GumpID = 0x15A1 + (int)(CanSelectedButton == ID_CCS_ARROW_PREV); //< gump /lighted
		if (CanPressedButton == ID_CCS_ARROW_PREV)
			GumpID = 0x15A3; //< gump pressed
		Orion->DrawGump(GumpID, 0, 586, 445); //< gump

		GumpID = 0x15A4 + (int)(CanSelectedButton == ID_CCS_ARROW_NEXT); //> gump /lighted
		if (CanPressedButton == ID_CCS_ARROW_NEXT)
			GumpID = 0x15A6; //> gump pressed
		Orion->DrawGump(GumpID, 0, 610, 445); //> gump

		Orion->DrawResizepicGump(0x0E10, 82, 125, 151, 310); //Create character field
		
		Orion->DrawGump(0x0709, 0, 280, 53); //Character name gump

		Orion->DrawGump(0x070A, 0, 240, 73); //Character name field (
		Orion->DrawGump(0x070B, 0, 248, 73, 215, 0); //Character name field body
		Orion->DrawGump(0x070C, 0, 463, 73); //Character name field )
		
		EntryPointer->DrawA(5, 1, 257, 65);
		
		Orion->DrawGump(0x0708, 0, 238, 98); //Cheate character podium
		
		Orion->DrawResizepicGump(0x0E10, 475, 125, 151, 310); //Create character field
		
		const WORD hairTextColorRange[2] = { 0x0381, 0x0021 };
		const WORD toneTextColorRange[2] = { 0x0481, 0x0021 };

		if (!CreateCharacterManager.Sex)
		{
			FontManager->DrawA(9, "Facial Hair Style", 0x0481, 100, 187);

			if (m_StyleSelection == CCSID_FACIAL_HAIR_STYLE)
			{
				Orion->DrawResizepicGump(0xBB8, 97, 199, 177, 142); //Facial Hair Style text field extended

				IFOR(i, 0, 8)
					FontManager->DrawA(9, CreateCharacterManager.GetBeard(i).Name.c_str(), hairTextColorRange[(int)(CanSelectedButton == (ID_CCS_STYLE_RANGE + i))], 101, 205 + (i * 14));
			}
			else
			{
				Orion->DrawResizepicGump(0xBB8, 97, 199, 121, 24); //Facial Hair Style text field
				FontManager->DrawA(9, CreateCharacterManager.GetBeard(CreateCharacterManager.BeardStyle).Name.c_str(), 0x0386, 101, 205, 117, TS_LEFT, UOFONT_FIXED);
				Orion->DrawGump(0x00FD, 0, 200, 201); //v gump
			}
		}

		FontManager->DrawA(9, "Hair Style", 0x0481, 100, 142);

		if (m_StyleSelection == CCSID_HAIR_STYLE)
		{
			Orion->DrawResizepicGump(0xBB8, 97, 154, 130, 170); //Hair Style text field extended

			int count = 10 + (int)CreateCharacterManager.Sex;

			IFOR(i, 0, count)
				FontManager->DrawA(9, CreateCharacterManager.GetHair(i).Name.c_str(), hairTextColorRange[(int)(CanSelectedButton == (ID_CCS_STYLE_RANGE + i))], 101, 159 + (i * 14));
		}
		else
		{
			Orion->DrawResizepicGump(0xBB8, 97, 154, 121, 24); //Hair Style text field
			FontManager->DrawA(9, CreateCharacterManager.GetHair(CreateCharacterManager.HairStyle).Name.c_str(), 0x0386, 101, 159);
			Orion->DrawGump(0x00FD, 0, 200, 156); //v gump
		}

		PBYTE huesData = (PBYTE)ColorManager->GetHuesRangePointer() + 32 + 4;
		int colorOffsetDivider = sizeof(HUES_GROUP) - 4;

		if (m_ColorSelection == 0)
		{
			FontManager->DrawA(9, "Skin Tone", toneTextColorRange[(int)(CanSelectedButton == ID_CCS_SKIN_TONE)], 490, 140);

			WORD color = CreateCharacterManager.SkinTone;

			if (color < 0x03EA)
				color = 0x03EA;
			else if (color > 0x0422)
				color = 0x0422;

			int colorIndex = (color + ((color + (color << 2)) << 1)) << 3;
			colorIndex += (colorIndex / colorOffsetDivider) << 2;
			color = *(PWORD)(huesData + colorIndex);

			DWORD clr = ColorManager->Color16To32(color);

			glColor3ub((GetRValue(clr)), GetGValue(clr), GetBValue(clr));
			g_GL.DrawPolygone(490, 154, 120, 25);
			glColor3f(1.0f, 1.0f, 1.0f);



			FontManager->DrawA(9, "Shirt Color", toneTextColorRange[(int)(CanSelectedButton == ID_CCS_SHIRT_COLOR)], 490, 185);

			color = CreateCharacterManager.GetShirtColor();
			//if (color < 0x044E) color = 0x044E;
			//else if (color > 0x04AD) color = 0x04AD;

			colorIndex = (color + ((color + (color << 2)) << 1)) << 3;
			colorIndex += (colorIndex / colorOffsetDivider) << 2;
			color = *(PWORD)(huesData + colorIndex);

			clr = ColorManager->Color16To32(color);

			glColor3ub((GetRValue(clr)), GetGValue(clr), GetBValue(clr));
			g_GL.DrawPolygone(490, 199, 120, 25);
			glColor3f(1.0f, 1.0f, 1.0f);



			color = toneTextColorRange[(int)(CanSelectedButton == ID_CCS_SKIRT_OR_PANTS_COLOR)];

			if (CreateCharacterManager.Sex)
				FontManager->DrawA(9, "Skirt Color", color, 490, 230);
			else
				FontManager->DrawA(9, "Pants Color", color, 490, 230);

			color = CreateCharacterManager.PantsColor;
			//if (color < 0x044E) color = 0x044E;
			//else if (color > 0x04AD) color = 0x04AD;

			colorIndex = (color + ((color + (color << 2)) << 1)) << 3;
			colorIndex += (colorIndex / colorOffsetDivider) << 2;
			color = *(PWORD)(huesData + colorIndex);

			clr = ColorManager->Color16To32(color);

			glColor3ub((GetRValue(clr)), GetGValue(clr), GetBValue(clr));
			g_GL.DrawPolygone(490, 244, 120, 25);
			glColor3f(1.0f, 1.0f, 1.0f);



			FontManager->DrawA(9, "Hair Color", toneTextColorRange[(int)(CanSelectedButton == ID_CCS_HAIR_COLOR)], 490, 275);

			color = CreateCharacterManager.HairColor;

			if (color < 0x044E)
				color = 0x044E;
			else if (color > 0x04AD)
				color = 0x04AD;

			colorIndex = (color + ((color + (color << 2)) << 1)) << 3;
			colorIndex += (colorIndex / colorOffsetDivider) << 2;
			color = *(PWORD)(huesData + colorIndex);

			clr = ColorManager->Color16To32(color);

			glColor3ub((GetRValue(clr)), GetGValue(clr), GetBValue(clr));
			g_GL.DrawPolygone(490, 289, 120, 25);
			glColor3f(1.0f, 1.0f, 1.0f);



			if (!CreateCharacterManager.Sex)
			{
				FontManager->DrawA(9, "Facial Hair Color", toneTextColorRange[(int)(CanSelectedButton == ID_CCS_FACIAL_HAIR_COLOR)], 490, 320);

				color = CreateCharacterManager.BeardColor;

				if (color < 0x044E)
					color = 0x044E;
				else if (color > 0x04AD)
					color = 0x04AD;

				colorIndex = (color + ((color + (color << 2)) << 1)) << 3;
				colorIndex += (colorIndex / colorOffsetDivider) << 2;
				color = *(PWORD)(huesData + colorIndex);

				clr = ColorManager->Color16To32(color);

				glColor3ub((GetRValue(clr)), GetGValue(clr), GetBValue(clr));
				g_GL.DrawPolygone(490, 333, 120, 25);
				glColor3f(1.0f, 1.0f, 1.0f);
			}
		}
		else
		{
			if (m_ColorSelection == CCSID_SKIN_TONE)
			{
				IFOR(y, 0, 8)
				{
					WORD startColor = 0x03E9 + y;

					IFOR(x, 0, 8)
					{
						int colorIndex = (startColor + ((startColor + (startColor << 2)) << 1)) << 3;
						colorIndex += (colorIndex / colorOffsetDivider) << 2;
						WORD color = *(PWORD)(huesData + colorIndex);

						DWORD clr = ColorManager->Color16To32(color);

						glColor3ub((GetRValue(clr)), GetGValue(clr), GetBValue(clr));
						g_GL.DrawPolygone(491 + (x * 15), 138 + (y * 35), 15, 35);

						if (g_LastSelectedObject == ID_CCS_COLOR_RANGE + (x * 8 + y))
						{
							glColor3f(1.0f, 1.0f, 1.0f);
							g_GL.DrawPolygone(490 + ((x * 15) + 6), 140 + ((y * 35) + 16), 2, 2);
						}

						if (x == 6)
							startColor = 0x03E9 + ((x + 1) * 8);
						else
							startColor += 8;
					}
				}
			}
			else if (m_ColorSelection == CCSID_SHIRT_COLOR || m_ColorSelection == CCSID_SKIRT_OR_PANTS_COLOR)
			{
				IFOR(y, 1, 48)
				{
					WORD startColor = (1 << (y % 4)) * 100 + 1 + (y / 4);

					IFOR(x, 0, 20)
					{
						int colorIndex = (startColor + ((startColor + (startColor << 2)) << 1)) << 3;
						colorIndex += (colorIndex / colorOffsetDivider) << 2;
						WORD color = *(PWORD)(huesData + colorIndex);

						DWORD clr = ColorManager->Color16To32(color);

						glColor3ub((GetRValue(clr)), GetGValue(clr), GetBValue(clr));
						g_GL.DrawPolygone(492 + (x * 6), 148 + (y * 5), 6, 5);

						if (g_LastSelectedObject == ID_CCS_COLOR_RANGE + (y * 20 + x))
						{
							glColor3f(1.0f, 1.0f, 1.0f);
							g_GL.DrawPolygone(492 + ((x * 6) + 2), 148 + ((y * 5) + 2), 2, 2);
						}

						startColor += 5;
					}
				}
			}
			else if (m_ColorSelection == CCSID_HAIR_COLOR || m_ColorSelection == CCSID_FACIAL_HAIR_COLOR)
			{
				IFOR(y, 0, 8)
				{
					WORD startColor = 0x44D + y;

					IFOR(x, 0, 6)
					{
						int colorIndex = (startColor + ((startColor + (startColor << 2)) << 1)) << 3;
						colorIndex += (colorIndex / colorOffsetDivider) << 2;
						WORD color = *(PWORD)(huesData + colorIndex);

						DWORD clr = ColorManager->Color16To32(color);

						glColor3ub((GetRValue(clr)), GetGValue(clr), GetBValue(clr));
						g_GL.DrawPolygone(490 + (x * 20), 140 + (y * 35), 20, 35);

						if (g_LastSelectedObject == ID_CCS_COLOR_RANGE + (x * 8 + y))
						{
							glColor3f(1.0f, 1.0f, 1.0f);
							g_GL.DrawPolygone(490 + ((x * 20) + 9), 140 + ((y * 35) + 16), 2, 2);
						}

						if (x == 6)
							startColor = 0x03E9 + ((x + 1) * 8);
						else
							startColor += 8;
					}
				}
			}

			glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
		}

		ColorizerShader->Use();

		if (CreateCharacterManager.Sex)
		{
			Orion->DrawGump(0x0760, CreateCharacterManager.SkinTone, 238, 98); //Character create female body gump
			Orion->DrawGump(0x0714, CreateCharacterManager.ShirtColor, 238, 98); //Character create skirt
			Orion->DrawGump(0x0764, CreateCharacterManager.PantsColor, 238, 98); //Character create dress

			if (CreateCharacterManager.HairStyle)
				Orion->DrawGump(CreateCharacterManager.GetHair(CreateCharacterManager.HairStyle).GumpID, CreateCharacterManager.HairColor, 238, 98); //Character hair
		}
		else
		{
			Orion->DrawGump(0x0761, CreateCharacterManager.SkinTone, 238, 98); //Character create male body gump
			Orion->DrawGump(0x0739, CreateCharacterManager.ShirtColor, 238, 98); //Character create shirt
			Orion->DrawGump(0x0738, CreateCharacterManager.PantsColor, 238, 98); //Character create pants
			Orion->DrawGump(0x0762, 0, 238, 98); //Character create shoes

			if (CreateCharacterManager.HairStyle)
				Orion->DrawGump(CreateCharacterManager.GetHair(CreateCharacterManager.HairStyle).GumpID, CreateCharacterManager.HairColor, 238, 98); //Character hair

			if (CreateCharacterManager.BeardStyle)
				Orion->DrawGump(CreateCharacterManager.GetBeard(CreateCharacterManager.BeardStyle).GumpID, CreateCharacterManager.BeardColor, 238, 98); //Character facial hair
		}

		UnuseShader();

		if (ConnectionManager.ClientVersion < CV_4011D)
		{
			if (CreateCharacterManager.Sex)
			{
				GumpID = 0x070D + (int)(CanSelectedButton == ID_CCS_FEMALE_BUTTON); //Character female button
				if (CanPressedButton == ID_CCS_FEMALE_BUTTON)
					GumpID = 0x070F; //Character female button pressed
				Orion->DrawGump(GumpID, 0, 310, 408);
			}
			else
			{
				GumpID = 0x0710 + (int)(CanSelectedButton == ID_CCS_MALE_BUTTON); //Character male button
				if (CanPressedButton == ID_CCS_MALE_BUTTON)
					GumpID = 0x0712; //Character male button pressed
				Orion->DrawGump(GumpID, 0, 310, 408);
			}
		}
		else
		{
		}

		InitToolTip();

		DrawSmoothMonitorEffect();

		MouseManager.Draw(0x2073); //Main Gump mouse cursor

		g_GL.EndDraw();
	}
	else
	{
		g_LastSelectedObject = 0;

		if (Orion->GumpPixelsInXY(0x1589, 555, 4))
			g_LastSelectedObject = ID_CCS_QUIT; //X gump
		else if (Orion->GumpPixelsInXY(0x15A1, 586, 445))
			g_LastSelectedObject = ID_CCS_ARROW_PREV; //< gump
		else if (Orion->GumpPixelsInXY(0x15A4, 610, 445))
			g_LastSelectedObject = ID_CCS_ARROW_NEXT; //> gump

		if (ConnectionManager.ClientVersion < CV_4011D)
		{
			if (CreateCharacterManager.Sex)
			{
				if (Orion->GumpPixelsInXY(0x070D, 310, 408))
					g_LastSelectedObject = ID_CCS_FEMALE_BUTTON; //female button
			}
			else
			{
				if (Orion->GumpPixelsInXY(0x0710, 310, 408))
					g_LastSelectedObject = ID_CCS_MALE_BUTTON; //male button
			}
		}
		else
		{
		}

		if (!CreateCharacterManager.Sex)
		{
			if (m_StyleSelection == CCSID_FACIAL_HAIR_STYLE)
			{
				DFOR(i, 8, 0)
				{
					if (Orion->PolygonePixelsInXY(101, 205 + (i * 14), 170, 25))
					{
						g_LastSelectedObject = ID_CCS_STYLE_RANGE + i; //Facial Hair text field extended
						break;
					}
				}
			}
			else
			{
				if (Orion->ResizepicPixelsInXY(0xBB8, 97, 199, 121, 24))
					g_LastSelectedObject = ID_CCS_FACIAL_HAIR_STYLE; //Facial Hair text field
			}
		}

		if (m_StyleSelection == CCSID_HAIR_STYLE)
		{
			int count = 10 + (int)CreateCharacterManager.Sex;

			DFOR(i, count, 0)
			{
				if (Orion->PolygonePixelsInXY(101, 159 + (i * 14), 123, 25))
				{
					g_LastSelectedObject = ID_CCS_STYLE_RANGE + i; //Hair text field extended
					break;
				}
			}
		}
		else
		{
			if (Orion->ResizepicPixelsInXY(0xBB8, 97, 154, 121, 24))
				g_LastSelectedObject = ID_CCS_HAIR_STYLE; //Hair text field
		}
		
		if (m_ColorSelection == 0)
		{
			if (Orion->PolygonePixelsInXY(491, 139, 120, 40))
				g_LastSelectedObject = ID_CCS_SKIN_TONE;
			else if (Orion->PolygonePixelsInXY(491, 184, 120, 40))
				g_LastSelectedObject = ID_CCS_SHIRT_COLOR;
			else if (Orion->PolygonePixelsInXY(491, 229, 120, 40))
				g_LastSelectedObject = ID_CCS_SKIRT_OR_PANTS_COLOR;
			else if (Orion->PolygonePixelsInXY(491, 274, 120, 40))
				g_LastSelectedObject = ID_CCS_HAIR_COLOR;
			else if (!CreateCharacterManager.Sex && Orion->PolygonePixelsInXY(491, 319, 120, 40))
				g_LastSelectedObject = ID_CCS_FACIAL_HAIR_COLOR;
		}
		else
		{
			if (m_ColorSelection == CCSID_SKIN_TONE)
			{
				RECT rc = { 0, 0, 120, 280 };
				POINT p = { g_MouseX - 490, g_MouseY - 140 };

				if (PtInRect(&rc, p))
				{
					int x = p.x / 15;
					int y = p.y / 35;

					int index = (x * 8 + y);

					g_LastSelectedObject = ID_CCS_COLOR_RANGE + index;
					WORD st = 0x03EA + index;

					if (st > 0x0422)
						st = 0x0422;

					CreateCharacterManager.SkinTone = st;
				}
			}
			else if (m_ColorSelection == CCSID_SHIRT_COLOR || m_ColorSelection == CCSID_SKIRT_OR_PANTS_COLOR)
			{
				RECT rc = { 0, 0, 120, 240 };
				POINT p = { g_MouseX - 492, g_MouseY - 148 };

				if (PtInRect(&rc, p))
				{
					int x = p.x / 6;
					int y = p.y / 5;

					int index = (y * 20 + x);

					g_LastSelectedObject = ID_CCS_COLOR_RANGE + index;

					WORD st = (1 << (y % 4)) * 100 + 1 + (y / 4);

					if (st)
						st += (x * 5);

					if (y)
					{
						if (m_ColorSelection == CCSID_SHIRT_COLOR)
							CreateCharacterManager.ShirtColor = st;
						else
							CreateCharacterManager.PantsColor = st;
					}
				}
			}
			else if (m_ColorSelection == CCSID_HAIR_COLOR || m_ColorSelection == CCSID_FACIAL_HAIR_COLOR)
			{
				RECT rc = { 0, 0, 120, 280 };
				POINT p = { g_MouseX - 490, g_MouseY - 140 };

				if (PtInRect(&rc, p))
				{
					int x = p.x / 20;
					int y = p.y / 35;
					int index = (x * 8 + y);

					g_LastSelectedObject = ID_CCS_COLOR_RANGE + index;

					if (m_ColorSelection == CCSID_HAIR_COLOR)
						CreateCharacterManager.HairColor = 0x44E + index;
					else
						CreateCharacterManager.BeardColor = 0x44E + index;
				}
			}
		}

		return g_LastSelectedObject;
	}

	return 0;
}
//---------------------------------------------------------------------------
/*!
Нажатие левой кнопки мыши
@return 
*/
void TCreateCharacterScreen::OnLeftMouseDown()
{
	Render(false);

	g_LeftMouseDown = true;
	g_LastObjectLeftMouseDown = g_LastSelectedObject;

	if (g_LastSelectedObject == 0)
	{
		if (m_StyleSelection != 0)
			m_StyleSelection = 0;

		return;
	}

	if (!CreateCharacterManager.Sex)
	{
		if (m_StyleSelection != CCSID_FACIAL_HAIR_STYLE)
		{
			if (g_LastSelectedObject == ID_CCS_FACIAL_HAIR_STYLE)
			{
				m_StyleSelection = CCSID_FACIAL_HAIR_STYLE;
				return;
			}
		}
		else
		{
			BYTE index = g_LastSelectedObject - ID_CCS_STYLE_RANGE;

			if (index < 8)
			{
				CreateCharacterManager.BeardStyle = (BYTE)index;
				m_StyleSelection = 0;
				return;
			}
		}
	}

	if (m_StyleSelection != CCSID_HAIR_STYLE)
	{
		if (g_LastSelectedObject == ID_CCS_HAIR_STYLE)
		{
			m_StyleSelection = CCSID_HAIR_STYLE;
			return;
		}
	}
	else
	{
		int count = 10 + (int)CreateCharacterManager.Sex;

		BYTE index = g_LastSelectedObject - ID_CCS_STYLE_RANGE;

		if (index < count)
		{
			CreateCharacterManager.HairStyle = index;
			m_StyleSelection = 0;
			return;
		}
	}
	
	if (m_ColorSelection == 0)
	{
		if (g_LastSelectedObject == ID_CCS_SKIN_TONE)
			m_ColorSelection = CCSID_SKIN_TONE;
		else if (g_LastSelectedObject == ID_CCS_SHIRT_COLOR)
			m_ColorSelection = CCSID_SHIRT_COLOR;
		else if (g_LastSelectedObject == ID_CCS_SKIRT_OR_PANTS_COLOR)
			m_ColorSelection = CCSID_SKIRT_OR_PANTS_COLOR;
		else if (g_LastSelectedObject == ID_CCS_HAIR_COLOR)
			m_ColorSelection = CCSID_HAIR_COLOR;
		else if (!CreateCharacterManager.Sex && g_LastSelectedObject == ID_CCS_FACIAL_HAIR_COLOR)
			m_ColorSelection = CCSID_FACIAL_HAIR_COLOR;
	}
	else
	{
		if (g_LastSelectedObject >= ID_CCS_COLOR_RANGE)
			m_ColorSelection = 0;
	}
}
//---------------------------------------------------------------------------
/*!
Отпускание левой кнопки мыши
@return 
*/
void TCreateCharacterScreen::OnLeftMouseUp()
{
	Render(false);

	g_LeftMouseDown = false;

	if (g_LastSelectedObject == 0 || g_LastSelectedObject != g_LastObjectLeftMouseDown)
	{
		g_LastObjectLeftMouseDown = 0;
		return;
	}

	if (g_LastObjectLeftMouseDown == ID_CCS_QUIT) //x button
		CreateSmoothAction(ID_SMOOTH_CCS_QUIT);
	else if (g_LastObjectLeftMouseDown == ID_CCS_ARROW_PREV) //< button
		CreateSmoothAction(ID_SMOOTH_CCS_GO_SCREEN_CHARACTER);
	else if (g_LastObjectLeftMouseDown == ID_CCS_ARROW_NEXT) //> button
	{
		if (EntryPointer->Length() < 2)
			CreateSmoothAction(ID_SMOOTH_CCS_GO_SCREEN_CONNECT);
		else
			CreateSmoothAction(ID_SMOOTH_CCS_GO_SCREEN_SELECT_TOWN);
	}
	else if (g_LastSelectedObject == ID_CCS_FEMALE_BUTTON)
		CreateCharacterManager.Sex = false;
	else if (g_LastSelectedObject == ID_CCS_MALE_BUTTON)
		CreateCharacterManager.Sex = true;

	g_LastObjectLeftMouseDown = 0;
}
//---------------------------------------------------------------------------
/*!
Обработка нажатия клавиши
@param [__in] wparam не подписанный параметр
@param [__in] lparam не подписанный параметр
@return 
*/
void TCreateCharacterScreen::OnCharPress( __in WPARAM wparam, __in LPARAM lparam)
{
	if (wparam == VK_RETURN || wparam == VK_BACK || wparam == VK_ESCAPE || wparam >= 0x100)
		return; //Ignore no print keys
	else if (!FontManager->IsPrintASCII(wparam & 0xFF))
		return;
	else if (EntryPointer == NULL)
		EntryPointer = CreateCharacterManager.m_Name;

	wchar_t wstr[2] = {0};
	wstr[0] = wparam;
	wstring str(wstr);

	if (!str.length())
		return;

	if (EntryPointer->Length() < 20) //add char to text field
		EntryPointer->Insert(wparam);
}
//---------------------------------------------------------------------------
/*!
Обработка нажатия клавиши
@param [__in] wparam не подписанный параметр
@param [__in] lparam не подписанный параметр
@return 
*/
void TCreateCharacterScreen::OnKeyPress( __in WPARAM wparam, __in LPARAM lparam)
{
	if (EntryPointer == NULL)
		EntryPointer = CreateCharacterManager.m_Name;

	EntryPointer->OnKey(NULL, wparam);
}
//---------------------------------------------------------------------------