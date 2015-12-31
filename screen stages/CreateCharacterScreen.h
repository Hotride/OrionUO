/****************************************************************************
**
** CreateCharacterGump.h
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
#ifndef CreateCharacterScreenH
#define CreateCharacterScreenH
//---------------------------------------------------------------------------
class TCreateCharacterScreen : public TBaseScreen
{
private:
public:
	TCreateCharacterScreen();
	virtual ~TCreateCharacterScreen();
	
	void Init();

	void ProcessSmoothAction(BYTE action = 0xFF);

	void InitTooltip();

	int Render(bool mode);
	
	void OnLeftMouseDown();
	void OnLeftMouseUp();
	void OnCharPress(WPARAM wparam, LPARAM lparam);
	void OnKeyPress(WPARAM wparam, LPARAM lparam);
};

extern TCreateCharacterScreen *CreateCharacterScreen;
//---------------------------------------------------------------------------
#endif