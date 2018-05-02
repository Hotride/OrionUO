#include "Stubs.h"

#if !defined(ORION_WINDOWS)

#include <cassert>
#include <cwctype>
#include <errno.h>

// Bad and very ugly "API" stuff
int GetSystemMetrics(int)
{
    NOT_IMPLEMENTED;
    return 0;
}
int DefWindowProc(void *, unsigned int, uintptr_t, uintptr_t)
{
    NOT_IMPLEMENTED;
    return 0;
}
int WideCharToMultiByte(int, int, const wchar_t *, int, char *, int, void *, void *)
{
    NOT_IMPLEMENTED;
    return 0;
}
int MultiByteToWideChar(int, int, const char *, int, wchar_t *, int)
{
    NOT_IMPLEMENTED;
    return 0;
}
bool GetWindowRect(void *, RECT *)
{
    NOT_IMPLEMENTED;
    return false;
}
bool SetWindowPos(void *, void *, int, int, int, int, int)
{
    NOT_IMPLEMENTED;
    return false;
}
bool AdjustWindowRectEx(RECT *, int, bool, int)
{
    NOT_IMPLEMENTED;
    return false;
}
bool SendMessage(void *, int, int, int)
{
    NOT_IMPLEMENTED;
    return false;
}
void PostMessage(void *, int, int, int)
{
    NOT_IMPLEMENTED;
}
bool OpenClipboard(void *)
{
    NOT_IMPLEMENTED;
    return false;
}
void *GetClipboardData(unsigned)
{
    NOT_IMPLEMENTED;
    return nullptr;
}
bool CloseClipboard()
{
    NOT_IMPLEMENTED;
    return false;
}
wchar_t *GetCommandLineW()
{
    NOT_IMPLEMENTED;
    return nullptr;
}
const wchar_t **CommandLineToArgvW(wchar_t *, int *)
{
    NOT_IMPLEMENTED;
    return nullptr;
}
int GetSystemDefaultLangID()
{
    NOT_IMPLEMENTED;
    return 0;
}
void *ShellExecuteA(void *, const char *, const char *, const char *, const char *, int)
{
    NOT_IMPLEMENTED;
    return nullptr;
}
void *LocalFree(void *p)
{
    free(p); /*wtf*/
    return nullptr;
}
void *GlobalLock(void *)
{
    NOT_IMPLEMENTED;
    return nullptr;
}
bool GlobalUnlock(void *)
{
    NOT_IMPLEMENTED;
    return false;
}
int GetProfileStringA(const char *, const char *, const char *, char *, int)
{
    NOT_IMPLEMENTED;
    return 0;
}

// Thread
void CloseHandle(void *)
{
    NOT_IMPLEMENTED;
}
void KillTimer(void *, unsigned int)
{
    NOT_IMPLEMENTED;
}
void SetTimer(void *, unsigned int, unsigned int, void *)
{
    NOT_IMPLEMENTED;
}
int timeBeginPeriod(int)
{
    NOT_IMPLEMENTED;
    return 0;
}
void *_beginthreadex(void *, unsigned, unsigned (*)(void *), void *, unsigned, unsigned *)
{
    NOT_IMPLEMENTED;
    return nullptr;
}
void _endthreadex(int)
{
    NOT_IMPLEMENTED;
}
int timeEndPeriod(int)
{
    NOT_IMPLEMENTED;
    return 0;
}
void GetLocalTime(SYSTEMTIME *)
{
    NOT_IMPLEMENTED;
}

// Socket
bool WSAStartup(int, void *)
{
    NOT_IMPLEMENTED;
    return false;
}
void WSASetLastError(int)
{
    NOT_IMPLEMENTED;
}
int WSACleanup(void)
{
    NOT_IMPLEMENTED;
    return 0;
}
int recvfrom(int, const char *, int, int, const struct sockaddr *, int *)
{
    NOT_IMPLEMENTED;
    return 0;
}

char *_strlwr(char *s)
{
    char *tmp = s;

    for (; *tmp; ++tmp)
    {
        *tmp = tolower((unsigned char)*tmp);
    }

    return s;
}

char *_strupr(char *s)
{
    char *tmp = s;

    for (; *tmp; ++tmp)
    {
        *tmp = toupper((unsigned char)*tmp);
    }

    return s;
}

wchar_t *_wcslwr(wchar_t *s)
{
    wchar_t *tmp = s;

    for (; *tmp; ++tmp)
    {
        *tmp = towlower(*tmp);
    }

    return s;
}

wchar_t *_wcsupr(wchar_t *s)
{
    wchar_t *tmp = s;

    for (; *tmp; ++tmp)
    {
        *tmp = towupper(*tmp);
    }

    return s;
}

#endif
