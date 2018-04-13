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
class CGumpTextEntryDialog : public CGump
{
public:
	uchar Variant = 0;
	uchar ParentID = 0;
	uchar ButtonID = 0;

private:
	static const int ID_GTED_BUTTON_OKAY = 1;
	static const int ID_GTED_BUTTON_CANCEL = 2;
	static const int ID_GTED_TEXT_FIELD = 3;

	string Text = string("");
	string m_Description = string("");
	int m_MaxLength{ 0 };

	CGUIGumppic *m_TextField{ NULL };
	CGUITextEntry *m_Entry{ NULL };

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
