﻿// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
/***********************************************************************************
**
** GumpShop.cpp
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#include "stdafx.h"
//----------------------------------------------------------------------------------
CGumpShop::CGumpShop(uint serial, bool isBuyGump, short x, short y)
: CGump(GT_SHOP, serial, x, y), IsBuyGump(isBuyGump)
{
	WISPFUN_DEBUG("c123_f1");
	Visible = !isBuyGump;

	if (isBuyGump)
		Add(new CGUIGumppic(0x0870, 0, 0));
	else
		Add(new CGUIGumppic(0x0872, 0, 0));

	Add(new CGUIShader(&g_ColorizerShader, true));
	m_ItemList[0] = (CGUIHTMLGump*)Add(new CGUIHTMLGump(ID_GB_SHOP_LIST, 0, 30, 60, 215, 176, false, true));
	Add(new CGUIShader(&g_ColorizerShader, false));

	if (isBuyGump)
		Add(new CGUIGumppic(0x0871, 170, 214));
	else
		Add(new CGUIGumppic(0x0873, 170, 214));

	m_ItemList[1] = (CGUIHTMLGump*)Add(new CGUIHTMLGump(ID_GB_SHOP_RESULT, 0, 200, 280, 215, 92, false, true));

	Add(new CGUIHitBox(ID_GB_BUTTON_ACCEPT, 200, 406, 34, 30, true));
	Add(new CGUIHitBox(ID_GB_BUTTON_CLEAR, 372, 410, 24, 24, true));

	if (isBuyGump)
	{
		m_TotalPriceText = (CGUIText*)Add(new CGUIText(0x0386, 240, 385));
		m_TotalPriceText->CreateTextureA(9, "0");
		m_TotalGoldText = (CGUIText*)Add(new CGUIText(0x0386, 358, 385));
		m_TotalGoldText->CreateTextureA(9, std::to_string(g_Player->Gold));
	}
	else
	{
		m_TotalPriceText = (CGUIText*)Add(new CGUIText(0x0386, 358, 386));
		m_TotalPriceText->CreateTextureA(9, "0");

		m_TotalGoldText = NULL;
	}

	m_NameText = (CGUIText*)Add(new CGUIText(0x0386, 242, 408));

	IFOR(i, 0, 2)
	{
		CGUIHTMLButton *button = m_ItemList[i]->m_ButtonUp;

		if (button != NULL)
		{
			button->Graphic = 0x0824;
			button->GraphicSelected = 0x0824;
			button->GraphicPressed = 0x0824;
			button->CheckPolygone = true;

			if (!i)
				button->SetY(button->GetY() - 11);
			else
				button->SetY(button->GetY() - 16);
		}

		button = m_ItemList[i]->m_ButtonDown;

		if (button != NULL)
		{
			button->Graphic = 0x0825;
			button->GraphicSelected = 0x0825;
			button->GraphicPressed = 0x0825;
			button->CheckPolygone = true;

			if (!i)
				button->SetY(button->GetY() - 25);
			else
			{
				button->SetX(button->GetX() - 1);
				button->SetY(button->GetY() + 18);
			}
		}

		CGUIHTMLSlider *slider = m_ItemList[i]->m_Slider;

		if (slider != NULL)
		{
			slider->Graphic = 0x001F;
			slider->GraphicSelected = 0x001F;
			slider->GraphicPressed = 0x001F;
			slider->BackgroundGraphic = 0;

			if (!i)
			{
				slider->SetY(slider->GetY() - 11);
				slider->Length -= 14;
			}
			else
			{
				slider->SetY(slider->GetY() - 16);
				slider->Length += 34;
			}
		}
	}
}
//----------------------------------------------------------------------------------
CGumpShop::~CGumpShop()
{
}
//----------------------------------------------------------------------------------
void CGumpShop::SendList()
{
	WISPFUN_DEBUG("c123_f2");
	CGameCharacter *vendor = g_World->FindWorldCharacter(Serial);

	if (vendor == NULL)
		return;

	if (IsBuyGump)
		CPacketBuyRequest(this).Send();
	else
		CPacketSellRequest(this).Send();
}
//----------------------------------------------------------------------------------
void CGumpShop::UpdateTotalPrice()
{
	WISPFUN_DEBUG("c123_f3");
	if (m_TotalPriceText != NULL)
	{
		int totalPrice = 0;

		QFOR(item, m_ItemList[1]->m_Items, CBaseGUI*)
		{
			if (item->Type == GOT_SHOPRESULT)
			{
				CGUIShopResult *shop = (CGUIShopResult*)item;

				totalPrice += shop->Price * shop->m_MinMaxButtons->Value;
			}
		}

		m_TotalPriceText->CreateTextureA(9, std::to_string(totalPrice));
	}
}
//----------------------------------------------------------------------------------
void CGumpShop::PrepareContent()
{
	WISPFUN_DEBUG("c123_f4");
	if (NoProcess && g_Player != NULL)
	{
		string name = g_Player->GetName();
		int len = (int)name.length();

		if (m_ContinueCounter > len)
			m_ContinueCounter = len;

		name.resize(m_ContinueCounter);

		static int counterCount = 0;

		if (m_ContinueCounter == len)
		{
			NoProcess = false;
			SendList();
		}
		else if (!counterCount)
		{
			if (m_ContinueCounter < len)
				m_ContinueCounter++;
		}

		counterCount++;

		if (counterCount >= 3)
			counterCount = 0;

		m_NameText->CreateTextureA(5, name);

		WantRedraw = true;
	}
}
//----------------------------------------------------------------------------------
void CGumpShop::GUMP_BUTTON_EVENT_C
{
	WISPFUN_DEBUG("c123_f5");
	if (serial == ID_GB_BUTTON_ACCEPT) //Accept
	{
		NoProcess = true;
		m_ContinueCounter = 0;
	}
	else if (serial == ID_GB_BUTTON_CLEAR) //Clear
		m_ContinueCounter = 0;
}
//----------------------------------------------------------------------------------
void CGumpShop::GUMP_SCROLL_BUTTON_EVENT_C
{
	WISPFUN_DEBUG("c123_f6");
	CGUIMinMaxButtons *minmax = (CGUIMinMaxButtons*)g_PressedObject.LeftObject;

	if (minmax == NULL)
		return;

	bool deleteItem = false;

	QFOR(item, m_ItemList[0]->m_Items, CBaseGUI*)
	{
		if (item->Type == GOT_SHOPITEM && item->Serial == minmax->Serial)
		{
			((CGUIShopItem*)item)->CreateCountText(minmax->Value);

			deleteItem = (minmax->Value < 1);

			break;
		}
	}

	if (deleteItem)
	{
		uint itemSerial = minmax->Serial;
		int y = 0;

		for (CBaseGUI *item = (CBaseGUI*)m_ItemList[1]->m_Items; item != NULL;)
		{
			CBaseGUI *next = (CBaseGUI*)item->m_Next;

			if (item->Type == GOT_SHOPRESULT)
			{
				if (item->Serial == itemSerial)
					m_ItemList[1]->Delete(item);
				else
				{
					item->SetY(y);
					y += item->GetSize().Height;
				}
			}

			item = next;
		}
	}

	UpdateTotalPrice();
}
//----------------------------------------------------------------------------------
void CGumpShop::OnLeftMouseButtonUp()
{
	WISPFUN_DEBUG("c123_f7");
	CGump::OnLeftMouseButtonUp();

	if (g_Target.IsTargeting() && !g_ObjectInHand.Enabled && g_World->FindWorldObject(g_SelectedObject.Serial) != NULL)
	{
		g_Target.SendTargetObject(g_SelectedObject.Serial);
		g_MouseManager.CancelDoubleClick = true;
	}
}
//----------------------------------------------------------------------------------
bool CGumpShop::OnLeftMouseButtonDoubleClick()
{
	WISPFUN_DEBUG("c123_f8");
	bool result = false;

	if (g_PressedObject.LeftObject != NULL && g_PressedObject.LeftObject->IsGUI())
	{
		if (((CBaseGUI*)g_PressedObject.LeftObject)->Type == GOT_SHOPITEM)
		{
			CGUIShopItem *shopItem = (CGUIShopItem*)g_PressedObject.LeftObject;
			CGUIShopResult *shopResult = NULL;

			int posY = 0;

			QFOR(item, m_ItemList[1]->m_Items, CBaseGUI*)
			{
				if (item->Type == GOT_SHOPRESULT)
				{
					if (item->Serial == shopItem->Serial)
					{
						shopResult = (CGUIShopResult*)item;
						break;
					}

					posY += ((CGUIShopResult*)item)->GetSize().Height;
				}
			}

			if (shopResult != NULL)
			{
				CGUIMinMaxButtons *minmax = shopResult->m_MinMaxButtons;

				if (minmax->Value < minmax->MaxValue)
				{
					if (g_ShiftPressed)
						minmax->Value = minmax->MaxValue;
					else
						minmax->Value += 1;

					minmax->UpdateText();
					shopItem->CreateCountText(minmax->Value);
					WantRedraw = true;
					result = true;
					UpdateTotalPrice();
				}
			}
			else
			{
				shopResult = (CGUIShopResult*)m_ItemList[1]->Add(new CGUIShopResult(shopItem, 0, posY));

				CGUIMinMaxButtons *minmax = shopResult->m_MinMaxButtons;

				if (g_ShiftPressed)
				{
					minmax->Value = minmax->MaxValue;
					minmax->UpdateText();
				}

				shopItem->CreateCountText(minmax->Value);
				WantRedraw = true;
				result = true;
				UpdateTotalPrice();
				m_ItemList[1]->CalculateDataSize();
			}
		}
	}

	return result;
}
//----------------------------------------------------------------------------------
