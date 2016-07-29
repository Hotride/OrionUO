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
	//!Идентификаторы событий для плавного перехода
	static const BYTE ID_SMOOTH_MS_QUIT = 1;
	static const BYTE ID_SMOOTH_MS_CONNECT = 2;

	//!Идентификаторы кнопок
	static const int ID_MS_QUIT = 1;
	static const int ID_MS_ARROW_NEXT = 2;
	static const int ID_MS_ACCOUNT = 3;
	static const int ID_MS_PASSWORD = 4;
	static const int ID_MS_SAVEPASSWORD = 5;
	static const int ID_MS_AUTOLOGIN = 6;

	enum
	{
		MSCC_ACTID = 1,
		MSCC_ACTPWD,
		MSCC_REMEMBERPWD,
		MSCC_AUTOLOGIN,
		MSCC_SMOOTHMONITOR_SCALE,
		MSCC_SMOOTHMONITOR
	};

	/*!
	Шифрование пароля для сохранения в конфиг
	@param [__in] buf Не зашифрованный пароль
	@param [__in] len Длина пароля
	@return Зашифрованный пароль
	*/
	string CryptPW(__in const char *buf, __in int len);

	/*!
	Расшифровка пароля
	@param [__in] buf Зашифрованный пароль
	@param [__in] len Длина пароля
	@return Расшифрованный пароль
	*/
	string DecryptPW(__in const char *buf, __in int len);

	/*!
	Получить код конфига по ключу
	@param [__in] key Ключ
	@return Код конфига
	*/
	int GetConfigKeyCode(__in string &key);

public:
	TMainScreen();
	virtual ~TMainScreen();

	//!Поля для ввода аккаунта и пароля
	TEntryText *m_Account;
	TEntryText *m_Password;
	TEntryText *m_PasswordFake;
	TGumpInterfaceButton *m_Arrow;
	TGumpInterfaceCheckbox *m_SavePassword;
	TGumpInterfaceCheckbox *m_AutoLogin;

	/*!
	Обработка события после плавного затемнения экрана
	@param [__in_opt] action Идентификатор действия
	@return 
	*/
	void ProcessSmoothAction(__in_opt BYTE action = 0xFF);

	/*!
	Загрузка конфига
	@return 
	*/
	void LoadGlobalConfig();

	/*!
	Сохранение конфига
	@return 
	*/
	void SaveGlobalConfig();

	/*!
	Инициализация
	@return 
	*/
	void Init();

	/*!
	Инициализация всплывающих подсказок
	@return 
	*/
	void InitToolTip();

	/*!
	Отрисовка/выбор объектов
	@param [__in] mode true - отрисовка, false - выбор
	@return При выборе объектов - идентификатор выбранного объекта
	*/
	int Render(__in bool mode);



	/*!
	Нажатие левой кнопки мыши
	@return 
	*/
	void OnLeftMouseDown();

	/*!
	Отпускание левой кнопки мыши
	@return 
	*/
	void OnLeftMouseUp();

	/*!
	Обработка нажатия клавиши
	@param [__in] wparam не подписанный параметр
	@param [__in] lparam не подписанный параметр
	@return 
	*/
	void OnCharPress(__in WPARAM wparam, __in LPARAM lparam);

	/*!
	Обработка нажатия клавиши
	@param [__in] wparam не подписанный параметр
	@param [__in] lparam не подписанный параметр
	@return 
	*/
	void OnKeyPress(__in WPARAM wparam, __in LPARAM lparam);
 };
//---------------------------------------------------------------------------
extern TMainScreen *MainScreen;
//---------------------------------------------------------------------------
#endif