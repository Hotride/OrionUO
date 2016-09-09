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
#include "Gump.h"
//----------------------------------------------------------------------------------
class CGumpMenubar : public CGump
{
	SETGETE(bool, Opened, OnChangeOpened);

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
	CGumpMenubar(uint serial, short x, short y);
	virtual ~CGumpMenubar();
	
	bool CanBeDisplayed() { return false; }

	void OnButton(const uint &serial);
	void OnTextEntry(const uint &serial);

	virtual void OnLeftMouseButtonUp();
};
//----------------------------------------------------------------------------------
#endif
//----------------------------------------------------------------------------------
