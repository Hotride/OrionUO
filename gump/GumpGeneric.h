/****************************************************************************
**
** GumpGeneric.h
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
#ifndef GumpGenericH
#define GumpGenericH
//---------------------------------------------------------------------------
class TGumpGeneric : public TGump
{
protected:
	int m_Page;
	bool m_Transparent;
	
	DWORD m_LastScrollChangeTime;

	static const int ID_GG_SCROLLBAR_BUTTON_UP = 10000;
	static const int ID_GG_SCROLLBAR_BUTTON_DOWN = 20000;
	static const int ID_GG_SCROLLBAR = 30000;
	static const int ID_GG_SCROLLBAR_BACKGROUND = 40000;

	void ListingList(TGumpHTMLGump *htmlGump, bool direction, int divizor = 1);
	void ApplyTransparent(TGumpObject *obj, int page);

	void CalculateGumpState();
public:
	TGumpGeneric(DWORD serial, short x, short y, DWORD id);
	virtual ~TGumpGeneric();
	
	static const int ID_GG_HTML_TEXT = 50000;
	
	bool EntryPointerHere();
	
	void PrepareTextures();

	int Draw(bool &mode);

	void GenerateFrame();
	
	void OnLeftMouseDown();
	void OnLeftMouseUp();
	void OnMouseWheel(MOUSE_WHEEL_STATE &state);
	void OnCharPress(WPARAM &wparam, LPARAM &lparam);

	void AddText(int index, wstring text);
};
//---------------------------------------------------------------------------
#endif