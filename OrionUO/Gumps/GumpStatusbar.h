/***********************************************************************************
**
** GumpStatusbar.h
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#ifndef GUMPSTATUSBAR_H
#define GUMPSTATUSBAR_H
//----------------------------------------------------------------------------------
#include "Gump.h"
//----------------------------------------------------------------------------------
class CGumpStatusbar : public CGump
{
private:
	static const int ID_GSB_MINIMIZE = 1;
	static const int ID_GSB_TEXT_FIELD = 2;
	static const int ID_GSB_BUTTON_HEAL_1 = 3;
	static const int ID_GSB_BUTTON_HEAL_2 = 4;
	static const int ID_GSB_BUTTON_REMOVE_FROM_GROUP = 5;
	static const int ID_GSB_BUFF_GUMP = 6;
	static const int ID_GSB_BUFF_LOCKER_STR = 7;
	static const int ID_GSB_BUFF_LOCKER_DEX = 8;
	static const int ID_GSB_BUFF_LOCKER_INT = 9;
	static const int ID_GSB_TEXT_STR = 11;
	static const int ID_GSB_TEXT_DEX = 12;
	static const int ID_GSB_TEXT_INT = 13;
	static const int ID_GSB_TEXT_SEX = 14;
	static const int ID_GSB_TEXT_ARMOR = 15;
	static const int ID_GSB_TEXT_HITS = 16;
	static const int ID_GSB_TEXT_STAM = 17;
	static const int ID_GSB_TEXT_MANA = 18;
	static const int ID_GSB_TEXT_MAX_STATS = 19;
	static const int ID_GSB_TEXT_LUCK = 20;
	static const int ID_GSB_TEXT_WEIGHT = 21;
	static const int ID_GSB_TEXT_DAMAGE = 22;
	static const int ID_GSB_TEXT_GOLD = 23;
	static const int ID_GSB_TEXT_FOLLOWERS = 24;
	static const int ID_GSB_TEXT_RESISTANCE_PHYSICAL = 25;
	static const int ID_GSB_TEXT_RESISTANCE_FIRE = 26;
	static const int ID_GSB_TEXT_RESISTANCE_COLD = 27;
	static const int ID_GSB_TEXT_RESISTANCE_POISON = 28;
	static const int ID_GSB_TEXT_RESISTANCE_ENERGY = 29;
	static const int ID_GSB_TEXT_TITHING_POINTS = 30;

protected:
	void SendRenameRequest();

	virtual void CalculateGumpState();

	bool m_WantFullUpdateContent;

	CGUIButton *m_StatusbarUnlocker;
	CGUIGumppic *m_Body;
	CGUIGumppic *m_HitsBody;
	CGUITextEntry *m_Entry;

	string m_Name;

public:
	CGumpStatusbar(uint serial, short x, short y, bool minimized);
	virtual ~CGumpStatusbar();

	static const int ID_GSB_LOCK_MOVING = 10;

	static int m_StatusbarDefaultWidth;
	static int m_StatusbarDefaultHeight;

	CGumpStatusbar *m_GroupNext;
	CGumpStatusbar *m_GroupPrev;

	CGumpStatusbar *GetTopStatusbar();
	CGumpStatusbar *GetNearStatusbar(int &x, int &y);
	bool GetStatusbarGroupOffset(int &x, int &y);
	void UpdateGroup(int x, int y);
	void AddStatusbar(CGumpStatusbar *bar);
	void RemoveFromGroup();

	bool InGroup() { return (m_GroupNext != NULL || m_GroupPrev != NULL); }

	virtual void InitToolTip();

	virtual void PrepareContent();

	virtual void UpdateContent();

	GUMP_BUTTON_EVENT_H;

	virtual void OnLeftMouseButtonDown();
	virtual bool OnLeftMouseButtonDoubleClick();
	virtual void OnCharPress(const WPARAM &wParam, const LPARAM &lParam);
	virtual void OnKeyDown(const WPARAM &wParam, const LPARAM &lParam);
};
//----------------------------------------------------------------------------------
#endif
//----------------------------------------------------------------------------------
