/***********************************************************************************
**
** GumpSecureTrading.cpp
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#include "stdafx.h"
//----------------------------------------------------------------------------------
CGumpSecureTrading::CGumpSecureTrading(uint serial, short x, short y, uint id, uint id2)
: CGump(GT_TRADE, serial, x, y), m_ID2(id2)
{
	m_ID = id;
}
//----------------------------------------------------------------------------------
CGumpSecureTrading::~CGumpSecureTrading()
{
}
//----------------------------------------------------------------------------------
void CGumpSecureTrading::CalculateGumpState()
{
	WISPFUN_DEBUG("c120_f1");
	CGump::CalculateGumpState();

	if (g_GumpPressed && g_PressedObject.LeftObject != NULL && g_PressedObject.LeftObject->IsText())
	{
		g_GumpMovingOffset.Reset();

		g_GumpTranslate.X = (float)m_X;
		g_GumpTranslate.Y = (float)m_Y;
	}

	if (g_GumpTranslate.X || g_GumpTranslate.Y)
		m_WantRedraw = true;
}
//----------------------------------------------------------------------------------
void CGumpSecureTrading::InitToolTip()
{
	WISPFUN_DEBUG("c120_f2");
	if (g_ConfigManager.UseToolTips || g_TooltipsEnabled)
	{
		uint id = g_SelectedObject.Serial;

		CGameObject *obj = g_World->FindWorldObject(id);

		if (obj != NULL && obj->ClilocMessage.length())
			g_ToolTip.Set(obj->ClilocMessage);
	}
}
//----------------------------------------------------------------------------------
void CGumpSecureTrading::PrepareContent()
{
	WISPFUN_DEBUG("c120_f3");
	if (m_MyCheck != NULL)
	{
		if (m_StateMy)
		{
			if (m_MyCheck->Graphic != 0x0869)
			{
				m_MyCheck->Graphic = 0x0869;
				m_MyCheck->GraphicSelected = 0x086A;
				m_MyCheck->GraphicPressed = 0x086A;
				m_WantRedraw = true;
			}
		}
		else if (m_MyCheck->Graphic != 0x0867)
		{
			m_MyCheck->Graphic = 0x0867;
			m_MyCheck->GraphicSelected = 0x0868;
			m_MyCheck->GraphicPressed = 0x0868;
			m_WantRedraw = true;
		}
	}

	if (m_OpponentCheck != NULL)
	{
		if (m_StateOpponent)
		{
			if (m_OpponentCheck->Graphic != 0x0869)
			{
				m_OpponentCheck->Graphic = 0x0869;
				m_WantRedraw = true;
			}
		}
		else if (m_OpponentCheck->Graphic != 0x0867)
		{
			m_OpponentCheck->Graphic = 0x0867;
			m_WantRedraw = true;
		}
	}

	if (m_TextRenderer.CalculatePositions(false))
		m_WantRedraw = true;
}
//----------------------------------------------------------------------------------
void CGumpSecureTrading::UpdateContent()
{
	WISPFUN_DEBUG("c120_f4");
	CGameObject *selobj = g_World->FindWorldObject(m_Serial);

	if (selobj == NULL)
		return; //Объект, к которому привязан гамп - исчез

	if (m_Items == NULL)
	{
		Add(new CGUIGumppic(0x0866, 0, 0)); //Trade Gump

		if (g_PacketManager.ClientVersion < CV_500A)
		{
			Add(new CGUIColoredPolygone(0, 0, 45, 90, 110, 60, 0xFF000001));

			Add(new CGUIColoredPolygone(0, 0, 192, 70, 110, 60, 0xFF000001));
		}

		if (m_StateMy)
			m_MyCheck = (CGUIButton*)Add(new CGUIButton(ID_GST_CHECKBOX, 0x0869, 0x086A, 0x086A, 52, 29));
		else
			m_MyCheck = (CGUIButton*)Add(new CGUIButton(ID_GST_CHECKBOX, 0x0867, 0x0868, 0x0868, 52, 29));

		CGUIText *text = (CGUIText*)Add(new CGUIText(0x0386, 84, 40));
		text->CreateTextureA(1, g_Player->Name);

		if (m_StateOpponent)
			m_OpponentCheck = (CGUIGumppic*)Add(new CGUIGumppic(0x0869, 266, 160));
		else
			m_OpponentCheck = (CGUIGumppic*)Add(new CGUIGumppic(0x0867, 266, 160));

		int fontWidth = 260 - g_FontManager.GetWidthA(1, m_Text.c_str(), m_Text.length());

		text = (CGUIText*)Add(new CGUIText(0x0386, fontWidth, 170));
		text->CreateTextureA(1, m_Text);

		Add(new CGUIShader(&g_ColorizerShader, true));

		Add(new CGUIScissor(true, 0, 0, 45, 70, 110, 80));
		m_MyDataBox = (CGUIDataBox*)Add(new CGUIDataBox());
		Add(new CGUIScissor(false));

		Add(new CGUIScissor(true, 0, 0, 192, 70, 110, 80));
		m_OpponentDataBox = (CGUIDataBox*)Add(new CGUIDataBox());
		Add(new CGUIScissor(false));

		Add(new CGUIShader(&g_ColorizerShader, false));
	}
	else
	{
		m_MyDataBox->Clear();
		m_OpponentDataBox->Clear();
	}

	//Отрисовка нашего товара (при наличии товара)
	CGameObject *container = g_World->FindWorldObject(m_ID);

	if (container != NULL && container->m_Items != NULL)
	{
		QFOR(item, container->m_Items, CGameItem*)
		{
			bool doubleDraw = false;
			ushort graphic = item->GetDrawGraphic(doubleDraw);

			CGUITilepicHightlighted *dataObject = (CGUITilepicHightlighted*)m_MyDataBox->Add(new CGUITilepicHightlighted(item->Serial, graphic, item->Color & 0x3FFF, 0x0035, 45 + item->X, 70 + item->Y, doubleDraw));
			dataObject->PartialHue = IsPartialHue(g_Orion.GetStaticFlags(graphic));

			if (dataObject->Y >= 150)
				dataObject->Y = 120;

			if (dataObject->X >= 155)
				dataObject->X = 125;
		}
	}

	//Отрисовка нашего опонента (при наличии товара)
	container = g_World->FindWorldObject(m_ID2);
	if (container != NULL && container->m_Items != NULL)
	{
		QFOR(item, container->m_Items, CGameItem*)
		{
			bool doubleDraw = false;
			ushort graphic = item->GetDrawGraphic(doubleDraw);

			CGUITilepicHightlighted *dataObject = (CGUITilepicHightlighted*)m_OpponentDataBox->Add(new CGUITilepicHightlighted(item->Serial, graphic, item->Color & 0x3FFF, 0x0035, 192 + item->X, 70 + item->Y, doubleDraw));
			dataObject->PartialHue = IsPartialHue(g_Orion.GetStaticFlags(graphic));

			if (dataObject->Y >= 150)
				dataObject->Y = 120;

			if (dataObject->X >= 302)
				dataObject->X = 272;
		}
	}
}
//----------------------------------------------------------------------------------
void CGumpSecureTrading::Draw()
{
	WISPFUN_DEBUG("c120_f5");
	CGameObject *selobj = g_World->FindWorldObject(m_Serial);

	if (selobj == NULL)
		return; //Объект, к которому привязан гамп - исчез
	
	if (g_GumpPressed)
		m_WantRedraw = true;

	CGump::Draw();

	glTranslatef(g_GumpTranslate.X, g_GumpTranslate.Y, 0.0f);

	g_FontColorizerShader.Use();

	m_TextRenderer.Draw();

	UnuseShader();

	glTranslatef(-g_GumpTranslate.X, -g_GumpTranslate.Y, 0.0f);
}
//----------------------------------------------------------------------------------
CRenderObject *CGumpSecureTrading::Select()
{
	WISPFUN_DEBUG("c120_f6");
	CGameObject *selobj = g_World->FindWorldObject(m_Serial);

	if (selobj == NULL)
		return NULL; //Объект, к которому привязан гамп - исчез

	CRenderObject *selected = CGump::Select();

	WISP_GEOMETRY::CPoint2Di oldPos = g_MouseManager.Position;
	g_MouseManager.Position = WISP_GEOMETRY::CPoint2Di(oldPos.X - (int)g_GumpTranslate.X, oldPos.Y - (int)g_GumpTranslate.Y);

	m_TextRenderer.Select(this);

	g_MouseManager.Position = oldPos;

	return selected;
}
//----------------------------------------------------------------------------------
void CGumpSecureTrading::GUMP_BUTTON_EVENT_C
{
	WISPFUN_DEBUG("c120_f7");
	if (serial == ID_GST_CHECKBOX) //Изменение состояния чекбокса
	{
		m_StateMy = !m_StateMy;

		SendTradingResponse(2);
	}
	else
	{
		if (!g_ClickObject.Enabled && !g_TooltipsEnabled)
		{
			CGameObject *clickTarget = g_World->FindWorldObject(serial);

			if (clickTarget == NULL)
				return;

			g_ClickObject.Init(clickTarget);
			g_ClickObject.Timer = g_Ticks + DCLICK_DELAY;
			g_ClickObject.X = g_MouseManager.Position.X - m_X;
			g_ClickObject.Y = g_MouseManager.Position.Y - m_Y;
		}
	}
}
//----------------------------------------------------------------------------------
void CGumpSecureTrading::OnLeftMouseButtonUp()
{
	WISPFUN_DEBUG("c120_f8");
	CGump::OnLeftMouseButtonUp();

	if (g_ObjectInHand.Enabled)
	{
		int x = m_X;
		int y = m_Y;

		if (g_Orion.PolygonePixelsInXY(x + 45, y + 70, 110, 80))
		{
			if (GetTopObjDistance(g_Player, g_World->FindWorldObject(m_ID2)) <= DRAG_ITEMS_DISTANCE)
			{
				x = g_MouseManager.Position.X - x - 45;
				y = g_MouseManager.Position.Y - y - 70;

				bool doubleDraw = false;
				ushort graphic = g_ObjectInHand.GetDrawGraphic(doubleDraw);

				CGLTexture *th = g_Orion.ExecuteStaticArt(graphic);

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

				g_Orion.DropItem(m_ID, x, y, 0);
				g_MouseManager.CancelDoubleClick = true;
			}
			else
				g_Orion.PlaySoundEffect(0x0051);
		}
	}
	else if (g_Target.IsTargeting() && g_World->FindWorldObject(g_SelectedObject.Serial) != NULL)
	{
		g_Target.SendTargetObject(g_SelectedObject.Serial);
		g_MouseManager.CancelDoubleClick = true;
	}
}
//----------------------------------------------------------------------------------
void CGumpSecureTrading::SendTradingResponse(int code)
{
	WISPFUN_DEBUG("c120_f9");
	//Ответ на трэйд окно
	CPacketTradeResponse(this, code).Send();

	if (code == 1) //Закрываем окно
		m_RemoveMark = true;
}
//----------------------------------------------------------------------------------
