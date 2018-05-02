// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
//----------------------------------------------------------------------------------
#include "stdafx.h"
#include "FileSystem.h"
#include "WispThread.h"
#include <SDL_timer.h>
namespace WISP_APPLICATION
{
CApplication *g_WispApplication = nullptr;
//----------------------------------------------------------------------------------
CApplication::CApplication()
{
    LOG("INITIATING CAPPLICATION\n");
    g_MainThread = CThread::GetCurrentThreadId();
    WISPFUN_DEBUG("c1_f1");
    g_WispApplication = this;
    m_UOPath = m_ExePath = fs_path_current();
    g_MainScreen.LoadCustomPath();
}
//----------------------------------------------------------------------------------
CApplication::~CApplication()
{
    WISPFUN_DEBUG("c1_f2");
    g_WispApplication = nullptr;
    Hinstance = 0;
}
//----------------------------------------------------------------------------------
int CApplication::Run(HINSTANCE hinstance)
{
    // WISPFUN_DEBUG("c1_f3");
#if USE_WISP
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
            SDL_Delay(1);

        OnMainLoop();
    }

    timeEndPeriod(1);

    return (int)msg.wParam;
#else
    bool quit = false;
    while (!quit)
    {
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            if (!(quit = WISP_WINDOW::g_WispWindow->OnWindowProc(event)))
                OnMainLoop();
        }
    }

    return EXIT_SUCCESS;
#endif
}
//---------------------------------------------------------------------------
string CApplication::GetFileVersion(uint *numericVerion) const
{
#if USE_WISP
    //File version info
    wchar_t szFilename[MAX_PATH] = { 0 };

    if (GetModuleFileName(Hinstance, &szFilename[0], sizeof(szFilename)))
    {
        DWORD dummy = 0;
        DWORD dwSize = GetFileVersionInfoSize(&szFilename[0], &dummy);

        if (dwSize > 0)
        {
            UCHAR_LIST lpVersionInfo(dwSize, 0);

            if (GetFileVersionInfoW(&szFilename[0], NULL, dwSize, &lpVersionInfo[0]))
            {
                UINT uLen = 0;
                VS_FIXEDFILEINFO *lpFfi = NULL;

                VerQueryValueW(&lpVersionInfo[0], PATH_SEP, (LPVOID *)&lpFfi, &uLen);

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
                    *numericVerion = ((dwLeftMost & 0xFF) << 24) | ((dwSecondLeft & 0xFF) << 16) |
                                     ((dwSecondRight & 0xFF) << 8) | (dwRightMost & 0xFF);

                char fileVersion[100] = { 0 };
                sprintf_s(
                    fileVersion,
                    "%i.%i.%i.%i",
                    dwLeftMost,
                    dwSecondLeft,
                    dwSecondRight,
                    dwRightMost);

                return fileVersion;
            }
        }
    }

    return "unknown";
#else
    return " Linux";
#endif
}
//---------------------------------------------------------------------------
os_path CApplication::ExeFilePath(const char *str, ...) const
{
    WISPFUN_DEBUG("c1_f4");
    va_list arg;
    va_start(arg, str);

    char out[MAX_PATH] = { 0 };
    vsprintf_s(out, str, arg);

    va_end(arg);

    os_path res{ m_ExePath.c_str() };
    res.append(PATH_SEP);
    res.append(ToPath(out));
    return res;
}
//---------------------------------------------------------------------------
os_path CApplication::UOFilesPath(const string &str, ...) const
{
    return UOFilesPath(str.c_str());
}
//---------------------------------------------------------------------------
os_path CApplication::UOFilesPath(const char *str, ...) const
{
    WISPFUN_DEBUG("c1_f6");
    va_list arg;
    va_start(arg, str);

    char out[MAX_PATH] = { 0 };
    vsprintf_s(out, str, arg);

    va_end(arg);

    os_path res{ m_UOPath.c_str() };
    res.append(PATH_SEP);
    res.append(ToPath(out));
    return res;
}
//----------------------------------------------------------------------------------
}; // namespace WISP_APPLICATION
//----------------------------------------------------------------------------------
