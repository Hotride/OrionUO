/***********************************************************************************
**
** GumpSecureTrading.h
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#ifndef GUMPSECURETRADING_H
#define GUMPSECURETRADING_H
//----------------------------------------------------------------------------------
#include "Gump.h"
#include "../TextEngine/TextRenderer.h"
//----------------------------------------------------------------------------------
class CGumpSecureTrading : public CGump
{
	SETGET(uint, ID2);
	SETGET(bool, StateMy);
	SETGET(bool, StateOpponent);
	SETGET(string, Text);

private:
	static const int ID_GST_CHECKBOX = 1;
	
	CTextRenderer m_TextRenderer;

	CGUIButton *m_MyCheck;
	CGUIGumppic *m_OpponentCheck;

	CGUIDataBox *m_MyDataBox;
	CGUIDataBox *m_OpponentDataBox;
	
protected:
	virtual void CalculateGumpState();

public:
	CGumpSecureTrading(uint serial, short x, short y, uint id, uint id2);
	virtual ~CGumpSecureTrading();

	void SendTradingResponse(int code);
	
	CTextRenderer *GetTextRenderer() {return &m_TextRenderer;}

	virtual void InitToolTip();

	virtual void PrepareContent();

	virtual void UpdateContent();

	virtual void Draw();
	virtual CRenderObject *Select();

	GUMP_BUTTON_EVENT_H;

	virtual void OnLeftMouseButtonUp();
};
//----------------------------------------------------------------------------------
#endif
//----------------------------------------------------------------------------------
