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
class CGumpJournal : public CGumpBaseScroll
{
private:
	static const int ID_GJ_SHOW_SYSTEM = 1;
	static const int ID_GJ_SHOW_OBJECTS = 2;
	static const int ID_GJ_SHOW_CLIENT = 3;
	static const int ID_GJ_BUTTON_LOCK = 4;
	static const int ID_GJ_LOCK_MOVING = 5;

	CGUIGumppic *m_BottomLine{ NULL };
	CGUIButton *m_TextLocker{ NULL };
	CGUICheckbox *m_CheckboxShowSystem{ NULL };
	CGUICheckbox *m_CheckboxShowObjects{ NULL };
	CGUICheckbox *m_CheckboxShowClient{ NULL };
	CGUIText *m_TextShowSystem{ NULL };
	CGUIText *m_TextShowObjects{ NULL };
	CGUIText *m_TextShowClient{ NULL };

protected:
	virtual void UpdateHeight();

	int RecalculateHeight();

public:
	CGumpJournal(short x, short y, bool minimized, int height);
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
