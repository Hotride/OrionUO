/***********************************************************************************
**
** MainScreen.h
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#pragma once
#include <SDL_events.h>
//----------------------------------------------------------------------------------
class CMainScreen : public CBaseScreen
{
private:
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

	CGumpScreenMain m_MainGump;

public:
	CMainScreen();
	virtual ~CMainScreen();

	//!Идентификаторы событий для плавного перехода
	static const uchar ID_SMOOTH_MS_QUIT = 1;
	static const uchar ID_SMOOTH_MS_CONNECT = 2;

	//!Поля для ввода аккаунта и пароля
	CEntryText *m_Account = nullptr;
	CEntryText *m_Password = nullptr;
	CGUICheckbox *m_SavePassword = nullptr;
	CGUICheckbox *m_AutoLogin = nullptr;

	void SetAccountName(const string &account);
	void SetPassword(const string &password);
	void SetEncryptedPassword(const string &password);

	string GetEncryptedPassword();

	void Paste();

	/*!
	Обработка события после плавного затемнения экрана
	@param [__in_opt] action Идентификатор действия
	@return
	*/
	void ProcessSmoothAction(uchar action = 0xFF);

	/*!
	Инициализация
	@return
	*/
	void Init();

#if USE_WISP
	virtual void OnCharPress(const WPARAM &wParam, const LPARAM &lParam);
	virtual void OnKeyDown(const WPARAM &wParam, const LPARAM &lParam);
#else
	virtual void OnTextInput(const SDL_TextInputEvent &ev) override;
	virtual void OnKeyDown(const SDL_KeyboardEvent &ev) override;
#endif

 };
 //----------------------------------------------------------------------------------
 extern CMainScreen g_MainScreen;
