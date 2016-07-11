/****************************************************************************
**
** SecureTradingGump.cpp
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
TGumpSecureTrading::TGumpSecureTrading(DWORD serial, short x, short y, DWORD id, DWORD id2)
: TGump(GT_TRADE, serial, x, y), m_ID2(id2), m_StateMy(false), m_StateOpponent(false)
{
	m_ID = id;
	TextRenderer = new TTextRenderer();
}
//----------------------------------------------------------------------------
TGumpSecureTrading::~TGumpSecureTrading()
{
	if (TextRenderer != NULL)
	{
		delete TextRenderer;
		TextRenderer = NULL;
	}
}
//----------------------------------------------------------------------------
void TGumpSecureTrading::PrepareTextures()
{
	UO->ExecuteGumpPart(0x0866, 5); //Trade Gump

	TGameObject *container = World->FindWorldObject(m_ID);
	if (container != NULL && container->m_Items != NULL)
	{
		TGameItem *item = (TGameItem*)container->m_Items;

		while (item != NULL)
		{
			if (item->Count > 0)
			{
				bool doubleDraw = false;
				WORD graphic = item->GetDrawGraphic(doubleDraw);

				UO->ExecuteStaticArt(graphic);
			}

			item = (TGameItem*)item->m_Next;
		}
	}

	container = World->FindWorldObject(m_ID2);
	if (container != NULL && container->m_Items != NULL)
	{
		TGameItem *item = (TGameItem*)container->m_Items;

		while (item != NULL)
		{
			if (item->Count > 0)
			{
				bool doubleDraw = false;
				WORD graphic = item->GetDrawGraphic(doubleDraw);

				UO->ExecuteStaticArt(graphic);
			}

			item = (TGameItem*)item->m_Next;
		}
	}
}
//----------------------------------------------------------------------------
void TGumpSecureTrading::GenerateFrame()
{
	if (!g_DrawMode)
	{
		m_FrameRedraw = false;
		m_FrameCreated = false;

		return;
	}

	TGameObject *selobj = World->FindWorldObject(m_Serial);
	if (selobj == NULL)
		return; //Объект, к которому привязан гамп - исчез

	CalculateGumpState();

	if (g_LastObjectType == SOT_TEXT_OBJECT)
		g_GumpPressedElement = false;

	glNewList((GLuint)this, GL_COMPILE);

		UO->DrawGump(0x0866, 0, 0, 0); //Trade Gump

		//Наш чекбокс
		WORD gumpID = 0x0867; //Trade window checkbox unselected
		if (g_GumpSelectElement == ID_GST_CHECKBOX)
		{
			if (m_StateMy)
				gumpID = 0x086A; //Trade window checkbox selected (lighted)
			else
				gumpID = 0x0868; //Trade window checkbox unselected (lighted)
		}
		else if (m_StateMy)
			gumpID = 0x0869; //Trade window checkbox selected

		UO->DrawGump(gumpID, 0, 52, 29);
		
		FontManager->DrawA(1, g_Player->GetName().c_str(), 0x0386, 84, 40);

		//Чекбокс опонента
		gumpID = 0x0867; //Trade window checkbox unselected
		if (m_StateOpponent)
			gumpID = 0x0869; //Trade window checkbox selected

		UO->DrawGump(gumpID, 0, 266, 160);
		
		int fontWidth = 260 - FontManager->GetWidthA(1, m_Text.c_str(), m_Text.length());
		FontManager->DrawA(1, m_Text.c_str(), 0x0386, fontWidth, 170);
		
		DWORD ticks = GetTickCount();

		ColorizerShader->Use();

		//Отрисовка нашего товара (при наличии товара)
		TGameObject *container = World->FindWorldObject(m_ID);
		if (container != NULL && container->m_Items != NULL)
		{
			g_GL.Scissor((int)g_GumpTranslateX + 45, (int)g_GumpTranslateY + 70, 110, 80);

			QFOR(item, container->m_Items, TGameItem*)
			{
				bool doubleDraw = false;
				WORD graphic = item->GetDrawGraphic(doubleDraw);
				WORD color = item->Color;
				bool selMode = false;
				int drawX = 45 + item->X;
				int drawY = 70 + item->Y;

				if (g_GumpSelectElement == item->Serial)
				{
					color = 0x0035;
					selMode = true;
				}

				UO->DrawStaticArtInContainer(graphic, color, drawX, drawY, selMode);
				if (doubleDraw)
					UO->DrawStaticArtInContainer(graphic, color, drawX + 5, drawY + 5, selMode);
			}
		
			//Восстанавливаем размеры рисуемой области
			glDisable(GL_SCISSOR_TEST);
		}
		


		//Отрисовка нашего опонента (при наличии товара)
		container = World->FindWorldObject(m_ID2);
		if (container != NULL && container->m_Items != NULL)
		{
			g_GL.Scissor((int)g_GumpTranslateX + 192, (int)g_GumpTranslateY + 70, 110, 80);

			QFOR(item, container->m_Items, TGameItem*)
			{
				bool doubleDraw = false;
				WORD graphic = item->GetDrawGraphic(doubleDraw);
				WORD color = item->Color;
				bool selMode = false;
				int drawX = 192 + item->X;
				int drawY = 70 + item->Y;

				if (g_GumpSelectElement == item->Serial)
				{
					color = 0x0035;
					selMode = true;
				}

				UO->DrawStaticArtInContainer(graphic, color, drawX, drawY, selMode);

				if (doubleDraw)
					UO->DrawStaticArtInContainer(graphic, color, drawX + 5, drawY + 5, selMode);
			}

			//Восстанавливаем размеры рисуемой области
			glDisable(GL_SCISSOR_TEST);
		}

		UnuseShader();

	glEndList();

	m_FrameRedraw = true;
	m_FrameCreated = true;
}
//----------------------------------------------------------------------------
int TGumpSecureTrading::Draw(bool &mode)
{
	TGameObject *selobj = World->FindWorldObject(m_Serial);
	if (selobj == NULL)
		return 0; //Объект, к которому привязан гамп - исчез
	
	DWORD index = (DWORD)this;

	CalculateGumpState();

	if (g_LastObjectType == SOT_TEXT_OBJECT)
		g_GumpPressedElement = false;

	/*if (mode && GetTopObjDistance(g_Player, selobj) < 3 && IsPressed && ObjectInHand == NULL && g_LastObjectLeftMouseDown != ID_GST_CHECKBOX && ((g_MouseX != g_DroppedLeftMouseX || g_MouseY != g_DroppedLeftMouseY) || (g_LastGumpMouseDownTime + DCLICK_DELAY < GetTickCount())))
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

			UO->PickupItem(selobj);

			//g_LastGumpLeftMouseDown = 0;
			g_LastObjectLeftMouseDown = 0;
		}
	}*/

	if (mode)
		TextRenderer->ClearRect();

	DWORD ticks = GetTickCount();
	
	//Вычисление положения, прозрачности и отрисовка текста
	TRenderTextObject *rto = TextRenderer->m_Items;

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
				int drawX = td->DrawX - go->GetTextOffsetX(td);
				int drawY = td->DrawY - go->GetTextOffsetY(td);

				if (mode)
				{
					TTextImageBounds ib(drawX, drawY, tth.Width, tth.Height, td);

					td->Transparent = TextRenderer->InRect(ib, go);

					TextRenderer->AddRect(ib);
				}
			}
		}

		if (rto->m_NextDraw == NULL)
			break;
	}

	if (mode) //Отрисовка
	{
		if (!m_FrameCreated || g_GumpSelectElement || g_GumpMovingOffsetX || g_GumpMovingOffsetY) // || g_GumpPressed
			GenerateFrame();
		else if (m_FrameRedraw)
		{
			GenerateFrame();
			m_FrameRedraw = false;
		}

		glTranslatef(g_GumpTranslateX, g_GumpTranslateY, 0.0f);

		glCallList((GLuint)index);

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

		glTranslatef(-g_GumpTranslateX, -g_GumpTranslateY, 0.0f);
	}
	else //Выбор объектов
	{
		int oldMouseX = g_MouseX;
		int oldMouseY = g_MouseY;
		g_MouseX -= (int)g_GumpTranslateX;
		g_MouseY -= (int)g_GumpTranslateY;

		int LSG = 0;

		//Если выбран основной гамп - меняем глобальный указатель на выбранный гамп на него
		if (UO->GumpPixelsInXY(0x0866, 0, 0))
		{
			g_LastSelectedObject = 0;
			g_LastSelectedGump = index;
		}
		
		if (UO->GumpPixelsInXY(0x0867, 52, 29))
			LSG = ID_GST_CHECKBOX;

		//Проверка нашего товара (при наличии товара)
		TGameObject *container = World->FindWorldObject(m_ID);

		if (container != NULL && container->m_Items != NULL && UO->PolygonePixelsInXY(45, 70, 110, 80))
		{
			QFOR(item, container->m_Items, TGameItem*)
			{
				if (item->Count > 0)
				{
					bool doubleDraw = false;
					WORD graphic = item->GetDrawGraphic(doubleDraw);
					int drawX = 45 + item->X;
					int drawY = 70 + item->Y;

					if (UO->StaticPixelsInXYInContainer(graphic, drawX, drawY))
						LSG = item->Serial;
					else if (doubleDraw)
					{
						if (UO->StaticPixelsInXYInContainer(graphic, drawX + 5, drawY + 5))
							LSG = item->Serial;
					}
				}
			}
		}
		
		container = World->FindWorldObject(m_ID2);

		if (container != NULL && container->m_Items != NULL && UO->PolygonePixelsInXY(192, 70, 110, 80))
		{
			QFOR(item, container->m_Items, TGameItem*)
			{
				if (item->Count > 0)
				{
					bool doubleDraw = false;
					WORD graphic = item->GetDrawGraphic(doubleDraw);
					int drawX = 192 + item->X;
					int drawY = 70 + item->Y;

					if (UO->StaticPixelsInXYInContainer(graphic, drawX, drawY))
						LSG = item->Serial;
					else if (doubleDraw)
					{
						if (UO->StaticPixelsInXYInContainer(graphic, drawX + 5, drawY + 5))
							LSG = item->Serial;
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
				
			if (td->Type == TT_OBJECT)
			{
				if (td->Timer >= ticks)
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
		}

		g_MouseX = oldMouseX;
		g_MouseY = oldMouseY;

		return LSG;
	}

	return 0;
}
//----------------------------------------------------------------------------
void TGumpSecureTrading::OnLeftMouseUp()
{
	if (ObjectInHand != NULL)
	{
		int x = m_X;
		int y = m_Y;

		if (UO->PolygonePixelsInXY(x + 45, y + 70, 110, 80))
		{
			if (GetTopObjDistance(g_Player, World->FindWorldObject(m_ID2)) < 3)
			{
				x = g_MouseX - x - 45;
				y = g_MouseY - y - 70;

				bool doubleDraw = false;
				WORD graphic = ObjectInHand->GetDrawGraphic(doubleDraw);

				TTextureObject *th = UO->ExecuteStaticArt(graphic);

				if (th != NULL)
				{
					x -= (th->Width / 2);
					y -= (th->Height / 2);

					if (x + th->Width > 110)
						x = 110 - th->Width;

					if (y + th->Height > 80)
						y = 80 - th->Height;
				}

				if (x < 0)
					x = 0;

				if (y < 0)
					y = 0;

				UO->DropItem(m_ID, x, y, 0);
			}
			else
				UO->PlaySoundEffect(0x0051);
		}
	}

	if (g_LastObjectLeftMouseDown != g_LastSelectedObject)
		return; //Нажатый объект - не последний выбранный под курсором...валим
	
	if (g_LastObjectLeftMouseDown == ID_GST_CHECKBOX) //Изменение состояния чекбокса
	{
		m_StateMy = !m_StateMy;

		SendTradingResponse(2);

		m_FrameCreated = false;
	}
	else
	{
		if (!g_ClickObjectReq)
		{
			TGameObject *click_target = World->FindWorldObject(g_LastObjectLeftMouseDown);
			if (click_target == NULL)
				return;

			g_ClickObjectReq = true;
			g_ClickObject.Init(COT_GAME_OBJECT);
			g_ClickObject.Serial = g_LastObjectLeftMouseDown;
			g_ClickObject.Timer = GetTickCount() + DCLICK_DELAY;
			g_ClickObject.X = g_MouseX - m_X;
			g_ClickObject.Y = g_MouseY - m_Y;
		}
	}
}
//----------------------------------------------------------------------------