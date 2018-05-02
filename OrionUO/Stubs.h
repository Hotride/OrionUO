#pragma once

#if !defined(ORION_WINDOWS)

#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#include <chrono>
#include <thread>

#define NO_SDL_GLEXT
#include <GL/glew.h>
#include <GL/gl.h>
#include <SDL2/SDL.h>
#include <zlib.h>
#include <FreeImage.h>

using namespace std;

#if defined(__clang__)
// Enable these incrementally to cleanup bad code
#pragma clang diagnostic ignored "-Wint-to-pointer-cast" // FIXME: CGLTextTexture
#pragma clang diagnostic ignored                                                                   \
    "-Wtautological-constant-out-of-range-compare" // FIXME: always true expression
#pragma clang diagnostic ignored                                                                   \
    "-Winconsistent-missing-override" // FIXME: OnCharPress, OnKeyDown, OnLeftMouse... etc.
#pragma clang diagnostic ignored                                                                   \
    "-Woverloaded-virtual" // FIXME: CGameItem::GetLightID, CTextContainer::Add, CJournal::Add etc.
#pragma clang diagnostic ignored "-Wlogical-op-parentheses" // FIXME!!!!!!!!!!!!!!!!!!!!!!!!
#pragma clang diagnostic ignored "-Wnull-conversion"        // FIXME: NULL to bool m_CanProcessAlpha
#pragma clang diagnostic ignored "-Wnull-arithmetic"        // FIXME: comparing NULL to non-pointer
#pragma clang diagnostic ignored "-Wsign-compare"
#pragma clang diagnostic ignored "-Wshadow"  // FIXME: shadowing local variables
#pragma clang diagnostic ignored "-Wreorder" // FIXME: Initialization order in class fields
#pragma clang diagnostic ignored "-Wunused-variable"
#pragma clang diagnostic ignored "-Wmissing-variable-declarations"
#pragma clang diagnostic ignored "-Wmissing-braces"
#pragma clang diagnostic ignored "-Wnon-pod-varargs" // FIXME: glshader
#pragma clang diagnostic ignored "-Wswitch"
#pragma clang diagnostic ignored "-Wformat"               // %li
#pragma clang diagnostic ignored "-Wstring-plus-char"     // FIXME: PacketManager.cpp wtf
#pragma clang diagnostic ignored "-Wmultichar"            // FIXME: 'ENU'
#pragma clang diagnostic ignored "-Wchar-subscripts"      // FIXME: [' ']
#pragma clang diagnostic ignored "-Wc++11-narrowing"      // FIXME: ID_BGS_BUTTON_*
#pragma clang diagnostic ignored "-Wunused-private-field" // FIXME: m_FakeInsertionPin
#pragma clang diagnostic ignored "-Wcomment"
#elif defined(__GNUC__)
// GCC warnings
#pragma GCC diagnostic ignored "-Wunknown-pragmas"
#pragma GCC diagnostic ignored "-Wmultichar"
#pragma GCC diagnostic ignored "-Wsign-compare"
#pragma GCC diagnostic ignored "-Wmissing-declarations"
#pragma GCC diagnostic ignored "-Wpointer-arith"
#pragma GCC diagnostic ignored "-Wtype-limits"
#pragma GCC diagnostic ignored "-Wformat="
#pragma GCC diagnostic ignored "-Wunused-value"
#pragma GCC diagnostic ignored "-Wparentheses"
#pragma GCC diagnostic ignored "-Wint-to-pointer-cast"
#pragma GCC diagnostic ignored "-Wshadow"
#pragma GCC diagnostic ignored "-Wpragmas"
#pragma GCC diagnostic ignored "-Wcomment"
#pragma GCC diagnostic ignored "-Wunused-but-set-variable"
#pragma GCC diagnostic ignored "-Wstrict-overflow"
#pragma GCC diagnostic ignored "-Wformat-extra-args"
#pragma GCC diagnostic ignored "-Wconversion-null"
#pragma GCC diagnostic ignored "-Wmultichar"
#pragma GCC diagnostic ignored "-Wswitch"
#pragma GCC diagnostic ignored "-Waggressive-loop-optimizations"
#pragma GCC diagnostic ignored "-Wstrict-overflow"
#pragma GCC diagnostic ignored "-Wuninitialized"
#endif

typedef int SOCKET;
typedef uint16_t WORD;
typedef uintptr_t LPARAM;
typedef uintptr_t LRESULT;
typedef uintptr_t WPARAM;
typedef int32_t HRESULT;
typedef int32_t LONG;
typedef unsigned char BYTE;
typedef unsigned char *PBYTE;
typedef void *PVOID;
typedef void *LPVOID;
typedef void *HANDLE;
typedef void *HWND;
typedef void *HICON;
typedef void *HBRUSH;
typedef void *HCURSOR;
typedef void *HINSTANCE;
typedef void *HDC;
typedef void *HMODULE;
typedef void *HSTREAM;
typedef void *HGLRC;
typedef const void *LPCVOID;
typedef size_t SIZE_T;
typedef const char *LPCSTR;
typedef const wchar_t *LPWSTR;
typedef const char *LPTSTR;
typedef const char *LPSTR;
typedef wchar_t WCHAR;
typedef unsigned int UINT;

