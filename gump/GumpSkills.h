/****************************************************************************
**
** GumpSkills.h
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
#ifndef GumpSkillsH
#define GumpSkillsH
//---------------------------------------------------------------------------
class TGumpSkills : public TGump
{
private:
	static const int ID_GS_BUTTON_MINIMIZE = 1;
	static const int ID_GS_BUTTON_RESIZE = 2;
	static const int ID_GS_SHOW_REAL = 3;
	static const int ID_GS_SHOW_CAP = 4;
	static const int ID_GS_BUTTON_NEW_GROUP = 5;
	static const int ID_GS_BUTTON_UP = 6;
	static const int ID_GS_BUTTON_DOWN = 7;
	static const int ID_GS_SCROLLER = 8;
	static const int ID_GS_LOCK_MOVING = 9;
	static const int ID_GS_GROUP_MINIMIZE = 10000;
	static const int ID_GS_GROUP = 11000;
	static const int ID_GS_SKILL_BUTTON = 12000;
	static const int ID_GS_SKILL = 13000;
	static const int ID_GS_SKILL_STATE = 14000;

	DWORD m_LastChangedLineTime;

	short m_Height;
	int m_CurrentLine;

	short m_LastResizeY;
	bool m_ShowReal;
	bool m_ShowCap;

	TTextTexture m_Text;

	//Получить группу над курсором
	TSkillGroupObject *GetGroupUnderCursor(int StartIndex);
public:
	TGumpSkills(DWORD serial, short x, short y, bool minimized);
	virtual ~TGumpSkills();
	
	void Init();
	void ChangeHeight();
	
	TEntryText *TextEntry;

	void PrepareTextures();
	
	void SetHeight(short val);
	
	short GetHeight() const {return m_Height;}

	bool GetResizing() {return (m_LastResizeY != 0);}
	
	int Draw(bool &mode);

	void GenerateFrame();

	void OnLeftMouseDown();
	void OnLeftMouseUp();
	bool OnLeftMouseDoubleClick();
	void OnMouseWheel(MOUSE_WHEEL_STATE &state);
	void OnCharPress(WPARAM &wparam, LPARAM &lparam);
	void OnKeyPress(WPARAM &wparam, LPARAM &lparam);
};
//---------------------------------------------------------------------------
#endif