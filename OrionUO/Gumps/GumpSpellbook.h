/***********************************************************************************
**
** GumpSpellbook.h
**
** Copyright (C) September 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#ifndef GUMPSPELLBOOK_H
#define GUMPSPELLBOOK_H
//----------------------------------------------------------------------------------
#include "Gump.h"
//----------------------------------------------------------------------------------
class CGumpSpellbook : public CGump
{
	SETGET(SPELLBOOK_TYPE, BookType);

private:
	static const int ID_GSB_BUTTON_PREV = 1;
	static const int ID_GSB_BUTTON_NEXT = 2;
	static const int ID_GSB_BUTTON_CIRCLE_1_2 = 3;
	static const int ID_GSB_BUTTON_CIRCLE_3_4 = 4;
	static const int ID_GSB_BUTTON_CIRCLE_5_6 = 5;
	static const int ID_GSB_BUTTON_CIRCLE_7_8 = 6;
	static const int ID_GSB_BUTTON_MINIMIZE = 7;
	static const int ID_GSB_LOCK_MOVING = 8;
	static const int ID_GSB_SPELL_ICON_LEFT = 100;
	static const int ID_GSB_SPELL_ICON_RIGHT = 1000;

	static ushort m_ReagentsIndex[8];

	int GetReagentIndex(const ushort &id);

	int m_SpellCount;
	uchar m_Spells[64];
	int m_PageCount;

	CGUIGumppic *m_Body;

	CGUIButton *m_PrevPage;
	CGUIButton *m_NextPage;

	CGUIGumppic *m_LastSpellPointer;
	CGUIGumppic *m_LastSpellBookmark;

	virtual void UpdateContentMage();
	virtual void UpdateContentNecro();
	virtual void UpdateContentPaladin();
	virtual void UpdateContentBushido();
	virtual void UpdateContentNinjitsu();
	virtual void UpdateContentSpellWeaving();
	virtual void UpdateContentMysticism();

public:
	CGumpSpellbook(uint serial, int x, int y);
	virtual ~CGumpSpellbook();

	void UpdateGraphic(const ushort &parentGraphic);

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
