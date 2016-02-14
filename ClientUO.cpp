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

TUltimaOnline *UO = NULL;
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
			if (UO == NULL || IsIconic(g_hWnd))
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
					r.right = g_GameWindowSizeX;
					r.bottom = g_GameWindowSizeY;
					AdjustWindowRectEx(&r, GetWindowLongA(g_hWnd, GWL_STYLE), FALSE, GetWindowLongA(g_hWnd, GWL_EXSTYLE));

					if (r.left < 0)
						r.right += (r.left * (-1));

					if (r.top < 0)
						r.bottom += (r.top * (-1));
						
					POINT min = {g_GameWindowSizeX, g_GameWindowSizeY};
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
			if (UO != NULL)
			{
				if (PluginManager != NULL)
					PluginManager->WindowProc(hWnd, message, wParam, lParam);

				UO->SaveLocalConfig();
				__try
				{
					delete UO;
				}
				__finally
				{
					UO = NULL;
				}
			}
			
			PostQuitMessage(0);

			return 0;
		}
		case WM_LBUTTONDOWN:
		{
			if (g_SmoothMonitorMode)
				break;

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

			break;
		}
		case WM_LBUTTONUP:
		{
			if (g_SmoothMonitorMode)
				break;

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
			if (g_SmoothMonitorMode)
				break;

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
				!(g_MouseX < g_GameWindowPosX || g_MouseY < g_GameWindowPosY || g_MouseX > (g_GameWindowPosX + g_GameWindowSizeX) ||
				g_MouseY > (g_GameWindowPosY + g_GameWindowSizeY)))
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

			break;
		}
		case WM_RBUTTONUP:
		{
			if (g_SmoothMonitorMode)
				break;

			g_SelectGumpObjects = true;

			CurrentScreen->OnRightMouseUp();
			
			g_RightMouseDown = false;
			g_DroppedRightMouseX = 0;
			g_DroppedRightMouseY = 0;
			g_LastGumpRightMouseDown = 0;
			g_LastSelectedGump = 0;
			g_MovingFromMouse = false;

			g_SelectGumpObjects = false;

			break;
		}
		case WM_CHAR:
		{
			if (g_SmoothMonitorMode)
				break;

			if (PluginManager != NULL && !PluginManager->WindowProc(hWnd, message, wParam, lParam))
				return 0;

			g_SelectGumpObjects = true;

			CurrentScreen->OnCharPress(wParam, lParam);

			g_SelectGumpObjects = false;
			
			return 0; //break;
		}
		//Нажатие на колесико мышки
		case 0x0208:
		{
			if (g_SmoothMonitorMode)
				break;

			if (PluginManager != NULL && !PluginManager->WindowProc(hWnd, message, wParam, lParam))
				return 0;

			g_SelectGumpObjects = true;

			CurrentScreen->OnMouseWheel(MWS_PRESS);

			g_SelectGumpObjects = false;

			break;
		}
		//Колесико мышки вверх/вниз
		case 0x020A:
		{
			if (g_SmoothMonitorMode)
				break;

			if (PluginManager != NULL && !PluginManager->WindowProc(hWnd, message, wParam, lParam))
				return 0;

			g_SelectGumpObjects = true;

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
			if (g_SmoothMonitorMode)
				break;

			if (PluginManager != NULL && !PluginManager->WindowProc(hWnd, message, wParam, lParam))
				return 0;

			//trace_printf("CurrentScreen->OnKeyPress(0x%08X, 0x%08X)\n", wParam, lParam);
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
			if (g_SmoothMonitorMode)
				break;

			if (PluginManager != NULL && !PluginManager->WindowProc(hWnd, message, wParam, lParam))
				return 0;

			if (wParam == VK_TAB && UO != NULL && g_GameState == GS_GAME)
			{
				if (ConfigManager.HoldTabForCombat)
					UO->ChangeWarmode(0);
			}

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
		case WM_NCPAINT:
		{
			if (PluginManager != NULL && !PluginManager->WindowProc(hWnd, message, wParam, lParam))
				return 0;

			break;
		}
		default:
			break;
	}

	return DefWindowProc(hWnd, message, wParam, lParam);
}
//---------------------------------------------------------------------------
ATOM MyRegisterClass()
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WindowProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= g_Hinstance;
	wcex.hIcon			= LoadIcon(g_Hinstance, MAKEINTRESOURCE(IDI_CLIENTUO));
	//wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hCursor		= LoadCursor(g_Hinstance, MAKEINTRESOURCE(IDC_CURSOR1));
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName	= NULL;
	wcex.lpszClassName	= L"Ultima Online";
	wcex.hIconSm		= LoadIcon(g_Hinstance, MAKEINTRESOURCE(IDI_CLIENTUO));

	return RegisterClassEx(&wcex);
}
//---------------------------------------------------------------------------
BOOL InitInstance(int nCmdShow)
{
	int WinWidth = GetSystemMetrics(SM_CXSCREEN);

	if (WinWidth > 640)
		WinWidth = 640;

	WinWidth += (2 * GetSystemMetrics(SM_CXSIZEFRAME));

	int WinHeight = GetSystemMetrics(SM_CYSCREEN);

	if (WinHeight > 480)
		WinHeight = 480;

	WinHeight += (GetSystemMetrics(SM_CYCAPTION) + (GetSystemMetrics(SM_CYFRAME) * 2));

	g_hWnd = CreateWindowEx(WS_EX_WINDOWEDGE, L"Ultima Online", L"Ultima Online", WS_OVERLAPPEDWINDOW,
		0, 0, WinWidth, WinHeight, NULL, NULL, g_Hinstance, NULL);

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

	UO = new TUltimaOnline();

	if (UO == NULL)
		return FALSE;

	if (!UO->Install())
	{
		delete UO;
		return FALSE;
	}
	
	ShowWindow(g_hWnd, nCmdShow);
	UpdateWindow(g_hWnd);
	
	UO->LoadPluginConfig();

	return TRUE;
}
//---------------------------------------------------------------------------
int APIENTRY _tWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPTSTR  lpCmdLine, _In_ int nCmdShow)
{
	g_Hinstance = hInstance;

	MyRegisterClass();

	if (!InitInstance(nCmdShow))
		return FALSE;

	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_CLIENTUO));

	MSG msg = {0};

	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateAccelerator(msg.hwnd, hAccelTable, &msg);
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		
		Sleep(1);
		
		if (UO != NULL)
		{
			MouseManager.UpdateMouse();

			UO->Process();
		}
	}
	
	return (int)msg.wParam;
}
//---------------------------------------------------------------------------
TUltimaOnline::TUltimaOnline()
: m_StaticData(NULL), m_StaticDataCount(0), m_StaticAnimList(NULL),
m_UsedLandList(NULL), m_UsedStaticList(NULL), m_UsedGumpList(NULL),
m_UsedTextureList(NULL), m_UsedSoundList(NULL), m_AnimData(NULL),
m_UsedLightList(NULL)
{
	g_Logger = new TLogger();
	string pth = g_DirectoryPath + "\\uolog.txt";
	g_Logger->Init(pth.c_str());
	TPRINT("Log open.\n");
}
//---------------------------------------------------------------------------
TUltimaOnline::~TUltimaOnline()
{
	if (MainScreen != NULL)
		MainScreen->SaveGlobalConfig();

	if (GumpManager != NULL)
		GumpManager->OnDelete();
	
	Disconnect();

	UnloadIndexFiles();
	
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

	if (m_StaticData != NULL)
	{
		delete m_StaticData;
		m_StaticData = NULL;
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

	TGumpMenubar::ReleaseTextTextures();
	TGumpOptions::ReleaseTextTextures();
	TGumpSpellbook::ReleaseTextTextures();
	TGumpPartyManifest::ReleaseTextTextures();
	TGumpWorldMap::ReleaseTextTextures();
	
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
	m_StaticDataCount = 0;

	FileManager.Unload();

	g_LightBuffer.Free();

	g_GL.Uninstall();
	
	if (g_Logger != NULL)
	{
		delete g_Logger;
		g_Logger = NULL;
	}
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
bool TUltimaOnline::Install()
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
		trace_printf("Error install OpenGL\n");
		MessageBoxA(g_hWnd, "Error install OpenGL", "Error install OpenGL!", MB_OK);
		return false;
	}
	
	g_GL.UpdateRect();
	
	LoadClientConfig();
	LoadAutoLoginNames();
	
	if (!FileManager.Load())
	{
		string tmp = string("Error loading file: ") + g_LastLoadFileError;
		trace_printf(tmp.c_str());
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
		trace_printf("Error loading fonts\n");
		MessageBoxA(g_hWnd, "Error loading fonts", "Error loading fonts!", MB_OK);

		return false;
	}

	if (!LoadSkills())
	{
		trace_printf("Error loading skills\n");
		MessageBoxA(g_hWnd, "Error loading skills", "Error loading skills!", MB_OK);

		return false;
	}
	
	PatchFiles();

	g_SkillSort.Init();
	
	if (!MouseManager.LoadCursorTextures())
	{
		trace_printf("Error loading cursors\n");
		MessageBoxA(g_hWnd, "Error loading cursors", "Error loading cursors!", MB_OK);

		return false;
	}

	if (!SoundManager.Init())
	{
		trace_printf("Error install SDL audio: %s\n", SDL_GetError());
		MessageBoxA(g_hWnd, "Error install SDL audio", "Error install SDL audio!", MB_OK);

		return false;
	}

	AnimationManager = new TAnimationManager();
	IFOR(i, 0, 6)
		AnimationManager->Init(i, (DWORD)FileManager.AnimIdx[i].Address, (DWORD)FileManager.AnimMul[i].Address, (DWORD)FileManager.AnimIdx[i].Size);

	PDWORD verda = (FileManager.UseVerdata ? (PDWORD)FileManager.VerdataMul.Address : NULL);
	AnimationManager->InitBodyconv(verda, FilePath("Bodyconv.def"));

	ClilocManager = new TClilocManager();

	LoadProfession();
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

	TGumpMenubar::InitTextTextures();
	TGumpOptions::InitTextTextures();
	TGumpSpellbook::InitTextTextures();
	TGumpPartyManifest::InitTextTextures();
	TGumpWorldMap::InitTextTextures();

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

	g_CircleOfTransparency.Create(200);

	MainScreen->LoadGlobalConfig();

	LoadShaders();

#ifdef _DEBUG
	CurrentScreen = DebugScreen;
	DebugScreen->Init();
#else
	InitScreen(GS_MAIN);
#endif

	return true;
}
//---------------------------------------------------------------------------
void TUltimaOnline::InitScreen(GAME_STATE state)
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
void TUltimaOnline::LoadTiledata()
{
	if (!FileManager.TiledataMul.Address)
		return;

	memcpy(&m_LandData[0], (PVOID)FileManager.TiledataMul.Address, 512 * sizeof(LAND_GROUP));

	m_StaticDataCount= (FileManager.TiledataMul.Size - 428032) / sizeof(STATIC_GROUP);

	m_StaticData = new STATIC_GROUP[m_StaticDataCount];

	memcpy(&m_StaticData[0], (PVOID)((DWORD)FileManager.TiledataMul.Address + 428032), m_StaticDataCount * sizeof(STATIC_GROUP));
}
//---------------------------------------------------------------------------
WORD TUltimaOnline::CalculateLightColor(WORD id)
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
void TUltimaOnline::LoadIndexFiles()
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

	IFOR(i, 0, 0x10000)
	{ 
		if (i < 0x4000)
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

			if (i < 0x1000)
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

				if (i < 0x0800)
				{
					TIndexSound &sound = m_SoundDataIndex[i];

					sound.Address = SoundPtr->Position;
					sound.Size = SoundPtr->Size;
					sound.LastAccessTime = 0;
					sound.Timer = 0;
					//sound.Sound = NULL;
					sound.soundStream = 0;
					
					if (sound.Address == 0xFFFFFFFF || !sound.Size || sound.Size == 0xFFFFFFFF)
					{
						sound.Address = 0;
						sound.Size = 0;
					}
					else
						sound.Address = sound.Address + (DWORD)FileManager.SoundMul.Address;

					SoundPtr++;

					if (i < 100)
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
void TUltimaOnline::InitStaticAnimList()
{
	TLinkedList *firstItem = NULL;

	if (m_AnimData != NULL)
	{
		IFOR(i, 0, 0x4000)
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
void TUltimaOnline::ProcessStaticAnimList()
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
				
				if (offset < pad->FrameCount - 1)
				{
					obj->Increment = pad->FrameData[offset];
					offset++;
				}
				else
				{
					obj->Increment = pad->FrameData[offset];
					offset = 0;
				}

				obj->AnimIndex = offset;
			}
			
			list = list->Next;
		}
	}
}
//---------------------------------------------------------------------------
void TUltimaOnline::UnloadIndexFiles()
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
			else if (((TIndexSound*)(list->Data))->soundStream != 0)
			{
				//Mix_FreeChunk(((TIndexSound*)(list->Data))->Sound);
				BASS_StreamFree(((TIndexSound*)(list->Data))->soundStream);
				((TIndexSound*)(list->Data))->soundStream = 0;
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
void TUltimaOnline::ClearUnusedTextures()
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

	ResetSoundEffects(ticks);
}
//---------------------------------------------------------------------------
void TUltimaOnline::PatchFiles()
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
			if (vh->BlockID >= 0x4000) //Run
			{
				WORD ID = (WORD)vh->BlockID - 0x4000;
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

				if (bID < 0 || bID >= m_StaticDataCount)
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
			trace_printf("Unused verdata block (fileID) = %i (BlockID+ %i\n", vh->FileID, vh->BlockID);
		}
	}

	ColorManager->CreateHuesPalette();
}
//---------------------------------------------------------------------------
bool TUltimaOnline::LoadSkills()
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

			//trace_printf("Skill load[%d] = %s\n", i, (char*)((DWORD)FileManager.SkillsMul.Address + sidx->Position + 1));
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
void TUltimaOnline::UnloadSkills()
{
	if (g_Skills != NULL)
	{
		delete []g_Skills;
		g_Skills = NULL;
	}
}
//--------------------------------------------------------------------------
void TUltimaOnline::LoadProfession()
{
	Profession = new TProfessionCategory();

	FILE *file = fopen(FilePath("prof.txt").c_str(), "rt");

	if (file != NULL)
	{
		while (!feof(file))
		{
			char buf[512] = {0};
			fgets(buf, 512, file);

			if (!strlen(buf))
				continue;

			buf[strlen(buf) - 1] = 0;
			char *text = _strlwr(buf);

			while (*text && (*text == ' ' || *text == '\t'))
				text++;

			if (!memcmp(text, "begin", 5))
			{
				TProfessionCategory *pc = new TProfessionCategory();
				TProfession *p = new TProfession();
				bool type = false; //false - Category; true - Profession
				bool topobj = false;
				int skill_index = 0;

				while (fgets(buf, 512, file))
				{
					if (!strlen(buf))
						continue;

					buf[strlen(buf) - 1] = 0;
					char data[512] = {0};
					strncpy(data, buf, strlen(buf));
					char *text = _strlwr(buf);

					while (*text && (*text == ' ' || *text == '\t'))
						text++;

					if (!memcmp(text, "name", 4))
					{
						text += 4;

						while (*text && (*text == ' ' || *text == '\t'))
							text++;

						if (type)
							p->SetDefName(text);
						else
							pc->SetDefName(text);
					}
					else if (!memcmp(text, "truename", 8))
					{
						text = data;

						while (*text && (*text == ' ' || *text == '\t'))
							text++;

						text += 8;

						while (*text && (*text == ' ' || *text == '\t'))
							text++;

						if (strlen(text) > 2)
						{
							text++;
							text[strlen(text) - 1] = 0;

							if (type)
								p->SetName(text);
							else
								pc->SetName(text);
						}
					}
					else if (!memcmp(text, "desc", 4))
					{
						//Ignore Desc section?

						text += 4;
						while (*text && (*text == ' ' || *text == '\t'))
							text++;

						if (type)
							p->Index = atoi(text);
						else
							pc->Index = atoi(text);
					}
					else if (!memcmp(text, "toplevel", 8))
					{
						text += 8;

						while (*text && (*text == ' ' || *text == '\t'))
							text++;

						if (!memcmp(text, "true", 4))
							topobj = true;
					}
					else if (!memcmp(text, "gump", 4))
					{
						text += 4;

						while (*text && (*text == ' ' || *text == '\t'))
							text++;

						WORD GumpID = atoi(text);
						ExecuteGump(GumpID, 0);
						ExecuteGump(GumpID + 1, 0);

						if (type)
							p->SetGump(GumpID);
						else
							pc->SetGump(GumpID);
					}
					else if (!memcmp(text, "type", 4))
					{
						text += 4;

						while (*text && (*text == ' ' || *text == '\t'))
							text++;

						if (!memcmp(text, "profession", 10))
						{
							if (!type)
							{
								p->SetDefName(pc->GetDefName());
								p->SetName(pc->GetName());
								p->SetGump(pc->GetGump());
								p->Index = pc->Index;
							}

							type = true;
							p->SetType(true);
						}
						else
						{
							type = false;
							p->SetType(false);
						}
					}
					else if (!memcmp(text, "children", 8))
					{
						text += 8;

						while (*text && (*text == ' ' || *text == '\t'))
							text++;

						if (!type)
						{
							char child[30] = {0};
							int child_len = 0;

							while (strlen(text))
							{
								if (*text == ',')
								{
									pc->AddChildren(child);
									memset(child, 0, 30);
									child_len = 0;
									text++;
								}
								else
								{
									child[child_len] = *text;
									child_len++;
									text++;
								}
							}

							if (child_len)
								pc->AddChildren(child);
						}
					}
					else if (!memcmp(text, "skill", 5))
					{
						if (!type)
						{
							type = true;
							p->SetDefName(pc->GetDefName());
							p->SetName(pc->GetName());
							p->SetGump(pc->GetGump());
							p->SetType(true);
							p->Index = pc->Index;
						}

						text = data;
						text += 6;

						while (*text && (*text == ' ' || *text == '\t'))
							text++;

						int skill_name_len = strlen(text);

						IFOR(j, 0, 54)
						{
							int sn_len = g_SkillName[j].length();

							if (skill_name_len < sn_len)
								sn_len = skill_name_len;

							if (!memcmp(text, g_SkillName[j].c_str(), sn_len))
							{
								p->SetSkillID(skill_index, j);
								text += (sn_len + 1);

								while (*text && (*text == ' ' || *text == '\t'))
									text++;

								p->SetSkillValue(skill_index, atoi(text));
								skill_index++;

								break;
							}
						}
					}
					else if (!memcmp(text, "stat", 4))
					{
						if (!type)
						{
							type = true;
							p->SetDefName(pc->GetDefName());
							p->SetName(pc->GetName());
							p->SetGump(pc->GetGump());
							p->SetType(true);
							p->Index = pc->Index;
						}

						text += 4;

						while (*text && (*text == ' ' || *text == '\t'))
							text++;

						if (!memcmp(text, "str", 3))
						{
							text += 3;

							while (*text && (*text == ' ' || *text == '\t'))
								text++;

							p->SetStr(atoi(text));
						}
						else if (!memcmp(text, "int", 3))
						{
							text += 3;

							while (*text && (*text == ' ' || *text == '\t'))
								text++;

							p->SetInt(atoi(text));
						}
						else if (!memcmp(text, "dex", 3))
						{
							text += 3;

							while (*text && (*text == ' ' || *text == '\t'))
								text++;

							p->SetDex(atoi(text));
						}
					}
					else if (!strstr(text, "end"))
						break;
				}

				if (type)
				{
					if (!Profession->Add(p))
						delete p;
					delete pc;
				}
				else
				{
					pc->m_Parent = Profession;
					if (!Profession->AddCategory(pc, topobj))
						delete pc;
					delete p;
				}
			}
		}

		fclose(file);

		ExecuteGump(0x15A9, 0);
		ExecuteGump(0x15AA, 0);

		TProfession *apc = new TProfession();
		apc->SetDefName("advanced");
		apc->SetName("Advanced");
		apc->Type = false;
		apc->Gump = 0x15A9;
		apc->m_Parent = Profession;
		apc->Index = -1;
		apc->Str = 44;
		apc->Int = 10;
		apc->Dex = 11;
		apc->SetSkillID(0, 0xFF);
		apc->SetSkillID(1, 0xFF);
		apc->SetSkillID(2, 0xFF);
		apc->SetSkillValue(0, 50);
		apc->SetSkillValue(1, 50);
		apc->SetSkillValue(2, 0);

		Profession->AddChildren("advanced");

		if (!Profession->Add(apc))
			{TPRINT("Failed to add Advanced\n");
		}

		Profession->SetDefName("parent");
		Profession->SetName("Parent");
		Profession->Index = -2;
		Profession->SetType(false);
		Profession->SetGump(0x15A9);

		LoadProfessionDescription();
	}
	else
	{
		TPRINT("Failed to load prof.txt\n");
		MessageBoxA(g_hWnd, "Failed to load prof.txt", "Failed to load", MB_OK);
	}
}
//--------------------------------------------------------------------------
void TUltimaOnline::LoadProfessionDescription()
{
	TMappedHeader file;
	memset(&file, 0, sizeof(file));

	if (FileManager.LoadFileToMemory(file, FilePath("professn.enu").c_str()))
	{
		char *ptr = (char*)file.Address;
		char *end = (char*)((DWORD)file.Address + file.Size);

		vector<string> list;

		while (ptr < end)
		{
			if (!memcmp(ptr, "TEXT", 4))
			{
				ptr += 8;

				while (ptr < end)
				{
					if (!(*(ptr - 1)) && !(*ptr)) //end of names section
					{
						ptr++;

						break;
					}

					list.push_back(ptr);
					ptr += strlen(ptr) + 1;
				}

				Profession->AddDescription(-2, "parent", ptr);
				ptr += strlen(ptr) + 1;
				//trace_printf("list.size() = %d\n", list.size());

				IFOR(i, 0, (int)list.size())
				{
					//trace_printf("list[%d] = %s\nptr = %s\n", i, list[i].c_str(), ptr);

					if (!Profession->AddDescription(i - 1, list[i], ptr))
						trace_printf("Failed to add description! (%s)\n", list[i].c_str());

					ptr += strlen(ptr) + 1;
				}

				break;
			}

			ptr++;
		}

		list.clear();

		FileManager.UnloadFileFromMemory(file);
	}
	else
	{
		MessageBoxA(g_hWnd, "Failed to load professn.enu", "Failed to load", MB_OK);
		TPRINT("Failed to load professn.enu\n");
	}
}
//--------------------------------------------------------------------------
void TUltimaOnline::LoadLogin()
{
	FILE *file = fopen(FilePath("login.cfg").c_str(), "rt");
	if (file != NULL)
	{
		while (!feof(file))
		{
			char buf[512] = {0};
			fgets(buf, 512, file);

			if (!strlen(buf))
				continue;

			if (!feof(file))
				buf[strlen(buf) - 1] = 0;

			char *text = _strlwr(buf);

			while (*text && (*text == ' ' || *text == '\t'))
				text++;

			if (!memcmp(text, "loginserver", 5))
			{
				text += 11;

				while (*text && (*text == ' ' || *text == '\t' || *text == '='))
					text++;

				char *port = strchr(text, ',');

				if (port >= 0)
				{
					*port = 0;

					while (strlen(text) && (text[strlen(text) - 1] == ' ' || text[strlen(text) - 1] == '\t'))
						text[strlen(text) - 1] = 0;

					g_ShardIP = text;
					g_ShardPort = atoi(port + 1);

					break;
				}
			}
		}

		fclose(file);
	}
}
//---------------------------------------------------------------------------
void TUltimaOnline::LoadAutoLoginNames()
{
	g_AutoLoginNames = "";

	FILE *file = fopen(FilePath("AutoLoginNames.cfg").c_str(), "r");

	if (file != NULL)
	{
		while (!feof(file))
		{
			char name[128] = {0};

			fgets(name, 128, file);

			if (strlen(name) > 1 && *name != '#' && *name != '/')
			{
				if (name[strlen(name) - 1] == '\n')
					name[strlen(name) - 1] = 0;

				if (!g_AutoLoginNames.length())
					g_AutoLoginNames = string("|");

				g_AutoLoginNames += string(name) + "|";
			}
		}

		fclose(file);
	}
}
//---------------------------------------------------------------------------
void TUltimaOnline::LoadShaders()
{
	CurrentShader = NULL;

	TMappedHeader frag;
	memset(&frag, 0, sizeof(TMappedHeader));
	TMappedHeader vect;
	memset(&vect, 0, sizeof(TMappedHeader));

	FileManager.LoadFileToMemory(vect, FilePath("shaders\\Shader.vert").c_str());
	FileManager.LoadFileToMemory(frag, FilePath("shaders\\DeathShader.frag").c_str());

	DeathShader = new TDeathShader((char*)vect.Address, (char*)frag.Address);

	FileManager.UnloadFileFromMemory(frag);



	FileManager.LoadFileToMemory(frag, FilePath("shaders\\ColorizerShader.frag").c_str());

	ColorizerShader = new TColorizerShader((char*)vect.Address, (char*)frag.Address);

	FileManager.UnloadFileFromMemory(frag);



	FileManager.LoadFileToMemory(frag, FilePath("shaders\\FontColorizerShader.frag").c_str());

	FontColorizerShader = new TColorizerShader((char*)vect.Address, (char*)frag.Address);

	FileManager.UnloadFileFromMemory(frag);



	FileManager.LoadFileToMemory(frag, FilePath("shaders\\LightColorizerShader.frag").c_str());

	LightColorizerShader = new TColorizerShader((char*)vect.Address, (char*)frag.Address);

	FileManager.UnloadFileFromMemory(frag);
	FileManager.UnloadFileFromMemory(vect);
}
//---------------------------------------------------------------------------
bool TUltimaOnline::AutoLoginNameExists(string name)
{
	if (!g_AutoLoginNames.length())
		return false;

	string search = string("|") + name + "|";
	
	return (g_AutoLoginNames.find(search) != string::npos);
}
//---------------------------------------------------------------------------
void TUltimaOnline::LoadClientConfig()
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

		if (ver == 2)
			CharacterList.ClientFlag = file.ReadByte();
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
DWORD TUltimaOnline::GetFileHashCode(DWORD address, DWORD size)
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
void TUltimaOnline::LoadPluginConfig()
{
	PluginClientInterface._GL = &g_GL;
	PluginClientInterface._UO = UO;
	PluginClientInterface._GumpManager = GumpManager;
	PluginClientInterface._ClilocManager = ClilocManager;
	PluginClientInterface._ColorManager = ColorManager;
	PluginClientInterface._PathFinder = PathFinder;

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
				//HMODULE dll = LoadLibraryA(name.c_str());

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

						//if (plugin->CanClientAccess())
							//plugin->m_PPS->Client = ClientInterface;
						
						if (plugin->CanParseRecv())
							plugin->m_PPS->Recv= PluginRecvFunction;

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
void TUltimaOnline::LoadClientStartupConfig()
{
	ConfigManager.Load(FilePath("options_debug.cuo"));

	SoundManager.SetMusicVolume(ConfigManager.MusicVolume);

	if (ConfigManager.Music)
		SoundManager.PlayMidi(8);
}
//---------------------------------------------------------------------------
void TUltimaOnline::LoadStartupConfig()
{
	char buf[MAX_PATH] = {0};
	sprintf(buf, "Desktop\\%s\\%s\\0x%08X\\options_debug.cuo", MainScreen->m_Account->c_str(), ServerList.GetServerName().c_str(), g_PlayerSerial);

	ConfigManager.Load(FilePath(buf));

	SoundManager.SetMusicVolume(ConfigManager.MusicVolume);

	if (!ConfigManager.Sound)
		UO->ResetSoundEffects(GetTickCount() + 100000);

	if (!ConfigManager.Music)
		SoundManager.StopMusic();
}
//---------------------------------------------------------------------------
void TUltimaOnline::LoadLocalConfig()
{
	if (g_ConfigLoaded)
		return;

	/*char lco[150] = {0};
	sprintf(lco, "Login confirm to %s", ServerList.GetServerName().c_str());
	
	CreateTextMessage(TT_SYSTEM, 0xFFFFFFFF, 3, 0x35, lco);
	CreateTextMessage(TT_SYSTEM, 0xFFFFFFFF, 3, 0, "Welcome to Ultima Online!");*/

	//g_ContainerStack.clear();
	//g_CheckContainerStackTimer = GetTickCount() + 1000;

	char buf[MAX_PATH] = {0};
	sprintf(buf, "Desktop\\%s\\%s\\0x%08X", MainScreen->m_Account->c_str(), ServerList.GetServerName().c_str(), g_PlayerSerial);

	string path(buf);

	ConfigManager.Load(path + "\\options_debug.cuo");
	SkillGroupManager.Load(path + "\\skills_debug.cuo");
	MacroManager->Load(path + "\\macros_debug.cuo");
	GumpManager->Load(path + "\\gumps_debug.cuo");

	if (ConfigManager.OffsetInterfaceWindows)
		ContainerRect.MakeDefault();

	if (ConfigManager.GetConsoleNeedEnter())
		EntryPointer = NULL;
	else
		EntryPointer = GameConsole;

	SoundManager.SetMusicVolume(ConfigManager.MusicVolume);

	if (!ConfigManager.Sound)
		UO->ResetSoundEffects(GetTickCount() + 100000);

	if (!ConfigManager.Music)
		SoundManager.StopMusic();

	g_ConfigLoaded = true;
}
//---------------------------------------------------------------------------
void TUltimaOnline::SaveLocalConfig()
{
	if (!g_ConfigLoaded)
		return;

	string path = FilePath("Desktop");
	CreateDirectoryA(path.c_str(), NULL);
	
	path += string("\\") + MainScreen->m_Account->c_str();
	CreateDirectoryA(path.c_str(), NULL);
	
	path += string("\\") + ServerList.GetServerName();
	CreateDirectoryA(path.c_str(), NULL);

	char serbuf[100] = {0};
	sprintf(serbuf, "\\0x%08X", g_PlayerSerial);
	
	path += serbuf;
	CreateDirectoryA(path.c_str(), NULL);

	ConfigManager.Save(path + "\\options_debug.cuo");
	SkillGroupManager.Save(path + "\\skills_debug.cuo");
	MacroManager->Save(path + "\\macros_debug.cuo");
	GumpManager->Save(path + "\\gumps_debug.cuo");

	path += string("_") + g_Player->GetName() + ".cuo";
	
	if (!PathFileExistsA(path.c_str()))
	{
		FILE *file = fopen(path.c_str(), "wb");
		fclose(file);
	}
}
//---------------------------------------------------------------------------
void TUltimaOnline::Process()
{
	static DWORD removeUnusedTexturesTime = GetTickCount() + CLEAR_TEXTURES_DELAY;

	ConnectionManager.Recv();

	DWORD ticks = GetTickCount();
	
	if (g_GameState >= GS_CHARACTER && g_LastSendTime + SEND_TIMEOUT_DELAY < ticks)
	{
		g_PingCount++;

		BYTE ping[2] = {0x73, g_PingSequence};

		Send(ping, 2);
	}

	if (IsIconic(g_hWnd))
		return;

	g_AltPressed = GetAsyncKeyState(VK_MENU) & 0x80000000;
	g_CtrlPressed = GetAsyncKeyState(VK_CONTROL) & 0x80000000;
	g_ShiftPressed = GetAsyncKeyState(VK_SHIFT) & 0x80000000;

	if (g_GameState >= GS_GAME) // || g_GameState == GS_GAME_BLOCKED)
	{
		g_ShowGumpLocker = ConfigManager.LockGumpsMoving && g_AltPressed && g_CtrlPressed;

		g_SelectedObject = NULL;
		g_SelectedTextObject = NULL;

		if (g_LastRenderTime < ticks)
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
			
			World->ProcessAnimation();
		}

		bool CanRenderSelect = false;

		if (g_GameState == GS_GAME)
		{
			SendWalkStack();

			MacroManager->Execute();

			if (g_ClickObjectReq && g_ClickObject.Timer < ticks)
			{
				if (g_ClickObject.Type == COT_GAME_OBJECT)
					NameReq(g_ClickObject.Serial);
				else if (g_ClickObject.Type == COT_GUMP)
				{
					if (g_ClickObject.GumpType == GT_SPELLBOOK)
					{
						TGumpSpellbook *gump = (TGumpSpellbook*)GumpManager->GetGump(g_ClickObject.Serial, g_ClickObject.GumpID, g_ClickObject.GumpType);

						if (gump != NULL)
						{
							gump->Page = g_ClickObject.GumpButtonID;
							gump->UpdateFrame();
						}
					}
					else if (g_ClickObject.GumpType == GT_BOOK)
					{
						TGumpBook *gump = (TGumpBook*)GumpManager->GetGump(g_ClickObject.Serial, g_ClickObject.GumpID, g_ClickObject.GumpType);

						if (gump != NULL)
						{
							gump->Page = g_ClickObject.GumpButtonID;
							gump->UpdateFrame();
						}
					}
					else if (g_ClickObject.GumpType == GT_PAPERDOLL)
					{
						TGumpPaperdoll *gump = (TGumpPaperdoll*)GumpManager->GetGump(g_ClickObject.Serial, 0, g_ClickObject.GumpType);

						if (gump != NULL)
						{
							TTextData *td = new TTextData();
							td->SetUnicode(false);
							td->SetFont(3);
							td->SetSerial(0);
							td->SetColor(0x03B5);
							td->SetTimer(GetTickCount());
							td->SetType(TT_CLIENT);
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

							UO->AddJournalMessage(td, "");
						}
					}
				}

				g_ClickObjectReq = false;
			}

			CanRenderSelect = true;

			if (g_LastObjectType == SOT_GAME_GUMP_SCOPE)
			{
				if (g_LeftMouseDown && !g_LastGumpLeftMouseDown && !g_LastGumpRightMouseDown)
				{
					if (g_LastObjectLeftMouseDown == 1 || g_LastObjectLeftMouseDown == 2)
						CanRenderSelect = false;
				}
			}
		}
		
		if (g_LastRenderTime <= ticks)
		{
			GameScreen->CalculateGameWindow();

			if (CanRenderSelect)
				GameScreen->Render(false);

			GameScreen->Render(true);
		}
	}
	else if (g_LastRenderTime <= ticks)
	{
		CurrentScreen->Render(false);
		CurrentScreen->Render(true);
	}

	if (removeUnusedTexturesTime < ticks)
	{
		ClearUnusedTextures();
		removeUnusedTexturesTime = ticks + CLEAR_TEXTURES_DELAY;
	}
}
//---------------------------------------------------------------------------
int TUltimaOnline::Send(PBYTE buf, int size)
{
	DWORD ticks = GetTickCount();
	g_TotalSendSize += size;
	
	TMessageType &type = PacketManager.GetType(*buf);

	trace_printf("--- ^(%d) s(+%d => %d) Client:: %s\n", ticks - g_LastPacketTime, size, g_TotalSendSize, type.name);

	if (*buf == 0x80 || *buf == 0x91)
	{
		TDUMP(buf, size / 2);
		TPRINT("**** PASSWORD CENSORED ****\n");
	}
	else
		TDUMP(buf, size);
	
	g_LastPacketTime = ticks;
	g_LastSendTime = ticks;

	if (type.direction != DIR_SEND && type.direction != DIR_BOTH)
		warning_printf("message direction invalid: 0x%02X\n", *buf);
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
TTextureObject *TUltimaOnline::ExecuteGump(WORD id, bool partialHue)
{
	TIndexObject &io = m_GumpDataIndex[id];

	if (io.Texture == NULL)
	{
		if (!io.Address)
			return NULL;

		io.Texture = MulReader.ReadGump(io.Address, io.Size, io.Width, io.Height);

		if (io.Texture != NULL)
		{
			ADD_LINKED(m_UsedGumpList, m_GumpDataIndex[id]);
		}
	}

	io.LastAccessTime = GetTickCount();

	return io.Texture;
}
//---------------------------------------------------------------------------
TTextureObject *TUltimaOnline::ExecuteLandArt(WORD id)
{
	TIndexObject &io = m_LandDataIndex[id];

	if (io.Texture == NULL)
	{
		if (!io.Address || id == 0x02) //nodraw tiles banned
			return NULL;

		io.Texture = MulReader.ReadArt(id, io.Address, io.Size);

		if (io.Texture != NULL)
		{
			ADD_LINKED(m_UsedLandList, m_LandDataIndex[id]);
		}
	}
	
	io.LastAccessTime = GetTickCount();

	return io.Texture;
}
//---------------------------------------------------------------------------
TTextureObject *TUltimaOnline::ExecuteStaticArtAnimated(WORD id)
{
	id += m_StaticDataIndex[id].Increment;

	return ExecuteStaticArt(id);
}
//---------------------------------------------------------------------------
TTextureObject *TUltimaOnline::ExecuteStaticArt(WORD id)
{
	TIndexObject &io = m_StaticDataIndex[id];

	if (io.Texture == NULL)
	{
		if (!io.Address || id == 0x01) //nodraw tiles banned
			return NULL;

		io.Texture = MulReader.ReadArt(id + 0x4000, io.Address, io.Size);

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
TTextureObject *TUltimaOnline::ExecuteTexture(WORD id)
{
	id = m_LandData[id / 32].Tiles[id % 32].TexID;

	if (!id)
		return NULL;

	TIndexObject &io = m_TextureDataIndex[id];

	if (io.Texture == NULL)
	{
		if (!io.Address)
			return NULL;

		io.Texture = MulReader.ReadTexture(id, io.Address, io.Size);

		if (io.Texture != NULL)
		{
			ADD_LINKED(m_UsedTextureList, m_TextureDataIndex[id]);
		}
	}
	
	io.LastAccessTime = GetTickCount();

	return io.Texture;
}
//---------------------------------------------------------------------------
TTextureObject *TUltimaOnline::ExecuteLight(BYTE &id)
{
	if (id > 100 || m_LightDataIndex[id].Address == NULL)
		id = 0;
	
	TIndexObject &io = m_LightDataIndex[id];

	if (io.Texture == NULL)
	{
		if (!io.Address)
			return NULL;

		io.Texture = MulReader.ReadLight(id, io.Address, io.Size, io.Width, io.Height);

		if (io.Texture != NULL)
		{
			ADD_LINKED(m_UsedLightList, m_LightDataIndex[id]);
		}
	}
	
	io.LastAccessTime = GetTickCount();

	return io.Texture;
}
//---------------------------------------------------------------------------
bool TUltimaOnline::ExecuteGumpPart(WORD id, int count)
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
void TUltimaOnline::DrawSphereGump(int value, float maxValue, int x, int y, int width)
{
	DrawGump(0x00D5, 0, x + 64, y); //Sphere line gump start
	DrawGump(0x00D7, 0, x + 152, y); //Sphere line gump end
	DrawGump(0x00D6, 0, x + 77, y, 75, 0); //Sphere line gump body

	int ofs = CalculateSphereOffset(100, value, width, maxValue);

	DrawGump(0x00D8, 0, x + 64 + ofs, y); //Sphere gump
}
//---------------------------------------------------------------------------
void TUltimaOnline::DrawGump(WORD id, WORD color, int x, int y, bool partialHue)
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
void TUltimaOnline::DrawGump(WORD id, WORD color, int x, int y, int width, int height, bool partialHue)
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
void TUltimaOnline::DrawResizepicGump(WORD id, int x, int y, int width, int height)
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
void TUltimaOnline::DrawLandTexture(WORD id, WORD color, int x, int y, RECT rc, TVector *normals)
{
	TTextureObject *th = ExecuteTexture(id);

	if (th == NULL)
	{
		DrawLandArt(id, color, x, y, rc.left / 4);

		return;
	}
	
	if (g_OutOfRangeColor)
		color = g_OutOfRangeColor;
	
	int drawMode = 6;

	if (!g_GrayedPixels && color)
	{
		drawMode = 7;
		ColorManager->SendColorsToShader(color);
	}

	glUniform1iARB(ShaderDrawMode, drawMode);

	x -= 23;
	y -= 23;
	
	g_GL.DrawLandTexture(th->Texture, x, y, rc, normals);
}
//---------------------------------------------------------------------------
void TUltimaOnline::DrawLandArt(WORD id, WORD color, int x, int y, int z)
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

		x -= 23;
		y -= 23 + (z * 4);

		g_GL.Draw(th->Texture, x, y, th->Width, th->Height);
	}
}
//---------------------------------------------------------------------------
void TUltimaOnline::DrawStaticArt(WORD id, WORD color, int x, int y, int z, bool selection)
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

		g_GL.Draw(th->Texture, x, y, th->Width, th->Height);
	}
}
//---------------------------------------------------------------------------
void TUltimaOnline::DrawStaticArtAnimated(WORD id, WORD color, int x, int y, int z, bool selection)
{
	id += m_StaticDataIndex[id].Increment;

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

		g_GL.Draw(th->Texture, x, y, th->Width, th->Height);
	}
}
//---------------------------------------------------------------------------
void TUltimaOnline::DrawStaticArtTransparent(WORD id, WORD color, int x, int y, int z, bool selection)
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
void TUltimaOnline::DrawStaticArtAnimatedTransparent(WORD id, WORD color, int x, int y, int z, bool selection)
{
	id += m_StaticDataIndex[id].Increment;

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
void TUltimaOnline::DrawStaticArtInContainer(WORD id, WORD color, int x, int y, bool selection, bool onMouse)
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

			if (partialHue)
				drawMode = 2;

			ColorManager->SendColorsToShader(color);
		}

		glUniform1iARB(ShaderDrawMode, drawMode);

		g_GL.Draw(th->Texture, x, y, DW, DH);
	}
}
//---------------------------------------------------------------------------
void TUltimaOnline::DrawLight(BYTE id, WORD color, int x, int y)
{
	TTextureObject *th = ExecuteLight(id);

	if (th != NULL)
	{
		x -= th->Width / 2;
		y -= th->Height / 2;

		int drawMode = 0;

		if (color)
		{
			drawMode = 1;

			ColorManager->SendColorsToShader(color);
		}

		glUniform1iARB(ShaderDrawMode, drawMode);

		g_GL.Draw(th->Texture, x, y, th->Width, th->Height);
	}
}
//---------------------------------------------------------------------------
bool TUltimaOnline::PolygonePixelsInXY(int x, int y, int width, int height)
{
	x = g_MouseX - x;
	y = g_MouseY - y;

	return !(x < 0 || y < 0 || x >= width || y >= height);
}
//---------------------------------------------------------------------------
bool TUltimaOnline::GumpPixelsInXY(WORD id, int x, int y, bool noSubMouse)
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
		result = MulReader.GumpPixelsInXY(io.Address, io.Size, th->Width, th->Height, x, y);
