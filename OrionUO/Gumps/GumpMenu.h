/***********************************************************************************
**
** GumpMenu.h
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#ifndef GUMPMENU_H
#define GUMPMENU_H
//----------------------------------------------------------------------------------
#include "Gump.h"
//----------------------------------------------------------------------------------
class CGumpMenu : public CGump
{
	SETGET(string, Text);
	SETGET(bool, TextChanged);

protected:
	virtual void CalculateGumpState();

public:
	CGumpMenu(uint serial, uint id, short x, short y);
	virtual ~CGumpMenu();

	static const int ID_GM_HTMLGUMP = 1000;

	CGUIText *m_TextObject;

	void SendMenuResponse(int index);
	
	virtual void PrepareContent();

	virtual bool OnLeftMouseButtonDoubleClick();
};
//----------------------------------------------------------------------------------
#endif
//----------------------------------------------------------------------------------
