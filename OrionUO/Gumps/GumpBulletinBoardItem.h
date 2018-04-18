/***********************************************************************************
**
** GumpBulletinBoardItem.h
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#ifndef GUMPBULLETINBOARDITEM_H
#define GUMPBULLETINBOARDITEM_H
//----------------------------------------------------------------------------------
class CGumpBulletinBoardItem : public CGumpBaseScroll
{
protected:
	static const int ID_GBBI_SUBJECT_TEXT_FIELD = 1;
	static const int ID_GBBI_TEXT_FIELD = 2;
	static const int ID_GBBI_POST = 3;
	static const int ID_GBBI_REPLY = 4;
	static const int ID_GBBI_REMOVE = 5;

	uchar m_Variant{ 0 };

	CGUIButton *m_ButtonPost{ NULL };
	CGUIButton *m_ButtonRemove{ NULL };
	CGUIButton *m_ButtonReply{ NULL };

	CGUIHitBox *m_HitBox{ NULL };

	void RecalculateHeight();

protected:
	virtual void UpdateHeight();

public:
	CGumpBulletinBoardItem(int serial, int x, int y, uchar variant, int id, const wstring &poster, const wstring &subject, const wstring &dataTime, const wstring &data);
	virtual ~CGumpBulletinBoardItem();

	CGUITextEntry *m_EntrySubject{ NULL };
	CGUITextEntry *m_Entry{ NULL };

	GUMP_BUTTON_EVENT_H;

	virtual void OnCharPress(const WPARAM &wParam, const LPARAM &lParam);
	virtual void OnKeyDown(const WPARAM &wParam, const LPARAM &lParam);
};
//----------------------------------------------------------------------------------
#endif
//----------------------------------------------------------------------------------
