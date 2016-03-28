/****************************************************************************
**
** TextEntryDialogGump.h
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
#ifndef GumpTextEntryDialogH
#define GumpTextEntryDialogH
//---------------------------------------------------------------------------
class TGumpTextEntryDialog : public TGump
{
private:
	static const int ID_GTED_BUTTON_OKAY = 1;
	static const int ID_GTED_BUTTON_CANCEL = 2;
	static const int ID_GTED_TEXT_FIELD = 3;

	void MoveItems();
	string m_Text;
	string m_Description;
	BYTE m_Variant;
	int m_MaxLength;
	BYTE m_ParentID;
	BYTE m_ButtonID;
public:
	TGumpTextEntryDialog(DWORD serial, short x, short y, BYTE variant, int maxLength, string text, string description);
	virtual ~TGumpTextEntryDialog();
	
	TEntryText *TextEntry;
	
	SETGET(BYTE, Variant);
	SETGET(BYTE, ParentID);
	SETGET(BYTE, ButtonID);

	void PrepareTextures();

	int Draw(bool &mode);

	void GenerateFrame();

	void OnLeftMouseUp();
	void OnCharPress(WPARAM &wparam, LPARAM &lparam);
	void OnKeyPress(WPARAM &wparam, LPARAM &lparam);

	bool EntryPointerHere() {return (EntryPointer == TextEntry);}
};
//---------------------------------------------------------------------------
#endif