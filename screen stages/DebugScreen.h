/****************************************************************************
**
** DebugScreen.h
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
#ifndef DebugScreenH
#define DebugScreenH
//---------------------------------------------------------------------------
class TDebugScreen : public TBaseScreen
{
private:
	static const BYTE ID_SMOOTH_DS_QUIT = 1;
	static const BYTE ID_SMOOTH_DS_GO_SCREEN_MAIN = 2;

	static const int ID_DS_QUIT = 1;
	static const int ID_DS_GO_SCREEN_MAIN = 2;

	void ProcessSmoothAction(BYTE action = 0xFF);

	TGameItem *m_Chair;
	TGameCharacter *m_Player;
	TGameItem *m_Mount;
	TGameItem *m_Cloak;

public:
	TDebugScreen();
	virtual ~TDebugScreen();

	void Init();

	void InitToolTip();

	int Render(bool mode);

	void OnLeftMouseUp();
	void OnCharPress(WPARAM wparam, LPARAM lparam);
	void OnKeyPress(WPARAM wparam, LPARAM lparam);
};
//---------------------------------------------------------------------------
extern TDebugScreen *DebugScreen;
//---------------------------------------------------------------------------
#endif