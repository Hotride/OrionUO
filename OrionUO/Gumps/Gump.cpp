// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
/***********************************************************************************
**
** Gump.h
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#include "stdafx.h"
//----------------------------------------------------------------------------------
CGump *g_ResizedGump = NULL;
CGump *g_CurrentCheckGump = NULL;
//----------------------------------------------------------------------------------
CGump::CGump()
: CGump(GT_NONE, 0, 0, 0)
{
}
//----------------------------------------------------------------------------------
CGump::CGump(GUMP_TYPE type, uint serial, int x, int y)
: CRenderObject(serial, 0, 0, x, y), m_GumpType(type)
{
}
//----------------------------------------------------------------------------------
CGump::~CGump()
{
	WISPFUN_DEBUG("c84_f1");
	//Если это гамп, блокирующий игровое окно
	if (m_Blocked)
	{
		//Уменьшаем счетчик блокирующих гампов
		g_GrayMenuCount--;

		//Если таких гампов больше нет - восстанавливаем игровой экран
		if (g_GrayMenuCount <= 0)
		{
			g_GrayMenuCount = 0;
			g_GameState = GS_GAME;
			g_CurrentScreen = &g_GameScreen;
		}
	}

	if (g_ClickObject.Gump == this)
		g_ClickObject.Clear();

	if (g_SelectedObject.Gump == this)
		g_SelectedObject.Clear();

	if (g_LastSelectedObject.Gump == this)
		g_LastSelectedObject.Clear();

	if (g_PressedObject.LeftGump == this)
		g_PressedObject.ClearLeft();
	if (g_PressedObject.RightGump == this)
		g_PressedObject.ClearRight();
	if (g_PressedObject.MidGump == this)
		g_PressedObject.ClearMid();
}
//---------------------------------------------------------------------------
void CGump::GUMP_DIRECT_HTML_LINK_EVENT_C
{
	g_FontManager.GoToWebLink(link);
	DebugMsg("OnDirectHTMLLink(%i)\n", link);
}
//---------------------------------------------------------------------------
void CGump::FixCoordinates()
{
	const int gumpOffsetX = 40;
	const int gumpOffsetY = 40;
	int maxX = g_OrionWindow.Size.Width - gumpOffsetX;
	int maxY = g_OrionWindow.Size.Height - gumpOffsetY;

	if (m_Minimized && m_GumpType != GT_MINIMAP)
	{
		if (m_MinimizedX + m_GumpRect.Position.X > maxX)
		{
			m_WantRedraw = true;
			m_MinimizedX = maxX;
		}

		if (m_MinimizedX < m_GumpRect.Position.X && m_MinimizedX + m_GumpRect.Position.X + m_GumpRect.Size.Width - gumpOffsetX < 0)
		{
			m_WantRedraw = true;
			m_MinimizedX = gumpOffsetX - (m_GumpRect.Position.X + m_GumpRect.Size.Width);
		}

		if (m_MinimizedY + m_GumpRect.Position.Y > maxY)
		{
			m_WantRedraw = true;
			m_MinimizedY = maxY;
		}

		if (m_MinimizedY < m_GumpRect.Position.Y && m_MinimizedY + m_GumpRect.Position.Y + m_GumpRect.Size.Height - gumpOffsetY < 0)
		{
			m_WantRedraw = true;
			m_MinimizedY = gumpOffsetY - (m_GumpRect.Position.Y + m_GumpRect.Size.Height);
		}
	}
	else
	{
		if (m_X + m_GumpRect.Position.X > maxX)
		{
			m_WantRedraw = true;
			m_X = maxX;
		}

		if (m_X < m_GumpRect.Position.X && m_X + m_GumpRect.Position.X + m_GumpRect.Size.Width - gumpOffsetX < 0)
		{
			m_WantRedraw = true;
			m_X = gumpOffsetX - (m_GumpRect.Position.X + m_GumpRect.Size.Width);
		}

		if (m_Y + m_GumpRect.Position.Y > maxY)
		{
			m_WantRedraw = true;
			m_Y = maxY;
		}

		if (m_Y < m_GumpRect.Position.Y && m_Y + m_GumpRect.Position.Y + m_GumpRect.Size.Height - gumpOffsetY < 0)
		{
			m_WantRedraw = true;
			m_Y = gumpOffsetY - (m_GumpRect.Position.Y + m_GumpRect.Size.Height);
		}
	}
}
//---------------------------------------------------------------------------
bool CGump::CanBeMoved()
{
	WISPFUN_DEBUG("c84_f2");
	bool result = true;

	if (m_NoMove)
		result = false;
	else if (g_ConfigManager.LockGumpsMoving)
		result = !m_LockMoving;

	return result;
}
//---------------------------------------------------------------------------
/*!
Отрисовать замочек гампа
@return
*/
void CGump::DrawLocker()
{
	WISPFUN_DEBUG("c84_f3");
	if (m_Locker.Serial && g_ShowGumpLocker)
		g_TextureGumpState[m_LockMoving].Draw(m_Locker.X, m_Locker.Y);
}
//---------------------------------------------------------------------------
bool CGump::SelectLocker()
{
	WISPFUN_DEBUG("c84_f4");
	return (m_Locker.Serial && g_ShowGumpLocker && g_Orion.PolygonePixelsInXY(m_Locker.X, m_Locker.Y, 10, 14));
}
//---------------------------------------------------------------------------
bool CGump::TestLockerClick()
{
	WISPFUN_DEBUG("c84_f5");
	bool result = (m_Locker.Serial && g_ShowGumpLocker && g_PressedObject.LeftObject == &m_Locker);

	if (result)
		OnButton(m_Locker.Serial);

	return result;
}
//---------------------------------------------------------------------------
void CGump::CalculateGumpState()
{
	WISPFUN_DEBUG("c84_f6");
	g_GumpPressed = (!g_ObjectInHand.Enabled && g_PressedObject.LeftGump == this /*&& g_SelectedObject.Gump() == this*/);
	g_GumpSelectedElement = ((g_SelectedObject.Gump == this) ? g_SelectedObject.Object : NULL);
	g_GumpPressedElement = NULL;

	CRenderObject *leftObj = g_PressedObject.LeftObject;

	if (g_GumpPressed && leftObj != NULL)
	{
		if (leftObj == g_SelectedObject.Object)
			g_GumpPressedElement = leftObj;
		else if (leftObj->IsGUI() && ((CBaseGUI*)leftObj)->IsPressedOuthit())
			g_GumpPressedElement = leftObj;
	}

	if (CanBeMoved() && g_GumpPressed && !g_ObjectInHand.Enabled && (!g_PressedObject.LeftSerial || g_GumpPressedElement == NULL || g_PressedObject.TestMoveOnDrag()))
		g_GumpMovingOffset = g_MouseManager.LeftDroppedOffset();
	else
		g_GumpMovingOffset.Reset();

	if (m_Minimized)
	{
		g_GumpTranslate.X = (float)(m_MinimizedX + g_GumpMovingOffset.X);
		g_GumpTranslate.Y = (float)(m_MinimizedY + g_GumpMovingOffset.Y);
	}
	else
	{
		g_GumpTranslate.X = (float)(m_X + g_GumpMovingOffset.X);
		g_GumpTranslate.Y = (float)(m_Y + g_GumpMovingOffset.Y);
	}
}
//---------------------------------------------------------------------------
void CGump::ProcessListing()
{
	WISPFUN_DEBUG("c84_f7");
	if (g_PressedObject.LeftGump != NULL && !g_PressedObject.LeftGump->NoProcess && g_PressedObject.LeftObject != NULL && g_PressedObject.LeftObject->IsGUI())
	{
		CBaseGUI *item = (CBaseGUI*)g_PressedObject.LeftObject;

		if (item->IsControlHTML())
		{
			if (item->Type == GOT_BUTTON)
			{
				((CGUIHTMLButton*)item)->Scroll(item->Color != 0, SCROLL_LISTING_DELAY / 7);
				g_PressedObject.LeftGump->WantRedraw = true;
				g_PressedObject.LeftGump->OnScrollButton();
			}
			else if (item->Type == GOT_HITBOX)
			{
				((CGUIHTMLHitBox*)item)->Scroll(item->Color != 0, SCROLL_LISTING_DELAY / 7);
				g_PressedObject.LeftGump->WantRedraw = true;
				g_PressedObject.LeftGump->OnScrollButton();
			}
		}
		else if (item->Type == GOT_BUTTON && ((CGUIButton*)item)->ProcessPressedState)
		{
			g_PressedObject.LeftGump->WantRedraw = true;
			g_PressedObject.LeftGump->OnButton(item->Serial);
		}
		else if (item->Type == GOT_MINMAXBUTTONS)
		{
			((CGUIMinMaxButtons*)item)->Scroll(SCROLL_LISTING_DELAY / 2);
			g_PressedObject.LeftGump->OnScrollButton();
			g_PressedObject.LeftGump->WantRedraw = true;
		}
		else if (item->Type == GOT_COMBOBOX)
		{
			CGUIComboBox *combo = (CGUIComboBox*)item;

			if (combo->ListingTimer < g_Ticks)
			{
				int index = combo->StartIndex;
				int direction = combo->ListingDirection;

				if (direction == 1 && index > 0)
					index--;
				else if (direction == 2 && index + 1 < combo->GetItemsCount())
					index++;

				if (index != combo->StartIndex)
				{
					if (index < 0)
						index = 0;

					combo->StartIndex = index;
					g_PressedObject.LeftGump->WantRedraw = true;
				}

				combo->ListingTimer = g_Ticks + 50;
			}
		}
	}
}
//---------------------------------------------------------------------------
bool CGump::ApplyTransparent(CBaseGUI *item, int page, const int &currentPage, const int draw2Page)
{
	WISPFUN_DEBUG("c84_f8");
	bool transparent = false;

	glClear(GL_STENCIL_BUFFER_BIT);
	glEnable(GL_STENCIL_TEST);

	bool canDraw = ((page == -1) || ((page >= currentPage && page <= currentPage + draw2Page || (!page && !draw2Page))));

	for (; item != NULL; item = (CBaseGUI*)item->m_Next)
	{
		if (item->Type == GOT_PAGE)
		{
			page = ((CGUIPage*)item)->Index;

			//if (page >= 2 && page > currentPage + draw2Page)
			//	break;

			canDraw = ((page == -1) || ((page >= currentPage && page <= currentPage + draw2Page || (!page && !draw2Page))));
		}
		else if (canDraw && item->Visible && item->Type == GOT_CHECKTRANS)
		{
			item->Draw(transparent);

			transparent = true;
		}
	}

	glDisable(GL_STENCIL_TEST);

	return transparent;
}
//----------------------------------------------------------------------------------
void CGump::DrawItems(CBaseGUI *start, const int &currentPage, const int draw2Page)
{
	WISPFUN_DEBUG("c84_f9");
	float alpha[2] = { 1.0f, 0.7f };
	CGUIComboBox *combo = NULL;

	bool transparent = ApplyTransparent(start, 0, currentPage, draw2Page);
	glColor4f(1.0f, 1.0f, 1.0f, alpha[transparent]);

	int page = 0;
	bool canDraw = ((page == -1) || ((page >= currentPage && page <= currentPage + draw2Page || (!page && !draw2Page))));

	QFOR(item, start, CBaseGUI*)
	{
		if (item->Type == GOT_PAGE)
		{
			page = ((CGUIPage*)item)->Index;

			//if (page >= 2 && page > currentPage + draw2Page)
			//	break;

			canDraw = ((page == -1) || ((page >= currentPage && page <= currentPage + draw2Page || (!page && !draw2Page))));
		}
		else if (canDraw && item->Visible && !item->SelectOnly)
		{
			switch (item->Type)
			{
				case GOT_DATABOX:
				{
					CGump::DrawItems((CBaseGUI*)item->m_Items, currentPage, draw2Page);
					break;
				}
				case GOT_HTMLGUMP:
				case GOT_XFMHTMLGUMP:
				case GOT_XFMHTMLTOKEN:
				{
					CGUIHTMLGump *htmlGump = (CGUIHTMLGump*)item;

					GLfloat x = (GLfloat)htmlGump->X;
					GLfloat y = (GLfloat)htmlGump->Y;

					glTranslatef(x, y, 0.0f);

					CBaseGUI *item = (CBaseGUI*)htmlGump->m_Items;

					IFOR(j, 0, 5)
					{
						if (item->Visible && !item->SelectOnly)
							item->Draw(false);

						item = (CBaseGUI*)item->m_Next;
					}

					GLfloat offsetX = (GLfloat)(htmlGump->DataOffset.X - htmlGump->CurrentOffset.X);
					GLfloat offsetY = (GLfloat)(htmlGump->DataOffset.Y - htmlGump->CurrentOffset.Y);

					glTranslatef(offsetX, offsetY, 0.0f);

					CGump::DrawItems(item, currentPage, draw2Page);
					g_GL.PopScissor();

					glTranslatef(-(x + offsetX), -(y + offsetY), 0.0f);

					break;
				}
				case GOT_CHECKTRANS:
				{
					transparent = ApplyTransparent((CBaseGUI*)item->m_Next, page/*m_Page*/, currentPage, draw2Page);
						
					glColor4f(1.0f, 1.0f, 1.0f, alpha[transparent]);

					break;
				}
				case GOT_COMBOBOX:
				{
					if (g_PressedObject.LeftObject == item)
					{
						combo = (CGUIComboBox*)item;
						break;
					}
				}
				default:
				{
					item->Draw(transparent);

					break;
				}
			}
		}
	}

	if (combo != NULL)
		combo->Draw(false);

	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
}
//----------------------------------------------------------------------------------
CRenderObject *CGump::SelectItems(CBaseGUI *start, const int &currentPage, const int draw2Page)
{
	WISPFUN_DEBUG("c84_f10");
	CRenderObject *selected = NULL;

	int page = 0;
	bool canDraw = ((page == -1) || ((page >= currentPage && page <= currentPage + draw2Page || (!page && !draw2Page))));
	vector<bool> scissorList;
	bool currentScissorState = true;
	CGUIComboBox *combo = NULL;

	WISP_GEOMETRY::CPoint2Di oldPos = g_MouseManager.Position;

	QFOR(item, start, CBaseGUI*)
	{
		if (item->Type == GOT_PAGE)
		{
			page = ((CGUIPage*)item)->Index;

			//if (page >= 2 && page > currentPage + draw2Page)
			//	break;

			canDraw = ((page == -1) || ((page >= currentPage && page <= currentPage + draw2Page || (!page && !draw2Page))));
		}
		else if (canDraw && item->Visible)
		{
			if (item->Type == GOT_SCISSOR)
			{
				if (item->Enabled)
				{
					currentScissorState = item->Select();
					scissorList.push_back(currentScissorState);
				}
				else
				{
					scissorList.pop_back();

					if (scissorList.size())
						currentScissorState = scissorList.back();
					else
						currentScissorState = true;
				}

				continue;
			}
			else if (!currentScissorState || !item->Enabled || (item->DrawOnly && selected != NULL) || !item->Select())
				continue;

			switch (item->Type)
			{
				case GOT_HTMLGUMP:
				case GOT_XFMHTMLGUMP:
				case GOT_XFMHTMLTOKEN:
				{
					CGUIHTMLGump *htmlGump = (CGUIHTMLGump*)item;

					g_MouseManager.Position = WISP_GEOMETRY::CPoint2Di(oldPos.X - htmlGump->X, oldPos.Y - htmlGump->Y);

					CBaseGUI *item = (CBaseGUI*)htmlGump->m_Items;

					CRenderObject *selectedHTML = NULL;

					IFOR(j, 0, 4)
					{
						if (item->Select())
							selectedHTML = item;

						item = (CBaseGUI*)item->m_Next;
					}

					//Scissor
					if (item->Select())
					{
						int offsetX = htmlGump->DataOffset.X - htmlGump->CurrentOffset.X;
						int offsetY = htmlGump->DataOffset.Y - htmlGump->CurrentOffset.Y;

						g_MouseManager.Position = WISP_GEOMETRY::CPoint2Di(g_MouseManager.Position.X - offsetX, g_MouseManager.Position.Y - offsetY);

						selected = CGump::SelectItems((CBaseGUI*)item->m_Next, currentPage, draw2Page);
					}
					else
						selected = NULL;

					if (selected == NULL)
					{
						selected = selectedHTML;

						if (selected == NULL)
							selected = item;
					}

					g_MouseManager.Position = oldPos;

					break;
				}
				case GOT_DATABOX:
				{
					CRenderObject *selectedBox = CGump::SelectItems((CBaseGUI*)item->m_Items, currentPage, draw2Page);

					if (selectedBox)
						selected = selectedBox;

					break;
				}
				case GOT_COMBOBOX:
				{
					//selected = ((CGUIComboBox*)item)->SelectedItem();

					if (g_PressedObject.LeftObject == item)
						combo = (CGUIComboBox*)item;
					else
						selected = item;

					break;
				}
				case GOT_SHOPRESULT:
				{
					selected = ((CGUIShopResult*)item)->SelectedItem();

					break;
				}
				case GOT_SKILLITEM:
				{
					selected = ((CGUISkillItem*)item)->SelectedItem();

					break;
				}
				case GOT_SKILLGROUP:
				{
					selected = ((CGUISkillGroup*)item)->SelectedItem();

					break;
				}
				default:
				{
					selected = item;

					break;
				}
			}
		}
	}

	if (combo != NULL)
		selected = combo->SelectedItem();

	return selected;
}
//----------------------------------------------------------------------------------
void CGump::TestItemsLeftMouseDown(CGump *gump, CBaseGUI *start, const int &currentPage, const int &draw2Page, int count)
{
	WISPFUN_DEBUG("c84_f11");
	int page = 0;
	int group = 0;
	bool canDraw = ((page == -1) || ((page >= currentPage && page <= currentPage + draw2Page || (!page && !draw2Page))));

	static bool htmlTextBackgroundCanBeColored = false;

	if (!(start != NULL && start->m_Next == NULL && start->Type == GOT_HTMLTEXT))
		htmlTextBackgroundCanBeColored = false;

	QFOR(item, start, CBaseGUI*)
	{
		if (!count)
			break;

		count--;

		if (item->Type == GOT_PAGE)
		{
			page = ((CGUIPage*)item)->Index;

			//if (page >= 2 && page > currentPage + draw2Page)
			//	break;

			canDraw = ((page == -1) || ((page >= currentPage && page <= currentPage + draw2Page || (!page && !draw2Page))));
		}
		else if (canDraw && item->Enabled && item->Visible)
		{
			if (item->Type == GOT_GROUP)
			{
				group = ((CGUIGroup*)item)->Index;
				continue;
			}
			else if (g_SelectedObject.Object != item && !item->IsHTMLGump())
			{
				if (item->Type == GOT_SHOPRESULT)
				{
					if (g_SelectedObject.Object == ((CGUIShopResult*)item)->m_MinMaxButtons)
					{
						WISP_GEOMETRY::CPoint2Di oldPos = g_MouseManager.Position;
						g_MouseManager.Position = WISP_GEOMETRY::CPoint2Di(g_MouseManager.Position.X - item->X, g_MouseManager.Position.Y - item->Y);

						((CGUIShopResult*)item)->m_MinMaxButtons->OnClick();

						g_MouseManager.Position = oldPos;
					}

					continue;
				}
				else if (item->Type != GOT_SKILLGROUP && item->Type != GOT_DATABOX /*&& item->Type != GOT_TEXTENTRY*/)
					continue;
			}

			switch (item->Type)
			{
				case GOT_HITBOX:
				{
					if (((CGUIPolygonal*)item)->CallOnMouseUp)
						break;

					CGUIHitBox *box = (CGUIHitBox*)item;

					if (box->ToPage != -1)
					{
						gump->Page = box->ToPage;

						//if (gump->Page < 1)
						//	gump->Page = 1;
					}
				}
				case GOT_COLOREDPOLYGONE:
				{
					if (((CGUIPolygonal*)item)->CallOnMouseUp)
						break;
				}
				case GOT_RESIZEPIC:
				{
					uint serial = item->Serial;

					if (!serial)
						break;

					int tempPage = -1;
					bool tempCanDraw = true;

					QFOR(testItem, start, CBaseGUI*)
					{
						if (testItem->Type == GOT_PAGE)
						{
							tempPage = ((CGUIPage*)testItem)->Index;

							tempCanDraw = ((tempPage == -1) || ((tempPage >= page && tempPage <= page + draw2Page || (!tempPage && !draw2Page))));
						}
						else if (tempCanDraw && testItem->Type == GOT_TEXTENTRY && testItem->Serial == serial && testItem->Enabled && testItem->Visible)
						{
							CGUITextEntry *entry = (CGUITextEntry*)testItem;

							if (!entry->ReadOnly)
							{
								int x = g_MouseManager.Position.X - item->X;
								int y = g_MouseManager.Position.Y - item->Y;

								entry->OnClick(gump, x, y);
							}

							break;
						}
					}

					gump->OnTextEntry(serial);
					gump->WantRedraw = true;

					return;
				}
				case GOT_SKILLGROUP:
				{
					CGUISkillGroup *skillGroup = (CGUISkillGroup*)item;

					if (g_SelectedObject.Object == skillGroup->m_Name)
					{
						gump->OnTextEntry(g_SelectedObject.Object->Serial);
						gump->WantRedraw = true;

						return;
					}

					break;
				}
				case GOT_RESIZEBUTTON:
				{
					gump->OnResizeStart(item->Serial);
					gump->WantRedraw = true;
					break;
				}
				case GOT_SHOPITEM:
				{
					((CGUIShopItem*)item)->OnClick();
					gump->WantRedraw = true;
					break;
				}
				case GOT_HTMLTEXT:
				{
					CGUIHTMLText *htmlText = (CGUIHTMLText*)item;

					ushort link = htmlText->m_Texture.WebLinkUnderMouse(item->X, item->Y);

					if (link && link != 0xFFFF)
					{
						gump->OnDirectHTMLLink(link);
						gump->WantRedraw = true;

						htmlText->m_Texture.ClearWebLink();
						htmlText->CreateTexture(htmlTextBackgroundCanBeColored);
					}

					break;
				}
				case GOT_DATABOX:
				{
					CGump::TestItemsLeftMouseDown(gump, (CBaseGUI*)item->m_Items, currentPage, draw2Page);
					break;
				}
				case GOT_BUTTON:
				case GOT_BUTTONTILEART:
				{
					gump->WantRedraw = true;
					break;
				}
				case GOT_TEXTENTRY:
				{
					CGUITextEntry *entry = (CGUITextEntry*)item;

					if (!entry->ReadOnly)
					{
						int x = g_MouseManager.Position.X - item->X;
						int y = g_MouseManager.Position.Y - item->Y;

						entry->OnClick(gump, x, y);
					}

					gump->OnTextEntry(item->Serial);
					gump->WantRedraw = true;

					return;
				}
				case GOT_SLIDER:
				{
					int x = g_MouseManager.Position.X - item->X;
					int y = g_MouseManager.Position.Y - item->Y;

					((CGUISlider*)item)->OnClick(x, y);

					gump->OnSliderClick(item->Serial);
					gump->WantRedraw = true;

					return;
				}
				case GOT_MINMAXBUTTONS:
				{
					((CGUIMinMaxButtons*)item)->OnClick();

					gump->WantRedraw = true;

					return;
				}
				case GOT_COMBOBOX:
				{
					gump->WantRedraw = true;

					return;
				}
				case GOT_HTMLGUMP:
				case GOT_XFMHTMLGUMP:
				case GOT_XFMHTMLTOKEN:
				{
					CGUIHTMLGump *htmlGump = (CGUIHTMLGump*)item;

					htmlTextBackgroundCanBeColored = !htmlGump->HaveBackground;

					WISP_GEOMETRY::CPoint2Di oldPos = g_MouseManager.Position;
					g_MouseManager.Position = WISP_GEOMETRY::CPoint2Di(oldPos.X - htmlGump->X, oldPos.Y - htmlGump->Y);

					CBaseGUI *item = (CBaseGUI*)htmlGump->m_Items;

					TestItemsLeftMouseDown(gump, item, currentPage, draw2Page, 5);

					IFOR(j, 0, 5)
						item = (CBaseGUI*)item->m_Next;

					int offsetX = htmlGump->DataOffset.X - htmlGump->CurrentOffset.X;
					int offsetY = htmlGump->DataOffset.Y - htmlGump->CurrentOffset.Y;

					g_MouseManager.Position = WISP_GEOMETRY::CPoint2Di(g_MouseManager.Position.X - offsetX, g_MouseManager.Position.Y - offsetY);

					TestItemsLeftMouseDown(gump, item, currentPage, draw2Page);

					g_MouseManager.Position = oldPos;

					break;
				}
				default:
					break;
			}
		}
	}
}
//----------------------------------------------------------------------------------
void CGump::TestItemsLeftMouseUp(CGump *gump, CBaseGUI *start, const int &currentPage, const int &draw2Page)
{
	WISPFUN_DEBUG("c84_f12");
	int page = 0;
	int group = 0;
	bool canDraw = ((page == -1) || ((page >= currentPage && page <= currentPage + draw2Page || (!page && !draw2Page))));

	QFOR(item, start, CBaseGUI*)
	{
		if (item->Type == GOT_PAGE)
		{
			page = ((CGUIPage*)item)->Index;

			//if (page >= 2 && page > currentPage + draw2Page)
			//	break;

			canDraw = ((page == -1) || ((page >= currentPage && page <= currentPage + draw2Page || (!page && !draw2Page))));
		}
		else if (canDraw && item->Enabled && item->Visible)
		{
			if (item->Type == GOT_GROUP)
			{
				group = ((CGUIGroup*)item)->Index;
				continue;
			}
			else if (!item->IsHTMLGump())
			{
				if (item->Type != GOT_DATABOX && item->Type != GOT_COMBOBOX && item->Type != GOT_SKILLITEM && item->Type != GOT_SKILLGROUP)
				{
					if (g_PressedObject.LeftObject == item)
					{
						if (g_SelectedObject.Object != g_PressedObject.LeftObject && !item->IsPressedOuthit())
							continue;
					}
					else
						continue;
				}
			}

			switch (item->Type)
			{
				case GOT_HITBOX:
				{
					if (!((CGUIPolygonal*)item)->CallOnMouseUp)
						break;

					CGUIHitBox *box = (CGUIHitBox*)item;

					if (box->ToPage != -1)
					{
						gump->Page = box->ToPage;

						//if (gump->Page < 1)
						//	gump->Page = 1;
					}

					gump->OnButton(item->Serial);
					gump->WantRedraw = true;

					return;
				}
				case GOT_COLOREDPOLYGONE:
				{
					if (!((CGUIPolygonal*)item)->CallOnMouseUp)
						break;

					gump->OnButton(item->Serial);
					gump->WantRedraw = true;

					return;
				}
				case GOT_RESIZEBUTTON:
				{
					gump->OnResizeEnd(item->Serial);
					gump->WantRedraw = true;
					break;
				}
				case GOT_SLIDER:
				{
					gump->WantRedraw = true;
					break;
				}
				case GOT_DATABOX:
				{
					CGump::TestItemsLeftMouseUp(gump, (CBaseGUI*)item->m_Items, 0);
					break;
				}
				case GOT_BUTTON:
				case GOT_BUTTONTILEART:
				{
					if (item->IsControlHTML())
						break;

					CGUIButton *button = (CGUIButton*)item;

					if (button->ToPage != -1)
					{
						gump->Page = button->ToPage;

						//if (gump->Page < 1)
						//	gump->Page = 1;
					}
					else
						gump->OnButton(item->Serial);

					gump->WantRedraw = true;

					return;
				}
				case GOT_TILEPICHIGHTLIGHTED:
				case GOT_GUMPPICHIGHTLIGHTED:
				{
					gump->OnButton(item->Serial);
					gump->WantRedraw = true;

					return;
				}
				case GOT_CHECKBOX:
				{
					CGUICheckbox *checkbox = (CGUICheckbox*)item;
					checkbox->Checked = !checkbox->Checked;

					gump->OnCheckbox(item->Serial, checkbox->Checked);
					gump->WantRedraw = true;

					return;
				}
				case GOT_RADIO:
				{
					CGUIRadio *radio = (CGUIRadio*)item;

					int radioPage = 0;
					int radioGroup = 0;

					QFOR(testRadio, start, CBaseGUI*)
					{
						if (testRadio->Type == GOT_PAGE)
							radioPage = ((CGUIPage*)testRadio)->Index;
						else if (testRadio->Type == GOT_GROUP)
							radioGroup = ((CGUIGroup*)testRadio)->Index;
						else if (testRadio->Type == GOT_RADIO)
						{
							if (page <= 1 && radioPage <= 1)
							{
								if (group == radioGroup)
								{
									if (((CGUIRadio*)testRadio)->Checked && testRadio != radio)
										gump->OnRadio(testRadio->Serial, false);

									((CGUIRadio*)testRadio)->Checked = false;
								}
							}
							else if (page == radioPage)
							{
								if (group == radioGroup)
								{
									if (((CGUIRadio*)testRadio)->Checked && testRadio != radio)
										gump->OnRadio(testRadio->Serial, false);

									((CGUIRadio*)testRadio)->Checked = false;
								}
							}
						}
					}

					radio->Checked = true;

					gump->OnRadio(item->Serial, true);
					gump->WantRedraw = true;

					return;
				}
				case GOT_COMBOBOX:
				{
					CGUIComboBox *combo = (CGUIComboBox*)item;

					int selectedCombo = combo->IsSelectedItem();

					if (selectedCombo != -1)
					{
						combo->SelectedIndex = selectedCombo;
						gump->OnComboboxSelection(item->Serial + selectedCombo);
						gump->WantRedraw = true;
					}

					break;
				}
				case GOT_SKILLITEM:
				{
					CGUISkillItem *skillItem = (CGUISkillItem*)item;

					if ((g_PressedObject.LeftObject == skillItem->m_ButtonUse && skillItem->m_ButtonUse != NULL) || g_PressedObject.LeftObject == skillItem->m_ButtonStatus)
					{
						gump->OnButton(g_PressedObject.LeftSerial);
						gump->WantRedraw = true;
					}

					break;
				}
				case GOT_SKILLGROUP:
				{
					CGUISkillGroup *skillGroup = (CGUISkillGroup*)item;

					if (g_PressedObject.LeftObject == skillGroup->m_Minimizer)
					{
						gump->OnButton(g_PressedObject.LeftSerial);
						gump->WantRedraw = true;
					}
					else
						TestItemsLeftMouseUp(gump, (CBaseGUI*)skillGroup->m_Items, currentPage, draw2Page);

					break;
				}
				case GOT_HTMLGUMP:
				case GOT_XFMHTMLGUMP:
				case GOT_XFMHTMLTOKEN:
				{
					TestItemsLeftMouseUp(gump, (CBaseGUI*)item->m_Items, currentPage, draw2Page);

					break;
				}
				default:
					break;
			}
		}
	}
}
//----------------------------------------------------------------------------------
void CGump::TestItemsScrolling(CGump *gump, CBaseGUI *start, const bool &up, const int &currentPage, const int &draw2Page)
{
	WISPFUN_DEBUG("c84_f13");
	const int delay = SCROLL_LISTING_DELAY / 7;

	int page = 0;
	int group = 0;
	bool canDraw = ((page == -1) || ((page >= currentPage && page <= currentPage + draw2Page || (!page && !draw2Page))));

	QFOR(item, start, CBaseGUI*)
	{
		if (item->Type == GOT_PAGE)
		{
			page = ((CGUIPage*)item)->Index;

			//if (page >= 2 && page > currentPage + draw2Page)
			//	break;

			canDraw = ((page == -1) || ((page >= currentPage && page <= currentPage + draw2Page || (!page && !draw2Page))));
		}
		else if (canDraw && item->Enabled && item->Visible)
		{
			if (item->Type == GOT_GROUP)
			{
				group = ((CGUIGroup*)item)->Index;
				continue;
			}
			else if (g_SelectedObject.Object != item && !item->IsHTMLGump())
				continue;

			switch (item->Type)
			{
				case GOT_RESIZEPIC:
				{
					if (!item->IsControlHTML())
						break;

					CGUIHTMLResizepic *resizepic = (CGUIHTMLResizepic*)item;
					resizepic->Scroll(up, delay);

					gump->OnSliderMove(item->Serial);
					gump->WantRedraw = true;

					break;
				}
				case GOT_SLIDER:
				{
					((CGUISlider*)item)->OnScroll(up, delay);

					gump->OnSliderMove(item->Serial);
					gump->WantRedraw = true;

					return;
				}
				case GOT_DATABOX:
				{
					CGump::TestItemsScrolling(gump, (CBaseGUI*)item->m_Items, up, currentPage, draw2Page);
					break;
				}
				case GOT_HTMLGUMP:
				case GOT_XFMHTMLGUMP:
				case GOT_XFMHTMLTOKEN:
				{
					if (item->Select())
					{
						CGUIHTMLGump *htmlGump = (CGUIHTMLGump*)item;

						WISP_GEOMETRY::CPoint2Di oldPos = g_MouseManager.Position;
						g_MouseManager.Position = WISP_GEOMETRY::CPoint2Di(oldPos.X - htmlGump->X, oldPos.Y - htmlGump->Y);

						CBaseGUI *item = (CBaseGUI*)htmlGump->m_Items;

						IFOR(j, 0, 5)
						{
							if (item->Type == GOT_SLIDER)
							{
								((CGUISlider*)item)->OnScroll(up, delay);

								gump->OnSliderMove(item->Serial);
							}

							item = (CBaseGUI*)item->m_Next;
						}

						int offsetX = htmlGump->DataOffset.X - htmlGump->CurrentOffset.X;
						int offsetY = htmlGump->DataOffset.Y - htmlGump->CurrentOffset.Y;

						g_MouseManager.Position = WISP_GEOMETRY::CPoint2Di(g_MouseManager.Position.X - offsetX, g_MouseManager.Position.Y - offsetY);

						TestItemsScrolling(gump, (CBaseGUI*)item, up, currentPage, draw2Page);

						g_MouseManager.Position = oldPos;

						gump->WantRedraw = true;
					}

					break;
				}
				default:
					break;
			}
		}
	}
}
//----------------------------------------------------------------------------------
void CGump::TestItemsDragging(CGump *gump, CBaseGUI *start, const int &currentPage, const int &draw2Page, int count)
{
	WISPFUN_DEBUG("c84_f14");
	int page = 0;
	int group = 0;
	bool canDraw = ((page == -1) || ((page >= currentPage && page <= currentPage + draw2Page || (!page && !draw2Page))));

	QFOR(item, start, CBaseGUI*)
	{
		if (!count)
			break;

		count--;

		if (item->Type == GOT_PAGE)
		{
			page = ((CGUIPage*)item)->Index;

			//if (page >= 2 && page > currentPage + draw2Page)
			//	break;

			canDraw = ((page == -1) || ((page >= currentPage && page <= currentPage + draw2Page || (!page && !draw2Page))));
		}
		else if (canDraw && item->Enabled && item->Visible)
		{
			if (item->Type == GOT_GROUP)
			{
				group = ((CGUIGroup*)item)->Index;
				continue;
			}
			else if (g_PressedObject.LeftObject != item && !item->IsHTMLGump())
				continue;

			switch (item->Type)
			{
				case GOT_SLIDER:
				{
					int x = g_MouseManager.Position.X - item->X;
					int y = g_MouseManager.Position.Y - item->Y;

					((CGUISlider*)item)->OnClick(x, y);

					gump->OnSliderMove(item->Serial);
					gump->WantRedraw = true;

					return;
				}
				case GOT_DATABOX:
				{
					CGump::TestItemsDragging(gump, (CBaseGUI*)item->m_Items, currentPage, draw2Page);
					break;
				}
				case GOT_RESIZEBUTTON:
				{
					gump->OnResize(item->Serial);
					gump->WantRedraw = true;

					break;
				}
				case GOT_HTMLGUMP:
				case GOT_XFMHTMLGUMP:
				case GOT_XFMHTMLTOKEN:
				{
					CGUIHTMLGump *htmlGump = (CGUIHTMLGump*)item;

					WISP_GEOMETRY::CPoint2Di oldPos = g_MouseManager.Position;
					g_MouseManager.Position = WISP_GEOMETRY::CPoint2Di(oldPos.X - htmlGump->X, oldPos.Y - htmlGump->Y);

					CBaseGUI *item = (CBaseGUI*)htmlGump->m_Items;

					TestItemsDragging(gump, item, currentPage, draw2Page, 5);

					IFOR(j, 0, 5)
						item = (CBaseGUI*)item->m_Next;

					int offsetX = htmlGump->DataOffset.X - htmlGump->CurrentOffset.X;
					int offsetY = htmlGump->DataOffset.Y - htmlGump->CurrentOffset.Y;

					g_MouseManager.Position = WISP_GEOMETRY::CPoint2Di(g_MouseManager.Position.X - offsetX, g_MouseManager.Position.Y - offsetY);

					TestItemsDragging(gump, item, currentPage, draw2Page);

					g_MouseManager.Position = oldPos;

					gump->WantRedraw = true;
					break;
				}
				default:
					break;
			}
		}
	}
}
//----------------------------------------------------------------------------------
void CGump::PrepareTextures()
{
	WISPFUN_DEBUG("c84_f15");
	QFOR(item, m_Items, CBaseGUI*)
		item->PrepareTextures();
}
//----------------------------------------------------------------------------------
bool CGump::EntryPointerHere()
{
	WISPFUN_DEBUG("c84_f16");
	QFOR(item, m_Items, CBaseGUI*)
	{
		if (item->Visible && item->EntryPointerHere())
			return true;
	}

	return false;
}
//----------------------------------------------------------------------------------
void CGump::GenerateFrame(const bool &stop)
{
	WISPFUN_DEBUG("c84_f17");
	if (!g_GL.Drawing)
	{
		m_FrameCreated = false;
		m_WantRedraw = true;

		return;
	}

	CalculateGumpState();

	PrepareTextures();

	if (g_ConfigManager.UseGLListsForInterface)
	{
		glNewList((GLuint)this, GL_COMPILE);

		DrawItems((CBaseGUI*)m_Items, m_Page, m_Draw2Page);

		if (stop)
			glEndList();
	}
	else
		DrawItems((CBaseGUI*)m_Items, m_Page, m_Draw2Page);

	m_WantRedraw = true;
	m_FrameCreated = true;
}
//----------------------------------------------------------------------------------
void CGump::Draw()
{
	WISPFUN_DEBUG("c84_f18");
	CalculateGumpState();

	if (m_WantUpdateContent)
	{
		UpdateContent();
		RecalculateSize();
		m_WantUpdateContent = false;
		m_FrameCreated = false;
	}

	if (!m_FrameCreated)
	{
		loc_create_frame:

		if (!g_ConfigManager.UseGLListsForInterface)
		{
			if (!m_FrameBuffer.Ready(m_GumpRect.Size))
				m_FrameBuffer.Init(m_GumpRect.Size);

			if (m_FrameBuffer.Use())
			{
				glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
				glClear(GL_COLOR_BUFFER_BIT);
				glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

				glTranslatef(-(GLfloat)m_GumpRect.Position.X, -(GLfloat)m_GumpRect.Position.Y, 0.0f);

				GenerateFrame(true);

				if (g_DeveloperMode == DM_DEBUGGING)
				{
					if (g_SelectedObject.Gump == this)
						glColor4f(0.0f, 1.0f, 0.0f, 0.2f);
					else
						glColor4f(1.0f, 1.0f, 1.0f, 0.2f);

					g_GL.DrawLine(m_GumpRect.Position.X + 1, m_GumpRect.Position.Y + 1, m_GumpRect.Position.X + m_GumpRect.Size.Width, m_GumpRect.Position.Y + 1);
					g_GL.DrawLine(m_GumpRect.Position.X + m_GumpRect.Size.Width, m_GumpRect.Position.Y + 1, m_GumpRect.Position.X + m_GumpRect.Size.Width, m_GumpRect.Position.Y + m_GumpRect.Size.Height);
					g_GL.DrawLine(m_GumpRect.Position.X + m_GumpRect.Size.Width, m_GumpRect.Position.Y + m_GumpRect.Size.Height, m_GumpRect.Position.X + 1, m_GumpRect.Position.Y + m_GumpRect.Size.Height);
					g_GL.DrawLine(m_GumpRect.Position.X + 1, m_GumpRect.Position.Y + m_GumpRect.Size.Height, m_GumpRect.Position.X + 1, m_GumpRect.Position.Y + 1);

					glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
				}

				glTranslatef((GLfloat)m_GumpRect.Position.X, (GLfloat)m_GumpRect.Position.Y, 0.0f);

				m_FrameBuffer.Release();
			}
		}
		else
			GenerateFrame(true);
	}
	else if (m_WantRedraw)
	{
		m_WantRedraw = false;
		goto loc_create_frame;
	}

	GLfloat posX = g_GumpTranslate.X;
	GLfloat posY = g_GumpTranslate.Y;

	if (!g_ConfigManager.UseGLListsForInterface)
	{
		posX += (GLfloat)m_GumpRect.Position.X;
		posY += (GLfloat)m_GumpRect.Position.Y;
	}

	glTranslatef(posX, posY, 0.0f);

	if (!g_ConfigManager.UseGLListsForInterface)
	{
		glEnable(GL_BLEND);
		//glBlendFunc(GL_DST_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

		m_FrameBuffer.Draw(0, 0);

		glDisable(GL_BLEND);
	}
	else
	{
		glCallList((GLuint)this);
		g_GL.OldTexture = 0;
	}

	DrawLocker();

	glTranslatef(-posX, -posY, 0.0f);
}
//----------------------------------------------------------------------------------
CRenderObject *CGump::Select()
{
	WISPFUN_DEBUG("c84_f19");
	g_CurrentCheckGump = this;
	CalculateGumpState();

	if (m_WantUpdateContent)
	{
		UpdateContent();
		RecalculateSize();
		m_WantUpdateContent = false;
		m_FrameCreated = false;
	}

	WISP_GEOMETRY::CPoint2Di oldPos = g_MouseManager.Position;
	g_MouseManager.Position = WISP_GEOMETRY::CPoint2Di(oldPos.X - (int)g_GumpTranslate.X, oldPos.Y - (int)g_GumpTranslate.Y);

	CRenderObject *selected = NULL;

	if (SelectLocker())
		selected = &m_Locker;
	else if (g_MouseManager.Position.X >= m_GumpRect.Position.X && g_MouseManager.Position.X < m_GumpRect.Position.X + m_GumpRect.Size.Width &&
		g_MouseManager.Position.Y >= m_GumpRect.Position.Y && g_MouseManager.Position.Y < m_GumpRect.Position.Y + m_GumpRect.Size.Height)
		selected = SelectItems((CBaseGUI*)m_Items, m_Page, m_Draw2Page);

	if (selected != NULL)
	{
		CBaseGUI *sel = (CBaseGUI*)selected;
		g_SelectedObject.Init(selected, this);
	}

	g_MouseManager.Position = oldPos;
	g_CurrentCheckGump = NULL;

	return selected;
}
//----------------------------------------------------------------------------------
void CGump::RecalculateSize()
{
	WISPFUN_DEBUG("c84_f19_1");

	WISP_GEOMETRY::CPoint2Di minPosition(999, 999);
	WISP_GEOMETRY::CPoint2Di maxPosition;
	WISP_GEOMETRY::CPoint2Di offset;

	GetItemsSize(this, (CBaseGUI*)m_Items, minPosition, maxPosition, offset, -1, m_Page, m_Draw2Page);

	WISP_GEOMETRY::CSize size(maxPosition.X - minPosition.X, maxPosition.Y - minPosition.Y);

	m_GumpRect = WISP_GEOMETRY::CRect(minPosition, size);
}
//----------------------------------------------------------------------------------
void CGump::GetItemsSize(CGump *gump, CBaseGUI *start, WISP_GEOMETRY::CPoint2Di &minPosition, WISP_GEOMETRY::CPoint2Di &maxPosition, WISP_GEOMETRY::CPoint2Di &offset, int count, const int &currentPage, const int &draw2Page)
{
	WISPFUN_DEBUG("c84_f19_2");

	int page = 0;
	bool canDraw = ((page == -1) || ((page >= currentPage && page <= currentPage + draw2Page || (!page && !draw2Page))));

	QFOR(item, start, CBaseGUI*)
	{
		if (!count)
			break;

		count--;

		if (item->Type == GOT_PAGE)
		{
			page = ((CGUIPage*)item)->Index;

			//if (page >= 2 && page > currentPage + draw2Page)
			//	break;

			canDraw = ((page == -1) || ((page >= currentPage && page <= currentPage + draw2Page || (!page && !draw2Page))));

			continue;
		}

		if (!canDraw || !item->Visible)
			continue;

		switch (item->Type)
		{
			case GOT_PAGE:
			case GOT_GROUP:
			case GOT_NONE:
			case GOT_MASTERGUMP:
			case GOT_CHECKTRANS:
			case GOT_SHADER:
			case GOT_BLENDING:
			case GOT_GLOBAL_COLOR:
			case GOT_TOOLTIP:
				break;
			case GOT_DATABOX:
			{
				CGump::GetItemsSize(gump, (CBaseGUI*)item->m_Items, minPosition, maxPosition, offset, count, currentPage, draw2Page);
				break;
			}
			case GOT_HTMLGUMP:
			case GOT_XFMHTMLGUMP:
			case GOT_XFMHTMLTOKEN:
			{
				WISP_GEOMETRY::CPoint2Di htmlOffset(offset.X + item->X, offset.X + item->Y);
				CGump::GetItemsSize(gump, (CBaseGUI*)item->m_Items, minPosition, maxPosition, htmlOffset, 5, currentPage, draw2Page);
				break;
			}
			case GOT_SCISSOR:
				((CGUIScissor*)item)->GumpParent = gump;
			default:
			{
				int x = item->X + offset.X;
				int y = item->Y + offset.Y;

				if (x < minPosition.X)
					minPosition.X = x;

				if (y < minPosition.Y)
					minPosition.Y = y;

				WISP_GEOMETRY::CSize itemSize = item->GetSize();

				x += itemSize.Width;
				y += itemSize.Height;

				if (x > maxPosition.X)
					maxPosition.X = x;

				if (y > maxPosition.Y)
					maxPosition.Y = y;

				break;
			}
		}
	}
}
//----------------------------------------------------------------------------------
void CGump::OnLeftMouseButtonDown()
{
	WISPFUN_DEBUG("c84_f20");
	g_CurrentCheckGump = this;
	WISP_GEOMETRY::CPoint2Di oldPos = g_MouseManager.Position;
	g_MouseManager.Position = WISP_GEOMETRY::CPoint2Di(oldPos.X - m_X, oldPos.Y - m_Y);

	TestItemsLeftMouseDown(this, (CBaseGUI*)m_Items, m_Page, m_Draw2Page);

	g_MouseManager.Position = oldPos;
	g_CurrentCheckGump = NULL;
}
//----------------------------------------------------------------------------------
void CGump::OnLeftMouseButtonUp()
{
	WISPFUN_DEBUG("c84_f21");
	g_CurrentCheckGump = this;
	TestItemsLeftMouseUp(this, (CBaseGUI*)m_Items, m_Page, m_Draw2Page);
	TestLockerClick();
	g_CurrentCheckGump = NULL;
}
//----------------------------------------------------------------------------------
void CGump::OnMidMouseButtonScroll(const bool &up)
{
	WISPFUN_DEBUG("c84_f22");
	g_CurrentCheckGump = this;
	WISP_GEOMETRY::CPoint2Di oldPos = g_MouseManager.Position;
	g_MouseManager.Position = WISP_GEOMETRY::CPoint2Di(oldPos.X - m_X, oldPos.Y - m_Y);

	TestItemsScrolling(this, (CBaseGUI*)m_Items, up, m_Page, m_Draw2Page);

	g_MouseManager.Position = oldPos;
	g_CurrentCheckGump = NULL;
}
//----------------------------------------------------------------------------------
void CGump::OnDragging()
{
	WISPFUN_DEBUG("c84_f23");
	g_CurrentCheckGump = this;
	WISP_GEOMETRY::CPoint2Di oldPos = g_MouseManager.Position;
	g_MouseManager.Position = WISP_GEOMETRY::CPoint2Di(oldPos.X - m_X, oldPos.Y - m_Y);

	TestItemsDragging(this, (CBaseGUI*)m_Items, m_Page, m_Draw2Page);

	g_MouseManager.Position = oldPos;
	g_CurrentCheckGump = NULL;
}
//----------------------------------------------------------------------------------
