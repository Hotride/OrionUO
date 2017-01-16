/***********************************************************************************
**
** MainScreen.h
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#ifndef MAINSCREEN_H
#define MAINSCREEN_H
//----------------------------------------------------------------------------------
#include "BaseScreen.h"
#include "../Gumps/GumpScreenMain.h"
//----------------------------------------------------------------------------------
class CMainScreen : public CBaseScreen
{
private:
	enum
	{
		MSCC_ACTID = 1,
		MSCC_ACTPWD,
		MSCC_REMEMBERPWD,
		MSCC_AUTOLOGIN,
		MSCC_SMOOTHMONITOR,
		MSCC_THE_ABYSS
	};

	/*!
	Шифрование пароля для сохранения в конфиг
	@param [__in] buf Не зашифрованный пароль
	@param [__in] len Длина пароля
	@return Зашифрованный пароль
	*/
	string CryptPW(const char *buf, int len);

	/*!
	Расшифровка пароля
	@param [__in] buf Зашифрованный пароль
	@param [__in] len Длина пароля
	@return Расшифрованный пароль
	*/
	string DecryptPW(const char *buf, int len);

	/*!
	Получить код конфига по ключу
	@param [__in] key Ключ
	@return Код конфига
	*/
	int GetConfigKeyCode(const string &key);

	CGumpScreenMain m_MainGump;

public:
	CMainScreen();
	virtual ~CMainScreen();

	//!Идентификаторы событий для плавного перехода
	static const uchar ID_SMOOTH_MS_QUIT = 1;
	static const uchar ID_SMOOTH_MS_CONNECT = 2;

	//!Поля для ввода аккаунта и пароля
	CEntryText *m_Account;
	CEntryText *m_Password;
	CGUICheckbox *m_SavePassword;
	CGUICheckbox *m_AutoLogin;

	void SetAccounting(const string &account, const string &password);

	void Paste();

	/*!
	Обработка события после плавного затемнения экрана
	@param [__in_opt] action Идентификатор действия
	@return 
	*/
	void ProcessSmoothAction(uchar action = 0xFF);

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



	virtual void OnCharPress(const WPARAM &wParam, const LPARAM &lParam);
	virtual void OnKeyDown(const WPARAM &wParam, const LPARAM &lParam);
 };
 //----------------------------------------------------------------------------------
 extern CMainScreen g_MainScreen;
 //----------------------------------------------------------------------------------
#endif
//----------------------------------------------------------------------------------
