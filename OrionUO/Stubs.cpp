#include "Stubs.h"

#if !defined(ORION_WINDOWS)

#include <cassert>
#include <cwctype>
#include <errno.h>

int GetLastError() { return errno; }

// Bad and very ugly "API" stuff
void *GetForegroundWindow() { assert(false && __func__ ); return nullptr; }
void ShowWindow(void *, bool) { assert(false && __func__ ); }
void ShowCursor(bool) { assert(false && __func__ ); }
void SetWindowTextA(void *, const char *) { assert(false && __func__ ); }
void SetWindowTextW(void *, const wchar_t *) { assert(false && __func__ ); }
bool IsZoomed(void *) { assert(false && __func__ ); return false; }
int GetSystemMetrics(int) { assert(false && __func__ ); return 0; }
int DefWindowProc(void *, unsigned int, uintptr_t, uintptr_t) { assert(false && __func__ ); return 0; }
bool PtInRect(const RECT *, POINT) { assert(false && __func__ ); return false; }
int GetAsyncKeyState(int) { assert(false && __func__ ); return 0; }
void ReleaseCapture() { assert(false && __func__ ); }
void OutputDebugStringA(void *) { assert(false && __func__ ); }
void OutputDebugStringW(void *) { assert(false && __func__ ); }
int GetACP() { assert(false && __func__ ); return 0; }
int WideCharToMultiByte(int, int, const wchar_t*, int, char*, int, void*, void*) { assert(false && __func__ ); return 0; }
int MultiByteToWideChar(int, int, const char*, int, wchar_t*, int) { assert(false && __func__ ); return 0; }
void ScreenToClient(void *, POINT*) { assert(false && __func__ ); }
void GetCursorPos(POINT*) { assert(false && __func__ ); }
bool GetWindowRect(void*,RECT*) { assert(false && __func__ ); return false; }
int GetWindowLongA(void*,int) { assert(false && __func__ ); return 0; }
int RegisterClassEx(const WNDCLASSEX*) { assert(false && __func__ ); return 0; }
bool SetWindowPos(void *, void*, int, int, int, int, int) { assert(false && __func__ ); return false; }
bool IsIconic(void*) { assert(false && __func__ ); return false; }
bool GetClientRect(void*, RECT*) { assert(false && __func__ ); return false; }
bool AdjustWindowRectEx(RECT*, int, bool, int) { assert(false && __func__ ); return false; }
void* CreateWindowEx(int, const wchar_t*, const wchar_t*, int, int, int, int, int, void*, void*, void*, void*) { assert(false && __func__ ); return nullptr; }
bool VerQueryValue(void*, const wchar_t*,void*,unsigned*) { assert(false && __func__ ); return false; }
int GetModuleFileName(void*,const wchar_t*,int) { assert(false && __func__ ); return 0; }
int DispatchMessage(const MSG*) { assert(false && __func__ ); return 0; }
bool TranslateMessage(const MSG*) { assert(false && __func__ ); return false; }
bool PeekMessage(const MSG*, void*, int, int, int) { assert(false && __func__ ); return false; }
bool SendMessage(void*, int, int, int) { assert(false && __func__ ); return false; }
void PostMessage(void*, int, int, int) { assert(false && __func__ ); }
void* GetDC(void*) { assert(false && __func__ ); return nullptr; }
void PostQuitMessage(int) { assert(false && __func__ ); }
void* SetCapture(void*) { assert(false && __func__ ); return nullptr;}
bool OpenClipboard(void*)  { assert(false && __func__ ); return false;}
void* GetClipboardData(unsigned)  { assert(false && __func__ ); return nullptr;}
bool CloseClipboard()  { assert(false && __func__ ); return false;}
wchar_t* GetCommandLineW() { assert(false && __func__ ); return nullptr; }
const wchar_t** CommandLineToArgvW(wchar_t*,int*) { assert(false && __func__ ); return nullptr; }
void ExitProcess(unsigned) { assert(false && __func__ ); }
int GetSystemDefaultLangID() { assert(false && __func__ ); return 0; }
bool BringWindowToTop(void *) { assert(false && __func__ ); return false; }
void* LoadIcon(void*, int) { assert(false && __func__ ); return nullptr; }
void* ShellExecuteA(void*, const char*, const char*, const char*, const char*, int) { assert(false && __func__ ); return nullptr; }
void *LocalFree(void*p) { free(p); /*wtf*/ return nullptr;}
void *LoadCursor(void*, int){ assert(false && __func__ ); return nullptr; }
void* GlobalLock(void*){ assert(false && __func__ ); return nullptr; }
bool GlobalUnlock(void*){ assert(false && __func__ ); return false; }
int GetProfileStringA(const char*, const char*, const char*, char*, int){ assert(false && __func__ ); return 0; }

// Thread
void CloseHandle(void *) { assert(false && __func__ ); }
void KillTimer(void *, unsigned int) { assert(false && __func__ ); }
void SetTimer(void *, unsigned int, unsigned int, void *) { assert(false && __func__ ); }
int GetTickCount() { assert(false && __func__ ); return 0; }
int timeBeginPeriod(int) { assert(false && __func__ ); return 0; }
void EnterCriticalSection(void*) { assert(false && __func__ ); }
void LeaveCriticalSection(void*) { assert(false && __func__ ); }
void DeleteCriticalSection(void*) { assert(false && __func__ ); }
void InitializeCriticalSection(void*) { assert(false && __func__ ); }
void* _beginthreadex(void *, unsigned, unsigned (*)( void * ), void *, unsigned, unsigned *) { assert(false && __func__ ); return nullptr; }
void _endthreadex(int) { assert(false && __func__ ); }
int timeEndPeriod(int) { assert(false && __func__ ); return 0; }
void GetLocalTime(SYSTEMTIME*) { assert(false && __func__ ); }

// Socket
bool WSAStartup(int, void *) { assert(false && __func__ ); return false; }
void WSASetLastError(int) { assert(false && __func__ ); }
int WSACleanup(void) { assert(false && __func__ ); return 0; }
int recvfrom(int, const char*, int, int, const struct sockaddr*, int*) { assert(false && __func__ ); return 0; }

char* _strlwr(char* s)
{
    char* tmp = s;

    for (;*tmp;++tmp) {
        *tmp = tolower((unsigned char) *tmp);
    }

    return s;
}

char* _strupr(char* s)
{
    char* tmp = s;

    for (;*tmp;++tmp) {
        *tmp = toupper((unsigned char) *tmp);
    }

    return s;
}

wchar_t* _wcslwr(wchar_t* s)
{
    wchar_t* tmp = s;

    for (;*tmp;++tmp) {
        *tmp = towlower(*tmp);
    }

    return s;
}

wchar_t* _wcsupr(wchar_t* s)
{
    wchar_t* tmp = s;

    for (;*tmp;++tmp) {
        *tmp = towupper(*tmp);
    }

    return s;
}

#endif
