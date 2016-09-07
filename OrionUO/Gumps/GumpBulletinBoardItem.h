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
#include "GumpBaseScroll.h"
//----------------------------------------------------------------------------------
class CGumpBulletinBoardItem : public CGumpBaseScroll
{
protected:
	static const int ID_GBBI_SUBJECT_TEXT_FIELD = 1;
	static const int ID_GBBI_TEXT_FIELD = 2;
	static const int ID_GBBI_POST = 3;
	static const int ID_GBBI_REPLY = 4;
	static const int ID_GBBI_REMOVE = 5;

	uchar m_Variant;

	CGUIButton *m_ButtonPost;
	CGUIButton *m_ButtonRemove;
	CGUIButton *m_ButtonReply;

	CGUIHitBox *m_HitBox;

	void RecalculateHeight();

protected:
	virtual void UpdateHeight();

public:
	CGumpBulletinBoardItem(uint serial, short x, short y, uchar variant, uint id, string poster, string subject, string dataTime, string data);
	virtual ~CGumpBulletinBoardItem();

	CGUITextEntry *m_EntrySubject;
	CGUITextEntry *m_Entry;

	GUMP_BUTTON_EVENT_H;

	virtual void OnCharPress(const WPARAM &wParam, const LPARAM &lParam);
	virtual void OnKeyDown(const WPARAM &wParam, const LPARAM &lParam);
};
//----------------------------------------------------------------------------------
#endif
//----------------------------------------------------------------------------------
