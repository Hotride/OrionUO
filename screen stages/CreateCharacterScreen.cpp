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
: TBaseScreen()
{
}
//---------------------------------------------------------------------------
TCreateCharacterScreen::~TCreateCharacterScreen()
{
}
//---------------------------------------------------------------------------
void TCreateCharacterScreen::Init()
{
	CreateCharacterManager.Clear();
	EntryPointer = CreateCharacterManager.m_Name;

	Tooltip.SeqIndex = 0;

	//Prepare textures on Main Screen:
	UO->ExecuteGump(0x0588); //Main Screen background
	UO->ExecuteGump(0x157C); //Main Screen
	UO->ExecuteGump(0x15A0); //Main Screen Notes
	UO->ExecuteGumpPart(0x1589, 3); //X gump
	UO->ExecuteGumpPart(0x15A1, 3); //< gump
	UO->ExecuteGumpPart(0x15A4, 3); //> gump
	UO->ExecuteResizepic(0x0E10); //Create character field
	UO->ExecuteGump(0x0709); //Character name gump
	UO->ExecuteGumpPart(0x070A, 3); //Character name field
	UO->ExecuteGump(0x0708); //Cheate character podium
	UO->ExecuteResizepic(0xBB8); //text field
	UO->ExecuteGump(0x00FD); //v gump
	UO->ExecuteGumpPart(0x070D, 3); //Character female button
	UO->ExecuteGump(0x0761); //Character create male body gump
	UO->ExecuteGump(0x0739); //Character create shirt
	UO->ExecuteGump(0x0738); //Character create pants
	UO->ExecuteGump(0x0762); //Character create shoes
	UO->ExecuteGumpPart(0x0710, 3); //Character male button
}
//---------------------------------------------------------------------------
void TCreateCharacterScreen::InitTooltip()
{
	if (!ConfigManager.UseToolTips)
		return;

}
//---------------------------------------------------------------------------
int TCreateCharacterScreen::Render(bool mode)
{
	DWORD ticks = GetTickCount();

	if (g_LastRenderTime > ticks)
	{
		if (mode || !g_SelectGumpObjects)
			return 0;
	}

	if (mode)
	{
		WORD cell = 10;

		int CanSelectedButton = g_LastSelectedObject;

		int CanPressedButton = 0;
		if (g_LeftMouseDown && g_LastObjectLeftMouseDown == g_LastSelectedObject)
			CanPressedButton = g_LastObjectLeftMouseDown;
	
		g_LastRenderTime = ticks + g_FrameDelay;

		g_GL.BeginDraw();

		UO->DrawGump(0x0588, 0, 0, 0, 640, 480); //Main Gump background
		UO->DrawGump(0x157C, 0, 0, 0); //Main Gump
		UO->DrawGump(0x15A0, 0, 0, 4); //Main Gump Notes
		
		WORD GumpID = 0x1589 + (int)(CanSelectedButton == 1); //X gump /lighted
		if (CanPressedButton == 1)
			GumpID = 0x158B; //X gump (pressed)
		UO->DrawGump(GumpID, 0, 555, 4);
		
		GumpID = 0x15A1 + (int)(CanSelectedButton == 2); //< gump /lighted
		if (CanPressedButton == 2)
			GumpID = 0x15A3; //< gump pressed
		UO->DrawGump(GumpID, 0, 586, 445); //< gump

		GumpID = 0x15A4 + (int)(CanSelectedButton == 3); //> gump /lighted
		if (CanPressedButton == 3)
			GumpID = 0x15A6; //> gump pressed
		UO->DrawGump(GumpID, 0, 610, 445); //> gump

		UO->DrawResizepicGump(0x0E10, 82, 125, 151, 310); //Create character field
		
		UO->DrawGump(0x0709, 0, 280, 53); //Character name gump

		UO->DrawGump(0x070A, 0, 240, 73); //Character name field (
		UO->DrawGump(0x070B, 0, 248, 73, 215, 0); //Character name field body
		UO->DrawGump(0x070C, 0, 463, 73); //Character name field )
		
		EntryPointer->DrawA(5, 1, 257, 65);
		
		UO->DrawGump(0x0708, 0, 238, 98); //Cheate character podium
		
		UO->DrawResizepicGump(0x0E10, 475, 125, 151, 310); //Create character field
		
		WORD FColor = 0;

		if (!CreateCharacterManager.Sex)
		{
			FontManager->DrawA(9, "Facial Hair Style", 0x0481, 100, 187);

			if (CreateCharacterManager.SelectedFace != 2)
			{
				UO->DrawResizepicGump(0xBB8, 97, 199, 121, 24); //Facial Hair Style text field
				FontManager->DrawA(9, CreateCharacterManager.GetBeard(CreateCharacterManager.BeardStyle).Name.c_str(), 0x0386, 101, 205, 117, TS_LEFT, UOFONT_FIXED);
				UO->DrawGump(0x00FD, 0, 200, 201); //v gump
			}
			else
			{
				UO->DrawResizepicGump(0xBB8, 97, 199, 177, 142); //Facial Hair Style text field extended

				IFOR(i, 0, 8)
				{
					FColor = 0x0386;
					if (CanSelectedButton == 20  + i)
						FColor = 0x0021;

					FontManager->DrawA(9, CreateCharacterManager.GetBeard(i).Name.c_str(), FColor, 101, 205 + (i * 14));
				}
			}
		}

		FontManager->DrawA(9, "Hair Style", 0x0481, 100, 142);

		if (CreateCharacterManager.SelectedFace != 1)
		{
			UO->DrawResizepicGump(0xBB8, 97, 154, 121, 24); //Hair Style text field
			FontManager->DrawA(9, CreateCharacterManager.GetHair(CreateCharacterManager.HairStyle).Name.c_str(), 0x0386, 101, 159);
			UO->DrawGump(0x00FD, 0, 200, 156); //v gump
		}
		else
		{
			UO->DrawResizepicGump(0xBB8, 97, 154, 130, 170); //Hair Style text field extended

			int count = 10 + (int)CreateCharacterManager.Sex;

			IFOR(i, 0, count)
			{
				FColor = 0x0386;
				if (CanSelectedButton == 20  + i)
					FColor = 0x0021;

				FontManager->DrawA(9, CreateCharacterManager.GetHair(i).Name.c_str(), FColor, 101, 159 + (i * 14));
			}
		}
		
		if (CreateCharacterManager.SelectedColor == 1)
		{
			glDisable(GL_TEXTURE_2D);

			IFOR(x, 0, 8)
			{
				IFOR(y, 0, 8)
				{
					FColor = 0x3EA + (x * 8 + (x < 7 ? y : 0));
					
					DWORD clr = ColorManager->GetPolygoneColor(cell, FColor);
					
					g_GL.DrawPolygone(clr, 490 + (x * 15.0f), 140.0f + (y * 35.0f), 15.0f, 35.0f);

					if (g_LastSelectedObject == 40 + (x * 8 + y))
						g_GL.DrawPolygone(0x007F7F7F, 490.0f + ((x * 15.0f) + 6.5f), 140.0f + ((y * 35.0f) + 16.5f), 2.0f, 2.0f);
				}
			}

			glColor3f(1.0f, 1.0f, 1.0f);
			glEnable(GL_TEXTURE_2D);
		}
		else if (CreateCharacterManager.SelectedColor == 0)
		{
			FColor = 0x0481;
			if (CanSelectedButton == 7)
				FColor = 0x0021;
			FontManager->DrawA(9, "Skin Tone", FColor, 490, 140);

			glDisable(GL_TEXTURE_2D);

			FColor = CreateCharacterManager.SkinTone;

			if (FColor < 0x03EA)
				FColor = 0x03EA;
			else if (FColor > 0x0422)
				FColor = 0x0422;

			DWORD clr = ColorManager->GetPolygoneColor(cell, FColor);

			g_GL.DrawPolygone(clr, 490.0f, 154.0f, 120.0f, 25.0f);

			glColor3f(1.0f, 1.0f, 1.0f);
			glEnable(GL_TEXTURE_2D);
		}
		
		if (CreateCharacterManager.SelectedColor == 2)
		{
			glDisable(GL_TEXTURE_2D);

			IFOR(y, 1, 48)
			{
				switch (y % 4)
				{
					case 0:
					{
						FColor = 102 + (y / 4);
						break;
					}
					case 1:
					{
						FColor = 202 + (y / 4);
						break;
					}
					case 2:
					{
						FColor = 402 + (y / 4);
						break;
					}
					case 3:
					{
						FColor = 802 + (y / 4);
						break;
					}
					default:
						break;
				}

				IFOR(x, 0, 20)
				{
					DWORD clr = ColorManager->GetPolygoneColor(7, FColor);
					
					g_GL.DrawPolygone(clr, 492.0f + (x * 6.0f), 148.0f + (y * 5.0f), 6.0f, 5.0f);

					FColor += 5;

					if (g_LastSelectedObject == 110 + (y * 20 + x))
						g_GL.DrawPolygone(0x007F7F7F, 492.0f + ((x * 6.0f) + 2.5f), 148.0f + ((y * 5.0f) + 2.0f), 2.0f, 2.0f);
				}
			}

			glColor3f(1.0f, 1.0f, 1.0f);
			glEnable(GL_TEXTURE_2D);
		}
		else if (CreateCharacterManager.SelectedColor == 0)
		{
			FColor = 0x0481;
			if (CanSelectedButton == 8)
				FColor = 0x0021;
			FontManager->DrawA(9, "Shirt Color", FColor, 490, 185);

			glDisable(GL_TEXTURE_2D);

			FColor = CreateCharacterManager.GetShirtColor();
			//if (FColor < 0x044E) FColor = 0x044E;
			//if (FColor > 0x04AD) FColor = 0x04AD;

			DWORD clr = ColorManager->GetPolygoneColor(cell, FColor);

			g_GL.DrawPolygone(clr, 490.0f, 199.0f, 120.0f, 25.0f);

			glColor3f(1.0f, 1.0f, 1.0f);
			glEnable(GL_TEXTURE_2D);
		}
		
		if (CreateCharacterManager.SelectedColor == 3)
		{
			glDisable(GL_TEXTURE_2D);

			IFOR(y, 1, 48)
			{
				switch (y % 4)
				{
					case 0:
					{
						FColor = 102 + (y / 4);
						break;
					}
					case 1:
					{
						FColor = 202 + (y / 4);
						break;
					}
					case 2:
					{
						FColor = 402 + (y / 4);
						break;
					}
					case 3:
					{
						FColor = 802 + (y / 4);
						break;
					}
					default:
						break;
				}

				IFOR(x, 0, 20)
				{
					DWORD clr = ColorManager->GetPolygoneColor(7, FColor);
					
					g_GL.DrawPolygone(clr, 492.0f + (x * 6.0f), 148.0f + (y * 5.0f), 6.0f, 5.0f);

					FColor += 5;

					if (g_LastSelectedObject == 110 + (y * 20 + x))
						g_GL.DrawPolygone(0x007F7F7F, 492.0f + ((x * 6.0f) + 2.5f), 148.0f + ((y * 5.0f) + 2.0f), 2.0f, 2.0f);
				}
			}

			glColor3f(1.0f, 1.0f, 1.0f);
			glEnable(GL_TEXTURE_2D);
		}
		else if (CreateCharacterManager.SelectedColor == 0)
		{
			FColor = 0x0481;
			if (CanSelectedButton == 9)
				FColor = 0x0021;
			if (CreateCharacterManager.Sex)
				FontManager->DrawA(9, "Skirt Color", FColor, 490, 230);
			else
				FontManager->DrawA(9, "Pants Color", FColor, 490, 230);

			glDisable(GL_TEXTURE_2D);

			FColor = CreateCharacterManager.PantsColor;
			//if (FColor < 0x044E) FColor = 0x044E;
			//if (FColor > 0x04AD) FColor = 0x04AD;

			DWORD clr = ColorManager->GetPolygoneColor(cell, FColor);

			g_GL.DrawPolygone(clr, 490.0f, 244.0f, 120.0f, 25.0f);

			glColor3f(1.0f, 1.0f, 1.0f);
			glEnable(GL_TEXTURE_2D);
		}

		if (CreateCharacterManager.SelectedColor == 4)
		{
			glDisable(GL_TEXTURE_2D);

			IFOR(x, 0, 6)
			{
				IFOR(y, 0, 8)
				{
					FColor = 0x44E + (x * 8 + y);

					DWORD clr = ColorManager->GetPolygoneColor(cell, FColor);

					g_GL.DrawPolygone(clr, 490.0f + (x * 20.0f), 140.0f + (y * 35.0f), 20.0f, 35.0f);

					if (g_LastSelectedObject == 40 + (x * 8 + y))
						g_GL.DrawPolygone(0x007F7F7F, 490.0f + ((x * 20.0f) + 9.0f), 140.0f + ((y * 35.0f) + 16.5f), 2.0f, 2.0f);
				}
			}

			glColor3f(1.0f, 1.0f, 1.0f);
			glEnable(GL_TEXTURE_2D);
		}
		else if (CreateCharacterManager.SelectedColor == 0)
		{
			FColor = 0x0481;
			if (CanSelectedButton == 10)
				FColor = 0x0021;
			FontManager->DrawA(9, "Hair Color", FColor, 490, 275);

			glDisable(GL_TEXTURE_2D);

			FColor = CreateCharacterManager.HairColor;
			if (FColor < 0x044E)
				FColor = 0x044E;
			if (FColor > 0x04AD)
				FColor = 0x04AD;

			DWORD clr = ColorManager->GetPolygoneColor(cell, FColor);
			g_GL.DrawPolygone(clr, 490.0f, 289.0f, 120.0f, 25.0f);

			glColor3f(1.0f, 1.0f, 1.0f);
			glEnable(GL_TEXTURE_2D);
		}

		if (CreateCharacterManager.Sex)
		{
			UO->DrawGump(0x0760, CreateCharacterManager.SkinTone, 238, 98); //Character create female body gump
			UO->DrawGump(0x0714, CreateCharacterManager.ShirtColor, 238, 98); //Character create skirt
			UO->DrawGump(0x0764, CreateCharacterManager.PantsColor, 238, 98); //Character create dress

			if (CreateCharacterManager.HairStyle)
				UO->DrawGump(CreateCharacterManager.GetHair(CreateCharacterManager.HairStyle).GumpID, CreateCharacterManager.HairColor, 238, 98); //Character hair

			GumpID = 0x070D + (int)(CanSelectedButton == 4); //Character female button
			if (CanPressedButton == 4)
				GumpID = 0x070F; //Character female button pressed
			UO->DrawGump(GumpID, 0, 310, 408);
		}
		else
		{
			if (CreateCharacterManager.SelectedColor == 5)
			{
				glDisable(GL_TEXTURE_2D);

				IFOR(x, 0, 6)
				{
					IFOR(y, 0, 8)
					{
						FColor = 0x44E + (x * 8 + y);

						DWORD clr = ColorManager->GetPolygoneColor(cell, FColor);

						g_GL.DrawPolygone(clr, 490.0f + (x * 20.0f), 140.0f + (y * 35.0f), 20.0f, 35.0f);

						if (g_LastSelectedObject == 40 + (x * 8 + y))
							g_GL.DrawPolygone(0x007F7F7F, 490.0f + ((x * 20.0f) + 9.0f), 140.0f + ((y * 35.0f) + 16.5f), 2.0f, 2.0f);
					}
				}

				glColor3f(1.0f, 1.0f, 1.0f);
				glEnable(GL_TEXTURE_2D);
			}
			else if (CreateCharacterManager.SelectedColor == 0)
			{
				FColor = 0x0481;
				if (CanSelectedButton == 11)
					FColor = 0x0021;
				FontManager->DrawA(9, "Facial Hair Color", FColor, 490, 320);

				glDisable(GL_TEXTURE_2D);

				FColor = CreateCharacterManager.BeardColor;
				if (FColor < 0x044E)
					FColor = 0x044E;
				if (FColor > 0x04AD)
					FColor = 0x04AD;

				DWORD clr = ColorManager->GetPolygoneColor(cell, FColor);

				g_GL.DrawPolygone(clr, 490.0f, 333.0f, 120.0f, 25.0f);

				glColor3f(1.0f, 1.0f, 1.0f);
				glEnable(GL_TEXTURE_2D);
			}

			UO->DrawGump(0x0761, CreateCharacterManager.SkinTone, 238, 98); //Character create male body gump
			UO->DrawGump(0x0739, CreateCharacterManager.ShirtColor, 238, 98); //Character create shirt
			UO->DrawGump(0x0738, CreateCharacterManager.PantsColor, 238, 98); //Character create pants
			UO->DrawGump(0x0762, 0, 238, 98); //Character create shoes

			if (CreateCharacterManager.HairStyle)
				UO->DrawGump(CreateCharacterManager.GetHair(CreateCharacterManager.HairStyle).GumpID, CreateCharacterManager.HairColor, 238, 98); //Character hair
			if (CreateCharacterManager.BeardStyle)
				UO->DrawGump(CreateCharacterManager.GetBeard(CreateCharacterManager.BeardStyle).GumpID, CreateCharacterManager.BeardColor, 238, 98); //Character facial hair

			GumpID = 0x0710 + (int)(CanSelectedButton == 4); //Character male button
			if (CanPressedButton == 4)
				GumpID = 0x0712; //Character male button pressed
			UO->DrawGump(GumpID, 0, 310, 408);
		}

		InitTooltip();

		MouseManager.Draw(0x2073); //Main Gump mouse cursor

		g_GL.EndDraw();
	}
	else
	{
		g_LastSelectedObject = 0;

		if (UO->GumpPixelsInXY(0x1589, 555, 4))
			g_LastSelectedObject = 1; //X gump
		if (UO->GumpPixelsInXY(0x15A1, 586, 445))
			g_LastSelectedObject = 2; //< gump
		if (UO->GumpPixelsInXY(0x15A4, 610, 445))
			g_LastSelectedObject = 3; //> gump
		
		WORD GumpID = 0x0710;
		if (CreateCharacterManager.Sex)
			GumpID = 0x070D;
		if (UO->GumpPixelsInXY(GumpID, 310, 408))
			g_LastSelectedObject = 4; //male / female button

		if (!CreateCharacterManager.Sex)
		{
			if (CreateCharacterManager.SelectedFace != 2)
			{
				if (UO->ResizepicPixelsInXY(0xBB8, 97, 199, 121, 24))
					g_LastSelectedObject = 5; //Facial Hair text field
			}
			else
			{
				IFOR(i, 0, 8)
				{
					RECT rc = {0, 0, 170, 25};
					POINT p = {g_MouseX - 101, g_MouseY - (205 + (i * 14))};
					if (PtInRect(&rc, p))
						g_LastSelectedObject = (20 + i); //Facial Hair text field extended
				}
			}
		}

		if (CreateCharacterManager.SelectedFace != 1)
		{
			if (UO->ResizepicPixelsInXY(0xBB8, 97, 154, 121, 24))
				g_LastSelectedObject = 6; //Hair text field
		}
		else
		{
			int count = 10 + (int)CreateCharacterManager.Sex;

			IFOR(i, 0, count)
			{
				RECT rc = {0, 0, 123, 25};
				POINT p = {g_MouseX - 101, g_MouseY - (159 + (i * 14))};
				if (PtInRect(&rc, p))
					g_LastSelectedObject = (20 + i); //Hair text field extended
			}
		}
		
		if (CreateCharacterManager.SelectedColor == 1)
		{
			RECT rc = {0, 0, 120, 280};
			POINT p = {g_MouseX - 490, g_MouseY - 140};

			if (PtInRect(&rc, p))
			{
				int x = p.x / 15;
				int y = p.y / 35;

				int Index = (x * 8 + y);

				g_LastSelectedObject = 40 + Index;
				WORD ST = 0x03EA + Index;

				if (ST > 0x0422)
					ST = 0x0422;
				CreateCharacterManager.SkinTone = ST;
			}
		}
		else if (CreateCharacterManager.SelectedColor == 0)
		{
			RECT rc = {0, 0, 120, 40};
			POINT p = {g_MouseX - 491, g_MouseY - 139};

			if (PtInRect(&rc, p))
				g_LastSelectedObject = 7;
		}
		
		if (CreateCharacterManager.SelectedColor == 2)
		{
			RECT rc = {0, 0, 120, 240};
			POINT p = {g_MouseX - 492, g_MouseY - 148};

			if (PtInRect(&rc, p))
			{
				int x = p.x / 6;
				int y = p.y / 5;
				
				int Index = (y * 20 + x);

				g_LastSelectedObject = 110 + Index;
				WORD ST = 0;
				
				switch (y % 4)
				{
					case 0:
					{
						ST = 102 + (y / 4);
						break;
					}
					case 1:
					{
						ST = 202 + (y / 4);
						break;
					}
					case 2:
					{
						ST = 402 + (y / 4);
						break;
					}
					case 3:
					{
						ST = 802 + (y / 4);
						break;
					}
					default:
						break;
				}

				if (ST)
					ST += (x * 5);
				
				if (y)
					CreateCharacterManager.ShirtColor = ST;
			}
		}
		else if (CreateCharacterManager.SelectedColor == 0)
		{
			RECT rc = {0, 0, 120, 40};
			POINT p = {g_MouseX - 491, g_MouseY - 184};

			if (PtInRect(&rc, p))
				g_LastSelectedObject = 8;
		}
		
		if (CreateCharacterManager.SelectedColor == 3)
		{
			RECT rc = {0, 0, 120, 240};
			POINT p = {g_MouseX - 492, g_MouseY - 148};

			if (PtInRect(&rc, p))
			{
				int x = p.x / 6;
				int y = p.y / 5;
				
				int Index = (y * 20 + x);

				g_LastSelectedObject = 110 + Index;
				WORD ST = 0;
				
				switch (y % 4)
				{
					case 0:
					{
						ST = 102 + (y / 4);
						break;
					}
					case 1:
					{
						ST = 202 + (y / 4);
						break;
					}
					case 2:
					{
						ST = 402 + (y / 4);
						break;
					}
					case 3:
					{
						ST = 802 + (y / 4);
						break;
					}
					default:
						break;
				}

				if (ST)
					ST += (x * 5);

				if (y)
					CreateCharacterManager.PantsColor = ST;
			}
		}
		else if (CreateCharacterManager.SelectedColor == 0)
		{
			RECT rc = {0, 0, 120, 40};
			POINT p = {g_MouseX - 491, g_MouseY - 229};

			if (PtInRect(&rc, p))
				g_LastSelectedObject = 9;
		}
		
		if (CreateCharacterManager.SelectedColor == 4)
		{
			RECT rc = {0, 0, 120, 280};
			POINT p = {g_MouseX - 490, g_MouseY - 140};

			if (PtInRect(&rc, p))
			{
				int x = p.x / 20;
				int y = p.y / 35;
				int Index = (x * 8 + y);

				g_LastSelectedObject = 40 + Index;

				CreateCharacterManager.HairColor = 0x44E + Index;
			}
		}
		else if (CreateCharacterManager.SelectedColor == 0)
		{
			RECT rc = {0, 0, 120, 40};
			POINT p = {g_MouseX - 491, g_MouseY - 274};

			if (PtInRect(&rc, p))
				g_LastSelectedObject = 10;
		}

		if (!CreateCharacterManager.Sex)
		{
			if (CreateCharacterManager.SelectedColor == 5)
			{
				RECT rc = {0, 0, 120, 280};
				POINT p = {g_MouseX - 490, g_MouseY - 140};

				if (PtInRect(&rc, p))
				{
					int x = p.x / 20;
					int y = p.y / 35;
					int Index = (x * 8 + y);

					g_LastSelectedObject = 40 + Index;

					CreateCharacterManager.BeardColor = 0x44E + Index;
				}
			}
			else if (CreateCharacterManager.SelectedColor == 0)
			{
				RECT rc = {0, 0, 120, 40};
				POINT p = {g_MouseX - 491, g_MouseY - 319};

				if (PtInRect(&rc, p))
					g_LastSelectedObject = 11;
			}
		}

		return g_LastSelectedObject;
	}

	return 0;
}
//---------------------------------------------------------------------------
void TCreateCharacterScreen::OnLeftMouseDown()
{
	Render(false);

	g_LeftMouseDown = true;
	g_LastObjectLeftMouseDown = g_LastSelectedObject;

	if (g_LastSelectedObject == 0)
	{
		if (CreateCharacterManager.SelectedFace != 0)
			CreateCharacterManager.SelectedFace = 0x00;

		return;
	}

	if (!CreateCharacterManager.Sex)
	{
		if (CreateCharacterManager.SelectedFace != 2)
		{
			if (g_LastSelectedObject == 5)
			{
				CreateCharacterManager.SelectedFace = 0x02;
				return;
			}
		}
		else
		{
			IFOR(i, 0, 8)
			{
				if (g_LastSelectedObject == 20  + i)
				{
					CreateCharacterManager.BeardStyle = (BYTE)i;
					CreateCharacterManager.SelectedFace = 0x00;
					return;
				}
			}
		}
	}

	if (CreateCharacterManager.SelectedFace != 1)
	{
		if (g_LastSelectedObject == 6)
		{
			CreateCharacterManager.SelectedFace = 0x01;
			return;
		}
	}
	else
	{
		int count = 10 + (int)CreateCharacterManager.Sex;

		IFOR(i, 0, count)
		{
			if (g_LastSelectedObject == 20  + i)
			{
				CreateCharacterManager.HairStyle = (BYTE)i;
				CreateCharacterManager.SelectedFace = 0x00;
				return;
			}
		}
	}
		
	if (g_LastSelectedObject == 7 || g_LastSelectedObject >= 40)
	{
		if (CreateCharacterManager.SelectedColor == 1) //Skin Tone
		{
			CreateCharacterManager.SelectedColor = 0;
			return;
		}
		else if (CreateCharacterManager.SelectedColor == 0)
			CreateCharacterManager.SelectedColor = 1;
	}
	
	if (g_LastSelectedObject == 8 || g_LastSelectedObject >= 110)
	{
		if (CreateCharacterManager.SelectedColor == 2) //Shirt Color
		{
			CreateCharacterManager.SelectedColor = 0;
			return;
		}
		else if (CreateCharacterManager.SelectedColor == 0)
			CreateCharacterManager.SelectedColor = 2;
	}
	
	if (g_LastSelectedObject == 9 || g_LastSelectedObject >= 110)
	{
		if (CreateCharacterManager.SelectedColor == 3) //Skirt / Pants Color
		{
			CreateCharacterManager.SelectedColor = 0;
			return;
		}
		else if (CreateCharacterManager.SelectedColor == 0)
			CreateCharacterManager.SelectedColor = 3;
	}
	
	if (g_LastSelectedObject == 10 || g_LastSelectedObject >= 40)
	{
		if (CreateCharacterManager.SelectedColor == 4) //Hair Color
		{
			CreateCharacterManager.SelectedColor = 0;
			return;
		}
		else if (CreateCharacterManager.SelectedColor == 0)
			CreateCharacterManager.SelectedColor = 4;
	}

	if (!CreateCharacterManager.Sex)
	{
		if (g_LastSelectedObject == 11 || g_LastSelectedObject >= 40)
		{
			if (CreateCharacterManager.SelectedColor == 5) //Facial Hair Color
				CreateCharacterManager.SelectedColor = 0;
			else if (CreateCharacterManager.SelectedColor == 0)
				CreateCharacterManager.SelectedColor = 5;
		}
	}
}
//---------------------------------------------------------------------------
void TCreateCharacterScreen::OnLeftMouseUp()
{
	Render(false);

	g_LeftMouseDown = false;

	if (g_LastSelectedObject == 0 || g_LastSelectedObject != g_LastObjectLeftMouseDown)
	{
		g_LastObjectLeftMouseDown = 0;
		return;
	}

	if (g_LastObjectLeftMouseDown == 1) //x button
		PostMessage(g_hWnd, WM_CLOSE, 0, 0);
	else if (g_LastObjectLeftMouseDown == 2) //< button
		UO->InitScreen(GS_CHARACTER);
	else if (g_LastObjectLeftMouseDown == 3) //> button
	{
		if (EntryPointer->Length() < 2)
		{
			UO->InitScreen(GS_GAME_CONNECT);
			ConnectionScreen->Type = CST_GAME;
			ConnectionScreen->ConnectionFailed = true;
			ConnectionScreen->ErrorCode = 1;
		}
		else
			UO->InitScreen(GS_SELECT_TOWN);
	}
	else if (g_LastSelectedObject == 4)
		CreateCharacterManager.Sex = !CreateCharacterManager.Sex;

	g_LastObjectLeftMouseDown = 0;
}
//---------------------------------------------------------------------------
void TCreateCharacterScreen::OnCharPress(WPARAM wparam, LPARAM lparam)
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
void TCreateCharacterScreen::OnKeyPress(WPARAM wparam, LPARAM lparam)
{
	if (EntryPointer == NULL)
		EntryPointer = CreateCharacterManager.m_Name;

	EntryPointer->OnKey(NULL, wparam);
}
//---------------------------------------------------------------------------