#define S_OK 0x0L
#define WM_USER 0x0400
#define WM_MOUSEWHEEL 1
#define WM_MBUTTONUP 2
#define WM_MBUTTONDOWN 3
#define WM_RBUTTONUP 4
#define WM_RBUTTONDOWN 5
#define WM_LBUTTONUP 6
#define WM_LBUTTONDOWN 7
#define WM_CLOSE 10
#define WM_XBUTTONDOWN 11
#define WM_SETTEXT 13
#define WM_SHOWWINDOW 14
#define WM_NCACTIVATE 15
#define WM_SYSKEYUP 16
#define WM_KEYUP 17
#define WM_SYSKEYDOWN 18
#define WM_KEYDOWN 19
#define WM_MOUSEMOVE 21
#define WM_SYSCOMMAND 25
#define SC_MAXIMIZE 0
#define SC_RESTORE 0

const unsigned int WM_NCPAINT = 0x85;
#define SM_CXSCREEN 0
#define SM_CYSCREEN 1

#define VK_RETURN 13
#define VK_ESCAPE 27
#define VK_TAB 9
#define VK_SHIFT 1
#define VK_CONTROL 2
#define VK_MENU 3
#define VK_RMENU 4
#define VK_LEFT 5
#define VK_RIGHT 6
#define VK_DOWN 7
#define VK_UP 8
#define VK_END 10
#define VK_HOME 11
#define VK_NEXT 12
#define VK_PRIOR 14
#define VK_F4 15
#define VK_DELETE 16
#define VK_F1 17
#define VK_F2 18
#define VK_F3 19
#define VK_F5 21
#define VK_F6 22
#define VK_F7 23
#define VK_F8 24
#define VK_F9 25
#define VK_F10 26
#define VK_F11 27
#define VK_F12 28
#define VK_CAPITAL 29
#define VK_PAUSE 30
#define VK_SPACE 31
#define VK_BACK 32
#define VK_SCROLL 33
#define VK_NUMPAD0 34
#define VK_NUMPAD1 35
#define VK_NUMPAD2 36
#define VK_NUMPAD3 37
#define VK_NUMPAD4 38
#define VK_NUMPAD5 39
#define VK_NUMPAD6 40
#define VK_NUMPAD7 41
#define VK_NUMPAD8 42
#define VK_NUMPAD9 43

#define PM_REMOVE 0x0001
#define MK_MBUTTON 0
#define MK_RBUTTON 1
#define MK_LBUTTON 2

#define GENERIC_READ 0
#define OPEN_EXISTING 0
#define FILE_ATTRIBUTE_NORMAL 0
#define FILE_MAP_READ 0
#define CP_UTF8 0
#define CALLBACK
#define HWND_TOP 0
#define GWL_STYLE 0
#define GWL_EXSTYLE 0
#define CS_HREDRAW 0
#define CS_OWNDC 0
#define CS_VREDRAW 0
#define WS_OVERLAPPEDWINDOW 0
#define WS_EX_WINDOWEDGE 0
#define SM_CYFRAME 0
#define SM_CYCAPTION 0
#define SM_CXSIZEFRAME 0
#define COLOR_WINDOW 0
#define SW_SHOWNORMAL 0
#define IDI_ORIONUO 0
#define IDC_CURSOR1 1
#define MAKEINTRESOURCE(x) x
#define CF_UNICODETEXT 0

#define LANG_RUSSIAN 0
#define LANG_FRENCH 1
#define LANG_GERMAN 2
#define LANG_SPANISH 3
#define LANG_JAPANESE 4
#define LANG_KOREAN 5

#define CF_TEXT 0

#define MAX_PATH 256

struct SYSTEMTIME
{
    WORD wYear;
    WORD wMonth;
    WORD wDayOfWeek;
    WORD wDay;
    WORD wHour;
    WORD wMinute;
    WORD wSecond;
    WORD wMilliseconds;
};
struct RECT
{
    int left;
    int top;
    int right;
    int bottom;
};

// Bad and very ugly "API" stuff
bool GetWindowRect(void *, RECT *);
bool SetWindowPos(void *, void *, int, int, int, int, int);
int GetSystemMetrics(int);
int DefWindowProc(void *, unsigned int, uintptr_t, uintptr_t);
bool SendMessage(void *, int, int, int);
void PostMessage(void *, int, int, int);
#define LOBYTE(x) (int)(x & 0xff)
int GetSystemDefaultLangID();
int GetProfileStringA(const char *, const char *, const char *, char *, int);
void *GlobalLock(void *);
bool GlobalUnlock(void *);

// cmd line
wchar_t *GetCommandLineW();
const wchar_t **CommandLineToArgvW(wchar_t *, int *);
void *ShellExecuteA(void *, const char *, const char *, const char *, const char *, int);
void *LocalFree(void *p);

// Input
bool OpenClipboard(void *);
void *GetClipboardData(unsigned);
bool CloseClipboard();

