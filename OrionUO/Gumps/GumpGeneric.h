/***********************************************************************************
**
** GumpGeneric.h
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#ifndef GUMPGENERIC_H
#define GUMPGENERIC_H
//----------------------------------------------------------------------------------
class CGumpGeneric : public CGump
{
public:
	uint MasterGump = 0;

	CGumpGeneric(uint serial, short x, short y, uint id);
	virtual ~CGumpGeneric();

	void AddText(const int &index, const wstring &text, CBaseGUI *start = NULL, const bool &backbroundCanBeColored = false);

	void SendGumpResponse(int index);

	virtual void InitToolTip();

	GUMP_BUTTON_EVENT_H;
	GUMP_DIRECT_HTML_LINK_EVENT_H;

	virtual bool OnLeftMouseButtonDoubleClick();
	virtual void OnCharPress(const WPARAM &wParam, const LPARAM &lParam);
	virtual void OnKeyDown(const WPARAM &wParam, const LPARAM &lParam);
};
//----------------------------------------------------------------------------------
#endif
//----------------------------------------------------------------------------------
