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

	if (m_Graphic < CONTAINERS_COUNT)
	{
		WORD sound = g_ContainerOffset[m_Graphic].CloseSound;

		if (sound)
			UO->PlaySoundEffect(sound);
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
		QFOR(obj, container->m_Items, TGameItem*)
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
//----------------------------------------------------------------------------
void TGumpContainer::OnToolTip()
{
	if (!m_Minimized)
	{
		DWORD selected = ((g_LastSelectedGump == (DWORD)this) ? g_LastSelectedObject : 0);

		TGameObject *obj = World->FindWorldObject(selected);

		if (obj != NULL && obj->ClilocMessage.length())
			ToolTip.Set(obj->ClilocMessage, SOT_GAME_OBJECT, selected);
	}

	/*DWORD selected = ((g_LastSelectedGump == (DWORD)this) ? g_LastSelectedObject : 0);

	if (!m_Minimized && selected)
	{
		TGameItem *container = World->FindWorldItem(Serial);

		if (container != NULL)
		{
			QFOR(obj, container->m_Items, TGameItem*)
			{
				if (selected == obj->Serial && obj->ClilocMessage.length())
				{
					ToolTip.Set(obj->ClilocMessage, SOT_GAME_OBJECT, selected);

					break;
				}
			}
		}
	}*/
}
//---------------------------------------------------------------------------
void TGumpContainer::GenerateFrame()
{
	if (!g_DrawMode)
	{
		m_FrameRedraw = false;
		m_FrameCreated = false;

		return;
	}

	TGameItem *container = World->FindWorldItem(Serial);

	if (container == NULL)
		return;

	CalculateGumpState();
	
	glNewList((GLuint)this, GL_COMPILE);
	
		if (m_Minimized)
			UO->DrawGump(0x0050, 0, 0, 0);
		else
		{
			UO->DrawGump((WORD)ID, 0, 0, 0);

			if (ID == 0x09)
			{
				if (m_CorpseEyesTicks < GetTickCount())
				{
					m_CorpseEyesOffset = (BYTE)!m_CorpseEyesOffset;
					m_CorpseEyesTicks = GetTickCount() + 750;
				}

				UO->DrawGump(0x0045 + m_CorpseEyesOffset, 0, 45, 30);
			}

			bool canSelect = (ObjectInHand == NULL || (ObjectInHand != NULL && !g_LeftMouseDown));

			ColorizerShader->Use();

			QFOR(obj, container->m_Items, TGameItem*)
			{
				int count = obj->Count;

				if ((obj->Layer == OL_NONE || (container->IsCorpse() && LAYER_UNSAFE[obj->Layer])) && count > 0)
				{
					bool doubleDraw = false;
					WORD graphic = obj->GetDrawGraphic(doubleDraw);
					WORD color = obj->Color;
					bool selMode = false;

					if (canSelect && g_GumpSelectElement == obj->Serial)
					{
						color = 0x0035;
						selMode = true;
					}

					if (m_IsGameBoard)
						UO->DrawGump(graphic - GAME_FIGURE_GUMP_OFFSET, color, obj->X, obj->Y - 20);
					else
					{
						UO->DrawStaticArtInContainer(graphic, color, obj->X, obj->Y, selMode);

						if (doubleDraw)
							UO->DrawStaticArtInContainer(graphic, color, obj->X + 5, obj->Y + 5, selMode);
					}
				}
			}

			UnuseShader();
		}

	glEndList();

	m_FrameRedraw = true;
	m_FrameCreated = true;
}
//----------------------------------------------------------------------------
int TGumpContainer::Draw(bool &mode)
{
	CalculateGumpState();

	if (mode && GetTopObjDistance(g_Player, World->FindWorldObject(Serial)) < 3 && g_GumpPressed && ObjectInHand == NULL && g_LastObjectLeftMouseDown != ID_GC_MINIMIZE && ((g_MouseX != g_DroppedLeftMouseX || g_MouseY != g_DroppedLeftMouseY) || (g_LastGumpMouseDownTime + DCLICK_DELAY < GetTickCount())))
	{
		TGameItem *selobj = World->FindWorldItem(g_LastObjectLeftMouseDown);
		if (selobj != NULL && selobj->IsStackable() && selobj->Count > 1)
		{
			TGumpDrag *newgump = new TGumpDrag(g_LastObjectLeftMouseDown, g_MouseX - 80, g_MouseY - 34);

			TGump *gumpEntry = GumpManager->GetTextEntryOwner();

			EntryPointer = newgump->TextEntry;
			EntryPointer->SetText(std::to_string(selobj->Count));
			
			if (gumpEntry != NULL)
				gumpEntry->FrameCreated = false;

			GumpManager->AddGump(newgump);
			CurrentScreen->OnLeftMouseDown();
			selobj->Dragged = true;

			CalculateGumpState();
		}
		else if (selobj != NULL)
		{
			if (Target.IsTargeting())
				Target.SendCancelTarget();

			UO->PickupItem(selobj, 0, m_IsGameBoard);

			g_LastGumpLeftMouseDown = 0;
			g_LastObjectLeftMouseDown = 0;

			m_FrameCreated = false;
		}
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
			if (!rto->IsText())
			{
				if (rto->m_NextDraw == NULL)
					break;

				continue;
			}

			TTextData *td = (TTextData*)rto;

			if (mode && td->Type == TT_OBJECT && td->Timer >= ticks)
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

			if (rto->m_NextDraw == NULL)
				break;
		}
	}

	DWORD index = (DWORD)this;

	if (mode)
	{
		if (!m_FrameCreated || g_GumpSelectElement || (ID == 0x09 && m_CorpseEyesTicks < GetTickCount()))
			GenerateFrame();
		else if (m_FrameRedraw)
		{
			GenerateFrame();
			m_FrameRedraw = false;
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
				
				if (td->Type == TT_OBJECT && td->Timer >= ticks)
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

		if (m_Minimized)
		{
			if (UO->GumpPixelsInXY(0x0050, 0, 0))
			{
				g_LastSelectedObject = 0;
				g_LastSelectedGump = index;
			}
		}
		else
		{
			if (UO->GumpPixelsInXY((WORD)m_ID, 0, 0))
			{
				g_LastSelectedObject = 0;
				g_LastSelectedGump = index;
			}

			if (UO->PolygonePixelsInXY(106, 162, 16, 16))
				LSG = ID_GC_MINIMIZE;

			TGameItem *container = World->FindWorldItem(Serial);

			if (container != NULL)
			{
				QFOR(obj, container->m_Items, TGameItem*)
				{
					int count = obj->Count;

					if ((obj->Layer == OL_NONE || (container->IsCorpse() && LAYER_UNSAFE[obj->Layer])) && count > 0)
					{
						bool doubleDraw = false;
						WORD graphic = obj->GetDrawGraphic(doubleDraw);

						if (m_IsGameBoard)
						{
							if (UO->GumpPixelsInXY(graphic - GAME_FIGURE_GUMP_OFFSET, obj->X, obj->Y - 20))
								LSG = obj->Serial;
						}
						else
						{
							if (UO->StaticPixelsInXYInContainer(graphic, obj->X, obj->Y))
								LSG = obj->Serial;

							if (doubleDraw)
							{
								if (UO->StaticPixelsInXYInContainer(graphic, obj->X + 5, obj->Y + 5))
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
				LSG = ID_GC_LOCK_MOVING;

			if (LSG != 0)
			{
				g_LastSelectedObject = LSG;
				g_LastSelectedGump = index;
			}
		}

		g_MouseX = oldMouseX;
		g_MouseY = oldMouseY;

		return LSG;
	}

	return 0;
}
//----------------------------------------------------------------------------
void TGumpContainer::OnLeftMouseUp()
{
	if (!m_Minimized && g_LastObjectLeftMouseDown == ID_GC_MINIMIZE && m_ID == 0x003C)
	{
		m_Minimized = true;
		m_FrameCreated = false;

		return;
	}
	else if (g_LastObjectLeftMouseDown == ID_GC_LOCK_MOVING)
	{
		m_LockMoving = !m_LockMoving;
		g_CancelDoubleClick = true;

		return;
	}

	DWORD dropContainer = m_Serial;

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

	if (!canDrop && ObjectInHand != NULL)
		UO->PlaySoundEffect(0x0051);

	int x = g_MouseX - m_X;
	int y = g_MouseY - m_Y;

	if (canDrop && ObjectInHand != NULL)
	{
		CONTAINER_OFFSET_RECT &r = g_ContainerOffset[Graphic].rect;

		bool doubleDraw = false;
		WORD graphic = ObjectInHand->GetDrawGraphic(doubleDraw);

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

		if (dropContainer != m_Serial)
		{
			TGameItem *target = World->FindWorldItem(g_LastSelectedObject);

			if (target->IsContainer())
			{
				x = -1;
				y = -1;
			}
		}

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

	if (!g_LastObjectLeftMouseDown && m_Minimized && m_ID == 0x003C)
	{
		m_Minimized = false;
		m_FrameCreated = false;

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