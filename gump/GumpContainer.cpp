/****************************************************************************
**
** ContainerGump.cpp
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
TGumpContainer::TGumpContainer(DWORD serial, short x, short y)
: TGump(GT_CONTAINER, serial, x, y), m_CorpseEyesTicks(0), m_CorpseEyesOffset(0),
m_IsGameBoard(false)
{
	TextRenderer = new TTextRenderer();
}
//---------------------------------------------------------------------------
TGumpContainer::~TGumpContainer()
{
	if (TextRenderer != NULL)
	{
		delete TextRenderer;
		TextRenderer = NULL;
	}
}
//---------------------------------------------------------------------------
void TGumpContainer::PrepareTextures()
{
	UO->ExecuteGump(0x0050); //Minimized
	UO->ExecuteGumpPart(0x0045, 2); //Corpse eyes
	UO->ExecuteGump((WORD)ID);
	
	TGameItem *container = World->FindWorldItem(Serial);
	if (container != NULL)
	{
		for (TGameItem *obj = (TGameItem*)container->m_Items; obj != NULL; obj = (TGameItem*)obj->m_Next)
		{
			if (obj->Layer == OL_NONE && obj->Count > 0)
			{
				if (m_IsGameBoard)
					UO->ExecuteGump(obj->Graphic - GAME_FIGURE_GUMP_OFFSET);
				else
					UO->ExecuteStaticArt(obj->Graphic);
			}
		}
	}
}
//---------------------------------------------------------------------------
void TGumpContainer::GenerateFrame(int posX, int posY)
{
	if (!g_DrawMode)
	{
		FrameRedraw = false;
		FrameCreated = false;

		return;
	}

	TGameItem *container = World->FindWorldItem(Serial);
	if (container == NULL)
		return;

	DWORD index = (DWORD)this;

	//Нажата ли кнопка в окне?
	bool IsPressed = (g_LeftMouseDown && g_LastGumpLeftMouseDown == index && g_LastSelectedGump == index);

	//Может ли быть подсвечен элемент?
	int CanSelectedButton = ((g_LastSelectedGump == index) ? g_LastSelectedObject : 0);
	
	if (g_LastObjectType == SOT_TEXT_OBJECT)
		CanSelectedButton = false;

	glNewList((GLuint)index, GL_COMPILE);
	
		if (Minimized)
		{
			UO->DrawGump(0x0050, 0, posX, posY);

			glEndList();

			FrameRedraw = true;
			FrameCreated = true;

			return;
		}

		UO->DrawGump((WORD)ID, 0, posX, posY);

		if (ID == 0x09)
		{
			if (m_CorpseEyesTicks < GetTickCount())
			{
				m_CorpseEyesOffset = (BYTE)!m_CorpseEyesOffset;
				m_CorpseEyesTicks = GetTickCount() + 750;
			}

			UO->DrawGump(0x0045 + m_CorpseEyesOffset, 0, posX + 45, posY + 30);
		}

		bool canSelect = (ObjectInHand == NULL || (ObjectInHand != NULL && !g_LeftMouseDown));
		bool isCorpse = (container->Graphic == 0x2006);
		
		ColorizerShader->Use();

		for (TGameItem *obj = (TGameItem*)container->m_Items; obj != NULL; obj = (TGameItem*)obj->m_Next)
		{
			int count = obj->Count;

			if ((obj->Layer == OL_NONE || (isCorpse && g_LayerUnsafe[obj->Layer])) && count > 0)
			{
				WORD graphic = obj->Graphic;
				WORD color = obj->Color;
				bool doubleDraw = false;
				bool selMode = false;
				int drawX = posX + obj->X;
				int drawY = posY + obj->Y;

				if (canSelect && CanSelectedButton == obj->Serial)
				{
					color = 0x0035;
					selMode = true;
				}
				
				if (m_IsGameBoard)
					UO->DrawGump(graphic - GAME_FIGURE_GUMP_OFFSET, color, drawX, drawY - 20);
				else
				{
					if (count > 1)
					{
						if (graphic == 0x0EED)
						{
							if (count > 5)
								graphic = 0x0EEF;
							else
								graphic = 0x0EEE;
						}
						else if (obj->IsStackable())
							doubleDraw = true;
					}

					UO->DrawStaticArtInContainer(graphic, color, drawX, drawY, selMode);

					if (doubleDraw)
						UO->DrawStaticArtInContainer(graphic, color, drawX + 5, drawY + 5, selMode);
				}
			}
		}

		UnuseShader();

	glEndList();

	FrameRedraw = true;
	FrameCreated = true;
}
//----------------------------------------------------------------------------
int TGumpContainer::Draw(bool &mode)
{
	DWORD index = (DWORD)this;

	//Нажата ли кнопка в окне?
	bool IsPressed = (g_LeftMouseDown && g_LastGumpLeftMouseDown == index && g_LastSelectedGump == index);

	//Может ли быть подсвечен элемент?
	int CanSelectedButton = ((g_LastSelectedGump == index) ? g_LastSelectedObject : 0);

	if (g_LastObjectType == SOT_TEXT_OBJECT)
		CanSelectedButton = false;
	
	int posX = X;
	int posY = Y;

	if (Minimized)
	{
		posX = MinimizedX;
		posY = MinimizedY;
	}

	if (mode && GetTopObjDistance(g_Player, World->FindWorldObject(Serial)) < 3 && IsPressed && ObjectInHand == NULL && g_LastObjectLeftMouseDown != ID_GC_MINIMIZE && ((g_MouseX != g_DroppedLeftMouseX || g_MouseY != g_DroppedLeftMouseY) || (g_LastGumpMouseDownTime + DCLICK_DELAY < GetTickCount())))
	{
		TGameItem *selobj = World->FindWorldItem(g_LastObjectLeftMouseDown);
		if (selobj != NULL && selobj->IsStackable() && selobj->Count > 1)
		{
			TGumpDrag *newgump = new TGumpDrag(g_LastObjectLeftMouseDown, g_MouseX - 80, g_MouseY - 34);

			char text_amount[20] = {0};
			sprintf(text_amount, "%d", selobj->Count);

			TGump *gumpEntry = GumpManager->GetTextEntryOwner();

			EntryPointer = newgump->TextEntry;
			EntryPointer->SetText(text_amount);
			
			if (gumpEntry != NULL)
				gumpEntry->UpdateFrame();

			GumpManager->AddGump(newgump);
			CurrentScreen->OnLeftMouseDown();
			selobj->Dragged = true;
		}
		else if (selobj != NULL)
		{
			if (Target.IsTargeting())
				Target.SendCancelTarget();

			UO->PickupItem(selobj, 0, m_IsGameBoard);

			g_LastGumpLeftMouseDown = 0;
			g_LastObjectLeftMouseDown = 0;

			UpdateFrame();
		}
	}
	
	if (CanBeMoved() && g_LeftMouseDown && g_LastGumpLeftMouseDown == index && !g_LastObjectLeftMouseDown && ObjectInHand == NULL)
	{
		posX += g_MouseX - g_DroppedLeftMouseX;
		posY += g_MouseY - g_DroppedLeftMouseY;
		
		if (mode)
			GenerateFrame(posX, posY);
	}
	else if (mode)
	{
		if (IsPressed || CanSelectedButton || (ID == 0x09 && m_CorpseEyesTicks < GetTickCount()))
			GenerateFrame(posX, posY);
		else if (FrameRedraw)
		{
			GenerateFrame(posX, posY);
			FrameRedraw = false;
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
			if (!rto->IsText())
			{
				if (rto->m_NextDraw == NULL)
					break;

				continue;
			}

			TTextData *td = (TTextData*)rto;

			if (td->Type == TT_OBJECT && td->Timer >= ticks)
			{
				TGameObject *go = World->FindWorldObject(td->Serial);

				if (go != NULL && go->Graphic < 0x4000)
				{
					TTextTexture &tth = td->m_Texture;
					int drawX = posX + td->DrawX - go->GetTextOffsetX(td);
					int drawY = posY + td->DrawY - go->GetTextOffsetY(td);

					if (mode)
					{
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
				
				if (td->Type == TT_OBJECT && td->Timer >= ticks)
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
			g_GL.Draw(g_TextureGumpState[LockMoving], posX, posY, 10.0f, 14.0f);
	}
	else
	{
		if (Minimized)
		{
			if (UO->GumpPixelsInXY(0x0050, posX, posY))
			{
				g_LastSelectedObject = 0;
				g_LastSelectedGump = index;
			}

			return 0;
		}

		if (UO->GumpPixelsInXY((WORD)ID, posX, posY))
		{
			g_LastSelectedObject = 0;
			g_LastSelectedGump = index;
		}

		int LSG = 0;

		RECT rc = {0, 0, 16, 16};
		POINT p = {g_MouseX - (posX + 106), g_MouseY - (posY + 162)};

		if (PtInRect(&rc, p))
			LSG = ID_GC_MINIMIZE;

		TGameItem *container = World->FindWorldItem(Serial);

		if (container != NULL)
		{
			bool isCorpse = (container->Graphic == 0x2006);

			for (TGameItem *obj = (TGameItem*)container->m_Items; obj != NULL; obj = (TGameItem*)obj->m_Next)
			{
				int count = obj->Count;

				if ((obj->Layer == OL_NONE || (isCorpse && g_LayerUnsafe[obj->Layer])) && count > 0)
				{
					WORD graphic = obj->Graphic;
					int drawX = posX + obj->X;
					int drawY = posY + obj->Y;

					if (m_IsGameBoard)
					{
						if (UO->GumpPixelsInXY(graphic - GAME_FIGURE_GUMP_OFFSET, drawX, drawY - 20))
							LSG = obj->Serial;
					}
					else
					{
						bool doubleDraw = false;

						if (count > 1)
						{
							if (graphic == 0x0EED)
							{
								if (count > 5)
									graphic = 0x0EEF;
								else
									graphic = 0x0EEE;
							}
							else if (obj->IsStackable())
								doubleDraw = true;
						}
					
						if (UO->StaticPixelsInXYInContainer(graphic, drawX, drawY))
							LSG = obj->Serial;

						if (doubleDraw)
						{
							if (UO->StaticPixelsInXYInContainer(graphic, drawX + 5, drawY + 5))
								LSG = obj->Serial;
						}
					}
				}
			}
		}
		
		//Проверка текста
		for (; rto != NULL; rto = rto->m_PrevDraw)
		{
			if (!rto->IsText())
				continue;

			TTextData *td = (TTextData*)rto;
				
			if (td->Type == TT_OBJECT && td->Timer >= ticks)
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
			LSG = ID_GC_LOCK_MOVING;

		if (LSG != 0)
		{
			g_LastSelectedObject = LSG;
			g_LastSelectedGump = index;
		}

		return LSG;
	}

	return 0;
}
//----------------------------------------------------------------------------
void TGumpContainer::OnLeftMouseUp()
{
	if (!Minimized && g_LastObjectLeftMouseDown == ID_GC_MINIMIZE && ID == 0x003C)
	{
		Minimized = true;
		GenerateFrame(MinimizedX, MinimizedY);

		return;
	}
	else if (g_LastObjectLeftMouseDown == ID_GC_LOCK_MOVING)
	{
		LockMoving = !LockMoving;
		g_CancelDoubleClick = true;

		return;
	}

	DWORD dropContainer = Serial;

	bool canDrop = (GetTopObjDistance(g_Player, World->FindWorldObject(dropContainer)) < 3);

	if (canDrop && g_LastSelectedObject && g_LastSelectedObject != ID_GC_MINIMIZE && g_LastSelectedObject != ID_GC_LOCK_MOVING)
	{
		canDrop = false;

		if (Target.IsTargeting())
		{
			Target.SendTargetObject(g_LastSelectedObject);

			return;
		}
		else if (ObjectInHand != NULL)
		{
			canDrop = true;

			TGameItem *target = World->FindWorldItem(g_LastSelectedObject);
			if (target != NULL)
			{
				if (target->IsContainer())
					dropContainer = target->Serial;
				else if (target->IsStackable() && target->Graphic == ObjectInHand->Graphic)
					dropContainer = target->Serial;
			}
		}
	}
	
	int x = g_MouseX - X;
	int y = g_MouseY - Y;

	if (canDrop && ObjectInHand != NULL)
	{
		CONTAINER_OFFSET_RECT &r = g_ContainerOffset[Graphic].rect;

		WORD graphic = ObjectInHand->Graphic;
		if (graphic == 0x0EED)
		{
			if (ObjectInHand->Count > 5)
				graphic = 0x0EEF;
			else if (ObjectInHand->Count > 1)
				graphic = 0x0EEE;
		}

		TTextureObject *th = UO->ExecuteStaticArt(graphic);
		if (m_IsGameBoard)
		{
			th = UO->ExecuteGump(graphic - GAME_FIGURE_GUMP_OFFSET);
			y += 20;
		}

		if (th != NULL)
		{
			x -= (th->Width / 2);
			y -= (th->Height / 2);

			if (x + th->Width > r.MaxX)
				x = r.MaxX - th->Width;

			if (y + th->Height > r.MaxY)
				y = r.MaxY - th->Height;
		}

		if (x < r.MinX)
			x = r.MinX;

		if (y < r.MinY)
			y = r.MinY;

		UO->DropItem(dropContainer, x, y, 0);
	}
	else if (ObjectInHand == NULL)
	{
		if (!g_ClickObjectReq)
		{
			TGameObject *clickTarget = World->FindWorldObject(g_LastObjectLeftMouseDown);

			if (clickTarget != NULL)
			{
				g_ClickObjectReq = true;
				g_ClickObject.Init(COT_GAME_OBJECT);
				g_ClickObject.Serial = g_LastObjectLeftMouseDown;
				g_ClickObject.Timer = GetTickCount() + DCLICK_DELAY;
				g_ClickObject.X = x;
				g_ClickObject.Y = y;
			}
		}
	}
}
//----------------------------------------------------------------------------
bool TGumpContainer::OnLeftMouseDoubleClick()
{
	bool result = false;

	if (!g_LastObjectLeftMouseDown && Minimized && ID == 0x003C)
	{
		Minimized = false;
		GenerateFrame(X, Y);

		result = true;
	}
	else if (g_LastSelectedObject && g_LastSelectedObject != ID_GC_MINIMIZE)
	{
		UO->DoubleClick(g_LastSelectedObject);

		result = true;
	}

	return result;
}
//----------------------------------------------------------------------------