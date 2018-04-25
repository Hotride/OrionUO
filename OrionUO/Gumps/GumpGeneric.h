/***********************************************************************************
**
** GumpGeneric.h
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#pragma once
#include <SDL_events.h>
//----------------------------------------------------------------------------------
class CGumpGeneric : public CGump
{
public:
	uint MasterGump = 0;

	CGumpGeneric(uint serial, short x, short y, uint id);
	virtual ~CGumpGeneric();

	void AddText(int index, const wstring &text, CBaseGUI *start = NULL, bool backbroundCanBeColored = false);

	void SendGumpResponse(int index);

	virtual void InitToolTip();

	GUMP_BUTTON_EVENT_H;
	GUMP_DIRECT_HTML_LINK_EVENT_H;

	virtual bool OnLeftMouseButtonDoubleClick();

#if USE_WISP
	virtual void OnCharPress(const WPARAM &wParam, const LPARAM &lParam);
	virtual void OnKeyDown(const WPARAM &wParam, const LPARAM &lParam);
#else
	virtual void OnTextInput(const SDL_TextInputEvent &ev) override;
	virtual void OnKeyDown(const SDL_KeyboardEvent &ev) override;
#endif	
};
