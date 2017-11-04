// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
//----------------------------------------------------------------------------------
#include "stdafx.h"
//----------------------------------------------------------------------------------
DWORD g_MainThread = 0;
deque<string> g_WispDebugFunStack;
#if USE_WISP_DEBUG_FUNCTION_NAMES == 2
char *g_WispCurrentFunctionName = NULL;
#endif
//----------------------------------------------------------------------------------
CWispFunDebug::CWispFunDebug(const char *str)
{
	if (g_MainThread == GetCurrentThreadId())
		g_WispDebugFunStack.push_back(str);
}
//----------------------------------------------------------------------------------
CWispFunDebug::~CWispFunDebug()
{
	if (g_MainThread == GetCurrentThreadId())
		g_WispDebugFunStack.pop_back();
}
//----------------------------------------------------------------------------------
int CalculatePercents(int max, int current, int maxValue)
{
	if (max > 0)
	{
		max = (current * 100) / max;

		if (max > 100)
			max = 100;

		if (max > 1)
			max = (maxValue * max) / 100;
	}

	return max;
}
//----------------------------------------------------------------------------------
string EncodeUTF8(const wstring &wstr)
{
	int size = WideCharToMultiByte(CP_UTF8, 0, &wstr[0], (int)wstr.size(), NULL, 0, NULL, NULL);
	string result = "";

	if (size > 0)
	{
		result.resize(size + 1);
		WideCharToMultiByte(CP_UTF8, 0, &wstr[0], (int)wstr.size(), &result[0], size, NULL, NULL);
		result.resize(size); // result[size] = 0;
	}

	return result;
}
//----------------------------------------------------------------------------------
wstring DecodeUTF8(const string &str)
{
	int size = MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), NULL, 0);
	wstring result = L"";

	if (size > 0)
	{
		result.resize(size + 1);
		MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), &result[0], size);
		result.resize(size); // result[size] = 0;
	}

	return result;
}
//----------------------------------------------------------------------------------
string ToCamelCaseA(string str)
{
	int offset = 'a' - 'A';
	bool lastSpace = true;

	for (char &c : str)
	{
		if (c == ' ')
			lastSpace = true;
		else if (lastSpace)
		{
			lastSpace = false;

			if (c >= 'a' && c <= 'z')
				c -= offset;
		}
	}

	return str;
}
//----------------------------------------------------------------------------------
wstring ToCamelCaseW(wstring str)
{
	int offset = L'a' - L'A';
	bool lastSpace = true;

	for (wchar_t &c : str)
	{
		if (c == L' ')
			lastSpace = true;
		else if (lastSpace)
		{
			lastSpace = false;

			if (c >= L'a' && c <= L'z')
				c -= offset;
		}
	}

	return str;
}
//----------------------------------------------------------------------------------
string ToString(const wstring &wstr)
{
	string str = "";
	int size = wstr.length();
	int newSize = ::WideCharToMultiByte(GetACP(), 0, wstr.c_str(), size, NULL, 0, NULL, NULL);

	if (newSize > 0)
	{
		str.resize(newSize + 1);
		::WideCharToMultiByte(GetACP(), 0, wstr.c_str(), size, &str[0], newSize, NULL, NULL);
		str.resize(newSize); // str[newSize] = 0;
	}

	return str;
}
//----------------------------------------------------------------------------------
wstring ToWString(const string &str)
{
	int size = str.length();
	wstring wstr = L"";

	if (size > 0)
	{
		wstr.resize(size + 1);
		MultiByteToWideChar(GetACP(), 0, str.c_str(), size, &wstr[0], size);
		wstr.resize(size); // wstr[size] = 0;
	}

	return wstr;
}
//----------------------------------------------------------------------------------
string Trim(const string &str)
{
	string::const_iterator it = str.begin();
	while (it != str.end() && isspace(*it))
		it++;

	string::const_reverse_iterator rit = str.rbegin();
	while (rit.base() != it && isspace(*rit))
		rit++;

	return string(it, rit.base());
}
//----------------------------------------------------------------------------------
int ToInt(const string &str)
{
	return atoi(str.c_str());
}
//----------------------------------------------------------------------------------
string ToLowerA(string str)
{
	if (str.length())
		_strlwr(&str[0]);

	return str.c_str();
}
//----------------------------------------------------------------------------------
string ToUpperA(string str)
{
	if (str.length())
		_strupr(&str[0]);

	return str.c_str();
}
//----------------------------------------------------------------------------------
wstring ToLowerW(wstring str)
{
	if (str.length())
		_wcslwr(&str[0]);

	return str.c_str();
}
//----------------------------------------------------------------------------------
wstring ToUpperW(wstring str)
{
	if (str.length())
		_wcsupr(&str[0]);

	return str.c_str();
}
//----------------------------------------------------------------------------------
bool Int32TryParse(string str, int &result)
{
	std::istringstream  convert(str);
	try
	{
		convert >> result;
	}
	catch (int)
	{
		result = 0;
		return false;
	}
	if (!convert.eof())
	{
		result = 0;
		return false;
	}
	return true;
}
//----------------------------------------------------------------------------------
bool ToBool(const string &str)
{
	string data = ToLowerA(str);

	const int countOfTrue = 3;
	const string m_TrueValues[countOfTrue] = { "on", "yes", "true" };
	bool result = false;

	IFOR(i, 0, countOfTrue && !result)
		result = (data == m_TrueValues[i]);

	return result;
}
//----------------------------------------------------------------------------------
#if DEBUGGIND_OUTPUT == 1
void DebugMsg(const char *format, ...)
{
	va_list arg;
	va_start(arg, format);

	char buf[512] = { 0 };
	vsprintf_s(buf, format, arg);

	OutputDebugStringA(buf);

	va_end(arg);
}
//----------------------------------------------------------------------------------
void DebugMsg(const wchar_t *format, ...)
{
	va_list arg;
	va_start(arg, format);

	wchar_t buf[512] = { 0 };
	vswprintf_s(buf, format, arg);

	OutputDebugStringW(buf);

	va_end(arg);
}
//----------------------------------------------------------------------------------
void DebugDump(puchar data, const int &size)
{
	int num_lines = size / 16;

	if (size % 16 != 0)
		num_lines++;

	for (int line = 0; line < num_lines; line++)
	{
		int row = 0;

		DebugMsg("%04x: ", line * 16);

		for (row = 0; row < 16; row++)
		{
			if (line * 16 + row < size)
				DebugMsg("%02x ", data[line * 16 + row]);
			else
				DebugMsg("-- ");
		}

		DebugMsg(": ");

		char buf[17] = { 0 };

		for (row = 0; row < 16; row++)
		{
			if (line * 16 + row < size)
				buf[row] = (isprint(data[line * 16 + row]) ? data[line * 16 + row] : '.');
		}

		DebugMsg(buf);
		DebugMsg("\n");
	}
}
#endif
//----------------------------------------------------------------------------------
