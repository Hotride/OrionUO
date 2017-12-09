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
class CGumpSecureTrading : public CGump
{
	SETGET(uint, ID2, 0);
	SETGET(bool, StateMy, false);
	SETGET(bool, StateOpponent, false);
	SETGET(string, Text, "");

private:
	static const int ID_GST_CHECKBOX = 1;

	CTextRenderer m_TextRenderer{ CTextRenderer() };

	CGUIButton *m_MyCheck{ NULL };
	CGUIGumppic *m_OpponentCheck{ NULL };

	CGUIDataBox *m_MyDataBox{ NULL };
	CGUIDataBox *m_OpponentDataBox{ NULL };
	
protected:
	virtual void CalculateGumpState();

public:
	CGumpSecureTrading(uint serial, short x, short y, uint id, uint id2);
	virtual ~CGumpSecureTrading();

	void SendTradingResponse(int code);
	
	CTextRenderer *GetTextRenderer() {return &m_TextRenderer;}

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
