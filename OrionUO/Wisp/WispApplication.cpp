﻿// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
//----------------------------------------------------------------------------------
#include "stdafx.h"

namespace WISP_APPLICATION
{
CApplication *g_WispApplication = NULL;
//----------------------------------------------------------------------------------
CApplication::CApplication()
{
	LOG("INITIATING CAPPLICATION");
	g_MainThread = GetCurrentThreadId();
	WISPFUN_DEBUG("c1_f1");
	g_WispApplication = this;
	ExePathA.resize(MAX_PATH, 0);
	ExePathW.resize(MAX_PATH, 0);

	GetCurrentDirectoryA(MAX_PATH, &ExePathA[0]);
	ExePathA = ExePathA.c_str();

	GetCurrentDirectoryW(MAX_PATH, &ExePathW[0]);
	ExePathW = ExePathW.c_str();
	UOFilesPathA = ExePathA;
	UOFilesPathW = ExePathW;
	g_MainScreen.LoadCustomPath();
}
//----------------------------------------------------------------------------------
CApplication::~CApplication()
{
	WISPFUN_DEBUG("c1_f2");
	g_WispApplication = NULL;
	Hinstance = 0;
}
//----------------------------------------------------------------------------------
int CApplication::Run(HINSTANCE hinstance)
{
	WISPFUN_DEBUG("c1_f3");
	timeBeginPeriod(1);
	Hinstance = hinstance;

	MSG msg = { 0 };

	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
			Sleep(1);

		OnMainLoop();
	}

	timeEndPeriod(1);

	return (int)msg.wParam;
}
//---------------------------------------------------------------------------
string CApplication::GetFileVersion(uint *numericVerion)
{
	//File version info
	wchar_t szFilename[MAX_PATH] = { 0 };

	if (GetModuleFileName(Hinstance, &szFilename[0], sizeof(szFilename)))
	{
		DWORD dummy = 0;
		DWORD dwSize = GetFileVersionInfoSize(&szFilename[0], &dummy);

		if (dwSize > 0)
		{
			UCHAR_LIST lpVersionInfo(dwSize, 0);

			if (GetFileVersionInfo(&szFilename[0], NULL, dwSize, &lpVersionInfo[0]))
			{
				UINT uLen = 0;
				VS_FIXEDFILEINFO *lpFfi = NULL;

				VerQueryValue(&lpVersionInfo[0], L"\\", (LPVOID *)&lpFfi, &uLen);

				DWORD dwFileVersionMS = 0;
				DWORD dwFileVersionLS = 0;

				if (lpFfi != NULL)
				{
					dwFileVersionMS = lpFfi->dwFileVersionMS;
					dwFileVersionLS = lpFfi->dwFileVersionLS;
				}

				int dwLeftMost = (int)HIWORD(dwFileVersionMS);
				int dwSecondLeft = (int)LOWORD(dwFileVersionMS);
				int dwSecondRight = (int)HIWORD(dwFileVersionLS);
				int dwRightMost = (int)LOWORD(dwFileVersionLS);

				if (numericVerion != NULL)
					*numericVerion = ((dwLeftMost & 0xFF) << 24) | ((dwSecondLeft & 0xFF) << 16) | ((dwSecondRight & 0xFF) << 8) | (dwRightMost & 0xFF);

				char fileVersion[100] = { 0 };
				sprintf_s(fileVersion, "%i.%i.%i.%i", dwLeftMost, dwSecondLeft, dwSecondRight, dwRightMost);

				return fileVersion;
			}
		}
	}

	return "unknown";
}
//---------------------------------------------------------------------------
string CApplication::ExeFilePath(const char *str, ...)
{
	WISPFUN_DEBUG("c1_f4");
	va_list arg;
	va_start(arg, str);

	char out[MAX_PATH] = { 0 };
	vsprintf_s(out, str, arg);

	va_end(arg);
	string res = ExePathA + "\\" + out;
	return res;
}
//---------------------------------------------------------------------------
wstring CApplication::ExeFilePath(const wchar_t *str, ...)
{
	WISPFUN_DEBUG("c1_f5");
	va_list arg;
	va_start(arg, str);

	wchar_t out[MAX_PATH] = { 0 };
	vswprintf_s(out, str, arg);

	va_end(arg);

	return ExePathW + L"\\" + out;
}
//---------------------------------------------------------------------------
string CApplication::UOFilesPath(const char *str, ...)
{
	WISPFUN_DEBUG("c1_f6");
	va_list arg;
	va_start(arg, str);

	char out[MAX_PATH] = { 0 };
	vsprintf_s(out, str, arg);

	va_end(arg);
	string res = UOFilesPathA + "\\" + out;
	return res;
}
//---------------------------------------------------------------------------
wstring CApplication::UOFilesPath(const wchar_t *str, ...)
{
	WISPFUN_DEBUG("c1_f7");
	va_list arg;
	va_start(arg, str);

	wchar_t out[MAX_PATH] = { 0 };
	vswprintf_s(out, str, arg);

	va_end(arg);

	return UOFilesPathW + L"\\" + out;
}
//----------------------------------------------------------------------------------
}; //namespace
//----------------------------------------------------------------------------------
