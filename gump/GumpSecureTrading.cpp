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
	ID = id;
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

	TGameObject *container = World->FindWorldObject(ID);
	if (container != NULL && container->m_Items != NULL)
	{
		TGameItem *item = (TGameItem*)container->m_Items;

		while (item != NULL)
		{
			if (item->Count > 0)
			{
				WORD graphic = item->Graphic;

				if (item->Count > 1)
				{
					if (graphic == 0x0EED)
					{
						if (item->Count > 5)
							graphic = 0x0EEF;
						else
							graphic = 0x0EEE;
					}
				}

				UO->ExecuteStaticArt(graphic);
			}

			item = (TGameItem*)item->m_Next;
		}
	}

	container = World->FindWorldObject(ID2);
	if (container != NULL && container->m_Items != NULL)
	{
		TGameItem *item = (TGameItem*)container->m_Items;

		while (item != NULL)
		{
			if (item->Count > 0)
			{
				WORD graphic = item->Graphic;

				if (item->Count > 1)
				{
					if (graphic == 0x0EED)
					{
						if (item->Count > 5)
							graphic = 0x0EEF;
						else
							graphic = 0x0EEE;
					}
				}

				UO->ExecuteStaticArt(graphic);
			}

			item = (TGameItem*)item->m_Next;
		}
	}
}
//----------------------------------------------------------------------------
void TGumpSecureTrading::GenerateFrame(int posX, int posY)
{
	if (!g_DrawMode)
	{
		FrameRedraw = false;
		FrameCreated = false;

		return;
	}

	TGameObject *selobj = World->FindWorldObject(Serial);
	if (selobj == NULL)
		return; //Объект, к которому привязан гамп - исчез
	
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

		UO->DrawGump(0x0866, 0, posX, posY); //Trade Gump

		//Наш чекбокс
		WORD gumpID = 0x0867; //Trade window checkbox unselected
		if (CanSelectedButton == ID_GST_CHECKBOX)
		{
			if (m_StateMy)
				gumpID = 0x086A; //Trade window checkbox selected (lighted)
			else
				gumpID = 0x0868; //Trade window checkbox unselected (lighted)
		}
		else if (m_StateMy)
			gumpID = 0x0869; //Trade window checkbox selected

		UO->DrawGump(gumpID, 0, posX + 52, posY + 29);
		
		FontManager->DrawA(1, g_Player->GetName().c_str(), 0x0386, posX + 84, posY + 40);

		//Чекбокс опонента
		gumpID = 0x0867; //Trade window checkbox unselected
		if (m_StateOpponent)
			gumpID = 0x0869; //Trade window checkbox selected

		UO->DrawGump(gumpID, 0, posX + 266, posY + 160);
		
		int fontWidth = 260 - FontManager->GetWidthA(1, m_Text.c_str(), m_Text.length());
		FontManager->DrawA(1, m_Text.c_str(), 0x0386, posX + fontWidth, posY + 170);
		
		DWORD ticks = GetTickCount();

		ColorizerShader->Use();

		//Отрисовка нашего товара (при наличии товара)
		TGameObject *container = World->FindWorldObject(ID);
		if (container != NULL && container->m_Items != NULL)
		{
			g_GL.ViewPort(posX + 45, posY + 70, 110, 80);

			posX += 45;
			posY += 70;

			TGameItem *item = (TGameItem*)container->m_Items;

			while (item != NULL)
			{
				if (item->Count > 0)
				{
					WORD graphic = item->Graphic;
					WORD color = item->Color;
					bool doubleDraw = false;
					bool selMode = false;
					int drawX = posX + item->X;
					int drawY = posY + item->Y;

					if (CanSelectedButton == item->Serial)
					{
						color = 0x0035;
						selMode = true;
					}

					if (item->Count > 1)
					{
						if (graphic == 0x0EED)
						{
							if (item->Count > 5)
								graphic = 0x0EEF;
							else
								graphic = 0x0EEE;
						}
						else if (item->IsStackable())
							doubleDraw = true;
					}

					UO->DrawStaticArtInContainer(graphic, color, drawX, drawY, selMode);
					if (doubleDraw)
						UO->DrawStaticArtInContainer(graphic, color, drawX + 5, drawY + 5, selMode);
				}

				item = (TGameItem*)item->m_Next;
			}
		
			//Восстанавливаем размеры рисуемой области
			g_GL.RestorePort();
			
			posX -= 45;
			posY -= 70;
		}
		


		//Отрисовка нашего опонента (при наличии товара)
		container = World->FindWorldObject(ID2);
		if (container != NULL && container->m_Items != NULL)
		{
			g_GL.ViewPort(posX + 192, posY + 70, 110, 80);

			posX += 192;
			posY += 70;

			TGameItem *item = (TGameItem*)container->m_Items;

			while (item != NULL)
			{
				WORD graphic = item->Graphic;
				WORD color = item->Color;
				bool doubleDraw = false;
				bool selMode = false;
				int drawX = posX + item->X;
				int drawY = posY + item->Y;

				if (CanSelectedButton == item->Serial)
				{
					color = 0x0035;
					selMode = true;
				}

				if (item->Count > 0)
				{
					if (item->Count > 1)
					{
						if (graphic == 0x0EED)
						{
							if (item->Count > 5)
								graphic = 0x0EEF;
							else
								graphic = 0x0EEE;
						}
						else if (item->IsStackable())
							doubleDraw = true;
					}

					UO->DrawStaticArtInContainer(graphic, color, drawX, drawY, selMode);

					if (doubleDraw)
						UO->DrawStaticArtInContainer(graphic, color, drawX + 5, drawY + 5, selMode);
				}

				item = (TGameItem*)item->m_Next;
			}

			//Восстанавливаем размеры рисуемой области
			g_GL.RestorePort();
			
			posX -= 192;
			posY -= 70;
		}

		UnuseShader();

	glEndList();

	FrameRedraw = true;
	FrameCreated = true;
}
//----------------------------------------------------------------------------
int TGumpSecureTrading::Draw(bool &mode)
{
	TGameObject *selobj = World->FindWorldObject(Serial);
	if (selobj == NULL)
		return 0; //Объект, к которому привязан гамп - исчез
	
	DWORD index = (DWORD)this;

	//Для быстрого доступа
	int posX = X;
	int posY = Y;

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

	//Если окошко захвачено для перемещения - вычислим оффсеты
	if (g_LeftMouseDown && g_LastGumpLeftMouseDown == index && !g_LastObjectLeftMouseDown)
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
	}
	
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

	if (mode) //Отрисовка
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
	}
	else //Выбор объектов
	{
		int LSG = 0;

		//Если выбран основной гамп - меняем глобальный указатель на выбранный гамп на него
		if (UO->GumpPixelsInXY(0x0866, posX, posY))
		{
			g_LastSelectedObject = 0;
			g_LastSelectedGump = index;
		}
		
		if (UO->GumpPixelsInXY(0x0867, posX + 52, posY + 29))
			LSG = ID_GST_CHECKBOX;

		RECT rc = {0, 0, 110, 80};
		POINT p = {g_MouseX - (posX + 45), g_MouseY - (posY + 70)};
		//if (PtInRect(&rc, p)) LSG = 2;
			
		//Проверка нашего товара (при наличии товара)
		TGameObject *container = World->FindWorldObject(ID);

		if (container != NULL && container->m_Items != NULL && PtInRect(&rc, p))
		{
			posX += 45;
			posY += 70;

			TGameItem *item = (TGameItem*)container->m_Items;
			while (item != NULL)
			{
				if (item->Count > 0)
				{
					WORD graphic = item->Graphic;
					bool doubleDraw = false;
					int drawX = posX + item->X;
					int drawY = posY + item->Y;

					if (item->Count > 1)
					{
						if (item->Graphic == 0x0EED)
						{
							if (item->Count > 5)
								graphic = 0x0EEF;
							else
								graphic = 0x0EEE;
						}
						else if (item->IsStackable())
							doubleDraw = true;
					}
					
					if (UO->StaticPixelsInXYInContainer(graphic, drawX, drawY))
						LSG = item->Serial;
					else if (doubleDraw)
					{
						if (UO->StaticPixelsInXYInContainer(graphic, drawX + 5, drawY + 5))
							LSG = item->Serial;
					}
				}

				item = (TGameItem*)item->m_Next;
			}
			
			posX -= 45;
			posY -= 70;
		}
		
		p.x = g_MouseX - (posX + 192);

		container = World->FindWorldObject(ID2);

		if (container != NULL && container->m_Items != NULL && PtInRect(&rc, p))
		{
			posX += 192;
			posY += 70;

			TGameItem *item = (TGameItem*)container->m_Items;

			while (item != NULL)
			{
				if (item->Count > 0)
				{
					WORD graphic = item->Graphic;
					bool doubleDraw = false;
					int drawX = posX + item->X;
					int drawY = posY + item->Y;

					if (item->Count > 1)
					{
						if (item->Graphic == 0x0EED)
						{
							if (item->Count > 5)
								graphic = 0x0EEF;
							else
								graphic = 0x0EEE;
						}
						else if (item->IsStackable())
							doubleDraw = true;
					}
					
					if (UO->StaticPixelsInXYInContainer(graphic, drawX, drawY))
						LSG = item->Serial;
					else if (doubleDraw)
					{
						if (UO->StaticPixelsInXYInContainer(graphic, drawX + 5, drawY + 5))
							LSG = item->Serial;
					}
				}

				item = (TGameItem*)item->m_Next;
			}
			
			posX -= 192;
			posY -= 70;
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
		}
		
		if (LSG != 0)
		{
			g_LastSelectedObject = LSG; //Если что-то нашлось - выбираем
			g_LastSelectedGump = index;
		}

		return LSG;
	}

	return 0;
}
//----------------------------------------------------------------------------
void TGumpSecureTrading::OnLeftMouseUp()
{
	if (ObjectInHand != NULL)
	{
		int x = X;
		int y = Y;

		RECT rc = {0, 0, 110, 80};
		POINT p = {g_MouseX - (x + 45), g_MouseY - (y + 70)};

		if (PtInRect(&rc, p))
		{
			x = g_MouseX - x - 45;
			y = g_MouseY - y - 70;

			WORD graphic = ObjectInHand->Graphic;

			if (graphic == 0x0EED)
			{
				if (ObjectInHand->Count > 5)
					graphic = 0x0EEF;
				else if (ObjectInHand->Count > 1)
					graphic = 0x0EEE;
			}
			
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

			UO->DropItem(ID, x, y, 0);
		}
	}

	if (g_LastObjectLeftMouseDown != g_LastSelectedObject)
		return; //Нажатый объект - не последний выбранный под курсором...валим
	
	if (g_LastObjectLeftMouseDown == ID_GST_CHECKBOX) //Изменение состояния чекбокса
	{
		m_StateMy = !m_StateMy;

		SendTradingResponse(2);

		GenerateFrame(X, Y);
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
			g_ClickObject.X = g_MouseX - X;
			g_ClickObject.Y = g_MouseY - Y;
		}
	}
}
//----------------------------------------------------------------------------