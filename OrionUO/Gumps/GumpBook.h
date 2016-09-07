/***********************************************************************************
**
** GumpBook.h
**
** Copyright (C) September 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#ifndef GUMPBOOK_H
#define GUMPBOOK_H
//----------------------------------------------------------------------------------
#include "Gump.h"
//----------------------------------------------------------------------------------
class CGumpBook : public CGump
{
	SETGET(bool, Writable);
	SETGET(short, PageCount);
	SETGET(bool, Unicode);

private:
	static const int ID_GB_TEXT_AREA_AUTHOR = 1;
	static const int ID_GB_TEXT_AREA_TITLE = 2;
	static const int ID_GB_TEXT_AREA_PAGE_LEFT = 3;
	static const int ID_GB_TEXT_AREA_PAGE_RIGHT = 4;
	static const int ID_GB_BUTTON_PREV = 5;
	static const int ID_GB_BUTTON_NEXT = 6;

	bool *m_ChangedPage;

	void InsertInContent(const WPARAM &wparam, const bool &isCharPress = true);

	CGUIButton *m_PrevPage;
	CGUIButton *m_NextPage;

public:
	CGumpBook(uint serial, short x, short y, short pageCount, bool writable, bool unicode);
	virtual ~CGumpBook();

	CGUITextEntry *m_EntryAuthor;
	CGUITextEntry *m_EntryTitle;

	CGUITextEntry *GetEntry(const int &page);

	virtual void DelayedClick(CRenderObject *obj);

	void SetPageData(const int &page, const string &data);
	void SetPageData(const int &page, const wstring &data);

	void ChangePage(int newPage);

	GUMP_BUTTON_EVENT_H;

	bool OnLeftMouseButtonDoubleClick();

	void OnCharPress(const WPARAM &wParam, const LPARAM &lParam);
	void OnKeyDown(const WPARAM &wParam, const LPARAM &lParam);
};
//----------------------------------------------------------------------------------
#endif
//----------------------------------------------------------------------------------