#endif
	}

	return result;
}
//---------------------------------------------------------------------------
bool TUltimaOnline::GumpPixelsInXY(WORD id, int x, int y, int width, int height, bool noSubMouse)
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
	result = MulReader.GumpPixelsInXY(io.Address, io.Size, th->Width, th->Height, x, y);
#endif

	return result;
}
//---------------------------------------------------------------------------
bool TUltimaOnline::ResizepicPixelsInXY(WORD id, int x, int y, int width, int height)
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
bool TUltimaOnline::StaticPixelsInXY(WORD id, int x, int y, int z)
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
		result = MulReader.ArtPixelsInXY(id + 0x4000, io.Address, io.Size, th->Width, th->Height, x, y);
#endif
	}

	return result;
}
//---------------------------------------------------------------------------
bool TUltimaOnline::StaticPixelsInXYAnimated(WORD id, int x, int y, int z)
{
	id += m_StaticDataIndex[id].Increment;

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
		result = MulReader.ArtPixelsInXY(id + 0x4000, io.Address, io.Size, th->Width, th->Height, x, y);
#endif
	}

	return result;
}
//---------------------------------------------------------------------------
bool TUltimaOnline::CircleTransPixelsInXY()
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
bool TUltimaOnline::StaticPixelsInXYInContainer(WORD id, int x, int y)
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
		result = MulReader.ArtPixelsInXY(id + 0x4000, io.Address, io.Size, th->Width, th->Height, x, y);
