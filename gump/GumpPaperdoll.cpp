/****************************************************************************
**
** Gump.cpp
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
int TGumpPaperdoll::UsedLayers[m_LayerCount] =
{
	OL_CLOAK,
	OL_RING,
	OL_SHIRT,
	OL_ARMS,
	OL_PANTS,
	OL_SHOES,
	OL_LEGS,
	OL_TORSO,
	OL_BRACELET,
	OL_15,
	OL_GLOVES,
	OL_TUNIC,
	OL_SKIRT,
	OL_NECKLACE,
	OL_HAIR,
	OL_ROBE,
	OL_EARRINGS,
	OL_BEARD,
	OL_HELMET,
	OL_WAIST,
	OL_1_HAND,
	OL_2_HAND
};
//---------------------------------------------------------------------------
TGumpPaperdoll::TGumpPaperdoll(DWORD serial, short x, short y, bool minimized)
: TGump(GT_PAPERDOLL, serial, x, y)
{
	if (minimized)
	{
		m_Minimized = true;
		m_MinimizedX = x;
		m_MinimizedY = y;
	}

	m_TextContainer = new TTextContainer(10);
	TextRenderer = new TTextRenderer();
}
//---------------------------------------------------------------------------
TGumpPaperdoll::~TGumpPaperdoll()
{
	if (m_TextContainer != NULL)
	{
		delete m_TextContainer;
		m_TextContainer = NULL;
	}

	if (TextRenderer != NULL)
	{
		delete TextRenderer;
		TextRenderer = NULL;
	}
}
//---------------------------------------------------------------------------
void TGumpPaperdoll::PrepareTextures()
{
	TGameCharacter *obj = World->FindWorldCharacter(m_Serial);

	WORD bodyColor = 0;

	if (obj != NULL)
		bodyColor = obj->Color;

	UO->ExecuteGump(0x07EE); //Paperdoll button Character
	UO->ExecuteGump(0x07D0); //Paperdoll (self)
	UO->ExecuteButton(0x07EF); //Paperdoll button Help
	UO->ExecuteButton(0x07D6); //Paperdoll button Options
	UO->ExecuteButton(0x07D9); //Paperdoll button Log Out
	UO->ExecuteButton(0x07DC); //Paperdoll button Journal
	UO->ExecuteButton(0x57B5); //Paperdoll button Quests
	UO->ExecuteButton(0x07DF); //Paperdoll button Skills
	UO->ExecuteButton(0x57B2); //Paperdoll button Guild
	UO->ExecuteButton(0x07E2); //Paperdoll button Chat
	UO->ExecuteGump(0x07E2, false); //Paperdoll button Chat
	UO->ExecuteButton(0x07E8); //Paperdoll button War
	UO->ExecuteButton(0x07E5); //Paperdoll button Peace
	//UO->ExecuteGump(0x0FA1); //Paperdoll mail bag
	UO->ExecuteGump(0x0071); //David's star
	UO->ExecuteGump(0x07D2); //Paperdoll party/profile scroll
	UO->ExecuteGump(0x07D1); //Paperdoll (other)
	UO->ExecuteButton(0x07EB); //Paperdoll button Status
	UO->ExecuteGump(0x000C); //Male gump
	UO->ExecuteGump(0x000D); //Female gump
	UO->ExecuteGump(0xC72B); //GM robe gump
	
	if (obj != NULL)
	{
		int gumpOffset = (obj->Sex ? FEMALE_GUMP_OFFSET : MALE_GUMP_OFFSET);

		//Draw equipment & backpack
		TGameItem *equipment = NULL;
		if (obj->IsHuman() && !obj->Dead())
		{
			IFOR(i, 0, m_LayerCount)
			{
				equipment = obj->FindLayer(UsedLayers[i]);

				if (equipment != NULL && equipment->AnimID)
				{
					int cOfs = gumpOffset;

					if (obj->Sex && !UO->ExecuteGump(equipment->AnimID + cOfs))
						cOfs = MALE_GUMP_OFFSET;

					UO->ExecuteGump(equipment->AnimID + cOfs);
				}
				else if (ObjectInHand != NULL && UsedLayers[i] == ObjectInHand->UsedLayer && ObjectInHand->AnimID)
				{
					equipment = obj->FindLayer(ObjectInHand->UsedLayer);

					if (equipment == NULL)
					{
						int cOfs = gumpOffset;

						if (obj->Sex && !UO->ExecuteGump(ObjectInHand->AnimID + cOfs))
							cOfs = MALE_GUMP_OFFSET;

						UO->ExecuteGump(ObjectInHand->AnimID + gumpOffset);
					}
				}
			}
		}
		else if (obj->IsHuman())
		{
			equipment = obj->FindLayer(OL_ROBE);

			if (equipment != NULL && equipment->AnimID)
			{
				int cOfs = gumpOffset;

				if (obj->Sex && !UO->ExecuteGump(equipment->AnimID + cOfs))
					cOfs = MALE_GUMP_OFFSET;

				UO->ExecuteGump(equipment->AnimID + cOfs);
			}
		}

		equipment = obj->FindLayer(OL_BACKPACK);
		if (equipment != NULL && equipment->AnimID != 0)
			UO->ExecuteGump(equipment->AnimID + 50000);
	}
}
//---------------------------------------------------------------------------
void TGumpPaperdoll::CalculateGumpState()
{
	TGump::CalculateGumpState();

	if (CanBeMoved() && g_LeftMouseDown && g_LastGumpLeftMouseDown == (DWORD)this && (!g_LastObjectLeftMouseDown || g_LastObjectLeftMouseDown == ID_GP_ITEMS + OL_BACKPACK) && ObjectInHand == NULL)
	{
		g_GumpMovingOffsetX = g_MouseX - g_DroppedLeftMouseX;
		g_GumpMovingOffsetY = g_MouseY - g_DroppedLeftMouseY;
	}
	else
	{
		g_GumpMovingOffsetX = 0;
		g_GumpMovingOffsetY = 0;
	}

	if (m_Minimized)
	{
		g_GumpTranslateX = (float)(m_MinimizedX + g_GumpMovingOffsetX);
		g_GumpTranslateY = (float)(m_MinimizedY + g_GumpMovingOffsetY);
	}
	else
	{
		g_GumpTranslateX = (float)(m_X + g_GumpMovingOffsetX);
		g_GumpTranslateY = (float)(m_Y + g_GumpMovingOffsetY);
	}
}
//---------------------------------------------------------------------------
void TGumpPaperdoll::GenerateFrame()
{
	if (!g_DrawMode)
	{
		m_FrameRedraw = false;
		m_FrameCreated = false;

		return;
	}

	PrepareTextures();

	TGameCharacter *obj = World->FindWorldCharacter(m_Serial);
	if (obj == NULL)
		return;

	CalculateGumpState();

	if (g_LastObjectType == SOT_TEXT_OBJECT)
		g_GumpPressedElement = false;

	glNewList((GLuint)this, GL_COMPILE);

		if (m_Minimized)
		{
			UO->DrawGump(0x07EE, 0, 0, 0); //Paperdoll button Character
			glEndList();

			m_FrameRedraw = true;
			m_FrameCreated = true;

			return;
		}

		ColorizerShader->Use();

		WORD gumpID = 0;

		if (m_Serial == g_PlayerSerial)
		{
			UO->DrawGump(0x07D0, 0, 0, 0); //Paperdoll (self)

			gumpID = 0x07EF;
			if (g_GumpPressedElement == ID_GP_BUTTON_HELP)
				gumpID = 0x07F0; //Paperdoll button Help (down)
			else if (g_GumpSelectElement == ID_GP_BUTTON_HELP)
				gumpID = 0x07F1; //Paperdoll button Help
			UO->DrawGump(gumpID, 0, 185, 44);
				
			gumpID = 0x07D6;
			if (g_GumpPressedElement == ID_GP_BUTTON_OPTIONS)
				gumpID = 0x07D7; //Paperdoll button Options (down)
			else if (g_GumpSelectElement == ID_GP_BUTTON_OPTIONS)
				gumpID = 0x07D8; //Paperdoll button Options
			UO->DrawGump(gumpID, 0, 185, 71);
				
			gumpID = 0x07D9;
			if (g_GumpPressedElement == ID_GP_BUTTON_LOGOUT)
				gumpID = 0x07DA; //Paperdoll button Log Out (down)
			else if (g_GumpSelectElement == ID_GP_BUTTON_LOGOUT)
				gumpID = 0x07DB; //Paperdoll button Log Out
			UO->DrawGump(gumpID, 0, 185, 98);
			
			if (ConnectionManager.ClientVersion >= CV_500A)
			{
				gumpID = 0x57B5;
				if (g_GumpPressedElement == ID_GP_BUTTON_JOURNAL_OR_QUESTS)
					gumpID = 0x57B7; //Paperdoll button Quests (down)
				else if (g_GumpSelectElement == ID_GP_BUTTON_JOURNAL_OR_QUESTS)
					gumpID = 0x57B6; //Paperdoll button Quests
			}
			else
			{
				gumpID = 0x07DC;
				if (g_GumpPressedElement == ID_GP_BUTTON_JOURNAL_OR_QUESTS)
					gumpID = 0x07DD; //Paperdoll button Journal (down)
				else if (g_GumpSelectElement == ID_GP_BUTTON_JOURNAL_OR_QUESTS)
					gumpID = 0x07DE; //Paperdoll button Journal
			}

			UO->DrawGump(gumpID, 0, 185, 125);
				
			gumpID = 0x07DF;
			if (g_GumpPressedElement == ID_GP_BUTTON_SKILLS)
				gumpID = 0x07E0; //Paperdoll button Skills (down)
			else if (g_GumpSelectElement == ID_GP_BUTTON_SKILLS)
				gumpID = 0x07E1; //Paperdoll button Skills
			UO->DrawGump(gumpID, 0, 185, 152);

			if (ConnectionManager.ClientVersion >= CV_500A)
			{
				gumpID = 0x57B2;
				if (g_GumpPressedElement == ID_GP_BUTTON_CHAT_OR_GUILD)
					gumpID = 0x57B4; //Paperdoll button Guild (down)
				else if (g_GumpSelectElement == ID_GP_BUTTON_CHAT_OR_GUILD)
					gumpID = 0x57B3; //Paperdoll button Guild
			}
			else
			{
				gumpID = 0x07E2;

				if (!g_ChatEnabled)
					DeathShader->Use();
				else
				{
					if (g_GumpPressedElement == ID_GP_BUTTON_CHAT_OR_GUILD)
						gumpID++; // = 0x07E3; //Paperdoll button Chat (down)
					else if (g_GumpSelectElement == ID_GP_BUTTON_CHAT_OR_GUILD)
						gumpID++; // = 0x07E4; //Paperdoll button Chat
				}
			}

			UO->DrawGump(gumpID, 0, 185, 179);
			
			if (!g_ChatEnabled)
				ColorizerShader->Use();

			if (g_Player->Warmode)
			{
				gumpID = 0x07E8;

				if (g_GumpPressedElement == ID_GP_BUTTON_WARMODE)
					gumpID = 0x07E9; //Paperdoll button War (down)
				else if (g_GumpSelectElement == ID_GP_BUTTON_WARMODE)
					gumpID = 0x07EA; //Paperdoll button War
			}
			else
			{
				gumpID = 0x07E5;

				if (g_GumpPressedElement == ID_GP_BUTTON_WARMODE)
					gumpID = 0x07E6; //Paperdoll button Peace (down)
				else if (g_GumpSelectElement == ID_GP_BUTTON_WARMODE)
					gumpID = 0x07E7; //Paperdoll button Peace
			}

			UO->DrawGump(gumpID, 0, 185, 206);
			
			//UO->DrawGump(0x0FA1, 0, posX + 80, posY + 4); //Paperdoll mail bag
			UO->DrawGump(0x0071, 0, 80, 4); //David's star
			
			UO->DrawGump(0x07D2, 0, 37, 196); //Paperdoll party scroll
		}
		else
			UO->DrawGump(0x07D1, 0, 0, 0); //Paperdoll (other)

		gumpID = 0x07EB;
		if (g_GumpPressedElement == ID_GP_BUTTON_STATUS)
			gumpID = 0x07EC; //Paperdoll button Status (down)
		else if (g_GumpSelectElement == ID_GP_BUTTON_STATUS)
			gumpID = 0x07ED; //Paperdoll button Status
		UO->DrawGump(gumpID, 0, 185, 233);
		
		if (obj->Graphic == 0x0190 || obj->Graphic == 0x0192)
			UO->DrawGump(0x000C, obj->Color, 8, 19, true); //Male gump
		else if (obj->Graphic == 0x0191 || obj->Graphic == 0x0193)
			UO->DrawGump(0x000D, obj->Color, 8, 19, true); //Female gump
		else if (obj->Graphic == 0x03DB)
		{
			UO->DrawGump(0x000C, 0x03EA, 8, 19, true); //Male gump
			UO->DrawGump(0xC72B, obj->Color, 8, 19); //GM robe gump
		}

		UO->DrawGump(0x07D2, 0, 23, 196); //Paperdoll profile scroll

		glUniform1iARB(ShaderDrawMode, 0);
		obj->m_PaperdollTextTexture.Draw(39, 262);

		int gumpOffset = (obj->Sex ? FEMALE_GUMP_OFFSET : MALE_GUMP_OFFSET);

		//Draw equipment & backpack
		TGameItem *equipment = NULL;
		if (obj->IsHuman() && !obj->Dead())
		{
			IFOR(i, 0, m_LayerCount)
			{
				equipment = obj->FindLayer(UsedLayers[i]);

				if (equipment != NULL && equipment->AnimID)
				{
					int cOfs = gumpOffset;

					if (obj->Sex && !UO->ExecuteGump(equipment->AnimID + cOfs))
						cOfs = MALE_GUMP_OFFSET;

					UO->DrawGump(equipment->AnimID + cOfs, equipment->Color, 8, 19);
				}
				else if (g_LastSelectedGump == (DWORD)this && ObjectInHand != NULL && UsedLayers[i] == ObjectInHand->UsedLayer && ObjectInHand->AnimID)
				{
					equipment = obj->FindLayer(ObjectInHand->UsedLayer);

					if (equipment == NULL)
					{
						int cOfs = gumpOffset;

						if (obj->Sex && !UO->ExecuteGump(ObjectInHand->AnimID + cOfs))
							cOfs = MALE_GUMP_OFFSET;

						glEnable(GL_BLEND);
						glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
						glColor4f(1.0f, 1.0f, 1.0f, 0.7f);

						UO->DrawGump(ObjectInHand->AnimID + cOfs, ObjectInHand->Color, 8, 19);
						
						glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
						glDisable(GL_BLEND);
					}
				}
			}
		}
		else if (obj->IsHuman())
		{
			equipment = obj->FindLayer(OL_ROBE);

			if (equipment != NULL && equipment->AnimID)
			{
				int cOfs = gumpOffset;

				if (obj->Sex && !UO->ExecuteGump(equipment->AnimID + cOfs))
					cOfs = MALE_GUMP_OFFSET;

				UO->DrawGump(equipment->AnimID + cOfs, equipment->Color, 8, 19);
			}
		}

		equipment = obj->FindLayer(OL_BACKPACK);
		if (equipment != NULL && equipment->AnimID != 0)
			UO->DrawGump(equipment->AnimID + 50000, equipment->Color, 8, 19);

		UnuseShader();

	glEndList();

	m_FrameRedraw = true;
	m_FrameCreated = true;
}
//---------------------------------------------------------------------------
int TGumpPaperdoll::Draw(bool &mode)
{
	TGameCharacter *obj = World->FindWorldCharacter(m_Serial);

	if (obj == NULL)
		return 0;
	
	DWORD index = (DWORD)this;

	CalculateGumpState();

	if (g_LastObjectType == SOT_TEXT_OBJECT)
		g_GumpPressedElement = false;

	if (mode && !g_Player->Dead() && Serial == g_PlayerSerial && g_GumpPressed && ObjectInHand == NULL && g_LastObjectLeftMouseDown != 0xFFFFFFFF && ((g_MouseX != g_DroppedLeftMouseX || g_MouseY != g_DroppedLeftMouseY) || (g_LastGumpMouseDownTime + DCLICK_DELAY < GetTickCount())))
	{
		int layer = g_LastObjectLeftMouseDown - ID_GP_ITEMS;

		if (layer != OL_HAIR && layer != OL_BEARD && layer != OL_BACKPACK)
		{
			TGameItem *equipment = obj->FindLayer(layer);

			if (equipment != NULL)
			{
				if (Target.IsTargeting())
					Target.SendCancelTarget();

				UO->PickupItem(equipment);
				//g_LastGumpLeftMouseDown = 0;
				g_LastObjectLeftMouseDown = 0;
				g_DroppedLeftMouseX = g_MouseX;
				g_DroppedLeftMouseY = g_MouseY;
			}
		}
	}

	if (g_LastSelectedGump == (DWORD)this && ObjectInHand != NULL && ObjectInHand->AnimID)
	{
		TGameItem *equipment = obj->FindLayer(ObjectInHand->UsedLayer);

		if (equipment == NULL)
			m_FrameCreated = false;
	}

	//Вычисление положения, прозрачности и отрисовка текста
	TRenderTextObject *rto = TextRenderer->m_Items;
	
	DWORD ticks = GetTickCount();

	if (!m_Minimized)
	{
		if (mode)
			TextRenderer->ClearRect();

		//Вычисление положения и прозрачности текста
		for (; rto != NULL; rto = rto->m_NextDraw)
		{
			if (!rto->IsText() || !mode)
			{
				if (rto->m_NextDraw == NULL)
					break;

				continue;
			}

			TTextData *td = (TTextData*)rto;
			
			if (td->Timer >= ticks)
			{
				if (td->Type == TT_OBJECT)
				{
					TGameObject *go = World->FindWorldObject(td->Serial);

					if (go != NULL && go->Graphic < 0x4000)
					{
						TTextTexture &tth = td->m_Texture;
						int drawX = td->DrawX - go->GetTextOffsetX(td);
						int drawY = td->DrawY - go->GetTextOffsetY(td);

						TTextImageBounds ib(drawX, drawY, tth.Width, tth.Height, td);

						td->Transparent = TextRenderer->InRect(ib, go);

						TextRenderer->AddRect(ib);
					}
				}
				else
				{
					TTextTexture &tth = td->m_Texture;
					int drawX = td->DrawX - (tth.Width / 2);
					int drawY = td->DrawY;

					TTextData *tdBuf = td;

					while (tdBuf != NULL)
					{
						drawY -= tdBuf->m_Texture.Height;

						tdBuf = tdBuf->m_Next;
					}

					TTextImageBounds ib(drawX, drawY + 1, tth.Width, tth.Height, td);

					td->Transparent = TextRenderer->InRect(ib, NULL);

					TextRenderer->AddRect(ib);
				}
			}

			if (rto->m_NextDraw == NULL)
				break;
		}
	}

	if (mode)
	{
		if (!m_FrameCreated || g_GumpSelectElement) // || g_GumpPressed
			GenerateFrame();
		else if (m_FrameRedraw)
		{
			GenerateFrame();
			m_FrameRedraw = false;
		}
		else
		{
			IFOR(i, 0, m_LayerCount)
			{
				if (g_LastSelectedGump == index && ObjectInHand != NULL && UsedLayers[i] == ObjectInHand->UsedLayer && ObjectInHand->AnimID)
				{
					TGameItem *equipment = obj->FindLayer(ObjectInHand->UsedLayer);
					if (equipment == NULL)
					{
						GenerateFrame();
						break;
					}
				}
			}
		}

		glTranslatef(g_GumpTranslateX, g_GumpTranslateY, 0.0f);

		glCallList((GLuint)index);
		
		if (!m_Minimized)
		{
			//Отрисовка текста
			for (; rto != NULL; rto = rto->m_PrevDraw)
			{
				if (!rto->IsText())
					continue;

				TTextData *td = (TTextData*)rto;
				
				if (td->Type != TT_SYSTEM && td->Timer >= ticks)
				{
					TTextTexture &tth = td->m_Texture;

					int drawX = td->DrawX - (tth.Width / 2);
					int drawY = td->DrawY;
					bool transparent = td->Transparent;
					
					while (td != NULL)
					{
						drawY -= td->m_Texture.Height;

						td = td->m_Next;
					}

					if (transparent)
					{
						glEnable(GL_BLEND);
						glBlendFunc(GL_ONE, GL_DST_COLOR);

						tth.Draw(drawX, drawY);

						glDisable(GL_BLEND);
					}
					else
						tth.Draw(drawX, drawY);
				}
			}
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

		int LSG = 0;

		if (m_Serial == g_PlayerSerial)
		{
			if (Minimized)
			{
				if (UO->GumpPixelsInXY(0x07EE, 0, 0))
				{
					g_LastSelectedObject = 0;
					g_LastSelectedGump = index; //Paperdoll button Character
				}

				g_MouseX = oldMouseX;
				g_MouseY = oldMouseY;

				return 0;
			}
			else if (UO->GumpPixelsInXY(0x07D0, 0, 0))
			{
				g_LastSelectedObject = 0;
				g_LastSelectedGump = index; //Paperdoll (self)
			}
			
			WORD gumpID = (g_Player->Warmode ? 0x07E8 : 0x07E5); //Paperdoll button War or Peace
			if (UO->GumpPixelsInXY(0x07EF, 185, 44))
				LSG = ID_GP_BUTTON_HELP; //Paperdoll button Help
			else if (UO->GumpPixelsInXY(0x07D6, 185, 71))
				LSG = ID_GP_BUTTON_OPTIONS; //Paperdoll button Options
			else if (UO->GumpPixelsInXY(0x07D9, 185, 98))
				LSG = ID_GP_BUTTON_LOGOUT; //Paperdoll button Log Out
			else if (UO->GumpPixelsInXY(0x07DF, 185, 152))
				LSG = ID_GP_BUTTON_SKILLS; //Paperdoll button Skills
			else if (UO->GumpPixelsInXY(gumpID, 185, 206))
				LSG = ID_GP_BUTTON_WARMODE; //Paperdoll button War/Peace
			else if (UO->GumpPixelsInXY(0x0071, 80, 4))
				LSG = ID_GP_BUTTON_VIRTURE; //David's star (virture button)
			else
			{
				if (ConnectionManager.ClientVersion >= CV_500A)
				{
					if (UO->GumpPixelsInXY(0x57B5, 185, 125))
						LSG = ID_GP_BUTTON_JOURNAL_OR_QUESTS; //Paperdoll button Quests
					else if (UO->GumpPixelsInXY(0x57B2, 185, 179))
						LSG = ID_GP_BUTTON_CHAT_OR_GUILD; //Paperdoll button Guild
				}
				else
				{
					if (UO->GumpPixelsInXY(0x07DC, 185, 125))
						LSG = ID_GP_BUTTON_JOURNAL_OR_QUESTS; //Paperdoll button Journal
					else if (UO->GumpPixelsInXY(0x07E2, 185, 179))
						LSG = ID_GP_BUTTON_CHAT_OR_GUILD; //Paperdoll button Chat
				}

				if (UO->PolygonePixelsInXY(226, 258, 16, 16))
					LSG = ID_GP_BUTTON_MINIMIZE;
				else if (UO->GumpPixelsInXY(0x07D2, 37, 196))
					LSG = ID_GP_PARTY_MANIFEST_SCROLL; //Paperdoll party manifest scroll
			}
		}
		else if (UO->GumpPixelsInXY(0x07D1, 0, 0))
		{
			g_LastSelectedObject = 0;
			g_LastSelectedGump = index; //Paperdoll (other)
		}
		
		if (UO->GumpPixelsInXY(0x07EB, 185, 233))
			LSG = ID_GP_BUTTON_STATUS; //Paperdoll button Status
		else if (UO->GumpPixelsInXY(0x07D2, 23, 196))
			LSG = ID_GP_PROFILE_SCROLL; //Paperdoll profile scroll

		int gumpOffset = (obj->Sex ? FEMALE_GUMP_OFFSET : MALE_GUMP_OFFSET);
		
		//Draw equipment & backpack
		TGameItem *equipment = NULL;
		if (obj->IsHuman())
		{
			IFOR(i, 0, m_LayerCount)
			{
				equipment = obj->FindLayer(UsedLayers[i]);

				if (equipment != NULL && equipment->AnimID != 0)
				{
					int cOfs = gumpOffset;

					if (obj->Sex && !UO->ExecuteGump(equipment->AnimID + cOfs))
						cOfs = MALE_GUMP_OFFSET;

					if (UO->GumpPixelsInXY(equipment->AnimID + cOfs, 8, 19))
						LSG = ID_GP_ITEMS + UsedLayers[i];
				}
			}
		}

		equipment = obj->FindLayer(OL_BACKPACK);
		if (equipment != NULL && equipment->AnimID != 0)
		{
			if (UO->GumpPixelsInXY(equipment->AnimID + 50000, 8, 19))
				LSG = ID_GP_ITEMS + OL_BACKPACK;
		}
		
		//Проверка текста
		for (; rto != NULL; rto = rto->m_PrevDraw)
		{
			if (!rto->IsText())
				continue;

			TTextData *td = (TTextData*)rto;
				
			if (td->Type != TT_SYSTEM && td->Timer >= ticks)
			{
				TTextTexture &tth = td->m_Texture;

				int drawX = td->DrawX - (tth.Width / 2);
				int drawY = td->DrawY;

				while (td != NULL)
				{
					drawY -= td->m_Texture.Height;

					td = td->m_Next;
				}

				if (tth.UnderMouse(drawX, drawY))
				{
					g_LastObjectType = SOT_TEXT_OBJECT;
					LSG = 1;
					g_SelectedTextObject = rto;
				}
			}
		}
		
		if (g_ShowGumpLocker && UO->PolygonePixelsInXY(0, 0, 10, 14))
		{
			LSG = ID_GP_LOCK_MOVING;
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
void TGumpPaperdoll::OnLeftMouseUp()
{
	TGameCharacter *container = World->FindWorldCharacter(m_Serial);
	if (container == NULL && g_LastSelectedObject >= ID_GP_ITEMS)
		return;

	//Что-то в руке
	if ((!g_LastSelectedObject || g_LastSelectedObject >= ID_GP_ITEMS) && ObjectInHand != NULL)
	{
		bool canWear = true;

		if (m_Serial != g_PlayerSerial && GetDistance(g_Player, container) > 3)
			canWear = false;

		if (canWear && container != NULL)
		{
			int layer = g_LastSelectedObject - ID_GP_ITEMS;

			if (layer == OL_BACKPACK) //Ткнули на пак
			{
				TGameItem *equipment = container->FindLayer(layer);
				if (equipment != NULL)
				{
					if (m_Serial != g_PlayerSerial)
						UO->DropItem(container->Serial, 0xFFFF, 0xFFFF, 0);
					else
						UO->DropItem(equipment->Serial, 0xFFFF, 0xFFFF, 0);

					g_DroppedLeftMouseX = g_MouseX;
					g_DroppedLeftMouseY = g_MouseY;

					return;
				}
			}
			else if (ObjectInHand->IsWearable()) //Можно одевать
			{
				TGameItem *equipment = container->FindLayer(ObjectInHand->UsedLayer);
				if (equipment == NULL) //На этом слое ничего нет
				{
					if (m_Serial != g_PlayerSerial)
						UO->EquipItem(container->Serial);
					else
						UO->EquipItem();

					g_DroppedLeftMouseX = g_MouseX;
					g_DroppedLeftMouseY = g_MouseY;

					return;
				}
			}
		}
		else
			UO->PlaySoundEffect(0x0051);
	}
	
	if (g_LastObjectLeftMouseDown != g_LastSelectedObject)
		return;

	if (g_LastObjectLeftMouseDown >= ID_GP_ITEMS)
	{
		int layer = g_LastObjectLeftMouseDown - ID_GP_ITEMS;
		TGameItem *equipment = container->FindLayer(layer);

		if (equipment != NULL)
		{
			if (Target.IsTargeting())
				Target.SendTargetObject(equipment->Serial);
			else if (ObjectInHand == NULL) //Click on object
			{
				if (!g_ClickObjectReq)
				{
					g_ClickObjectReq = true;
					g_ClickObject.Init(COT_GAME_OBJECT);
					g_ClickObject.Serial = equipment->Serial;
					g_ClickObject.Timer = GetTickCount() + DCLICK_DELAY;
					g_ClickObject.X = g_MouseX - X;
					g_ClickObject.Y = g_MouseY - Y;
				}
			}
		}

		return;
	}

	switch (g_LastObjectLeftMouseDown)
	{
		case ID_GP_BUTTON_HELP: //Paperdoll button Help
		{
			UO->HelpRequest();
			break;
		}
		case ID_GP_BUTTON_OPTIONS: //Paperdoll button Options
		{
			UO->OpenConfiguration();
			break;
		}
		case ID_GP_BUTTON_LOGOUT: //Paperdoll button Log Out
		{
			UO->OpenLogOut();
			break;
		}
		case ID_GP_BUTTON_JOURNAL_OR_QUESTS: //Paperdoll button Journal
		{
			if (ConnectionManager.ClientVersion >= CV_500A)
				UO->RequestQuestGump();
			else
				UO->OpenJournal();
			break;
		}
		case ID_GP_BUTTON_SKILLS: //Paperdoll button Skills
		{
			UO->OpenSkills();
			break;
		}
		case ID_GP_BUTTON_CHAT_OR_GUILD: //Paperdoll button Chat
		{
			if (ConnectionManager.ClientVersion >= CV_500A)
				UO->RequestGuildGump();
			else
				UO->OpenChat();
			break;
		}
		case ID_GP_BUTTON_WARMODE: //Paperdoll button Peace/War
		{
			UO->ChangeWarmode();
			break;
		}
		case ID_GP_BUTTON_STATUS: //Paperdoll button Status
		{
			UO->OpenStatus(m_Serial);
			g_LastLClickTime = 0;
			break;
		}
		case ID_GP_BUTTON_MINIMIZE: //Paperdoll button Minimize
		{
			m_Minimized = true;
			m_FrameCreated = false;
			break;
		}
		case ID_GP_LOCK_MOVING:
		{
			m_LockMoving = !m_LockMoving;
			g_CancelDoubleClick = true;
			break;
		}
		case ID_GP_PROFILE_SCROLL:
		case ID_GP_PARTY_MANIFEST_SCROLL:
		{
			if (!g_ClickObjectReq)
			{
				g_ClickObjectReq = true;

				g_ClickObject.Init(COT_GUMP);
				g_ClickObject.GumpType = m_GumpType;
				g_ClickObject.Serial = m_Serial;
				g_ClickObject.GumpID = 0;
				g_ClickObject.GumpButtonID = (int)(g_LastObjectLeftMouseDown == ID_GP_PROFILE_SCROLL);
				g_ClickObject.Timer = GetTickCount() + DCLICK_DELAY;
			}

			break;
		}
		default:
			break;
	}
}
//----------------------------------------------------------------------------
bool TGumpPaperdoll::OnLeftMouseDoubleClick()
{
	if (m_Minimized)
	{
		m_Minimized = false;
		m_FrameCreated = false;

		return true;
	}

	bool result = false;

	if (g_LastObjectLeftMouseDown < ID_GP_ITEMS)
	{
		if (g_LastObjectLeftMouseDown == ID_GP_PROFILE_SCROLL)
		{
			UO->OpenProfile(m_Serial);

			result = true;
		}
		else if (g_LastObjectLeftMouseDown == ID_GP_PARTY_MANIFEST_SCROLL)
		{
			UO->OpenPartyManifest();

			result = true;
		}
		else if (g_LastObjectLeftMouseDown == ID_GP_BUTTON_VIRTURE)
		{
			TPacketVirtureRequest packet(1);
			packet.Send();

			result = true;
		}
	}
	else
	{
		TGameCharacter *container = World->FindWorldCharacter(m_Serial);

		if (container != NULL)
		{
			int layer = g_LastObjectLeftMouseDown - ID_GP_ITEMS;

			TGameItem *equipment = container->FindLayer(layer);

			if (equipment != NULL)
			{
				UO->DoubleClick(equipment->Serial);
				result = true;
			}
		}
	}

	return result;
}
//---------------------------------------------------------------------------