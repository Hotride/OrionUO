/****************************************************************************
**
** ClientUO.cpp
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
#include "stdafx.h"
#include "ClientUO.h"

TOrion *Orion = NULL;
PLUGIN_CLIENT_INTERFACE PluginClientInterface = { 0 };
//---------------------------------------------------------------------------
LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	static DWORD LastRClickTime = 0;

	if (message >= WM_USER && PluginManager != NULL)
		PluginManager->WindowProc(hWnd, message, wParam, lParam);

	switch (message)
	{
		case WM_GETMINMAXINFO:
		case WM_SIZE:
		{
			if (Orion == NULL || IsIconic(g_hWnd))
				return DefWindowProc(hWnd, message, wParam, lParam);
				
			if (message == WM_GETMINMAXINFO)
			{
				MINMAXINFO *pInfo = (MINMAXINFO*)lParam;

				if (g_GameState < GS_GAME)
				{
					RECT r = {0, 0, 0, 0};
					r.right = 640;
					r.bottom = 480;
					AdjustWindowRectEx(&r, GetWindowLongA(g_hWnd, GWL_STYLE), FALSE, GetWindowLongA(g_hWnd, GWL_EXSTYLE));

					if (r.left < 0)
						r.right += (r.left * (-1));

					if (r.top < 0)
						r.bottom += (r.top * (-1));

					POINT min = {r.right, r.bottom};
					POINT max = {r.right, r.bottom};
					g_GL.UpdateRect();
					pInfo->ptMinTrackSize = min;
					pInfo->ptMaxTrackSize = max;
				}
				else
				{
					RECT r = {0, 0, 0, 0};
					r.right = g_GameWindowWidth;
					r.bottom = g_GameWindowWidth;
					AdjustWindowRectEx(&r, GetWindowLongA(g_hWnd, GWL_STYLE), FALSE, GetWindowLongA(g_hWnd, GWL_EXSTYLE));

					if (r.left < 0)
						r.right += (r.left * (-1));

					if (r.top < 0)
						r.bottom += (r.top * (-1));
						
					POINT min = { g_GameWindowWidth, g_GameWindowHeight };
					pInfo->ptMinTrackSize = min;
					POINT max = {GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN)};
					g_GL.UpdateRect();
					pInfo->ptMinTrackSize = min;
					pInfo->ptMaxTrackSize = max;
				}

				return 0;
			}

			g_GL.UpdateRect();

			break;
		}
		case WM_CLOSE:
		case WM_NCDESTROY:
		case WM_DESTROY:
		{
			if (Orion != NULL)
			{
				if (PluginManager != NULL)
					PluginManager->WindowProc(hWnd, message, wParam, lParam);

				Orion->SaveLocalConfig();

				__try
				{
					delete Orion;
				}
				__finally
				{
					Orion = NULL;
				}
			}
			
			PostQuitMessage(0);

			return 0;
		}
		case WM_LBUTTONDOWN:
		{
			if (SmoothMonitor.Type != SMT_NONE)
				break;

			MouseManager.UpdateMouse();
			g_GeneratedMouseDown = false;
			DWORD ticks = GetTickCount();
			g_LastMouseDownTime = ticks;
			g_CancelDoubleClick = false;

			g_SelectGumpObjects = true;

			if (g_LastLClickTime + DCLICK_DELAY >= ticks)
			{
				g_ClickObjectReq = false;
				g_LastLClickTime = 0;

				if (!CurrentScreen->OnLeftMouseDoubleClick())
				{
					CurrentScreen->OnLeftMouseDown();

					g_LeftMouseDown = true;
					g_DroppedLeftMouseX = g_MouseX;
					g_DroppedLeftMouseY = g_MouseY;
					g_LastObjectLeftMouseDown = g_LastSelectedObject;
					g_LastGumpLeftMouseDown = g_LastSelectedGump;
				}
				else
					g_LastLClickTime = -1;

				g_SelectGumpObjects = false;

				break;
			}

			CurrentScreen->OnLeftMouseDown();

			g_LeftMouseDown = true;
			g_DroppedLeftMouseX = g_MouseX;
			g_DroppedLeftMouseY = g_MouseY;
			g_LastObjectLeftMouseDown = g_LastSelectedObject;
			g_LastGumpLeftMouseDown = g_LastSelectedGump;

			g_LastGumpMouseDownTime = ticks;

			g_SelectGumpObjects = false;

			if (g_CancelDoubleClick)
				g_LastLClickTime = 0;
			else
				g_LastLClickTime = ticks;

			if (g_PopupMenu != NULL && g_LastSelectedGump != (DWORD)g_PopupMenu)
				GumpManager->RemoveGump(g_PopupMenu);

			break;
		}
		case WM_LBUTTONUP:
		{
			if (SmoothMonitor.Type != SMT_NONE)
				break;

			MouseManager.UpdateMouse();

			if (g_LastLClickTime != -1)
			{
				g_SelectGumpObjects = true;

				CurrentScreen->OnLeftMouseUp();
				
				if (g_MovingFromMouse && !g_LastSelectedGump)
					g_AutoMoving = true;

				g_LeftMouseDown = false;
				g_DroppedLeftMouseX = 0;
				g_DroppedLeftMouseY = 0;
				g_LastObjectLeftMouseDown = 0;
				g_LastGumpLeftMouseDown = 0;
				g_LastSelectedGump = 0;

				g_SelectGumpObjects = false;
			}

			if (g_CancelDoubleClick)
				g_LastLClickTime = 0;
			
			g_LastGumpMouseDownTime = 0;

			break;
		}
		case WM_RBUTTONDOWN:
		{
			if (SmoothMonitor.Type != SMT_NONE)
				break;

			MouseManager.UpdateMouse();

			DWORD ticks = GetTickCount();

			g_SelectGumpObjects = true;

			if (LastRClickTime + DCLICK_DELAY >= ticks)
			{
				LastRClickTime = 0;

				if (!CurrentScreen->OnRightMouseDoubleClick())
				{
					CurrentScreen->OnRightMouseDown();

					g_RightMouseDown = true;
					g_DroppedRightMouseX = g_MouseX;
					g_DroppedRightMouseY = g_MouseY;
					g_LastGumpRightMouseDown = g_LastSelectedGump;
				}

				g_SelectGumpObjects = false;

				break;
			}

			CurrentScreen->OnRightMouseDown();
			
			if (!g_LastSelectedGump &&
				!(g_MouseX < g_GameWindowPosX || g_MouseY < g_GameWindowPosY || g_MouseX > (g_GameWindowPosX + g_GameWindowWidth) ||
				g_MouseY > (g_GameWindowPosY + g_GameWindowHeight)))
			{
				g_MovingFromMouse = true;
				g_AutoMoving = false;
			}

			g_RightMouseDown = true;
			g_DroppedRightMouseX = g_MouseX;
			g_DroppedRightMouseY = g_MouseY;
			g_LastGumpRightMouseDown = g_LastSelectedGump;

			g_SelectGumpObjects = false;

			LastRClickTime = ticks;

			if (g_PopupMenu != NULL && g_LastSelectedGump != (DWORD)g_PopupMenu)
				GumpManager->RemoveGump(g_PopupMenu);

			break;
		}
		case WM_RBUTTONUP:
		{
			if (SmoothMonitor.Type != SMT_NONE)
				break;

			MouseManager.UpdateMouse();

			g_SelectGumpObjects = true;

			CurrentScreen->OnRightMouseUp();
			
			g_RightMouseDown = false;
			g_DroppedRightMouseX = 0;
			g_DroppedRightMouseY = 0;
			g_LastGumpRightMouseDown = 0;
			g_LastSelectedGump = 0;
			g_MovingFromMouse = false;

			if (g_CancelDoubleClick)
				LastRClickTime = 0;

			g_SelectGumpObjects = false;

			break;
		}
		case WM_CHAR:
		{
			if (SmoothMonitor.Type != SMT_NONE)
				break;

			if (PluginManager != NULL && PluginManager->WindowProc(hWnd, message, wParam, lParam))
				return 0;

			g_SelectGumpObjects = true;

			CurrentScreen->OnCharPress(wParam, lParam);

			g_SelectGumpObjects = false;
			
			return 0; //break;
		}
		//Нажатие на колесико мышки
		case 0x0208:
		{
			if (SmoothMonitor.Type != SMT_NONE)
				break;

			if (PluginManager != NULL && PluginManager->WindowProc(hWnd, message, wParam, lParam))
				return 0;

			g_SelectGumpObjects = true;

			MouseManager.UpdateMouse();

			CurrentScreen->OnMouseWheel(MWS_PRESS);

			g_SelectGumpObjects = false;

			break;
		}
		//Колесико мышки вверх/вниз
		case 0x020A:
		{
			if (SmoothMonitor.Type != SMT_NONE)
				break;

			if (PluginManager != NULL && PluginManager->WindowProc(hWnd, message, wParam, lParam))
				return 0;

			g_SelectGumpObjects = true;

			MouseManager.UpdateMouse();

			if (short(HIWORD(wParam)) > 0)
				CurrentScreen->OnMouseWheel(MWS_UP);
			else
				CurrentScreen->OnMouseWheel(MWS_DOWN);

			g_SelectGumpObjects = false;

			break;
		}
		case WM_KEYDOWN:
		case WM_SYSKEYDOWN:
		{
			if (SmoothMonitor.Type != SMT_NONE)
				break;

			if (PluginManager != NULL && PluginManager->WindowProc(hWnd, message, wParam, lParam))
				return 0;

			//TPRINT("CurrentScreen->OnKeyPress(0x%08X, 0x%08X)\n", wParam, lParam);
			if (wParam == VK_TAB && (lParam & 0x40000000))
				break; //Уже нажато
			else if (wParam == VK_F4 && (GetAsyncKeyState(VK_MENU) & 0x80000000)) //Пропускаем Alt + F4
				break;

			g_SelectGumpObjects = true;

			CurrentScreen->OnKeyPress(wParam, lParam);

			g_SelectGumpObjects = false;
			
			return 0; //break;
		}
		case WM_KEYUP:
		case WM_SYSKEYUP:
		{
			if (SmoothMonitor.Type != SMT_NONE)
				break;

			if (PluginManager != NULL && PluginManager->WindowProc(hWnd, message, wParam, lParam))
				return 0;

			if (wParam == VK_TAB && Orion != NULL && g_GameState == GS_GAME)
			{
				if (ConfigManager.HoldTabForCombat)
					Orion->ChangeWarmode(0);
			}
			else if (wParam == 0x2C) //Print Screen
				g_ScreenshotBuilder.SaveScreen();

			return 0; //break;
		}
		case MM_MCINOTIFY:
		{
			SoundManager.StopMusic();

			if (g_GameState < GS_GAME)
				SoundManager.PlayMidi(8);

			break;
		}
		case WM_NCACTIVATE:
		{
			if (!g_ConfigLoaded)
				break;
			if (wParam == 0)
				SoundManager.PauseSound();
			else
				SoundManager.ResumeSound();
		}
		case WM_NCPAINT:
		{
			if (PluginManager != NULL && !PluginManager->Empty())
				return PluginManager->WindowProc(hWnd, message, wParam, lParam);

			break;
		}
		case WM_TIMER:
		{
			if (wParam == IDT_UPDATE_MOUSE_TIMER)
				MouseManager.UpdateMouse();

			break;
		}
		case UOMSG_RECV:
			PacketManager.PluginReceiveHandler((PBYTE)wParam, lParam);
			break;
		case UOMSG_SEND:
			ConnectionManager.Send((PBYTE)wParam, lParam);
			break;
		default:
			break;
	}

	return DefWindowProc(hWnd, message, wParam, lParam);
}
//---------------------------------------------------------------------------
BOOL InitInstance(int nCmdShow)
{
	WNDCLASSEX wcex = { 0 };
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WindowProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = g_Hinstance;
	//wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hCursor = LoadCursor(g_Hinstance, MAKEINTRESOURCE(IDC_CURSOR1));
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = L"Orion UO Client";
	wcex.hIcon = LoadIcon(g_Hinstance, MAKEINTRESOURCE(IDI_CLIENTUO));
	wcex.hIconSm = LoadIcon(g_Hinstance, MAKEINTRESOURCE(IDI_CLIENTUO));

	RegisterClassEx(&wcex);

	int winWidth = GetSystemMetrics(SM_CXSCREEN);

	if (winWidth > 640)
		winWidth = 640;

	winWidth += (2 * GetSystemMetrics(SM_CXSIZEFRAME));

	int winHeight = GetSystemMetrics(SM_CYSCREEN);

	if (winHeight > 480)
		winHeight = 480;

	winHeight += (GetSystemMetrics(SM_CYCAPTION) + (GetSystemMetrics(SM_CYFRAME) * 2));

	g_hWnd = CreateWindowEx(WS_EX_WINDOWEDGE, L"Orion UO Client", L"Ultima Online", WS_OVERLAPPEDWINDOW,
		0, 0, winWidth, winHeight, NULL, NULL, g_Hinstance, NULL);

	if (!g_hWnd)
		return FALSE;

	ShowCursor(nCmdShow);

	SetUnhandledExceptionFilter(MyUnhandledExceptionFilter);
	
	RECT r = {0, 0, 0, 0};
	r.right = 640;
	r.bottom = 480;
	AdjustWindowRectEx(&r, GetWindowLongA(g_hWnd, GWL_STYLE), FALSE, GetWindowLongA(g_hWnd, GWL_EXSTYLE));

	if (r.left < 0)
		r.right += (r.left * (-1));

	if (r.top < 0)
		r.bottom += (r.top * (-1));
	
	SetWindowPos(g_hWnd, HWND_TOP, 0, 0, r.right, r.bottom, 0);
	
	char buf[MAX_PATH] = {0};
	GetCurrentDirectoryA(MAX_PATH, buf);
	g_DirectoryPath = buf;

	srand(unsigned(time(NULL)));
	
	GetClientRect(g_hWnd, &r);
	g_ClientWidth = r.right - r.left;
	g_ClientHeight = r.bottom - r.top;

	Orion = new TOrion();

	if (Orion == NULL)
		return FALSE;

	if (!Orion->Install())
	{
		delete Orion;
		return FALSE;
	}
	
	SetTimer(g_hWnd, IDT_UPDATE_MOUSE_TIMER, 100, NULL);

	ShowWindow(g_hWnd, nCmdShow);
	UpdateWindow(g_hWnd);
	
	Orion->LoadPluginConfig();

	return TRUE;
}
//---------------------------------------------------------------------------
int APIENTRY _tWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPTSTR  lpCmdLine, _In_ int nCmdShow)
{
	g_Hinstance = hInstance;

	if (!InitInstance(nCmdShow))
		return FALSE;

	MSG msg = { 0 };
	timeBeginPeriod(1);
	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
			Sleep(1);
		
		if (Orion != NULL)
		{
			MouseManager.UpdateMouse();

			Orion->Process();
		}
	}
	
	return (int)msg.wParam;
}
//---------------------------------------------------------------------------
TOrion::TOrion()
: m_StaticAnimList(NULL), m_UsedLandList(NULL), m_UsedStaticList(NULL),
m_UsedGumpList(NULL), m_UsedTextureList(NULL), m_UsedSoundList(NULL),
m_AnimData(NULL), m_UsedLightList(NULL)
{
	g_Logger = new CLogger();
	string pth = g_DirectoryPath + "\\uolog.txt";
	g_Logger->Init(pth.c_str());
}
//---------------------------------------------------------------------------
TOrion::~TOrion()
{
	KillTimer(g_hWnd, IDT_UPDATE_MOUSE_TIMER);

	if (MainScreen != NULL)
		MainScreen->SaveGlobalConfig();

	if (GumpManager != NULL)
		GumpManager->OnDelete();
	
	Disconnect();

	UnloadIndexFiles();

	if (SpeechManager != NULL)
	{
		delete SpeechManager;
		SpeechManager = NULL;
	}

	if (FontManager != NULL)
	{
		delete FontManager;
		FontManager = NULL;
	}

	if (DebugScreen != NULL)
	{
		delete DebugScreen;
		DebugScreen = NULL;
	}

	if (MainScreen != NULL)
	{
		delete MainScreen;
		MainScreen = NULL;
	}

	if (ConnectionScreen != NULL)
	{
		delete ConnectionScreen;
		ConnectionScreen = NULL;
	}

	if (ServerScreen != NULL)
	{
		delete ServerScreen;
		ServerScreen = NULL;
	}

	if (CharacterListScreen != NULL)
	{
		delete CharacterListScreen;
		CharacterListScreen = NULL;
	}

	if (CreateCharacterScreen != NULL)
	{
		delete CreateCharacterScreen;
		CreateCharacterScreen = NULL;
	}
	
	if (SelectProfessionScreen != NULL)
	{
		delete SelectProfessionScreen;
		SelectProfessionScreen = NULL;
	}

	if (SelectTownScreen != NULL)
	{
		delete SelectTownScreen;
		SelectTownScreen = NULL;
	}
	
	if (GameScreen != NULL)
	{
		delete GameScreen;
		GameScreen = NULL;
	}

	if (GameBlockedScreen != NULL)
	{
		delete GameBlockedScreen;
		GameBlockedScreen = NULL;
	}

	CurrentScreen = NULL;

	if (SystemChat != NULL)
	{
		delete SystemChat;
		SystemChat = NULL;
	}
	
	if (Journal != NULL)
	{
		delete Journal;
		Journal = NULL;
	}

	if (WorldTextRenderer != NULL)
	{
		delete WorldTextRenderer;
		WorldTextRenderer = NULL;
	}

	if (ColorManager != NULL)
	{
		delete ColorManager;
		ColorManager = NULL;
	}

	if (AnimationManager != NULL)
	{
		delete AnimationManager;
		AnimationManager = NULL;
	}

	if (m_StaticAnimList != NULL)
	{
		delete m_StaticAnimList;
		m_StaticAnimList = NULL;
	}

	if (m_UsedLandList != NULL)
	{
		delete m_UsedLandList;
		m_UsedLandList = NULL;
	}

	if (m_UsedStaticList != NULL)
	{
		delete m_UsedStaticList;
		m_UsedStaticList = NULL;
	}

	if (m_UsedGumpList != NULL)
	{
		delete m_UsedGumpList;
		m_UsedGumpList = NULL;
	}
	
	if (m_UsedTextureList != NULL)
	{
		delete m_UsedTextureList;
		m_UsedTextureList = NULL;
	}

	if (m_UsedSoundList != NULL)
	{
		delete m_UsedSoundList;
		m_UsedSoundList = NULL;
	}

	if (MapManager != NULL)
	{
		delete MapManager;
		MapManager = NULL;
	}

	if (World != NULL)
	{
		delete World;
		World = NULL;
	}

	if (EffectManager != NULL)
	{
		delete EffectManager;
		EffectManager = NULL;
	}
	
	if (GumpManager != NULL)
	{
		delete GumpManager;
		GumpManager = NULL;
	}
	
	if (MultiMap != NULL)
	{
		delete MultiMap;
		MultiMap = NULL;
	}

	if (m_AnimData != NULL)
	{
		delete m_AnimData;
		m_AnimData = NULL;
	}

	if (PathFinder != NULL)
	{
		delete PathFinder;
		PathFinder = NULL;
	}

	if (ProfessionManager != NULL)
	{
		delete ProfessionManager;
		ProfessionManager = NULL;
	}

	if (OptionsMacroManager != NULL)
	{
		delete OptionsMacroManager;
		OptionsMacroManager = NULL;
	}

	if (MacroManager != NULL)
	{
		delete MacroManager;
		MacroManager = NULL;
	}

	if (CityManager != NULL)
	{
		delete CityManager;
		CityManager = NULL;
	}

	if (CityList != NULL)
	{
		delete CityList;
		CityList = NULL;
	}
	
	if (GameConsole != NULL)
	{
		delete GameConsole;
		GameConsole = NULL;
	}

	if (PluginManager != NULL)
	{
		delete PluginManager;
		PluginManager = NULL;
	}

	IFOR(i, 0, 6)
	{
		if (g_MapTexture[i] != 0)
			glDeleteTextures(1, &g_MapTexture[i]);

		g_MapTexture[i] = 0;
	}

	EntryPointer = NULL;

	TGumpConsoleType::ReleaseTextTextures();
	TGumpQuestion::ReleaseTextTextures();
	TGumpJournal::ReleaseTextTextures();
	TGumpMenubar::ReleaseTextTextures();
	TGumpOptions::ReleaseTextTextures();
	TGumpSpellbook::ReleaseTextTextures();
	TGumpPartyManifest::ReleaseTextTextures();
	TGumpWorldMap::ReleaseTextTextures();
	TGumpBook::ReleaseTextTextures();
	
	if (ClilocManager != NULL)
	{
		delete ClilocManager;
		ClilocManager = NULL;
	}
	
	IFOR(i, 0, 2)
	{
		if (g_TextureGumpState[i] != 0)
		{
			glDeleteTextures(1, &g_TextureGumpState[i]);
			g_TextureGumpState[i] = 0;
		}
	}

	if (DeathShader != NULL)
	{
		delete DeathShader;
		DeathShader = NULL;
	}

	if (ColorizerShader != NULL)
	{
		delete ColorizerShader;
		ColorizerShader = NULL;
	}

	if (FontColorizerShader != NULL)
	{
		delete FontColorizerShader;
		FontColorizerShader = NULL;
	}

	if (LightColorizerShader != NULL)
	{
		delete LightColorizerShader;
		LightColorizerShader = NULL;
	}

	CurrentShader = NULL;

	SoundManager.Free();

	ServerList.Clear();

	FileManager.Unload();

	g_LightBuffer.Free();

	g_GL.Uninstall();

	if (g_Logger != NULL)
	{
		delete g_Logger;
		g_Logger = NULL;
	}
	timeEndPeriod(1);
}
//---------------------------------------------------------------------------
DWORD Reflect(DWORD source, int c)
{
	DWORD value = 0;

	IFOR(i, 1, c + 1)
	{
		if (source & 0x1)
			value |= (1 << (c - i));

		source >>= 1;
	}

	return value;
}
//---------------------------------------------------------------------------
bool TOrion::Install()
{
	IFOR(i, 0, 256)
	{
		m_CRC_Table[i] = Reflect(i, 8) << 24;

		IFOR(j ,0, 8)
			m_CRC_Table[i] = (m_CRC_Table[i] << 1) ^ ((m_CRC_Table[i] & (1 << 31)) ? 0x04C11DB7 : 0);

		m_CRC_Table[i] = Reflect(m_CRC_Table[i], 32);
	}

	if (!g_GL.Install(g_hWnd))
	{
		TPRINT("Error install OpenGL\n");
		MessageBoxA(g_hWnd, "Error install OpenGL", "Error install OpenGL!", MB_OK);
		return false;
	}
	
	g_GL.UpdateRect();
	
	LoadClientConfig();
	LoadAutoLoginNames();
	
	if (!FileManager.Load())
	{
		string tmp = string("Error loading file: ") + g_LastLoadFileError;
		TPRINT(tmp.c_str());
		MessageBoxA(g_hWnd, tmp.c_str(), "Error loading file!", MB_OK);

		return false;
	}

	m_AnimData = new BYTE[FileManager.AnimdataMul.Size];
	memcpy(&m_AnimData[0], FileManager.AnimdataMul.Address, FileManager.AnimdataMul.Size);

	ColorManager = new TColorManager();
	
	LoadTiledata();
	LoadIndexFiles();
	InitStaticAnimList();

	FontManager = new TFontsManager();
	if (!FontManager->LoadFonts())
	{
		TPRINT("Error loading fonts\n");
		MessageBoxA(g_hWnd, "Error loading fonts", "Error loading fonts!", MB_OK);

		return false;
	}

	if (!LoadSkills())
	{
		TPRINT("Error loading skills\n");
		MessageBoxA(g_hWnd, "Error loading skills", "Error loading skills!", MB_OK);

		return false;
	}
	SpeechManager = new TSpeechManager();
	if (!SpeechManager->LoadSpeech())
	{
		TPRINT("Error loading speech.mul\n");
		MessageBoxA(g_hWnd, "Error loading speech.mul", "Error loading speech.mul!", MB_OK);

		return false;
	}
	
	PatchFiles();
	IndexReplaces();

	g_SkillSort.Init();
	
	if (!MouseManager.LoadCursorTextures())
	{
		TPRINT("Error loading cursors\n");
		MessageBoxA(g_hWnd, "Error loading cursors", "Error loading cursors!", MB_OK);

		return false;
	}

	if (!SoundManager.Init())
	{
		//TPRINT("Error install SDL audio: %s\n", SDL_GetError());
		//TPRINT("Error install BASS audio: %s\n", BASS_error());
		MessageBoxA(g_hWnd, "Failed to init BASS audio.", "Sound error!", MB_OK);

		return false;
	}

	AnimationManager = new TAnimationManager();
	IFOR(i, 0, 6)
		AnimationManager->Init(i, (DWORD)FileManager.AnimIdx[i].Address, (DWORD)FileManager.AnimMul[i].Address, (DWORD)FileManager.AnimIdx[i].Size);

	PDWORD verda = (FileManager.UseVerdata ? (PDWORD)FileManager.VerdataMul.Address : NULL);
	AnimationManager->InitIndexReplaces(verda);

	ClilocManager = new TClilocManager();
	ProfessionManager = new TProfessionManager();
	ProfessionManager->Load();

	LoadLogin();

	HKEY hKey;
	if (ERROR_SUCCESS == RegOpenKeyEx(HKEY_LOCAL_MACHINE, L"SYSTEM\\CurrentControlSet\\Control\\Nls\\CodePage", 0, KEY_QUERY_VALUE, &hKey))
	{
		char val[4096] = {0};
		DWORD dsize = 4096;
		RegQueryValueEx (hKey, L"ACP", NULL, NULL, (LPBYTE)val, &dsize);
		RegCloseKey(hKey);
		g_CodePage = atoi(val);
	}
	
	ExecuteStaticArt(0x0EED); //gp 1
	ExecuteStaticArt(0x0EEE); //gp 2-5
	ExecuteStaticArt(0x0EEF); //gp 6+

	CreateDirectoryA(FilePath("snapshots").c_str(), NULL);

	DebugScreen = new TDebugScreen();
	MainScreen = new TMainScreen();
	ConnectionScreen = new TConnectionScreen();
	ServerScreen = new TServerScreen();
	CharacterListScreen = new TCharacterListScreen();
	CreateCharacterScreen = new TCreateCharacterScreen();
	SelectProfessionScreen = new TSelectProfessionScreen();
	SelectTownScreen = new TSelectTownScreen();
	GameScreen = new TGameScreen();
	GameBlockedScreen = new TGameBlockedScreen();

	GumpManager = new TGumpManager();
	MapManager = new TMapManager();
	EffectManager = new TEffectManager();
	CityManager = new TCityManager();

	CityList = new TCityList();

	GameConsole = new TGameConsole();
	EntryPointer = NULL;
	
	SystemChat = new TTextContainer(30);
	Journal = new TJournal(50);
	WorldTextRenderer = new TTextRenderer();

	MacroManager = new TMacroManager();
	OptionsMacroManager = new TOptionsMacroManager();
	PluginManager = new TPluginManager();

	LoadClientStartupConfig();

	CreateCharacterManager.Init();

	TGumpConsoleType::InitTextTextures();
	TGumpQuestion::InitTextTextures();
	TGumpJournal::InitTextTextures();
	TGumpMenubar::InitTextTextures();
	TGumpOptions::InitTextTextures();
	TGumpSpellbook::InitTextTextures();
	TGumpPartyManifest::InitTextTextures();
	TGumpWorldMap::InitTextTextures();
	TGumpBook::InitTextTextures();

	Party.Leader = 0;
	Party.Inviter = 0;
	Party.Clear();

	PathFinder = new TPathFinder();

	MultiMap = new TMultiMap();

	g_ClickObject.Init(COT_NONE);
	
	WORD b = 0x0000;
	WORD r = 0xFC00; // 0xFF0000FF;
	WORD g = 0x83E0; // 0xFF00FF00;

	WORD pdwlt[] =
	{
		b, r, r, r, r, r, r, r, b, b,
		b, r, r, r, r, r, r, r, b, b,
		b, b, r, r, b, r, r, b, b, b,
		b, b, r, r, b, r, r, b, b, b,
		b, b, r, r, b, r, r, b, b, b,
		r, r, r, r, r, r, r, r, r, b,
		r, r, r, r, r, r, r, r, r, b,
		r, r, b, b, b, b, b, r, r, b,
		r, r, b, r, r, b, b, r, r, b,
		r, r, b, b, r, b, b, r, r, b,
		r, r, b, b, r, b, b, r, r, b,
		r, r, r, b, b, b, r, r, r, b,
		b, r, r, r, r, r, r, r, b, b,
		b, b, r, r, r, r, r, b, b, b
	};

	WORD pdwult[] =
	{
		b, b, b, g, g, g, g, g, g, g,
		b, b, b, g, g, g, g, g, g, g,
		b, b, b, b, g, g, b, b, b, b,
		b, b, b, b, g, g, b, b, b, b,
		b, b, b, b, g, g, b, b, b, b,
		g, g, g, g, g, g, g, g, g, b,
		g, g, g, g, g, g, g, g, g, b,
		g, g, b, b, b, b, b, g, g, b,
		g, g, b, g, g, b, b, g, g, b,
		g, g, b, b, g, b, b, g, g, b,
		g, g, b, b, g, b, b, g, g, b,
		g, g, g, b, b, b, g, g, g, b,
		b, g, g, g, g, g, g, g, b, b,
		b, b, g, g, g, g, g, b, b, b
	};
	
	g_GL.BindTexture16(g_TextureGumpState[1], 10, 14, pdwlt);
	g_GL.BindTexture16(g_TextureGumpState[0], 10, 14, pdwult);

	g_LightBuffer.Init(GetSystemMetrics(SM_CXMAXIMIZED), GetSystemMetrics(SM_CYMAXIMIZED));
	g_CharacterBuffer.Init(GetSystemMetrics(SM_CXMAXIMIZED), GetSystemMetrics(SM_CYMAXIMIZED));

	g_CircleOfTransparency.Create(200);

	CreateObjectHandlesBackground();

	MainScreen->LoadGlobalConfig();

	LoadShaders();

#ifdef _DEBUG
	CurrentScreen = DebugScreen;
	DebugScreen->Init();
#else
	InitScreen(GS_MAIN);
#endif

	//MacroManager->Convert(FilePath("macros.txt"));

	/*TMappedHeader mh;
	memset(&mh, 0, sizeof(mh));

	if (FileManager.LoadFileToMemory(mh, FilePath("Client7.exe").c_str()))
	{
		PBYTE ptr = (PBYTE)mh.Address;
		PBYTE end = ptr + mh.Size;

		while (ptr < end - 2)
		{
			WORD val = *(PWORD)ptr;

			if (val == 0x7558 || val == 0x7542 || val == 0x7563)
				TPRINT("[0x%04X]=0x%08X\n", val, ptr - (PBYTE)mh.Address);

			ptr++;
		}

		FileManager.UnloadFileFromMemory(mh);
	}*/

	/*TTextFileParser tfp(FilePath("cld1.txt").c_str(), " ", "", "");

	WORD id[126] = { 0 };

	while (!tfp.IsEOF())
	{
		std::vector<std::string> strings = tfp.ReadTokens();

		int sz = (int)strings.size();

		if (sz >= 2)
		{
			char *end;
			int v = strtoul(strings[0].c_str(), &end, 16);

			id[(v - 4004) / 4] = strtoul(strings[1].c_str(), &end, 16);
		}
	}

	TPRINT("{ ");
	IFOR(i, 0, 126)
		TPRINT("0x%04X, ", id[i]);
	TPRINT("}\n");*/

	/*TTextFileParser tfp(FilePath("ilog.txt").c_str(), " ", "", "");

	int mi = 111111;
	int ma = 0;
	int mi380 = 0;

	while (!tfp.IsEOF())
	{
		std::vector<std::string> strings = tfp.ReadTokens();

		int sz = (int)strings.size();

		if (sz == 2 && strings[0] == "WalkReqDelta")
		{
			int v = atoi(strings[1].c_str());

			if (v < 380)
				mi380++;
			else if (v < mi)
				mi = v;

			if (v > ma)
				ma = v;
		}
	}

	TPRINT("mi=%i ma=%i; mi380=%i\n", mi, ma, mi380);*/

	return true;
}
//---------------------------------------------------------------------------
void TOrion::CreateObjectHandlesBackground()
{
	TTextureObject *th[9] = { NULL };
	WORD gumpID[9] = { 0 };

	IFOR(i, 0, 9)
	{
		TTextureObject *pth = ExecuteGump(0x24EA + i);

		if (pth == NULL)
		{
			EPRINT("Failed to create Object Handles background data!\n");
			return;
		}

		if (i == 4)
		{
			th[8] = pth;
			gumpID[8] = 0x24EA + i;
		}
		else if (i > 4)
		{
			th[i - 1] = pth;
			gumpID[i - 1] = 0x24EA + i;
		}
		else
		{
			th[i] = pth;
			gumpID[i] = 0x24EA + i;
		}
	}

	IFOR(i, 0, 8)
	{
		if (i == 3 || i == 4)
			continue;

		TIndexObject &io = m_GumpDataIndex[gumpID[i]];

		int drawWidth = io.Width;
		int drawHeight = io.Height;
		int drawX = 0;
		int drawY = 0;

		switch (i)
		{
			case 1:
			{
				drawX = th[0]->Width;

				drawWidth = g_ObjectHandlesWidth - th[0]->Width - th[2]->Width;

				break;
			}
			case 2:
			{
				drawX = g_ObjectHandlesWidth - drawWidth;

				break;
			}
			case 3:
			{
				drawY = th[0]->Height;

				drawHeight = g_ObjectHandlesHeight - th[0]->Height - th[5]->Height;

				break;
			}
			case 4:
			{
				drawX = g_ObjectHandlesWidth - drawWidth;
				drawY = th[2]->Height;

				drawHeight = g_ObjectHandlesHeight - th[2]->Height - th[7]->Height;

				break;
			}
			case 5:
			{
				drawY = g_ObjectHandlesHeight - drawHeight;

				break;
			}
			case 6:
			{
				drawX = th[5]->Width;
				drawY = g_ObjectHandlesHeight - drawHeight;

				drawWidth = g_ObjectHandlesWidth - th[5]->Width - th[7]->Width;

				break;
			}
			case 7:
			{
				drawX = g_ObjectHandlesWidth - drawWidth;
				drawY = g_ObjectHandlesHeight - drawHeight;

				break;
			}
			case 8:
			{
				drawX = th[0]->Width;
				drawY = th[0]->Height;

				drawWidth = g_ObjectHandlesWidth - th[0]->Width - th[2]->Width;
				drawHeight = g_ObjectHandlesHeight - th[2]->Height - th[7]->Height;

				break;
			}
			default:
				break;
		}

		if (drawX < 0)
			drawX = 0;

		if (drawY < 0)
			drawY = 0;

		drawWidth += drawX;
		if (drawWidth > g_ObjectHandlesWidth)
			drawWidth = g_ObjectHandlesWidth - drawX;

		drawHeight += drawY;
		if (drawHeight > g_ObjectHandlesHeight)
			drawHeight = g_ObjectHandlesHeight - drawY;

		PWORD pixels = MulReader.GetGumpPixels(io);

		if (pixels != NULL)
		{
			int gumpWidth = io.Width;
			int gumpHeight = io.Height;
			int srcX = 0;

			IFOR(x, drawX, drawWidth)
			{
				int srcY = 0;
				IFOR(y, drawY, drawHeight)
				{
					WORD &pixel = g_ObjectHandlesBackgroundPixels[(y * g_ObjectHandlesWidth) + x];
					if (pixel == 0)
						pixel = pixels[((srcY % gumpHeight) * gumpWidth) + (srcX % gumpWidth)];

					srcY++;
				}

				srcX++;
			}

			delete pixels;
		}
	}
}
//---------------------------------------------------------------------------
void TOrion::InitScreen(GAME_STATE state)
{
	g_GameState = state;
	g_RightMouseDown = false;
	g_LeftMouseDown = false;
	g_SelectGumpObjects = false;
	g_DroppedLeftMouseX = 0;
	g_DroppedLeftMouseY = 0;
	g_DroppedRightMouseX = 0;
	g_DroppedRightMouseY = 0;
	g_LastObjectType = SOT_NO_OBJECT;
	g_LastSelectedObject = 0;
	g_LastObjectLeftMouseDown = 0;
	g_LastObjectRightMouseDown = 0;
	g_LastSelectedGump = 0;
	g_LastGumpLeftMouseDown = 0;
	g_LastGumpRightMouseDown = 0;
	g_SelectedObject = NULL;
	g_SelectedTextObject = NULL;

	switch (state)
	{
		case GS_MAIN:
		{
			CurrentScreen = MainScreen;
			break;
		}
		case GS_MAIN_CONNECT:
		{
			CurrentScreen = ConnectionScreen;
			break;
		}
		case GS_SERVER:
		{
			CurrentScreen = ServerScreen;
			break;
		}
		case GS_SERVER_CONNECT:
		{
			CurrentScreen = ConnectionScreen;
			break;
		}
		case GS_CHARACTER:
		{
			CurrentScreen = CharacterListScreen;
			break;
		}
		case GS_DELETE:
		{
			CurrentScreen = ConnectionScreen;
			break;
		}
		case GS_PROFESSION_SELECT:
		{
			CurrentScreen = SelectProfessionScreen;
			break;
		}
		case GS_SELECT_TOWN:
		{
			CurrentScreen = SelectTownScreen;
			break;
		}
		case GS_CREATE:
		{
			CurrentScreen = CreateCharacterScreen;
			break;
		}
		case GS_GAME_CONNECT:
		{
			CurrentScreen = ConnectionScreen;
			break;
		}
		case GS_GAME:
		{
			CurrentScreen = GameScreen;
			break;
		}
		case GS_GAME_BLOCKED:
		{
			CurrentScreen = GameBlockedScreen;
			break;
		}
		default:
			break;
	}

	if (CurrentScreen != NULL)
		CurrentScreen->Init();
}
//---------------------------------------------------------------------------
void TOrion::LoadTiledata()
{
	if (!FileManager.TiledataMul.Address)
		return;

	memcpy(&m_LandData[0], (PVOID)FileManager.TiledataMul.Address, 512 * sizeof(LAND_GROUP));

	memcpy(&m_StaticData[0], (PVOID)((DWORD)FileManager.TiledataMul.Address + 428032), 512 * sizeof(STATIC_GROUP));
}
//---------------------------------------------------------------------------
WORD TOrion::CalculateLightColor(WORD id)
{
	WORD color = m_StaticData[id / 32].Tiles[id % 32].Hue;

	if (!color)
	{
		switch (id)
		{
			case 0x0B1A:
			case 0x0B1B:
			case 0x0B1C:
			case 0x0B1D:
			case 0x0B1E:
			case 0x0B1F:
			case 0x0B20:
			case 0x0B21:
			case 0x0B22:
			case 0x0B23:
			case 0x0B24:
			case 0x0B25:
			case 0x0B26:
			case 0x0B27:
			case 0x0B28:
			{
				color = 0x029A;
				break;
			}
			case 0x0E2D:
			case 0x0E2E:
			case 0x0E2F:
			case 0x0E30:
			{
				color = 0x003E;
				break;
			}
			case 0x088C:
			{
				color = 0x001F;
				break;
			}
			//fire pit
			case 0x0FAC:
			{
				color = 0x001E;
				break;
			}
			//forge
			case 0x0FB1:
			{
				color = 0x003C;
				break;
			}
			case 0x1647:
			{
				color = 0x003D;
				break;
			}
			//blue moongate
			case 0x0F6C:
			//moongate
			case 0x1FD4:
			{
				color = 0x0002;
				break;
			}
			//brazier
			case 0x0E31:
			case 0x0E32:
			case 0x0E33:
			case 0x19BB:
			case 0x1F2B:
			{
				color = 0x0028;
				break;
			}
			//lava
			case 0x3547:
			case 0x3548:
			case 0x3549:
			case 0x354A:
			case 0x354B:
			case 0x354C:
			{
				color = 0x001F;
				break;
			}
			default:
				break;
		}

		if (!color)
		{
			//wall sconce
			if (CMP_RANGE(id, 0x09FB, 0x0A14))
				color = 0x001E;
			//lava
			else if (CMP_RANGE(id, 0x12EE, 0x134D) || CMP_RANGE(id, 0x306A, 0x329B) || CMP_RANGE(id, 0x343B, 0x346C))
				color = 0x001F;
			//?
			else if (CMP_RANGE(id, 0x3E27, 0x3E3A))
				color = 0x001F;
			//?
			else if (CMP_RANGE(id, 0x3E02, 0x3E0B))
				color = 0x0001;
			//fire field
			else if (CMP_RANGE(id, 0x398C, 0x399F))
				color = 0x001F;
			//paralyze field
			else if (CMP_RANGE(id, 0x3967, 0x397A))
				color = 0x0006;
			//energy field
			else if (CMP_RANGE(id, 0x3946, 0x3964))
				color = 0x0006;
			//poison field
			else if (CMP_RANGE(id, 0x3914, 0x3929))
				color = 0x0001;
			//glowing runes
			else if (CMP_RANGE(id, 0x0E5C, 0x0E6A))
				color = 0x0006;
			//?
			else if (CMP_RANGE(id, 0x3E02, 0x3E0B))
				color = 0x0002;
			else if (CMP_RANGE(id, 0x3E27, 0x3E3A) || CMP_RANGE(id, 0x398C, 0x399F))
				color = 0x029A;
			else if (CMP_RANGE(id, 0x1ECD, 0x1ECF) || CMP_RANGE(id, 0x1ED0, 0x1ED2))
				color = 0x0001;
			else if (CMP_RANGE(id, 0x19AB, 0x19B6) || CMP_RANGE(id, 0x197A, 0x19A9))
				color = 0x003C;
			else if (CMP_RANGE(id, 0x1853, 0x185A) || CMP_RANGE(id, 0x1849, 0x1850))
				color = 0x003D;
			else if (CMP_RANGE(id, 0x0DE1, 0x0DEA))
				color = 0x001F;
			else if (CMP_RANGE(id, 0x0A15, 0x0A29))
				color = 0x029A;
			else if (CMP_RANGE(id, 0x09FB, 0x0A14))
				color = 0x001E;
		}
	}

	return color;
}
//---------------------------------------------------------------------------
void TOrion::LoadIndexFiles()
{
	PART_IDX_BLOCK LandArtPtr = (PART_IDX_BLOCK)FileManager.ArtIdx.Address;
	PART_IDX_BLOCK StaticArtPtr = (PART_IDX_BLOCK)((DWORD)FileManager.ArtIdx.Address + (0x4000 * sizeof(ART_IDX_BLOCK)));
	PGUMP_IDX_BLOCK GumpArtPtr = (PGUMP_IDX_BLOCK)FileManager.GumpIdx.Address;
	PTEXTURE_IDX_BLOCK TexturePtr = (PTEXTURE_IDX_BLOCK)FileManager.TextureIdx.Address;
	PMULTI_IDX_BLOCK MultiPtr = (PMULTI_IDX_BLOCK)FileManager.MultiIdx.Address;
	PSOUND_IDX_BLOCK SoundPtr = (PSOUND_IDX_BLOCK)FileManager.SoundIdx.Address;
	PLIGHT_IDX_BLOCK LightPtr = (PLIGHT_IDX_BLOCK)FileManager.LightIdx.Address;

	g_MultiIndexCount = FileManager.MultiIdx.Size / sizeof(MULTI_IDX_BLOCK);
	if (g_MultiIndexCount > 0x2000)
		g_MultiIndexCount = 0x2000;

	IFOR(i, 0, MAX_GUMP_DATA_INDEX_COUNT)
	{ 
		if (i < MAX_LAND_DATA_INDEX_COUNT)
		{
			TIndexObject &land = m_LandDataIndex[i];

			land.Address = LandArtPtr->Position;
			land.Size = LandArtPtr->Size;

			if (land.Address == 0xFFFFFFFF || !land.Size || land.Size == 0xFFFFFFFF)
			{
				land.Address = 0;
				land.Size = 0;
			}
			else
				land.Address = land.Address + (DWORD)FileManager.ArtMul.Address;

			land.Width = 0;
			land.Height = 0;
			land.Texture = NULL;
			land.LastAccessTime = 0;

			LandArtPtr++;

			TIndexObject &statics = m_StaticDataIndex[i];

			statics.Address = StaticArtPtr->Position;
			statics.Size = StaticArtPtr->Size;

			if (statics.Address == 0xFFFFFFFF || !statics.Size || statics.Size == 0xFFFFFFFF)
			{
				statics.Address = 0;
				statics.Size = 0;
			}
			else
				statics.Address = statics.Address + (DWORD)FileManager.ArtMul.Address;

			statics.Width = 0;
			statics.Height = 0;
			statics.Texture = NULL;
			statics.LastAccessTime = 0;

			StaticArtPtr++;

			if (i < MAX_LAND_TEXTURES_DATA_INDEX_COUNT)
			{
				TIndexObject &textures = m_TextureDataIndex[i];

				textures.Address = TexturePtr->Position;
				textures.Size = TexturePtr->Size;

				if (textures.Address == 0xFFFFFFFF || !textures.Size || textures.Size == 0xFFFFFFFF)
				{
					textures.Address = 0;
					textures.Size = 0;
				}
				else
					textures.Address = textures.Address + (DWORD)FileManager.TextureMul.Address;

				textures.Width = 0;
				textures.Height = 0;
				textures.Texture = NULL;
				textures.LastAccessTime = 0;

				TexturePtr++;

				if (i < MAX_SOUND_DATA_INDEX_COUNT)
				{
					TIndexSound &sound = m_SoundDataIndex[i];

					sound.Address = SoundPtr->Position;
					sound.Size = SoundPtr->Size;
					sound.LastAccessTime = 0;
					sound.Timer = 0;
					//sound.Sound = NULL;
					sound.hStream = 0;
					
					if (sound.Address == 0xFFFFFFFF || !sound.Size || sound.Size == 0xFFFFFFFF)
					{
						sound.Address = 0;
						sound.Size = 0;
					}
					else
						sound.Address = sound.Address + (DWORD)FileManager.SoundMul.Address;

					SoundPtr++;

					if (i < MAX_LIGHTS_DATA_INDEX_COUNT)
					{
						TIndexObject &light = m_LightDataIndex[i];

						light.Address = LightPtr->Position;
						light.Size = LightPtr->Size;
						light.Width = LightPtr->Width;
						light.Height = LightPtr->Height;
						light.LastAccessTime = 0;
						light.Texture = NULL;
					
						if (light.Address == 0xFFFFFFFF || !light.Size || light.Size == 0xFFFFFFFF)
						{
							light.Address = 0;
							light.Size = 0;
						}
						else
							light.Address = light.Address + (DWORD)FileManager.LightMul.Address;

						LightPtr++;
					}
				}
			}
		}

		TIndexObject &gump = m_GumpDataIndex[i];

		gump.Address = GumpArtPtr->Position;
		gump.Size = GumpArtPtr->Size;

		if (gump.Address == 0xFFFFFFFF || !gump.Size || gump.Size == 0xFFFFFFFF)
		{
			gump.Address = 0;
			gump.Size = 0;
		}
		else
			gump.Address = gump.Address + (DWORD)FileManager.GumpMul.Address;

		gump.Width = GumpArtPtr->Width;
		gump.Height = GumpArtPtr->Height;
		gump.Texture = NULL;
		gump.LastAccessTime = 0;

		GumpArtPtr++;

		if (i < g_MultiIndexCount)
		{
			TIndexMulti &multi = m_MultiDataIndex[i];

			multi.Address = MultiPtr->Position;
			multi.Size = MultiPtr->Size;

			if (multi.Address == 0xFFFFFFFF || !multi.Size || multi.Size == 0xFFFFFFFF)
			{
				multi.Address = 0;
				multi.Size = 0;
			}
			else
				multi.Address += (DWORD)FileManager.MultiMul.Address;

			multi.Count = (WORD)(multi.Size / sizeof(MULTI_IDX_BLOCK));

			MultiPtr++;
		}
	}
}
//---------------------------------------------------------------------------
void TOrion::InitStaticAnimList()
{
	TLinkedList *firstItem = NULL;

	if (m_AnimData != NULL)
	{
		IFOR(i, 0, MAX_STATIC_DATA_INDEX_COUNT)
		{
			m_StaticDataIndex[i].Index = i;

			DWORD flags = m_StaticData[i / 32].Tiles[i % 32].Flags;

			m_StaticDataIndex[i].LightColor = CalculateLightColor((WORD)i);

			if (IsAnimated(flags))
			{
				if (m_StaticAnimList == NULL)
				{
					m_StaticAnimList = new TLinkedList(&m_StaticDataIndex[i]);
					firstItem = m_StaticAnimList;
				}
				else
				{
					m_StaticAnimList->Next = new TLinkedList(&m_StaticDataIndex[i]);
					m_StaticAnimList = m_StaticAnimList->Next;
				}
			}
		}
	}

	m_StaticAnimList = firstItem;
}
//---------------------------------------------------------------------------
void TOrion::ProcessStaticAnimList()
{
	if (m_AnimData)
	{
		TLinkedList *list = m_StaticAnimList;

		DWORD ticks = GetTickCount();
		int delay = (ConfigManager.StandartItemsAnimationDelay ? 0x50 : 50);

		while (list != NULL)
		{
			TIndexObjectStatic *obj = (TIndexObjectStatic*)list->Data;
			
			if (obj->NextChange < ticks)
			{
				DWORD addr = (obj->Index * 68) + 4 * ((obj->Index / 8) + 1);
				PANIM_DATA pad = (PANIM_DATA)(m_AnimData + addr);
			
				int offset = obj->AnimIndex;

				obj->NextChange = ticks + (pad->FrameInterval * delay);
				
				if (offset < pad->FrameCount)
				{
					obj->Increment = pad->FrameData[offset];
					offset++;
				}

				if (offset >= pad->FrameCount)
					offset = 0;

				obj->AnimIndex = offset;
			}
			
			list = list->Next;
		}
	}
}
//---------------------------------------------------------------------------
void TOrion::UnloadIndexFiles()
{
	TLinkedList *lists[6] =
	{
		m_UsedLandList,
		m_UsedStaticList,
		m_UsedGumpList,
		m_UsedTextureList,
		m_UsedLightList,
		m_UsedSoundList
	};

	IFOR(i, 0, 6)
	{
		TLinkedList *list = lists[i];

		while (list != NULL)
		{
			TLinkedList *delList = list;

			if (i < 5)
			{
				if (((TIndexObject*)(list->Data))->Texture != NULL)
				{
					delete ((TIndexObject*)(list->Data))->Texture;
					((TIndexObject*)(list->Data))->Texture = NULL;
				}
			}
			else if (((TIndexSound*)(list->Data))->hStream != 0)
			{
				//Mix_FreeChunk(((TIndexSound*)(list->Data))->Sound);
				BASS_StreamFree(((TIndexSound*)(list->Data))->hStream);
				((TIndexSound*)(list->Data))->hStream = 0;
				//((TIndexSound*)(list->Data))->Sound = NULL;
			}

			list = list->Next;

			delList->Next = NULL;
			delete delList;
		}
	}

	m_UsedLandList = NULL;
	m_UsedStaticList = NULL;
	m_UsedGumpList = NULL;
	m_UsedTextureList = NULL;
	m_UsedLightList = NULL;
	m_UsedSoundList = NULL;
}
//---------------------------------------------------------------------------
void TOrion::ClearUnusedTextures()
{
	if (g_GameState < GS_GAME)
		return;

	static int clearMap = 0;

	DWORD ticks = GetTickCount();

	if (clearMap == 1)
	{
		clearMap = 2;

		MapManager->ClearUnusedBlocks();

		return;
	}
	else if (clearMap == 2)
	{
		if (AnimationManager != NULL)
			AnimationManager->ClearUnusedTextures(ticks);
		
		clearMap = 0;

		return;
	}

	clearMap = 1;

	m_StaticDataIndex[0x0EED].LastAccessTime = ticks;
	m_StaticDataIndex[0x0EEE].LastAccessTime = ticks;
	m_StaticDataIndex[0x0EEF].LastAccessTime = ticks;

	IFOR(i, 0, 2)
	{
		IFOR(j, 0, 16)
			m_StaticDataIndex[g_CursorData[i][j]].LastAccessTime = ticks;
	}

	IFOR(i, 0, 8)
		m_StaticDataIndex[QuestArrow.m_Gump + i].LastAccessTime = ticks;

	GumpManager->PrepareTextures();

	ticks -= CLEAR_TEXTURES_DELAY;
	
	TLinkedList *indexList[5] =
	{
		m_UsedLandList,
		m_UsedStaticList,
		m_UsedGumpList,
		m_UsedTextureList,
		m_UsedLightList
	};

	IFOR(i, 0, 5)
	{
		TLinkedList *list = indexList[i];
		TLinkedList *prev = list;

		while (list != NULL)
		{
			TIndexObject *obj = (TIndexObject*)list->Data;
		
			if (obj->Texture != NULL && obj->LastAccessTime < ticks)
			{
				delete obj->Texture;
				obj->Texture = NULL;

				TLinkedList *next = list->Next;
				if (list == indexList[i])
				{
					switch (i)
					{
						case 0:
						{
							m_UsedLandList = m_UsedLandList->Next;
							indexList[i] = m_UsedLandList;
							break;
						}
						case 1:
						{
							m_UsedStaticList = m_UsedStaticList->Next;
							indexList[i] = m_UsedStaticList;
							break;
						}
						case 2:
						{
							m_UsedGumpList = m_UsedGumpList->Next;
							indexList[i] = m_UsedGumpList;
							break;
						}
						case 3:
						{
							m_UsedTextureList = m_UsedTextureList->Next;
							indexList[i] = m_UsedTextureList;
							break;
						}
						case 4:
						{
							m_UsedLightList = m_UsedLightList->Next;
							indexList[i] = m_UsedLightList;
							break;
						}
						default:
							break;
					}

					next = indexList[i];
					prev = next;
				}
				else
					prev->Next = list->Next;

				list->Next = NULL;
				delete list;

				list = next;
			}
			else
			{
				prev = list;
				list = list->Next;
			}
		}
	}

	AdjustSoundEffects(ticks);
}
//---------------------------------------------------------------------------
void TOrion::PatchFiles()
{
/*
	-map0 = 0;
	-staidx0 = 1;
	-statics0 = 2;
	+artidx = 3;
	+art = 4;
	-animidx = 5;
	-anim = 6;
	-soundidx = 7;
	-sound = 8;
	-texidx = 9;
	-texmaps = 10;
	+gumpidx = 11;
	+gumpart = 12;
	+multiidx = 13;
	+multi = 14;
	-skillsidx = 15;
	-skills = 16;
	+tiledata = 30;
	-animdata = 31;
	+hues = 32;
*/

	if (!FileManager.UseVerdata || !FileManager.VerdataMul.Address)
	{
		ColorManager->CreateHuesPalette();
		return;
	}

	int dataCount = *(PDWORD)FileManager.VerdataMul.Address;

	DWORD vAddr = (DWORD)FileManager.VerdataMul.Address;

	IFOR(i, 0, dataCount)
	{
		PVERDATA_HEADER vh = (PVERDATA_HEADER)(vAddr + 4 + (i * sizeof(VERDATA_HEADER)));

		if (vh->FileID == 4) //Art
		{
			if (vh->BlockID >= MAX_LAND_DATA_INDEX_COUNT) //Run
			{
				WORD ID = (WORD)vh->BlockID - MAX_LAND_DATA_INDEX_COUNT;
				m_StaticDataIndex[ID].Address = vAddr + vh->Position;
				m_StaticDataIndex[ID].Size = vh->Size;
			}
			else //Raw
			{
				m_LandDataIndex[vh->BlockID].Address = vAddr + vh->Position;
				m_LandDataIndex[vh->BlockID].Size = vh->Size;
			}
		}
		else if (vh->FileID == 12) //Gumpart
		{
			m_GumpDataIndex[vh->BlockID].Address = vAddr + vh->Position;
			m_GumpDataIndex[vh->BlockID].Size = vh->Size;
			m_GumpDataIndex[vh->BlockID].Width = vh->GumpData >> 16;
			m_GumpDataIndex[vh->BlockID].Height = vh->GumpData & 0xFFFF;
		}
		else if (vh->FileID == 14 && (int)vh->BlockID < g_MultiIndexCount) //Multi
		{
			m_MultiDataIndex[vh->BlockID].Address = vAddr + vh->Position;
			m_MultiDataIndex[vh->BlockID].Size = vh->Size;
			m_MultiDataIndex[vh->BlockID].Count = (WORD)(vh->Size / sizeof(MULTI_IDX_BLOCK));
		}
		else if (vh->FileID == 16 && (int)vh->BlockID < g_SkillsCount) //Skills
		{
			int index = (int)vh->BlockID;
			g_Skills[index].m_Button = *(PBYTE)(vAddr + vh->Position);

			char namebuf[128] = {0};
			strncpy(namebuf, (char*)(vAddr + vh->Position + 1), vh->Size - 1);

			if (strlen(namebuf) > 0)
			{
				g_Skills[index].m_Name = namebuf;
				g_Skills[index].m_Texture.Clear();
				FontManager->GenerateA(9, g_Skills[index].m_Texture, namebuf, 0x0288);
			}
			else
			{
				g_Skills[index].m_Texture.Clear();
				g_Skills[i].m_Name = "";
			}
		}
		else if (vh->FileID == 30) //Tiledata
		{
			if (vh->Size == 836)
			{
				if (vh->BlockID >= 512)
					continue;

				memcpy(&m_LandData[vh->BlockID], (PVOID)(vAddr + vh->Position), sizeof(LAND_GROUP));
			}
			else if (vh->Size == 1188)
			{
				int bID = (int)vh->BlockID - 0x0200;

				if (bID < 0 || bID >= 512)
					continue;

				memcpy(&m_StaticData[bID], (PVOID)(vAddr + vh->Position), sizeof(STATIC_GROUP));
			}
		}
		else if (vh->FileID == 32) //Hues
		{
			if ((int)vh->BlockID < ColorManager->GetHuesCount())
			{
				PVERDATA_HUES_GROUP group = (PVERDATA_HUES_GROUP)(vAddr + vh->Position);
				ColorManager->SetHuesBlock(vh->BlockID, group);
			}
		}
		else if (vh->FileID != 5 && vh->FileID != 6) //no Anim / Animidx
		{
			TPRINT("Unused verdata block (fileID) = %i (BlockID+ %i\n", vh->FileID, vh->BlockID);
		}
	}

	ColorManager->CreateHuesPalette();
}
//----------------------------------------------------------------------------
void TOrion::IndexReplaces()
{
	TTextFileParser newDataParser("", " \t,{}", "#;//", "");
	TTextFileParser artParser(FilePath("Art.def").c_str(), " \t", "#;//", "{}");
	TTextFileParser textureParser(FilePath("TexTerr.def").c_str(), " \t", "#;//", "{}");
	TTextFileParser gumpParser(FilePath("Gump.def").c_str(), " \t", "#;//", "{}");
	TTextFileParser multiParser(FilePath("Multi.def").c_str(), " \t", "#;//", "{}");
	TTextFileParser soundParser(FilePath("Sound.def").c_str(), " \t", "#;//", "{}");
	TTextFileParser mp3Parser(FilePath("Music\\Digital\\Config.txt").c_str(), " ,", "#;", "");

	while (!artParser.IsEOF())
	{
		std::vector<std::string> strings = artParser.ReadTokens();

		if (strings.size() >= 3)
		{
			int index = atoi(strings[0].c_str());

			if (index < 0 || index >= MAX_LAND_DATA_INDEX_COUNT + MAX_STATIC_DATA_INDEX_COUNT)
				continue;

			std::vector<std::string> newArt = newDataParser.GetTokens(strings[1].c_str());

			int size = (int)newArt.size();

			IFOR(i, 0, size)
			{
				int checkIndex = atoi(newArt[i].c_str());

				if (checkIndex < 0 || checkIndex >= MAX_LAND_DATA_INDEX_COUNT + MAX_STATIC_DATA_INDEX_COUNT)
					continue;

				if (index < MAX_LAND_DATA_INDEX_COUNT && checkIndex < MAX_LAND_DATA_INDEX_COUNT && m_LandDataIndex[checkIndex].Address != NULL)
				{
					memcpy(&m_LandDataIndex[index], &m_LandDataIndex[checkIndex], sizeof(TIndexObject));
					m_LandDataIndex[index].Texture = NULL;
					m_LandDataIndex[index].Color = atoi(strings[2].c_str());

					break;
				}
				else if (index >= MAX_LAND_DATA_INDEX_COUNT && checkIndex >= MAX_LAND_DATA_INDEX_COUNT && m_StaticDataIndex[checkIndex - 0x4000].Address != NULL)
				{
					checkIndex -= MAX_LAND_DATA_INDEX_COUNT;
					index -= MAX_LAND_DATA_INDEX_COUNT;

					memcpy(&m_StaticDataIndex[index], &m_StaticDataIndex[checkIndex], sizeof(TIndexObjectStatic));
					m_StaticDataIndex[index].Texture = NULL;
					m_StaticDataIndex[index].Color = atoi(strings[2].c_str());

					break;
				}
			}
		}
	}

	while (!textureParser.IsEOF())
	{
		std::vector<std::string> strings = textureParser.ReadTokens();

		if (strings.size() >= 3)
		{
			int index = atoi(strings[0].c_str());

			if (index < 0)
				continue;

			std::vector<std::string> newTexture = newDataParser.GetTokens(strings[1].c_str());

			int size = (int)newTexture.size();

			IFOR(i, 0, size)
			{
				int checkIndex = atoi(newTexture[i].c_str());

				if (checkIndex < 0)
					continue;

				if (index < MAX_LAND_TEXTURES_DATA_INDEX_COUNT && checkIndex < MAX_LAND_TEXTURES_DATA_INDEX_COUNT && m_TextureDataIndex[checkIndex].Address != NULL)
				{
					memcpy(&m_TextureDataIndex[index], &m_TextureDataIndex[checkIndex], sizeof(TIndexObject));
					m_TextureDataIndex[index].Texture = NULL;
					m_TextureDataIndex[index].Color = atoi(strings[2].c_str());

					break;
				}
			}
		}
	}

	while (!gumpParser.IsEOF())
	{
		std::vector<std::string> strings = gumpParser.ReadTokens();

		if (strings.size() >= 3)
		{
			int index = atoi(strings[0].c_str());

			if (index < 0 || index >= MAX_GUMP_DATA_INDEX_COUNT)
				continue;

			std::vector<std::string> newGump = newDataParser.GetTokens(strings[1].c_str());

			int size = (int)newGump.size();

			IFOR(i, 0, size)
			{
				int checkIndex = atoi(newGump[i].c_str());

				if (checkIndex < 0 || checkIndex >= MAX_GUMP_DATA_INDEX_COUNT || m_GumpDataIndex[checkIndex].Address == NULL)
					continue;

				memcpy(&m_GumpDataIndex[index], &m_GumpDataIndex[checkIndex], sizeof(TIndexObject));
				m_GumpDataIndex[index].Texture = NULL;
				m_GumpDataIndex[index].Color = atoi(strings[2].c_str());

				break;
			}
		}
	}

	while (!multiParser.IsEOF())
	{
		std::vector<std::string> strings = multiParser.ReadTokens();

		if (strings.size() >= 3)
		{
			int index = atoi(strings[0].c_str());

			if (index < 0 || index >= g_MultiIndexCount)
				continue;

			std::vector<std::string> newMulti = newDataParser.GetTokens(strings[1].c_str());

			int size = (int)newMulti.size();

			IFOR(i, 0, size)
			{
				int checkIndex = atoi(newMulti[i].c_str());

				if (checkIndex < 0 || checkIndex >= g_MultiIndexCount && m_MultiDataIndex[checkIndex].Address == NULL)
					continue;

				memcpy(&m_MultiDataIndex[index], &m_MultiDataIndex[checkIndex], sizeof(TIndexMulti));

				break;
			}
		}
	}

	while (!soundParser.IsEOF())
	{
		std::vector<std::string> strings = soundParser.ReadTokens();

		if (strings.size() >= 2)
		{
			int index = atoi(strings[0].c_str());

			if (index < 0 || index >= MAX_SOUND_DATA_INDEX_COUNT)
				continue;

			std::vector<std::string> newSound = newDataParser.GetTokens(strings[1].c_str());

			int size = (int)newSound.size();

			IFOR(i, 0, size)
			{
				int checkIndex = atoi(newSound[i].c_str());

				if (checkIndex < -1 || checkIndex >= MAX_SOUND_DATA_INDEX_COUNT)
					continue;

				TIndexSound &in = m_SoundDataIndex[index];

				if (checkIndex == -1)
				{
					in.Address = 0;
					in.Size = 0;
					in.Timer = 0;
					in.LastAccessTime = 0;
				}
				else
				{
					TIndexSound &out = m_SoundDataIndex[checkIndex];

					if (out.Address == NULL)
						continue;

					in.Address = out.Address;
					in.Size = out.Size;
					in.Timer = out.Timer;
					in.LastAccessTime = out.LastAccessTime;
				}

				in.waveFile.clear();
				in.hStream = NULL;

				break;
			}
		}
	}
	while (!mp3Parser.IsEOF())
	{
		std::vector<std::string> strings = mp3Parser.ReadTokens();
		int size = strings.size();
		if (size > 0)
		{
			TMP3Struct &mp3 = m_MP3Data[std::stoi(strings[0])];
			if (size > 1)
				mp3.FileName = FilePath("Music\\Digital\\" + strings[1] + ".mp3");
			if (size > 2)
				mp3.Loop = true;
		}		
	}
}
//---------------------------------------------------------------------------
bool TOrion::LoadSkills()
{
	if (!FileManager.SkillsIdx.Address || !FileManager.SkillsMul.Address || g_SkillsCount)
		return false;
	
	PSKILLS_IDX_BLOCK sidx = NULL, start = NULL, end = NULL;

	int cnt = FileManager.SkillsIdx.Size / sizeof(SKILLS_IDX_BLOCK);
	
	IFOR(i, 0, cnt)
	{
		sidx = (PSKILLS_IDX_BLOCK)((DWORD)FileManager.SkillsIdx.Address + (i * sizeof(SKILLS_IDX_BLOCK)));

		if (sidx->Size && sidx->Position != 0xFFFFFFFF && sidx->Size != 0xFFFFFFFF)
		{
			if (start == NULL)
				start = sidx;
			end = sidx;

			//TPRINT("Skill load[%d] = %s\n", i, (char*)((DWORD)FileManager.SkillsMul.Address + sidx->Position + 1));
		}
	}

	g_SkillsCount = (int)(end - start) + 1;

	if (g_SkillsCount < 2 || g_SkillsCount > 60)
	{
		g_SkillsCount = 0;

		return false;
	}

	g_Skills = new SKILLS_DATA[g_SkillsCount];

	for (int i = 0; ; i++, start++)
	{
		g_Skills[i].m_Button = *(PBYTE)((DWORD)FileManager.SkillsMul.Address + start->Position);

		char namebuf[128] = {0};
		strncpy(namebuf, (char*)((DWORD)FileManager.SkillsMul.Address + start->Position + 1), start->Size - 1);

		if (strlen(namebuf) > 0)
		{
			g_Skills[i].m_Name = namebuf;
			FontManager->GenerateA(9, g_Skills[i].m_Texture, namebuf, 0x0288);
		}
		else
			g_Skills[i].m_Name = "";

		if (start->Position == end->Position)
			break;
	}

	return true;
}
//---------------------------------------------------------------------------
void TOrion::UnloadSkills()
{
	if (g_Skills != NULL)
	{
		delete[]g_Skills;
		g_Skills = NULL;
	}
}
//--------------------------------------------------------------------------
void TOrion::LoadLogin()
{
	TTextFileParser file(FilePath("login.cfg").c_str(), "=,", "#;", "");

	while (!file.IsEOF())
	{
		std::vector<std::string> strings = file.ReadTokens();

		if (strings.size() >= 3)
		{
			string lo = ToLowerA(strings[0]);

			if (lo == string("loginserver"))
			{
				g_ShardIP = strings[1];
				g_ShardPort = atoi(strings[2].c_str());
			}
		}
	}
}
//---------------------------------------------------------------------------
void TOrion::LoadAutoLoginNames()
{
	TTextFileParser file(FilePath("AutoLoginNames.cfg").c_str(), "", "#;", "");

	while (!file.IsEOF())
	{
		vector<string> strings = file.ReadTokens();

		if (strings.size())
		{
			if (!g_AutoLoginNames.length())
				g_AutoLoginNames = string("|");

			g_AutoLoginNames += strings[0] + "|";
		}
	}
}
//---------------------------------------------------------------------------
void TOrion::LoadShaders()
{
	CurrentShader = NULL;

#if UO_USE_SHADER_FILES == 1
	TMappedHeader frag;
	memset(&frag, 0, sizeof(TMappedHeader));
	TMappedHeader vert;
	memset(&vert, 0, sizeof(TMappedHeader));

	FileManager.LoadFileToMemory(vert, FilePath("shaders\\Shader.vert").c_str());
	FileManager.LoadFileToMemory(frag, FilePath("shaders\\DeathShader.frag").c_str());

	DeathShader = new TDeathShader((char*)vert.Address, (char*)frag.Address);

	FileManager.UnloadFileFromMemory(frag);



	FileManager.LoadFileToMemory(frag, FilePath("shaders\\ColorizerShader.frag").c_str());

	ColorizerShader = new TColorizerShader((char*)vert.Address, (char*)frag.Address);

	FileManager.UnloadFileFromMemory(frag);



	FileManager.LoadFileToMemory(frag, FilePath("shaders\\FontColorizerShader.frag").c_str());

	FontColorizerShader = new TColorizerShader((char*)vert.Address, (char*)frag.Address);

	FileManager.UnloadFileFromMemory(frag);



	FileManager.LoadFileToMemory(frag, FilePath("shaders\\LightColorizerShader.frag").c_str());

	LightColorizerShader = new TColorizerShader((char*)vert.Address, (char*)frag.Address);

	FileManager.UnloadFileFromMemory(frag);
	FileManager.UnloadFileFromMemory(vert);
#else
	DeathShader = new TDeathShader(g_Vert_ShaderData, g_Frag_DeathShaderData);
	ColorizerShader = new TColorizerShader(g_Vert_ShaderData, g_Frag_ColorizerShaderData);
	FontColorizerShader = new TColorizerShader(g_Vert_ShaderData, g_Frag_FontShaderData);
	LightColorizerShader = new TColorizerShader(g_Vert_ShaderData, g_Frag_LightShaderData);
#endif
}
//---------------------------------------------------------------------------
bool TOrion::AutoLoginNameExists(string name)
{
	if (!g_AutoLoginNames.length())
		return false;

	string search = string("|") + name + "|";
	
	return (g_AutoLoginNames.find(search) != string::npos);
}
//---------------------------------------------------------------------------
void TOrion::LoadClientConfig()
{
	TMappedHeader file;
	memset(&file, 0, sizeof(file));

	if (FileManager.LoadFileToMemory(file, FilePath("Client.cuo").c_str()))
	{
		BYTE ver = file.ReadByte();

		ConnectionManager.EncryptionType = (ENCRYPTION_TYPE)file.ReadByte();

		ConnectionManager.ClientVersion = (CLIENT_VERSION)file.ReadByte();
		PacketManager.SetClientVersion(ConnectionManager.ClientVersion);

		int len = file.ReadByte();

		g_ClientVersionText = file.ReadString(len);

		g_CryptKey[0] = file.ReadDWord();
		g_CryptKey[1] = file.ReadDWord();
		g_CryptKey[2] = file.ReadDWord();

		g_SeedKey = file.ReadWord();

		g_InverseBuylist = file.ReadByte();

		IFOR(i, 0, 6)
		{
			g_MapSizeX[i] = file.ReadWord();
			g_MapSizeY[i] = file.ReadWord();

			g_MapBlockX[i] = g_MapSizeX[i] / 8;
			g_MapBlockY[i] = g_MapSizeY[i] / 8;
		}

		if (ver >= 2)
		{
			CharacterList.ClientFlag = file.ReadByte();

			if (ver >= 3)
				FileManager.UseVerdata = (file.ReadByte() != 0);
		}
		else
			CharacterList.ClientFlag = 0;

		FileManager.UnloadFileFromMemory(file);
	}
	else
	{
		g_ClientVersionText = "2.0.3";
		g_InverseBuylist = false;
		memset(g_CryptKey, 0, sizeof(g_CryptKey));
		g_SeedKey = 1357;
		CharacterList.ClientFlag = 0;
	}
}
//---------------------------------------------------------------------------
DWORD TOrion::GetFileHashCode(DWORD address, DWORD size)
{
	DWORD crc = 0xFFFFFFFF;

	PBYTE ptr = (PBYTE)address;

	while (size > 0)
	{
		crc = (crc >> 8) ^ m_CRC_Table[(crc & 0xFF) ^ *ptr];

		ptr++;
		size--;
	}

	return (crc & 0xFFFFFFFF);
}
//---------------------------------------------------------------------------
void TOrion::LoadPluginConfig()
{
	PluginClientInterface.GL = &g_Interface_GL;
	PluginClientInterface.UO = &g_Interface_UO;
	PluginClientInterface.ClilocManager = &g_Interface_ClilocManager;
	PluginClientInterface.ColorManager = &g_Interface_ColorManager;
	PluginClientInterface.PathFinder = &g_Interface_PathFinder;

	TMappedHeader file;
	memset(&file, 0, sizeof(file));

	if (FileManager.LoadFileToMemory(file, FilePath("Client.cuo").c_str()))
	{
		BYTE ver = file.ReadByte();

		file.Move(2);
		int len = file.ReadByte();
		file.Move(len + 39);

		if (ver >= 2)
		{
			file.Move(1);
			
			if (ver >= 3)
				file.Move(1);

			char count = file.ReadChar();

			IFOR(i, 0, count)
			{
				short len = file.ReadShort();
				string name = file.ReadString(len);

				DWORD password = file.ReadWord() << 16;
				DWORD flags = file.ReadDWord();
				password |= file.ReadWord();

				len = file.ReadShort();
				string subName = file.ReadString(len);
				
				HMODULE dll = LoadLibraryA(FilePath(name).c_str());

				if (dll != NULL)
				{
					typedef void __cdecl dllFunc(PPLUGIN_INTERFACE);

					dllFunc *initFunc = (dllFunc*)GetProcAddress(dll, subName.c_str());
					TPlugin *plugin = NULL;

					if (initFunc != NULL)
					{
						plugin = new TPlugin(flags);

						initFunc(plugin->m_PPS);

						if (password && (DWORD)plugin->m_PPS->Owner != password)
						{
							delete plugin;
							plugin = NULL;
						}
					}

					if (plugin == NULL)
						FreeLibrary(dll);
					else
					{
						plugin->m_PPS->Owner = plugin;

						if (plugin->CanClientAccess())
							plugin->m_PPS->Client = &PluginClientInterface;
						
						if (plugin->CanParseRecv())
							plugin->m_PPS->Recv = PluginRecvFunction;

						if (plugin->CanParseSend())
							plugin->m_PPS->Send = PluginSendFunction;

						initFunc(plugin->m_PPS);

						PluginManager->Add(plugin);
					}
				}
			}
		}

		FileManager.UnloadFileFromMemory(file);
	}
}
//---------------------------------------------------------------------------
void TOrion::LoadClientStartupConfig()
{
	ConfigManager.Load(FilePath("options_debug.cuo"));

	SoundManager.SetMusicVolume(ConfigManager.MusicVolume);

	if (ConfigManager.Music)
		PlayMusic(8);
}
//---------------------------------------------------------------------------
void TOrion::LoadStartupConfig()
{
	char buf[MAX_PATH] = {0};
	sprintf(buf, "Desktop\\%s\\%s\\0x%08X\\options_debug.cuo", MainScreen->m_Account->c_str(), ServerList.GetServerName().c_str(), g_PlayerSerial);

	ConfigManager.Load(FilePath(buf));

	SoundManager.SetMusicVolume(ConfigManager.MusicVolume);

	if (!ConfigManager.Sound)
		Orion->AdjustSoundEffects(GetTickCount() + 100000);

	if (!ConfigManager.Music)
		SoundManager.StopMusic();
}
//---------------------------------------------------------------------------
void TOrion::LoadLocalConfig()
{
	if (g_ConfigLoaded)
		return;

	/*char lco[150] = {0};
	sprintf(lco, "Login confirm to %s", ServerList.GetServerName().c_str());
	
	CreateTextMessage(TT_SYSTEM, 0xFFFFFFFF, 3, 0x35, lco);
	CreateTextMessage(TT_SYSTEM, 0xFFFFFFFF, 3, 0, "Welcome to Ultima Online!");*/

	g_CheckContainerStackTimer = GetTickCount() + 1000;

	char buf[MAX_PATH] = {0};
	sprintf(buf, "Desktop\\%s\\%s\\0x%08X", MainScreen->m_Account->c_str(), ServerList.GetServerName().c_str(), g_PlayerSerial);

	string path(buf);

	if (!ConfigManager.Load(path + "\\options_debug.cuo"))
	{
		if (!ConfigManager.Load(FilePath("options_debug.cuo")))
		{
			ConfigManager.Init();

			if (g_GameState >= GS_GAME)
			{
				SendMessage(g_hWnd, WM_SYSCOMMAND, SC_RESTORE, 0);
				SendMessage(g_hWnd, WM_SYSCOMMAND, SC_MAXIMIZE, 0);
			}
		}
	}

	if (!SkillGroupManager.Load(path + "\\skills_debug.cuo"))
		SkillGroupManager.Load(FilePath("skills_debug.cuo"));

	if (!MacroManager->Load(path + "\\macros_debug.cuo", path + "\\macros.txt"))
	{
		if (!MacroManager->Load(FilePath("\\macros_debug.cuo"), FilePath("\\macros.txt")))
		{
			//Создать стандартные макросы
		}
	}

	GumpManager->Load(path + "\\gumps_debug.cuo");

	if (ConfigManager.OffsetInterfaceWindows)
		ContainerRect.MakeDefault();

	if (ConfigManager.GetConsoleNeedEnter())
		EntryPointer = NULL;
	else
		EntryPointer = GameConsole;

	SoundManager.SetMusicVolume(ConfigManager.MusicVolume);

	if (!ConfigManager.Sound)
		Orion->AdjustSoundEffects(GetTickCount() + 100000);

	if (!ConfigManager.Music)
		SoundManager.StopMusic();

	g_ConfigLoaded = true;
}
//---------------------------------------------------------------------------
void TOrion::SaveLocalConfig()
{
	if (!g_ConfigLoaded)
		return;

	string path = FilePath("Desktop");
	CreateDirectoryA(path.c_str(), NULL);
	
	path += string("\\") + MainScreen->m_Account->c_str();
	CreateDirectoryA(path.c_str(), NULL);
	
	path += string("\\") + ServerList.GetServerName();
	CreateDirectoryA(path.c_str(), NULL);

	char serbuf[20] = {0};
	sprintf(serbuf, "\\0x%08X", g_PlayerSerial);
	
	path += serbuf;
	CreateDirectoryA(path.c_str(), NULL);

	ConfigManager.Save(path + "\\options_debug.cuo");
	SkillGroupManager.Save(path + "\\skills_debug.cuo");
	MacroManager->Save(path + "\\macros_debug.cuo");
	GumpManager->Save(path + "\\gumps_debug.cuo");

	ConfigManager.Save(FilePath("options_debug.cuo").c_str());
	MacroManager->Save(FilePath("macros_debug.cuo").c_str());

	path += string("_") + g_Player->GetName() + ".cuo";
	
	if (!PathFileExistsA(path.c_str()))
	{
		FILE *file = fopen(path.c_str(), "wb");
		fclose(file);
	}
}
//---------------------------------------------------------------------------
void TOrion::ProcessDelayedClicks()
{
	if (g_ClickObjectReq && g_ClickObject.Timer < GetTickCount())
	{
		if (g_ClickObject.Type == COT_GAME_OBJECT)
		{
			NameReq(g_ClickObject.Serial);

			if (g_ClickObject.Serial < 0x40000000)
			{
				if (!ConfigManager.HoldShiftForContextMenus || g_ShiftPressed)
				{
					TPacketRequestPopupMenu packet(g_ClickObject.Serial);
					packet.Send();
				}
			}
		}
		else if (g_ClickObject.Type == COT_GUMP)
		{
			if (g_ClickObject.GumpType == GT_SPELLBOOK)
			{
				TGumpSpellbook *gump = (TGumpSpellbook*)GumpManager->UpdateGump(g_ClickObject.Serial, g_ClickObject.GumpID, g_ClickObject.GumpType);

				if (gump != NULL)
				{
					gump->Page = g_ClickObject.GumpButtonID;
					Orion->PlaySoundEffect(0x0055);
				}
			}
			else if (g_ClickObject.GumpType == GT_BOOK)
			{
				TGumpBook *gump = (TGumpBook*)GumpManager->UpdateGump(g_ClickObject.Serial, g_ClickObject.GumpID, g_ClickObject.GumpType);

				if (gump != NULL)
					gump->ChangePage(g_ClickObject.GumpButtonID);
			}
			else if (g_ClickObject.GumpType == GT_PAPERDOLL)
			{
				TGumpPaperdoll *gump = (TGumpPaperdoll*)GumpManager->UpdateGump(g_ClickObject.Serial, 0, g_ClickObject.GumpType);

				if (gump != NULL)
				{
					TTextData *td = new TTextData();
					td->Unicode = false;
					td->Font = 3;
					td->Serial = 0;
					td->Color = 0x03B5;
					td->Timer = GetTickCount();
					td->Type = TT_CLIENT;
					td->DrawX = g_MouseX - gump->X;
					td->DrawY = g_MouseY - gump->Y;

					string text = "Party Manifest";
					if (g_ClickObject.GumpButtonID)
						text = "Character Profile";

					td->SetText(text);

					int width = FontManager->GetWidthA(3, text.c_str(), text.length());

					FontManager->SavePixels = true;

					if (width > TEXT_MESSAGE_MAX_WIDTH)
						td->GenerateTexture(TEXT_MESSAGE_MAX_WIDTH, 0, TS_CENTER);
					else
						td->GenerateTexture(0, 0, TS_CENTER);

					FontManager->SavePixels = false;

					gump->m_TextContainer->Add(td);

					TTextRenderer *tr = gump->GetTextRenderer();

					if (tr != NULL)
						tr->Add(td);

					Orion->AddJournalMessage(td, "");
				}
			}
		}

		g_ClickObjectReq = false;
	}
}
//---------------------------------------------------------------------------
void TOrion::Process()
{
	static DWORD removeUnusedTexturesTime = GetTickCount() + CLEAR_TEXTURES_DELAY;
	static DWORD processGameObjectsTimer = GetTickCount();

	ConnectionManager.Recv();
	PacketManager.SendMegaClilocRequests();

	DWORD ticks = GetTickCount();
	
	if (g_GameState >= GS_CHARACTER && g_LastSendTime + SEND_TIMEOUT_DELAY < ticks)
	{
		g_PingCount++;

		BYTE ping[2] = {0x73, g_PingSequence};

		Send(ping, 2);
	}

	g_AltPressed = GetAsyncKeyState(VK_MENU) & 0x80000000;
	g_CtrlPressed = GetAsyncKeyState(VK_CONTROL) & 0x80000000;
	g_ShiftPressed = GetAsyncKeyState(VK_SHIFT) & 0x80000000;

	if (g_GameState >= GS_GAME) // || g_GameState == GS_GAME_BLOCKED)
	{
		g_ShowGumpLocker = ConfigManager.LockGumpsMoving && g_AltPressed && g_CtrlPressed;

		g_SelectedObject = NULL;
		g_SelectedTextObject = NULL;

		if (processGameObjectsTimer <= ticks)
		{
			TWalkData *wd = g_Player->m_WalkStack.m_Items;

			if (wd != NULL)
			{
				g_RemoveRangeXY.x = wd->X;
				g_RemoveRangeXY.y = wd->Y;
			}
			else
			{
				g_RemoveRangeXY.x = g_Player->X;
				g_RemoveRangeXY.y = g_Player->Y;
			}

			ProcessStaticAnimList();
			RemoveRangedObjects();

			EffectManager->UpdateEffects();

			TGumpBuff *gumpBuff = (TGumpBuff*)GumpManager->GetGump(g_PlayerSerial, 0, GT_BUFF);

			if (gumpBuff != NULL)
				gumpBuff->UpdateBuffIcons();

			processGameObjectsTimer = ticks + 30;
		}

		if (abs((int)processGameObjectsTimer - (int)ticks) > 500)
			processGameObjectsTimer = ticks + 30;

		World->ProcessAnimation();

		PathFinder->ProcessAutowalk();

		bool canRenderSelect = false;

		if (g_GameState == GS_GAME)
		{
			MouseManager.ProcessWalking();

			MacroManager->Execute();

			ProcessDelayedClicks();

			canRenderSelect = true;

			if (g_LastObjectType == SOT_GAME_GUMP_SCOPE)
			{
				if (g_LeftMouseDown && !g_LastGumpLeftMouseDown && !g_LastGumpRightMouseDown)
				{
					if (g_LastObjectLeftMouseDown == 1 || g_LastObjectLeftMouseDown == 2)
						canRenderSelect = false;
				}
			}
		}
		
		if (g_LastRenderTime <= ticks)
		{
			GameScreen->CalculateGameWindowBounds();

			GameScreen->CalculateRenderList();
			GameScreen->RenderListInitalized = true;


			if (!IsIconic(g_hWnd))
			{
				if (canRenderSelect)
					GameScreen->Render(false);

				GameScreen->Render(true);
			}

			GameScreen->RenderListInitalized = false;

			MapManager->Init(true);

			for (CORPSE_LIST_MAP::iterator i = g_CorpseSerialList.begin(); i != g_CorpseSerialList.end(); i++)
			{
				if (i->second < ticks)
					i = g_CorpseSerialList.erase(i);
			}
		}
	}
	else if (g_LastRenderTime <= ticks)
	{
		if (!IsIconic(g_hWnd))
		{
			CurrentScreen->Render(false);
			CurrentScreen->Render(true);
		}
	}

	if (removeUnusedTexturesTime < ticks)
	{
		ClearUnusedTextures();
		removeUnusedTexturesTime = ticks + CLEAR_TEXTURES_DELAY;
	}
}
//---------------------------------------------------------------------------
int TOrion::Send(PBYTE buf, int size)
{
	DWORD ticks = GetTickCount();
	g_TotalSendSize += size;
	
	TMessageType &type = PacketManager.GetType(*buf);

	TPRINT("--- ^(%d) s(+%d => %d) Client:: %s\n", ticks - g_LastPacketTime, size, g_TotalSendSize, type.Name);

	if (*buf == 0x80 || *buf == 0x91)
	{
		TDUMP(buf, size / 2);
		TPRINT("**** PASSWORD CENSORED ****\n");
	}
	else
		TDUMP(buf, size);
	
	g_LastPacketTime = ticks;
	g_LastSendTime = ticks;

	if (type.Direction != DIR_SEND && type.Direction != DIR_BOTH)
		WPRINT("message direction invalid: 0x%02X\n", *buf);
	else
	{
		/*if (g_ClientPaused)
		{
			PACKET_DATA pd;
			pd.buf = (PBYTE)malloc(size);
			memcpy(pd.buf, buf, size);
			pd.size = size;
			m_PacketStream.push_back(pd);
			TPRINT("Packet backup to resume client\n");
			return size;
		}
		else*/
		if (PluginManager->PacketSend(buf, size))
			return ConnectionManager.Send(buf, size);
	}

	return 0;
}
//---------------------------------------------------------------------------
TTextureObject *TOrion::ExecuteGump(const WORD &id, const bool &partialHue)
{
	TIndexObject &io = m_GumpDataIndex[id];

	if (io.Texture == NULL)
	{
		if (!io.Address)
			return NULL;

		io.Texture = MulReader.ReadGump(io);

		if (io.Texture != NULL)
		{
			ADD_LINKED(m_UsedGumpList, m_GumpDataIndex[id]);
		}
	}

	io.LastAccessTime = GetTickCount();

	return io.Texture;
}
//---------------------------------------------------------------------------
TTextureObject *TOrion::ExecuteLandArt(const WORD &id)
{
	TIndexObject &io = m_LandDataIndex[id];

	if (io.Texture == NULL)
	{
		if (!io.Address || id == 0x02) //nodraw tiles banned
			return NULL;

		io.Texture = MulReader.ReadArt(id, io);

		if (io.Texture != NULL)
		{
			ADD_LINKED(m_UsedLandList, m_LandDataIndex[id]);
		}
	}
	
	io.LastAccessTime = GetTickCount();

	return io.Texture;
}
//---------------------------------------------------------------------------
TTextureObject *TOrion::ExecuteStaticArtAnimated(const WORD &id)
{
	return ExecuteStaticArt(id + m_StaticDataIndex[id].Increment);
}
//---------------------------------------------------------------------------
TTextureObject *TOrion::ExecuteStaticArt(const WORD &id)
{
	TIndexObject &io = m_StaticDataIndex[id];

	if (io.Texture == NULL)
	{
		if (!io.Address || id == 0x01) //nodraw tiles banned
			return NULL;

		io.Texture = MulReader.ReadArt(id + 0x4000, io);

		if (io.Texture != NULL)
		{
			io.Width = ((io.Texture->Width / 2) + 1);

			io.Height = io.Texture->Height - 20;

			ADD_LINKED(m_UsedStaticList, m_StaticDataIndex[id]);
		}
	}
	
	io.LastAccessTime = GetTickCount();

	return io.Texture;
}
//---------------------------------------------------------------------------
TTextureObject *TOrion::ExecuteTexture(WORD id)
{
	id = m_LandData[id / 32].Tiles[id % 32].TexID;

	if (!id)
		return NULL;

	TIndexObject &io = m_TextureDataIndex[id];

	if (io.Texture == NULL)
	{
		if (!io.Address)
			return NULL;

		io.Texture = MulReader.ReadTexture(io);

		if (io.Texture != NULL)
		{
			ADD_LINKED(m_UsedTextureList, m_TextureDataIndex[id]);
		}
	}
	
	io.LastAccessTime = GetTickCount();

	return io.Texture;
}
//---------------------------------------------------------------------------
TTextureObject *TOrion::ExecuteLight(BYTE &id)
{
	if (id >= MAX_LIGHTS_DATA_INDEX_COUNT)
		id = 0;
	
	TIndexObject &io = m_LightDataIndex[id];

	if (io.Texture == NULL)
	{
		if (!io.Address)
			return NULL;

		io.Texture = MulReader.ReadLight(io);

		if (io.Texture != NULL)
		{
			ADD_LINKED(m_UsedLightList, m_LightDataIndex[id]);
		}
	}
	
	io.LastAccessTime = GetTickCount();

	return io.Texture;
}
//---------------------------------------------------------------------------
bool TOrion::ExecuteGumpPart(const WORD &id, const int &count)
{
	bool result = true;

	IFOR(i, 0, count)
	{
		if (ExecuteGump(id + i, 0) == NULL)
			result = false;
	}

	return result;
}
//---------------------------------------------------------------------------
void TOrion::DrawSphereGump(const int &value, const float &maxValue, const int &x, const int &y, const int &width)
{
	DrawGump(0x00D5, 0, x + 64, y); //Sphere line gump start
	DrawGump(0x00D7, 0, x + 152, y); //Sphere line gump end
	DrawGump(0x00D6, 0, x + 77, y, 75, 0); //Sphere line gump body

	int ofs = CalculateSphereOffset(100, value, width, maxValue);

	DrawGump(0x00D8, 0, x + 64 + ofs, y); //Sphere gump
}
//---------------------------------------------------------------------------
void TOrion::DrawGump(const WORD &id, const WORD &color, const int &x, const int &y, const bool &partialHue)
{
	TTextureObject *th = ExecuteGump(id);

	if (th != NULL)
	{
		int drawMode = (!g_GrayedPixels && color);

		if (drawMode)
		{
			if (partialHue)
				drawMode = 2;

			ColorManager->SendColorsToShader(color);
		}

		glUniform1iARB(ShaderDrawMode, drawMode);

		g_GL.Draw(th->Texture, x, y, th->Width, th->Height);
	}
}
//---------------------------------------------------------------------------
void TOrion::DrawGump(const WORD &id, const WORD &color, const int &x, const int &y, int width, int height, const bool &partialHue)
{
	TTextureObject *th = ExecuteGump(id);

	if (th != NULL)
	{
		int DW = th->Width;
		int DH = th->Height;

		if (width == 0)
			width = DW;
		if (height == 0)
			height = DH;

		int drawMode = (!g_GrayedPixels && color);

		if (drawMode)
		{
			if (partialHue)
				drawMode = 2;

			ColorManager->SendColorsToShader(color);
		}

		glUniform1iARB(ShaderDrawMode, drawMode);

		g_GL.Draw(th->Texture, x, y, DW, DH, width, height);
	}
}
//---------------------------------------------------------------------------
void TOrion::DrawResizepicGump(const WORD &id, const int &x, const int &y, const int &width, const int &height)
{
	TTextureObject *th[9] = {NULL};

	IFOR(i, 0, 9)
	{
		TTextureObject *pth = ExecuteGump(id + i);

		if (pth == NULL)
			return;

		if (i == 4)
			th[8] = pth;
		else if (i > 4)
			th[i - 1] = pth;
		else
			th[i] = pth;
	}

	g_GL.DrawResizepic(th, x, y, width, height);
}
//---------------------------------------------------------------------------
void TOrion::DrawLandTexture(const WORD &id, WORD color, const int &x, const int &y, RECT &rc, TVector *normals)
{
	if (id == 2)
		DrawLandArt(id, color, x, y, rc.left / 4);
	else
	{
		TTextureObject *th = ExecuteTexture(id);

		if (th == NULL)
			DrawLandArt(id, color, x, y, rc.left / 4);
		else
		{
			if (g_OutOfRangeColor)
				color = g_OutOfRangeColor;

			int drawMode = 6;

			if (!g_GrayedPixels && color)
			{
				drawMode = 7;
				ColorManager->SendColorsToShader(color);
			}

			glUniform1iARB(ShaderDrawMode, drawMode);

			g_GL.DrawLandTexture(th->Texture, x, y, rc, normals);
		}
	}
}
//---------------------------------------------------------------------------
void TOrion::DrawLandArt(const WORD &id, WORD color, const int &x, const int &y, const int &z)
{
	TTextureObject *th = ExecuteLandArt(id);

	if (th != NULL)
	{
		if (g_OutOfRangeColor)
			color = g_OutOfRangeColor;

		int drawMode = (!g_GrayedPixels && color);

		if (drawMode)
			ColorManager->SendColorsToShader(color);

		glUniform1iARB(ShaderDrawMode, drawMode);

		g_GL.Draw(th->Texture, x - 23, y - (23 + (z * 4)), th->Width, th->Height);
	}
}
//---------------------------------------------------------------------------
void TOrion::DrawStaticArt(const WORD &id, WORD color, const int &x, const int &y, const int &z, const bool &selection)
{
	TTextureObject *th = ExecuteStaticArt(id);

	if (th != NULL && id > 1)
	{
		if (g_OutOfRangeColor)
			color = g_OutOfRangeColor;
		
		int drawMode = (!g_GrayedPixels && color);

		if (drawMode)
		{
			bool partialHue = (!selection && IsPartialHue(GetStaticFlags(id)));

			if (partialHue)
				drawMode = 2;

			ColorManager->SendColorsToShader(color);
		}

		glUniform1iARB(ShaderDrawMode, drawMode);

		g_GL.Draw(th->Texture, x - m_StaticDataIndex[id].Width, y - (m_StaticDataIndex[id].Height + (z * 4)), th->Width, th->Height);
	}
}
//---------------------------------------------------------------------------
void TOrion::DrawStaticArtAnimated(const WORD &id, const WORD &color, const int &x, const int &y, const int &z, const bool &selection)
{
	DrawStaticArt(id + m_StaticDataIndex[id].Increment, color, x, y, z, selection);
}
//---------------------------------------------------------------------------
void TOrion::DrawStaticArtRotated(const WORD &id, WORD color, const int &x, const int &y, const int &z, const float &angle)
{
	TTextureObject *th = ExecuteStaticArt(id);

	if (th != NULL && id > 1)
	{
		if (g_OutOfRangeColor)
			color = g_OutOfRangeColor;

		int drawMode = (!g_GrayedPixels && color);

		if (drawMode)
			ColorManager->SendColorsToShader(color);

		glUniform1iARB(ShaderDrawMode, drawMode);

		g_GL.DrawRotated(th->Texture, x, y - (z * 4), th->Width, th->Height / 2, angle);
	}
}
//---------------------------------------------------------------------------
void TOrion::DrawStaticArtAnimatedRotated(const WORD &id, const WORD &color, const int &x, const int &y, const int &z, const float &angle)
{
	DrawStaticArtRotated(id + m_StaticDataIndex[id].Increment, color, x, y, z, angle);
}
//---------------------------------------------------------------------------
void TOrion::DrawStaticArtTransparent(const WORD &id, WORD color, int x, int y, const int &z, const bool &selection)
{
	TTextureObject *th = ExecuteStaticArt(id);

	if (th != NULL && id > 1)
	{
		if (g_OutOfRangeColor)
			color = g_OutOfRangeColor;
		
		x -= m_StaticDataIndex[id].Width;
		y -= m_StaticDataIndex[id].Height;

		int drawMode = (!g_GrayedPixels && color);

		if (drawMode)
		{
			bool partialHue = (!selection && IsPartialHue(GetStaticFlags(id)));

			if (partialHue)
				drawMode = 2;

			ColorManager->SendColorsToShader(color);
		}

		glUniform1iARB(ShaderDrawMode, drawMode);

		y -= (z * 4);

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glColor4f(1.0f, 1.0f, 1.0f, 0.25f);
		
		g_GL.Draw(th->Texture, x, y, th->Width, th->Height);

		glDisable(GL_BLEND);

		glEnable(GL_STENCIL_TEST);
		
		g_GL.Draw(th->Texture, x, y, th->Width, th->Height);

		glDisable(GL_STENCIL_TEST);
	}
}
//---------------------------------------------------------------------------
void TOrion::DrawStaticArtAnimatedTransparent(const WORD &id, const WORD &color, const int &x, const int &y, const int &z, const bool &selection)
{
	DrawStaticArtTransparent(id + m_StaticDataIndex[id].Increment, color, x, y, z, selection);
}
//---------------------------------------------------------------------------
void TOrion::DrawStaticArtInContainer(const WORD &id, WORD color, int x, int y, const bool &selection, const bool &onMouse)
{
	TTextureObject *th = ExecuteStaticArt(id);

	if (th != NULL)
	{
		int DW = th->Width;
		int DH = th->Height;

		if (onMouse)
		{
			x -= (DW / 2);
			y -= (DH / 2);
		}

		int drawMode = (!g_GrayedPixels && color);

		if (drawMode)
		{
			bool partialHue = (!selection && IsPartialHue(GetStaticFlags(id)));

			if (color >= 0x4000)
				color = 0x100;
			else if (partialHue)
				drawMode = 2;

			ColorManager->SendColorsToShader(color);
		}

		glUniform1iARB(ShaderDrawMode, drawMode);

		g_GL.Draw(th->Texture, x, y, DW, DH);
	}
}
//---------------------------------------------------------------------------
void TOrion::DrawLight(BYTE id, const WORD &color, const int &x, const int &y)
{
	TTextureObject *th = ExecuteLight(id);

	if (th != NULL)
	{
		int drawMode = 0;

		if (color)
		{
			drawMode = 1;

			ColorManager->SendColorsToShader(color);
		}

		glUniform1iARB(ShaderDrawMode, drawMode);

		g_GL.Draw(th->Texture, x - (th->Width / 2), y - (th->Height / 2), th->Width, th->Height);
	}
}
//---------------------------------------------------------------------------
bool TOrion::PolygonePixelsInXY(int x, int y, const int &width, const int &height)
{
	x = g_MouseX - x;
	y = g_MouseY - y;

	return !(x < 0 || y < 0 || x >= width || y >= height);
}
//---------------------------------------------------------------------------
bool TOrion::GumpPixelsInXY(const WORD &id, int x, int y, const bool &noSubMouse)
{
	TIndexObject &io = m_GumpDataIndex[id];

	bool result = false;

	TTextureObject *th = io.Texture;
	if (th != NULL)
	{
		if (!noSubMouse)
		{
			x = g_MouseX - x;
			y = g_MouseY - y;
		}

#if UO_ENABLE_TEXTURE_DATA_SAVING == 1
		if (x >= 0 && y >= 0 && x < th->Width && y < th->Height)
			result = th->Data[(y * th->Width) + x] != 0;
#else
		result = MulReader.GumpPixelsInXY(io, x, y);
#endif
	}

	return result;
}
//---------------------------------------------------------------------------
bool TOrion::GumpPixelsInXY(const WORD &id, int x, int y, int width, int height, const bool &noSubMouse)
{
	if (!noSubMouse)
	{
		x = g_MouseX - x;
		y = g_MouseY - y;
	}

	if (x < 0 || y < 0 || (width > 0 && x >= width) || (height > 0 && y >= height))
		return false;
	
	TIndexObject &io = m_GumpDataIndex[id];

	TTextureObject *th = io.Texture;
	if (th == NULL)
		return false;
	
	if (width == 0)
		width = th->Width;

	if (height == 0)
		height = th->Height;
	
	while (x > th->Width && width > th->Width)
	{
		x -= th->Width;
		width -= th->Width;
	}

	while (y > th->Height && height > th->Height)
	{
		y -= th->Height;
		height -= th->Height;
	}

	if (x > width || y > height)
		return false;

	bool result = false;

#if UO_ENABLE_TEXTURE_DATA_SAVING == 1
	if (x >= 0 && y >= 0 && x < th->Width && y < th->Height)
		result = th->Data[(y * th->Width) + x] != 0;
#else
	result = MulReader.GumpPixelsInXY(io, x, y);
#endif

	return result;
}
//---------------------------------------------------------------------------
bool TOrion::ResizepicPixelsInXY(const WORD &id, int x, int y, const int &width, const int &height)
{
	x = g_MouseX - x;
	y = g_MouseY - y;

	if (x < 0 || y < 0 || x >= width || y >= height)
		return false;

	TTextureObject *th[9] = {NULL};

	IFOR(i, 0, 9)
	{
		TTextureObject *pth = m_GumpDataIndex[id + i].Texture;

		if (pth == NULL)
			return false;

		if (i == 4)
			th[8] = pth;
		else if (i > 4)
			th[i - 1] = pth;
		else
			th[i] = pth;
	}
	
	IFOR(i, 0, 9)
	{
		switch (i)
		{
			case 0:
			{
				if (GumpPixelsInXY(id, x, y, true))
					return true;
				break;
			}
			case 1:
			{
				int DW = width - th[0]->Width - th[2]->Width;
				if (DW < 1)
					break;

				if (GumpPixelsInXY(id + 1, x - th[0]->Width, y, DW, 0, true))
					return true;

				break;
			}
			case 2:
			{
				if (GumpPixelsInXY(id + 2, x - width + th[i]->Width, y, true))
					return true;

				break;
			}
			case 3:
			{
				int DH = height - th[0]->Height - th[5]->Height;
				if (DH < 1)
					break;

				if (GumpPixelsInXY(id + 3, x, y - th[0]->Height, 0, DH, true))
					return true;

				break;
			}
			case 4:
			{
				int DH = height - th[2]->Height - th[7]->Height;
				if (DH < 1)
					break;

				if (GumpPixelsInXY(id + 5, x - width + th[i]->Width, y - th[2]->Height, 0, DH, true))
					return true;

				break;
			}
			case 5:
			{
				if (GumpPixelsInXY(id + 6, x, y - height + th[i]->Height, true))
					return true;

				break;
			}
			case 6:
			{
				int DW = width - th[5]->Width - th[7]->Width;
				if (DW < 1)
					break;

				if (GumpPixelsInXY(id + 7, x - th[5]->Width, y - height + th[i]->Height, DW, 0, true))
					return true;

				break;
			}
			case 7:
			{
				if (GumpPixelsInXY(id + 8, x - width + th[i]->Width, y - height + th[i]->Height, true))
					return true;

				break;
			}
			case 8:
			{
				int DW = width - th[0]->Width - th[2]->Width;

				if (DW < 1)
					break;

				int DH = height - th[2]->Height - th[7]->Height;

				if (DH < 1)
					break;

				if (GumpPixelsInXY(id + 4, x - th[0]->Width, y - th[0]->Height, DW, DH, true))
					return true;

				break;
			}
			default:
				break;
		}
	}

	return false;
}
//---------------------------------------------------------------------------
bool TOrion::StaticPixelsInXY(const WORD &id, int x, int y, const int &z)
{
	TIndexObject &io = m_StaticDataIndex[id];

	bool result = false;

	TTextureObject *th = io.Texture;
	if (th != NULL)
	{
		x = (g_MouseX - x) + io.Width;
		y = (g_MouseY - y) + io.Height + (z * 4);

#if UO_ENABLE_TEXTURE_DATA_SAVING == 1
		if (x >= 0 && y >= 0 && x < th->Width && y < th->Height)
			result = th->Data[(y * th->Width) + x] != 0;
#else
		result = MulReader.ArtPixelsInXY(false, io, x, y);
#endif
	}

	return result;
}
//---------------------------------------------------------------------------
bool TOrion::StaticPixelsInXYAnimated(const WORD &id, const int &x, const int &y, const int &z)
{
	return StaticPixelsInXY(id + m_StaticDataIndex[id].Increment, x, y, z);
}
//---------------------------------------------------------------------------
bool TOrion::CircleTransPixelsInXY()
{
	int x = (g_MouseX - g_CircleOfTransparency.X);
	int y = (g_MouseY - g_CircleOfTransparency.Y);

	bool result = false;

	if (x >= 0 && y >= 0 && x < g_CircleOfTransparency.Width && y < g_CircleOfTransparency.Height)
	{
		int pos = (y * g_CircleOfTransparency.Width) + x;
		result = (g_CircleOfTransparency.PixelData[pos] != 0);
	}

	return result;
}
//---------------------------------------------------------------------------
bool TOrion::StaticPixelsInXYInContainer(const WORD &id, int x, int y)
{
	TIndexObject &io = m_StaticDataIndex[id];

	bool result = false;

	TTextureObject *th = io.Texture;
	if (th != NULL)
	{
		x = g_MouseX - x;
		y = g_MouseY - y;

#if UO_ENABLE_TEXTURE_DATA_SAVING == 1
		if (x >= 0 && y >= 0 && x < th->Width && y < th->Height)
			result = th->Data[(y * th->Width) + x] != 0;
#else
		result = MulReader.ArtPixelsInXY(false, io, x, y);
#endif
	}

	return result;
}
//---------------------------------------------------------------------------
bool TOrion::LandPixelsInXY(const WORD &id, int x, int  y, const int &z)
{
	TIndexObject &io = m_LandDataIndex[id];

	bool result = false;

	TTextureObject *th = io.Texture;
	if (th != NULL)
	{
		x = (g_MouseX - x) + 23;
		y = (g_MouseY - y) + 23 + (z * 4);

#if UO_ENABLE_TEXTURE_DATA_SAVING == 1
		if (x >= 0 && y >= 0 && x < th->Width && y < th->Height)
			result = th->Data[(y * th->Width) + x] != 0;
#else
		result = MulReader.ArtPixelsInXY(true, io, x, y);
#endif
	}

	return result;
}
//---------------------------------------------------------------------------
bool TOrion::LandTexturePixelsInXY(int x, int  y, RECT &r)
{
	y -= 23;
	int testX = g_MouseX - x;

	int y0 = -r.left;
	int y1 = 22 - r.top;
	int y2 = 44 - r.right;
	int y3 = 22 - r.bottom;
	
	bool result = (
		(g_MouseY >= testX * (y1 - y0) / -22 + y + y0) &&
		(g_MouseY >=testX * (y3 - y0) / 22 + y + y0) &&
		(g_MouseY <= testX * (y3 - y2) / 22 + y+ y2) &&
		(g_MouseY <= testX * (y1 - y2) / -22 + y + y2)
		);
	
	return result;
}
//--------------------------------------------------------------------------
DWORD TOrion::GetLandFlags(const WORD &id)
{
	WORD divID = id / 32;
	if (divID < 512)
		return m_LandData[divID].Tiles[id % 32].Flags;

	return 0;
}
//--------------------------------------------------------------------------
DWORD TOrion::GetStaticFlags(const WORD &id)
{
	WORD divID = id / 32;
	if (divID < 512)
		return m_StaticData[divID].Tiles[id % 32].Flags;

	return 0;
}
//---------------------------------------------------------------------------
void TOrion::GetArtDimension(const WORD &id, POINT &p)
{
	p.x = 0;
	p.y = 0;

	if (id >= 0x4000) //run
	{
		TTextureObject *th = m_StaticDataIndex[id - 0x4000].Texture;

		if (th != NULL)
		{
			p.x = th->Width;
			p.y = th->Height;
		}
	}
	else //raw
	{
		TTextureObject *th = m_LandDataIndex[id].Texture;

		if (th != NULL)
		{
			p.x = th->Width;
			p.y = th->Height;
		}
	}
}
//---------------------------------------------------------------------------
void TOrion::GetStaticArtRealPixelDimension(const WORD &id, RECT &r)
{
	MulReader.ReadStaticArtPixelDimension(m_StaticDataIndex[id], r);
}
//---------------------------------------------------------------------------
void TOrion::GetGumpDimension(const WORD &id, POINT &p)
{
	TTextureObject *th = m_GumpDataIndex[id].Texture;

	if (th != NULL)
	{
		p.x = th->Width;
		p.y = th->Height;
	}
	else
	{
		p.x = 0;
		p.y = 0;
	}
}
//---------------------------------------------------------------------------
TIndexObjectStatic *TOrion::GetStaticPointer(const WORD &id)
{
	return &m_StaticDataIndex[id];
}
//---------------------------------------------------------------------------
TIndexObject *TOrion::GetGumpPointer(const WORD &id)
{
	return &m_GumpDataIndex[id];
}
//---------------------------------------------------------------------------
TIndexMulti *TOrion::GetMultiPointer(const WORD &id)
{
	return &m_MultiDataIndex[id];
}
//---------------------------------------------------------------------------
int TOrion::GetConfigValue(const char *option, int value)
{
	string key = ToLowerA(option);

	if (key == "alwaysrun")
	{
		if (value == -1)
			value = ConfigManager.AlwaysRun;
		else
			ConfigManager.AlwaysRun = (value != 0);
	}
	else if (key == "circletransvalue")
	{
		if (value == -1)
			value = ConfigManager.CircleTransRadius;
		else
		{
			if (value < 0)
				value = 0;
			else if (value > 255)
				value = 255;

			ConfigManager.CircleTransRadius = value;
		}
	}
	else if (key == "circletrans")
	{
		if (value == -1)
			value = ConfigManager.UseCircleTrans;
		else
			ConfigManager.UseCircleTrans = (value != 0);
	}

	return value;
}
//---------------------------------------------------------------------------
void TOrion::PlaySoundEffect(const WORD &id, float volume)
{
	if (id >= 0x0800 || !ConfigManager.Sound)
		return;

	TIndexSound &is = m_SoundDataIndex[id];
	
	if (is.Address == 0)
		return;

	if (is.hStream == 0)
	{
		is.hStream = SoundManager.LoadSoundEffect(m_SoundDataIndex[id]);

		if (is.hStream == 0)
			return;

		ADD_LINKED(m_UsedSoundList, m_SoundDataIndex[id]);
	}
	else {
		if (is.LastAccessTime + is.Timer > GetTickCount())
			return;

		SoundManager.FreeStream(is.hStream);

		is.hStream = SoundManager.LoadSoundEffect(m_SoundDataIndex[id]);
	}	

	if (volume <= 0)
		volume = SoundManager.GetVolumeValue();

	if (volume > 0)
	{
		SoundManager.PlaySoundEffect(is.hStream, volume);
		is.LastAccessTime = GetTickCount();
	}

	//if (is.Sound == NULL)
	//{
	//	is.Sound = SoundManager.LoadSoundEffect(m_SoundDataIndex[id]);

	//	if (is.Sound == NULL)
	//		return;

	//	ADD_LINKED(m_UsedSoundList, m_SoundDataIndex[id]);
	//}
	//else
	//{
	//	if (is.LastAccessTime + is.Timer > GetTickCount())
	//		return;

	//	Mix_FreeChunk(is.Sound);
	//	is.Sound = SoundManager.LoadSoundEffect(m_SoundDataIndex[id]);
	//}

	//if (volume < 0)
	//	volume = ConfigManager.SoundVolume;

	//if (volume > 0)
	//{
	//	SoundManager.PlaySoundEffect(is.Sound, volume);
	//	is.LastAccessTime = GetTickCount();
	//}
}
//---------------------------------------------------------------------------
void TOrion::PlayMusic(int index, bool warmode)
{
	//Тимур, здесь прикручивай взависимости от конфига играть мп3 или миди.
	//Сейчас только мп3 будет играть.
	if (index >= MAX_MP3_COUNT) return;
	TMP3Struct &mp3Info = m_MP3Data[index];
	SoundManager.PlayMP3(mp3Info.FileName, mp3Info.Loop, warmode);
	//SoundManager.PlayMidi(index);
}
//---------------------------------------------------------------------------
void TOrion::AdjustSoundEffects(DWORD ticks, float volume)
{
	TLinkedList *list = m_UsedSoundList;
	TLinkedList *prev = list;

	while (list != NULL)
	{
		TIndexSound *obj = (TIndexSound*)list->Data;

		if (obj->hStream != 0 && obj->LastAccessTime + obj->Timer < ticks)
		{
			if (volume > 0)
			{
				BASS_ChannelSetAttribute(obj->hStream, BASS_ATTRIB_VOL, volume);
			}
			else
			{
				BASS_StreamFree(obj->hStream);

				//Mix_FreeChunk(obj->Sound);
				//obj->Sound = NULL;
				obj->hStream = 0;
			}
			TLinkedList *next = list->Next;
			if (list == m_UsedSoundList)
			{
				m_UsedSoundList = m_UsedSoundList->Next;
				next = m_UsedSoundList;
			}
			else
				prev->Next = list->Next;

			list->Next = NULL;
			delete list;

			list = next;
		}
		else
		{
			prev = list;
			list = list->Next;
		}
	}
}
//---------------------------------------------------------------------------
void TOrion::CreateTextMessageF(BYTE font, WORD color, const char *format, ...)
{
	va_list arg;
	va_start(arg, format);

	char buf[256] = { 0 };
	vsprintf(buf, format, arg);

	Orion->CreateTextMessage(TT_SYSTEM, 0xFFFFFFFF, font, color, buf);

	va_end(arg);
}
//---------------------------------------------------------------------------
void TOrion::CreateUnicodeTextMessageF(BYTE font, WORD color, const char *format, ...)
{
	va_list arg;
	va_start(arg, format);

	char buf[256] = { 0 };
	vsprintf(buf, format, arg);

	Orion->CreateUnicodeTextMessage(TT_SYSTEM, 0xFFFFFFFF, font, color, ToWString(buf));

	va_end(arg);
}
//---------------------------------------------------------------------------
void TOrion::CreateTextMessage(TEXT_TYPE type, DWORD serial, WORD font, WORD color, string text)
{
	TTextData *td = new TTextData();
	td->Unicode = false;
	td->Font = font;
	td->Serial = serial;
	td->Color = color;
	td->Timer = GetTickCount();
	td->Type = type;
	td->SetText(text);
	
	switch (type)
	{
		case TT_SYSTEM:
		{
			td->GenerateTexture(300);
			AddSystemMessage(td);

			break;
		}
		case TT_OBJECT:
		{
			TGameObject *obj = World->FindWorldObject(serial);

			if (obj != NULL)
			{
				int width = FontManager->GetWidthA((BYTE)font, text.c_str(), text.length());

				FontManager->SavePixels = true;

				td->Color = 0;

				if (width > TEXT_MESSAGE_MAX_WIDTH)
				{
					width = FontManager->GetWidthExA((BYTE)font, text.c_str(), text.length(), TEXT_MESSAGE_MAX_WIDTH, TS_LEFT, 0);
					td->GenerateTexture(width, 0, TS_LEFT);
					//td->GenerateTexture(TEXT_MESSAGE_MAX_WIDTH, 0, TS_LEFT);
				}
				else
					td->GenerateTexture(0, 0, TS_CENTER);

				td->Color = color;

				FontManager->SavePixels = false;

				obj->AddText(td);

				DWORD container = obj->Container;

				if (container == 0xFFFFFFFF)
					WorldTextRenderer->Add(td);
				else if (!obj->NPC)
				{
					td->DrawX = g_ClickObject.X;
					td->DrawY = g_ClickObject.Y;

					TGump *gump = GumpManager->GetGump(container, 0, GT_CONTAINER);
					
					if (gump == NULL)
					{
						TGameObject *topobj = obj->GetTopObject();

						if (((TGameItem*)obj)->Layer != OL_NONE)
							gump = GumpManager->GetGump(topobj->Serial, 0, GT_PAPERDOLL);

						if (gump == NULL)
						{
							gump = GumpManager->GetGump(topobj->Serial, 0, GT_TRADE);

							if (gump == NULL)
							{
								topobj = (TGameObject*)topobj->m_Items;

								while (topobj != NULL && topobj->Graphic != 0x1E5E)
									topobj = (TGameObject*)topobj->m_Next;

								if (topobj != NULL)
									gump = GumpManager->GetGump(0, topobj->Serial, GT_TRADE);
							}
						}
					}

					if (gump != NULL)
					{
						TTextRenderer *tr = gump->GetTextRenderer();

						if (tr != NULL)
							tr->Add(td);
					}
				}
			}
			else
			{
				td->GenerateTexture(300);
				AddSystemMessage(td);
			}

			break;
		}
		case TT_CLIENT:
		{
			int width = FontManager->GetWidthA((BYTE)font, text.c_str(), text.length());
			
			FontManager->SavePixels = true;

			if (width > TEXT_MESSAGE_MAX_WIDTH)
			{
				width = FontManager->GetWidthExA((BYTE)font, text.c_str(), text.length(), TEXT_MESSAGE_MAX_WIDTH, TS_LEFT, 0);
				td->GenerateTexture(width, 0, TS_LEFT);
				//td->GenerateTexture(TEXT_MESSAGE_MAX_WIDTH, 0, TS_CENTER);
			}
			else
				td->GenerateTexture(0, 0, TS_CENTER);

			FontManager->SavePixels = false;

			TRenderWorldObject *rwo = (TRenderWorldObject*)serial;
			rwo->AddText(td);
			WorldTextRenderer->Add(td);

			break;
		}
	}
}
//---------------------------------------------------------------------------
void TOrion::CreateUnicodeTextMessage(TEXT_TYPE type, DWORD serial, WORD font, WORD color, wstring text)
{
	TTextData *td = new TTextData();
	td->Unicode = true;
	td->Font = font;
	td->Serial = serial;
	td->Color = color;
	td->Timer = GetTickCount();
	td->Type = type;
	td->SetUnicodeText(text);
	
	switch (type)
	{
		case TT_SYSTEM:
		{
			td->GenerateTexture(300, UOFONT_BLACK_BORDER);
			AddSystemMessage(td);

			break;
		}
		case TT_OBJECT:
		{
			TGameObject *obj = World->FindWorldObject(serial);

			if (obj != NULL)
			{
				int width = FontManager->GetWidthW((BYTE)font, text.c_str(), text.length());

				FontManager->SavePixels = true;

				if (width > TEXT_MESSAGE_MAX_WIDTH)
				{
					width = FontManager->GetWidthExW((BYTE)font, text.c_str(), text.length(), TEXT_MESSAGE_MAX_WIDTH, TS_LEFT, UOFONT_BLACK_BORDER);
					td->GenerateTexture(width, UOFONT_BLACK_BORDER, TS_LEFT);
					//td->GenerateTexture(TEXT_MESSAGE_MAX_WIDTH, UOFONT_BLACK_BORDER, TS_LEFT);
				}
				else
					td->GenerateTexture(0, UOFONT_BLACK_BORDER, TS_CENTER);
				
				FontManager->SavePixels = false;

				obj->AddText(td);

				DWORD container = obj->Container;

				if (container == 0xFFFFFFFF)
					WorldTextRenderer->Add(td);
				else if (!obj->NPC)
				{
					td->DrawX = g_ClickObject.X;
					td->DrawY = g_ClickObject.Y;

					TGump *gump = GumpManager->GetGump(container, 0, GT_CONTAINER);

					if (gump == NULL)
					{
						TGameObject *topobj = obj->GetTopObject();

						if (((TGameItem*)obj)->Layer != OL_NONE)
							gump = GumpManager->GetGump(topobj->Serial, 0, GT_PAPERDOLL);

						if (gump == NULL)
						{
							gump = GumpManager->GetGump(topobj->Serial, 0, GT_TRADE);

							if (gump == NULL)
							{
								topobj = (TGameObject*)topobj->m_Items;

								while (topobj != NULL && topobj->Graphic != 0x1E5E)
									topobj = (TGameObject*)topobj->m_Next;

								if (topobj != NULL)
									gump = GumpManager->GetGump(0, topobj->Serial, GT_TRADE);
							}
						}
					}

					if (gump != NULL)
					{
						TTextRenderer *tr = gump->GetTextRenderer();

						if (tr != NULL)
							tr->Add(td);
					}
				}
			}
			else
			{
				td->GenerateTexture(300, UOFONT_BLACK_BORDER);
				AddSystemMessage(td);
			}

			break;
		}
		default:
		{
			delete td;
			break;
		}
	}
}
//---------------------------------------------------------------------------
void TOrion::AddSystemMessage(TTextData *msg)
{
	SystemChat->Add(msg);
	AddJournalMessage(msg, "");
}
//---------------------------------------------------------------------------
void TOrion::AddJournalMessage(TTextData *msg, string name)
{
	TTextData *jmsg = new TTextData(msg);

	if (!jmsg->Unicode)
	{
		jmsg->SetText(name + jmsg->GetText());
		jmsg->Font = 9;
	}
	else
	{
		if (msg->Type == TT_SYSTEM)
			jmsg->Color = 0;

		jmsg->SetUnicodeText(ToWString(name) + jmsg->GetUnicodeText());
		jmsg->Font = 0;
	}

	if (msg->Type == TT_OBJECT)
		jmsg->GenerateTexture(214, UOFONT_INDENTION | UOFONT_BLACK_BORDER);
	else
		jmsg->GenerateTexture(214, UOFONT_INDENTION);

	GumpManager->UpdateGump(g_PlayerSerial, 0, GT_JOURNAL);

	Journal->Add(jmsg);
}
//---------------------------------------------------------------------------
void TOrion::Connect()
{
	InitScreen(GS_MAIN_CONNECT);

	g_LastRenderTime = 0;
	Process();

	//g_ClientPaused = false;
	//ClearPacketStream();

	ConnectionManager.Disconnect();

	if (ConnectionManager.Init(g_CryptKey[0], g_CryptKey[1], g_CryptKey[2], g_SeedKey) == CE_NO_ERROR) //Configure
	{
		if (ConnectionManager.Connect(g_ShardIP.c_str(), g_ShardPort, g_GameSeed) == CE_NO_ERROR)
		{
			ConnectionScreen->Connected = true;

			TPacketFirstLogin packet;
			packet.Send();
		}
		else
		{
			ConnectionScreen->ConnectionFailed = true;
			ConnectionScreen->ErrorCode = 8;
		}
	}
	else
	{
		ConnectionScreen->ConnectionFailed = true;
		ConnectionScreen->ErrorCode = 8;
	}
}
//---------------------------------------------------------------------------
void TOrion::Disconnect()
{
	if (PluginManager != NULL)
		PluginManager->Disconnect();

	if (SystemChat != NULL)
		SystemChat->Clear();

	if (Journal != NULL)
		Journal->Clear();

	ConnectionManager.Disconnect();

	Party.Leader = 0;
	Party.Inviter = 0;
	Party.Clear();

	g_ConsoleStack[0] = L"";
	g_ConsoleStackCount = 0;
	g_ConsoleSelectedIndex = 0;

	g_ResizedGump = NULL;
}
//---------------------------------------------------------------------------
void TOrion::ServerSelection(int pos)
{
	InitScreen(GS_SERVER_CONNECT);
	Process();

	ServerList.SelectedServer = pos;

	if (PluginManager != NULL)
		PluginManager->WindowProc(g_hWnd, UOMSG_SET_SERVER_NAME, (WPARAM)ServerList.GetServerName().c_str(), 0);

	TPacketSelectServer packet((BYTE)ServerList.GetIndex(pos));
	packet.Send();
}
//---------------------------------------------------------------------------
void TOrion::RelayServer(const char *ip, int port, PBYTE gameSeed)
{
	if (ConnectionManager.Init(gameSeed) == CE_NO_ERROR)
	{
		memcpy(g_GameSeed, gameSeed, 4);

		if (ConnectionManager.Connect(ip, port, g_GameSeed) == CE_NO_ERROR)
		{
			ConnectionScreen->Connected = true;

			if (PluginManager != NULL)
				PluginManager->WindowProc(g_hWnd, UOMSG_SET_ACCOUNT, (WPARAM)MainScreen->m_Account->c_str(), 0);

			TPacketSecondLogin packet;
			packet.Send();
		}
		else
		{
			ConnectionScreen->ConnectionFailed = true;
			ConnectionScreen->ErrorCode = 8;
		}
	}
	else
	{
		ConnectionScreen->ConnectionFailed = true;
		ConnectionScreen->ErrorCode = 8;
	}
}
//---------------------------------------------------------------------------
void TOrion::CharacterSelection(int pos)
{
	InitScreen(GS_GAME_CONNECT);
	ConnectionScreen->Type = CST_GAME;

	if (PluginManager != NULL)
		PluginManager->WindowProc(g_hWnd, UOMSG_SET_PLAYER_NAME, (WPARAM)CharacterList.GetName(pos).c_str(), 0);

	TPacketSelectCharacter packet(pos, CharacterList.GetName(pos));
	packet.Send();
}
//---------------------------------------------------------------------------
void TOrion::LoginComplete()
{
	if (!ConnectionScreen->Completed)
	{
		ConnectionScreen->Completed = true;

		char buf[256] = {0};
		sprintf(buf, "Ultima Online - %s (%s)", g_Player->GetName().c_str(), ServerList.GetServerName().c_str());

		SetWindowTextA(g_hWnd, buf);
	
		//UO->SkillsReq(serial);
		TPacketClientVersion packet(g_ClientVersionText);
		packet.Send();
		packet.Free();
	
		SkillsReq(g_PlayerSerial);
		StatusReq(g_PlayerSerial);
		PaperdollReq(g_PlayerSerial);

		/*TPacketOpenChat openChatPacket(L"");
		openChatPacket.Send();

		TPacketRazorAnswer razorPacket;
		razorPacket.Send();*/

		TPacketLanguage langPacket(g_Language.c_str());
		langPacket.Send();
		langPacket.Free();

		if (PacketManager.GetClientVersion() >= CV_306E)
		{
			TPacketClientType clientTypePacket(g_ClientFlag);
			clientTypePacket.Send();
		}

		if (PacketManager.GetClientVersion() >= CV_305D)
		{
			TPacketClientViewRange vrPacket(g_UpdateRange);
			vrPacket.Send();
		}

		InitScreen(GS_GAME);

		LoadLocalConfig();
	}
}
//---------------------------------------------------------------------------
void TOrion::PickupItem(TGameItem *obj, int count, bool isGameFigure)
{
	if (ObjectInHand == NULL)
	{
		ObjectInHand = new TObjectOnCursor(obj);

		if (!count)
			count = obj->Count;

		ObjectInHand->Separated = count != obj->Count;
		ObjectInHand->IsGameFigure = isGameFigure;
		ObjectInHand->DragCount = count;

		if (obj->Container != 0xFFFFFFFF)
			GumpManager->UpdateGump(obj->Container, 0, GT_CONTAINER);

		TPacketPickupRequest packet(obj->Serial, count);
		packet.Send();
	}
}
//---------------------------------------------------------------------------
void TOrion::DropItem(DWORD container, WORD x, WORD y, char z)
{
	if (ObjectInHand != NULL)
	{
		if (ObjectInHand->Dropped)
		{
			delete ObjectInHand;
			ObjectInHand = NULL;
		}
		else
		{
			ObjectInHand->Dropped = true;

			if (ConnectionManager.ClientVersion >= CV_6017)
			{
				TPacketDropRequestNew packet(ObjectInHand->Serial, x, y, z, 0, container);
				packet.Send();
			}
			else
			{
				TPacketDropRequestOld packet(ObjectInHand->Serial, x, y, z, container);
				packet.Send();
			}
		}
	}
}
//---------------------------------------------------------------------------
void TOrion::EquipItem(DWORD container)
{
	if (ObjectInHand != NULL)
	{
		if (ObjectInHand->Dropped)
		{
			delete ObjectInHand;
			ObjectInHand = NULL;
		}
		else if (ObjectInHand->IsWearable())
		{
			ObjectInHand->Dropped = true;

			WORD graphic = ObjectInHand->Graphic;

			if (graphic < 0x4000)
			{
				STATIC_TILES &st = m_StaticData[graphic / 32].Tiles[graphic % 32];

				if (!container)
					container = g_PlayerSerial;

				TPacketEquipRequest packet(ObjectInHand->Serial, st.Quality, container);
				packet.Send();
			}
		}
	}
}
//---------------------------------------------------------------------------
void TOrion::ChangeWarmode(BYTE status)
{
	BYTE newstatus = (BYTE)(!g_Player->Warmode);
	if (status != 0xFF)
	{
		if (g_Player->Warmode == (bool)status)
			return;

		newstatus = status;
	}

	if (newstatus == 1 && ConfigManager.Music)
		//38, 39 и 40 это индексы боевой музыки.
		Orion->PlayMusic(rand() % (40 - 38 + 1) + 38, true);
	else if (newstatus == 0)
		SoundManager.StopWarMusic();
	TPacketChangeWarmode packet(newstatus);
	packet.Send();
}
//---------------------------------------------------------------------------
void TOrion::HelpRequest()
{
	TPacketHelpRequest packet;
	packet.Send();
}
//---------------------------------------------------------------------------
void TOrion::StatusReq(DWORD serial)
{
	TPacketStatusRequest packet(serial);
	packet.Send();
}
//---------------------------------------------------------------------------
void TOrion::SkillsReq(DWORD serial)
{
	TPacketSkillsRequest packet(serial);
	packet.Send();
}
//---------------------------------------------------------------------------
void TOrion::SkillStatusChange(BYTE skill, BYTE state)
{
	TPacketSkillsStatusChangeRequest packet(skill, state);
	packet.Send();
}
//---------------------------------------------------------------------------
void TOrion::Click(DWORD serial)
{
	TPacketClickRequest packet(serial);
	packet.Send();

	TGameObject *obj = World->FindWorldObject(serial);
	if (obj != NULL)
		obj->Clicked = true;
}
//---------------------------------------------------------------------------
void TOrion::DoubleClick(DWORD serial)
{
	if (serial >= 0x40000000)
		g_LastUseObject = serial;

	TPacketDoubleClickRequest packet(serial);
	packet.Send();
}
//---------------------------------------------------------------------------
void TOrion::PaperdollReq(DWORD serial)
{
	//g_LastUseObject = serial;

	TPacketDoubleClickRequest packet(serial | 0x80000000);
	packet.Send();
}
//---------------------------------------------------------------------------
void TOrion::Attack(DWORD serial)
{
	if (ConfigManager.CriminalActionsQuery && World != NULL)
	{
		TGameCharacter *target = World->FindWorldCharacter(serial);

		if (target != NULL && (NOTORIETY_TYPE)g_Player->Notoriety == NT_INNOCENT && (NOTORIETY_TYPE)target->Notoriety == NT_INNOCENT)
		{
			int x = g_GameWindowPosX + (g_GameWindowWidth / 2) - 40;
			int y = g_GameWindowPosY + (g_GameWindowHeight / 2) - 20;

			TGumpQuestion *newgump = new TGumpQuestion(g_PlayerSerial, x, y, 2);
			newgump->SetID(serial);
			
			GumpManager->AddGump(newgump);

			Orion->InitScreen(GS_GAME_BLOCKED);
			GameBlockedScreen->Code = 3;

			return;
		}
	}

	AttackReq(serial);
}
//---------------------------------------------------------------------------
void TOrion::AttackReq(DWORD serial)
{
	g_LastAttackObject = serial;
	
	TPacketStatusRequest statusReq(serial);
	statusReq.Send();

	TPacketAttackRequest packet(serial);
	packet.Send();
}
//---------------------------------------------------------------------------
void TOrion::SendASCIIText(const char *str, SPEECH_TYPE type)
{
	TPacketASCIISpeechRequest packet(str, type, 3, ConfigManager.SpeechColor);
	packet.Send();
	packet.Free();
}
//---------------------------------------------------------------------------
void TOrion::CastSpell(int index)
{
	if (index >= 0)
	{
		g_LastSpellIndex = index;

		TPacketCastSpell packet(index);
		packet.Send();
	}
}
//---------------------------------------------------------------------------
void TOrion::CastSpellFromBook(int index, DWORD serial)
{
	if (index >= 0)
	{
		g_LastSpellIndex = index;

		TPacketCastSpellFromBook packet(index, serial);
		packet.Send();
		packet.Free();
	}
}
//---------------------------------------------------------------------------
void TOrion::UseSkill(int index)
{
	if (index >= 0)
	{
		g_LastSkillIndex = index;

		TPacketUseSkill packet(index);
		packet.Send();
	}
}
//---------------------------------------------------------------------------
void TOrion::OpenDoor()
{
	TPacketOpenDoor packet;
	packet.Send();
}
//---------------------------------------------------------------------------
void TOrion::EmoteAction(const char *action)
{
	TPacketEmoteAction packet(action);
	packet.Send();
	packet.Free();
}
//---------------------------------------------------------------------------
void TOrion::AllNames()
{
	TGameObject *obj = World->m_Items;

	while (obj != NULL)
	{
		if ((obj->NPC && !obj->IsPlayer()) || obj->IsCorpse())
		{
			TPacketClickRequest packet(obj->Serial);
			packet.Send();
		}

		obj = (TGameObject*)obj->m_Next;
	}
}
//---------------------------------------------------------------------------
void TOrion::GoToWebLink(string url)
{
	if (url.length())
		ShellExecuteA(0, "Open", url.c_str(), NULL, NULL, SW_SHOWNORMAL);
}
//---------------------------------------------------------------------------
void TOrion::RemoveRangedObjects()
{
	int objectsRange = g_UpdateRange + 1;

	if (World != NULL)
	{
		TGameObject *go = World->m_Items;
	
		while (go != NULL)
		{
			TGameObject *next = (TGameObject*)go->m_Next;

			if (go->Container == 0xFFFFFFFF && !go->IsPlayer())
			{
				if (go->Graphic >= 0x4000)
				{
					if (GetMultiDistance(g_RemoveRangeXY, go) > objectsRange)
					//if (GetDistance(g_RemoveRangeXY, go) > 31)
						World->RemoveObject(go);
				}
				else if (GetDistance(g_RemoveRangeXY, go) > objectsRange)
				{
					if (Party.Contains(go->Serial))
						go->RemoveRender();
					else
						World->RemoveObject(go);
				}
			}

			go = next;
		}
	}

	if (GumpManager != NULL)
		GumpManager->RemoveRangedGumps();

	if (EffectManager != NULL)
		EffectManager->RemoveRangedEffects();
}
//---------------------------------------------------------------------------
void TOrion::LogOut()
{
	TPRINT("TUltimaOnline::LogOut->Start\n");
	SaveLocalConfig();

	if (g_SendLogoutNotification)
	{
		TPacketLogoutNotification loPacket;
		loPacket.Send();
	}

	Disconnect();
	TPRINT("\tDisconnected?\n");
	
	if (Walker != NULL)
	{
		delete Walker;
		Walker = NULL;
	}
	TPRINT("\tWalker deleted?\n");

	if (ObjectInHand != NULL)
	{
		delete ObjectInHand;
		ObjectInHand = NULL;
	}
	TPRINT("\tObjectInHand removed?\n");

	if (World != NULL)
	{
		delete World;
		World = NULL;
	}
	TPRINT("\tWorld removed?\n");
	
	g_PopupMenu = NULL;

	if (GumpManager != NULL)
		GumpManager->Clear();
	TPRINT("\tGump Manager cleared?\n");
	
	if (EffectManager != NULL)
		EffectManager->Clear();
	TPRINT("\tEffect List cleared?\n");
	
	//ClearPacketStream();
	//TPRINT("\tPacket Stream removed?\n");

	if (GameConsole != NULL)
		GameConsole->Clear();
	TPRINT("\tGame console cleared?\n");

	EntryPointer = NULL;
	g_GrayMenuCount = 0;

	Target.Reset();
	TPRINT("\tTarget reseted?\n");

	if (SystemChat != NULL)
		SystemChat->Clear();
	TPRINT("\tSystem chat cleared?\n");

	if (Journal != NULL)
		Journal->Clear();
	TPRINT("\tJournal cleared?\n");

	if (MapManager != NULL)
		MapManager->Clear();

	g_CurrentMap = 0;

	InitScreen(GS_MAIN);
	TPRINT("TUltimaOnline::LogOut->End\n");
}
//---------------------------------------------------------------------------
void TOrion::OpenPaperdoll()
{
	PaperdollReq(g_PlayerSerial);
}
//---------------------------------------------------------------------------
void TOrion::OpenStatus(DWORD serial)
{
	int x = g_MouseX - 76;
	int y = g_MouseY - 30;

	StatusReq(serial);

	TGumpStatusbar *gump = new TGumpStatusbar(serial, x, y, true);

	GumpManager->AddGump(gump);
}
//---------------------------------------------------------------------------
void TOrion::OpenMinimap()
{
	TGumpMinimap *gump = new TGumpMinimap(g_PlayerSerial, 0, 0, true);
	GumpManager->AddGump(gump);
}
//---------------------------------------------------------------------------
void TOrion::OpenWorldMap()
{
	int x = g_GameWindowPosX + (g_GameWindowWidth / 2) - 200;
	int y = g_GameWindowPosY + (g_GameWindowHeight / 2) - 150;
	TGumpWorldMap *gump = new TGumpWorldMap(g_PlayerSerial, x, y);
	gump->Called = true;
	GumpManager->AddGump(gump);
}
//---------------------------------------------------------------------------
void TOrion::OpenJournal()
{
	TGumpJournal *gump = new TGumpJournal(g_PlayerSerial, 0, 0, false);

	GumpManager->AddGump(gump);
}
//---------------------------------------------------------------------------
void TOrion::OpenSkills()
{
	TGumpSkills *gump = new TGumpSkills(g_PlayerSerial, 0, 0, false);

	GumpManager->AddGump(gump);
}
//---------------------------------------------------------------------------
void TOrion::OpenBackpack()
{
	if (g_Player != NULL)
	{
		TGameItem *pack = g_Player->FindLayer(OL_BACKPACK);
		if (pack != NULL)
			DoubleClick(pack->Serial);
	}
}
//---------------------------------------------------------------------------
void TOrion::OpenLogOut()
{
	int x = g_GameWindowPosX + (g_GameWindowWidth / 2) - 40;
	int y = g_GameWindowPosY + (g_GameWindowHeight / 2) - 20;
	TGumpQuestion *gump = new TGumpQuestion(g_PlayerSerial, x, y, 1);
	
	GumpManager->AddGump(gump);

	InitScreen(GS_GAME_BLOCKED);
	GameBlockedScreen->SetCode(3);
}
//---------------------------------------------------------------------------
void TOrion::OpenChat()
{
}
//---------------------------------------------------------------------------
void TOrion::OpenConfiguration()
{
	int x = (GetSystemMetrics(SM_CXSCREEN) / 2) - 383;
	int y = (GetSystemMetrics(SM_CYSCREEN) / 2) - 260;

	TGumpOptions *gump = new TGumpOptions(g_PlayerSerial, x, y);

	g_OptionsConfig = ConfigManager;

	GumpManager->AddGump(gump);
}
//---------------------------------------------------------------------------
void TOrion::OpenMail()
{
}
//---------------------------------------------------------------------------
void TOrion::OpenPartyManifest()
{
	int x = (g_ClientWidth / 2) - 272;
	int y = (g_ClientHeight / 2) - 240;

	TGumpPartyManifest *gump = new TGumpPartyManifest(0, x, y, Party.CanLoot);

	GumpManager->AddGump(gump);
}
//---------------------------------------------------------------------------
void TOrion::OpenProfile(DWORD serial)
{
	if (!serial)
		serial = g_PlayerSerial;

	TPacketProfileRequest packet(serial);
	packet.Send();
}
//---------------------------------------------------------------------------
void TOrion::RequestGuildGump()
{
	TPacketGuildMenuRequest packet;
	packet.Send();
}
//---------------------------------------------------------------------------
void TOrion::RequestQuestGump()
{
	TPacketQuestMenuRequest packet;
	packet.Send();
}
//---------------------------------------------------------------------------
void TOrion::ChangeMap(BYTE newmap)
{
	if (newmap < 0 || newmap > 5)
		newmap = 0;

	if (g_CurrentMap != newmap)
	{
		g_CurrentMap = newmap;

		if (World != NULL && g_Player != NULL)
		{
			g_Player->MapIndex = g_CurrentMap;
			g_Player->RemoveRender();

			TGameObject *obj = (TGameObject*)World->m_Items;

			while (obj != NULL)
			{
				TGameObject *next = (TGameObject*)obj->m_Next;

				if (obj->MapIndex != g_CurrentMap)
					World->RemoveObject(obj);

				obj = next;
			}

			MapManager->Clear();
			MapManager->Init();
	
			MapManager->AddRender(g_Player);
		}
	}
}
//---------------------------------------------------------------------------
void TOrion::ConsolePromptSend()
{
	int len = GameConsole->Length();
	bool cancel = (len < 1);

	switch (g_ConsolePrompt)
	{
		case PT_ASCII:
		{
			TPacketASCIIPromptResponse packet(GameConsole->c_str(), len, cancel);
			packet.Send();
			packet.Free();

			break;
		}
		case PT_UNICODE:
		{
			TPacketUnicodePromptResponse packet(GameConsole->Data(), len, "ENU", cancel);
			packet.Send();
			packet.Free();

			break;
		}
		default:
			break;
	}

	g_ConsolePrompt = PT_NONE;
}
//---------------------------------------------------------------------------
void TOrion::ConsolePromptCancel()
{
	switch (g_ConsolePrompt)
	{
		case PT_ASCII:
		{
			TPacketASCIIPromptResponse packet("", 0, true);
			packet.Send();
			packet.Free();

			break;
		}
		case PT_UNICODE:
		{
			TPacketUnicodePromptResponse packet(L"", 0, "ENU", true);
			packet.Send();
			packet.Free();

			break;
		}
		default:
			break;
	}

	g_ConsolePrompt = PT_NONE;
}
//---------------------------------------------------------------------------