/****************************************************************************
**
** GenericGump.cpp
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
TGumpGeneric::TGumpGeneric(DWORD serial, short x, short y, DWORD id)
: TGump(GT_GENERIC, serial, x, y), m_Page(1), m_LastScrollChangeTime(0),
m_Transparent(false)
{
	ID = id;
}
//---------------------------------------------------------------------------
TGumpGeneric::~TGumpGeneric()
{
}
//---------------------------------------------------------------------------
void TGumpGeneric::ApplyTransparent(TGumpObject *obj, int page, int &x, int &y)
{
	m_Transparent = false;

	glClear(GL_STENCIL_BUFFER_BIT);
	glEnable(GL_STENCIL_TEST);

	while (obj != NULL)
	{
		if (obj->Type == GOT_PAGE)
		{
			page = ((TGumpPage*)obj)->Page;

			if (page >= 2 && page > m_Page)
				break;
		}
		else if (page == m_Page || !page)
		{
			switch (obj->Type)
			{
				case GOT_CHECKTRANS:
				{
					obj->Draw(x, y, m_Transparent, false);

					m_Transparent = true;

					break;
				}
				case GOT_PAGE:
				{
					page = ((TGumpPage*)obj)->Page;
					break;
				}
				default:
					break;
			}
		}

		obj = (TGumpObject*)obj->m_Next;
	}

	glDisable(GL_STENCIL_TEST);
}
//---------------------------------------------------------------------------
void TGumpGeneric::AddText(int index, wstring text)
{
	TGumpObject *item = (TGumpObject*)m_Items;

	while (item != NULL)
	{
		switch (item->Type)
		{
			case GOT_TEXT:
			case GOT_CROPPEDTEXT:
			{
				TGumpText *gt = (TGumpText*)item;

				if (gt->TextIndex == index)
				{
					int width = 0;
					WORD flags = UOFONT_BLACK_BORDER;

					if (item->Type == GOT_CROPPEDTEXT)
					{
						width = ((TGumpCroppedText*)gt)->Width;
						flags |= UOFONT_CROPPED;
					}
					
					WORD color = gt->Color;
					if (color)
						color++;

					FontManager->GenerateW((BYTE)(ConnectionManager.ClientVersion > CV_OLD), gt->m_Text, text.c_str(), color, 30, width, TS_LEFT, flags);
				}

				break;
			}
			case GOT_TEXTENTRYLIMITED:
			case GOT_TEXTENTRY:
			{
				TGumpTextEntry *gte = (TGumpTextEntry*)item;

				if (gte->TextIndex == index)
				{
					if (gte->TextEntry != NULL)
						gte->TextEntry->SetText(text);
					
					if (item->Type == GOT_TEXTENTRYLIMITED)
					{
						int maxLength = ((TGumpTextEntryLimited*)gte)->MaxLength;
						gte->TextEntry->SetMaxLength(maxLength);
					}
				}

				break;
			}
			case GOT_HTMLGUMP:
			{
				TGumpHTMLGump *ghtml = (TGumpHTMLGump*)item;

				if (ghtml->TextIndex == index)
				{
					int w = ghtml->Width;
					if (ghtml->HaveScrollbar)
						w -= 16;

					if (w > 10)
					{
						WORD color = 0;
						if (!ghtml->HaveBackground)
						{
							color = 0xFFFF;

							if (!ghtml->HaveScrollbar)
								FontManager->SetUseHTML(true, 0xFF010101);
							else
								FontManager->SetUseHTML(true);
						}
						else
						{
							w -= 9;
							FontManager->SetUseHTML(true, 0xFF010101);
						}

						ghtml->SetHTMLText(text);
						FontManager->GenerateW(1, ghtml->m_Text, text.c_str(), color, 30, w, TS_LEFT);

						FontManager->SetUseHTML(false);
					}
				}

				break;
			}
			default:
				break;
		}

		item = (TGumpObject*)item->m_Next;
	}
}
//---------------------------------------------------------------------------
bool TGumpGeneric::EntryPointerHere()
{
	bool result = false;
	TGumpObject *item = (TGumpObject*)m_Items;

	while (item != NULL)
	{
		if (item->Type == GOT_TEXTENTRY || item->Type == GOT_TEXTENTRYLIMITED)
		{
			TGumpTextEntry *gte = (TGumpTextEntry*)item;

			if (gte->TextEntry == EntryPointer)
			{
				result = true;

				break;
			}
		}

		item = (TGumpObject*)item->m_Next;
	}

	return result;
}
//---------------------------------------------------------------------------
void TGumpGeneric::PrepareTextures()
{
	TGumpObject *item = (TGumpObject*)m_Items;

	while (item != NULL)
	{
		switch (item->Type)
		{
			case GOT_RESIZEPIC:
			{
				UO->ExecuteResizepic(item->Graphic);

				break;
			}
			case GOT_BUTTON:
			case GOT_BUTTONTILEART:
			{
				TGumpButton *btn = (TGumpButton*)item;

				UO->ExecuteGump(btn->Graphic);
				UO->ExecuteGump(btn->GraphicPressed);
				UO->ExecuteGump(btn->GraphicLighted);

				if (item->Type == GOT_BUTTONTILEART)
				{
					TGumpButtonTileArt *bta = (TGumpButtonTileArt*)item;

					UO->ExecuteStaticArt(bta->TileGraphic);
				}
						
				break;
			}
			case GOT_RADIO:
			case GOT_CHECKBOX:
			{
				UO->ExecuteGump(item->Graphic);

				if (item->Type == GOT_CHECKBOX)
				{
					TGumpCheckbox *cb = (TGumpCheckbox*)item;
					UO->ExecuteGump(cb->GraphicChecked);
					UO->ExecuteGump(cb->GraphicDisabled);

				}
				else
				{
					TGumpRadio *rb = (TGumpRadio*)item;
					UO->ExecuteGump(rb->GraphicChecked);
					UO->ExecuteGump(rb->GraphicDisabled);
				}

				break;
			}
			case GOT_GUMPPIC:
			case GOT_GUMPPICTILED:
			{
				UO->ExecuteGump(item->Graphic);

				break;
			}
			case GOT_TILEPIC:
			{
				UO->ExecuteStaticArt(item->Graphic);

				break;
			}
			case GOT_HTMLGUMP:
			case GOT_XFMHTMLGUMP:
			case GOT_XFMHTMLTOKEN:
			{
				UO->ExecuteResizepic(0x0BB8);

				UO->ExecuteGump(0x00FF); //^ background
				UO->ExecuteGump(0x0100); //background
				UO->ExecuteGump(0x00FA); //^
				UO->ExecuteGump(0x00FE); //bar
				UO->ExecuteGump(0x00FC); //v

				break;
			}
			default:
				break;
		}

		item = (TGumpObject*)item->m_Next;
	}
}
//---------------------------------------------------------------------------
void TGumpGeneric::GenerateFrame(int posX, int posY)
{
	if (!g_DrawMode)
	{
		FrameRedraw = false;
		FrameCreated = false;

		return;
	}

	DWORD index = (DWORD)this;

	//Нажата ли кнопка в окне?
	bool IsScrollerPressed = (g_LeftMouseDown && g_LastGumpLeftMouseDown == index);

	//Нажата ли кнопка в окне?
	bool IsPressed = (g_LeftMouseDown && g_LastGumpLeftMouseDown == index && g_LastSelectedGump == index);

	//Может ли быть нажат элемент?
	int CanPressedButton = 0;
	if (IsPressed && g_LastObjectLeftMouseDown == g_LastSelectedObject)
		CanPressedButton = g_LastObjectLeftMouseDown;

	glNewList((GLuint)index, GL_COMPILE);

		int currentPage = 0;
		int currentGroup = 0;
		TGumpObject *item = (TGumpObject*)m_Items;
		int objectIndex = 1;
		bool haveHTML = false;
		float alpha[2] = { 1.0f, 0.5f };

		ApplyTransparent(item, m_Page, posX, posY);
		
		while (item != NULL)
		{
			if (item->Type == GOT_PAGE)
			{
				currentPage = ((TGumpPage*)item)->Page;

				if (currentPage >= 2 && currentPage > m_Page)
					break;
			}
			else if (currentPage == m_Page || !currentPage)
			{
				switch (item->Type)
				{
					case GOT_BUTTON:
					case GOT_BUTTONTILEART:
					case GOT_CHECKBOX:
					case GOT_RADIO:
					{
						item->Draw(posX, posY, m_Transparent, CanPressedButton == objectIndex);
						break;
					}
					case GOT_GUMPPIC:
					case GOT_TILEPIC:
					{
						ColorizerShader->Use();
						
						item->Draw(posX, posY, m_Transparent, false);
						
						UnuseShader();

						break;
					}
					case GOT_GUMPPICTILED:
					case GOT_RESIZEPIC:
					case GOT_TEXT:
					case GOT_CROPPEDTEXT:
					case GOT_TEXTENTRYLIMITED:
					case GOT_TEXTENTRY:
					{
						item->Draw(posX, posY, m_Transparent, false);
						break;
					}
					case GOT_HTMLGUMP:
					case GOT_XFMHTMLGUMP:
					case GOT_XFMHTMLTOKEN:
					{
						haveHTML = true;
						break;
					}
					case GOT_CHECKTRANS:
					{
						ApplyTransparent(((TGumpObject*)(item->m_Next)), m_Page, posX, posY);
						
						glColor4f(1.0f, 1.0f, 1.0f, alpha[m_Transparent]);

						break;
					}
					case GOT_PAGE:
					{
						currentPage = ((TGumpPage*)item)->Page;
						break;
					}
					case GOT_GROUP:
					{
						currentGroup = ((TGumpGroup*)item)->Group;
						break;
					}
					case GOT_ENDGROUP:
					{
						currentGroup = 0;
						break;
					}
					default:
						break;
				}
			}

			item = (TGumpObject*)item->m_Next;
			objectIndex++;
		}

		glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

		if (haveHTML)
		{
			currentPage = 0;
			item = (TGumpObject*)m_Items;
			objectIndex = 1;

			while (item != NULL)
			{
				if (item->Type == GOT_PAGE)
				{
					currentPage = ((TGumpPage*)item)->Page;

					if (currentPage >= 2 && currentPage > m_Page)
						break;
				}
				else if (currentPage == m_Page || !currentPage)
				{
					switch (item->Type)
					{
						case GOT_HTMLGUMP:
						case GOT_XFMHTMLGUMP:
						case GOT_XFMHTMLTOKEN:
						{
							TGumpHTMLGump *htmlGump = (TGumpHTMLGump*)item;

							int wOffs = htmlGump->Width;
							int textOffsX = 3;
							int textOffsY = 3;

							if (htmlGump->HaveScrollbar)
							{
								int drawX = posX + htmlGump->X;
								int drawY = posY + htmlGump->Y;

								//Для вычисления задержек между скроллингом
								DWORD ticks = GetTickCount();

								//Проверим, вдруг необходимо изменить положение отображаемого элемента
								if (IsPressed && m_LastScrollChangeTime < ticks)
								{
									if (g_LastObjectLeftMouseDown == ID_GG_SCROLLBAR_BUTTON_UP + objectIndex) //Скроллинг вверх (гампом-стрелкой)
										ListingList(htmlGump, true, 10);
									if (g_LastObjectLeftMouseDown == ID_GG_SCROLLBAR_BUTTON_DOWN + objectIndex) //Скроллинг вниз (гампом-стрелкой)
										ListingList(htmlGump, false, 10);
								}

								int scrollerY = 0;

								int heightToScrolling = 0;
							
								bool canMoveScroller = true;

								int curHeight = htmlGump->m_Text.Height;
								
								if (htmlGump->HaveBackground)
								{
									UO->DrawResizepicGump(0x0BB8, drawX, drawY, htmlGump->Width - 15, htmlGump->Height);
									curHeight += 7;
								}
								else
								{
									textOffsX = -1;
									textOffsY = -1;
								}
							
								if (curHeight < htmlGump->Height)
									canMoveScroller = false;
								else
									heightToScrolling = curHeight - htmlGump->Height;
	
								int maxScrollerY = htmlGump->Height - (25 + 21 + 21);

								int visibleLines = heightToScrolling / GUMP_SCROLLING_PIXEL_STEP;
								int cLine = htmlGump->LineOffset;

								if (IsScrollerPressed && g_LastObjectLeftMouseDown == ID_GG_SCROLLBAR + objectIndex && canMoveScroller) //Scroller pressed
								{
									int currentY = (g_MouseY - 25) - drawY; //Scroller position

									scrollerY = CalculateScrollerAndTextPosition(cLine, visibleLines, maxScrollerY, currentY);

									htmlGump->LineOffset = cLine;
								}
								else if (htmlGump->LineOffset && canMoveScroller)
									scrollerY = CalculateScrollerY(cLine, visibleLines, maxScrollerY);

								htmlGump->BarOffset = scrollerY;

								drawX += (htmlGump->Width - 15);
								UO->DrawGump(0x0100, 0, drawX, drawY + 21, 0, htmlGump->Height - 28); //background
								UO->DrawGump(0x00FA, 0, drawX, drawY); //^
								UO->DrawGump(0x00FE, 0, drawX + 1, drawY + 21 + scrollerY); //bar
								UO->DrawGump(0x00FC, 0, drawX, drawY + htmlGump->Height - 21); //v

								wOffs -= 14;
							}
							else if (htmlGump->HaveBackground)
								UO->DrawResizepicGump(0x0BB8, posX + htmlGump->X, posY + htmlGump->Y, htmlGump->Width, htmlGump->Height);
							else
							{
								textOffsX = -1;
								textOffsY = -1;
							}

							int drawX = posX + htmlGump->X + textOffsX;
							int drawY = posY + htmlGump->Y + textOffsY;
						
							int textHeight = htmlGump->Height;
							if (htmlGump->HaveBackground)
								textHeight -= 4;

							g_GL.ViewPort(drawX, drawY, wOffs, textHeight);

							htmlGump->m_Text.Draw(drawX, drawY - (htmlGump->LineOffset * GUMP_SCROLLING_PIXEL_STEP));

							g_GL.RestorePort();
						
							break;
						}
						case GOT_PAGE:
						{
							currentPage = ((TGumpPage*)item)->Page;
							break;
						}
						default:
							break;
					}
				}

				item = (TGumpObject*)item->m_Next;
				objectIndex++;
			}
		}

	glEndList();

	FrameRedraw = true;
	FrameCreated = true;
}
//----------------------------------------------------------------------------
int TGumpGeneric::Draw(bool &mode)
{
	DWORD index = (DWORD)this;

	int posX = X;
	int posY = Y;

	//Нажата ли кнопка в окне?
	bool IsScrollerPressed = (g_LeftMouseDown && g_LastGumpLeftMouseDown == index);

	//Нажата ли кнопка в окне?
	bool IsPressed = (g_LeftMouseDown && g_LastGumpLeftMouseDown == index && g_LastSelectedGump == index);

	//Может ли быть нажат элемент?
	int CanPressedButton = 0;
	if (IsPressed && g_LastObjectLeftMouseDown == g_LastSelectedObject)
		CanPressedButton = g_LastObjectLeftMouseDown;

	if (g_LeftMouseDown && g_LastGumpLeftMouseDown == index && (!g_LastObjectLeftMouseDown || g_LastObjectLeftMouseDown >= ID_GG_HTML_TEXT) && !NoMove)
	{
		posX += (g_MouseX - g_DroppedLeftMouseX);
		posY += (g_MouseY - g_DroppedLeftMouseY);

		if (mode)
			GenerateFrame(posX, posY);
	}

	if (mode) //Отрисовка
	{
		if (!FrameCreated)
			GenerateFrame(posX, posY);

		glCallList((GLuint)index);
	}
	else //Выбор объектов
	{
		int currentPage = 0;
		int currentGroup = 0;
		TGumpObject *item = (TGumpObject*)m_Items;
		int objectIndex = 1;
		int LSG = 0;
		bool haveHTML = false;

		while (item != NULL)
		{
			if (item->Type == GOT_PAGE)
			{
				currentPage = ((TGumpPage*)item)->Page;

				if (currentPage >= 2 && currentPage > m_Page)
					break;
			}
			else if (currentPage == m_Page || !currentPage)
			{
				switch (item->Type)
				{
					case GOT_RESIZEPIC:
					{
						if (UO->ResizepicPixelsInXY(item->Graphic, posX + item->X, posY + item->Y, ((TGumpResizepic*)item)->Width, ((TGumpResizepic*)item)->Height))
						{
							g_LastSelectedObject = 0;
							g_LastSelectedGump = index;
						}
						break;
					}
					case GOT_GUMPPIC:
					{
						if (UO->GumpPixelsInXY(item->Graphic, posX + item->X, posY + item->Y))
						{
							g_LastSelectedObject = 0;
							g_LastSelectedGump = index;
						}
						break;
					}
					case GOT_GUMPPICTILED:
					{
						if (UO->GumpPixelsInXY(item->Graphic, posX + item->X, posY + item->Y, ((TGumpGumppicTiled*)item)->Width, ((TGumpGumppicTiled*)item)->Height))
						{
							g_LastSelectedObject = 0;
							g_LastSelectedGump = index;
						}
						break;
					}
					case GOT_TILEPIC:
					{
						if (UO->StaticPixelsInXYInContainer(item->Graphic, posX + item->X, posY + item->Y))
						{
							g_LastSelectedObject = 0;
							g_LastSelectedGump = index;
						}
						break;
					}
					case GOT_BUTTONTILEART:
					{
						if (UO->GumpPixelsInXY(item->Graphic, posX + item->X, posY + item->Y))
							LSG = objectIndex;

						TGumpButtonTileArt *bta = (TGumpButtonTileArt*)item;
						if (UO->StaticPixelsInXYInContainer(bta->TileGraphic, posX + bta->TileX, posY + bta->TileY))
						{
							g_LastSelectedObject = 0;
							g_LastSelectedGump = index;
						}
						break;
					}
					case GOT_BUTTON:
					case GOT_CHECKBOX:
					case GOT_RADIO:
					{
						if (UO->GumpPixelsInXY(item->Graphic, posX + item->X, posY + item->Y))
							LSG = objectIndex;
						break;
					}
					case GOT_TEXTENTRYLIMITED:
					case GOT_TEXTENTRY:
					{
						TGumpTextEntry *gte = (TGumpTextEntry*)item;

						RECT rc = {0, 0, gte->Width, gte->Height};
						POINT p = {g_MouseX - (posX + item->X), g_MouseY - (posY + item->Y)};
						if (PtInRect(&rc, p))
							LSG = objectIndex;
						break;
					}
					case GOT_HTMLGUMP:
					case GOT_XFMHTMLGUMP:
					case GOT_XFMHTMLTOKEN:
					{
						haveHTML = true;
						break;
					}
					case GOT_PAGE:
					{
						currentPage = ((TGumpPage*)item)->Page;
						break;
					}
					case GOT_GROUP:
					{
						currentGroup = ((TGumpGroup*)item)->Group;
						break;
					}
					case GOT_ENDGROUP:
					{
						currentGroup = 0;
						break;
					}
					default:
						break;
				}
			}

			item = (TGumpObject*)item->m_Next;
			objectIndex++;
		}

		if (haveHTML)
		{
			currentPage = 0;
			item = (TGumpObject*)m_Items;
			objectIndex = 1;

			while (item != NULL)
			{
				if (item->Type == GOT_PAGE)
				{
					currentPage = ((TGumpPage*)item)->Page;

					if (currentPage >= 2 && currentPage > m_Page)
						break;
				}
				else if (currentPage == m_Page || !currentPage)
				{
					switch (item->Type)
					{
						case GOT_HTMLGUMP:
						case GOT_XFMHTMLGUMP:
						case GOT_XFMHTMLTOKEN:
						{
							TGumpHTMLGump *htmlGump = (TGumpHTMLGump*)item;

							int wOffs = htmlGump->Width;
							int textOffsX = 3;
							int textOffsY = 3;

							if (htmlGump->HaveScrollbar)
							{
								int drawX = posX + htmlGump->X;
								int drawY = posY + htmlGump->Y;
							
								if (htmlGump->HaveBackground && UO->ResizepicPixelsInXY(0x0BB8, drawX, drawY, htmlGump->Width - 15, htmlGump->Height))
								{
									g_LastSelectedObject = 0;
									LSG = ID_GG_HTML_TEXT + objectIndex;
									g_LastSelectedGump = index;
								}

								drawX += (htmlGump->Width - 15);
							
								if (UO->GumpPixelsInXY(0x00FA, drawX, drawY))
									LSG = ID_GG_SCROLLBAR_BUTTON_UP + objectIndex; //^
								else if (UO->GumpPixelsInXY(0x00FC, drawX, drawY + htmlGump->Height - 21))
									LSG = ID_GG_SCROLLBAR_BUTTON_DOWN + objectIndex; //v
								else if (UO->GumpPixelsInXY(0x00FE, drawX + 1, drawY + 21 + htmlGump->BarOffset))
									LSG = ID_GG_SCROLLBAR + objectIndex; //bar
								else if (UO->GumpPixelsInXY(0x0100, drawX, drawY + 21, 0, htmlGump->Height - 28))
									LSG = ID_GG_SCROLLBAR_BACKGROUND + objectIndex; //background

								wOffs -= 14;
								textOffsX = -1;
								textOffsY = -1;
							}
							else if (htmlGump->HaveBackground && UO->ResizepicPixelsInXY(0x0BB8, posX + htmlGump->X, posY + htmlGump->Y, htmlGump->Width, htmlGump->Height))
							{
								g_LastSelectedObject = 0;
								LSG = ID_GG_HTML_TEXT + objectIndex;
								g_LastSelectedGump = index;
							}
						
							RECT rc = {0, 0, wOffs, htmlGump->Height};
							POINT p = {g_MouseX - (posX + item->X + textOffsX), g_MouseY - (posY + item->Y + textOffsY)};
							if (PtInRect(&rc, p))
							{
								g_LastSelectedObject = 0;
								LSG = ID_GG_HTML_TEXT + objectIndex;
								g_LastSelectedGump = index;
							}

							break;
						}
						case GOT_PAGE:
						{
							currentPage = ((TGumpPage*)item)->Page;
							break;
						}
						default:
							break;
					}
				}

				item = (TGumpObject*)item->m_Next;
				objectIndex++;
			}
		}

		if (LSG != 0)
			g_LastSelectedObject = LSG;

		return LSG;
	}

	return 0;
}
//----------------------------------------------------------------------------
void TGumpGeneric::OnLeftMouseDown()
{
	if (g_LastObjectLeftMouseDown != g_LastSelectedObject)
		return;

	int currentPage = 0;
	TGumpObject *item = (TGumpObject*)m_Items;
	int objectIndex = 1;

	while (item != NULL)
	{
		if (item->Type == GOT_PAGE)
		{
			currentPage = ((TGumpPage*)item)->Page;

			if (currentPage >= 2 && currentPage > m_Page)
				break;
		}
		else if (currentPage == m_Page || !currentPage)
		{
			switch (item->Type)
			{
				case GOT_HTMLGUMP:
				case GOT_XFMHTMLGUMP:
				case GOT_XFMHTMLTOKEN:
				{
					TGumpHTMLGump *htmlGump = (TGumpHTMLGump*)item;
					
					if (htmlGump->HaveScrollbar)
					{
						if (g_LastObjectLeftMouseDown == ID_GG_SCROLLBAR_BACKGROUND + objectIndex)
						{
							int drawY = Y + htmlGump->Y + 21 + htmlGump->BarOffset;
						
							if (g_DroppedLeftMouseY < drawY) //^
							{
								g_LastObjectLeftMouseDown -= ID_GG_SCROLLBAR_BACKGROUND;
								g_LastObjectLeftMouseDown += ID_GG_SCROLLBAR_BUTTON_UP;
							}
							else if (g_DroppedLeftMouseY > drawY + 25) //v
							{
								g_LastObjectLeftMouseDown -= ID_GG_SCROLLBAR_BACKGROUND;
								g_LastObjectLeftMouseDown += ID_GG_SCROLLBAR_BUTTON_UP;
							}
						}
					}

					break;
				}
				case GOT_PAGE:
				{
					currentPage = ((TGumpPage*)item)->Page;
					break;
				}
				default:
					break;
			}
		}

		item = (TGumpObject*)item->m_Next;
		objectIndex++;
	}
}
//----------------------------------------------------------------------------
void TGumpGeneric::OnLeftMouseUp()
{
	if (g_LastObjectLeftMouseDown != g_LastSelectedObject)
		return;

	int currentPage = 0;
	int currentGroup = 0;
	TGumpObject *item = (TGumpObject*)m_Items;
	int objectIndex = 1;
	int LSG = 0;

	while (item != NULL)
	{
		if (item->Type == GOT_PAGE)
		{
			currentPage = ((TGumpPage*)item)->Page;

			if (currentPage >= 2 && currentPage > m_Page)
				break;
		}
		else if (currentPage == m_Page || !currentPage)
		{
			switch (item->Type)
			{
				case GOT_BUTTON:
				case GOT_BUTTONTILEART:
				{
					if (g_LastObjectLeftMouseDown == objectIndex)
					{
						TGumpButton *gb = (TGumpButton*)item;
						if (!gb->Action) //Change page
						{
							m_Page = gb->ToPage;
							if (m_Page < 1)
								m_Page = 1;

							UpdateFrame();
							return;
						}
						else //Click button
						{
							SendGumpResponse(((TGumpButton*)item)->Index);
							return;
						}
					}
					break;
				}
				case GOT_CHECKBOX:
				{
					if (g_LastObjectLeftMouseDown == objectIndex)
					{
						TGumpCheckbox *cb = (TGumpCheckbox*)item;
						cb->Action = !cb->Action;
						
						UpdateFrame();
						return;
					}
					break;
				}
				case GOT_RADIO:
				{
					if (g_LastObjectLeftMouseDown == objectIndex)
					{
						TGumpRadio *tb = (TGumpRadio*)item;

						TGumpRadio *radio = (TGumpRadio*)m_Items;
						int radioPage = 0;
						int radioGroup = 0;

						while (radio != NULL)
						{
							if (radio->Type == GOT_PAGE)
							{
								radioPage = ((TGumpPage*)radio)->Page;
							}
							else if (radio->Type == GOT_GROUP)
							{
								radioGroup = ((TGumpGroup*)radio)->Group;
							}
							else if (radio->Type == GOT_RADIO)
							{
								if (currentPage <= 1 && radioPage <= 1)
								{
									if (currentGroup == radioGroup)
									{
										radio->Action = false;
									}
								}
								else if (currentPage == radioPage)
								{
									if (currentGroup == radioGroup)
									{
										radio->Action = false;
									}
								}
							}

							radio = (TGumpRadio*)radio->m_Next;
						}

						tb->Action = true;
						UpdateFrame();

						return;
					}
					break;
				}
				case GOT_TEXTENTRYLIMITED:
				case GOT_TEXTENTRY:
				{
					TGumpTextEntry *gte = (TGumpTextEntry*)item;

					if (g_LastObjectLeftMouseDown == objectIndex && gte->TextEntry != NULL)
					{
						int x = g_MouseX - (X + item->X);
						int y = g_MouseY - (Y + item->Y);

						gte->TextEntry->OnClick(this, 1, false, x, y);
					}
					break;
				}
				case GOT_HTMLGUMP:
				case GOT_XFMHTMLGUMP:
				case GOT_XFMHTMLTOKEN:
				{
					TGumpHTMLGump *htmlGump = (TGumpHTMLGump*)item;
					int textOffsX = 3;
					int textOffsY = 3;
					
					if (htmlGump->HaveScrollbar)
					{
						textOffsX = -1;
						textOffsY = -1;

						/*if (g_LastObjectLeftMouseDown == ID_GG_SCROLLBAR_BACKGROUND + objectIndex)
						{
							int drawY = Y + htmlGump->Y + 21 + htmlGump->BarOffset;
						
							if (g_DroppedLeftMouseY < drawY) //^
							{
								g_LastObjectLeftMouseDown -= ID_GG_SCROLLBAR_BACKGROUND;
								g_LastObjectLeftMouseDown += ID_GG_SCROLLBAR_BUTTON_UP;
							}
							else if (g_DroppedLeftMouseY > drawY + 25) //v
							{
								g_LastObjectLeftMouseDown -= ID_GG_SCROLLBAR_BACKGROUND;
								g_LastObjectLeftMouseDown += ID_GG_SCROLLBAR_BUTTON_UP;
							}
						}*/
						
						DWORD ticks = GetTickCount();

						if (m_LastScrollChangeTime < ticks)
						{
							if (g_LastObjectLeftMouseDown == ID_GG_SCROLLBAR_BUTTON_UP + objectIndex)
								ListingList(htmlGump, true);
							else if (g_LastObjectLeftMouseDown == ID_GG_SCROLLBAR_BUTTON_DOWN + objectIndex)
								ListingList(htmlGump, false);
						}
					}

					if (g_LastObjectLeftMouseDown == ID_GG_HTML_TEXT + objectIndex)
					{
						int drawX = X + htmlGump->X + textOffsX;
						int drawY = Y + htmlGump->Y + textOffsY - (htmlGump->LineOffset * GUMP_SCROLLING_PIXEL_STEP);
						
						WORD link = htmlGump->m_Text.WebLinkUnderMouse(drawX, drawY);

						if (link && link != 0xFFFF)
						{
							FontManager->GoToWebLink(link);

							htmlGump->m_Text.Clear();
							htmlGump->m_Text.ClearWebLink();

							int w = htmlGump->Width;
							if (htmlGump->HaveScrollbar)
								w -= 15;

							if (w > 10)
							{
								if (!htmlGump->HaveBackground)
								{
									if (!htmlGump->HaveScrollbar)
										FontManager->SetUseHTML(true, 0xFF010101);
									else
										FontManager->SetUseHTML(true);
								}
								else
									FontManager->SetUseHTML(true, 0xFF010101);

								if (item->Type == GOT_HTMLGUMP)
								{
									WORD color = 0;
									if (!htmlGump->HaveBackground)
										color = 0xFFFF;

									FontManager->GenerateW(1, htmlGump->m_Text, htmlGump->GetHTMLText().c_str(), color, 30, w, TS_LEFT);
								}
								else
								{
									TCliloc *cliloc = ClilocManager->Cliloc(g_Language);
									wstring text = cliloc->GetW((DWORD)htmlGump->TextIndex);
		
									FontManager->GenerateW(1, htmlGump->m_Text, text.c_str(), htmlGump->Color, 30, w, TS_LEFT);
								}

								FontManager->SetUseHTML(false);
							}
						}
					}

					break;
				}
				case GOT_PAGE:
				{
					currentPage = ((TGumpPage*)item)->Page;
					break;
				}
				case GOT_GROUP:
				{
					currentGroup = ((TGumpGroup*)item)->Group;
					break;
				}
				case GOT_ENDGROUP:
				{
					currentGroup = 0;
					break;
				}
				default:
					break;
			}
		}

		item = (TGumpObject*)item->m_Next;
		objectIndex++;
	}
}
//----------------------------------------------------------------------------
void TGumpGeneric::OnMouseWheel(MOUSE_WHEEL_STATE &state)
{
	if (g_LastSelectedObject != 0 && !g_LeftMouseDown && !g_RightMouseDown)
	{
		DWORD ticks = GetTickCount();

		if (m_LastScrollChangeTime < ticks)
		{
			int currentPage = 0;
			TGumpObject *item = (TGumpObject*)m_Items;
			int objectIndex = 1;

			while (item != NULL)
			{
				if (item->Type == GOT_PAGE)
				{
					currentPage = ((TGumpPage*)item)->Page;

					if (currentPage >= 2 && currentPage > m_Page)
						break;
				}
				else if (currentPage == m_Page || !currentPage)
				{
					switch (item->Type)
					{
						case GOT_HTMLGUMP:
						case GOT_XFMHTMLGUMP:
						case GOT_XFMHTMLTOKEN:
						{
							if (g_LastSelectedObject == ID_GG_HTML_TEXT + objectIndex)
							{
								TGumpHTMLGump *htmlGump = (TGumpHTMLGump*)item;

								if (htmlGump->HaveScrollbar)
								{
									if (state == MWS_UP)
									{
										ListingList(htmlGump, true, 25);
										GenerateFrame(X, Y);
									}
									else if (state == MWS_DOWN)
									{
										ListingList(htmlGump, false, 25);
										GenerateFrame(X, Y);
									}
								}
							}

							break;
						}
						case GOT_PAGE:
						{
							currentPage = ((TGumpPage*)item)->Page;
							break;
						}
						default:
							break;
					}
				}

				item = (TGumpObject*)item->m_Next;
				objectIndex++;
			}
		}
	}
}
//----------------------------------------------------------------------------
void TGumpGeneric::OnCharPress(WPARAM &wparam, LPARAM &lparam)
{
	//Ввод текста
	if (wparam == VK_RETURN || wparam == VK_BACK || EntryPointer == NULL)
		return; //Ignore no print keys

	TGumpObject *item = (TGumpObject*)m_Items;

	for (; item != NULL; item = (TGumpObject*)item->m_Next)
	{
		if (item->Type == GOT_TEXTENTRY || item->Type == GOT_TEXTENTRYLIMITED)
			break;
	}

	if (item != NULL)
	{
		TGumpTextEntry *gte = (TGumpTextEntry*)item;

		EntryPointer->Insert(wparam);

		if (gte->Type == GOT_TEXTENTRYLIMITED)
		{
			int val = FontManager->GetWidthA(6, EntryPointer->c_str(), EntryPointer->Length());

			if (val > 170)
				EntryPointer->Remove(true);
			else
				GenerateFrame(m_X, m_Y);
		}
		else
			GenerateFrame(m_X, m_Y);
	}
}
//----------------------------------------------------------------------------
void TGumpGeneric::ListingList(TGumpHTMLGump *htmlGump, bool direction, int divizor)
{
	if (direction) //Up
	{
		if (htmlGump->LineOffset > 1)
			htmlGump->DecLineOffset();
		else
			htmlGump->LineOffset = 0;
	}
	else //Down
	{
		int maxidx = htmlGump->m_Text.Height;
		if (htmlGump->HaveBackground)
			maxidx += 7;

		maxidx = (maxidx - htmlGump->Height) / GUMP_SCROLLING_PIXEL_STEP;

		if (maxidx < 0)
			maxidx = 0;

		if (htmlGump->LineOffset < maxidx)
			htmlGump->IncLineOffset();
		else
			htmlGump->LineOffset = maxidx;
	}

	m_LastScrollChangeTime = GetTickCount() + (SCROLL_LISTING_DELAY / divizor);
}
//----------------------------------------------------------------------------