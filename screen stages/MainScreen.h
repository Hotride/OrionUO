/****************************************************************************
**
** MainScreen.h
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
#ifndef MainScreenH
#define MainScreenH
//---------------------------------------------------------------------------
class TMainScreen : public TBaseScreen
{
private:
	bool m_SavePassword;
	bool m_AutoLogin;

	static const int ID_MS_QUIT = 1;
	static const int ID_MS_ARROW_NEXT = 2;
	static const int ID_MS_ACCOUNT = 3;
	static const int ID_MS_PASSWORD = 4;
	static const int ID_MS_SAVEPASSWORD = 5;
	static const int ID_MS_AUTOLOGIN = 6;

	TTextTexture m_Text[6];

	string CryptPW(const char *buf, int len);
	string DecryptPW(const char *buf, int len);

public:
	TMainScreen();
	virtual ~TMainScreen();

	SETGET(bool, SavePassword)
	SETGET(bool, AutoLogin)
	
	void LoadGlobalConfig();
	void SaveGlobalConfig();

	void Init();

	void InitTooltip();

	TEntryText *m_Account;
	TEntryText *m_Password;

	int Render(bool mode);
	
	void OnLeftMouseDown();
	void OnLeftMouseUp();
	void OnCharPress(WPARAM wparam, LPARAM lparam);
	void OnKeyPress(WPARAM wparam, LPARAM lparam);
};

extern TMainScreen *MainScreen;
//---------------------------------------------------------------------------
#endif