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

TTextTexture TGumpJournal::m_Text[3];
//----------------------------------------------------------------------------
TGumpJournal::TGumpJournal(DWORD serial, short x, short y, bool minimized)
: TGump(GT_JOURNAL, serial, x, y), m_Height(200), m_HeightBuffer(0),
m_LastScrollChangeTime(0)
{
	if (minimized)
	{
		m_Minimized = true;
		m_MinimizedX = x;
		m_MinimizedY = y;
	}
}
//----------------------------------------------------------------------------
TGumpJournal::~TGumpJournal()
{
}
//----------------------------------------------------------------------------
void TGumpJournal::InitTextTextures()
{
	FontManager->GenerateA(1, m_Text[0], "System", 0x0386);
	FontManager->GenerateA(1, m_Text[1], "Objects", 0x0386);
	FontManager->GenerateA(1, m_Text[2], "Client", 0x0386);
}
//----------------------------------------------------------------------------
void TGumpJournal::ReleaseTextTextures()
{
	IFOR(i, 0, 3)
		m_Text[i].Clear();
}
//----------------------------------------------------------------------------
void TGumpJournal::PrepareTextures()
{
	Orion->ExecuteGump(0x001F);
	Orion->ExecuteGumpPart(0x00D2, 2);
	Orion->ExecuteGumpPart(0x0820, 6);
	Orion->ExecuteGumpPart(0x082A, 7);
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
void TGumpJournal::GenerateFrame()
{
	if (!g_DrawMode)
	{
		m_FrameRedraw = false;
		m_FrameCreated = false;

		return;
	}

	CalculateGumpState();

	glNewList((GLuint)this, GL_COMPILE);

		if (m_Minimized)
		{
			Orion->DrawGump(0x0830, 0, 0, 0);

			glEndList();

			m_FrameRedraw = true;
			m_FrameCreated = true;

			return;
		}

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

		Orion->DrawGump(0x082D, 0, 137, 0); //Minimize
		Orion->DrawGump(0x0820, 0, 0, 23); //Top scroll
		Orion->DrawGump(0x082A, 0, 111, 34); //Journal text gump
		
		//Journal body
		int curposY = 59;
		
		while (true)
		{
			int deltaHeight = height - (curposY - 36);

			if (deltaHeight  < 70)
			{
				if (deltaHeight > 0)
					Orion->DrawGump(0x0821, 0, 18, curposY, 0, deltaHeight);

				break;
			}
			else
				Orion->DrawGump(0x0821, 0, 18, curposY);

			curposY += 70;

			deltaHeight = height - (curposY - 36);

			if (deltaHeight < 70)
			{
				if (deltaHeight > 0)
					Orion->DrawGump(0x0822, 0, 18, curposY, 0, deltaHeight);

				break;
			}
			else
				Orion->DrawGump(0x0822, 0, 18, curposY);

			curposY += 70;
		}
		
		//Up
		//UO->DrawGump(0x001F, 0, posX + 257, posY + 72); //Scroller
		//Down
		//UO->DrawGump(0x001F, 0, posX + 257, posY + Height - 17); //Scroller
		
		int scrollerY = height - 17;

		if (g_GumpPressedScroller && g_LastObjectLeftMouseDown == ID_GJ_SCROLLER) //Scroller pressed
		{
			int currentY = (g_MouseY - 10) - ((int)g_GumpTranslateY + 72); //Scroller position

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
		
		Orion->DrawGump(0x001F, 0, 257, scrollerY); //Scroller
		
		Orion->DrawGump(0x082B, 0, 30, 58); //Top line
		Orion->DrawGump(0x0824, 0, 249, 56); //^ button
		Orion->DrawGump(0x082B, 0, 31, height + 21); //Bottom line
		Orion->DrawGump(0x0825, 0, 249, height + 17); //v button
		
		if (Journal->SelectionIndex)
			Orion->DrawGump(0x082C, 0, 233, height + 17); //Lock
		
		Orion->DrawGump(0x0823, 0, 18, height + 34); //Bottom scroll
		
		Orion->DrawGump(0x00D2 + (int)g_JournalShowSystem, 0, 40, height + 43); //Show System
		m_Text[0].Draw(63, height + 47);
		
		Orion->DrawGump(0x00D2 + (int)g_JournalShowObjects, 0, 126, height + 43); //Show Objects
		m_Text[1].Draw(149, height + 47);
		
		Orion->DrawGump(0x00D2 + (int)g_JournalShowClient, 0, 210, height + 43); //Show Client
		m_Text[2].Draw(233, height + 47);
		
		WORD gumpID = 0x082E + (int)(g_GumpSelectElement == ID_GJ_BUTTON_RESIZE);
		Orion->DrawGump(gumpID, 0, 137, height + 66); //Resize


		g_GL.Scissor((int)g_GumpTranslateX + 38, (int)g_GumpTranslateY + 70, 214, height - 50);

		//Отрисовка сообщений систем чата
		int textOffsY = height + 16;
		int textYBounds = 70;
		int drawX = 38;

		int journalHeight = 0;

		if (g_JournalShowSystem)
			journalHeight += Journal->SystemHeight;

		if (g_JournalShowObjects)
			journalHeight += Journal->ObjectsHeight;

		if (g_JournalShowClient)
			journalHeight += Journal->ClientHeight;

		if (journalHeight < height - 50)
		{
			textYBounds = height;
			textOffsY = 70;

			TTextData *td = Journal->m_Top;

			while (td != NULL)
			{
				//if (TextOffsY > TextYBounds) break;

				if (td->CanBeDrawedInJournalGump())
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
					textYBounds = height;
					textOffsY = 70;

					TTextData *td = Journal->m_Top;

					while (td != NULL)
					{
						if (textOffsY > textYBounds)
							break;

						if (td->CanBeDrawedInJournalGump())
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
						currentID++;

						if (td->CanBeDrawedInJournalGump())
						{
							TTextTexture &tth = td->m_Texture;
							testHeight += tth.Height;

							last_td = td;
						}

						if (currentID >= maxID)
							break;

						td = td->m_Prev;
					}

					textYBounds = height;
					textOffsY = 70;

					td = last_td;

					while (td != NULL)
					{
						if (textOffsY > textYBounds)
							break;

						if (td->CanBeDrawedInJournalGump())
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

					if (td->CanBeDrawedInJournalGump())
					{
						TTextTexture &tth = td->m_Texture;

						textOffsY -= tth.Height;
						tth.Draw(drawX, textOffsY);
					}

					td = td->m_Prev;
				}
			}
		}

		glDisable(GL_SCISSOR_TEST);

	glEndList();

	m_FrameRedraw = true;
	m_FrameCreated = true;
}
//----------------------------------------------------------------------------
int TGumpJournal::Draw(bool &mode)
{
	DWORD index = (DWORD)this;

	CalculateGumpState();

	if (Minimized)
		m_HeightBuffer = 0;
	
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
			m_FrameCreated = false;
	}

	DWORD ticks = g_Ticks;

	if (g_GumpPressedScroller && m_LastScrollChangeTime < ticks)
	{
		if (g_LastObjectLeftMouseDown == ID_GJ_BUTTON_UP)
		{
			if (Journal->SelectionIndex < Journal->Size)
				Journal->IncSelectionIndex();
			else
				Journal->SelectionIndex = Journal->Size;
		}
		else if (g_LastObjectLeftMouseDown == ID_GJ_BUTTON_DOWN)
		{
			if (Journal->SelectionIndex > 1)
				Journal->DecSelectionIndex();
			else
				Journal->SelectionIndex = 1;
		}

		m_LastScrollChangeTime = ticks + SCROLL_LISTING_DELAY;
	}
	
	int scrollerY = height - 17;

	if (!m_Minimized) //Check scroller
	{
		if (g_GumpPressedScroller && g_LastObjectLeftMouseDown == ID_GJ_SCROLLER) //Scroller pressed
		{
			int currentY = (g_MouseY - 10) - ((int)g_GumpTranslateY + 72); //Scroller position

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
				m_FrameCreated = false;
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
				m_FrameCreated = false;
		}
	}

	if (mode)
	{
		if (!m_FrameCreated || (g_GumpSelectElement && !m_HeightBuffer) || g_GumpMovingOffsetX || g_GumpMovingOffsetY)
			GenerateFrame();
		else if (m_FrameRedraw)
		{
			GenerateFrame();
			FrameRedraw = false;
		}

		glTranslatef(g_GumpTranslateX, g_GumpTranslateY, 0.0f);

		glCallList((GLuint)index);

		if (m_Minimized)
		{
			glTranslatef(-g_GumpTranslateX, -g_GumpTranslateY, 0.0f);

			return 0;
		}

		DrawLocker();

		glTranslatef(-g_GumpTranslateX, -g_GumpTranslateY, 0.0f);
	}
	else
	{
		int oldMouseX = g_MouseX;
		int oldMouseY = g_MouseY;
		g_MouseX -= (int)g_GumpTranslateX;
		g_MouseY -= (int)g_GumpTranslateY;

		if (m_Minimized)
		{
			if (Orion->GumpPixelsInXY(0x0830, 0, 0))
			{
				g_LastSelectedObject = 0;
				g_LastSelectedGump = index;
			}

			return 0;
		}
		
		int LSG = 0;
		
		if (Orion->GumpPixelsInXY(0x082D, 137, 0)) //Minimize
		{
			LSG = ID_GJ_BUTTON_MINIMIZE;
			g_LastSelectedGump = index;
		}
		
		if (Orion->GumpPixelsInXY(0x0820, 0, 23)) //Top scroll
		{
			g_LastSelectedObject = 0;
			g_LastSelectedGump = index;
		}

		//Journal body
		int curposY = 59;

		while (true)
		{
			int deltaHeight = height - (curposY - 36);

			if (deltaHeight  < 70)
			{
				if (deltaHeight > 0)
				{
					if (Orion->GumpPixelsInXY(0x0821, 18, curposY, 0, deltaHeight))
					{
						g_LastSelectedObject = 0;
						g_LastSelectedGump = index;
					}
				}

				break;
			}
			else if (Orion->GumpPixelsInXY(0x0821, 18, curposY))
			{
				g_LastSelectedObject = 0;
				g_LastSelectedGump = index;

				break;
			}

			curposY += 70;

			deltaHeight = height - (curposY - 36);

			if (deltaHeight < 70)
			{
				if (deltaHeight > 0)
				{
					if (Orion->GumpPixelsInXY(0x0822, 18, curposY, 0, deltaHeight))
					{
						g_LastSelectedObject = 0;
						g_LastSelectedGump = index;
					}
				}

				break;
			}
			else if (Orion->GumpPixelsInXY(0x0822, 18, curposY))
			{
				g_LastSelectedObject = 0;
				g_LastSelectedGump = index;

				break;
			}

			curposY += 70;
		}

		if (Orion->GumpPixelsInXY(0x0823, 18, height + 34)) //Bottom scroll
		{
			g_LastSelectedObject = 0;
			g_LastSelectedGump = index;
		}

		if (Orion->GumpPixelsInXY(0x00D2, 40, height + 43)) //Show System
			LSG = ID_GJ_SHOW_SYSTEM;
		else if (Orion->GumpPixelsInXY(0x00D2, 126, height + 43)) //Show Objects
			LSG = ID_GJ_SHOW_OBJECTS;
		else if (Orion->GumpPixelsInXY(0x00D2, 210, height + 43)) //Show Client
			LSG = ID_GJ_SHOW_CLIENTS;
		else if (Orion->GumpPixelsInXY(0x082C, 233, height + 17)) //Lock
			LSG = ID_GJ_BUTTON_LOCK;
		else if (Orion->GumpPixelsInXY(0x0824, 249, 56)) //^ button
			LSG = ID_GJ_BUTTON_UP;
		else if (Orion->GumpPixelsInXY(0x0825, 249, height + 17)) //v button
			LSG = ID_GJ_BUTTON_DOWN;
		
		if (Orion->GumpPixelsInXY(0x001F, 257, scrollerY)) //Scroller
			LSG = ID_GJ_SCROLLER;
		
		if (Orion->GumpPixelsInXY(0x082E, 137, height + 66)) //Resize
			LSG = ID_GJ_BUTTON_RESIZE;

		if (g_ShowGumpLocker && Orion->PolygonePixelsInXY(0, 0, 10, 14))
			LSG = ID_GJ_LOCK_MOVING;

		g_MouseX = oldMouseX;
		g_MouseY = oldMouseY;

		return LSG;
	}

	return 0;
}
//----------------------------------------------------------------------------
void TGumpJournal::OnLeftMouseUp()
{
	if (g_LastObjectLeftMouseDown != g_LastSelectedObject)
		return;

	DWORD ticks = g_Ticks;

	if (g_LastObjectLeftMouseDown == ID_GJ_BUTTON_MINIMIZE)
	{
		m_Minimized = true;
		m_FrameCreated = false;
	}
	else if (g_LastObjectLeftMouseDown == ID_GJ_LOCK_MOVING)
	{
		m_LockMoving = !m_LockMoving;
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
		m_FrameCreated = false;
	}
	else if (g_LastObjectLeftMouseDown == ID_GJ_BUTTON_UP && m_LastScrollChangeTime < ticks)
	{
		if (Journal->SelectionIndex < Journal->Size)
			Journal->IncSelectionIndex();
		else
			Journal->SelectionIndex = Journal->Size;

		m_LastScrollChangeTime = ticks + SCROLL_LISTING_DELAY;
		m_FrameCreated = false;
	}
	else if (g_LastObjectLeftMouseDown == ID_GJ_BUTTON_DOWN && m_LastScrollChangeTime < ticks)
	{
		if (Journal->SelectionIndex > 1)
			Journal->DecSelectionIndex();
		else
			Journal->SelectionIndex = 1;

		m_LastScrollChangeTime = ticks + SCROLL_LISTING_DELAY;
		m_FrameCreated = false;
	}
}
//----------------------------------------------------------------------------
bool TGumpJournal::OnLeftMouseDoubleClick()
{
	if (m_Minimized)
	{
		m_Minimized = false;
		m_FrameCreated = false;

		return true;
	}

	return false;
}
//----------------------------------------------------------------------------
void TGumpJournal::OnMouseWheel(MOUSE_WHEEL_STATE &state)
{
	if (!m_Minimized && !g_LeftMouseDown && !g_RightMouseDown)
	{
		DWORD ticks = g_Ticks;

		if (state == MWS_UP && m_LastScrollChangeTime < ticks)
		{
			if (Journal->SelectionIndex < Journal->Size)
				Journal->IncSelectionIndex();
			else
				Journal->SelectionIndex = Journal->Size;

			m_LastScrollChangeTime = ticks + (SCROLL_LISTING_DELAY / 4);
			m_FrameCreated = false;
		}
		else if (state == MWS_DOWN && m_LastScrollChangeTime < ticks)
		{
			if (Journal->SelectionIndex > 1)
				Journal->DecSelectionIndex();
			else
				Journal->SelectionIndex = 1;

			m_LastScrollChangeTime = ticks + (SCROLL_LISTING_DELAY / 4);
			m_FrameCreated = false;
		}
	}
}
//----------------------------------------------------------------------------