/****************************************************************************
**
** MainScreen.cpp
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
#include "stdafx.h"

TMainScreen *MainScreen = NULL;
//---------------------------------------------------------------------------
TMainScreen::TMainScreen()
: TBaseScreen(), m_SavePassword(false), m_AutoLogin(false)
{
	m_Account = new TEntryText(32, 0, 300);
	m_Password = new TEntryText(32, 0, 300);
	m_PasswordFake = new TEntryText(32, 0, 300);

	FontManager->GenerateA(2, m_Text[0], "Log in to Ultima Online", 0x0386);
	FontManager->GenerateA(2, m_Text[1], "Account Name", 0x0386);
	FontManager->GenerateA(2, m_Text[2], "Password", 0x0386);
	FontManager->GenerateA(9, m_Text[3], "Save Password", 0x0386);
	FontManager->GenerateA(9, m_Text[4], "Auto Login", 0x0386);

	string cver("UO Version " + g_ClientVersionText + ".");
	FontManager->GenerateA(9, m_Text[5], cver.c_str(), 0x034E);
}
//---------------------------------------------------------------------------
TMainScreen::~TMainScreen()
{
	IFOR(i, 0, 6)
		m_Text[i].Clear();
	
	delete m_Account;
	delete m_Password;
	delete m_PasswordFake;
}
//---------------------------------------------------------------------------
/*!
Инициализация
@return 
*/
void TMainScreen::Init()
{
	g_ConfigLoaded = false;

	SetWindowTextA(g_hWnd, "Ultima Online");

	RECT r = {0, 0, 0, 0};
	r.right = 640;
	r.bottom = 480;
	AdjustWindowRectEx(&r, GetWindowLongA(g_hWnd, GWL_STYLE),FALSE, GetWindowLongA(g_hWnd, GWL_EXSTYLE));

	if (r.left < 0)
		r.right += (r.left * (-1));

	if (r.top < 0)
		r.bottom += (r.top * (-1));

	if (!m_SavePassword)
	{
		m_Password->SetText(L"");
		m_PasswordFake->SetText(L"");
	}

	EntryPointer = m_PasswordFake;

	SetWindowPos(g_hWnd, HWND_TOP, 0, 0, r.right, r.bottom, 0);

	if (AnimationManager != NULL)
		AnimationManager->ClearUnusedTextures(GetTickCount() + 100000);

	QuestArrow.Enabled = false;

	g_TotalSendSize = 0;
	g_TotalRecvSize = 0;

	g_LightLevel = 0;
	g_PersonalLightLevel = 0;

	FontManager->SetSavePixels(false);

	SmoothMonitor.UseSunrise();
	m_SmoothScreenAction = 0;

	ToolTip.SeqIndex = 0;

	//Prepare textures on Main Screen:
	UO->ExecuteGump(0x0588); //Main Screen background
	UO->ExecuteGump(0x157C); //Main Screen
	UO->ExecuteGump(0x15A0); //Main Screen Notes
	UO->ExecuteResizepic(0x13BE); //ActPwd Container
	UO->ExecuteGump(0x058A); //Main Screen Castle?
	UO->ExecuteGumpPart(0x1589, 3); //X gump
	UO->ExecuteGumpPart(0x15A4, 3); //> gump
	UO->ExecuteResizepic(0x0BB8); //Account/Password text field
	UO->ExecuteGumpPart(0x00D2, 2); //Checkbox on / off
}
//---------------------------------------------------------------------------
/*!
Обработка события после плавного затемнения экрана
@param [__in_opt] action Идентификатор действия
@return 
*/
void TMainScreen::ProcessSmoothAction( __in_opt BYTE action)
{
	if (action == 0xFF)
		action = m_SmoothScreenAction;

	if (action == ID_SMOOTH_MS_CONNECT)
		UO->Connect();
	else if (action == ID_SMOOTH_MS_QUIT)
		PostMessage(g_hWnd, WM_CLOSE, 0, 0);
}
//---------------------------------------------------------------------------
/*!
Инициализация всплывающих подсказок
@return 
*/
void TMainScreen::InitToolTip()
{
	if (!ConfigManager.UseToolTips)
		return;

	switch (g_LastSelectedObject)
	{
		case ID_MS_QUIT:
		{
			ToolTip.Set(L"Quit Ultima Online", SOT_NO_OBJECT, g_LastSelectedObject, 80);
			break;
		}
		case ID_MS_ARROW_NEXT:
		{
			ToolTip.Set(L"Next screen", SOT_NO_OBJECT, g_LastSelectedObject);
			break;
		}
		case ID_MS_ACCOUNT:
		{
			ToolTip.Set(L"Click here to enter your user name", SOT_NO_OBJECT, g_LastSelectedObject, 150);
			break;
		}
		case ID_MS_PASSWORD:
		{
			ToolTip.Set(L"Click here to enter your password", SOT_NO_OBJECT, g_LastSelectedObject, 150);
			break;
		}
		case ID_MS_SAVEPASSWORD:
		{
			ToolTip.Set(L"Save your password in config file", SOT_NO_OBJECT, g_LastSelectedObject, 150);
			break;
		}
		case ID_MS_AUTOLOGIN:
		{
			ToolTip.Set(L"Auto LogIn (without selection screens)", SOT_NO_OBJECT, g_LastSelectedObject, 150);
			break;
		}
		default:
			break;
	}
}
//---------------------------------------------------------------------------
/*!
Отрисовка/выбор объектов
@param [__in] mode true - отрисовка, false - выбор
@return При выборе объектов - идентификатор выбранного объекта
*/
int TMainScreen::Render(__in bool mode)
{
	DWORD ticks = GetTickCount();

	static DWORD lastArrowTick = 0;
	static bool arrowLighted = false;
	
	if (g_LastRenderTime > ticks)
	{
		if (mode || !g_SelectGumpObjects)
			return 0;
	}
	
	if (mode)
	{
		int CanSelectedButton = g_LastSelectedObject;

		int CanPressedButton = 0;
		if (g_LeftMouseDown && g_LastObjectLeftMouseDown == g_LastSelectedObject)
			CanPressedButton = g_LastObjectLeftMouseDown;
	
		g_LastRenderTime = ticks + (g_FrameDelay[(int)(GetForegroundWindow() == g_hWnd)]);

		g_GL.BeginDraw();

		if (DrawSmoothMonitor())
			return 0;

		glColor3f(1.0f, 1.0f, 1.0f);

		static DWORD times = GetTickCount() + 3000;

		UO->DrawGump(0x0588, 0, 0, 0, 640, 480); //Main Gump background
		UO->DrawGump(0x0E14, 0, 0, 0); //Main Gump background 2
		UO->DrawGump(0x157C, 0, 0, 0); //Main Gump
		UO->DrawGump(0x15A0, 0, 0, 4); //Main Gump Notes
		UO->DrawResizepicGump(0x13BE, 128, 288, 451, 157); //ActPwd Container
		UO->DrawGump(0x058A, 0, 286, 45); //Main Gump Castle?

		WORD GumpID = 0x1589 + (int)(CanSelectedButton == ID_MS_QUIT); //X gump /lighted
		if (CanPressedButton == ID_MS_QUIT)
			GumpID = 0x158B; //X gump (pressed)
		UO->DrawGump(GumpID, 0, 555, 4);
		
		GumpID = 0x15A4 + (int)(arrowLighted || (CanSelectedButton == ID_MS_ARROW_NEXT)); //> gump / lighted
		if (CanPressedButton == ID_MS_ARROW_NEXT)
			GumpID = 0x15A6; //> gump pressed
		UO->DrawGump(GumpID, 0, 610, 445);

		if (lastArrowTick < ticks)
		{
			arrowLighted = !arrowLighted;
			lastArrowTick = ticks + 500;
		}

		UO->DrawResizepicGump(0x0BB8, 328, 343, 210, 30); //Account text field
		UO->DrawResizepicGump(0x0BB8, 328, 383, 210, 30); //Password text field
		UO->DrawGump(0x00D3 - (int)(m_SavePassword == false), 0, 328, 417); //Checkbox on / off
		UO->DrawGump(0x00D3 - (int)(m_AutoLogin == false), 0, 183, 417); //Checkbox on / off
		
		m_Text[0].Draw(253, 305);
		m_Text[1].Draw(183, 345);
		m_Text[2].Draw(183, 385);
		m_Text[3].Draw(351, 421);
		m_Text[4].Draw(206, 421);
		m_Text[5].Draw(286, 455);

		WORD TextColor = 0x034F;
		if (EntryPointer == m_Account)
			TextColor = 0x0021;
		else if (g_LastSelectedObject == ID_MS_ACCOUNT)
			TextColor = 0x03E3;
		m_Account->DrawA(5, TextColor, 335, 343);

		TextColor = 0x034F;
		if (EntryPointer == m_PasswordFake)
			TextColor = 0x0021;
		else if (g_LastSelectedObject == ID_MS_PASSWORD)
			TextColor = 0x03E3;
		m_PasswordFake->DrawMaskA(5, TextColor, 335, 385);

		InitToolTip();

		DrawSmoothMonitorEffect();

		MouseManager.Draw(0x2073); //Main Gump mouse cursor

		g_GL.EndDraw();
	}
	else
	{
		g_LastSelectedObject = 0;

		if (UO->GumpPixelsInXY(0x1589, 555, 4))
			g_LastSelectedObject = ID_MS_QUIT; //X gump
		else if (UO->GumpPixelsInXY(0x15A4, 610, 445))
			g_LastSelectedObject = ID_MS_ARROW_NEXT; //> gump
		else if (UO->ResizepicPixelsInXY(0xBB8, 328, 343, 210, 30))
			g_LastSelectedObject = ID_MS_ACCOUNT; //Account text field
		else if (UO->ResizepicPixelsInXY(0xBB8, 328, 383, 210, 30))
			g_LastSelectedObject = ID_MS_PASSWORD; //Password text field
		else if (UO->GumpPixelsInXY(0x00D3, 328, 417))
			g_LastSelectedObject = ID_MS_SAVEPASSWORD; //Save password checkbox
		else if (UO->GumpPixelsInXY(0x00D3, 183, 417))
			g_LastSelectedObject = ID_MS_AUTOLOGIN; //Auto Login checkbox

		return g_LastSelectedObject;
	}

	return 0;
}
//---------------------------------------------------------------------------
/*!
Нажатие левой кнопки мыши
@return 
*/
void TMainScreen::OnLeftMouseDown()
{
	Render(false);

	g_DroppedLeftMouseX = g_MouseX;
	g_DroppedLeftMouseY = g_MouseY;
	g_LeftMouseDown = true;
	g_LastObjectLeftMouseDown = g_LastSelectedObject;

	if (g_LastSelectedObject == 0)
		return;

	if (g_LastSelectedObject == ID_MS_ACCOUNT) //Account text field
	{
		int x = g_MouseX - 335;
		int y = g_MouseY - 343;

		m_Account->OnClick(NULL, 5, false, x, y);
	}
	else if (g_LastSelectedObject == ID_MS_PASSWORD) //Password text field
	{
		int x = g_MouseX - 335;
		int y = g_MouseY - 385;

		m_PasswordFake->OnClick(NULL, 5, false, x, y);
		m_Password->SetPos(m_PasswordFake->Pos());
	}
}
//---------------------------------------------------------------------------
/*!
Отпускание левой кнопки мыши
@return 
*/
void TMainScreen::OnLeftMouseUp()
{
	Render(false);

	g_LeftMouseDown = false;
	g_DroppedLeftMouseX = 0;
	g_DroppedLeftMouseY = 0;

	if (g_LastSelectedObject == 0 || g_LastSelectedObject != g_LastObjectLeftMouseDown)
	{
		g_LastObjectLeftMouseDown = 0;
		return;
	}

	if (g_LastObjectLeftMouseDown == ID_MS_QUIT) //x button
		CreateSmoothAction(ID_SMOOTH_MS_QUIT);
	else if (g_LastObjectLeftMouseDown == ID_MS_ARROW_NEXT) //> button
	CreateSmoothAction(ID_SMOOTH_MS_CONNECT);
	else if (g_LastObjectLeftMouseDown == ID_MS_SAVEPASSWORD) //Save password checkbox
		m_SavePassword = !m_SavePassword;
	else if (g_LastObjectLeftMouseDown == ID_MS_AUTOLOGIN) //Auto Login checkbox
		m_AutoLogin = !m_AutoLogin;

	g_LastObjectLeftMouseDown = 0;
}
//---------------------------------------------------------------------------
/*!
Обработка нажатия клавиши
@param [__in] wparam не подписанный параметр
@param [__in] lparam не подписанный параметр
@return 
*/
void TMainScreen::OnCharPress( __in WPARAM wparam, __in LPARAM lparam)
{
	if (wparam == VK_RETURN || wparam == VK_BACK || wparam == VK_ESCAPE || wparam >= 0x100)
		return; //Ignore no print keys
	else if (!FontManager->IsPrintASCII(wparam & 0xFF))
		return;
	else if (EntryPointer == NULL)
		EntryPointer = m_PasswordFake;

	wchar_t wstr[2] = {0};
	wstr[0] = wparam;
	wstring str(wstr);

	if (!str.length())
		return;

	if (EntryPointer->Length() < 16) //add char to text field
	{
		if (EntryPointer == m_PasswordFake)
		{
			if (EntryPointer->Insert(L'*'))
				m_Password->Insert(wparam);
		}
		else
			EntryPointer->Insert(wparam);
	}
}
//---------------------------------------------------------------------------
/*!
Обработка нажатия клавиши
@param [__in] wparam не подписанный параметр
@param [__in] lparam не подписанный параметр
@return 
*/
void TMainScreen::OnKeyPress( __in WPARAM wparam, __in LPARAM lparam)
{
	if (EntryPointer == NULL)
		EntryPointer = m_PasswordFake;

	switch (wparam)
	{
		case VK_TAB:
		{
			if (EntryPointer == m_Account)
				EntryPointer = m_PasswordFake;
			else
				EntryPointer = m_Account;

			break;
		}
		case VK_RETURN:
		{
			CreateSmoothAction(ID_SMOOTH_MS_CONNECT);

			break;
		}
		default:
		{
			if (EntryPointer == m_PasswordFake)
				m_Password->OnKey(NULL, wparam);

			EntryPointer->OnKey(NULL, wparam);

			break;
		}
	}
}
//---------------------------------------------------------------------------
/*!
Получить код конфига по ключу
@param [__in] key Ключ
@return 
*/
int TMainScreen::GetConfigKeyCode( __in string &key)
{
	const int keyCount = 6;

	const string m_Keys[keyCount] =
	{
		"acctid",
		"acctpassword",
		"rememberacctpw",
		"autologin",
		"smoothmonitorscale",
		"smoothmonitor"
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
//---------------------------------------------------------------------------
/*!
Загрузка конфига
@return 
*/
void TMainScreen::LoadGlobalConfig()
{
	m_AutoLogin = false;
	SmoothMonitor.Enabled = false;
	SmoothMonitor.Scale = 1;

	TTextFileParser file(FilePath("uo_debug.cfg").c_str(), "=", "#;", "");

	while (!file.IsEOF())
	{
		PBYTE filePtr = file.GetFilePtr();
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
					filePtr += strings[0].length();

					while (*filePtr && (*filePtr == ' ' || *filePtr == '\t'))
						filePtr++;

					if (*filePtr == '=')
						filePtr++;

					PBYTE end = filePtr;

					while (*end && *end != '\n' && *end != '\r')
						end++;

					if (end - filePtr > 0)
					{
						string password((char*)filePtr, end - filePtr);
						int len = password.length();

						if (len)
						{
							m_Password->SetText(DecryptPW(password.c_str(), len));

							IFOR(zv, 0, len)
								m_PasswordFake->Insert(L'*');
						}

						m_Password->SetPos(len);
					}
					else
					{
						m_PasswordFake->SetText("");
						m_PasswordFake->SetPos(0);
						m_Password->SetText("");
						m_Password->SetPos(0);
					}

					break;
				}
				case MSCC_REMEMBERPWD:
				{
					m_SavePassword = ToBool(strings[1]);
					
					if (!m_SavePassword)
					{
						m_PasswordFake->SetText("");
						m_PasswordFake->SetPos(0);
						m_Password->SetText("");
						m_Password->SetPos(0);
					}

					break;
				}
				case MSCC_AUTOLOGIN:
				{
					m_AutoLogin = ToBool(strings[1]);

					break;
				}
				case MSCC_SMOOTHMONITOR_SCALE:
				{
					int scale = atoi(strings[1].c_str());

					if (scale > 0 && scale <= 15)
						SmoothMonitor.Scale = scale;

					break;
				}
				case MSCC_SMOOTHMONITOR:
				{
					SmoothMonitor.Enabled = ToBool(strings[1]);

					break;
				}
				default:
					break;
			}
		}
	}
}
//---------------------------------------------------------------------------
/*!
Сохранение конфига
@return 
*/
void TMainScreen::SaveGlobalConfig()
{
	FILE *uo_cfg = fopen(FilePath("uo_debug.cfg").c_str(), "w");

	char buf[128] = { 0 };

	sprintf(buf, "AcctID=%s\n", m_Account->c_str());
	fputs(buf, uo_cfg);

	if (m_SavePassword)
	{
		sprintf(buf, "AcctPassword=%s\n", CryptPW(m_Password->c_str(), m_Password->Length()).c_str());
		fputs(buf, uo_cfg);
		sprintf(buf, "RememberAcctPW=yes\n");
		fputs(buf, uo_cfg);
	}
	else
	{
		fputs("AcctPassword=\n", uo_cfg);
		sprintf(buf, "RememberAcctPW=no\n");
		fputs(buf, uo_cfg);
	}

	sprintf(buf, "AutoLogin=%s\n", (m_AutoLogin ? "yes" : "no"));
	fputs(buf, uo_cfg);

	sprintf(buf, "SmoothMonitor=%s\n", (SmoothMonitor.Enabled ? "yes" : "no"));
	fputs(buf, uo_cfg);

	sprintf(buf, "SmoothMonitorScale=%i\n", SmoothMonitor.Scale);
	fputs(buf, uo_cfg);

	fclose(uo_cfg);
}
//---------------------------------------------------------------------------
/*!
Шифрование пароля для сохранения в конфиг
@param [__in] buf Не зашифрованный пароль
@param [__in] len Длина пароля
@return Зашифрованный пароль
*/
string TMainScreen::CryptPW(__in const char *buf, __in int len)
{
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
//---------------------------------------------------------------------------
/*!
Расшифровка пароля
@param [__in] buf Зашифрованный пароль
@param [__in] len Длина пароля
@return Расшифрованный пароль
*/
string TMainScreen::DecryptPW(__in const char *buf, __in int len)
{
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
//---------------------------------------------------------------------------