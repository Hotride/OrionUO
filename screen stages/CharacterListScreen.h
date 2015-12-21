/****************************************************************************
**
** CharacterListGump.h
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
#ifndef CharacterListScreenH
#define CharacterListScreenH
//---------------------------------------------------------------------------
class TCharacterListScreen : public TBaseScreen
{
private:
	TTextTexture m_Text;

	static const int ID_CS_QUIT = 1;
	static const int ID_CS_ARROW_PREV = 2;
	static const int ID_CS_ARROW_NEXT = 3;
	static const int ID_CS_NEW = 4;
	static const int ID_CS_DELETE = 5;
	static const int ID_CS_CHARACTERS = 6;

public:
	TCharacterListScreen();
	virtual ~TCharacterListScreen();
	
	void Init();

	void InitTooltip();

	int Render(bool mode);

	void OnLeftMouseDown();
	void OnLeftMouseUp();
	bool OnLeftMouseDoubleClick();
};

extern TCharacterListScreen *CharacterListScreen;
//---------------------------------------------------------------------------
#endif