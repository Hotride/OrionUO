/***********************************************************************************
**
** GumpJournal.h
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#ifndef GUMPJOURNAL_H
#define GUMPJOURNAL_H
//----------------------------------------------------------------------------------
#include "GumpBaseScroll.h"
//----------------------------------------------------------------------------------
class CGumpJournal : public CGumpBaseScroll
{
private:
	static const int ID_GJ_SHOW_SYSTEM = 1;
	static const int ID_GJ_SHOW_OBJECTS = 2;
	static const int ID_GJ_SHOW_CLIENT = 3;
	static const int ID_GJ_BUTTON_LOCK = 4;
	static const int ID_GJ_LOCK_MOVING = 5;

	CGUIGumppic *m_BottomLine;
	CGUIButton *m_TextLocker;
	CGUICheckbox *m_CheckboxShowSystem;
	CGUICheckbox *m_CheckboxShowObjects;
	CGUICheckbox *m_CheckboxShowClient;
	CGUIText *m_TextShowSystem;
	CGUIText *m_TextShowObjects;
	CGUIText *m_TextShowClient;

protected:
	virtual void UpdateHeight();

	int RecalculateHeight();

public:
	CGumpJournal(uint serial, short x, short y, bool minimized, int height = 250);
	virtual ~CGumpJournal();

	void AddText(class CTextData *obj);
	void DeleteText(class CTextData *obj);

	virtual void InitToolTip();

	GUMP_BUTTON_EVENT_H;
	GUMP_CHECKBOX_EVENT_H;
	GUMP_SLIDER_CLICK_EVENT_H { m_TextLocker->Visible = true; }
	GUMP_SLIDER_MOVE_EVENT_H { m_TextLocker->Visible = true; }
	GUMP_SCROLL_BUTTON_EVENT_H { m_TextLocker->Visible = true; }

	bool OnLeftMouseButtonDoubleClick();
};
//----------------------------------------------------------------------------------
#endif
//----------------------------------------------------------------------------------
