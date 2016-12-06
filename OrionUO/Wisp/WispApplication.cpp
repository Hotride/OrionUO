//----------------------------------------------------------------------------------
#include "WispApplication.h"
#include <Mmsystem.h>

#pragma comment(lib, "Winmm.lib")

namespace WISP_APPLICATION
{
CApplication *g_WispApplication = NULL;
//----------------------------------------------------------------------------------
CApplication::CApplication()
: m_Hinstance(0), m_ExePathA(""), m_ExePathW(L"")
{
	g_WispApplication = this;

	m_ExePathA.resize(MAX_PATH, 0);
	m_ExePathW.resize(MAX_PATH, 0);

	GetCurrentDirectoryA(MAX_PATH, &m_ExePathA[0]);
	m_ExePathA = m_ExePathA.c_str();

	GetCurrentDirectoryW(MAX_PATH, &m_ExePathW[0]);
	m_ExePathW = m_ExePathW.c_str();
}
//----------------------------------------------------------------------------------
CApplication::~CApplication()
{
	g_WispApplication = NULL;
	m_Hinstance = 0;
}
//----------------------------------------------------------------------------------
int CApplication::Run(HINSTANCE hinstance)
{
	timeBeginPeriod(1);
	m_Hinstance = hinstance;

	MSG msg = { 0 };

	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			Sleep(1);
			OnIDLELoop();
		}
	}

	timeEndPeriod(1);

	return (int)msg.wParam;
}
//---------------------------------------------------------------------------
string CApplication::FilePath(const char *str, ...)
{
	va_list arg;
	va_start(arg, str);

	char out[MAX_PATH] = { 0 };
	vsprintf_s(out, str, arg);

	va_end(arg);

	return m_ExePathA + "\\" + out;
}
//---------------------------------------------------------------------------
wstring CApplication::FilePath(const wchar_t *str, ...)
{
	va_list arg;
	va_start(arg, str);

	wchar_t out[MAX_PATH] = { 0 };
	vswprintf_s(out, str, arg);

	va_end(arg);

	return m_ExePathW + L"\\" + out;
}
//----------------------------------------------------------------------------------
}; //namespace
//----------------------------------------------------------------------------------