#endif
	}

	return result;
}
//---------------------------------------------------------------------------
bool TUltimaOnline::LandPixelsInXY(WORD id, int x, int  y, int z)
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
		result = MulReader.ArtPixelsInXY(id, io.Address, io.Size, th->Width, th->Height, x, y);
#endif
	}

	return result;
}
//---------------------------------------------------------------------------
bool TUltimaOnline::LandTexturePixelsInXY(int x, int  y, RECT &r)
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
DWORD TUltimaOnline::GetLandFlags(WORD id)
{
	WORD DivID = id / 32;
	if (DivID < 512)
		return m_LandData[DivID].Tiles[id % 32].Flags;

	return 0;
}
//--------------------------------------------------------------------------
DWORD TUltimaOnline::GetStaticFlags(WORD id)
{
	WORD divID = id / 32;
	if (divID < m_StaticDataCount)
		return m_StaticData[divID].Tiles[id % 32].Flags;

	return 0;
}
//---------------------------------------------------------------------------
void TUltimaOnline::GetArtDimension(WORD id, POINT &p)
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
void TUltimaOnline::GetGumpDimension(WORD id, POINT &p)
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
TIndexObject *TUltimaOnline::GetGumpPointer(WORD id)
{
	return &m_GumpDataIndex[id];
}
//---------------------------------------------------------------------------
TIndexMulti *TUltimaOnline::GetMultiPointer(WORD id)
{
	return &m_MultiDataIndex[id];
}
//---------------------------------------------------------------------------
void TUltimaOnline::PlaySoundEffect(WORD id, int volume)
{
	if (id >= 0x0800 || !ConfigManager.Sound)
		return;

	//HSTREAM hStream = BASS_StreamCreateFile(false, L"c:\\n_frst01.wav", 0, 0, 0);
	//BASS_ChannelPlay(hStream, true);	

	TIndexSound &is = m_SoundDataIndex[id];
	
	if (is.Address == 0)
		return;

	if (is.soundStream == 0)
	{
		is.soundStream = SoundManager.LoadSoundEffect(m_SoundDataIndex[id]);

		if (is.soundStream == 0)
			return;

		ADD_LINKED(m_UsedSoundList, m_SoundDataIndex[id]);
	}
	else {
		if (is.LastAccessTime + is.Timer > GetTickCount())
			return;

		BASS_StreamFree(is.soundStream);

		is.soundStream = SoundManager.LoadSoundEffect(m_SoundDataIndex[id]);
	}	

	if (volume < 0)
		volume = ConfigManager.SoundVolume;

	if (volume > 0)
	{
		SoundManager.PlaySoundEffect(is.soundStream, volume);
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
void TUltimaOnline::ResetSoundEffects(DWORD ticks)
{
	TLinkedList *list = m_UsedSoundList;
	TLinkedList *prev = list;

	while (list != NULL)
	{
		TIndexSound *obj = (TIndexSound*)list->Data;

		if (obj->soundStream != 0 && obj->LastAccessTime + obj->Timer < ticks)
		{
			BASS_StreamFree(obj->soundStream);

			//Mix_FreeChunk(obj->Sound);
			//obj->Sound = NULL;
			obj->soundStream = 0;

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
void TUltimaOnline::CreateTextMessage(TEXT_TYPE type, DWORD serial, WORD font, WORD color, string text)
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
					td->GenerateTexture(TEXT_MESSAGE_MAX_WIDTH, 0, TS_LEFT);
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
				td->GenerateTexture(TEXT_MESSAGE_MAX_WIDTH, 0, TS_CENTER);
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
void TUltimaOnline::CreateUnicodeTextMessage(TEXT_TYPE type, DWORD serial, WORD font, WORD color, wstring text)
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
					td->GenerateTexture(TEXT_MESSAGE_MAX_WIDTH, UOFONT_BLACK_BORDER, TS_LEFT);
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
void TUltimaOnline::AddSystemMessage(TTextData *msg)
{
	SystemChat->Add(msg);
	AddJournalMessage(msg, "");
}
//---------------------------------------------------------------------------
void TUltimaOnline::AddJournalMessage(TTextData *msg, string name)
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

	Journal->Add(jmsg);
}
//---------------------------------------------------------------------------
void TUltimaOnline::Connect()
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
void TUltimaOnline::Disconnect()
{
	if (SystemChat != NULL)
		SystemChat->Clear();

	if (Journal != NULL)
		Journal->Clear();

	ConnectionManager.Disconnect();

	Party.Leader = 0;
	Party.Inviter = 0;
	Party.Clear();

	g_ResizedGump = NULL;
}
//---------------------------------------------------------------------------
void TUltimaOnline::ServerSelection(int pos)
{
	InitScreen(GS_SERVER_CONNECT);
	Process();

	ServerList.SelectedServer = pos;

	TPacketSelectServer packet((BYTE)ServerList.GetIndex(pos));
	packet.Send();
}
//---------------------------------------------------------------------------
void TUltimaOnline::RelayServer(const char *ip, int port, PBYTE gameSeed)
{
	if (ConnectionManager.Init(gameSeed) == CE_NO_ERROR)
	{
		memcpy(g_GameSeed, gameSeed, 4);

		if (ConnectionManager.Connect(ip, port, g_GameSeed) == CE_NO_ERROR)
		{
			ConnectionScreen->Connected = true;

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
void TUltimaOnline::CharacterSelection(int pos)
{
	InitScreen(GS_GAME_CONNECT);
	ConnectionScreen->Type = CST_GAME;

	TPacketSelectCharacter packet(pos, CharacterList.GetName(pos));
	packet.Send();
}
//---------------------------------------------------------------------------
void TUltimaOnline::LoginComplete()
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

		//TPacketClientType clientTypePacket(g_ClientFlag);
		//clientTypePacket.Send();
	
		//TPacketClientViewRange vrPacket(g_UpdateRange);
		//vrPacket.Send();

		InitScreen(GS_GAME);

		LoadLocalConfig();
	}
}
//---------------------------------------------------------------------------
void TUltimaOnline::PickupItem(TGameItem *obj, int count, bool isGameFigure)
{
	if (ObjectInHand != NULL)
		return;

	ObjectInHand = new TObjectOnCursor(obj);
	
	if (!count)
		count = obj->Count;

	ObjectInHand->Separated = count != obj->Count;
	ObjectInHand->IsGameFigure = isGameFigure;
	ObjectInHand->DragCount = count;

	TPacketPickupRequest packet(obj->Serial, count);
	packet.Send();
}
//---------------------------------------------------------------------------
void TUltimaOnline::DropItem(DWORD container, WORD x, WORD y, char z)
{
	if (ObjectInHand == NULL)
		return;

	if (ObjectInHand->Dropped)
	{
		delete ObjectInHand;
		ObjectInHand = NULL;

		return;
	}

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
//---------------------------------------------------------------------------
void TUltimaOnline::EquipItem(DWORD container)
{
	if (ObjectInHand == NULL)
		return;

	if (ObjectInHand->Dropped)
	{
		delete ObjectInHand;
		ObjectInHand = NULL;

		return;
	}

	if (!ObjectInHand->IsWearable())
		return;

	ObjectInHand->Dropped = true;

	int ID = ObjectInHand->Graphic / 32;
	if (ID >= m_StaticDataCount)
		return;

	STATIC_TILES &st = m_StaticData[ID].Tiles[ObjectInHand->Graphic % 32];

	if (!container)
		container = g_PlayerSerial;

	TPacketEquipRequest packet(ObjectInHand->Serial, st.Quality, container);
	packet.Send();
}
//---------------------------------------------------------------------------
void TUltimaOnline::ChangeWarmode(BYTE status)
{
	BYTE newstatus = (BYTE)(!g_Player->Warmode);
	if (status != 0xFF)
	{
		if (g_Player->Warmode == (bool)status)
			return;

		newstatus = status;
	}

	TPacketChangeWarmode packet(newstatus);
	packet.Send();
}
//---------------------------------------------------------------------------
void TUltimaOnline::HelpRequest()
{
	TPacketHelpRequest packet;
	packet.Send();
}
//---------------------------------------------------------------------------
void TUltimaOnline::StatusReq(DWORD serial)
{
	TPacketStatusRequest packet(serial);
	packet.Send();
}
//---------------------------------------------------------------------------
void TUltimaOnline::SkillsReq(DWORD serial)
{
	TPacketSkillsRequest packet(serial);
	packet.Send();
}
//---------------------------------------------------------------------------
void TUltimaOnline::SkillStatusChange(BYTE skill, BYTE state)
{
	TPacketSkillsStatusChangeRequest packet(skill, state);
	packet.Send();
}
//---------------------------------------------------------------------------
void TUltimaOnline::Click(DWORD serial)
{
	TPacketClickRequest packet(serial);
	packet.Send();

	TGameObject *obj = World->FindWorldObject(serial);
	if (obj != NULL)
		obj->Clicked = true;
}
//---------------------------------------------------------------------------
void TUltimaOnline::DoubleClick(DWORD serial)
{
	g_LastUseObject = serial;

	TPacketDoubleClickRequest packet(serial);
	packet.Send();
}
//---------------------------------------------------------------------------
void TUltimaOnline::PaperdollReq(DWORD serial)
{
	g_LastUseObject = serial;

	TPacketDoubleClickRequest packet(serial | 0x80000000);
	packet.Send();
}
//---------------------------------------------------------------------------
void TUltimaOnline::Attack(DWORD serial)
{
	if (ConfigManager.CriminalActionsQuery && World != NULL)
	{
		TGameCharacter *target = World->FindWorldCharacter(serial);

		if (target != NULL && (NOTORIETY_TYPE)g_Player->Notoriety == NT_INNOCENT && (NOTORIETY_TYPE)target->Notoriety == NT_INNOCENT)
		{
			int x = g_GameWindowPosX + (g_GameWindowSizeX / 2) - 40;
			int y = g_GameWindowPosY + (g_GameWindowSizeY / 2) - 20;

			TGumpQuestion *newgump = new TGumpQuestion(g_PlayerSerial, x, y, 2);
			newgump->SetID(serial);
			
			GumpManager->AddGump(newgump);

			UO->InitScreen(GS_GAME_BLOCKED);
			GameBlockedScreen->Code = 3;

			return;
		}
	}

	AttackReq(serial);
}
//---------------------------------------------------------------------------
void TUltimaOnline::AttackReq(DWORD serial)
{
	g_LastAttackObject = serial;
	
	TPacketAttackRequest packet(serial);
	packet.Send();
}
//---------------------------------------------------------------------------
void TUltimaOnline::SendASCIIText(const char *str, SPEECH_TYPE type)
{
	TPacketASCIISpeechRequest packet(str, type, 3, ConfigManager.SpeechColor);
	packet.Send();
	packet.Free();
}
//---------------------------------------------------------------------------
void TUltimaOnline::CastSpell(int index)
{
	if (index < 0)
		return;

	g_LastSpellIndex = index;

	TPacketCastSpell packet(index);
	packet.Send();
}
//---------------------------------------------------------------------------
void TUltimaOnline::CastSpellFromBook(int index, DWORD serial)
{
	if (index < 0)
		return;

	g_LastSpellIndex = index;

	TPacketCastSpellFromBook packet(index, serial);
	packet.Send();
	packet.Free();
}
//---------------------------------------------------------------------------
void TUltimaOnline::UseSkill(int index)
{
	if (index < 0)
		return;

	g_LastSkillIndex = index;
	
	TPacketUseSkill packet(index);
	packet.Send();
}
//---------------------------------------------------------------------------
void TUltimaOnline::OpenDoor()
{
	TPacketOpenDoor packet;
	packet.Send();
}
//---------------------------------------------------------------------------
void TUltimaOnline::EmoteAction(const char *action)
{
	TPacketEmoteAction packet(action);
	packet.Send();
	packet.Free();
}
//---------------------------------------------------------------------------
void TUltimaOnline::AllNames()
{
	TGameObject *obj = World->m_Items;

	while (obj != NULL)
	{
		if (obj->NPC && !obj->IsPlayer())
		{
			TPacketClickRequest packet(obj->Serial);
			packet.Send();
		}

		obj = (TGameObject*)obj->m_Next;
	}
}
//---------------------------------------------------------------------------
void TUltimaOnline::GoToWebLink(string url)
{
	if (url.length())
		ShellExecuteA(0, "Open", url.c_str(), NULL, NULL, SW_SHOWNORMAL);
}
//---------------------------------------------------------------------------
void TUltimaOnline::RemoveRangedObjects()
{
	int objectsRange = g_UpdateRange + 1;
	int multiRange = g_UpdateRange + 12;

	if (World != NULL)
	{
		TGameObject *go = World->m_Items;
	
		while (go != NULL)
		{
			TGameObject *next = (TGameObject*)go->m_Next;

			if (go->Container == 0xFFFFFFFF && !go->IsPlayer())
			{
				int distance = GetDistance(g_RemoveRangeXY, go);
				
				if (go->Graphic >= 0x4000)
				{
					if (distance > multiRange)
						World->RemoveObject(go);
				}
				else if (distance > objectsRange)
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
	{
		EffectManager->RemoveRangedEffects();
		EffectManager->UpdateEffects();
	}
}
//---------------------------------------------------------------------------
void TUltimaOnline::SendWalkStack()
{
	if (g_LastStepTime > GetTickCount() || g_WalkRequestCount > 1 || !Walker->m_SendStack.size())
		return;

	WALKER_SEND_ITEM &wsi = Walker->m_SendStack.front();

	BYTE seq = Walker->GetSequence();
	Walker->SetSequence(seq, wsi.Dir);

	BYTE buf[7] = {0};
	*buf = 0x02;
	buf[1] = wsi.Dir;
	buf[2] = seq;
	pack32(buf + 3, Walker->m_FastWalkStack.Pop());

	Send(buf, 7);

	g_WalkRequestCount++;

	Walker->IncSequence();

	g_LastStepTime = GetTickCount() + wsi.Time;

	Walker->m_SendStack.pop_front();
}
//---------------------------------------------------------------------------
void TUltimaOnline::LogOut()
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

	if (World != NULL)
	{
		delete World;
		World = NULL;
	}
	TPRINT("\tWorld removed?\n");
	
	if (ObjectInHand != NULL)
	{
		delete ObjectInHand;
		ObjectInHand = NULL;
	}
	TPRINT("\tObjectInHand removed?\n");
	
	if (GumpManager != NULL)
		GumpManager->Clear();
	TPRINT("\tGump List cleared?\n");
	
	if (EffectManager != NULL)
		EffectManager->Clear();
	TPRINT("\tEffect List cleared?\n");
	
	//ClearPacketStream();
	//TPRINT("\tPacket Stream removed?\n");

	if (GameConsole != NULL)
		GameConsole->Clear();
	EntryPointer = NULL;
	TPRINT("\tGame console cleared?\n");

	g_GrayMenuCount = 0;
	Target.Reset();
	TPRINT("\tTarget reseted?\n");

	if (SystemChat != NULL)
		SystemChat->Clear();
	TPRINT("\tSystem chat cleared?\n");

	if (Journal != NULL)
		Journal->Clear();
	TPRINT("\tJournal cleared?\n");

	InitScreen(GS_MAIN);
	TPRINT("TUltimaOnline::LogOut->End\n");
}
//---------------------------------------------------------------------------
void TUltimaOnline::OpenPaperdoll()
{
	PaperdollReq(g_PlayerSerial);
}
//---------------------------------------------------------------------------
void TUltimaOnline::OpenStatus(DWORD serial)
{
	int x = g_MouseX - 76;
	int y = g_MouseY - 30;

	StatusReq(serial);

	TGumpStatusbar *gump = new TGumpStatusbar(serial, x, y, true);

	GumpManager->AddGump(gump);
}
//---------------------------------------------------------------------------
void TUltimaOnline::OpenMinimap()
{
	TGumpMinimap *gump = new TGumpMinimap(g_PlayerSerial, 0, 0, true);
	GumpManager->AddGump(gump);
}
//---------------------------------------------------------------------------
void TUltimaOnline::OpenWorldMap()
{
	int x = g_GameWindowPosX + (g_GameWindowSizeX / 2) - 200;
	int y = g_GameWindowPosY + (g_GameWindowSizeY / 2) - 150;
	TGumpWorldMap *gump = new TGumpWorldMap(g_PlayerSerial, x, y);
	gump->Called = true;
	GumpManager->AddGump(gump);
}
//---------------------------------------------------------------------------
void TUltimaOnline::OpenJournal()
{
	TGumpJournal *gump = new TGumpJournal(g_PlayerSerial, 0, 0, false);

	GumpManager->AddGump(gump);
}
//---------------------------------------------------------------------------
void TUltimaOnline::OpenSkills()
{
	TGumpSkills *gump = new TGumpSkills(g_PlayerSerial, 0, 0, false);

	GumpManager->AddGump(gump);
}
//---------------------------------------------------------------------------
void TUltimaOnline::OpenBackpack()
{
	if (g_Player != NULL)
	{
		TGameItem *pack = g_Player->FindLayer(OL_BACKPACK);
		if (pack != NULL)
			DoubleClick(pack->Serial);
	}
}
//---------------------------------------------------------------------------
void TUltimaOnline::OpenSpellbook()
{
}
//---------------------------------------------------------------------------
void TUltimaOnline::OpenLogOut()
{
	int x = g_GameWindowPosX + (g_GameWindowSizeX / 2) - 40;
	int y = g_GameWindowPosY + (g_GameWindowSizeY / 2) - 20;
	TGumpQuestion *gump = new TGumpQuestion(g_PlayerSerial, x, y, 1);
	
	GumpManager->AddGump(gump);

	InitScreen(GS_GAME_BLOCKED);
	GameBlockedScreen->SetCode(3);
}
//---------------------------------------------------------------------------
void TUltimaOnline::OpenChat()
{
}
//---------------------------------------------------------------------------
void TUltimaOnline::OpenConfiguration()
{
	int x = (GetSystemMetrics(SM_CXSCREEN) / 2) - 383;
	int y = (GetSystemMetrics(SM_CYSCREEN) / 2) - 260;

	TGumpOptions *gump = new TGumpOptions(g_PlayerSerial, x, y);

	g_OptionsConfig = ConfigManager;

	GumpManager->AddGump(gump);
}
//---------------------------------------------------------------------------
void TUltimaOnline::OpenMail()
{
}
//---------------------------------------------------------------------------
void TUltimaOnline::OpenPartyManifest()
{
	int x = (g_ClientWidth / 2) - 272;
	int y = (g_ClientHeight / 2) - 240;

	TGumpPartyManifest *gump = new TGumpPartyManifest(0, x, y, Party.CanLoot);

	GumpManager->AddGump(gump);
}
//---------------------------------------------------------------------------
void TUltimaOnline::OpenPartyChat()
{
}
//---------------------------------------------------------------------------
void TUltimaOnline::OpenProfile(DWORD serial)
{
	if (!serial)
		serial = g_PlayerSerial;

	TPacketProfileRequest packet(serial);
	packet.Send();
}
//---------------------------------------------------------------------------
void TUltimaOnline::ChangeMap(BYTE newmap)
{
	if (newmap < 0 || newmap > 5)
		newmap = 0;

	if (g_CurrentMap != newmap)
	{
		g_CurrentMap = newmap;

		if (g_Player != NULL)
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
void TUltimaOnline::ConsolePromptSend()
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
void TUltimaOnline::ConsolePromptCancel()
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