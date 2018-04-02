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
class CGumpBook : public CGump
{
	SETGET(bool, Writable, false);
	SETGET(short, PageCount, 0);
	SETGET(bool, Unicode, false);

private:
	static const int ID_GB_TEXT_AREA_AUTHOR = 1;
	static const int ID_GB_TEXT_AREA_TITLE = 2;
	static const int ID_GB_TEXT_AREA_PAGE_LEFT = 3;
	static const int ID_GB_TEXT_AREA_PAGE_RIGHT = 4;
	static const int ID_GB_BUTTON_PREV = 5;
	static const int ID_GB_BUTTON_NEXT = 6;

	bool *m_ChangedPage{ NULL };
	bool *m_PageDataReceived{ NULL };

	void InsertInContent(const WPARAM &wparam, const bool &isCharPress = true);

	CGUIButton *m_PrevPage{ NULL };
	CGUIButton *m_NextPage{ NULL };

	void SetPagePos(int val, int page);

public:
	CGumpBook(uint serial, short x, short y, short pageCount, bool writable, bool unicode);
	virtual ~CGumpBook();

	virtual void PasteClipboardData(wstring &data) override;

	CGUITextEntry *m_EntryAuthor{ NULL };
	CGUITextEntry *m_EntryTitle{ NULL };

	CGUITextEntry *GetEntry(const int &page);

	virtual void DelayedClick(CRenderObject *obj);

	virtual void PrepareContent();

	void SetPageData(const int &page, const wstring &data);

	void ChangePage(int newPage, bool playSound = true);

	GUMP_BUTTON_EVENT_H;

	bool OnLeftMouseButtonDoubleClick();

	void OnCharPress(const WPARAM &wParam, const LPARAM &lParam);
	void OnKeyDown(const WPARAM &wParam, const LPARAM &lParam);
};
//----------------------------------------------------------------------------------
#endif
//----------------------------------------------------------------------------------
