/***********************************************************************************
**
** GumpBuff.h
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#ifndef GUMPBUFF_H
#define GUMPBUFF_H
//----------------------------------------------------------------------------------
class CGumpBuff : public CGump
{
private:
	static const int BUFF_ITEM_STEP_OFFSET_X = 3;
	static const int BUFF_ITEM_STEP_OFFSET_Y = 3;
	static const int MUNIMUM_ICON_ALPHA = 80;
	static const int ALPHA_CHANGE_KOEFF = 600;
	static const int USE_ALPHA_BLENDING_WHEN_TIMER_LESS = 10000;

	static const int ID_GB_NEXT_WINDOW_DIRECTION = 1;
	static const int ID_GB_LOCK_MOVING = 2;
	static const int ID_GB_BUFF_ITEM = 10;

	void GetGumpStatus(WISP_GEOMETRY::CPoint2Di &ball, WISP_GEOMETRY::CPoint2Di &items, bool &useX, bool &decX, bool &decY, WISP_GEOMETRY::CPoint2Di &startGump, WISP_GEOMETRY::CSize &endGump);

public:
	CGumpBuff(short x, short y);
	virtual ~CGumpBuff();

	virtual bool CanBeDisplayed();

	void AddBuff(const ushort &id, const ushort &timer, const wstring &text);

	void DeleteBuff(const ushort &id);

	void UpdateBuffIcons();

	virtual void InitToolTip();

	virtual void PrepareContent();

	virtual void UpdateContent();

	GUMP_BUTTON_EVENT_H;
};
//----------------------------------------------------------------------------------
#endif
//----------------------------------------------------------------------------------