// Thread
void CloseHandle(void *); // WispThread.cpp
void KillTimer(void *, unsigned int);
void SetTimer(void *, unsigned int, unsigned int, void *);
int timeBeginPeriod(int);
void *_beginthreadex(void *, unsigned, unsigned (*)(void *), void *, unsigned, unsigned *);
void _endthreadex(int);
int timeEndPeriod(int);
void GetLocalTime(SYSTEMTIME *);

// Socket
struct WSADATA
{
    WORD wVersion;
    WORD wHighVersion;
    char szDescription[256];
    char szSystemStatus[256];
    unsigned short iMaxSockets;
    unsigned short iMaxUdpDg;
    char *lpVendorInfo;
};
bool WSAStartup(int, void *);
void WSASetLastError(int);
int WSACleanup(void);
int recvfrom(int, const char *, int, int, const struct sockaddr *, int *);
#define closesocket close
#define INVALID_SOCKET -1
#define SOCKET_ERROR -1
#define MAKEWORD(x, y) (int)(x)
typedef struct hostent HOSTENT;
typedef HOSTENT *LPHOSTENT;
#define SOCKADDR struct sockaddr
#define SOCKADDR_IN struct in_addr
#define LPIN_ADDR struct in_addr *
#define LPSOCKADDR const SOCKADDR *

// String
#define strncpy_s strncpy
#define lstrlenW wcslen
#define sprintf_s sprintf
#define sscanf_s sscanf
#define vsprintf_s vsprintf
#define vswprintf_s(a, b, c) vswprintf(a, 0, b, c)
int WideCharToMultiByte(int, int, const wchar_t *, int, char *, int, void *, void *);
int MultiByteToWideChar(int, int, const char *, int, wchar_t *, int);

// http://en.cppreference.com/w/cpp/locale/codecvt_utf8
inline int _wtoi(const wchar_t *a)
{
    return std::stoi(wstring(a));
}
char *_strlwr(char *s);
char *_strupr(char *s);
wchar_t *_wcslwr(wchar_t *s);
wchar_t *_wcsupr(wchar_t *s);

// BASS
#define BASS_OK 0
#define BASS_ERROR_FILEOPEN 1
#define BASS_ERROR_DRIVER 2
#define BASS_ERROR_BUFLOST 3
#define BASS_ERROR_HANDLE 4
#define BASS_ERROR_FORMAT 5
#define BASS_ERROR_POSITION 6
#define BASS_ERROR_INIT 7
#define BASS_ERROR_START 8
#define BASS_ERROR_SSL 9
#define BASS_ERROR_ALREADY 10
#define BASS_ERROR_NOCHAN 11
#define BASS_ERROR_ILLTYPE 12
#define BASS_ERROR_ILLPARAM 13
#define BASS_ERROR_NO3D 14
#define BASS_ERROR_NOEAX 15
#define BASS_ERROR_DEVICE 16
#define BASS_ERROR_NOPLAY 17
#define BASS_ERROR_UNKNOWN 18
#define BASS_ERROR_BUSY 19
#define BASS_ERROR_ENDED 19
#define BASS_ERROR_CODEC 19
#define BASS_ERROR_VERSION 19
#define BASS_ERROR_SPEAKER 19
#define BASS_ERROR_FILEFORM 19
#define BASS_ERROR_TIMEOUT 19
#define BASS_ERROR_DX 19
#define BASS_ERROR_DECODE 19
#define BASS_ERROR_NOTAVAIL 19
#define BASS_ERROR_NOFX 19
#define BASS_ERROR_CREATE 19
#define BASS_ERROR_NONET 19
#define BASS_ERROR_NOHW 19
#define BASS_ERROR_EMPTY 19
#define BASS_ERROR_NOTFILE 19
#define BASS_ERROR_FREQ 19
#define BASS_ERROR_MEM 19
#define BASS_ErrorGetCode() 0
#define BASS_SAMPLE_FLOAT 0
#define BASS_SAMPLE_3D 0
#define BASS_SAMPLE_SOFTWARE 0
#define BASS_ChannelPlay(x, y) false
#define BASS_StreamFree(x) false
#define BASS_ChannelIsActive(x) false
#define BASS_ChannelStop(x)
#define BASS_ATTRIB_VOL 0
#define BASS_MIDI_DECAYEND 0
#define BASS_SAMPLE_LOOP 0
#define MAXERRORLENGTH 64
#define BASS_GetVolume() 0.0f
#define BASS_Start()
#define BASS_Pause()
#define BASS_Free()
#define BASS_CONFIG_MIDI_DEFFONT 0
#define BASS_3DALG_FULL 0
#define BASS_CONFIG_3DALGORITHM 0
#define BASS_CONFIG_SRC 0
#define BASS_DEVICE_3D 0
#define BASS_Init(a, b, c, d, e) true
#define BASS_SetConfig(a, b) true
#define BASS_StreamCreateFile(a, b, c, d, e) nullptr
#define BASS_ChannelSetAttribute(a, b, c)
#define BASS_MIDI_StreamCreateFile(a, b, c, d, e, f) nullptr
#define BASS_SetConfigPtr(a, b) false
#define mciGetErrorString(a, b, c) false

#endif
