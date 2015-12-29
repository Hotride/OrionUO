/****************************************************************************
**
** SelectProfessionScreen.cpp
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

TSelectProfessionScreen *SelectProfessionScreen = NULL;
//---------------------------------------------------------------------------
TSelectProfessionScreen::TSelectProfessionScreen()
: TBaseScreen(), m_PixelOffset(0), m_SkillSelection(0), m_LastScrollChangeTime(0)
{
	FontManager->GenerateA(1, m_TextStat[0], "Strength", 1);
	FontManager->GenerateA(1, m_TextStat[1], "Dexterity", 1);
	FontManager->GenerateA(1, m_TextStat[2], "Intelligence", 1);
	
	IFOR(i, 0, g_SkillsCount)
	{
		int skillIndex = g_SkillSort.m_Skills[i];
		FontManager->GenerateA(9, m_TextSkillInList[i][0], g_Skills[skillIndex].m_Name.c_str(), 1);
		FontManager->GenerateA(9, m_TextSkillInList[i][1], g_Skills[skillIndex].m_Name.c_str(), 0x0035);
	}
}
//---------------------------------------------------------------------------
TSelectProfessionScreen::~TSelectProfessionScreen()
{
	IFOR(i, 0, 3)
		m_TextStat[i].Clear();

	IFOR(i, 0, g_SkillsCount)
	{
		m_TextSkillInList[i][0].Clear();
		m_TextSkillInList[i][1].Clear();
	}
}
//---------------------------------------------------------------------------
void TSelectProfessionScreen::Init()
{
	UsedProfessionCategory = Profession;
	UsedProfession = NULL;
	m_PixelOffset = 0;
	m_SkillSelection = 0;
	m_LastScrollChangeTime = 0;

	if (g_UseSmoothMonitor)
	{
		g_SmoothMonitorMode = SMOOTH_MONITOR_NONE;
		g_SmoothMonitorColor = 1.0f;
		//g_SmoothMonitorMode = SMOOTH_MONITOR_SUNRISE;
		//g_SmoothMonitorColor = 0.0f;
		g_SmoothMonitorStep = (GLfloat)g_SmoothMonitorScale * 0.01f;
		m_SmoothScreenAction = 0;
	}
	else
		g_SmoothMonitorMode = SMOOTH_MONITOR_NONE;

	Tooltip.SeqIndex = 0;

	//Prepare textures:
	UO->ExecuteGump(0x0588); //Main Screen background
	UO->ExecuteGump(0x157C); //Main Screen
	UO->ExecuteGump(0x15A0); //Main Screen Notes
	UO->ExecuteGumpPart(0x1589, 3); //X gump
	UO->ExecuteGumpPart(0x15A1, 3); //< gump
	UO->ExecuteGumpPart(0x15A4, 3); //> gump
	UO->ExecuteGump(0x058B); //Create character header
	UO->ExecuteGump(0x0589); //Label container
	UO->ExecuteGumpPart(0x119C, 3); //Arrow <
	UO->ExecuteResizepic(0x0BB8); //Description text field
	UO->ExecuteResizepic(0x0A28); //Character profeccion selection field
	UO->ExecuteResizepic(0x00FA); //scroll bar
	UO->ExecuteGumpPart(0x00D5, 3); //Sphere line gump

	UO->ExecuteGump(0x0589); //Earth container
	UO->ExecuteGumpPart(0x15E8, 3); //Earth button
}
//---------------------------------------------------------------------------
void TSelectProfessionScreen::InitTooltip()
{
	if (!ConfigManager.UseToolTips)
		return;

}
//---------------------------------------------------------------------------
int TSelectProfessionScreen::Render(bool mode)
{
	DWORD ticks = GetTickCount();

	if (g_LastRenderTime > ticks)
	{
		if (mode || !g_SelectGumpObjects)
			return 0;
	}

	int CanSelectedButton = g_LastSelectedObject;

	int CanPressedButton = 0;
	if (g_LeftMouseDown && g_LastObjectLeftMouseDown == g_LastSelectedObject)
		CanPressedButton = g_LastObjectLeftMouseDown;
	
	//Проверим, вдруг необходимо изменить положение отображаемого элемента
	if (g_LeftMouseDown && m_LastScrollChangeTime < ticks)
	{
		if (g_LastObjectLeftMouseDown == ID_SPS_SCROLLBAR_UP) //Скроллинг вверх (гампом-стрелкой)
			ListingList(true, 10);
		else if (g_LastObjectLeftMouseDown == ID_SPS_SCROLLBAR_DOWN) //Скроллинг вниз (гампом-стрелкой)
			ListingList(false, 10);
	}

	int scrollerY = 0;

	int heightToScrolling = 0;

	bool canMoveScroller = true;

	int curHeight = GetScrollBoxHeight();

	if (curHeight < 200)
		canMoveScroller = false;
	else
		heightToScrolling = curHeight - 200;
	
	int maxScrollerY = 147;

	int visibleLines = heightToScrolling / GUMP_SCROLLING_PIXEL_STEP;

	if (g_LeftMouseDown && g_LastObjectLeftMouseDown == ID_SPS_SCROLLBAR && canMoveScroller) //Scroller pressed
	{
		int currentY = (g_MouseY - 25) - 146; //Scroller position

		scrollerY = CalculateScrollerAndTextPosition(m_PixelOffset, visibleLines, maxScrollerY, currentY);
	}
	else if (m_PixelOffset && canMoveScroller)
		scrollerY = CalculateScrollerY(m_PixelOffset, visibleLines, maxScrollerY);

	if (mode)
	{
		g_LastRenderTime = ticks + g_FrameDelay;

		g_GL.BeginDraw();
		
		UO->DrawGump(0x0588, 0, 0, 0, 640, 480); //Main Gump background
		UO->DrawGump(0x157C, 0, 0, 0); //Main Gump
		UO->DrawGump(0x15A0, 0, 0, 4); //Main Gump Notes
		
		WORD GumpID = 0x1589 + (int)(CanSelectedButton == ID_SPS_QUIT); //X gump /lighted
		if (CanPressedButton == ID_SPS_QUIT)
			GumpID = 0x158B; //X gump (pressed)
		UO->DrawGump(GumpID, 0, 555, 4);
		
		GumpID = 0x15A1 + (int)(CanSelectedButton == ID_SPS_ARROW_PREV); //< gump /lighted
		if (CanPressedButton == ID_SPS_ARROW_PREV)
			GumpID = 0x15A3; //< gump pressed
		UO->DrawGump(GumpID, 0, 586, 445); //< gump

		UO->DrawResizepicGump(0xA28, 80, 80, 546, 352); //Character profeccion selection field
		
		UO->DrawGump(0x058B, 0, 145, 57); //Create character header
		UO->DrawGump(0x0589, 0, 222, 44); //Label container
		
		GumpID = 0x119C + (int)(CanSelectedButton == ID_SPS_ARROW_BACK_PROFESSION); //Arrow < /lighted
		if (CanPressedButton == ID_SPS_ARROW_BACK_PROFESSION)
			GumpID = 0x119E; //Arrow < pressed
		UO->DrawGump(GumpID, 0, 200, 356); //Arrow <
		


		UO->DrawResizepicGump(0xBB8, 120, 137, 204, 214); //Description text field
		
		//Description scroll bar:
		UO->DrawGump(0x0100, 0, 324, 149, 0, 190); //background
		UO->DrawGump(0x00FA, 0, 324, 137); //^
		UO->DrawGump(0x00FE, 0, 325, 158 + scrollerY); //bar
		UO->DrawGump(0x00FC, 0, 324, 330); //v
		
		//Text in box
		g_GL.ViewPort(123, 140, 195, 206);

		int ofsY = -(m_PixelOffset * GUMP_SCROLLING_PIXEL_STEP);

		if (!m_SkillSelection)
		{
			if (UsedProfessionCategory != NULL)
				UsedProfessionCategory->m_TextureDescription.Draw(123, 140 + ofsY);
			else if (UsedProfession != NULL)
				UsedProfession->m_TextureDescription.Draw(123, 140 + ofsY);
		}
		else
		{
			int yPtr = 140 + ofsY;

			IFOR(i, 0, g_SkillsCount)
			{
				int selected = (int)(g_LastSelectedObject == ID_SPS_SKILLS_LIST + i);
				m_TextSkillInList[i][selected].Draw(123, yPtr);

				yPtr += m_TextSkillInList[i][0].Height;
			}
		}

		g_GL.RestorePort();
		


		if (UsedProfessionCategory != NULL) //category
		{
			GumpID = UsedProfessionCategory->Gump;
			UO->DrawGump(GumpID, 0, 231, 53); //Label gump

			int offsY = 0;

			if (UsedProfessionCategory->m_CatChildCount > 0)
			{
				for (int i = 0; i < UsedProfessionCategory->m_CatChildCount; i++)
				{
					UO->DrawGump(0x0589, 0, 500, 100 + offsY); //Label container

					GumpID = UsedProfessionCategory->m_CatChild[i]->Gump;
					if (CanPressedButton == ID_SPS_LABEL + i)
						GumpID++;

					UO->DrawGump(GumpID, 0, 509, 109 + offsY); //Label gump
					FontManager->DrawA(9, UsedProfessionCategory->m_CatChild[i]->GetName().c_str(), 0x1, 350, 135 + offsY);

					offsY += 79;
				}
			}

			if (UsedProfessionCategory->m_ProfChildCount > 0)
			{
				for (int i = 0; i < UsedProfessionCategory->m_ProfChildCount; i++)
				{
					UO->DrawGump(0x0589, 0, 500, 100 + offsY); //Label container

					GumpID = UsedProfessionCategory->m_ProfChild[i]->Gump;
					if (CanPressedButton == ID_SPS_LABEL + i + UsedProfessionCategory->m_CatChildCount)
						GumpID++;

					UO->DrawGump(GumpID, 0, 509, 109 + offsY); //Label gump
					FontManager->DrawA(9, UsedProfessionCategory->m_ProfChild[i]->GetName().c_str(), 0x1, 350, 135 + offsY);

					offsY += 79;
				}
			}
		}
		else if (UsedProfession != NULL) //profession
		{
			UO->DrawGump(UsedProfession->Gump, 0, 231, 53); //Label gump
			
			const float SphereListWidth = 95.0f;
			float ValPer = 0.0f;
			
			//Stats
			if (g_LastObjectLeftMouseDown >= ID_SPS_STATS_SPHERE && g_LastObjectLeftMouseDown <= ID_SPS_STATS_SPHERE + 2)
				ShuffleStats();

			//Skills
			if (g_LastObjectLeftMouseDown >= ID_SPS_SKILLS_SPHERE && g_LastObjectLeftMouseDown <= ID_SPS_SKILLS_SPHERE + 2)
				ShuffleSkills();

			char val[15] = {0};
			int statVal[3] = {UsedProfession->Str, UsedProfession->Dex, UsedProfession->Int};

			int yPtr = 136;

			IFOR(i, 0, 3)
			{
				m_TextStat[i].Draw(360, yPtr);

				sprintf(val, "%d", statVal[i]);
				FontManager->DrawA(1, val, 1, 460, yPtr);

				UO->DrawSphereGump((statVal[i] - 10), 35.0f, 436, yPtr);

				yPtr += 30;
			}

			if (UsedProfession->Index >= 0)
			{
				yPtr = 260;

				IFOR(i, 0, 3)
				{
					int skillID = UsedProfession->GetSkillID(i);

					if (skillID >= g_SkillsCount)
						skillID = 0;

					FontManager->DrawA(1, g_Skills[skillID].m_Name.c_str(), 1, 360, yPtr, 90, TS_LEFT, UOFONT_FIXED);

					sprintf(val, "%d", UsedProfession->GetSkillValue(i));
					FontManager->DrawA(1, val, 1, 460, yPtr);

					yPtr += 32;
				}
			}
			else //advanced
			{
				yPtr = 256;

				IFOR(i, 0, 3)
				{
					UO->DrawResizepicGump(0xBB8, 350, yPtr, 105, 25); //Skill Name text field

					int skillID = UsedProfession->GetSkillID(i);

					WORD textColor = 1;
					
					if (m_SkillSelection && m_SkillSelection - 1 == i)
						textColor = 0x0021;
					else if (g_LastSelectedObject == ID_SPS_SKILLS_FILED + i)
						textColor = 0x0386;
					
					if (skillID >= g_SkillsCount)
						FontManager->DrawA(9, "Click here", textColor, 354, yPtr + 5);
					else
						FontManager->DrawA(9, g_Skills[skillID].m_Name.c_str(), textColor, 354, yPtr + 5, 90, TS_LEFT, UOFONT_FIXED);

					sprintf(val, "%d", UsedProfession->GetSkillValue(i));

					FontManager->DrawA(1, val, 1, 460, yPtr + 4);

					yPtr += 32;
				}
			}

			//Skills
			UO->DrawSphereGump(UsedProfession->GetSkillValue(0), 50.0f, 436, 258);
			UO->DrawSphereGump(UsedProfession->GetSkillValue(1), 50.0f, 436, 290);
			UO->DrawSphereGump(UsedProfession->GetSkillValue(2), 50.0f, 436, 322);

			GumpID = 0x15A4 + (int)(CanSelectedButton == ID_SPS_ARROW_NEXT);
			if (CanPressedButton == ID_SPS_ARROW_NEXT)
				GumpID = 0x15A6; //> gump pressed
			UO->DrawGump(GumpID, 0, 610, 445); //> gump
		}

		InitTooltip();

		MouseManager.Draw(0x2073); //Main Gump mouse cursor

		g_GL.EndDraw();
	}
	else
	{
		g_LastSelectedObject = 0;

		if (UO->GumpPixelsInXY(0x1589, 555, 4))
			g_LastSelectedObject = ID_SPS_QUIT; //X gump
		else if (UO->GumpPixelsInXY(0x15A1, 586, 445))
			g_LastSelectedObject = ID_SPS_ARROW_PREV; //< gump
		else if (UO->GumpPixelsInXY(0x119C, 200, 356))
			g_LastSelectedObject = ID_SPS_ARROW_BACK_PROFESSION; //Arrow <
		else if (UO->GumpPixelsInXY(0x00FA, 324, 137))
			g_LastSelectedObject = ID_SPS_SCROLLBAR_UP; //^
		else if (UO->GumpPixelsInXY(0x00FC, 324, 330))
			g_LastSelectedObject = ID_SPS_SCROLLBAR_DOWN; //v
		else if (UO->GumpPixelsInXY(0x00FE, 325, 158 + scrollerY))
			g_LastSelectedObject = ID_SPS_SCROLLBAR; //bar
		else if (UO->GumpPixelsInXY(0x0100, 324, 149, 0, 190))
			g_LastSelectedObject = ID_SPS_SCROLLBAR_BACKGROUND; //background
		else if (UsedProfessionCategory != NULL)
		{
			if (UO->GumpPixelsInXY(UsedProfessionCategory->Gump, 231, 53))
				g_LastSelectedObject = ID_SPS_LABEL_BACK_PROFESSION; //Label gump

			int offsY = 0;
			if (UsedProfessionCategory->m_CatChildCount > 0)
			{
				int count = UsedProfessionCategory->m_CatChildCount;

				IFOR(i, 0, count)
				{
					if (UO->GumpPixelsInXY(UsedProfessionCategory->m_CatChild[i]->Gump, 509, 109 + offsY))
						g_LastSelectedObject = ID_SPS_LABEL + i; //Label gump

					offsY += 79;
				}
			}

			if (UsedProfessionCategory->m_ProfChildCount > 0)
			{
				int count = UsedProfessionCategory->m_ProfChildCount;

				IFOR(i, 0, count)
				{
					if (UO->GumpPixelsInXY(UsedProfessionCategory->m_ProfChild[i]->Gump, 509, 109 + offsY))
						g_LastSelectedObject = ID_SPS_LABEL + i + UsedProfessionCategory->m_CatChildCount; //Label gump

					offsY += 79;
				}
			}
		}
		else if (UsedProfession != NULL)
		{
			if (UO->GumpPixelsInXY(UsedProfession->Gump, 231, 53))
				g_LastSelectedObject = ID_SPS_LABEL_BACK_PROFESSION; //Label gump
			else if (UO->GumpPixelsInXY(0x15A4, 610, 445))
				g_LastSelectedObject = ID_SPS_ARROW_NEXT; //> gump
			else
			{
				const int sphereListWidth = 95;
			
				int statVal[3] = {UsedProfession->Str, UsedProfession->Dex, UsedProfession->Int};
				int yPtr = 136;

				IFOR(i, 0, 3)
				{
					int ofs = CalculateSphereOffset(100, (statVal[i] - 10), sphereListWidth, 35.0f);
					if (UO->GumpPixelsInXY(0x00D8, 500 + ofs, yPtr)) //Sphere gump
						g_LastSelectedObject = ID_SPS_STATS_SPHERE + i;

					yPtr += 30;
				}
			
				yPtr = 258;

				IFOR(i, 0, 3)
				{
					int ofs = CalculateSphereOffset(100, UsedProfession->GetSkillValue(i), sphereListWidth, 50.0f);
					if (UO->GumpPixelsInXY(0x00D8, 500 + ofs, yPtr)) //Sphere gump
						g_LastSelectedObject = ID_SPS_SKILLS_SPHERE + i;

					yPtr += 32;
				}
				
				if (UsedProfession->Index == -1) //advanced
				{
					yPtr = 256;

					IFOR(i, 0, 3)
					{
						if (UO->ResizepicPixelsInXY(0xBB8, 350, yPtr, 105, 25)) //Skill Name text field
							g_LastSelectedObject = ID_SPS_SKILLS_FILED + i;

						yPtr += 32;
					}

					if (m_SkillSelection && UO->PolygonePixelsInXY(123, 140, 195, 206))
					{
						int ofsY = -(m_PixelOffset * GUMP_SCROLLING_PIXEL_STEP);

						yPtr = 140 + ofsY;

						IFOR(i, 0, g_SkillsCount)
						{
							int tw = 195; //m_TextSkillInList[i][0].Width;
							int th = m_TextSkillInList[i][0].Height;

							if (UO->PolygonePixelsInXY(123, yPtr, tw, th))
								g_LastSelectedObject = ID_SPS_SKILLS_LIST + i;

							yPtr += m_TextSkillInList[i][0].Height;
						}
					}
				}
			}
		}

		return g_LastSelectedObject;
	}

	return 0;
}
//---------------------------------------------------------------------------
void TSelectProfessionScreen::OnLeftMouseDown()
{
	Render(false);

	g_LeftMouseDown = true;
	g_LastObjectLeftMouseDown = g_LastSelectedObject;

	if (g_LastSelectedObject == ID_SPS_SCROLLBAR_BACKGROUND) //Scrollbar background click
	{
		int drawY = 146;

		int heightToScrolling = GetScrollBoxHeight() - 200;

		if (heightToScrolling > 0)
		{
			int maxScrollerY = 147;

			int visibleLines = heightToScrolling / GUMP_SCROLLING_PIXEL_STEP;

			int jsize = visibleLines;

			float ValPer = (m_PixelOffset / (float)jsize) * 100.0f;

			jsize = (int)((jsize * ValPer) / 100.0f);

			if (jsize < 1)
				jsize = 1;
			else if (jsize > visibleLines)
				jsize = visibleLines;
			
			if (jsize >= visibleLines)
				drawY += maxScrollerY;
			else
				drawY += (int)((maxScrollerY * ValPer) / 100.0f);
		}

		if (g_MouseY < drawY) //^
			g_LastSelectedObject = ID_SPS_SCROLLBAR_UP;
		else if (g_MouseY > drawY + 25) //v
			g_LastSelectedObject = ID_SPS_SCROLLBAR_DOWN;
	}
}
//---------------------------------------------------------------------------
void TSelectProfessionScreen::OnLeftMouseUp()
{
	Render(false);
	g_LeftMouseDown = false;

	if (g_LastSelectedObject == 0 || g_LastSelectedObject != g_LastObjectLeftMouseDown)
	{
		g_LastObjectLeftMouseDown = 0;
		return;
	}

	if (g_LastObjectLeftMouseDown == ID_SPS_QUIT) //x button
		PostMessage(g_hWnd, WM_CLOSE, 0, 0);
	else if (g_LastObjectLeftMouseDown == ID_SPS_ARROW_PREV) //< button
		UO->InitScreen(GS_CHARACTER);
	else if (g_LastObjectLeftMouseDown == ID_SPS_ARROW_NEXT) //> button
	{
		if (UsedProfession != NULL)
		{
			bool passed = true;

			if (UsedProfession->Index == -1) //Advanced
			{
				IFOR(i, 0, 3)
				{
					IFOR(j, 0, 3)
					{
						if (i != j)
						{
							if (UsedProfession->GetSkillID(i) == 0xFF || UsedProfession->GetSkillID(i) == UsedProfession->GetSkillID(j))
							{
								passed = false;
								
								UO->InitScreen(GS_GAME_CONNECT);
								ConnectionScreen->Type = CST_SELECT_PROFESSOIN;

								break;
							}
						}
					}
				}
			}

			if (passed)
				UO->InitScreen(GS_CREATE);
		}
	}
	else if (g_LastObjectLeftMouseDown == ID_SPS_ARROW_BACK_PROFESSION || g_LastObjectLeftMouseDown == ID_SPS_LABEL_BACK_PROFESSION) //Arrow < or General Label gump
	{
		if (UsedProfessionCategory != NULL)
		{
			if (UsedProfessionCategory->m_Parent != NULL)
			{
				UsedProfessionCategory = UsedProfessionCategory->m_Parent;
				UsedProfession = NULL;
				g_LastObjectLeftMouseDown = 0;
				m_PixelOffset = 0;
				m_SkillSelection = 0;

				return;
			}
		}
		else if (UsedProfession != NULL)
		{
			if (UsedProfession->m_Parent != NULL)
			{
				UsedProfessionCategory = UsedProfession->m_Parent;
				UsedProfession = NULL;
				g_LastObjectLeftMouseDown = 0;
				m_PixelOffset = 0;
				m_SkillSelection = 0;

				return;
			}
		}

		UsedProfessionCategory = Profession;
	}
	else if (UsedProfessionCategory != NULL)
	{
		if (UsedProfessionCategory->m_CatChildCount > 0)
		{
			int count = UsedProfessionCategory->m_CatChildCount;

			IFOR(i, 0, count)
			{
				if (g_LastObjectLeftMouseDown == ID_SPS_LABEL + i)
				{
					UsedProfessionCategory = UsedProfessionCategory->m_CatChild[i];
					UsedProfession = NULL;
					g_LastObjectLeftMouseDown = 0;
					m_PixelOffset = 0;
					m_SkillSelection = 0;

					return;
				}
			}
		}

		if (UsedProfessionCategory->m_ProfChildCount > 0)
		{
			int count = UsedProfessionCategory->m_ProfChildCount;

			IFOR(i, 0, count)
			{
				if (g_LastObjectLeftMouseDown == ID_SPS_LABEL + i + UsedProfessionCategory->m_CatChildCount)
				{
					UsedProfession = UsedProfessionCategory->m_ProfChild[i];
					UsedProfessionCategory = NULL;
					m_PixelOffset = 0;
					m_SkillSelection = 0;

					break;
				}
			}
		}
	}
	else if (UsedProfession != NULL)
	{
		if (g_LastObjectLeftMouseDown >= ID_SPS_SKILLS_LIST)
		{
			m_SkillSelection--;
			int index = g_LastObjectLeftMouseDown - ID_SPS_SKILLS_LIST;
			index = g_SkillSort.m_Skills[index];

			UsedProfession->SetSkillID(m_SkillSelection, index);

			m_SkillSelection = 0;
		}
		else
		{
			IFOR(i, 0, 3)
			{
				if (g_LastObjectLeftMouseDown == ID_SPS_SKILLS_FILED + i)
				{
					m_SkillSelection = i + 1;
					m_PixelOffset = 0;

					break;
				}
			}
		}
	}
	
	if (m_LastScrollChangeTime < GetTickCount())
	{
		if (g_LastObjectLeftMouseDown == ID_SPS_SCROLLBAR_UP)
			ListingList(true);
		else if (g_LastObjectLeftMouseDown == ID_SPS_SCROLLBAR_DOWN)
			ListingList(false);
	}

	g_LastObjectLeftMouseDown = 0;
}
//----------------------------------------------------------------------------
void TSelectProfessionScreen::OnMouseWheel(MOUSE_WHEEL_STATE state)
{
	if (!g_LeftMouseDown && !g_RightMouseDown && m_LastScrollChangeTime < GetTickCount())
	{
		if (g_MouseX >= 123 && g_MouseX < 318 && g_MouseY >= 140 && g_MouseY < 346)
		{
			if (state == MWS_UP)
				ListingList(true, 25);
			else if (state == MWS_DOWN)
				ListingList(false, 25);
		}
	}
}
//----------------------------------------------------------------------------
void TSelectProfessionScreen::ListingList(bool direction, int divizor)
{
	if (direction) //Up
	{
		if (m_PixelOffset > 1)
			m_PixelOffset--;
		else
			m_PixelOffset = 0;
	}
	else //Down
	{
		int maxidx = (GetScrollBoxHeight() - 200) / GUMP_SCROLLING_PIXEL_STEP;

		if (maxidx < 0)
			maxidx = 0;

		if (m_PixelOffset < maxidx)
			m_PixelOffset++;
		else
			m_PixelOffset = maxidx;
	}

	m_LastScrollChangeTime = GetTickCount() + (SCROLL_LISTING_DELAY / divizor);
}
//----------------------------------------------------------------------------
int TSelectProfessionScreen::GetScrollBoxHeight()
{
	int result = 10;

	if (m_SkillSelection)
	{
		IFOR(i, 0, g_SkillsCount)
			result += m_TextSkillInList[i][0].Height;
	}
	else
	{
		if (UsedProfessionCategory != NULL)
			result += UsedProfessionCategory->m_TextureDescription.Height;
		else if (UsedProfession != NULL)
			result += UsedProfession->m_TextureDescription.Height;
	}

	return result;
}
//---------------------------------------------------------------------------
void TSelectProfessionScreen::ShuffleStats()
{
	int MX = g_MouseX - 500;

	if (MX < 0)
		MX = 0;

	if (MX > 95)
		MX = 95;

	float ValPer = MX / 95.0f * 100.0f;
	ValPer = (35.0f * ValPer) / 100.0f;

	if (ValPer < 0.0f)
		ValPer = 0.0f;

	int stats[3] = {UsedProfession->Str, UsedProfession->Dex, UsedProfession->Int};

	int used_stat = g_LastObjectLeftMouseDown - ID_SPS_STATS_SPHERE;
	int others_stat[2] = {0};

	others_stat[0] = 0;
	if (others_stat[0] == used_stat)
		others_stat[0]++;

	others_stat[1] = others_stat[0] + 1;
	if (others_stat[1] == used_stat)
		others_stat[1]++;

	stats[used_stat] = 10 + (int)ValPer;

	int stat_sum = 65 - (stats[0] + stats[1] + stats[2]);

	if (stat_sum > 0) //stat will decrease
	{
		while (stat_sum > 0)
		{
			if (stats[others_stat[0]] < 45)
			{
				stat_sum--;
				stats[others_stat[0]]++;
			}
			else if (stats[others_stat[1]] < 45)
			{
				stat_sum--;
				stats[others_stat[1]]++;
			}
			else
				break;
		}
	}
	else if (stat_sum < 0) //stat will increase
	{
		while (stat_sum < 0)
		{
			if (stats[others_stat[0]] > 10)
			{
				stat_sum++;
				stats[others_stat[0]]--;
			}
			else if (stats[others_stat[1]] > 10)
			{
				stat_sum++;
				stats[others_stat[1]]--;
			}
			else
				break;
		}
	}

	UsedProfession->Str = stats[0];
	UsedProfession->Dex = stats[1];
	UsedProfession->Int = stats[2];
}
//---------------------------------------------------------------------------
void TSelectProfessionScreen::ShuffleSkills()
{
	int MX = g_MouseX - 500;

	if (MX < 0)
		MX = 0;

	if (MX > 95)
		MX = 95;

	float ValPer = (MX / 95.0f) * 100.0f;
	ValPer = (50.0f * ValPer) / 100.0f;

	if (ValPer < 0)
		ValPer = 0;
	
	int skills[3] = {UsedProfession->GetSkillValue(0), UsedProfession->GetSkillValue(1), UsedProfession->GetSkillValue(2)};

	int used_skill = g_LastObjectLeftMouseDown - ID_SPS_SKILLS_SPHERE;
	int others_skills[2] = {0};

	others_skills[0] = 0;
	if (others_skills[0] == used_skill)
		others_skills[0]++;

	others_skills[1] = others_skills[0] + 1;
	if (others_skills[1] == used_skill)
		others_skills[1]++;

	skills[used_skill] = (int)ValPer;

	int skills_sum = 100 - (skills[0] + skills[1] + skills[2]);
	
	if (skills_sum > 0) //skill will decrease
	{
		while (skills_sum > 0)
		{
			if (skills[others_skills[0]] < 50)
			{
				skills_sum--;
				skills[others_skills[0]]++;
			}
			else if (skills[others_skills[1]] < 50)
			{
				skills_sum--;
				skills[others_skills[1]]++;
			}
			else
				break;
		}
	}
	else if (skills_sum < 0) //skill will increase
	{
		while (skills_sum < 0)
		{
			if (skills[others_skills[0]] > 0)
			{
				skills_sum++;
				skills[others_skills[0]]--;
			}
			else if (skills[others_skills[1]] > 0)
			{
				skills_sum++;
				skills[others_skills[1]]--;
			}
			else
				break;
		}
	}

	UsedProfession->SetSkillValue(0, skills[0]);
	UsedProfession->SetSkillValue(1, skills[1]);
	UsedProfession->SetSkillValue(2, skills[2]);
}
//---------------------------------------------------------------------------