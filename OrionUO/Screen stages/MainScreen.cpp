/***********************************************************************************
**
** MainScreen.cpp
**
** Copyright (C) August 2016 Hotride
**
************************************************************************************
*/
//----------------------------------------------------------------------------------
#include "stdafx.h"
//----------------------------------------------------------------------------------
CMainScreen g_MainScreen;
//----------------------------------------------------------------------------------
CMainScreen::CMainScreen()
: CBaseScreen(m_MainGump), m_Account(NULL), m_Password(NULL), m_SavePassword(NULL),
m_AutoLogin(NULL)
{
	WISPFUN_DEBUG("c165_f1");
	m_Password = new CEntryText(32, 0, 300);
}
//----------------------------------------------------------------------------------
CMainScreen::~CMainScreen()
{
	WISPFUN_DEBUG("c165_f2");
	delete m_Password;
}
//----------------------------------------------------------------------------------
/*!
Инициализация
@return 
*/
void CMainScreen::Init()
{
	WISPFUN_DEBUG("c165_f3");
	g_ConfigLoaded = false;
	g_GlobalScale = 1.0;

	g_OrionWindow.Size = WISP_GEOMETRY::CSize(640, 480);
	g_OrionWindow.NoResize = true;
	g_OrionWindow.SetTitle("Ultima Online");
	g_GL.UpdateRect();

	if (!m_SavePassword->Checked)
	{
		m_Password->SetText(L"");
		m_MainGump.m_PasswordFake->SetText(L"");
	}

	g_EntryPointer = m_MainGump.m_PasswordFake;

	g_AnimationManager.ClearUnusedTextures(g_Ticks + 100000);

	g_QuestArrow.Enabled = false;

	g_TotalSendSize = 0;
	g_TotalRecvSize = 0;

	g_LightLevel = 0;
	g_PersonalLightLevel = 0;

	g_FontManager.SavePixels = false;

	g_ScreenEffectManager.UseSunrise();
	m_SmoothScreenAction = 0;

	m_Gump.PrepareTextures();
}
//----------------------------------------------------------------------------------
/*!
Обработка события после плавного затемнения экрана
@param [__in_opt] action Идентификатор действия
@return 
*/
void CMainScreen::ProcessSmoothAction(uchar action)
{
	WISPFUN_DEBUG("c165_f4");
	if (action == 0xFF)
		action = m_SmoothScreenAction;

	if (action == ID_SMOOTH_MS_CONNECT)
		g_Orion.Connect();
	else if (action == ID_SMOOTH_MS_QUIT)
		g_OrionWindow.Destroy();
}
//----------------------------------------------------------------------------------
void CMainScreen::SetAccounting(const string &account, const string &password)
{
	WISPFUN_DEBUG("c165_f5");
	m_Account->SetText(account);
	m_Password->SetText(password);

	int len = password.length();
	m_MainGump.m_PasswordFake->Clear();

	IFOR(i, 0, len)
		m_MainGump.m_PasswordFake->Insert(L'*');
}
//----------------------------------------------------------------------------------
void CMainScreen::Paste()
{
	WISPFUN_DEBUG("c165_f6");
	if (g_EntryPointer == m_MainGump.m_PasswordFake)
	{
		m_Password->Paste();

		int len = m_Password->Length();
		g_EntryPointer->Clear();

		IFOR(i, 0, len)
			g_EntryPointer->Insert(L'*');
	}
	else
		g_EntryPointer->Paste();
}
//----------------------------------------------------------------------------------
/*!
Обработка нажатия клавиши
@param [__in] wparam не подписанный параметр
@param [__in] lparam не подписанный параметр
@return 
*/
void CMainScreen::OnCharPress(const WPARAM &wParam, const LPARAM &lParam)
{
	WISPFUN_DEBUG("c165_f7");
	if (wParam >= 0x0100 || !g_FontManager.IsPrintASCII(wParam))
		return;
	else if (g_EntryPointer == NULL)
		g_EntryPointer = m_MainGump.m_PasswordFake;

	if (g_EntryPointer->Length() < 16) //add char to text field
	{
		if (g_EntryPointer == m_MainGump.m_PasswordFake)
		{
			if (g_EntryPointer->Insert(L'*'))
				m_Password->Insert(wParam);
		}
		else
			g_EntryPointer->Insert(wParam);
	}

	m_Gump.WantRedraw = true;
}
//----------------------------------------------------------------------------------
/*!
Обработка нажатия клавиши
@param [__in] wparam не подписанный параметр
@param [__in] lparam не подписанный параметр
@return 
*/
void CMainScreen::OnKeyDown(const WPARAM &wParam, const LPARAM &lParam)
{
	WISPFUN_DEBUG("c165_f8");
	if (g_EntryPointer == NULL)
		g_EntryPointer = m_MainGump.m_PasswordFake;

	switch (wParam)
	{
		case VK_TAB:
		{
			if (g_EntryPointer == m_Account)
				g_EntryPointer = m_MainGump.m_PasswordFake;
			else
				g_EntryPointer = m_Account;

			break;
		}
		case VK_RETURN:
		{
			CreateSmoothAction(ID_SMOOTH_MS_CONNECT);

			break;
		}
		default:
		{
			if (g_EntryPointer == m_MainGump.m_PasswordFake)
				m_Password->OnKey(NULL, wParam);

			g_EntryPointer->OnKey(NULL, wParam);

			break;
		}
	}

	m_Gump.WantRedraw = true;
}
//----------------------------------------------------------------------------------
/*!
Получить код конфига по ключу
@param [__in] key Ключ
@return 
*/
int CMainScreen::GetConfigKeyCode(const string &key)
{
	WISPFUN_DEBUG("c165_f9");
	const int keyCount = MSCC_COUNT - 1;

	static const string m_Keys[keyCount] =
	{
		"acctid",
		"acctpassword",
		"rememberacctpw",
		"autologin",
		"smoothmonitor",
		"theabyss",
		"asmut"
	};

	string str = ToLowerA(key);
	int result = 0;

	IFOR(i, 0, keyCount && !result)
	{
		if (str == m_Keys[i])
			result = i + 1;
	}

	return result;
}
//----------------------------------------------------------------------------------
/*!
Загрузка конфига
@return 
*/
void CMainScreen::LoadGlobalConfig()
{
	WISPFUN_DEBUG("c165_f10");
	m_AutoLogin->Checked = false;
	g_ScreenEffectManager.Enabled = false;

	WISP_FILE::CTextFileParser file(g_App.FilePath("uo_debug.cfg"), "=", "#;", "");

	while (!file.IsEOF())
	{
		std::vector<std::string> strings = file.ReadTokens();

		if (strings.size() >= 2)
		{
			int code = GetConfigKeyCode(strings[0]);

			switch (code)
			{
				case MSCC_ACTID:
				{
					m_Account->SetText(strings[1]);
					m_Account->SetPos(strings[1].length());
					
					break;
				}
				case MSCC_ACTPWD:
				{
					string password = file.GetRawLine();
					int pos = password.find_first_of("=");
					password = password.substr(pos + 1, password.length() - (pos + 1));

					int len = password.length();

					if (len)
					{
						m_Password->SetText(DecryptPW(password.c_str(), len));

						IFOR(zv, 0, len)
							m_MainGump.m_PasswordFake->Insert(L'*');

						m_Password->SetPos(len);
					}
					else
					{
						m_MainGump.m_PasswordFake->SetText("");
						m_MainGump.m_PasswordFake->SetPos(0);
						m_Password->SetText("");
						m_Password->SetPos(0);
					}

					break;
				}
				case MSCC_REMEMBERPWD:
				{
					m_SavePassword->Checked = ToBool(strings[1]);
					
					if (!m_SavePassword->Checked)
					{
						m_MainGump.m_PasswordFake->SetText("");
						m_MainGump.m_PasswordFake->SetPos(0);
						m_Password->SetText("");
						m_Password->SetPos(0);
					}

					break;
				}
				case MSCC_AUTOLOGIN:
				{
					m_AutoLogin->Checked = ToBool(strings[1]);

					break;
				}
				case MSCC_SMOOTHMONITOR:
				{
					g_ScreenEffectManager.Enabled = ToBool(strings[1]);

					break;
				}
				case MSCC_THE_ABYSS:
				{
					g_TheAbyss = ToBool(strings[1]);

					break;
				}
				case MSCC_ASMUT:
				{
					g_Asmut = ToBool(strings[1]);

					break;
				}
				default:
					break;
			}
		}
	}
}
//----------------------------------------------------------------------------------
/*!
Сохранение конфига
@return 
*/
void CMainScreen::SaveGlobalConfig()
{
	WISPFUN_DEBUG("c165_f11");
	FILE *uo_cfg = NULL;
	fopen_s(&uo_cfg, g_App.FilePath("uo_debug.cfg").c_str(), "w");

	if (uo_cfg == NULL)
		return;

	char buf[128] = { 0 };

	sprintf_s(buf, "AcctID=%s\n", m_Account->c_str());
	fputs(buf, uo_cfg);

	if (m_SavePassword->Checked)
	{
		sprintf_s(buf, "AcctPassword=%s\n", CryptPW(m_Password->c_str(), m_Password->Length()).c_str());
		fputs(buf, uo_cfg);
		sprintf_s(buf, "RememberAcctPW=yes\n");
		fputs(buf, uo_cfg);
	}
	else
	{
		fputs("AcctPassword=\n", uo_cfg);
		sprintf_s(buf, "RememberAcctPW=no\n");
		fputs(buf, uo_cfg);
	}

	sprintf_s(buf, "AutoLogin=%s\n", (m_AutoLogin->Checked ? "yes" : "no"));
	fputs(buf, uo_cfg);

	sprintf_s(buf, "SmoothMonitor=%s\n", (g_ScreenEffectManager.Enabled ? "yes" : "no"));
	fputs(buf, uo_cfg);

	sprintf_s(buf, "TheAbyss=%s\n", (g_TheAbyss ? "yes" : "no"));
	fputs(buf, uo_cfg);

	sprintf_s(buf, "Asmut=%s\n", (g_Asmut ? "yes" : "no"));
	fputs(buf, uo_cfg);

	fclose(uo_cfg);
}
//----------------------------------------------------------------------------------
/*!
Шифрование пароля для сохранения в конфиг
@param [__in] buf Не зашифрованный пароль
@param [__in] len Длина пароля
@return Зашифрованный пароль
*/
string CMainScreen::CryptPW(const char *buf, int len)
{
	WISPFUN_DEBUG("c165_f12");
	char ret[50] = {0};

	IFOR(i, 0, len)
	{
		char c = buf[i];
		c += 13;

		if (c > 126)
			c -= 95;
		if (c == 32)
			c = 127;

		ret[i] = c;
	}

	return ret;
}
//----------------------------------------------------------------------------------
/*!
Расшифровка пароля
@param [__in] buf Зашифрованный пароль
@param [__in] len Длина пароля
@return Расшифрованный пароль
*/
string CMainScreen::DecryptPW(const char *buf, int len)
{
	WISPFUN_DEBUG("c165_f13");
	char ret[50] = {0};

	IFOR(i, 0, len)
	{
		char c = buf[i];
		if (c == 127)
			c = 32;

		c -= 13;
		if (c < 33)
			c += 95;

		ret[i] = c;
	}

	return ret;
}
//----------------------------------------------------------------------------------
