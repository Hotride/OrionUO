#include "stdafx.h"

#include "FileSystem.h"
#include <SDL.h>

#if USE_WISP

int APIENTRY _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
{
    WISPFUN_DEBUG("c_main");
    INITLOGGER(L"uolog.txt");

    //ParseCommandLine(); // FIXME
    if (SDL_Init(SDL_INIT_TIMER) < 0)
    {
        SDL_LogError(
            SDL_LOG_CATEGORY_APPLICATION, "Unable to initialize SDL: %s\n", SDL_GetError());
        return EXIT_FAILURE;
    }

    auto path = g_App.ExeFilePath("crashlogs");
    fs_path_create(path);

    SYSTEMTIME st;
    GetLocalTime(&st);

    char buf[100] = { 0 };

    sprintf_s(
        buf,
        "/crash_%i_%i_%i___%i_%i_%i_%i.txt",
        st.wYear,
        st.wMonth,
        st.wDay,
        st.wHour,
        st.wMinute,
        st.wSecond,
        st.wMilliseconds);

    path += ToPath(buf);

    INITCRASHLOGGER(path);

    socket_init();
    g_OrionWindow.hInstance = hInstance;
    if (!g_OrionWindow.Create("Orion UO Client", "Ultima Online", true, 640, 480))
    {
        socket_shutdown();
        return 0;
    }

    g_OrionWindow.ShowWindow(true);
    g_OrionWindow.NoResize = true;

    g_Orion.LoadPluginConfig();

    auto r = g_App.Run(hInstance);
    socket_shutdown();
    SDL_Quit();
    return r;
}

#else

#if USE_ORIONDLL
ENCRYPTION_TYPE g_EncryptionType;
#endif

static bool g_isHeadless = false;
extern ENCRYPTION_TYPE g_EncryptionType;

extern COrionWindow g_OrionWindow;

int main(int argc, char **argv)
{
    WISPFUN_DEBUG();

    // TODO: good cli parsing api
    // keep this simple for now just for travis-ci
    for (int i = 0; i < argc; i++)
    {
        if (!strcmp(argv[i], "--headless"))
            g_isHeadless = true;
        else if (!strcmp(argv[i], "--nocrypt"))
            g_EncryptionType = ET_NOCRYPT;
    }

    if (SDL_Init(SDL_INIT_TIMER) < 0)
    {
        SDL_LogError(
            SDL_LOG_CATEGORY_APPLICATION, "Unable to initialize SDL: %s\n", SDL_GetError());
        return EXIT_FAILURE;
    }
    SDL_Log("SDL Initialized.");

    INITLOGGER("uolog.txt");
    auto path = g_App.ExeFilePath("crashlogs");
    fs_path_create(path);

    // FIXME: log stuff
    /*
	SYSTEMTIME st;
	GetLocalTime(&st);

	char buf[100] = { 0 };

	sprintf_s(buf, "\\crash_%i_%i_%i___%i_%i_%i_%i.txt", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);

	path += buf;

	INITCRASHLOGGER(path.c_str());
	*/

    if (!g_isHeadless)
    {
        if (!g_OrionWindow.Create("Orion UO Client", "Ultima Online", false, 640, 480))
        {
            SDL_LogWarn(
                SDL_LOG_CATEGORY_APPLICATION,
                "Failed to create OrionUO client window. Fallbacking to headless mode.\n");
            g_isHeadless = true;
        }
    }

    // FIXME: headless: lets end here so we can run on travis for now
    if (g_isHeadless)
        return EXIT_SUCCESS;

    g_Orion.LoadPluginConfig();

    auto ret = g_App.Run(nullptr);
    SDL_Quit();
    return ret;
}

#endif
