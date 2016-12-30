/***********************************************************************************
**
** GumpRacialAbilitiesBook.h
**
** Copyright (C) December 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#ifndef GUMPRACIALABILITIESBOOK_H
#define GUMPRACIALABILITIESBOOK_H
//----------------------------------------------------------------------------------
#include "Gump.h"
/*
graphic = 0x2B02;
minimizedGraphic = 0x2B05;
iconStartGraphic = 0x5200;
*/
//----------------------------------------------------------------------------------
class CGumpRacialAbilitiesBook : public CGump
{
	SETGET(int, DictionaryPagesCount);
	SETGET(int, AbilityCount);
	SETGET(int, PagesCount);

private:
	static const int ID_GCB_BUTTON_PREV = 1;
	static const int ID_GCB_BUTTON_NEXT = 2;
	static const int ID_GCB_BUTTON_MINIMIZE = 3;
	static const int ID_GCB_LOCK_MOVING = 4;
	static const int ID_GCB_ICON = 100;

	CGUIButton *m_PrevPage;
	CGUIButton *m_NextPage;

public:
	CGumpRacialAbilitiesBook(const int &x, const int &y);
	virtual ~CGumpRacialAbilitiesBook();

	virtual void DelayedClick(CRenderObject *obj);

	void ChangePage(int newPage);

	virtual void InitToolTip();

	virtual void UpdateContent();

	GUMP_BUTTON_EVENT_H;

	virtual bool OnLeftMouseButtonDoubleClick();
};
//----------------------------------------------------------------------------------
#endif
//----------------------------------------------------------------------------------
