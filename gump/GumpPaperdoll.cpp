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
		Minimized = true;
		MinimizedX = x;
		MinimizedY = y;
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
	TGameCharacter *obj = World->FindWorldCharacter(Serial);

	WORD bodyColor = 0;

	if (obj != NULL)
		bodyColor = obj->Color;

	UO->ExecuteGump(0x07EE); //Paperdoll button Character
	UO->ExecuteGump(0x07D0); //Paperdoll (self)
	UO->ExecuteButton(0x07EF); //Paperdoll button Help
	UO->ExecuteButton(0x07D6); //Paperdoll button Options
	UO->ExecuteButton(0x07D9); //Paperdoll button Log Out
	UO->ExecuteButton(0x07DC); //Paperdoll button Journal
	UO->ExecuteButton(0x07DF); //Paperdoll button Skills
	UO->ExecuteButton(0x07E2); //Paperdoll button Chat
	UO->ExecuteColoredGump(0x07E2, 0x0386, false); //Paperdoll button Chat
	UO->ExecuteButton(0x07E8); //Paperdoll button War
	UO->ExecuteButton(0x07E5); //Paperdoll button Peace
	//UO->ExecuteGump(0x0FA1); //Paperdoll mail bag
	UO->ExecuteGump(0x0071); //David's star
	UO->ExecuteGump(0x07D2); //Paperdoll party/profile scroll
	UO->ExecuteGump(0x07D1); //Paperdoll (other)
	UO->ExecuteButton(0x07EB); //Paperdoll button Status
	UO->ExecuteColoredGump(0x000C, bodyColor); //Male gump
	UO->ExecuteColoredGump(0x000C, 0x03EA); //Male gump (for GM robe)
	UO->ExecuteColoredGump(0x000D, bodyColor); //Female gump
	UO->ExecuteColoredGump(0xC72B, bodyColor); //GM robe gump
	
	if (obj != NULL)
	{
		int gumpOffset = (obj->Sex ? g_FemaleGumpOffset : g_MaleGumpOffset);

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
						cOfs = g_MaleGumpOffset;

					UO->ExecuteColoredGump(equipment->AnimID + cOfs, equipment->Color);
				}
				else if (ObjectInHand != NULL && UsedLayers[i] == ObjectInHand->UsedLayer && ObjectInHand->AnimID)
				{
					equipment = obj->FindLayer(ObjectInHand->UsedLayer);

					if (equipment == NULL)
					{
						int cOfs = gumpOffset;

						if (obj->Sex && !UO->ExecuteGump(ObjectInHand->AnimID + cOfs))
							cOfs = g_MaleGumpOffset;

						UO->ExecuteColoredGump(ObjectInHand->AnimID + gumpOffset, ObjectInHand->Color);
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
					cOfs = g_MaleGumpOffset;

				UO->ExecuteColoredGump(equipment->AnimID + cOfs, equipment->Color);
			}
		}

		equipment = obj->FindLayer(OL_BACKPACK);
		if (equipment != NULL && equipment->AnimID != 0)
			UO->ExecuteColoredGump(equipment->AnimID + 50000, equipment->Color);
	}
}
//---------------------------------------------------------------------------
void TGumpPaperdoll::GenerateFrame(int posX, int posY)
{
	if (!g_DrawMode)
	{
		FrameRedraw = false;
		FrameCreated = false;

		return;
	}

	PrepareTextures();

	TGameCharacter *obj = World->FindWorldCharacter(Serial);
	if (obj == NULL)
		return;
	
	DWORD index = (DWORD)this;

	//Нажата ли кнопка в окне?
	bool IsPressed = (g_LeftMouseDown && g_LastGumpLeftMouseDown == index && g_LastSelectedGump == index);

	//Может ли быть подсвечен элемент?
	int CanSelectedButton = ((g_LastSelectedGump == index) ? g_LastSelectedObject : 0);

	//Может ли быть нажат элемент?
	int CanPressedButton = 0;
	if (IsPressed && g_LastObjectLeftMouseDown == g_LastSelectedObject)
		CanPressedButton = g_LastObjectLeftMouseDown;
	
	if (g_LastObjectType == SOT_TEXT_OBJECT)
	{
		CanSelectedButton = false;
		CanPressedButton = false;
	}

	glNewList((GLuint)index, GL_COMPILE);
	
		WORD gumpID = 0;

		if (Serial == g_PlayerSerial)
		{
			if (Minimized)
			{
				UO->DrawGump(0x07EE, 0, posX, posY); //Paperdoll button Character
				glEndList();

				FrameRedraw = true;
				FrameCreated = true;
				return;
			}

			UO->DrawGump(0x07D0, 0, posX, posY); //Paperdoll (self)

			gumpID = 0x07EF;
			if (CanPressedButton == ID_GP_BUTTON_HELP)
				gumpID = 0x07F0; //Paperdoll button Help (down)
			else if (CanSelectedButton == ID_GP_BUTTON_HELP)
				gumpID = 0x07F1; //Paperdoll button Help
			UO->DrawGump(gumpID, 0, posX + 185, posY + 44);
				
			gumpID = 0x07D6;
			if (CanPressedButton == ID_GP_BUTTON_OPTIONS)
				gumpID = 0x07D7; //Paperdoll button Options (down)
			else if (CanSelectedButton == ID_GP_BUTTON_OPTIONS)
				gumpID = 0x07D8; //Paperdoll button Options
			UO->DrawGump(gumpID, 0, posX + 185, posY + 71);
				
			gumpID = 0x07D9;
			if (CanPressedButton == ID_GP_BUTTON_LOGOUT)
				gumpID = 0x07DA; //Paperdoll button Log Out (down)
			else if (CanSelectedButton == ID_GP_BUTTON_LOGOUT)
				gumpID = 0x07DB; //Paperdoll button Log Out
			UO->DrawGump(gumpID, 0, posX + 185, posY + 98);
				
			gumpID = 0x07DC;
			if (CanPressedButton == ID_GP_BUTTON_JOURNAL)
				 gumpID = 0x07DD; //Paperdoll button Journal (down)
			else if (CanSelectedButton == ID_GP_BUTTON_JOURNAL)
				gumpID = 0x07DE; //Paperdoll button Journal
			UO->DrawGump(gumpID, 0, posX + 185, posY + 125);
				
			gumpID = 0x07DF;
			if (CanPressedButton == ID_GP_BUTTON_SKILLS)
				gumpID = 0x07E0; //Paperdoll button Skills (down)
			else if (CanSelectedButton == ID_GP_BUTTON_SKILLS)
				gumpID = 0x07E1; //Paperdoll button Skills
			UO->DrawGump(gumpID, 0, posX + 185, posY + 152);
			
			gumpID = 0x07E2;
			WORD chatColor = 0;

			if (!g_ChatEnabled)
				chatColor = 0x0386;
			else
			{
				if (CanPressedButton == ID_GP_BUTTON_CHAT)
					gumpID = 0x07E3; //Paperdoll button Chat (down)
				else if (CanSelectedButton == ID_GP_BUTTON_CHAT)
					gumpID = 0x07E4; //Paperdoll button Chat
			}

			UO->DrawGump(gumpID, chatColor, posX + 185, posY + 179);

			if (g_Player->Warmode)
			{
				gumpID = 0x07E8;

				if (CanPressedButton == ID_GP_BUTTON_WARMODE)
					gumpID = 0x07E9; //Paperdoll button War (down)
				else if (CanSelectedButton == ID_GP_BUTTON_WARMODE)
					gumpID = 0x07EA; //Paperdoll button War
			}
			else
			{
				gumpID = 0x07E5;

				if (CanPressedButton == ID_GP_BUTTON_WARMODE)
					gumpID = 0x07E6; //Paperdoll button Peace (down)
				else if (CanSelectedButton == ID_GP_BUTTON_WARMODE)
					gumpID = 0x07E7; //Paperdoll button Peace
			}

			UO->DrawGump(gumpID, 0, posX + 185, posY + 206);
			
			//UO->DrawGump(0x0FA1, 0, posX + 80, posY + 4); //Paperdoll mail bag
			UO->DrawGump(0x0071, 0, posX + 80, posY + 4); //David's star
			
			UO->DrawGump(0x07D2, 0, posX + 37, posY + 196); //Paperdoll party scroll
		}
		else
			UO->DrawGump(0x07D1, 0, posX, posY); //Paperdoll (other)

		gumpID = 0x07EB;
		if (CanPressedButton == ID_GP_BUTTON_STATUS)
			gumpID = 0x07EC; //Paperdoll button Status (down)
		else if (CanSelectedButton == ID_GP_BUTTON_STATUS)
			gumpID = 0x07ED; //Paperdoll button Status
		UO->DrawGump(gumpID, 0, posX + 185, posY + 233);
		
		if (obj->Graphic == 0x0190 || obj->Graphic == 0x0192)
			UO->DrawGump(0x000C, obj->Color, posX + 8, posY + 19, true); //Male gump
		else if (obj->Graphic == 0x0191 || obj->Graphic == 0x0193)
			UO->DrawGump(0x000D, obj->Color, posX + 8, posY + 19, true); //Female gump
		else if (obj->Graphic == 0x03DB)
		{
			UO->DrawGump(0x000C, 0x03EA, posX + 8, posY + 19, true); //Male gump
			UO->DrawGump(0xC72B, obj->Color, posX + 8, posY + 19); //GM robe gump
		}

		UO->DrawGump(0x07D2, 0, posX + 23, posY + 196); //Paperdoll profile scroll

		obj->m_PaperdollTextTexture.Draw(posX + 39, posY + 262);

		posX += 8;
		posY += 19;

		int gumpOffset = (obj->Sex ? g_FemaleGumpOffset : g_MaleGumpOffset);

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
						cOfs = g_MaleGumpOffset;

					UO->DrawGump(equipment->AnimID + cOfs, equipment->Color, posX, posY);
				}
				else if (g_LastSelectedGump == index && ObjectInHand != NULL && UsedLayers[i] == ObjectInHand->UsedLayer && ObjectInHand->AnimID)
				{
					equipment = obj->FindLayer(ObjectInHand->UsedLayer);

					if (equipment == NULL)
					{
						int cOfs = gumpOffset;

						if (obj->Sex && !UO->ExecuteGump(ObjectInHand->AnimID + cOfs))
							cOfs = g_MaleGumpOffset;

						glEnable(GL_BLEND);
						glBlendFunc(GL_SRC_COLOR, GL_ONE_MINUS_DST_COLOR);

						UO->DrawGump(ObjectInHand->AnimID + cOfs, ObjectInHand->Color, posX, posY);
						
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
					cOfs = g_MaleGumpOffset;

				UO->DrawGump(equipment->AnimID + cOfs, equipment->Color, posX, posY);
			}
		}

		equipment = obj->FindLayer(OL_BACKPACK);
		if (equipment != NULL && equipment->AnimID != 0)
			UO->DrawGump(equipment->AnimID + 50000, equipment->Color, posX, posY);

	glEndList();

	FrameRedraw = true;
	FrameCreated = true;
}
//---------------------------------------------------------------------------
int TGumpPaperdoll::Draw(bool &mode)
{
	TGameCharacter *obj = World->FindWorldCharacter(Serial);
	if (obj == NULL)
		return 0;
	
	DWORD index = (DWORD)this;

	int posX = X;
	int posY = Y;

	if (Minimized)
	{
		posX = MinimizedX;
		posY = MinimizedY;
	}
	
	//Нажата ли кнопка в окне?
	bool IsPressed = (g_LeftMouseDown && g_LastGumpLeftMouseDown == index && g_LastSelectedGump == index);

	//Может ли быть подсвечен элемент?
	int CanSelectedButton = ((g_LastSelectedGump == index) ? g_LastSelectedObject : 0);
	
	//Может ли быть нажат элемент?
	int CanPressedButton = 0;
	if (IsPressed && g_LastObjectLeftMouseDown == g_LastSelectedObject)
		CanPressedButton = g_LastObjectLeftMouseDown;
	
	if (g_LastObjectType == SOT_TEXT_OBJECT)
	{
		CanSelectedButton = false;
		CanPressedButton = false;
	}

	if (mode && !g_Player->Dead() && Serial == g_PlayerSerial && IsPressed && ObjectInHand == NULL && g_LastObjectLeftMouseDown != 0xFFFFFFFF && ((g_MouseX != g_DroppedLeftMouseX || g_MouseY != g_DroppedLeftMouseY) || (g_LastGumpMouseDownTime + DCLICK_DELAY < GetTickCount())))
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

	if (CanBeMoved() && g_LeftMouseDown && g_LastGumpLeftMouseDown == index && (!g_LastObjectLeftMouseDown || g_LastObjectLeftMouseDown == ID_GP_ITEMS + OL_BACKPACK) && ObjectInHand == NULL)
	{
		posX += g_MouseX - g_DroppedLeftMouseX;
		posY += g_MouseY - g_DroppedLeftMouseY;
		
		if (mode)
			GenerateFrame(posX, posY);
	}
	else if (mode)
	{
		if (IsPressed || CanSelectedButton)
			GenerateFrame(posX, posY);
		else if (FrameRedraw)
		{
			GenerateFrame(posX, posY);
			FrameRedraw = false;
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
						GenerateFrame(posX, posY);
						break;
					}
				}
			}
		}
	}

	//Вычисление положения, прозрачности и отрисовка текста
	TRenderTextObject *rto = TextRenderer->m_Items;
	
	DWORD ticks = GetTickCount();

	if (!Minimized)
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
						int drawX = posX + td->DrawX - go->GetTextOffsetX(td);
						int drawY = posY + td->DrawY - go->GetTextOffsetY(td);

						TEXT_IMAGE_BOUNDS ib;
						ib.X = drawX;
						ib.Y = drawY;
						ib.Width = drawX + tth.Width;
						ib.Height = drawY + tth.Height;
						ib.m_Text = td;

						td->Transparent = TextRenderer->InRect(ib, go);

						TextRenderer->AddRect(ib);
					}
				}
				else
				{
					TTextTexture &tth = td->m_Texture;
					int drawX = posX + td->DrawX - (tth.Width / 2);
					int drawY = posY + td->DrawY;

					TTextData *tdBuf = td;

					while (tdBuf != NULL)
					{
						drawY -= tdBuf->m_Texture.Height;

						tdBuf = tdBuf->m_Next;
					}

					TEXT_IMAGE_BOUNDS ib;
					ib.X = drawX;
					ib.Y = drawY + 1;
					ib.Width = drawX + tth.Width;
					ib.Height = drawY + tth.Height;
					ib.m_Text = td;

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
		if (!FrameCreated)
			GenerateFrame(posX, posY);

		glCallList((GLuint)index);
		
		if (!Minimized)
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

					int drawX = posX + td->DrawX - (tth.Width / 2);
					int drawY = posY + td->DrawY;
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

		if (g_ShowGumpLocker)
			g_GL.Draw(g_TextureGumpState[LockMoving], (GLfloat)posX, (GLfloat)posY, 10.0f, 14.0f);
	}
	else
	{
		int LSG = 0;

		if (Serial == g_PlayerSerial)
		{
			if (Minimized)
			{
				if (UO->GumpPixelsInXY(0x07EE, posX, posY))
				{
					g_LastSelectedObject = 0;
					g_LastSelectedGump = index; //Paperdoll button Character
				}

				return 0;
			}
			else if (UO->GumpPixelsInXY(0x07D0, posX, posY))
			{
				g_LastSelectedObject = 0;
				g_LastSelectedGump = index; //Paperdoll (self)
			}
			
			WORD gumpID = (g_Player->Warmode ? 0x07E8 : 0x07E5); //Paperdoll button War or Peace
			if (UO->GumpPixelsInXY(0x07EF, posX + 185, posY + 44))
				LSG = ID_GP_BUTTON_HELP; //Paperdoll button Help
			else if (UO->GumpPixelsInXY(0x07D6, posX + 185, posY + 71))
				LSG = ID_GP_BUTTON_OPTIONS; //Paperdoll button Options
			else if (UO->GumpPixelsInXY(0x07D9, posX + 185, posY + 98))
				LSG = ID_GP_BUTTON_LOGOUT; //Paperdoll button Log Out
			else if (UO->GumpPixelsInXY(0x07DC, posX + 185, posY + 125))
				LSG = ID_GP_BUTTON_JOURNAL; //Paperdoll button Journal
			else if (UO->GumpPixelsInXY(0x07DF, posX + 185, posY + 152))
				LSG = ID_GP_BUTTON_SKILLS; //Paperdoll button Skills
			else if (UO->GumpPixelsInXY(0x07E2, posX + 185, posY + 179))
				LSG = ID_GP_BUTTON_CHAT; //Paperdoll button Chat
			else if (UO->GumpPixelsInXY(gumpID, posX + 185, posY + 206))
				LSG = ID_GP_BUTTON_WARMODE; //Paperdoll button War/Peace
			else
			{
				RECT rc = {0, 0, 16, 16};
				POINT p = {g_MouseX - (posX + 226), g_MouseY - ((posY) + 258)};
				if (PtInRect(&rc, p))
					LSG = ID_GP_BUTTON_MINIMIZE;
				else if (UO->GumpPixelsInXY(0x07D2, posX + 37, posY + 196))
					LSG = ID_GP_PARTY_MANIFEST_SCROLL; //Paperdoll party manifest scroll
			}
		}
		else if (UO->GumpPixelsInXY(0x07D1, posX, posY))
		{
			g_LastSelectedObject = 0;
			g_LastSelectedGump = index; //Paperdoll (other)
		}
		
		if (UO->GumpPixelsInXY(0x07EB, posX + 185, posY + 233))
			LSG = ID_GP_BUTTON_STATUS; //Paperdoll button Status
		else if (UO->GumpPixelsInXY(0x07D2, posX + 23, posY + 196))
			LSG = ID_GP_PROFILE_SCROLL; //Paperdoll profile scroll

		int gumpOffset = (obj->Sex ? g_FemaleGumpOffset : g_MaleGumpOffset);
		
		posX += 8;
		posY += 19;

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
						cOfs = g_MaleGumpOffset;

					if (UO->GumpPixelsInXY(equipment->AnimID + cOfs, posX, posY))
						LSG = ID_GP_ITEMS + UsedLayers[i];
				}
			}
		}

		equipment = obj->FindLayer(OL_BACKPACK);
		if (equipment != NULL && equipment->AnimID != 0)
		{
			if (UO->GumpPixelsInXY(equipment->AnimID + 50000, posX, posY))
				LSG = ID_GP_ITEMS + OL_BACKPACK;
		}
		
		posX -= 8;
		posY -= 19;

		//Проверка текста
		for (; rto != NULL; rto = rto->m_PrevDraw)
		{
			if (!rto->IsText())
				continue;

			TTextData *td = (TTextData*)rto;
				
			if (td->Type != TT_SYSTEM && td->Timer >= ticks)
			{
				TTextTexture &tth = td->m_Texture;

				int drawX = posX + td->DrawX - (tth.Width / 2);
				int drawY = posY + td->DrawY;

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
		
		if (g_ShowGumpLocker && UO->PolygonePixelsInXY(posX, posY, 10, 14))
		{
			LSG = ID_GP_LOCK_MOVING;
			g_LastSelectedGump = index;
		}

		if (LSG != 0)
			g_LastSelectedObject = LSG;

		return LSG;
	}

	return 0;
}
//----------------------------------------------------------------------------
void TGumpPaperdoll::OnLeftMouseUp()
{
	bool IsPlayerPaperdoll = (Serial == g_PlayerSerial);

	TGameCharacter *container = World->FindWorldCharacter(Serial);
	if (container == NULL && g_LastSelectedObject >= ID_GP_ITEMS)
		return;

	//Что-то в руке
	if ((!g_LastSelectedObject || g_LastSelectedObject >= ID_GP_ITEMS) && ObjectInHand != NULL)
	{
		bool CanWear = true;

		if (!IsPlayerPaperdoll && GetDistance(g_Player, container) > 3)
			CanWear = false;

		if (CanWear && container != NULL)
		{
			int layer = g_LastSelectedObject - ID_GP_ITEMS;

			if (layer == OL_BACKPACK) //Ткнули на пак
			{
				TGameItem *equipment = container->FindLayer(layer);
				if (equipment != NULL)
				{
					if (!IsPlayerPaperdoll)
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
					if (!IsPlayerPaperdoll)
						UO->EquipItem(container->Serial);
					else
						UO->EquipItem();

					g_DroppedLeftMouseX = g_MouseX;
					g_DroppedLeftMouseY = g_MouseY;

					return;
				}
			}
		}
	}
	
	if (g_LastObjectLeftMouseDown != g_LastSelectedObject)
		return;

	if (g_LastObjectLeftMouseDown >= ID_GP_ITEMS)
	{
		int layer = g_LastObjectLeftMouseDown - ID_GP_ITEMS;
		TGameItem *equipment = container->FindLayer(layer);

		if (equipment == NULL)
			return;

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
		case ID_GP_BUTTON_JOURNAL: //Paperdoll button Journal
		{
			UO->OpenJournal();
			break;
		}
		case ID_GP_BUTTON_SKILLS: //Paperdoll button Skills
		{
			UO->OpenSkills();
			break;
		}
		case ID_GP_BUTTON_CHAT: //Paperdoll button Chat
		{
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
			UO->OpenStatus(Serial);
			g_LastLClickTime = 0;
			break;
		}
		case ID_GP_BUTTON_MINIMIZE: //Paperdoll button Minimize
		{
			Minimized = true;
			GenerateFrame(MinimizedX, MinimizedY);
			break;
		}
		case ID_GP_LOCK_MOVING:
		{
			LockMoving = !LockMoving;
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
				g_ClickObject.GumpType = GumpType;
				g_ClickObject.Serial = Serial;
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
	if (Minimized)
	{
		Minimized = false;
		GenerateFrame(X, Y);

		return true;
	}

	bool result = false;

	if (g_LastObjectLeftMouseDown < ID_GP_ITEMS)
	{
		if (g_LastObjectLeftMouseDown == ID_GP_PROFILE_SCROLL)
		{
			UO->OpenProfile(Serial);

			result = true;
		}
		else if (g_LastObjectLeftMouseDown == ID_GP_PARTY_MANIFEST_SCROLL)
		{
			UO->OpenPartyManifest();

			result = true;
		}

		return result;
	}

	TGameCharacter *container = World->FindWorldCharacter(Serial);
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

	return result;
}
//---------------------------------------------------------------------------