/***********************************************************************************
**
** GumpCombatBook.h
**
** Copyright (C) December 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#ifndef GUMPCOMBATBOOK_H
#define GUMPCOMBATBOOK_H
//----------------------------------------------------------------------------------
class CGumpCombatBook : public CGump
{
	int DictionaryPagesCount = 6;
	int AbilityCount = MAX_ABILITIES_COUNT;
	int PagesCount = 37;

private:
	static const int ID_GCB_BUTTON_PREV = 1;
	static const int ID_GCB_BUTTON_NEXT = 2;
	static const int ID_GCB_BUTTON_MINIMIZE = 3;
	static const int ID_GCB_LOCK_MOVING = 4;
	static const int ID_GCB_ICON_FIRST = 5;
	static const int ID_GCB_ICON_SECOND = 6;
	static const int ID_GCB_ICON = 100;

	static const string m_AbilityName[MAX_ABILITIES_COUNT];

	CGUIButton *m_PrevPage{ NULL };
	CGUIButton *m_NextPage{ NULL };

public:
	CGumpCombatBook(const int &x, const int &y);
	virtual ~CGumpCombatBook();

	static USHORT_LIST GetItemsList(const uchar &index);

	virtual void DelayedClick(CRenderObject *obj);

	void ChangePage(int newPage);

	virtual void InitToolTip();

	virtual void PrepareContent();

	virtual void UpdateContent();

	GUMP_BUTTON_EVENT_H;

	virtual bool OnLeftMouseButtonDoubleClick();
};
//----------------------------------------------------------------------------------
#endif
//----------------------------------------------------------------------------------
