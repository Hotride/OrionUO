/****************************************************************************
**
** GumpStatusbar.cpp
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
TGumpStatusbar::TGumpStatusbar(DWORD serial, short x, short y, bool minimized)
: TGump(GT_STATUSBAR, serial, x, y), m_GroupNext(NULL), m_GroupPrev(NULL)
{
	TextEntry = new TEntryText(32, 150, 150);

	if (minimized)
	{
		m_Minimized = true;
		m_MinimizedX = x;
		m_MinimizedY = y;
	}
	
	TGameObject *character = World->FindWorldObject(serial);
	if (character != NULL)
		TextEntry->SetText(character->GetName());
}
//---------------------------------------------------------------------------
TGumpStatusbar::~TGumpStatusbar()
{
	if (ConnectionManager.Connected() && World != NULL && World->FindWorldObject(m_Serial) != NULL)
	{
		TPacketCloseStatusbarGump packet(m_Serial);
		packet.Send();
	}

	RemoveFromGroup();

	if (TextEntry != NULL)
	{
		delete TextEntry;
		TextEntry = NULL;
	}
}
//---------------------------------------------------------------------------
TGumpStatusbar *TGumpStatusbar::GetTopStatusbar()
{
	if (!InGroup())
		return NULL;

	if (m_GroupPrev == NULL)
		return this;

	TGumpStatusbar *gump = m_GroupPrev;

	while (gump != NULL && gump->m_GroupPrev != NULL)
		gump = gump->m_GroupPrev;

	return gump;
}
//---------------------------------------------------------------------------
TGumpStatusbar *TGumpStatusbar::GetNearStatusbar(int &x, int &y)
{
	if (InGroup() || !m_Minimized)
		return NULL;

	//154x59 mini-gump
	
	const int rangeX = 77;
	const int rangeY = 29;
	const int rangeOffsetX = 60;
	const int rangeOffsetY = 24;

	TGump *gump = (TGump*)GumpManager->m_Items;

	while (gump != NULL)
	{
		if (gump != this && gump->GumpType == GT_STATUSBAR && gump->Minimized)
		{
			int gumpX = gump->MinimizedX;
			int offsetX = abs(x - gumpX);
			int passed = 0;
			
			if (x >= gumpX && x <= (gumpX + 154))
				passed = 2;
			else if (offsetX < rangeOffsetX) //left part of gump
				passed = 1;
			else
			{
				offsetX = abs(x - (gumpX + 154));

				if (offsetX < rangeOffsetX) //right part of gump
					passed = -1;
				else if (x >= (gumpX - rangeX) && x <= (gumpX + 154 + rangeX))
					passed = 2;
			}

			int gumpY = gump->MinimizedY;

			if (abs(passed) == 1)
			{
				if (y < (gumpY - rangeY) || y > (gumpY + 59 + rangeY))
					passed = 0;
			}
			else if (passed == 2) //in gump range X
			{
				int offsetY = abs(y - gumpY);

				if (offsetY < rangeOffsetY) //top part of gump
					passed = 2;
				else
				{
					offsetY = abs(y - (gumpY + 59));

					if (offsetY < rangeOffsetY) //bottom part of gump
						passed = -2;
					else
						passed = 0;
				}
			}

			if (passed)
			{
				int testX = gumpX;
				int testY = gumpY;

				switch (passed)
				{
					case -2: //gump bottom
					{
						testY += 59;
						break;
					}
					case -1: //gump right
					{
						testX += 154;
						break;
					}
					case 1: //gump left
					{
						testX -= 154;
						break;
					}
					case 2: //gump top
					{
						testY -= 59;
						break;
					}
					default:
						break;
				}

				TGump *testGump = (TGump*)GumpManager->m_Items;

				while (testGump != NULL)
				{
					if (testGump != this && testGump->GumpType == GT_STATUSBAR && testGump->Minimized)
					{
						if (testGump->MinimizedX == testX && testGump->MinimizedY == testY)
							break;
					}

					testGump = (TGump*)testGump->m_Next;
				}

				if (testGump == NULL)
				{
					x = testX;
					y = testY;

					break;
				}
			}
		}

		gump = (TGump*)gump->m_Next;
	}

	return (TGumpStatusbar*)gump;
}
//---------------------------------------------------------------------------
bool TGumpStatusbar::GetStatusbarGroupOffset(int &x, int &y)
{
	if (InGroup() && m_Minimized && g_LeftMouseDown && !g_LastObjectLeftMouseDown)
	{
		TGumpStatusbar *gump = GetTopStatusbar();

		while (gump != NULL)
		{
			//Если гамп захватили и (может быть) двигают
			if (gump != this && g_LastGumpLeftMouseDown == (DWORD)gump)
			{
				x += g_MouseX - g_DroppedLeftMouseX;
				y += g_MouseY - g_DroppedLeftMouseY;

				return true;
			}

			gump = gump->m_GroupNext;
		}
	}

	return false;
}
//---------------------------------------------------------------------------
void TGumpStatusbar::UpdateGroup(int x, int y)
{
	if (!InGroup())
		return;

	TGumpStatusbar *gump = GetTopStatusbar();

	while (gump != NULL)
	{
		if (gump != this)
		{
			gump->AddMinimizedX(x);
			gump->AddMinimizedY(y);

			GumpManager->MoveToBack(gump);
			gump->FrameCreated = false;
		}

		gump = gump->m_GroupNext;
	}

	GumpManager->MoveToBack(this);
}
//---------------------------------------------------------------------------
void TGumpStatusbar::AddStatusbar(TGumpStatusbar *bar)
{
	if (m_GroupNext == NULL)
	{
		m_GroupNext = bar;
		bar->m_GroupPrev = this;
		bar->m_GroupNext = NULL;
	}
	else
	{
		TGumpStatusbar *gump = m_GroupNext;

		while (gump != NULL && gump->m_GroupNext != NULL)
			gump = gump->m_GroupNext;

		gump->m_GroupNext = bar;
		bar->m_GroupPrev = gump;
		bar->m_GroupNext = NULL;
	}

	m_FrameCreated = false;
}
//---------------------------------------------------------------------------
void TGumpStatusbar::RemoveFromGroup()
{
	if (m_GroupNext != NULL)
		m_GroupNext->m_GroupPrev = m_GroupPrev;
	
	if (m_GroupPrev != NULL)
		m_GroupPrev->m_GroupNext = m_GroupNext;

	m_GroupNext = NULL;
	m_GroupPrev = NULL;
}
//---------------------------------------------------------------------------
void TGumpStatusbar::PrepareTextures()
{
	UO->ExecuteGumpPart(0x0802, 8);

	if (ConnectionManager.ClientVersion >= CV_308D)
		UO->ExecuteGump(0x2A6C);
}
//---------------------------------------------------------------------------
void TGumpStatusbar::GenerateFrame()
{
	if (!g_DrawMode)
	{
		m_FrameRedraw = false;
		m_FrameCreated = false;

		return;
	}
	
	if (!ConfigManager.DisableNewTargetSystem && m_Serial == NewTargetSystem.Serial)
		GumpManager->UpdateGump(m_Serial, 0, GT_TARGET_SYSTEM);

	CalculateGumpState();

	glNewList((GLuint)this, GL_COMPILE);

	if (m_Serial == g_PlayerSerial) //Если это статусбар игрока
	{
		if (!m_Minimized) //Если это "полная" версия статусбара
		{
			char text[30] = {0};

			WORD gumpID = 0x0802;
			if (ConnectionManager.ClientVersion >= CV_308D)
				gumpID = 0x2A6C;

			UO->DrawGump(gumpID, 0, 0, 0); //Гамп статусбара

			//Отрисовка набора характеристик, расположение в зависимости от версии протокола, комментировать не буду...
			if (ConnectionManager.ClientVersion >= CV_308Z)
			{
				//Отрисуем имя игрока
				if (g_Player->GetName().length())
					FontManager->DrawA(1, g_Player->GetName().c_str(), 0x0386, 58, 50, 320, TS_CENTER);
				


				//Кнопка вызова гампа бафов
				if (ConnectionManager.ClientVersion >= CV_5020)
					UO->DrawGump(0x7538, 0, 40, 50);



				sprintf(text, "%d", g_Player->Str);
				FontManager->DrawA(1, text, 0x0386, 88, 77);

				sprintf(text, "%d", g_Player->Dex);
				FontManager->DrawA(1, text, 0x0386, 88, 105);

				sprintf(text, "%d", g_Player->Int);
				FontManager->DrawA(1, text, 0x0386, 88, 133);
				
				
				
				int textWidth = 40;

				//Hits
				sprintf(text, "%d", g_Player->Hits);
				FontManager->DrawA(1, text, 0x0386, 146, 70, textWidth, TS_CENTER);
				
				glColor4f(0.22f, 0.22f, 0.22f, 1.0f);
				g_GL.DrawLine(146, 82, 180, 82);
				glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

				sprintf(text, "%d", g_Player->MaxHits);
				FontManager->DrawA(1, text, 0x0386, 146, 83, textWidth, TS_CENTER);
				
				//Stam
				sprintf(text, "%d", g_Player->Stam);
				FontManager->DrawA(1, text, 0x0386, 146, 98, textWidth, TS_CENTER);
				
				glColor4f(0.22f, 0.22f, 0.22f, 1.0f);
				g_GL.DrawLine(146, 110, 180, 110);
				glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

				sprintf(text, "%d", g_Player->MaxStam);
				FontManager->DrawA(1, text, 0x0386, 146, 111, textWidth, TS_CENTER);

				//Mana
				sprintf(text, "%d", g_Player->Mana);
				FontManager->DrawA(1, text, 0x0386, 146, 126, textWidth, TS_CENTER);
				
				glColor4f(0.22f, 0.22f, 0.22f, 1.0f);
				g_GL.DrawLine(146, 138, 180, 138);
				glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

				sprintf(text, "%d", g_Player->MaxMana);
				FontManager->DrawA(1, text, 0x0386, 146, 139, textWidth, TS_CENTER);


				
				sprintf(text, "%d", g_Player->StatsCap);
				FontManager->DrawA(1, text, 0x0386, 220, 77);

				sprintf(text, "%d", g_Player->Luck);
				FontManager->DrawA(1, text, 0x0386, 220, 105);
				
				//Weights
				sprintf(text, "%d", g_Player->Weight);
				FontManager->DrawA(1, text, 0x0386, 216, 126, textWidth, TS_CENTER);
				
				glColor4f(0.22f, 0.22f, 0.22f, 1.0f);
				g_GL.DrawLine(216, 138, 250, 138);
				glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

				sprintf(text, "%d", g_Player->MaxWeight);
				FontManager->DrawA(1, text, 0x0386, 216, 139, textWidth, TS_CENTER);

				

				sprintf(text, "%d-%d", g_Player->MinDamage, g_Player->MaxDamage);
				FontManager->DrawA(1, text, 0x0386, 280, 77);

				sprintf(text, "%d", g_Player->Gold);
				FontManager->DrawA(1, text, 0x0386, 280, 105);
				
				sprintf(text, "%d/%d", g_Player->Followers, g_Player->MaxFollowers);
				FontManager->DrawA(1, text, 0x0386, 280 + 5, 133);
				


				sprintf(text, "%d", g_Player->Armor);
				FontManager->DrawA(1, text, 0x0386, 354, 76);
				
				sprintf(text, "%d", g_Player->FireResistance);
				FontManager->DrawA(1, text, 0x0386, 354, 92);
				
				sprintf(text, "%d", g_Player->ColdResistance);
				FontManager->DrawA(1, text, 0x0386, 354, 106);
				
				sprintf(text, "%d", g_Player->PoisonResistance);
				FontManager->DrawA(1, text, 0x0386, 354, 120);
				
				sprintf(text, "%d", g_Player->EnergyResistance);
				FontManager->DrawA(1, text, 0x0386, 354, 134);
			}
			else
			{
				//Отрисуем имя игрока
				if (g_Player->GetName().length())
					FontManager->DrawA(1, g_Player->GetName().c_str(), 0x0386, 86, 42);

				sprintf(text, "%d", g_Player->Str);
				FontManager->DrawA(1, text, 0x0386, 86, 61);

				sprintf(text, "%d", g_Player->Dex);
				FontManager->DrawA(1, text, 0x0386, 86, 73);

				sprintf(text, "%d", g_Player->Int);
				FontManager->DrawA(1, text, 0x0386, 86, 85);

				sprintf(text, "%s", (g_Player->Sex ? "F" : "M"));
				FontManager->DrawA(1, text, 0x0386, 86, 97);

				sprintf(text, "%d", g_Player->Armor);
				FontManager->DrawA(1, text, 0x0386, 86, 109);

				sprintf(text, "%d/%d", g_Player->Hits, g_Player->MaxHits);
				FontManager->DrawA(1, text, 0x0386, 171, 61);

				sprintf(text, "%d/%d", g_Player->Mana, g_Player->MaxMana);
				FontManager->DrawA(1, text, 0x0386, 171, 73);

				sprintf(text, "%d/%d", g_Player->Stam, g_Player->MaxStam);
				FontManager->DrawA(1, text, 0x0386, 171, 85);

				sprintf(text, "%d", g_Player->Gold);
				FontManager->DrawA(1, text, 0x0386, 171, 97);

				sprintf(text, "%d", g_Player->Weight);
				FontManager->DrawA(1, text, 0x0386, 171, 109);
			
				if (ConnectionManager.ClientVersion == CV_308D)
				{
					sprintf(text, "%d", g_Player->StatsCap);
					FontManager->DrawA(1, text, 0x0386, 171, 124);
				}
				else if (ConnectionManager.ClientVersion == CV_308J)
				{
					sprintf(text, "%d", g_Player->StatsCap);
					FontManager->DrawA(1, text, 0x0386, 180, 131);
				
					sprintf(text, "%d/%d", g_Player->Followers, g_Player->MaxFollowers);
					FontManager->DrawA(1, text, 0x0386, 180, 144);
				}
			}
		}
		else //Это уменьшенная врсия статусбара (с полосками)
		{
			if (Party.Leader != 0) //inParty
			{
				FontManager->DrawA(3, "[* SELF *]", 0x0481, 16, -2);

				WORD gumpID = 0x0938 + (g_GumpPressedElement == ID_GSB_BUTTON_HEAL_1 ? 2 : 0);
				UO->DrawGump(gumpID, 0, 16, 20);

				gumpID = 0x0939 + (int)(g_GumpPressedElement == ID_GSB_BUTTON_HEAL_2);
				UO->DrawGump(gumpID, 0, 16, 33);

				//Hits
				UO->DrawGump(0x0028, 0, 34, 20);

				int per = CalculatePercents(g_Player->MaxHits, g_Player->Hits, 96);
				if (per > 0)
					UO->DrawGump(0x0029, 0, 34, 20, per, 0);
				
				//Mana
				UO->DrawGump(0x0028, 0, 34, 33);

				per = CalculatePercents(g_Player->MaxMana, g_Player->Mana, 96);
				if (per > 0)
					UO->DrawGump(0x0029, 0x0482, 34, 33, per, 0); //0x0170 green //0x0035 yellow
				
				//Stam
				UO->DrawGump(0x0028, 0, 34, 45);

				per = CalculatePercents(g_Player->MaxStam, g_Player->Stam, 96);
				if (per > 0)
					UO->DrawGump(0x0029, 0x0075, 34, 45, per, 0);
			}
			else
			{
				WORD gumpid = 0x0803; //Гамп статусбара
				if (g_Player->Warmode)
					gumpid = 0x0807; //Версия с включенным вармодом
				UO->DrawGump(gumpid, 0, 0, 0);
			
				//Hits
				UO->DrawGump(0x0805, 0, 34, 12);

				int per = CalculatePercents(g_Player->MaxHits, g_Player->Hits, 109);
				if (per > 0)
				{
					gumpid = 0x0806; //Character status line (blue)
					if (g_Player->Poisoned())
						gumpid = 0x0808; //Character status line (green)
					else if (g_Player->YellowHits())
						gumpid = 0x0809; //Character status line (yellow)
				
					UO->DrawGump(gumpid, 0, 34, 12, per, 0);
				}
				
				//Mana
				UO->DrawGump(0x0805, 0, 34, 25);

				per = CalculatePercents(g_Player->MaxMana, g_Player->Mana, 109);
				if (per > 0)
					UO->DrawGump(0x0806, 0, 34, 25, per, 0);
				
				//Stam
				UO->DrawGump(0x0805, 0, 34, 38);

				per = CalculatePercents(g_Player->MaxStam, g_Player->Stam, 109);
				if (per > 0)
					UO->DrawGump(0x0806, 0, 34, 38, per, 0);
			}
			
			if (InGroup())
				UO->DrawGump(0x082C, 0, 136, 24);
		}
	}
	else //Чужой статусбар
	{
		if (Party.Contains(m_Serial))
		{
			IFOR(i, 0, 10)
			{
				if (Party.Member[i].Serial == m_Serial)
				{
					TPartyObject &member = Party.Member[i];
					
					//Вычисляем цвет статусбара
					WORD color = ConfigManager.GetColorByNotoriety(member.Notoriety);
					string memberName = member.GetName(0);

					if (member.CanChangeName) //Если игрок может изменить имя существу
					{
						if (TextEntry != EntryPointer && !TextEntry->Length())
							TextEntry->SetText(memberName);

						TextEntry->DrawA(1, 0x000E, 16, -2, TS_LEFT, UOFONT_FIXED);
					}
					else if (memberName.length())
						FontManager->DrawA(1, memberName.c_str(), 0x0386, 16, -2);
					else if (TextEntry->Length())
						TextEntry->DrawA(1, 0x0386, 16, -2, TS_LEFT, UOFONT_FIXED);
		
					WORD gumpID = 0x0938 + (g_GumpPressedElement == ID_GSB_BUTTON_HEAL_1 ? 2 : 0);
					UO->DrawGump(gumpID, 0, 16, 20);

					gumpID = 0x0939 + (int)(g_GumpPressedElement == ID_GSB_BUTTON_HEAL_2);
					UO->DrawGump(gumpID, 0, 16, 33);

					//Hits
					UO->DrawGump(0x0028, 0, 34, 20);

					int per = CalculatePercents(member.MaxHits, member.Hits, 96);
					if (per > 0)
						UO->DrawGump(0x0029, 0, 34, 20, per, 0);
				
					//Mana
					UO->DrawGump(0x0028, 0, 34, 33);

					per = CalculatePercents(member.MaxMana, member.Mana, 96);
					if (per > 0)
						UO->DrawGump(0x0029, 0x0482, 34, 33, per, 0); //0x0170 green //0x0035 yellow
				
					//Stam
					UO->DrawGump(0x0028, 0, 34, 45);
					
					per = CalculatePercents(member.MaxStam, member.Stam, 96);
					if (per > 0)
						UO->DrawGump(0x0029, 0x0075, 34, 45, per, 0);

					break;
				}
			}
		}
		else
		{
			TGameCharacter *obj = World->FindWorldCharacter(m_Serial);
			if (obj != NULL)
			{
				//Вычисляем цвет статусбара
				WORD color = ConfigManager.GetColorByNotoriety(obj->Notoriety);

				if (obj->Notoriety == NT_CRIMINAL || obj->Notoriety == NT_SOMEONE_GRAY)
					color = 0;

				ColorizerShader->Use();

				//Гамп статус бара
				UO->DrawGump(0x0804, color, 0, 0);

				UnuseShader();

				string objName = obj->GetName();

				if (obj->CanChangeName) //Если игрок может изменить имя существу
				{
					if (TextEntry != EntryPointer && !TextEntry->Length())
						TextEntry->SetText(objName);

					TextEntry->DrawA(1, 0x000E, 16, 14, TS_LEFT, UOFONT_FIXED);
				}
				else if (objName.length())
					FontManager->DrawA(1, objName.c_str(), 0x0386, 16, 14, 150, TS_LEFT, UOFONT_FIXED);
				else if (TextEntry->Length())
					TextEntry->DrawA(1, 0x0386, 16, 14, TS_LEFT, UOFONT_FIXED);
				
				//Hits
				UO->DrawGump(0x0805, 0, 34, 38);
				
				int per = CalculatePercents(obj->MaxHits, obj->Hits, 109);
				if (per > 0)
				{
					WORD gumpid = 0x0806; //Character status line (blue)
					if (obj->Poisoned())
						gumpid = 0x0808; //Character status line (green)
					else if (obj->YellowHits())
						gumpid = 0x0809; //Character status line (yellow)

					UO->DrawGump(gumpid, 0, 34, 38, per, 0);
				}
			}
			else //Серенький статус
			{
				ColorizerShader->Use();

				//Гамп статус бара
				UO->DrawGump(0x0804, 0x0386, 0, 0);

				//Hits
				UO->DrawGump(0x0805, 0x0386, 34, 38);
				
				UnuseShader();

				TextEntry->DrawA(1, 0x0386, 16, 14, TS_LEFT, UOFONT_FIXED);
			}
		}

		if (InGroup())
			UO->DrawGump(0x082C, 0, 136, 24);
	}

	glEndList();
	
	m_FrameCreated = true;
}
//----------------------------------------------------------------------------
int TGumpStatusbar::Draw(bool &mode)
{
	DWORD index = (DWORD)this;

	CalculateGumpState();

	//Если гамп захватили и (может быть) двигают
	if (g_GumpMovingOffsetX || g_GumpMovingOffsetX)
	{
		if (Target.IsTargeting())
			g_GeneratedMouseDown = true;

		if (!InGroup())
		{
			int testX = g_MouseX;
			int testY = g_MouseY;

			if (GetNearStatusbar(testX, testY) != NULL)
			{
				g_GumpTranslateX = (float)testX;
				g_GumpTranslateY = (float)testY;
			}
		}
	}
	else if (InGroup())
	{
		int x = (int)g_GumpTranslateX;
		int y = (int)g_GumpTranslateY;

		GetStatusbarGroupOffset(x, y);

		g_GumpTranslateX = (float)x;
		g_GumpTranslateY = (float)y;
	}

	if (mode) //Отрисовка
	{
		//Если список отображения не был сделан - сделаем его
		if (!m_FrameCreated)
			GenerateFrame();
		else if (m_FrameRedraw)
		{
			GenerateFrame();
			m_FrameRedraw = false;
		}

		glTranslatef(g_GumpTranslateX, g_GumpTranslateY, 0.0f);

		//Рисуем заранее заготовленный список отображения
		glCallList((GLuint)index);

		DrawLocker();

		glTranslatef(-g_GumpTranslateX, -g_GumpTranslateY, 0.0f);
	}
	else //Выбор объектов
	{
		int oldMouseX = g_MouseX;
		int oldMouseY = g_MouseY;
		g_MouseX -= (int)g_GumpTranslateX;
		g_MouseY -= (int)g_GumpTranslateY;

		int LSG = 0;

		if (m_Serial == g_PlayerSerial) //Гамп для игрока
		{
			if (!m_Minimized) //Полная версия
			{
				WORD gumpID = 0x0802;
				POINT p = { 0, 0 };

				if (ConnectionManager.ClientVersion >= CV_308D)
				{
					gumpID = 0x2A6C;

					if (ConnectionManager.ClientVersion >= CV_308Z)
					{
						p.x = 389;
						p.y = 152;
					}
					else
					{
						p.x = 243;
						p.y = 150;
					}
				}
				else
				{
					p.x = 244;
					p.y = 112;
				}

				if (UO->GumpPixelsInXY(gumpID, 0, 0))
				{
					g_LastSelectedObject = 0;
					g_LastSelectedGump = index;
					
					//Кнопка вызова гампа бафов
					if (ConnectionManager.ClientVersion >= CV_5020 && UO->GumpPixelsInXY(0x7538, 40, 50))
						LSG = ID_GSB_BUFF_GUMP;
				}

				if (UO->PolygonePixelsInXY(p.x, p.y, 16, 16))
				{
					LSG = ID_GSB_MINIMIZE;
					g_LastSelectedGump = index;
				}
			}
			else //Минимизированный гамп (с полосками)
			{
				if (UO->GumpPixelsInXY(0x0803, 0, 0))
				{
					g_LastSelectedObject = 0;
					g_LastSelectedGump = index;

					if (Party.Leader != 0) //inParty
					{
						if (UO->GumpPixelsInXY(0x0938, 16, 20))
							LSG = ID_GSB_BUTTON_HEAL_1;
						else if (UO->GumpPixelsInXY(0x0938, 16, 33))
							LSG = ID_GSB_BUTTON_HEAL_2;
					}

					if (InGroup() && UO->GumpPixelsInXY(0x082C, 136, 24))
						LSG = ID_GSB_BUTTON_REMOVE_FROM_GROUP;
				}
			}
		}
		else //Чужой гамп
		{
			if (UO->GumpPixelsInXY(0x0804, 0, 0))
			{
				g_LastSelectedObject = 0;
				g_LastSelectedGump = index;
				g_StatusbarUnderMouse = index;

				if (Party.Contains(m_Serial))
				{
					if (UO->GumpPixelsInXY(0x0938, 16, 20))
						LSG = ID_GSB_BUTTON_HEAL_1;
					else if (UO->GumpPixelsInXY(0x0938, 16, 33))
						LSG = ID_GSB_BUTTON_HEAL_2;
				}

				TGameCharacter *obj = World->FindWorldCharacter(m_Serial);

				if (obj != NULL && obj->CanChangeName && TextEntry != NULL)
				{
					//Для изменения имени
					if (UO->PolygonePixelsInXY(16, 14, 109, 16))
						LSG = ID_GSB_TEXT_FIELD;
				}

				if (InGroup() && UO->GumpPixelsInXY(0x082C, 136, 24))
					LSG = ID_GSB_BUTTON_REMOVE_FROM_GROUP;
			}
		}

		if (g_ShowGumpLocker && UO->PolygonePixelsInXY(0, 0, 10, 14))
		{
			LSG = ID_GSB_LOCK_MOVING;
			g_LastSelectedGump = index;
		}

		g_MouseX = oldMouseX;
		g_MouseY = oldMouseY;

		if (LSG != 0)
			g_LastSelectedObject = LSG;

		return LSG;
	}

	return 0;
}
//----------------------------------------------------------------------------
void TGumpStatusbar::OnLeftMouseDown()
{
	if (g_GeneratedMouseDown)
		return;

	if (!g_LastObjectLeftMouseDown)
	{
		//Проверим, может быть есть таргет, который нужно повесить на данного чара
		if (Target.IsTargeting())
		{
			Target.SendTargetObject(m_Serial);
			g_CancelDoubleClick = true;
		}
	}
}
//----------------------------------------------------------------------------
void TGumpStatusbar::OnLeftMouseUp()
{
	if (g_GeneratedMouseDown)
		return;

	if (g_LastObjectLeftMouseDown == ID_GSB_MINIMIZE && m_Serial == g_PlayerSerial)
	{
		//Кнопка минимизации на полной версии гампа
		m_Minimized = true;
		m_FrameCreated = false;
	}
	else if (g_LastObjectLeftMouseDown == ID_GSB_LOCK_MOVING)
	{
		m_LockMoving = !m_LockMoving;
		g_CancelDoubleClick = true;
	}
	else if (g_LastObjectLeftMouseDown == ID_GSB_TEXT_FIELD && m_Serial != g_PlayerSerial)
	{
		int bonusY = 14;

		if (Party.Contains(m_Serial))
			bonusY = -2;

		int x = g_MouseX - (m_MinimizedX + 16);
		int y = g_MouseY - (m_MinimizedY + bonusY);

		TextEntry->OnClick(this, 1, false, x, y, TS_LEFT, UOFONT_FIXED);
	}
	else if (g_LastObjectLeftMouseDown == ID_GSB_BUTTON_HEAL_1)
	{
		UO->CastSpell(29);
		g_PartyHelperTimer = GetTickCount() + 500;
		g_PartyHelperTarget = m_Serial;
		g_CancelDoubleClick = true;
		FrameCreated = false;
	}
	else if (g_LastObjectLeftMouseDown == ID_GSB_BUTTON_HEAL_2)
	{
		UO->CastSpell(11);
		g_PartyHelperTimer = GetTickCount() + 500;
		g_PartyHelperTarget = m_Serial;
		g_CancelDoubleClick = true;
		FrameCreated = false;
	}
	else if (g_LastObjectLeftMouseDown == ID_GSB_BUTTON_REMOVE_FROM_GROUP)
	{
		TGumpStatusbar *oldGroup = m_GroupNext;

		if (oldGroup == NULL)
			oldGroup = m_GroupPrev;

		RemoveFromGroup();
		m_FrameCreated = false;

		if (oldGroup != NULL)
		{
			oldGroup->UpdateGroup(0, 0);
			oldGroup->FrameCreated = false;
		}
	}
	else if (!g_LastObjectLeftMouseDown)
	{
		//Проверим, может быть есть таргет, который нужно повесить на данного чара
		/*if (Target.IsTargeting())
			Target.SendTargetObject(Serial);*/
	}
}
//----------------------------------------------------------------------------
bool TGumpStatusbar::OnLeftMouseDoubleClick()
{
	if (g_GeneratedMouseDown)
		return false;

	if (!g_LastObjectLeftMouseDown && m_Serial == g_PlayerSerial && m_Minimized)
	{
		//Если это статусбар игрока (с полосками) то развернем его до полной версии
		m_Minimized = false;

		if (InGroup())
		{
			TGumpStatusbar *oldGroup = m_GroupNext;

			if (oldGroup == NULL)
				oldGroup = m_GroupPrev;

			RemoveFromGroup();

			if (oldGroup != NULL)
			{
				oldGroup->UpdateGroup(0, 0);
				oldGroup->FrameCreated = false;
			}
		}

		FrameCreated = false;

		return true;
	}
	else if (m_Serial != g_PlayerSerial)
	{
		if (g_Player->Warmode)
			UO->Attack(m_Serial); //Если в вармоде - атакуем
		else
			UO->DoubleClick(m_Serial); //Или используем предмет

		return true;
	}
	else if (!m_Minimized)
	{
		//По даблклику по полной версии статусбара теперь открывается папердолл
		UO->PaperdollReq(m_Serial);

		return true;
	}

	return false;
}
//----------------------------------------------------------------------------
void TGumpStatusbar::OnCharPress(WPARAM &wparam, LPARAM &lparam)
{
	if (wparam == VK_RETURN || wparam == VK_BACK || wparam == VK_ESCAPE || EntryPointer == NULL)
		return; //Ignore no print keys

	wchar_t wstr[2] = {0};
	wstr[0] = wparam;

	wstring str(wstr);
	if (!str.length())
		return;

	//Изменение имени существа
	if (Serial != g_PlayerSerial) //Только чужие статусбары
	{
		if ((EntryPointer->Length() <= 15) && FontManager->GetWidthA(1, TextEntry->c_str(), TextEntry->Pos()) <= 100 && ((wparam >= 'a' && wparam <= 'z') || (wparam >= 'A' && wparam <= 'Z')))
		{
			EntryPointer->Insert(wparam);
			FrameCreated = false;
		}
	}
}
//----------------------------------------------------------------------------
void TGumpStatusbar::OnKeyPress(WPARAM &wparam, LPARAM &lparam)
{
	switch (wparam)
	{
		case VK_RETURN:
		{
			//Если всё впорядке - изменяем имя
			if (EntryPointer->Length())
				SendRenameRequest();
			else //Нельзя изменить имя на пустое
			{
				TGameObject *obj = World->FindWorldObject(m_Serial);
				if (obj != NULL)
					EntryPointer->SetText(obj->GetName());
			}

			if (ConfigManager.GetConsoleNeedEnter())
				EntryPointer = NULL;
			else
				EntryPointer = GameConsole;

			FrameCreated = false; //Перерисуем

			break;
		}
		case VK_HOME:
		{
			EntryPointer->SetPos(0);
			FrameCreated = false;

			break;
		}
		case VK_END:
		{
			EntryPointer->SetPos(EntryPointer->Length());
			FrameCreated = false;

			break;
		}
		case VK_LEFT:
		{
			EntryPointer->AddPos(-1);
			FrameCreated = false;

			break;
		}
		case VK_RIGHT:
		{
			EntryPointer->AddPos(1);
			FrameCreated = false;

			break;
		}
		case VK_BACK:
		{
			EntryPointer->Remove(true);
			FrameCreated = false;

			break;
		}
		case VK_DELETE:
		{
			EntryPointer->Remove(false);
			FrameCreated = false;

			break;
		}
		case VK_ESCAPE:
		{
			//По тыку на Esc можно выйти из редактирования имени существа

			TGameObject *obj = World->FindWorldObject(m_Serial);
			if (obj != NULL)
				EntryPointer->SetText(obj->GetName());

			if (ConfigManager.GetConsoleNeedEnter())
				EntryPointer = NULL;
			else
				EntryPointer = GameConsole;

			FrameCreated = false; //Перерисуем

			break;
		}
		default:
			break;
	}
}
//----------------------------------------------------------------------------