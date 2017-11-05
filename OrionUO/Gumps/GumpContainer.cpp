// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
/***********************************************************************************
**
** GumpContainer.cpp
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#include "stdafx.h"
//----------------------------------------------------------------------------------
CGumpContainer::CGumpContainer(uint serial, uint id, short x, short y)
: CGump(GT_CONTAINER, serial, x, y), m_IsGameBoard(id == 0x091A || id == 0x092E)
{
	WISPFUN_DEBUG("c93_f1");
	m_Page = 1;
	m_Locker.Serial = ID_GC_LOCK_MOVING;
	m_ID = id;

	Add(new CGUIPage(1));
	Add(new CGUIGumppic(0x0050, 0, 0));

	Add(new CGUIPage(2));

	m_BodyGump = (CGUIGumppic*)Add(new CGUIGumppic((ushort)m_ID, 0, 0));

	if (m_ID == 0x0009)
	{
		if (m_CorpseEyesTicks < g_Ticks)
		{
			m_CorpseEyesOffset = (BYTE)!m_CorpseEyesOffset;
			m_CorpseEyesTicks = g_Ticks + 750;
		}

		m_CorpseEyes = (CGUIGumppic*)Add(new CGUIGumppic(0x0045, 45, 30));
	}

	if (m_ID == 0x003C)
	{
		CGUIHitBox *box = (CGUIHitBox*)Add(new CGUIHitBox(ID_GC_MINIMIZE, 106, 162, 16, 16, true));
		box->ToPage = 1;
	}

	Add(new CGUIShader(&g_ColorizerShader, true));

	m_DataBox = (CGUIDataBox*)Add(new CGUIDataBox());

	Add(new CGUIShader(&g_ColorizerShader, false));
}
//----------------------------------------------------------------------------------
CGumpContainer::~CGumpContainer()
{
}
//----------------------------------------------------------------------------------
void CGumpContainer::UpdateItemCoordinates(CGameObject *item)
{
	WISPFUN_DEBUG("c93_f3");
	if (m_Graphic < g_ContainerOffset.size())
	{
		const CContainerOffsetRect &rect = g_ContainerOffset[m_Graphic].Rect;

		if (item->X < rect.MinX)
			item->X = rect.MinX;

		if (item->Y < rect.MinY)
			item->Y = rect.MinY;

		if (item->X > rect.MinX + rect.MaxX)
			item->X = rect.MinX + rect.MaxX;

		if (item->Y > rect.MinY + rect.MaxY)
			item->Y = rect.MinY + rect.MaxY;
	}
}
//----------------------------------------------------------------------------------
void CGumpContainer::CalculateGumpState()
{
	WISPFUN_DEBUG("c93_f4");
	CGump::CalculateGumpState();

	if (g_GumpPressed && g_PressedObject.LeftObject != NULL && g_PressedObject.LeftObject->IsText())
	{
		g_GumpMovingOffset.Reset();

		if (m_Minimized)
		{
			g_GumpTranslate.X = (float)m_MinimizedX;
			g_GumpTranslate.Y = (float)m_MinimizedY;
		}
		else
		{
			g_GumpTranslate.X = (float)m_X;
			g_GumpTranslate.Y = (float)m_Y;
		}
	}
}
//----------------------------------------------------------------------------------
void CGumpContainer::PrepareTextures()
{
	WISPFUN_DEBUG("c93_f5");
	CGump::PrepareTextures();
	g_Orion.ExecuteGumpPart(0x0045, 2); //Corpse eyes
}
//----------------------------------------------------------------------------------
void CGumpContainer::InitToolTip()
{
	WISPFUN_DEBUG("c93_f6");
	if (!m_Minimized)
	{
		uint id = g_SelectedObject.Serial;

		if (id == ID_GC_MINIMIZE && g_ConfigManager.UseToolTips)
			g_ToolTip.Set(L"Minimize the container gump");
		else if (id == ID_GC_LOCK_MOVING && g_ConfigManager.UseToolTips)
			g_ToolTip.Set(L"Lock moving/closing the container gump");
		else if (g_ConfigManager.UseToolTips || g_TooltipsEnabled)
			g_ObjectPropertiesManager.Display(id);
	}
	else if (g_ConfigManager.UseToolTips)
		g_ToolTip.Set(L"Double click to maximize container gump");
}
//----------------------------------------------------------------------------------
void CGumpContainer::PrepareContent()
{
	WISPFUN_DEBUG("c93_f7");
	if (!g_Player->Dead() && GetTopObjDistance(g_Player, g_World->FindWorldObject(m_Serial)) <= DRAG_ITEMS_DISTANCE && g_PressedObject.LeftGump == this && !g_ObjectInHand.Enabled && g_PressedObject.LeftSerial != ID_GC_MINIMIZE && g_MouseManager.LastLeftButtonClickTimer < g_Ticks)
	{
		WISP_GEOMETRY::CPoint2Di offset = g_MouseManager.LeftDroppedOffset();

		if (CanBeDraggedByOffset(offset) || (g_MouseManager.LastLeftButtonClickTimer + g_MouseManager.DoubleClickDelay < g_Ticks))
		{
			CGameItem *selobj = g_World->FindWorldItem(g_PressedObject.LeftSerial);

			if (selobj != NULL && selobj->IsStackable() && selobj->Count > 1 && !g_ShiftPressed)
			{
				CGumpDrag *newgump = new CGumpDrag(g_PressedObject.LeftSerial, g_MouseManager.Position.X - 80, g_MouseManager.Position.Y - 34);

				g_GumpManager.AddGump(newgump);
				g_OrionWindow.EmulateOnLeftMouseButtonDown();
				selobj->Dragged = true;
			}
			else if (selobj != NULL)
			{
				//if (g_Target.IsTargeting())
				//	g_Target.SendCancelTarget();

				g_Orion.PickupItem(selobj, 0, m_IsGameBoard);

				g_PressedObject.ClearLeft();

				m_WantRedraw = true;
			}
		}
	}

	if (m_ID == 0x09 && m_CorpseEyes != NULL)
	{
		if (m_CorpseEyesTicks < g_Ticks)
		{
			m_CorpseEyesOffset = (uchar)!m_CorpseEyesOffset;
			m_CorpseEyesTicks = g_Ticks + 750;

			m_CorpseEyes->Graphic = 0x0045 + m_CorpseEyesOffset;
			m_WantRedraw = true;
		}
	}

	if (m_Minimized)
	{
		if (m_Page != 1)
		{
			m_Page = 1;
			m_WantUpdateContent = true;
		}
	}
	else
	{
		if (m_Page != 2)
		{
			m_Page = 2;
			m_WantUpdateContent = true;
		}

		if (m_TextRenderer.CalculatePositions(false))
			m_WantRedraw = true;
	}
}
//----------------------------------------------------------------------------------
void CGumpContainer::UpdateContent()
{
	WISPFUN_DEBUG("c93_f8");
	CGameItem *container = g_World->FindWorldItem(m_Serial);

	if (container == NULL)
		return;

	if ((ushort)m_ID == 0x003C)
	{
		ushort graphic = (ushort)m_ID;

		CGameItem *backpack = g_Player->FindLayer(OL_BACKPACK);

		if (backpack != NULL && backpack->Serial == m_Serial)
		{
			switch (g_ConfigManager.CharacterBackpackStyle)
			{
				case CBS_SUEDE:
					graphic = 0x775E;
					break;
				case CBS_POLAR_BEAR:
					graphic = 0x7760;
					break;
				case CBS_GHOUL_SKIN:
					graphic = 0x7762;
					break;
				default:
					graphic = 0x003C;
					break;
			}

			if (g_Orion.ExecuteGump(graphic) == NULL)
				graphic = 0x003C;

			m_BodyGump->Graphic = graphic;
		}
	}

	m_DataBox->Clear();

	m_IsGameBoard = (m_ID == 0x091A || m_ID == 0x092E);

	QFOR(obj, container->m_Items, CGameItem*)
	{
		int count = obj->Count;

		if ((obj->Layer == OL_NONE || (container->IsCorpse() && LAYER_UNSAFE[obj->Layer])) && count > 0)
		{
			bool doubleDraw = false;
			ushort graphic = obj->GetDrawGraphic(doubleDraw);
			CGUIGumppicHightlighted *item = NULL;

			if (m_IsGameBoard)
			{
				item = (CGUIGumppicHightlighted*)m_DataBox->Add(new CGUIGumppicHightlighted(obj->Serial, graphic - GAME_FIGURE_GUMP_OFFSET, obj->Color & 0x3FFF, 0x0035, obj->X, obj->Y - 20));
				item->PartialHue = false;
			}
			else
			{
				item = (CGUIGumppicHightlighted*)m_DataBox->Add(new CGUITilepicHightlighted(obj->Serial, graphic, obj->Color & 0x3FFF, 0x0035, obj->X, obj->Y, doubleDraw));
				item->PartialHue = IsPartialHue(g_Orion.GetStaticFlags(graphic));
			}
		}
	}
}
//----------------------------------------------------------------------------------
void CGumpContainer::Draw()
{
	WISPFUN_DEBUG("c93_f9");
	CGump::Draw();

	if (!m_Minimized)
	{
		glTranslatef(g_GumpTranslate.X, g_GumpTranslate.Y, 0.0f);

		g_FontColorizerShader.Use();

		m_TextRenderer.Draw();

		UnuseShader();

		glTranslatef(-g_GumpTranslate.X, -g_GumpTranslate.Y, 0.0f);
	}
}
//----------------------------------------------------------------------------------
CRenderObject *CGumpContainer::Select()
{
	WISPFUN_DEBUG("c93_f10");
	CRenderObject *selected = CGump::Select();

	if (!m_Minimized)
	{
		WISP_GEOMETRY::CPoint2Di oldPos = g_MouseManager.Position;
		g_MouseManager.Position = WISP_GEOMETRY::CPoint2Di(oldPos.X - (int)g_GumpTranslate.X, oldPos.Y - (int)g_GumpTranslate.Y);

		m_TextRenderer.Select(this);

		g_MouseManager.Position = oldPos;
	}

	return selected;
}
//----------------------------------------------------------------------------------
void CGumpContainer::GUMP_BUTTON_EVENT_C
{
	WISPFUN_DEBUG("c93_f11");
	if (!m_Minimized && serial == ID_GC_MINIMIZE && m_ID == 0x003C)
		m_Minimized = true;
	else if (serial == ID_GC_LOCK_MOVING)
	{
		m_LockMoving = !m_LockMoving;
		g_MouseManager.CancelDoubleClick = true;
	}
}
//----------------------------------------------------------------------------------
void CGumpContainer::OnLeftMouseButtonUp()
{
	WISPFUN_DEBUG("c93_f12");
	CGump::OnLeftMouseButtonUp();

	uint dropContainer = m_Serial;
	uint selectedSerial = g_SelectedObject.Serial;

	if (g_Target.IsTargeting() && !g_ObjectInHand.Enabled && selectedSerial && selectedSerial != ID_GC_MINIMIZE && selectedSerial != ID_GC_LOCK_MOVING)
	{
		g_Target.SendTargetObject(selectedSerial);
		g_MouseManager.CancelDoubleClick = true;

		return;
	}

	bool canDrop = (GetTopObjDistance(g_Player, g_World->FindWorldObject(dropContainer)) <= DRAG_ITEMS_DISTANCE);

	if (canDrop && selectedSerial && selectedSerial != ID_GC_MINIMIZE && selectedSerial != ID_GC_LOCK_MOVING)
	{
		canDrop = false;

		if (g_ObjectInHand.Enabled)
		{
			canDrop = true;

			CGameItem *target = g_World->FindWorldItem(selectedSerial);

			if (target != NULL)
			{
				if (target->IsContainer())
					dropContainer = target->Serial;
				else if (target->IsStackable() && target->Graphic == g_ObjectInHand.Graphic)
					dropContainer = target->Serial;
				else
				{
					switch (target->Graphic)
					{
						case 0x0EFA:
						case 0x2253:
						case 0x2252:
						case 0x238C:
						case 0x23A0:
						case 0x2D50:
						{
							dropContainer = target->Serial;
							break;
						}
						default:
							break;
					}
				}
			}
		}
	}

	if (!canDrop && g_ObjectInHand.Enabled)
		g_Orion.PlaySoundEffect(0x0051);

	int x = g_MouseManager.Position.X - m_X;
	int y = g_MouseManager.Position.Y - m_Y;

	if (canDrop && g_ObjectInHand.Enabled)
	{
		const CContainerOffsetRect &r = g_ContainerOffset[Graphic].Rect;

		bool doubleDraw = false;
		ushort graphic = g_ObjectInHand.GetDrawGraphic(doubleDraw);

		CGLTexture *th = g_Orion.ExecuteStaticArt(graphic);

		if (m_IsGameBoard)
		{
			th = g_Orion.ExecuteGump(graphic - GAME_FIGURE_GUMP_OFFSET);
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
			CGameItem *target = g_World->FindWorldItem(selectedSerial);

			if (target->IsContainer())
			{
				x = -1;
				y = -1;
			}
		}

		g_Orion.DropItem(dropContainer, x, y, 0);
		g_MouseManager.CancelDoubleClick = true;
	}
	else if (!g_ObjectInHand.Enabled)
	{
		if (!g_ClickObject.Enabled)
		{
			CGameObject *clickTarget = g_World->FindWorldObject(selectedSerial);

			if (clickTarget != NULL)
			{
				g_ClickObject.Init(clickTarget);
				g_ClickObject.Timer = g_Ticks + g_MouseManager.DoubleClickDelay;
				g_ClickObject.X = x;
				g_ClickObject.Y = y;
			}
		}
	}
}
//----------------------------------------------------------------------------------
bool CGumpContainer::OnLeftMouseButtonDoubleClick()
{
	WISPFUN_DEBUG("c93_f13");
	bool result = false;

	if (!g_PressedObject.LeftSerial && m_Minimized && m_ID == 0x003C)
	{
		m_Minimized = false;
		m_Page = 2;
		m_WantUpdateContent = true;

		result = true;
	}
	else if (g_PressedObject.LeftSerial && g_PressedObject.LeftSerial != ID_GC_MINIMIZE)
	{
		g_Orion.DoubleClick(g_PressedObject.LeftSerial);
		m_FrameCreated = false;

		result = true;
	}

	return result;
}
//----------------------------------------------------------------------------------
