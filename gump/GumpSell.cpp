/****************************************************************************
**
** SellGump.cpp
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
//----------------------------------------------------------------------------
#include "stdafx.h"
//---------------------------------------------------------------------------
TGumpSell::TGumpSell(DWORD serial, short x, short y, int count)
: TGump(GT_SHOP, serial, x, y), m_Count(count), m_LastChangedLineTime(0),
m_LastChangeCountTime(0), m_StartChangeCountTime(0), m_SelectedLine1(0),
m_SelectedLine2(0), m_SelectedListCount(0), m_ContinueCounter(0)
{
}
//---------------------------------------------------------------------------
TGumpSell::~TGumpSell()
{
}
//----------------------------------------------------------------------------
void TGumpSell::PrepareTextures()
{
	Orion->ExecuteGumpPart(0x0872, 2);
	Orion->ExecuteGumpPart(0x0039, 3);
	Orion->ExecuteGump(0x0828);
	Orion->ExecuteGumpPart(0x0037, 2);

	QFOR(go, m_Items, TGumpSellObject*)
		Orion->ExecuteStaticArt(go->Graphic);
}
//----------------------------------------------------------------------------
void TGumpSell::CalculateSelectedListCount()
{
	m_SelectedListCount = 0;

	QFOR(go, m_Items, TGumpSellObject*)
	{
		if (go->SelectedCount)
			m_SelectedListCount++;
	}
}
//---------------------------------------------------------------------------
void TGumpSell::GenerateFrame()
{
	if (!g_DrawMode)
	{
		m_FrameRedraw = false;
		m_FrameCreated = false;

		return;
	}

	if (m_Items == NULL)
		return;

	TGameCharacter *vendor = World->FindWorldCharacter(m_Serial);
	if (vendor == NULL)
		return; //Объект, к которому привязан гамп - исчез

	CalculateSelectedListCount(); //Вычисление размеров листа выбранных объектов

	if (!m_NoClose)
		m_ContinueCounter = 0;

	CalculateGumpState();

	int scrollerY1 = 0;
	int scrollerY2 = 0;

	int visibleLines = m_Count - 1;
	int maxScrollerY = 91;

	if (g_GumpPressedScroller && g_LastObjectLeftMouseDown == ID_GS_SCROLL_1) //Scroller 1 pressed
	{
		int currentY = (g_MouseY - 10) - (int)g_GumpTranslateY - 66; //Scroller 1 position

		scrollerY1 = CalculateScrollerAndTextPosition(m_SelectedLine1, visibleLines, maxScrollerY, currentY);
	}
	else if (m_SelectedLine1)
		scrollerY1 = CalculateScrollerY(m_SelectedLine1, visibleLines, maxScrollerY);
	
	visibleLines = m_SelectedListCount - 1;
	maxScrollerY = 57;

	if (g_GumpPressedScroller && g_LastObjectLeftMouseDown == ID_GS_SCROLL_2) //Scroller 2 pressed
	{
		int currentY = (g_MouseY - 10) - (int)g_GumpTranslateY - 280; //Scroller 2 position

		scrollerY2 = CalculateScrollerAndTextPosition(m_SelectedLine2, visibleLines, maxScrollerY, currentY);
	}
	else if (m_SelectedLine2)
		scrollerY2 = CalculateScrollerY(m_SelectedLine2, visibleLines, maxScrollerY);

	glNewList((GLuint)this, GL_COMPILE);

		Orion->DrawGump(0x0872, 0, 0, 0);

		//My Inventory

		g_GL.Scissor((int)g_GumpTranslateX + 30, (int)g_GumpTranslateY + 60, 200, 176);

		int drawY = 60;
			
		TTextureObject *thDelim1 = Orion->ExecuteGump(0x0039);
		TTextureObject *thDelim2 = Orion->ExecuteGump(0x003A);
		TTextureObject *thDelim3 = Orion->ExecuteGump(0x003B);
		
		int delim1Height = 20;
		int delim1Width = 30;
		int delim1posX = 32;
		int delim2posX = 62;
		int delim3posX = 30;
		int delimLineWidth = 140;

		TGumpSellObject *go = (TGumpSellObject*)m_Items;

		if (thDelim1 == NULL || thDelim2 == NULL || thDelim3 == NULL)
		{
			//TPRINT("Error, Delimiter line gump not found!\n");
		}
		else
		{
			delim1Height = thDelim1->Height;
			delim1Width = thDelim1->Width;
			delim2posX = 32 + delim1Width;
			delim3posX = 226 - thDelim3->Width;
			delimLineWidth = 200 - (thDelim1->Width + thDelim3->Width);
		}

		int currentIndex = 0;
		int startIndex = m_SelectedLine1;

		int drawYBounds = 236;

		while (go != NULL)
		{
			if (currentIndex < startIndex)
			{
				currentIndex++;
				go = (TGumpSellObject*)go->m_Next;

				continue;
			}

			if (drawY >= drawYBounds)
				break;

			TTextureObject *th = Orion->ExecuteStaticArt(go->Graphic);

			if (th != NULL)
			{
				WORD textColor = 0x021F;
				if (go->Selected)
					textColor = 0x0021;

				TTextTexture tth;
				string str = go->GetName() + " at " + std::to_string(go->Price) + "gp";
				FontManager->GenerateA(9, tth, str.c_str(), textColor, 90);

				if (!tth.Empty())
				{
					int imageOffsetY = 0;
					int textOffsetY = 0;

					int maxOffset = th->Height;

					if (maxOffset < tth.Height)
					{
						imageOffsetY = ((tth.Height - maxOffset) / 2);
						maxOffset = tth.Height;
					}
					else
						textOffsetY = ((maxOffset - tth.Height) / 2);

					tth.Draw(82, drawY + textOffsetY);
					tth.Clear();
					
					str = std::to_string(go->Count - go->SelectedCount);
					int amountWidth = FontManager->GetWidthA(9, str.c_str(), str.length());
					FontManager->GenerateA(9, tth, str.c_str(), textColor, amountWidth);

					if (!tth.Empty())
					{
						tth.Draw(226 - amountWidth, drawY + (maxOffset / 2) - (tth.Height / 2));
						tth.Clear();
					}

					ColorizerShader->Use();

					Orion->DrawStaticArtInContainer(go->Graphic, go->Color, delim1posX, drawY + imageOffsetY);

					UnuseShader();
					
					drawY += maxOffset;
					
					Orion->DrawGump(0x0039, 0, delim1posX, drawY);
					Orion->DrawGump(0x003A, 0, delim2posX, drawY, delimLineWidth, 0);
					Orion->DrawGump(0x003B, 0, delim3posX, drawY);

					drawY += delim1Height;
				}
			}

			currentIndex++;

			go = (TGumpSellObject*)go->m_Next;
		}

		glDisable(GL_SCISSOR_TEST);
		
		//Up
		//UO->DrawGump(0x0828, 0, posX + 237, posY + 66); //Scroller 1 min
		//Down
		//UO->DrawGump(0x0828, 0, posX + 237, posY + 157); //Scroller 1 max
			
		Orion->DrawGump(0x0828, 0, 237, 66 + scrollerY1); //Scroller 1

		//Offer

		Orion->DrawGump(0x0873, 0, 170, 214);
			
		g_GL.Scissor((int)g_GumpTranslateX + 200, (int)g_GumpTranslateY + 280, 200, 92);

		drawY = 282;

		currentIndex = 0;
		startIndex = m_SelectedLine2;

		drawYBounds = 372;

		int totalPrice = 0;

		go = (TGumpSellObject*)m_Items;

		while (go != NULL)
		{
			if (!go->SelectedCount)
			{
				go = (TGumpSellObject*)go->m_Next;

				continue;
			}

			totalPrice += (go->SelectedCount * (int)go->Price);
				
			if (currentIndex < startIndex)
			{
				currentIndex++;
				go = (TGumpSellObject*)go->m_Next;

				continue;
			}

			if (drawY >= drawYBounds)
			{
				go = (TGumpSellObject*)go->m_Next;
				continue;
			}

			WORD textColor = 0x021F;

			TTextTexture tth;
			string str = go->GetName();
			FontManager->GenerateA(9, tth, str.c_str(), textColor, 100);

			if (!tth.Empty())
			{
				int maxOffset = tth.Height;

				tth.Draw(234, drawY);
				tth.Clear();

				str = "at " + std::to_string(go->Price) + " g.p.";
				FontManager->GenerateA(9, tth, str.c_str(), textColor, 100);

				if (!tth.Empty())
				{
					tth.Draw(234, drawY + maxOffset - 2);

					maxOffset += (tth.Height - 2);

					tth.Clear();
				}
						
				str = std::to_string(go->SelectedCount);
				int amountWidth = FontManager->GetWidthA(9, str.c_str(), str.length());
				FontManager->GenerateA(9, tth, str.c_str(), textColor, amountWidth);

				if (!tth.Empty())
				{
					tth.Draw(204, drawY);
					tth.Clear();
				}

				int drawOffsetY = drawY + (maxOffset / 2) - 6;
					
				Orion->DrawGump(0x0037, 0, 356, drawOffsetY); //+
				Orion->DrawGump(0x0038, 0, 376, drawOffsetY); //-

				drawY += maxOffset;
			}

			currentIndex++;

			go = (TGumpSellObject*)go->m_Next;
		}

		glDisable(GL_SCISSOR_TEST);
		
		//Up
		//UO->DrawGump(0x0828, 0, posX + 407, posY + 280); //Scroller 2 min
		//Down
		//UO->DrawGump(0x0828, 0, posX + 407, posY + 337); //Scroller 2 max

		Orion->DrawGump(0x0828, 0, 407, 280 + scrollerY2); //Scroller 2

		FontManager->DrawA(9, std::to_string(totalPrice).c_str(), 0x0386, 358, 386);
		
		if (m_NoClose)
		{
			if (g_Player != NULL)
			{
				string name = g_Player->GetName();

				if (m_ContinueCounter > (int)name.length())
					m_ContinueCounter = name.length();

				name.resize(m_ContinueCounter);

				static int sCounterCount = 0;

				if (m_ContinueCounter == g_Player->GetName().length())
				{
					m_NoClose = false;
					SendSellList();
				}
				else if (!sCounterCount)
				{
					if (m_ContinueCounter < (int)g_Player->GetName().length())
						m_ContinueCounter++;
				}

				sCounterCount++;

				if (sCounterCount >= 3)
					sCounterCount = 0;

				FontManager->DrawA(5, name.c_str(), 0x0386, 242, 408);
			}
		}

	glEndList();

	m_FrameRedraw = true;
	m_FrameCreated = true;
}
//----------------------------------------------------------------------------
int TGumpSell::Draw(bool &mode)
{
	if (m_Items == NULL)
		return 0;

	TGameCharacter *vendor = World->FindWorldCharacter(m_Serial);

	if (vendor == NULL)
		return 0; //Объект, к которому привязан гамп - исчез

	DWORD index = (DWORD)this;

	CalculateSelectedListCount(); //Вычисление размеров листа выбранных объектов

	CalculateGumpState();

	if (g_GumpPressedScroller)
		g_GumpPressedElement = g_LastObjectLeftMouseDown;

	DWORD ticks = g_Ticks;

	if (!m_NoClose)
		m_ContinueCounter = 0;
	else
		m_FrameCreated = false;
	
	if (mode && g_GumpPressedElement >= ID_GS_BUTTON_INC && g_GumpPressedElement < ID_GS_BUTTON_END && m_LastChangeCountTime < ticks)
	{
		int countToAdd = 1;
			
		if (m_StartChangeCountTime < 7)
			{}
		else if (m_StartChangeCountTime < 14)
			countToAdd = 3;
		else if (m_StartChangeCountTime < 20)
			countToAdd = 5;
		else if (m_StartChangeCountTime < 35)
			countToAdd = 10;
		else
			countToAdd = 20;

		m_StartChangeCountTime++;

		if (g_GumpPressedElement < ID_GS_BUTTON_DEC) //+
		{
			int idx = g_GumpPressedElement - ID_GS_BUTTON_INC;

			ChangeItemCount(true, idx, countToAdd);
		}
		else //-
		{
			int idx = g_GumpPressedElement - ID_GS_BUTTON_DEC;
			
			if (ChangeItemCount(false, idx, countToAdd))
			{
				g_LeftMouseDown = false;
				g_LastGumpLeftMouseDown = 0;
				g_LastObjectLeftMouseDown = 0;
			}
		}

		m_LastChangeCountTime = ticks + CHANGE_SHOP_COUNT_DELAY;

		m_FrameCreated = false;
	}
	
	if (g_GumpPressedScroller && m_LastChangedLineTime < ticks)
	{
		if (g_LastObjectLeftMouseDown == ID_GS_BUTTON_UP_1) //1 gump ^
		{
			ListingList(true, false);
			m_FrameCreated = false;
		}
		else if (g_LastObjectLeftMouseDown == ID_GS_BUTTON_DOWN_1) //1 gump v
		{
			ListingList(false, false);
			m_FrameCreated = false;
		}
		else if (g_LastObjectLeftMouseDown == ID_GS_BUTTON_UP_2) //2 gump ^
		{
			ListingList(true, true);
			m_FrameCreated = false;
		}
		else if (g_LastObjectLeftMouseDown == ID_GS_BUTTON_DOWN_2) //2 gump v
		{
			ListingList(false, true);
			m_FrameCreated = false;
		}
	}
	
	int scrollerY1 = 0;
	int scrollerY2 = 0;
	bool noRedraw = false;
	int visibleLines = m_Count - 1;
	int maxScrollerY = 91;

	if (g_GumpPressedScroller && g_LastObjectLeftMouseDown == ID_GS_SCROLL_1) //Scroller 1 pressed
	{
		int currentY = (g_MouseY - 10) - (int)g_GumpTranslateY - 66; //Scroller 1 position

		scrollerY1 = CalculateScrollerAndTextPosition(m_SelectedLine1, visibleLines, maxScrollerY, currentY);

		if (mode)
			m_FrameCreated = false;
	}
	else if (m_SelectedLine1)
	{
		scrollerY1 = CalculateScrollerY(m_SelectedLine1, visibleLines, maxScrollerY);

		if (mode)
			m_FrameCreated = false;
	}
	
	visibleLines = m_SelectedListCount - 1;
	maxScrollerY = 57;

	if (g_GumpPressedScroller && g_LastObjectLeftMouseDown == ID_GS_SCROLL_2) //Scroller 2 pressed
	{
		int currentY = (g_MouseY - 10) - (int)g_GumpTranslateY - 280; //Scroller 2 position

		scrollerY2 = CalculateScrollerAndTextPosition(m_SelectedLine2, visibleLines, maxScrollerY, currentY);

		if (mode)
			m_FrameCreated = false;
	}
	else if (m_SelectedLine2)
	{
		scrollerY2 = CalculateScrollerY(m_SelectedLine2, visibleLines, maxScrollerY);

		if (mode)
			m_FrameCreated = false;
	}

	if (mode) //Отрисовка
	{
		if (!m_FrameCreated || g_GumpMovingOffsetX || g_GumpMovingOffsetY)
			GenerateFrame();
		else if (m_FrameRedraw)
		{
			GenerateFrame();
			m_FrameRedraw = false;
		}

		glTranslatef(g_GumpTranslateX, g_GumpTranslateY, 0.0f);

		glCallList((GLuint)index);

		glTranslatef(-g_GumpTranslateX, -g_GumpTranslateY, 0.0f);
	}
	else //Выбор объектов
	{
		int oldMouseX = g_MouseX;
		int oldMouseY = g_MouseY;
		g_MouseX -= (int)g_GumpTranslateX;
		g_MouseY -= (int)g_GumpTranslateY;

		int LSG = 0;

		bool secondGumpSelected = false;

		if (Orion->GumpPixelsInXY(0x0873, 170, 214))
		{
			g_LastSelectedObject = 0;
			g_LastSelectedGump = index;
			secondGumpSelected = true;
		}
		else if (Orion->GumpPixelsInXY(0x0872, 0, 0))
		{
			g_LastSelectedObject = 0;
			g_LastSelectedGump = index;
		}

		if (g_LastSelectedGump == index)
		{
			//My Inventory

			int drawY = 60;
			
			TTextureObject *thDelim1 = Orion->ExecuteGump(0x0039);
			
			int delimHeight = 20;

			if (thDelim1 != NULL)
				delimHeight = thDelim1->Height;

			int currentIndex = 0;
			int startIndex = m_SelectedLine1;

			int drawYBounds = 236;
			
			bool completedSearch = false;
			
			TGumpSellObject *go = (TGumpSellObject*)m_Items;

			if (!secondGumpSelected)
			{
				while (go != NULL)
				{
					if (currentIndex < startIndex)
					{
						currentIndex++;
						go = (TGumpSellObject*)go->m_Next;
						continue;
					}

					if (drawY >= drawYBounds)
						break;

					POINT p_art = {0, 0};
					Orion->GetArtDimension(go->Graphic + 0x4000, p_art);

					if (p_art.y)
					{
						string str = go->GetName() + " at " + std::to_string(go->Price) + "gp";
						int textHeight = FontManager->GetHeightA(9, str.c_str(), 90);
						
						if (textHeight)
						{
							int maxOffset = p_art.y;

							if (maxOffset < textHeight)
								maxOffset = textHeight;

							int checkHeight = maxOffset;
							if (drawY + maxOffset > drawYBounds)
								checkHeight = drawYBounds - drawY;
						
							if (Orion->PolygonePixelsInXY(30, drawY, 200, checkHeight)) //Item in list on gump 1
							{
								LSG = ID_GS_ITEM_LIST + currentIndex;
								completedSearch = true;

								break;
							}

							drawY += (maxOffset + delimHeight);
						}
					}

					currentIndex++;

					if (completedSearch)
						break;

					go = (TGumpSellObject*)go->m_Next;
				}
			}

			if (!completedSearch) //Offer
			{
				drawY = 282;

				currentIndex = 0;
				startIndex = m_SelectedLine2;

				drawYBounds = 372;

				go = (TGumpSellObject*)m_Items;

				while (go != NULL)
				{
					if (!go->SelectedCount)
					{
						go = (TGumpSellObject*)go->m_Next;
						continue;
					}

					if (currentIndex < startIndex)
					{
						currentIndex++;
						go = (TGumpSellObject*)go->m_Next;

						continue;
					}

					if (drawY >= drawYBounds)
						break;

					string str = go->GetName();
					int textHeight = FontManager->GetHeightA(9, str.c_str(), 100);

					if (textHeight)
					{
						int maxOffset = textHeight;

						str = "at " + std::to_string(go->Price) + " g.p.";
						textHeight = FontManager->GetHeightA(9, str.c_str(), 100);

						if (textHeight)
							maxOffset += (textHeight - 2);
							
						int drawOffsetY = drawY + (maxOffset / 2) - 4;
					
						if (Orion->PolygonePixelsInXY(356, drawOffsetY, 14, 14)) //+
						{
							LSG = ID_GS_BUTTON_INC + currentIndex;
							completedSearch = true;

							break;
						}

						if (Orion->PolygonePixelsInXY(376, drawOffsetY, 14, 14)) //-
						{
							LSG = ID_GS_BUTTON_DEC + currentIndex;
							completedSearch = true;

							break;
						}

						drawY += maxOffset;
					}

					currentIndex++;

					if (completedSearch)
						break;

					go = (TGumpSellObject*)go->m_Next;
				}
			}
		}
		
		if (Orion->GumpPixelsInXY(0x0828, 237, 66 + scrollerY1)) //Scroller 1
			LSG = ID_GS_SCROLL_1;
		else if (Orion->GumpPixelsInXY(0x0828, 407, 280 + scrollerY2)) //Scroller 2
			LSG = ID_GS_SCROLL_2;
		else
		{
			if (Orion->PolygonePixelsInXY(233, 49, 20, 16)) //1 gump ^
				LSG = ID_GS_BUTTON_UP_1;
			else if (Orion->PolygonePixelsInXY(191, 49, 20, 16)) //1 gump v
				LSG = ID_GS_BUTTON_DOWN_1;
			else if (Orion->PolygonePixelsInXY(403, 264, 20, 16)) //2 gump ^
				LSG = ID_GS_BUTTON_UP_2;
			else if (Orion->PolygonePixelsInXY(403, 371, 20, 16)) //2 gump v
				LSG = ID_GS_BUTTON_DOWN_2;
			else if (Orion->PolygonePixelsInXY(200, 406, 34, 30)) //Accept
				LSG = ID_GS_BUTTON_ACCEPT;
			else if (Orion->PolygonePixelsInXY(372, 410, 24, 24)) //Clear
				LSG = ID_GS_BUTTON_CLEAR;
		}

		g_MouseX = oldMouseX;
		g_MouseY = oldMouseY;

		return LSG;
	}

	return 0;
}
//----------------------------------------------------------------------------
void TGumpSell::OnLeftMouseDown()
{
	if (!m_NoClose)
	{
		if (g_LastSelectedObject >= ID_GS_BUTTON_INC && g_LastSelectedObject < ID_GS_BUTTON_END) // +/-
		{
			m_StartChangeCountTime = 0;
			m_LastChangeCountTime = g_Ticks + (CHANGE_SHOP_COUNT_DELAY * 5);
		}
	}
}
//----------------------------------------------------------------------------
void TGumpSell::OnLeftMouseUp()
{
	if (m_NoClose || g_LastObjectLeftMouseDown != g_LastSelectedObject)
		return;

	if (g_LastObjectLeftMouseDown == ID_GS_BUTTON_UP_1) //1 gump ^
	{
		//ListingList(gump, true, false);
	}
	else if (g_LastObjectLeftMouseDown == ID_GS_BUTTON_DOWN_1) //1 gump v
	{
		//ListingList(gump, false, false);
	}
	else if (g_LastObjectLeftMouseDown == ID_GS_BUTTON_UP_2) //2 gump ^
	{
		//ListingList(gump, true, true);
	}
	else if (g_LastObjectLeftMouseDown == ID_GS_BUTTON_DOWN_2) //2 gump v
	{
		//ListingList(gump, false, true);
	}
	else if (g_LastObjectLeftMouseDown == ID_GS_BUTTON_ACCEPT) //Accept
	{
		m_NoClose = true;
		m_ContinueCounter = 0;
	}
	else if (g_LastObjectLeftMouseDown == ID_GS_BUTTON_CLEAR) //Clear
		m_ContinueCounter = 0;
	else if (g_LastObjectLeftMouseDown >= ID_GS_ITEM_LIST)
	{
		if (g_LastObjectLeftMouseDown < ID_GS_BUTTON_INC) //Item in list on gump 1
		{
			int idx = g_LastObjectLeftMouseDown - ID_GS_ITEM_LIST;

			int currentIndex = 0;

			QFOR(go, m_Items, TGumpSellObject*)
			{
				go->Selected = (idx == currentIndex);

				currentIndex++;
			}

			m_FrameCreated = false;
		}
		else if (g_LastObjectLeftMouseDown < ID_GS_BUTTON_DEC) //+
		{
			if (m_LastChangeCountTime > g_Ticks)
				ChangeItemCount(true, g_LastObjectLeftMouseDown - ID_GS_BUTTON_INC);

			m_FrameCreated = false;
		}
		else //-
		{
			if (m_LastChangeCountTime > g_Ticks)
				ChangeItemCount(false, g_LastObjectLeftMouseDown - ID_GS_BUTTON_DEC);

			m_FrameCreated = false;
		}
	}
}
//----------------------------------------------------------------------------
bool TGumpSell::OnLeftMouseDoubleClick()
{
	if (m_NoClose)
		return false;

	if (g_LastObjectLeftMouseDown >= ID_GS_ITEM_LIST && g_LastObjectLeftMouseDown < ID_GS_BUTTON_INC) //Item in list on gump 1
	{
		if (m_Items != NULL) //Sell
		{
			int idx = g_LastObjectLeftMouseDown - ID_GS_ITEM_LIST;

			int currentIndex = 0;

			QFOR(go, m_Items, TGumpSellObject*)
			{
				if (idx == currentIndex)
				{
					go->Selected = true;

					if (go->SelectedCount < go->Count)
						go->IncSelectedCount();
				}
				else
					go->Selected = false;

				currentIndex++;
			}

			m_FrameCreated = false;

			return true;
		}
	}

	return false;
}
//----------------------------------------------------------------------------
bool TGumpSell::ChangeItemCount(bool increase, int idx, int count)
{
	if (m_NoClose || m_Items == NULL)
		return false;

	int currentIndex = 0;

	QFOR(go, m_Items, TGumpSellObject*)
	{
		if (go->SelectedCount)
		{
			if (idx == currentIndex)
			{
				if (increase) //+
				{
					if (go->SelectedCount < go->Count)
					{
						go->AddSelectedCount(count);

						if (go->SelectedCount > go->Count)
							go->SelectedCount = go->Count;
					}
				}
				else //-
				{
					go->AddSelectedCount(-count);

					if (go->SelectedCount <= 0)
					{
						go->SelectedCount = 0;

						CalculateSelectedListCount();

						if (m_SelectedLine2 >= m_SelectedListCount)
						{
							if (m_SelectedListCount)
								m_SelectedLine2 = m_SelectedListCount - 1;
							else
								m_SelectedLine2 = 0;
						}

						return true;
					}
				}

				break;
			}
					
			currentIndex++;
		}
	}

	return false;
}
//----------------------------------------------------------------------------
void TGumpSell::ListingList(bool direction, bool gumpNumber, int divizor)
{
	if (m_NoClose) return;

	DWORD ticks = g_Ticks;

	if (!gumpNumber) //First gump
	{
		if (direction) //Up
		{
			if (m_SelectedLine1 > 0)
				m_SelectedLine1--;
			else
				m_SelectedLine1 = 0;

			m_LastChangedLineTime = ticks + (SCROLL_LISTING_DELAY / divizor);
		}
		else //Down
		{
			int visibleLines = m_Count - 1;

			if (m_SelectedLine1 < visibleLines)
				m_SelectedLine1++;
			else
				m_SelectedLine1 = visibleLines;

			m_LastChangedLineTime = ticks + (SCROLL_LISTING_DELAY / divizor);
		}
	}
	else //Second gump
	{
		CalculateSelectedListCount();

		if (direction) //Up
		{
			if (m_SelectedLine2 > 0)
				m_SelectedLine2--;
			else
				m_SelectedLine2 = 0;

			m_LastChangedLineTime = ticks + (SCROLL_LISTING_DELAY / divizor);
		}
		else if (m_SelectedListCount) //Down
		{
			int visibleLines = m_SelectedListCount - 1;

			if (m_SelectedLine2 < visibleLines)
				m_SelectedLine2++;
			else
				m_SelectedLine2 = visibleLines;

			m_LastChangedLineTime = ticks + (SCROLL_LISTING_DELAY / divizor);
		}
	}
}
//----------------------------------------------------------------------------
void TGumpSell::OnMouseWheel(MOUSE_WHEEL_STATE &state)
{
	if (m_NoClose)
		return;

	if (!g_LeftMouseDown && !g_RightMouseDown && state != MWS_PRESS)
	{
		if (m_LastChangedLineTime > g_Ticks)
			return;

		if (Orion->GumpPixelsInXY(0x0873, m_X + 170, m_Y + 214))
		{
			ListingList((bool)(state == MWS_UP), true, 5);

			m_FrameCreated = false;
		}
		else
		{
			if (Orion->GumpPixelsInXY(0x0872, m_X, m_Y))
			{
				ListingList((bool)(state == MWS_UP), false, 5);

				m_FrameCreated = false;
			}
		}
	}
}
//----------------------------------------------------------------------------