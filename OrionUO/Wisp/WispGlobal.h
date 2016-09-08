//----------------------------------------------------------------------------------
#ifndef WISPGLOBAL_H
#define WISPGLOBAL_H

#define _CRT_SECURE_NO_WARNINGS

#include <windows.h>
#include <stdio.h>

#define _USE_MATH_DEFINES
#include <math.h>

#include <string>
using std::string;
using std::wstring;

#include <vector>
using std::vector;

#include <deque>
using std::deque;

#include <map>
using std::map;

using std::pair;
//----------------------------------------------------------------------------------
const int PACKET_VARIABLE_SIZE = 0;
//----------------------------------------------------------------------------------
int CalculatePercents(int max, int current, int maxValue);
//----------------------------------------------------------------------------------
string EncodeUTF8(const wstring &str);
wstring DecodeUTF8(const string &str);
string ToString(const wstring &wstr);
wstring ToWString(const string &str);
string ToLowerA(string str);
string ToUpperA(string str);
wstring ToLowerW(wstring str);
wstring ToUpperW(wstring str);
bool ToBool(const string &str);
//----------------------------------------------------------------------------------
inline float deg2radf(const float &degr)
{
	return degr * (float)(M_PI / 180.0f);
}
//----------------------------------------------------------------------------------
inline float rad2degf(float radians)
{
	return (float)(radians * 180.0f / M_PI);
}
//----------------------------------------------------------------------------------
inline int RandomInt(int n)
{
	return (rand() % n);
}
//----------------------------------------------------------------------------------
inline int RandomIntMinMax(int n, int m)
{
	return (rand() % (m - n) + n);
}
//----------------------------------------------------------------------------------
inline DWORD unpack32(PBYTE buf)
{
	return (buf[0] << 24) | (buf[1] << 16) | (buf[2] << 8) | buf[3];
}
//----------------------------------------------------------------------------------
inline WORD unpack16(PBYTE buf)
{
	return (buf[0] << 8) | buf[1];
}
//----------------------------------------------------------------------------------
inline void pack32(PBYTE buf, DWORD x)
{
	buf[0] = BYTE(x >> 24);
	buf[1] = BYTE((x >> 16) & 0xff);
	buf[2] = BYTE((x >> 8) & 0xff);
	buf[3] = BYTE(x & 0xff);
}
//----------------------------------------------------------------------------------
inline void pack16(PBYTE buf, WORD x)
{
	buf[0] = x >> 8;
	buf[1] = x & 0xff;
}
//----------------------------------------------------------------------------------
#define WISP_ONE_NAME 1

#if WISP_ONE_NAME == 1
#define WISP_APPLICATION Wisp
#define WISP_WINDOW Wisp
#define WISP_MOUSE Wisp
#define WISP_GEOMETRY Wisp
#define WISP_THREAD Wisp
#define WISP_THREADED_TIMER Wisp
#define WISP_DATASTREAM Wisp
#define WISP_NETWORK Wisp
#define WISP_FILE Wisp
#define WISP_LOGGER Wisp
#else
#define WISP_APPLICATION WispApplication
#define WISP_WINDOW WispWindow
#define WISP_MOUSE WispMouse
#define WISP_GEOMETRY WispGeometry
#define WISP_THREAD WispThread
#define WISP_THREADED_TIMER WispThreadedTimer
#define WISP_DATASTREAM WispDataStream
#define WISP_NETWORK WispNetwork
#define WISP_FILE WispFile
#define WISP_LOGGER WispLogger
#endif
//----------------------------------------------------------------------------------
typedef unsigned char uchar;
typedef unsigned short ushort;
typedef unsigned int uint;
typedef unsigned __int64 uint64;
typedef uchar *puchar;
typedef ushort *pushort;
typedef uint *puint;
typedef uint64 *puint64;
typedef char *pchar;
typedef short *pshort;
typedef int *pint;
typedef __int64 *pint64;
//----------------------------------------------------------------------------------
typedef vector<uchar> UCHAR_LIST;
typedef vector<ushort> USHORT_LIST;
typedef vector<uint> UINT_LIST;
typedef vector<string> STRING_LIST;
//----------------------------------------------------------------------------------
#define DEBUGGIND_OUTPUT 1

#if DEBUGGIND_OUTPUT == 1
void DebugMsg(const char *format, ...);
void DebugMsg(const wchar_t *format, ...);
void DebugDump(puchar data, const int &size);
#else
#define DebugMsg(...)
#define DebugDump(buf, size)
#endif
//----------------------------------------------------------------------------------
#define RELEASE_POINTER(ptr) \
if (ptr != NULL) \
{ \
	delete ptr; \
	ptr = NULL; \
}
//----------------------------------------------------------------------------------
//!Incremented ordinary for
#define IFOR(var, start, stop) for (int var = start; var < stop; var ++)
//!Decremented ordinary for
#define DFOR(var, start, stop) for (int var = start; var >= stop; var --)
//----------------------------------------------------------------------------------
#define IN_RANGE(name, id1, id2) (name >= id1 && name <= id2)
#define OUT_RANGE(name, id1, id2) (name < id1 || name > id2)
//----------------------------------------------------------------------------------
//!Set/Get ordinary class property
#define SETGET(type, name) \
	protected: \
	type m_##name; \
	public: \
	inline void __fastcall Set##name(const type &val) { m_##name = val; } \
	inline type Get##name() const { return m_##name; } \
	__declspec(property(get = Get##name, put = Set##name)) type name
//----------------------------------------------------------------------------------
//!Set/Get ordinary class property with inc/dec/add functions
#define SETGETEX(type, name) \
	SETGET(type, name); \
	inline void Inc##name() { m_##name++; } \
	inline void Dec##name() { m_##name--; } \
	inline void Add##name(type val) { m_##name += val; }
//----------------------------------------------------------------------------------
//!Set/Get ordinary class property with event
#define SETGETE(type, name, event) \
	protected: \
	type m_##name; \
	public: \
	void event(const type &val); \
	inline void __fastcall Set##name(const type &val) { event(val); m_##name = val; } \
	inline type Get##name() const { return m_##name; } \
	__declspec(property(get = Get##name, put = Set##name)) type name
//----------------------------------------------------------------------------------
//!Set/Get ordinary class property with inc/dec/add functions
#define SETGETEXE(type, name, event) \
	SETGETE(type, name, event); \
	inline void Inc##name() { event(m_##name + 1); m_##name++; } \
	inline void Dec##name() { event(m_##name - 1); m_##name--; } \
	inline void Add##name(type val) { event(m_##name + val); m_##name += val; }
//----------------------------------------------------------------------------------
#endif

