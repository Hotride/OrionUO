#include "Stubs.h"
#include <cassert>
#include <cwctype>

// Bad and very ugly "API" stuff
void *GetForegroundWindow() { assert(false && __func__ ## ": Not Implemented"); return nullptr; }
void ShowWindow(void *, bool) { assert(false && __func__ ## ": Not Implemented"); }
void ShowCursor(bool) { assert(false && __func__ ## ": Not Implemented"); }
void SetWindowTextA(void *, const char *) { assert(false && __func__ ## ": Not Implemented"); }
void SetWindowTextW(void *, const wchar_t *) { assert(false && __func__ ## ": Not Implemented"); }
bool IsZoomed(void *) { assert(false && __func__ ## ": Not Implemented"); return false; }
void UpdateWindow(void *) { assert(false && __func__ ## ": Not Implemented"); }
int GetSystemMetrics(int) { assert(false && __func__ ## ": Not Implemented"); return 0; }
int DefWindowProc(void *, unsigned int, uintptr_t, uintptr_t) { assert(false && __func__ ## ": Not Implemented"); return 0; }
bool PtInRect(const RECT *, POINT) { assert(false && __func__ ## ": Not Implemented"); return false; }
int GetAsyncKeyState(int) { assert(false && __func__ ## ": Not Implemented"); return 0; }
void ReleaseCapture() { assert(false && __func__ ## ": Not Implemented"); }
int GetLastError() { assert(false && __func__ ## ": Not Implemented"); return 0; }
void OutputDebugStringA(void *) { assert(false && __func__ ## ": Not Implemented"); }
void OutputDebugStringW(void *) { assert(false && __func__ ## ": Not Implemented"); }
int GetACP() { assert(false && __func__ ## ": Not Implemented"); return 0; }
int WideCharToMultiByte(int, int, const wchar_t*, int, char*, int, void*, void*) { assert(false && __func__ ## ": Not Implemented"); return 0; }
int MultiByteToWideChar(int, int, const char*, int, wchar_t*, int) { assert(false && __func__ ## ": Not Implemented"); return 0; }
void ScreenToClient(void *, POINT*) { assert(false && __func__ ## ": Not Implemented"); }
void GetCursorPos(POINT*) { assert(false && __func__ ## ": Not Implemented"); }
bool GetWindowRect(void*,RECT*) { assert(false && __func__ ## ": Not Implemented"); return false; }
int GetWindowLongA(void*,int) { assert(false && __func__ ## ": Not Implemented"); return 0; }
int RegisterClassEx(const WNDCLASSEX*) { assert(false && __func__ ## ": Not Implemented"); return 0; }
bool SetWindowPos(void *, void*, int, int, int, int, int) { assert(false && __func__ ## ": Not Implemented"); return false; }
bool IsIconic(void*) { assert(false && __func__ ## ": Not Implemented"); return false; }
int MessageBoxA(void*, const char*, const char*, int) { assert(false && __func__ ## ": Not Implemented"); return 0; }
int MessageBoxW(void*, const wchar_t*, const wchar_t*, int) { assert(false && __func__ ## ": Not Implemented"); return 0; }
int MessageBox(void*, const wchar_t*, const wchar_t*, int) { assert(false && __func__ ## ": Not Implemented"); return 0; }
bool GetClientRect(void*, RECT*) { assert(false && __func__ ## ": Not Implemented"); return false; }
bool AdjustWindowRectEx(RECT*, int, bool, int) { assert(false && __func__ ## ": Not Implemented"); return false; }
void* CreateWindowEx(int, const wchar_t*, const wchar_t*, int, int, int, int, int, void*, void*, void*, void*) { assert(false && __func__ ## ": Not Implemented"); return nullptr; }
bool VerQueryValue(void*, const wchar_t*,void*,unsigned*) { assert(false && __func__ ## ": Not Implemented"); return false; }
int GetModuleFileName(void*,const wchar_t*,int) { assert(false && __func__ ## ": Not Implemented"); return 0; }
int DispatchMessage(const MSG*) { assert(false && __func__ ## ": Not Implemented"); return 0; }
bool TranslateMessage(const MSG*) { assert(false && __func__ ## ": Not Implemented"); return false; }
bool PeekMessage(const MSG*, void*, int, int, int) { assert(false && __func__ ## ": Not Implemented"); return false; }
bool SendMessage(void*, int, int, int) { assert(false && __func__ ## ": Not Implemented"); return false; }
void PostMessage(void*, int, int, int) { assert(false && __func__ ## ": Not Implemented"); }
void* GetDC(void*) { assert(false && __func__ ## ": Not Implemented"); return nullptr; }
void PostQuitMessage(int) { assert(false && __func__ ## ": Not Implemented"); }
void* SetCapture(void*) { assert(false && __func__ ## ": Not Implemented"); return nullptr;}
bool OpenClipboard(void*)  { assert(false && __func__ ## ": Not Implemented"); return false;}
void* GetClipboardData(unsigned)  { assert(false && __func__ ## ": Not Implemented"); return nullptr;}
bool CloseClipboard()  { assert(false && __func__ ## ": Not Implemented"); return false;}
wchar_t* GetCommandLineW() { assert(false && __func__ ## ": Not Implemented"); return nullptr; }
const wchar_t** CommandLineToArgvW(wchar_t*,int*) { assert(false && __func__ ## ": Not Implemented"); return nullptr; }
void ExitProcess(unsigned) { assert(false && __func__ ## ": Not Implemented"); }
int GetSystemDefaultLangID() { assert(false && __func__ ## ": Not Implemented"); return 0; }
bool BringWindowToTop(void *) { assert(false && __func__ ## ": Not Implemented"); return false; }
void* LoadIcon(void*, int) { assert(false && __func__ ## ": Not Implemented"); return nullptr; }
void* ShellExecuteA(void*, const char*, const char*, const char*, const char*, int) { assert(false && __func__ ## ": Not Implemented"); return nullptr; }
void *LocalFree(void*p) { free(p); /*wtf*/ return nullptr;}
void *LoadCursor(void*, int){ assert(false && __func__ ## ": Not Implemented"); return nullptr; }
void* GlobalLock(void*){ assert(false && __func__ ## ": Not Implemented"); return nullptr; }
bool GlobalUnlock(void*){ assert(false && __func__ ## ": Not Implemented"); return false; }
int GetProfileStringA(const char*, const char*, const char*, char*, int){ assert(false && __func__ ## ": Not Implemented"); return 0; }

// VM
void DebugBreak() { assert(false && __func__ ## ": Not Implemented"); }
size_t VirtualQueryEx(void*, void*, MEMORY_BASIC_INFORMATION*, size_t){ assert(false && __func__ ## ": Not Implemented"); return 0; }
void GetSystemInfo(SYSTEM_INFO*){ assert(false && __func__ ## ": Not Implemented"); }

// dll
void* LoadLibraryA(const char*) { assert(false && __func__ ## ": Not Implemented"); return nullptr; }
void FreeLibrary(void*) { assert(false && __func__ ## ": Not Implemented"); }
void* GetProcAddress(void*, const char*) { assert(false && __func__ ## ": Not Implemented"); return nullptr; }

// Exception
#define SetUnhandledExceptionFilter(x) assert(false && "Not Implemented");

// Filesystem
bool PathFileExistsW(const wchar_t *) { assert(false && __func__ ## ": Not Implemented"); return false; }
bool PathFileExistsA(const char *) { assert(false && __func__ ## ": Not Implemented"); return false; }
void* CreateFileMapping(void *, void *, int, int, void *, void *) { assert(false && __func__ ## ": Not Implemented"); return nullptr; }
int GetFileVersionInfoSize(void *, void *) { assert(false && __func__ ## ": Not Implemented"); return 0; }
int GetFileVersionInfo(void *, void *, int, void*) { assert(false && __func__ ## ": Not Implemented"); return 0; }
int GetFileSize(void *, void *) { assert(false && __func__ ## ": Not Implemented"); return 0; }
void GetCurrentDirectoryA(int, char *) { assert(false && __func__ ## ": Not Implemented"); }
void GetCurrentDirectoryW(int, wchar_t *) { assert(false && __func__ ## ": Not Implemented"); }
void CloseHandle(void *) { assert(false && __func__ ## ": Not Implemented"); }
void UnmapViewOfFile(void *) { assert(false && __func__ ## ": Not Implemented"); }
void* MapViewOfFile(void*, int, int, int, int) { assert(false && __func__ ## ": Not Implemented"); return 0; }
void* CreateFileA(const char *, int, int, void *, int, int, void *) { assert(false && __func__ ## ": Not Implemented"); return nullptr; }
void* CreateFileW(const wchar_t *, int, int, void *, int, int, void *) { assert(false && __func__ ## ": Not Implemented"); return nullptr; }
bool CreateDirectoryA(const char*, SECURITY_ATTRIBUTES*) { assert(false && __func__ ## ": Not Implemented"); return false; }

// Thread
void KillTimer(void *, unsigned int) { assert(false && __func__ ## ": Not Implemented"); }
void SetTimer(void *, unsigned int, unsigned int, void *) { assert(false && __func__ ## ": Not Implemented"); }
int GetTickCount() { assert(false && __func__ ## ": Not Implemented"); return 0; }
void Sleep(int) { assert(false && __func__ ## ": Not Implemented"); }
int timeBeginPeriod(int) { assert(false && __func__ ## ": Not Implemented"); return 0; }
int GetCurrentThreadId() { assert(false && __func__ ## ": Not Implemented"); return 0; }
void EnterCriticalSection(void*) { assert(false && __func__ ## ": Not Implemented"); }
void LeaveCriticalSection(void*) { assert(false && __func__ ## ": Not Implemented"); }
void DeleteCriticalSection(void*) { assert(false && __func__ ## ": Not Implemented"); }
void InitializeCriticalSection(void*) { assert(false && __func__ ## ": Not Implemented"); }
void* _beginthreadex(void *, unsigned, unsigned (*)( void * ), void *, unsigned, unsigned *) { assert(false && __func__ ## ": Not Implemented"); return nullptr; }
void _endthreadex(int) { assert(false && __func__ ## ": Not Implemented"); }
int timeGetTime() { assert(false && __func__ ## ": Not Implemented"); return 0; }
int timeEndPeriod(int) { assert(false && __func__ ## ": Not Implemented"); return 0; }
void GetLocalTime(SYSTEMTIME*) { assert(false && __func__ ## ": Not Implemented"); }

// Socket
bool WSAStartup(int, void *) { assert(false && __func__ ## ": Not Implemented"); return false; }
void WSASetLastError(int) { assert(false && __func__ ## ": Not Implemented"); }
int WSACleanup(void) { assert(false && __func__ ## ": Not Implemented"); return 0; }
int recvfrom(int, const char*, int, int, const struct sockaddr*, int*) { assert(false && __func__ ## ": Not Implemented"); return 0; }

// OpenGL
bool wglDeleteContext(void*)  { assert(false && __func__ ## ": Not Implemented"); return false; }
void* wglCreateContext(void*)  { assert(false && __func__ ## ": Not Implemented"); return nullptr; }
bool wglMakeCurrent(void*, void*) { assert(false && __func__ ## ": Not Implemented"); return false; }
void* wglGetProcAddress(const char*) { assert(false && __func__ ## ": Not Implemented"); return nullptr; }
void wglSwapIntervalEXT(int) { assert(false && __func__ ## ": Not Implemented"); }
int ChoosePixelFormat(void*, PIXELFORMATDESCRIPTOR*) { assert(false && __func__ ## ": Not Implemented"); return 0; }
bool SetPixelFormat(void*, int, PIXELFORMATDESCRIPTOR*) { assert(false && __func__ ## ": Not Implemented"); return false; }
void SwapBuffers(void*) { assert(false && __func__ ## ": Not Implemented"); }

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
