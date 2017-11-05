/***********************************************************************************
**
** GumpMenubar.h
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#ifndef GUMPMENUBAR_H
#define GUMPMENUBAR_H
//----------------------------------------------------------------------------------
class CGumpMenubar : public CGump
{
	SETGETE(bool, Opened, true);

private:
	static const int ID_GMB_MINIMIZE = 1;
	static const int ID_GMB_MAP = 2;
	static const int ID_GMB_PAPERDOLL = 3;
	static const int ID_GMB_INVENTORY = 4;
	static const int ID_GMB_JOURNAL = 5;
	static const int ID_GMB_CHAT = 6;
	static const int ID_GMB_HELP = 7;
	static const int ID_GMB_WORLD_MAP = 8;
	static const int ID_GMB_INFO = 9;

public:
	CGumpMenubar(short x, short y);
	virtual ~CGumpMenubar();
	
	virtual bool CanBeDisplayed() { return false; }

	virtual void InitToolTip();

	GUMP_BUTTON_EVENT_H;
	GUMP_TEXT_ENTRY_EVENT_H;

	virtual void OnLeftMouseButtonUp();
};
//----------------------------------------------------------------------------------
#endif
//----------------------------------------------------------------------------------
