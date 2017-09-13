/***********************************************************************************
**
** GumpShop.h
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#ifndef GUMPSHOP_H
#define GUMPSHOP_H
//----------------------------------------------------------------------------------
class CGumpShop : public CGump
{
	SETGET(bool, IsBuyGump, false);
	SETGET(bool, Visible, false);

private:
	static const int ID_GB_BUTTON_ACCEPT = 1;
	static const int ID_GB_BUTTON_CLEAR = 2;
	static const int ID_GB_SHOP_LIST = 3;
	static const int ID_GB_SHOP_RESULT = 4;

	int m_ContinueCounter{ 0 };

	void UpdateTotalPrice();

public:
	CGumpShop(uint serial, bool isBuyGump, short x, short y);
	virtual ~CGumpShop();

	CGUIHTMLGump *m_ItemList[2];
	CGUIText *m_TotalPriceText{ NULL };
	CGUIText *m_TotalGoldText{ NULL };
	CGUIText *m_NameText{ NULL };

	void SendList();

	virtual void PrepareContent();

	virtual bool CanBeDisplayed() { return m_Visible; }

	GUMP_BUTTON_EVENT_H;
	GUMP_SCROLL_BUTTON_EVENT_H;

	virtual void OnLeftMouseButtonUp();
	virtual bool OnLeftMouseButtonDoubleClick();
};
//----------------------------------------------------------------------------------
#endif
//----------------------------------------------------------------------------------
