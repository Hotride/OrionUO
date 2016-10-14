/***********************************************************************************
**
** GumpNotify.h
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#ifndef GUMPNOTIFY_H
#define GUMPNOTIFY_H
//----------------------------------------------------------------------------------
#include "Gump.h"
//----------------------------------------------------------------------------------
class CGumpNotify : public CGump
{
	SETGET(uchar, Variant);
	SETGET(short, Width);
	SETGET(short, Height);
	SETGET(string, Text);

private:
	static const int ID_GN_BUTTON_OK = 1;

	void Process();

public:
	CGumpNotify(uint serial, short x, short y, uchar variant, short width, short height, string text);
	virtual ~CGumpNotify();

	static const int ID_GN_STATE_LOGOUT = 1;
	static const int ID_GN_STATE_NOTIFICATION = 2;

	GUMP_BUTTON_EVENT_H;
	void OnKeyDown(const WPARAM &wParam, const LPARAM &lParam);
};
//----------------------------------------------------------------------------------
#endif
//----------------------------------------------------------------------------------
