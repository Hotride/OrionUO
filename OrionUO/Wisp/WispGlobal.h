//----------------------------------------------------------------------------------
#ifndef WISPGLOBAL_H
#define WISPGLOBAL_H

#if !defined(ORION_LINUX)
#if !defined(_CRT_SECURE_NO_WARNINGS)
#define _CRT_SECURE_NO_WARNINGS
#endif
#include <windows.h>
#endif
#include <stdio.h>

#define _USE_MATH_DEFINES
#include <math.h>
#include <thread>
#include <string>
using std::string;
using std::wstring;

#include <vector>
using std::vector;

#include <deque>
using std::deque;

#include <map>
using std::map;

#include <unordered_map>
using std::unordered_map;

using std::pair;

#include "WispDefinitions.h"
//----------------------------------------------------------------------------------
class CWispFunDebug
{
public:
	CWispFunDebug(const char *str);
	~CWispFunDebug();
};
//----------------------------------------------------------------------------------
extern std::thread::id g_MainThread;
extern deque<string> g_WispDebugFunStack;
//----------------------------------------------------------------------------------
typedef vector<uchar> UCHAR_LIST;
typedef vector<ushort> USHORT_LIST;
typedef vector<uint> UINT_LIST;
typedef vector<string> STRING_LIST;
typedef vector<wstring> WSTRING_LIST;
//----------------------------------------------------------------------------------
const int PACKET_VARIABLE_SIZE = 0;
//----------------------------------------------------------------------------------
int CalculatePercents(int max, int current, int maxValue);
//----------------------------------------------------------------------------------
string EncodeUTF8(const wstring &str);
wstring DecodeUTF8(const string &str);
string ToCamelCaseA(string str);
wstring ToCamelCaseW(wstring str);
#if defined(ORION_LINUX)
const string &ToString(const string &str);
#endif
string ToString(const wstring &wstr);
wstring ToWString(const string &str);
string Trim(const string &str);
int ToInt(const string &str);
string ToLowerA(string str);
string ToUpperA(string str);
wstring ToLowerW(wstring str);
wstring ToUpperW(wstring str);
bool Int32TryParse(string str, int &result);
bool ToBool(const string &str);
//----------------------------------------------------------------------------------
inline float deg2radf(float degr)
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
inline uint unpack32(puchar buf)
{
	return (buf[0] << 24) | (buf[1] << 16) | (buf[2] << 8) | buf[3];
}
//----------------------------------------------------------------------------------
inline ushort unpack16(puchar buf)
{
	return (buf[0] << 8) | buf[1];
}
//----------------------------------------------------------------------------------
inline void pack32(puchar buf, uint x)
{
	buf[0] = uchar(x >> 24);
	buf[1] = uchar((x >> 16) & 0xff);
	buf[2] = uchar((x >> 8) & 0xff);
	buf[3] = uchar(x & 0xff);
}
//----------------------------------------------------------------------------------
inline void pack16(puchar buf, ushort x)
{
	buf[0] = x >> 8;
	buf[1] = x & 0xff;
}
//----------------------------------------------------------------------------------
#endif

