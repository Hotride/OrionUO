/****************************************************************************
**
** GumpBulletinBoard.h
**
** Copyright (C) October 2015 Hotride
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
#ifndef GumpBulletinBoardH
#define GumpBulletinBoardH
//---------------------------------------------------------------------------
//!Класс доски объявлений
class TGumpBulletinBoard : public TGump
{
private:
	//!Идентификаторы кнопок
	static const int ID_GBB_POST_MESSAGE = 1;
	static const int ID_GBB_BUTTON_UP = 2;
	static const int ID_GBB_BUTTON_DOWN = 3;
	static const int ID_GBB_SCROLLBAR = 4;
	static const int ID_GBB_MESSAGE = 5;

	TTextTexture m_Text;
	
	int m_CurrentLine;
	DWORD m_LastScrollChangeTime;

	int GetHeight();

	/*!
	Пролистать список
	@param [__in] direction Направление
	@param [__in_opt] divizor Делитель
	@return
	*/
	void ListingList(__in bool direction, __in_opt int divizor = 1);

public:
	TGumpBulletinBoard(DWORD serial, short x, short y, string name);
	virtual ~TGumpBulletinBoard();

	void PrepareTextures();

	int Draw(bool &mode);

	void GenerateFrame();

	void OnLeftMouseUp();
	bool OnLeftMouseDoubleClick();
	void OnMouseWheel(MOUSE_WHEEL_STATE &state);
};
//---------------------------------------------------------------------------
#endif