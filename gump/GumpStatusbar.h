/****************************************************************************
**
** GumpStatusbar.h
**
** Copyright (C) September 2015 Hotride
**
** This program is free software; you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation; either version 2 of the License, or
** (at your option) any later version.
**
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**
*****************************************************************************
*/
//---------------------------------------------------------------------------
#ifndef GumpStatusbarH
#define GumpStatusbarH
//---------------------------------------------------------------------------
class TGumpStatusbar : public TGump
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
	void CalculateGumpState();

public:
	TGumpStatusbar(DWORD serial, short x, short y, bool minimized);
	virtual ~TGumpStatusbar();

	static const int ID_GSB_LOCK_MOVING = 10;

	TEntryText *TextEntry;
	
	TGumpStatusbar *m_GroupNext;
	TGumpStatusbar *m_GroupPrev;

	TGumpStatusbar *GetTopStatusbar();
	TGumpStatusbar *GetNearStatusbar(int &x, int &y);
	bool GetStatusbarGroupOffset(int &x, int &y);
	void UpdateGroup(int x, int y);
	void AddStatusbar(TGumpStatusbar *bar);
	void RemoveFromGroup();

	bool InGroup() {return (m_GroupNext != NULL || m_GroupPrev != NULL);}

	void PrepareTextures();

	int Draw(bool &mode);

	void GenerateFrame();
	
	void OnLeftMouseDown();
	void OnLeftMouseUp();
	bool OnLeftMouseDoubleClick();
	void OnCharPress(WPARAM &wparam, LPARAM &lparam);
	void OnKeyPress(WPARAM &wparam, LPARAM &lparam);

	bool EntryPointerHere() {return (EntryPointer == TextEntry);}
};
//---------------------------------------------------------------------------
#endif