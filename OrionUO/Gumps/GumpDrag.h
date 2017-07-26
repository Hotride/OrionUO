/***********************************************************************************
**
** GumpDrag.h
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#ifndef GUMPDRAG_H
#define GUMPDRAG_H
//----------------------------------------------------------------------------------
#include "Gump.h"
//----------------------------------------------------------------------------------
class CGumpDrag : public CGump
{
private:
	static const int ID_GD_SCROLL = 1;
	static const int ID_GD_OKAY = 2;
	static const int ID_GD_TEXT_FIELD = 3;

	bool m_StartText{ true };

	CGUITextEntry *m_Entry{ NULL };
	CGUISlider *m_Slider{ NULL };

public:
	CGumpDrag(uint serial, short x, short y);
	virtual ~CGumpDrag();

	void UpdateContent();

	GUMP_BUTTON_EVENT_H;
	GUMP_SLIDER_CLICK_EVENT_H;
	GUMP_SLIDER_MOVE_EVENT_H;
	virtual void OnCharPress(const WPARAM &wParam, const LPARAM &lParam);
	virtual void OnKeyDown(const WPARAM &wParam, const LPARAM &lParam);

	void OnOkayPressed();
};
//----------------------------------------------------------------------------------
#endif
//----------------------------------------------------------------------------------
