/****************************************************************************
**
** GumpBuy.cpp
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

int TGumpBuy::m_Layers[2] = {OL_BUY_RESTOCK, OL_BUY};
//---------------------------------------------------------------------------
TGumpBuy::TGumpBuy(DWORD serial, short x, short y)
: TGump(GT_SHOP, serial, x, y), m_LastChangedLineTime(0), m_LastChangeCountTime(0),
m_StartChangeCountTime(0), m_SelectedLine1(0), m_SelectedLine2(0), m_SelectedListCount(0),
m_ContinueCounter(0), m_Count(0)
{
}
//---------------------------------------------------------------------------
TGumpBuy::~TGumpBuy()
{
}
//----------------------------------------------------------------------------
void TGumpBuy::PrepareTextures()
{
	UO->ExecuteGumpPart(0x0870, 2);
	UO->ExecuteGumpPart(0x0039, 3);
	UO->ExecuteGump(0x0828);
	UO->ExecuteGumpPart(0x0037, 2);

	TGameCharacter *vendor = World->FindWorldCharacter(m_Serial);

	if (vendor == NULL)
		return;

	IFOR(i, 0, 2)
	{
		TGameItem *box = vendor->FindLayer(m_Layers[i]);
			
		if (box == NULL)
			continue;

		box = (TGameItem*)box->m_Items;

		while (box != NULL)
		{
			UO->ExecuteStaticArt(box->Graphic);

			box = (TGameItem*)box->m_Next;
		}
	}
}
//----------------------------------------------------------------------------
void TGumpBuy::CalculateBuyListCount()
{
	m_Count = 0;

	TGameCharacter *vendor = World->FindWorldCharacter(m_Serial);

	if (vendor == NULL)
		return;

	int count = 0;

	IFOR(i, 0, 2)
	{
		TGameItem *box = vendor->FindLayer(m_Layers[i]);

		if (box == NULL)
			continue;

		box = (TGameItem*)box->m_Items;

		while (box != NULL)
		{
			if (box->Count)
				count++;

			box = (TGameItem*)box->m_Next;
		}
	}

	m_Count = count;
}
//----------------------------------------------------------------------------
void TGumpBuy::CalculateSelectedListCount()
{
	m_SelectedListCount = 0;

	TGameCharacter *vendor = World->FindWorldCharacter(m_Serial);

	if (vendor == NULL)
		return;
	
	IFOR(i, 0, 2)
	{
		TGameItem *box = vendor->FindLayer(m_Layers[i]);
		
		if (box == NULL)
			continue;

		box = (TGameItem*)box->m_Items;

		while (box != NULL)
		{
			if (box->ShopItem != NULL && box->ShopItem->Count)
				m_SelectedListCount++;

			box = (TGameItem*)box->m_Next;
		}
	}
}
//---------------------------------------------------------------------------
void TGumpBuy::GenerateFrame(int posX, int posY)
{
	if (!g_DrawMode)
	{
		m_FrameRedraw = false;
		m_FrameCreated = false;

		return;
	}

	TGameCharacter *vendor = World->FindWorldCharacter(m_Serial);

	if (vendor == NULL)
		return; //Объект, к которому привязан гамп - исчез
	
	if (!m_Count)
		CalculateBuyListCount(); //Вычисление размера байлиста (только 1 раз)

	CalculateSelectedListCount(); //Вычисление размеров листа выбранных объектов

	CalculateGumpState();

	int scrollerY1 = 0;
	int scrollerY2 = 0;

	int visibleLines = m_Count - 1;
	int maxScrollerY = 91;

	if (g_GumpPressedScroller && g_LastObjectLeftMouseDown == ID_GB_SCROLL_1) //Scroller 1 pressed
	{
		int currentY = (g_MouseY - 10) - (int)g_GumpTranslateY; //Scroller 1 position

		scrollerY1 = CalculateScrollerAndTextPosition(m_SelectedLine1, visibleLines, maxScrollerY, currentY);
	}
	else if (m_SelectedLine1)
		scrollerY1 = CalculateScrollerY(m_SelectedLine1, visibleLines, maxScrollerY);

	visibleLines = m_SelectedListCount - 1;
	maxScrollerY = 57;
	
	if (g_GumpPressedScroller && g_LastObjectLeftMouseDown == ID_GB_SCROLL_2) //Scroller 2 pressed
	{
		int currentY = (g_MouseY - 10) - (int)g_GumpTranslateY; //Scroller 2 position

		scrollerY2 = CalculateScrollerAndTextPosition(m_SelectedLine2, visibleLines, maxScrollerY, currentY);
	}
	else if (m_SelectedLine2)
		scrollerY2 = CalculateScrollerY(m_SelectedLine2, visibleLines, maxScrollerY);

	glNewList((GLuint)this, GL_COMPILE);

		UO->DrawGump(0x0870, 0, 0, 0);

		//My Inventory

		g_GL.Scissor((int)g_GumpTranslateX + 30, (int)g_GumpTranslateY + 60, 200, 176);

		int drawY = 60;
			
		TTextureObject *thDelim1 = UO->ExecuteGump(0x0039);
		TTextureObject *thDelim2 = UO->ExecuteGump(0x003A);
		TTextureObject *thDelim3 = UO->ExecuteGump(0x003B);
			
		int delim1Height = 20;
		int delim1Width = 30;
		int delim1posX = 32;
		int delim2posX = 62;
		int delim3posX = 30;
		int delimLineWidth = 140;

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

		IFOR(i, 0, 2)
		{
			TGameItem *box = vendor->FindLayer(m_Layers[i]);
			
			if (box == NULL)
				continue;

			box = (TGameItem*)box->m_Items;

			while (box != NULL)
			{
				TShopItem *si = box->ShopItem;

				if (currentIndex < startIndex || si == NULL)
				{
					currentIndex++;
					box = (TGameItem*)box->m_Next;
					continue;
				}

				if (drawY >= drawYBounds)
				{
					i = 2;
					break;
				}

				TTextureObject *th = UO->ExecuteStaticArt(box->Graphic);
				if (th != NULL)
				{
					WORD textColor = 0x021F;
					if (si->Selected)
						textColor = 0x0021;

					TTextTexture tth;
					string str = si->GetName() + " at " + std::to_string(si->Price) + "gp";
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
						
						str = std::to_string(box->Count - si->Count);
						int amountWidth = FontManager->GetWidthA(9, str.c_str(), str.length());
						FontManager->GenerateA(9, tth, str.c_str(), textColor, amountWidth);

						if (!tth.Empty())
						{
							tth.Draw(226 - amountWidth, drawY + (maxOffset / 2) - (tth.Height / 2));
							tth.Clear();
						}

						ColorizerShader->Use();

						UO->DrawStaticArtInContainer(box->Graphic, box->Color, delim1posX, drawY + imageOffsetY);

						UnuseShader();
						
						drawY += maxOffset;
						
						UO->DrawGump(0x0039, 0, delim1posX, drawY);
						UO->DrawGump(0x003A, 0, delim2posX, drawY, delimLineWidth, 0);
						UO->DrawGump(0x003B, 0, delim3posX, drawY);

						drawY += delim1Height;
					}
				}

				currentIndex++;

				box = (TGameItem*)box->m_Next;
			}
		}

		glDisable(GL_SCISSOR_TEST);
		
		UO->DrawGump(0x0828, 0, 237, 66 + scrollerY1); //Scroller 1

		//Offer

		UO->DrawGump(0x0871, 0, 170, 214);
			
		g_GL.Scissor((int)g_GumpTranslateX + 200, (int)g_GumpTranslateY + 280, 200, 92);

		drawY = 282;

		currentIndex = 0;
		startIndex = m_SelectedLine2;

		drawYBounds = 372;

		int totalPrice = 0;
		
		IFOR(i, 0, 2)
		{
			TGameItem *box = vendor->FindLayer(m_Layers[i]);

			if (box == NULL)
				continue;

			box = (TGameItem*)box->m_Items;

			while (box != NULL)
			{
				TShopItem *si = box->ShopItem;

				if (si == NULL || si->Count == 0)
				{
					box = (TGameItem*)box->m_Next;
					continue;
				}

				totalPrice += (si->Count * (int)si->Price);
				
				if (currentIndex < startIndex)
				{
					currentIndex++;
					box = (TGameItem*)box->m_Next;
					continue;
				}

				if (drawY >= drawYBounds)
				{
					box = (TGameItem*)box->m_Next;
					continue;
				}

				WORD textColor = 0x021F;

				TTextTexture tth;
				string str = si->GetName();
				FontManager->GenerateA(9, tth, str.c_str(), textColor, 100);

				if (!tth.Empty())
				{
					int maxOffset = tth.Height;

					tth.Draw(234, drawY);
					tth.Clear();

					str = "at " + std::to_string(si->Price) + " g.p.";
					FontManager->GenerateA(9, tth, str.c_str(), textColor, 100);

					if (!tth.Empty())
					{
						tth.Draw(234, drawY + maxOffset - 2);
						maxOffset += (tth.Height - 2);
						tth.Clear();
					}
						
					str = std::to_string(si->Count);
					int amountWidth = FontManager->GetWidthA(9, str.c_str(), str.length());
					FontManager->GenerateA(9, tth, str.c_str(), textColor, amountWidth);

					if (!tth.Empty())
					{
						tth.Draw(204, drawY);
						tth.Clear();
					}

					int drawOffsetY = drawY + (maxOffset / 2) - 6;
					
					UO->DrawGump(0x0037, 0, 356, drawOffsetY); //+
					UO->DrawGump(0x0038, 0, 376, drawOffsetY); //-

					drawY += maxOffset;
				}

				currentIndex++;

				box = (TGameItem*)box->m_Next;
			}
		}

		glDisable(GL_SCISSOR_TEST);
		
		UO->DrawGump(0x0828, 0, 407, 280 + scrollerY2); //Scroller 2
		
		FontManager->DrawA(9, std::to_string(totalPrice).c_str(), 0x0386, 240, 385);
		
		FontManager->DrawA(9, std::to_string(g_Player->Gold).c_str(), 0x0386, 358, 385);
		
		if (m_NoClose)
		{
			if (g_Player != NULL)
			{
				string name = g_Player->GetName();

				if (m_ContinueCounter > (int)name.length())
					m_ContinueCounter = name.length();

				name.resize(m_ContinueCounter);

				static int bCounterCount = 0;

				if (m_ContinueCounter == (int)g_Player->GetName().length())
				{
					NoClose = false;
					SendBuyList();
				}
				else if (!bCounterCount)
				{
					if (m_ContinueCounter < (int)g_Player->GetName().length())
						m_ContinueCounter++;
				}

				bCounterCount++;

				if (bCounterCount >= 3)
					bCounterCount = 0;

				FontManager->DrawA(5, name.c_str(), 0x0386, 242, 408);
			}
		}

	glEndList();

	m_FrameRedraw = true;
	m_FrameCreated = true;
}
//----------------------------------------------------------------------------
int TGumpBuy::Draw(bool &mode)
{
	TGameCharacter *vendor = World->FindWorldCharacter(m_Serial);

	if (vendor == NULL)
		return 0; //Объект, к которому привязан гамп - исчез

	DWORD index = (DWORD)this;

	CalculateGumpState();

	if (!m_Count)
		CalculateBuyListCount(); //Вычисление размера байлиста (только 1 раз)

	CalculateSelectedListCount(); //Вычисление размеров листа выбранных объектов

	DWORD ticks = GetTickCount();

	if (!m_NoClose)
		m_ContinueCounter = 0;
	else
		m_FrameCreated = false;

	if (mode && g_GumpPressedElement >= ID_GB_BUTTON_INC && g_GumpPressedElement < ID_GB_BUTTON_END && m_LastChangeCountTime < ticks)
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

		if (g_GumpPressedElement < ID_GB_BUTTON_DEC) //+
			ChangeItemCount(true, g_GumpPressedElement - ID_GB_BUTTON_INC, countToAdd);
		else //-
		{
			if (ChangeItemCount(false, g_GumpPressedElement - ID_GB_BUTTON_DEC, countToAdd))
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
		if (g_LastObjectLeftMouseDown == ID_GB_BUTTON_UP_1) //1 gump ^
		{
			ListingList(true, false);
			m_FrameCreated = false;
		}
		else if (g_LastObjectLeftMouseDown == ID_GB_BUTTON_DOWN_1) //1 gump v
		{
			ListingList(false, false);
			m_FrameCreated = false;
		}
		else if (g_LastObjectLeftMouseDown == ID_GB_BUTTON_UP_2) //2 gump ^
		{
			ListingList(true, true);
			m_FrameCreated = false;
		}
		else if (g_LastObjectLeftMouseDown == ID_GB_BUTTON_DOWN_2) //2 gump v
		{
			ListingList(false, true);
			m_FrameCreated = false;
		}
	}
	
	int scrollerY1 = 66;
	int scrollerY2 = 280;
	bool noRedraw = false;

	int visibleLines = m_Count - 1;
	int maxScrollerY = 91;

	if (g_GumpPressedScroller && g_LastObjectLeftMouseDown == ID_GB_SCROLL_1) //Scroller 1 pressed
	{
		int currentY = (g_MouseY - 10) - (int)g_GumpTranslateY; //Scroller 1 position

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
	
	if (g_GumpPressedScroller && g_LastObjectLeftMouseDown == ID_GB_SCROLL_2) //Scroller 2 pressed
	{
		int currentY = (g_MouseY - 10) - (int)g_GumpTranslateY; //Scroller 2 position

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
			GenerateFrame(0, 0);
		else if (m_FrameRedraw)
		{
			GenerateFrame(0, 0);
			m_FrameRedraw = false;
		}

		glTranslatef(g_GumpTranslateX, g_GumpTranslateY, 0.0f);

		glCallList((GLuint)index);

		glTranslatef(-g_GumpTranslateX, -g_GumpTranslateY, 0.0f);
	}
	else //Выбор объектов
	{
		int LSG = 0;

		int oldMouseX = g_MouseX;
		int oldMouseY = g_MouseY;
		g_MouseX -= (int)g_GumpTranslateX;
		g_MouseY -= (int)g_GumpTranslateY;

		bool secondGumpSelected = false;

		if (UO->GumpPixelsInXY(0x0871, 170, 214))
		{
			g_LastSelectedObject = 0;
			g_LastSelectedGump = index;
			secondGumpSelected = true;
		}
		else if (UO->GumpPixelsInXY(0x0870, 0, 0))
		{
			g_LastSelectedObject = 0;
			g_LastSelectedGump = index;
		}

		if (g_LastSelectedGump == index)
		{
			//My Inventory

			int drawY = 60;
			
			TTextureObject *thDelim1 = UO->ExecuteGump(0x0039);
			
			int delimHeight = 20;

			if (thDelim1 != NULL)
				delimHeight = thDelim1->Height;

			int currentIndex = 0;
			int startIndex = m_SelectedLine1;

			int drawYBounds = 236;
			
			bool completedSearch = false;
			
			if (!secondGumpSelected)
			{
				IFOR(i, 0, 2)
				{
					TGameItem *box = vendor->FindLayer(m_Layers[i]);

					if (box == NULL)
						continue;

					box = (TGameItem*)box->m_Items;

					while (box != NULL)
					{
						TShopItem *si = box->ShopItem;

						if (currentIndex < startIndex || si == NULL)
						{
							currentIndex++;
							box = (TGameItem*)box->m_Next;
							continue;
						}

						if (drawY >= drawYBounds)
							break;

						POINT p_art = {0, 0};
						UO->GetArtDimension(box->Graphic + 0x4000, p_art);

						if (p_art.y)
						{
							string str = si->GetName() + " at " + std::to_string(si->Price) + "gp";
							int textHeight = FontManager->GetHeightA(9, str.c_str(), 90);
						
							if (textHeight)
							{
								int maxOffset = p_art.y;

								if (maxOffset < textHeight)
									maxOffset = textHeight;

								int checkHeight = maxOffset;
								if (drawY + maxOffset > drawYBounds)
									checkHeight = drawYBounds - drawY;
						
								if (UO->PolygonePixelsInXY(30, drawY, 200, checkHeight)) //Item in list on gump 1
								{
									LSG = ID_GB_ITEM_LIST + currentIndex;
									completedSearch = true;
									break;
								}

								drawY += (maxOffset + delimHeight);
							}
						}

						currentIndex++;

						if (completedSearch)
							break;

						box = (TGameItem*)box->m_Next;
					}
				}
			}

			if (!completedSearch) //Offer
			{
				drawY = 282;

				currentIndex = 0;
				startIndex = m_SelectedLine2;

				drawYBounds = 372;

				IFOR(i, 0, 2)
				{
					TGameItem *box = vendor->FindLayer(m_Layers[i]);

					if (box == NULL)
						continue;

					box = (TGameItem*)box->m_Items;

					while (box != NULL)
					{
						TShopItem *si = box->ShopItem;

						if (si == NULL || si->Count == 0)
						{
							box = (TGameItem*)box->m_Next;
							continue;
						}

						if (currentIndex < startIndex)
						{
							currentIndex++;
							box = (TGameItem*)box->m_Next;
							continue;
						}

						if (drawY >= drawYBounds)
							break;

						string str = si->GetName();
						int textHeight = FontManager->GetHeightA(9, str.c_str(), 100);

						if (textHeight)
						{
							int maxOffset = textHeight;

							str = "at " + std::to_string(si->Price) + " g.p.";
							textHeight = FontManager->GetHeightA(9, str.c_str(), 100);

							if (textHeight)
								maxOffset += (textHeight - 2);
							
							int drawOffsetY = drawY + (maxOffset / 2) - 4;
					
							if (UO->PolygonePixelsInXY(356, drawOffsetY, 14, 14)) //+
							{
								LSG = ID_GB_BUTTON_INC + currentIndex;
								completedSearch = true;
								break;
							}

							if (UO->PolygonePixelsInXY(376, drawOffsetY, 14, 14)) //-
							{
								LSG = ID_GB_BUTTON_DEC + currentIndex;
								completedSearch = true;
								break;
							}

							drawY += maxOffset;
						}

						currentIndex++;

						if (completedSearch)
							break;

						box = (TGameItem*)box->m_Next;
					}
				}
			}
		}
		
		if (UO->GumpPixelsInXY(0x0828, 237, 66 + scrollerY1)) //Scroller 1
		{
			g_LastSelectedGump = index;
			LSG = ID_GB_SCROLL_1;
		}
		else if (UO->GumpPixelsInXY(0x0828, 407, 280 + scrollerY2)) //Scroller 2
		{
			g_LastSelectedGump = index;
			LSG = ID_GB_SCROLL_2;
		}
		else if (g_LastSelectedGump == index)
		{
			if (UO->PolygonePixelsInXY(233, 49, 20, 14)) //1 gump ^
				LSG = ID_GB_BUTTON_UP_1;
			else if (UO->PolygonePixelsInXY(233, 191, 20, 14)) //1 gump v
				LSG = ID_GB_BUTTON_DOWN_1;
			else if (UO->PolygonePixelsInXY(403, 264, 20, 14)) //2 gump ^
				LSG = ID_GB_BUTTON_UP_2;
			else if (UO->PolygonePixelsInXY(403, 371, 20, 14)) //2 gump v
				LSG = ID_GB_BUTTON_DOWN_2;
			else if (UO->PolygonePixelsInXY(200, 406, 34, 30)) //Accept
				LSG = ID_GB_BUTTON_ACCEPT;
			else if (UO->PolygonePixelsInXY(372, 410, 24, 24)) //Clear
				LSG = ID_GB_BUTTON_CLEAR;
		}

		g_MouseX = oldMouseX;
		g_MouseY = oldMouseY;

		if (LSG != 0)
			g_LastSelectedObject = LSG; //Если что-то нашлось - выбираем

		return LSG;
	}

	return 0;
}
//----------------------------------------------------------------------------
void TGumpBuy::OnLeftMouseDown()
{
	if (NoClose)
		return;

	if (g_LastSelectedObject >= ID_GB_BUTTON_INC && g_LastSelectedObject < ID_GB_BUTTON_END) // +/-
	{
		m_StartChangeCountTime = 0;
		m_LastChangeCountTime = GetTickCount() + (CHANGE_SHOP_COUNT_DELAY * 5);
	}
}
//----------------------------------------------------------------------------
void TGumpBuy::OnLeftMouseUp()
{
	if (NoClose)
		return;

	if (g_LastObjectLeftMouseDown != g_LastSelectedObject)
		return;

	DWORD ticks = GetTickCount();
	
	if (g_LastObjectLeftMouseDown == ID_GB_BUTTON_UP_1) //1 gump ^
	{
		//ListingList(gump, true, false);
	}
	else if (g_LastObjectLeftMouseDown == ID_GB_BUTTON_DOWN_1) //1 gump v
	{
		//ListingList(gump, false, false);
	}
	else if (g_LastObjectLeftMouseDown == ID_GB_BUTTON_UP_2) //2 gump ^
	{
		//ListingList(gump, true, true);
	}
	else if (g_LastObjectLeftMouseDown == ID_GB_BUTTON_DOWN_2) //2 gump v
	{
		//ListingList(gump, false, true);
	}
	else if (g_LastObjectLeftMouseDown == ID_GB_BUTTON_ACCEPT) //Accept
	{
		NoClose = true;
		m_ContinueCounter = 0;
	}
	else if (g_LastObjectLeftMouseDown == ID_GB_BUTTON_CLEAR) //Clear
	{
		m_ContinueCounter = 0;
	}
	else if (g_LastObjectLeftMouseDown >= ID_GB_ITEM_LIST)
	{
		if (g_LastObjectLeftMouseDown < ID_GB_BUTTON_INC) //Item in list on gump 1
		{
			int idx = g_LastObjectLeftMouseDown - ID_GB_ITEM_LIST;
			
			TGameCharacter *vendor = World->FindWorldCharacter(Serial);

			if (vendor != NULL)
			{
				int currentIndex = 0;

				IFOR(i, 0, 2)
				{
					TGameItem *box = vendor->FindLayer(m_Layers[i]);
					
					if (box == NULL)
						continue;

					box = (TGameItem*)box->m_Items;

					while (box != NULL)
					{
						TShopItem *si = box->ShopItem;

						if (si != NULL)
							si->Selected = (idx == currentIndex);
				
						currentIndex++;

						box = (TGameItem*)box->m_Next;
					}
				}
			}

			GenerateFrame(X, Y);
		}
		else if (g_LastObjectLeftMouseDown < ID_GB_BUTTON_DEC) //+
		{
			if (m_LastChangeCountTime > GetTickCount())
			{
				ChangeItemCount(true, g_LastObjectLeftMouseDown - ID_GB_BUTTON_INC);
				GenerateFrame(X, Y);
			}
		}
		else //-
		{
			if (m_LastChangeCountTime > GetTickCount())
			{
				ChangeItemCount(false, g_LastObjectLeftMouseDown - ID_GB_BUTTON_DEC);
				GenerateFrame(X, Y);
			}
		}
	}
}
//----------------------------------------------------------------------------
bool TGumpBuy::OnLeftMouseDoubleClick()
{
	if (NoClose)
		return false;

	if (g_LastObjectLeftMouseDown >= ID_GB_ITEM_LIST && g_LastObjectLeftMouseDown < ID_GB_BUTTON_INC) //Item in list on gump 1
	{
		TGameCharacter *vendor = World->FindWorldCharacter(Serial);

		if (vendor == NULL)
			return false;
		
		int idx = g_LastObjectLeftMouseDown - ID_GB_ITEM_LIST;
		int currentIndex = 0;

		IFOR(i, 0, 2)
		{
			TGameItem *box = vendor->FindLayer(m_Layers[i]);
			
			if (box == NULL)
				continue;

			box = (TGameItem*)box->m_Items;

			while (box != NULL)
			{
				TShopItem *si = box->ShopItem;

				if (si != NULL)
				{
					if (idx == currentIndex)
					{
						si->Selected = true;

						if (si->Count < (int)box->Count)
							si->IncCount();
					}
					else
						si->Selected = false;
				}

				currentIndex++;

				box = (TGameItem*)box->m_Next;
			}
		}

		GenerateFrame(X, Y);

		return true;
	}

	return false;
}
//----------------------------------------------------------------------------
bool TGumpBuy::ChangeItemCount(bool increase, int idx, int count)
{
	if (NoClose)
		return false;

	TGameCharacter *vendor = World->FindWorldCharacter(Serial);

	if (vendor == NULL)
		return false;
		
	int currentIndex = 0;

	IFOR(i, 0, 2)
	{
		TGameItem *box = vendor->FindLayer(m_Layers[i]);
			
		if (box == NULL)
			continue;

		box = (TGameItem*)box->m_Items;

		while (box != NULL)
		{
			TShopItem *si = box->ShopItem;

			if (si != NULL && si->Count)
			{
				if (idx == currentIndex)
				{
					if (increase) //+
					{
						if (si->Count < (int)box->Count)
						{
							si->AddCount(count);

							if (si->Count >(int)box->Count)
								si->Count = box->Count;
						}
					}
					else
					{
						si->AddCount(-count);

						if (si->Count <= 0)
						{
							si->Count = 0;
							
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

					i = 2;
					break;
				}

				currentIndex++;
			}

			box = (TGameItem*)box->m_Next;
		}
	}

	return false;
}
//----------------------------------------------------------------------------
void TGumpBuy::ListingList(bool direction, bool gumpNumber, int divizor)
{
	if (NoClose)
		return;

	DWORD ticks = GetTickCount();

	if (!gumpNumber) //First gump
	{
		if (direction) //Up
		{
			if (m_SelectedLine1 > 0)
				m_SelectedLine1--;
			else
				m_SelectedLine1 = 0;

			m_LastChangedLineTime = ticks + (SCROLL_LISTING_DELAY / divizor);
			GenerateFrame(X, Y);
		}
		else //Down
		{
			int visibleLines = m_Count - 1;

			if (m_SelectedLine1 < visibleLines)
				m_SelectedLine1++;
			else
				m_SelectedLine1 = visibleLines;

			m_LastChangedLineTime = ticks + (SCROLL_LISTING_DELAY / divizor);
			GenerateFrame(X, Y);
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
			GenerateFrame(X, Y);
		}
		else if (m_SelectedListCount) //Down
		{
			int visibleLines = m_SelectedListCount - 1;

			if (m_SelectedLine2 < visibleLines)
				m_SelectedLine2++;
			else
				m_SelectedLine2 = visibleLines;

			m_LastChangedLineTime = ticks + (SCROLL_LISTING_DELAY / divizor);
			GenerateFrame(X, Y);
		}
	}
}
//----------------------------------------------------------------------------
void TGumpBuy::OnMouseWheel(MOUSE_WHEEL_STATE &state)
{
	if (NoClose)
		return;

	if (!g_LeftMouseDown && !g_RightMouseDown && state != MWS_PRESS)
	{
		if (m_LastChangedLineTime > GetTickCount())
			return;

		register int x = X;
		register int y = Y;

		if (UO->GumpPixelsInXY(0x0871, x + 170, y + 214))
		{
			ListingList((bool)(state == MWS_UP), true, 5);

			GenerateFrame(x, y);
		}
		else if (UO->GumpPixelsInXY(0x0870, x, y))
		{
			ListingList((bool)(state == MWS_UP), false, 5);

			GenerateFrame(x, y);
		}
	}
}
//----------------------------------------------------------------------------