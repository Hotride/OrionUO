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
	//Флаг сохранения пароля
	bool m_SavePassword;

	//Флаг автологина
	bool m_AutoLogin;

	//Идентификаторы событий для плавного перехода
	static const BYTE ID_SMOOTH_MS_QUIT = 1;
	static const BYTE ID_SMOOTH_MS_CONNECT = 2;

	//Идентификаторы кнопок
	static const int ID_MS_QUIT = 1;
	static const int ID_MS_ARROW_NEXT = 2;
	static const int ID_MS_ACCOUNT = 3;
	static const int ID_MS_PASSWORD = 4;
	static const int ID_MS_SAVEPASSWORD = 5;
	static const int ID_MS_AUTOLOGIN = 6;

	//Объекты текста
	TTextTexture m_Text[6];

	//Шифрование пароля для сохранения в конфиг
	string CryptPW(const char *buf, int len);

	//Расшифровка пароля
	string DecryptPW(const char *buf, int len);

	//Обработка события после плавного затемнения экрана
	void ProcessSmoothAction(BYTE action = 0xFF);
public:
	TMainScreen();
	virtual ~TMainScreen();

	SETGET(bool, SavePassword);
	SETGET(bool, AutoLogin);

	//Загрузка конфига
	void LoadGlobalConfig();

	//Сохранение конфига
	void SaveGlobalConfig();

	//Инициализация
	void Init();

	//Инициализация тултипа
	void InitTooltip();

	//Коля для ввода аккаунта и пароля
	TEntryText *m_Account;
	TEntryText *m_Password;

	//Рисование экрана
	int Render(bool mode);

	//События
	void OnLeftMouseDown();
	void OnLeftMouseUp();
	void OnCharPress(WPARAM wparam, LPARAM lparam);
	void OnKeyPress(WPARAM wparam, LPARAM lparam);
};
//---------------------------------------------------------------------------
extern TMainScreen *MainScreen;
//---------------------------------------------------------------------------
#endif