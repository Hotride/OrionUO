/****************************************************************************
**
** JournalGump.cpp
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
//----------------------------------------------------------------------------
TGumpJournal::TGumpJournal(DWORD serial, short x, short y, bool minimized)
: TGump(GT_JOURNAL, serial, x, y), m_Height(200), m_HeightBuffer(0),
m_LastScrollChangeTime(0)
{
	if (minimized)
	{
		Minimized = true;
		MinimizedX = x;
		MinimizedY = y;
	}
}
//----------------------------------------------------------------------------
TGumpJournal::~TGumpJournal()
{
}
//----------------------------------------------------------------------------
void TGumpJournal::PrepareTextures()
{
	UO->ExecuteGump(0x001F);
	UO->ExecuteGumpPart(0x00D2, 2);
	UO->ExecuteGumpPart(0x0820, 6);
	UO->ExecuteGumpPart(0x082A, 7);
}
//----------------------------------------------------------------------------
void TGumpJournal::SetHeight(int val)
{
	if (val < 120)
		val = 120;

	int buf = (GetSystemMetrics(SM_CYSCREEN) - 50);

	if (val >= buf)
		val = buf;

	m_Height = val;
}
//----------------------------------------------------------------------------
void TGumpJournal::ChangeHeight()
{
	if (m_HeightBuffer)
		SetHeight(m_Height + (g_MouseY - m_HeightBuffer));

	m_HeightBuffer = 0;
	g_ResizedGump = NULL;
}
//---------------------------------------------------------------------------
void TGumpJournal::GenerateFrame(int posX, int posY)
{
	if (!g_DrawMode)
	{
		FrameRedraw = false;
		FrameCreated = false;

		return;
	}

	DWORD index = (DWORD)this;

	//Нажата ли кнопка в окне?
	bool IsPressed = (g_LeftMouseDown && g_LastGumpLeftMouseDown == index && g_LastSelectedGump == index);

	//Нажата ли кнопка в окне?
	bool IsScrollerPressed = (g_LeftMouseDown && g_LastGumpLeftMouseDown == index);

	//Может ли быть подсвечен элемент?
	int CanSelectedButton = ((g_LastSelectedGump == index) ? g_LastSelectedObject : 0);

	//Может ли быть нажат элемент?
	int CanPressedButton = 0;
	if (IsPressed && g_LastObjectLeftMouseDown == g_LastSelectedObject)
		CanPressedButton = g_LastObjectLeftMouseDown;
	
	int height = m_Height;

	if (m_HeightBuffer)
	{
		height += (g_MouseY - m_HeightBuffer);

		if (height < 120)
			height = 120;

		int buf = (GetSystemMetrics(SM_CYSCREEN) - 50);

		if (height >= buf)
			height = buf;
	}
	
	glNewList((GLuint)index, GL_COMPILE);

		if (Minimized)
		{
			UO->DrawGump(0x0830, 0, posX, posY);

			glEndList();

			FrameRedraw = true;
			FrameCreated = true;

			return;
		}

		UO->DrawGump(0x082D, 0, posX + 137, posY); //Minimize
		UO->DrawGump(0x0820, 0, posX, posY + 23); //Top scroll
		UO->DrawGump(0x082A, 0, posX + 111, posY + 34); //Journal text gump
		
		//Journal body
		int curposY = posY + 59;
		
		while (true)
		{
			int deltaHeight = (height + posY) - (curposY - 36);

			if (deltaHeight  < 70)
			{
				if (deltaHeight > 0)
					UO->DrawGump(0x0821, 0, posX + 18, curposY, 0, deltaHeight);

				break;
			}
			else
				UO->DrawGump(0x0821, 0, posX + 18, curposY);

			curposY += 70;

			deltaHeight = (height + posY) - (curposY - 36);

			if (deltaHeight < 70)
			{
				if (deltaHeight > 0)
					UO->DrawGump(0x0822, 0, posX + 18, curposY, 0, deltaHeight);

				break;
			}
			else
				UO->DrawGump(0x0822, 0, posX + 18, curposY);

			curposY += 70;
		}
		
		//Up
		//UO->DrawGump(0x001F, 0, posX + 257, posY + 72); //Scroller
		//Down
		//UO->DrawGump(0x001F, 0, posX + 257, posY + Height - 17); //Scroller
		
		int scrollerY = height - 17;

		if (IsScrollerPressed && g_LastObjectLeftMouseDown == ID_GJ_SCROLLER) //Scroller pressed
		{
			int currentY = (g_MouseY - 10) - (posY + 72); //Scroller position

			if (currentY < (scrollerY - 72))
			{
				if (currentY > 0)
				{
					scrollerY  -= 72;

					float ValPer = (currentY / (float)scrollerY) * 100.0f;

					int jsize = Journal->Size;

					jsize = jsize - (int)((jsize * ValPer) / 100.0f);
					if (jsize < 1)
						jsize = 1;
					else if (jsize > Journal->Size)
						jsize = Journal->Size;

					Journal->SelectionIndex = jsize;

					scrollerY = currentY + 72;
				}
				else
				{
					scrollerY = 72;
					Journal->SelectionIndex = Journal->Size;
				}
			}
			else
				Journal->SelectionIndex = 1;
		}
		else if (Journal->SelectionIndex)
		{
			int jsize = Journal->Size;

			float ValPer = (Journal->SelectionIndex / (float)jsize) * 100.0f;

			jsize = (int)((jsize * ValPer) / 100.0f);
			if (jsize < 1)
				jsize = 1;
			else if (jsize > Journal->Size)
				jsize = Journal->Size;
			
			if (jsize == Journal->Size)
				scrollerY = 72;
			else if (jsize != 1)
			{
				scrollerY -= 72;
				scrollerY = (scrollerY + 72) - (int)((scrollerY * ValPer) / 100.0f);
			}
		}
		
		UO->DrawGump(0x001F, 0, posX + 257, posY + scrollerY); //Scroller
		
		UO->DrawGump(0x082B, 0, posX + 30, posY + 58); //Top line
		UO->DrawGump(0x0824, 0, posX + 249, posY + 56); //^ button
		UO->DrawGump(0x082B, 0, posX + 31, posY + height + 21); //Bottom line
		UO->DrawGump(0x0825, 0, posX + 249, posY + height + 17); //v button
		
		if (Journal->SelectionIndex)
			UO->DrawGump(0x082C, 0, posX + 233, posY + height + 17); //Lock
		
		UO->DrawGump(0x0823, 0, posX + 18, posY + height + 34); //Bottom scroll
		
		UO->DrawGump(0x00D2 + (int)g_JournalShowSystem, 0, posX + 40, posY + height + 43); //Show System
		FontManager->DrawA(1, "System", 0x0386, posX + 63, posY + height + 47);
		
		UO->DrawGump(0x00D2 + (int)g_JournalShowObjects, 0, posX + 126, posY + height + 43); //Show Objects
		FontManager->DrawA(1, "Objects", 0x0386, posX + 149, posY + height + 47);
		
		UO->DrawGump(0x00D2 + (int)g_JournalShowClient, 0, posX + 210, posY + height + 43); //Show Client
		FontManager->DrawA(1, "Client", 0x0386, posX + 233, posY + height + 47);
		
		WORD gumpID = 0x082E + (int)(CanSelectedButton == ID_GJ_BUTTON_RESIZE);
		UO->DrawGump(gumpID, 0, posX + 137, posY + height + 66); //Resize

	glEndList();

	FrameRedraw = true;
	FrameCreated = true;
}
//----------------------------------------------------------------------------
int TGumpJournal::Draw(bool &mode)
{
	DWORD index = (DWORD)this;

	int posX = X;
	int posY = Y;
	
	if (Minimized)
	{
		posX = MinimizedX;
		posY = MinimizedY;
		m_HeightBuffer = 0;
	}
	
	//Нажата ли кнопка в окне?
	bool IsPressed = (g_LeftMouseDown && g_LastGumpLeftMouseDown == index && g_LastSelectedGump == index);

	//Нажата ли кнопка в окне?
	bool IsScrollerPressed = (g_LeftMouseDown && g_LastGumpLeftMouseDown == index);

	//Может ли быть подсвечен элемент?
	int CanSelectedButton = ((g_LastSelectedGump == index) ? g_LastSelectedObject : 0);

	//Может ли быть нажат элемент?
	int CanPressedButton = 0;
	if (IsPressed && g_LastObjectLeftMouseDown == g_LastSelectedObject)
		CanPressedButton = g_LastObjectLeftMouseDown;
	
	int height = m_Height;

	if (m_HeightBuffer)
	{
		height += (g_MouseY - m_HeightBuffer);

		if (height < 120)
			height = 120;

		int buf = (GetSystemMetrics(SM_CYSCREEN) - 50);

		if (height >= buf)
			height = buf;

		if (mode)
			GenerateFrame(posX, posY);
	}

	DWORD ticks = GetTickCount();

	if (IsScrollerPressed && m_LastScrollChangeTime < ticks)
	{
		if (g_LastObjectLeftMouseDown == 7)
		{
			if (Journal->SelectionIndex < Journal->Size)
				Journal->IncSelectionIndex();
			else
				Journal->SelectionIndex = Journal->Size;
		}
		else if (g_LastObjectLeftMouseDown == 8)
		{
			if (Journal->SelectionIndex > 1)
				Journal->DecSelectionIndex();
			else
				Journal->SelectionIndex = 1;
		}

		m_LastScrollChangeTime = ticks + SCROLL_LISTING_DELAY;
	}
	
	int scrollerY = height - 17;
	bool noRedraw = false;

	if (!Minimized) //Check scroller
	{
		if (IsScrollerPressed && g_LastObjectLeftMouseDown == ID_GJ_SCROLLER) //Scroller pressed
		{
			int currentY = (g_MouseY - 10) - (posY + 72); //Scroller position

			if (currentY < (scrollerY - 72))
			{
				if (currentY > 0)
				{
					scrollerY  -= 72;
					float ValPer = (currentY / (float)scrollerY) * 100.0f;

					int jsize = Journal->Size;

					jsize = jsize - (int)((jsize * ValPer) / 100.0f);
					if (jsize < 1)
						jsize = 1;
					else if (jsize > Journal->Size)
						jsize = Journal->Size;

					Journal->SelectionIndex = jsize;

					scrollerY = currentY + 72;
				}
				else
				{
					scrollerY = 72;
					Journal->SelectionIndex = Journal->Size;
				}
			}
			else
				Journal->SelectionIndex = 1;

			if (mode)
				GenerateFrame(posX, posY);

			noRedraw = true;
		}
		else if (Journal->SelectionIndex)
		{
			int jsize = Journal->Size;

			float ValPer = (Journal->SelectionIndex / (float)jsize) * 100.0f;

			jsize = (int)((jsize * ValPer) / 100.0f);
			if (jsize < 1)
				jsize = 1;
			else if (jsize > Journal->Size)
				jsize = Journal->Size;
			
			if (jsize == Journal->Size)
				scrollerY = 72;
			else if (jsize != 1)
			{
				scrollerY -= 72;
				scrollerY = (scrollerY + 72) - (int)((scrollerY * ValPer) / 100.0f);
			}

			if (mode)
				GenerateFrame(posX, posY);

			noRedraw = true;
		}
	}

	if (CanBeMoved() && g_LeftMouseDown && g_LastGumpLeftMouseDown == index && !g_LastObjectLeftMouseDown)
	{
		posX += g_MouseX - g_DroppedLeftMouseX;
		posY += g_MouseY - g_DroppedLeftMouseY;
		
		if (mode && !m_HeightBuffer)
			GenerateFrame(posX, posY);
	}
	else if (mode && !noRedraw)
	{
		if (CanSelectedButton && !m_HeightBuffer)
			GenerateFrame(posX, posY);
		else if (FrameRedraw)
		{
			GenerateFrame(posX, posY);
			FrameRedraw = false;
		}
	}

	if (mode)
	{
		if (!FrameCreated)
			GenerateFrame(posX, posY);

		glCallList((GLuint)index);

		if (Minimized)
			return 0;

		//g_GL.DrawPolygone(0x007F7F7F, posX + 38, posY + 70, 214, Height - 50);

		g_GL.ViewPort(posX + 38, posY + 70, 214, height - 50);

		//Отрисовка сообщений систем чата
		int textOffsY = posY + height + 16;
		int textYBounds = posY + 70;
		int drawX = posX + 38;

		int journalHeight = 0;

		if (g_JournalShowSystem)
			journalHeight += Journal->SystemHeight;

		if (g_JournalShowObjects)
			journalHeight += Journal->ObjectsHeight;

		if (g_JournalShowClient)
			journalHeight += Journal->ClientHeight;

		if (journalHeight < height - 50)
		{
			textYBounds = posY + height;
			textOffsY = posY + 70;

			TTextData *td = Journal->m_Top;

			while (td != NULL)
			{
				//if (TextOffsY > TextYBounds) break;
				
				bool candraw = true;

				switch (td->Type)
				{
					case TT_SYSTEM:
					{
						candraw = g_JournalShowSystem;
						break;
					}
					case TT_OBJECT:
					{
						candraw = g_JournalShowObjects;
						break;
					}
					case TT_CLIENT:
					{
						candraw = g_JournalShowClient;
						break;
					}
					default:
						break;
				}

				if (candraw)
				{
					TTextTexture &tth = td->m_Texture;

					tth.Draw(drawX, textOffsY);
					textOffsY += tth.Height;
				}

				td = td->m_Next;
			}
		}
		else
		{
			if (Journal->SelectionIndex)
			{
				if (Journal->SelectionIndex >= Journal->Size)
				{
					textYBounds = posY + height;
					textOffsY = posY + 70;

					TTextData *td = Journal->m_Top;

					while (td != NULL)
					{
						if (textOffsY > textYBounds) break;
				
						bool candraw = true;

						switch (td->Type)
						{
							case TT_SYSTEM:
							{
								candraw = g_JournalShowSystem;
								break;
							}
							case TT_OBJECT:
							{
								candraw = g_JournalShowObjects;
								break;
							}
							case TT_CLIENT:
							{
								candraw = g_JournalShowClient;
								break;
							}
							default:
								break;
						}

						if (candraw)
						{
							TTextTexture &tth = td->m_Texture;

							tth.Draw(drawX, textOffsY);
							textOffsY += tth.Height;
						}

						td = td->m_Next;
					}
				}
				else
				{
					int testHeight = 0;
					int maxID = Journal->SelectionIndex;
					int currentID = 0;
					
					TTextData *td = Journal->m_Head;
					TTextData *last_td = Journal->m_Head;

					while (td != NULL)
					{
						bool candraw = true;

						currentID++;

						switch (td->Type)
						{
							case TT_SYSTEM:
							{
								candraw = g_JournalShowSystem;
								break;
							}
							case TT_OBJECT:
							{
								candraw = g_JournalShowObjects;
								break;
							}
							case TT_CLIENT:
							{
								candraw = g_JournalShowClient;
								break;
							}
							default:
								break;
						}

						if (candraw)
						{
							TTextTexture &tth = td->m_Texture;
							testHeight += tth.Height;

							last_td = td;
						}

						if (currentID >= maxID)
							break;

						td = td->m_Prev;
					}

					textYBounds = posY + height;
					textOffsY = posY + 70;
					
					td = last_td;

					while (td != NULL)
					{
						if (textOffsY > textYBounds)
							break;
				
						bool candraw = true;

						switch (td->Type)
						{
							case TT_SYSTEM:
							{
								candraw = g_JournalShowSystem;
								break;
							}
							case TT_OBJECT:
							{
								candraw = g_JournalShowObjects;
								break;
							}
							case TT_CLIENT:
							{
								candraw = g_JournalShowClient;
								break;
							}
							default:
								break;
						}

						if (candraw)
						{
							TTextTexture &tth = td->m_Texture;

							tth.Draw(drawX, textOffsY);
							textOffsY += tth.Height;
						}

						td = td->m_Next;
					}
				}
			}
			else
			{
				TTextData *td = Journal->m_Head;

				while (td != NULL)
				{
					if (textOffsY < textYBounds)
						break;

					bool candraw = true;

					switch (td->Type)
					{
						case TT_SYSTEM:
						{
							candraw = g_JournalShowSystem;
							break;
						}
						case TT_OBJECT:
						{
							candraw = g_JournalShowObjects;
							break;
						}
						case TT_CLIENT:
						{
							candraw = g_JournalShowClient;
							break;
						}
						default:
							break;
					}

					if (candraw)
					{
						TTextTexture &tth = td->m_Texture;

						textOffsY -= tth.Height;
						tth.Draw(drawX, textOffsY);
					}

					td = td->m_Prev;
				}
			}
		}

		g_GL.RestorePort();

		DrawLocker(posX, posY);
	}
	else
	{
		if (Minimized)
		{
			if (UO->GumpPixelsInXY(0x0830, posX, posY))
			{
				g_LastSelectedObject = 0;
				g_LastSelectedGump = index;
			}

			return 0;
		}
		
		int LSG = 0;
		
		if (UO->GumpPixelsInXY(0x082D, posX + 137, posY)) //Minimize
		{
			LSG = ID_GJ_BUTTON_MINIMIZE;
			g_LastSelectedGump = index;
		}
		
		if (UO->GumpPixelsInXY(0x0820, posX, posY + 23)) //Top scroll
		{
			g_LastSelectedObject = 0;
			g_LastSelectedGump = index;
		}

		//Journal body
		int curposY = posY + 59;
		while (true)
		{
			int deltaHeight = (height + posY) - (curposY - 36);

			if (deltaHeight  < 70)
			{
				if (deltaHeight > 0)
				{
					if (UO->GumpPixelsInXY(0x0821, posX + 18, curposY, 0, deltaHeight))
					{
						g_LastSelectedObject = 0;
						g_LastSelectedGump = index;
					}
				}

				break;
			}
			else if (UO->GumpPixelsInXY(0x0821, posX + 18, curposY))
			{
				g_LastSelectedObject = 0;
				g_LastSelectedGump = index;

				break;
			}

			curposY += 70;

			deltaHeight = (height + posY) - (curposY - 36);

			if (deltaHeight < 70)
			{
				if (deltaHeight > 0)
				{
					if (UO->GumpPixelsInXY(0x0822, posX + 18, curposY, 0, deltaHeight))
					{
						g_LastSelectedObject = 0;
						g_LastSelectedGump = index;
					}
				}

				break;
			}
			else if (UO->GumpPixelsInXY(0x0822, posX + 18, curposY))
			{
				g_LastSelectedObject = 0;
				g_LastSelectedGump = index;

				break;
			}

			curposY += 70;
		}

		if (UO->GumpPixelsInXY(0x0823, posX + 18, posY + height + 34)) //Bottom scroll
		{
			g_LastSelectedObject = 0;
			g_LastSelectedGump = index;
		}

		if (g_LastSelectedGump == index)
		{
			if (UO->GumpPixelsInXY(0x00D2, posX + 40, posY + height + 43)) //Show System
				LSG = ID_GJ_SHOW_SYSTEM;
			else if (UO->GumpPixelsInXY(0x00D2, posX + 126, posY + height + 43)) //Show Objects
				LSG = ID_GJ_SHOW_OBJECTS;
			else if (UO->GumpPixelsInXY(0x00D2, posX + 210, posY + height + 43)) //Show Client
				LSG = ID_GJ_SHOW_CLIENTS;
			else if (UO->GumpPixelsInXY(0x082C, posX + 233, posY + height + 17)) //Lock
				LSG = ID_GJ_BUTTON_LOCK;
			else if (UO->GumpPixelsInXY(0x0824, posX + 249, posY + 56)) //^ button
				LSG = ID_GJ_BUTTON_UP;
			else if (UO->GumpPixelsInXY(0x0825, posX + 249, posY + height + 17)) //v button
				LSG = ID_GJ_BUTTON_DOWN;
		}
		
		if (UO->GumpPixelsInXY(0x001F, posX + 257, posY + scrollerY)) //Scroller
		{
			g_LastSelectedGump = index;
			LSG = ID_GJ_SCROLLER;
		}
		
		if (UO->GumpPixelsInXY(0x082E, posX + 137, posY + height + 66)) //Resize
		{
			g_LastSelectedGump = index;
			LSG = ID_GJ_BUTTON_RESIZE;
		}

		if (LSG != 0)
			g_LastSelectedObject = LSG;

		if (g_ShowGumpLocker && UO->PolygonePixelsInXY(posX, posY, 10, 14))
		{
			g_LastSelectedObject = ID_GJ_LOCK_MOVING;
			g_LastSelectedGump = index;
		}

		return LSG;
	}

	return 0;
}
//----------------------------------------------------------------------------
void TGumpJournal::OnLeftMouseUp()
{
	if (g_LastObjectLeftMouseDown != g_LastSelectedObject)
		return;

	DWORD ticks = GetTickCount();

	if (g_LastObjectLeftMouseDown == ID_GJ_BUTTON_MINIMIZE)
	{
		Minimized = true;
		GenerateFrame(MinimizedX, MinimizedY);
	}
	else if (g_LastObjectLeftMouseDown == ID_GJ_LOCK_MOVING)
	{
		LockMoving = !LockMoving;
		g_CancelDoubleClick = true;
	}
	else if (g_LastObjectLeftMouseDown == ID_GJ_BUTTON_RESIZE)
	{
		if (!m_HeightBuffer)
		{
			m_HeightBuffer = g_DroppedLeftMouseY;
			g_ResizedGump = this;
		}
		else
		{
			SetHeight(m_Height + (g_MouseY - m_HeightBuffer));
			m_HeightBuffer = 0;
			g_ResizedGump = NULL;
		}
	}
	else if (g_LastObjectLeftMouseDown == ID_GJ_SHOW_SYSTEM)
		g_JournalShowSystem = !g_JournalShowSystem;
	else if (g_LastObjectLeftMouseDown == ID_GJ_SHOW_OBJECTS)
		g_JournalShowObjects = !g_JournalShowObjects;
	else if (g_LastObjectLeftMouseDown == ID_GJ_SHOW_CLIENTS)
		g_JournalShowClient = !g_JournalShowClient;
	else if (g_LastObjectLeftMouseDown == ID_GJ_BUTTON_LOCK)
	{
		Journal->SelectionIndex = 0;
		UpdateFrame();
	}
	else if (g_LastObjectLeftMouseDown == ID_GJ_BUTTON_UP && m_LastScrollChangeTime < ticks)
	{
		if (Journal->SelectionIndex < Journal->Size)
			Journal->IncSelectionIndex();
		else
			Journal->SelectionIndex = Journal->Size;

		m_LastScrollChangeTime = ticks + SCROLL_LISTING_DELAY;
		UpdateFrame();
	}
	else if (g_LastObjectLeftMouseDown == ID_GJ_BUTTON_DOWN && m_LastScrollChangeTime < ticks)
	{
		if (Journal->SelectionIndex > 1)
			Journal->DecSelectionIndex();
		else
			Journal->SelectionIndex = 1;

		m_LastScrollChangeTime = ticks + SCROLL_LISTING_DELAY;
		UpdateFrame();
	}
}
//----------------------------------------------------------------------------
bool TGumpJournal::OnLeftMouseDoubleClick()
{
	if (Minimized)
	{
		Minimized = false;
		GenerateFrame(X, Y);

		return true;
	}

	return false;
}
//----------------------------------------------------------------------------
void TGumpJournal::OnMouseWheel(MOUSE_WHEEL_STATE &state)
{
	if (!Minimized && ! g_LeftMouseDown && !g_RightMouseDown)
	{
		DWORD ticks = GetTickCount();

		if (state == MWS_UP && m_LastScrollChangeTime < ticks)
		{
			if (Journal->SelectionIndex < Journal->Size)
				Journal->IncSelectionIndex();
			else
				Journal->SelectionIndex = Journal->Size;

			m_LastScrollChangeTime = ticks + (SCROLL_LISTING_DELAY / 4);
			UpdateFrame();
		}
		else if (state == MWS_DOWN && m_LastScrollChangeTime < ticks)
		{
			if (Journal->SelectionIndex > 1)
				Journal->DecSelectionIndex();
			else
				Journal->SelectionIndex = 1;

			m_LastScrollChangeTime = ticks + (SCROLL_LISTING_DELAY / 4);
			UpdateFrame();
		}
	}
}
//----------------------------------------------------------------------------