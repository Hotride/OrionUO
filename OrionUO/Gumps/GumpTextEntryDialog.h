/***********************************************************************************
**
** GumpTextEntryDialog.h
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#ifndef GUMPTEXTENTRYDIALOG_H
#define GUMPTEXTENTRYDIALOG_H
//----------------------------------------------------------------------------------
#include "Gump.h"
//----------------------------------------------------------------------------------
class CGumpTextEntryDialog : public CGump
{
	SETGET(uchar, Variant);
	SETGET(uchar, ParentID);
	SETGET(uchar, ButtonID);

private:
	static const int ID_GTED_BUTTON_OKAY = 1;
	static const int ID_GTED_BUTTON_CANCEL = 2;
	static const int ID_GTED_TEXT_FIELD = 3;

	string m_Text;
	string m_Description;
	int m_MaxLength;

	CGUIGumppic *m_TextField;
	CGUITextEntry *m_Entry;

public:
	CGumpTextEntryDialog(uint serial, short x, short y, uchar variant, int maxLength, string text, string description);
	virtual ~CGumpTextEntryDialog();

	void SendTextEntryDialogResponse(bool mode);

	virtual void PrepareContent();

	virtual void UpdateContent();



	GUMP_BUTTON_EVENT_H;

	void OnCharPress(const WPARAM &wParam, const LPARAM &lParam);
	void OnKeyDown(const WPARAM &wParam, const LPARAM &lParam);
};
//----------------------------------------------------------------------------------
#endif
//----------------------------------------------------------------------------------
